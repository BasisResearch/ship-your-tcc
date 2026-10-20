#!/usr/bin/env python3
"""Build/run the isolated native capture utility without writing dependencies.

No Lake invocation, source-project mutation, or semantic proof is performed.
Run the build and capture under the memory cap documented in capture-cut.md.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import struct
import subprocess
import sys

from gen_capture_cut import generate

ROOT = Path(__file__).resolve().parents[1]
LUA = Path.home() / "Documents/code/ship-your-lua"
TOOLCHAIN = Path.home() / ".elan/toolchains/leanprover--lean4---v4.34.0"
OUT = ROOT / "build/capture-cut"
LIMIT = 8_000_000


def sha(path):
    h = hashlib.sha256()
    with Path(path).open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def run(cmd, **kwargs):
    print(shlex.join(map(str, cmd)), file=sys.stderr, flush=True)
    subprocess.run(list(map(str, cmd)), check=True, **kwargs)


def inside_repo(path):
    path = path.resolve()
    if not path.is_relative_to(ROOT):
        raise ValueError(f"output must stay inside this checkout: {path}")
    return path


def build(args):
    out = inside_repo(args.build_dir)
    lua, tc = args.lua_root.resolve(), args.toolchain.resolve()
    response = lua / "riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator.rsp"
    # Reuse the emulator's exact existing link closure. Remove its main object,
    # and never ask Lake to repair or rebuild a dependency.
    tokens = shlex.split(response.read_text())
    mains = [t for t in tokens if Path(t).name == "Main.c.o.export"]
    if len(mains) != 1:
        raise ValueError("expected exactly one emulator Main.c.o.export")
    # Lua's copied .rsp can still name the historical vsa-iris-v2 checkout.
    # Rebase that recorded project prefix onto the existing Lua copy; no
    # symlinks, object generation, or writes in either dependency tree.
    marker = "/riscv-lean/lean_emulator/"
    if marker not in mains[0]:
        raise ValueError("cannot identify emulator response-file project root")
    old_root = mains[0].split(marker, 1)[0]
    dependencies = [t.replace(old_root + "/", str(lua) + "/")
                    for t in tokens if t != mains[0]]
    objects = [Path(t) for t in dependencies if t.endswith((".o", ".o.export"))]
    if not objects or any(not p.is_file() for p in objects):
        raise ValueError("prebuilt emulator link objects are missing; refusing dependency rebuild")
    lean_dirs = [lua / ".lake/build/lib/lean"]
    for p in objects:
        stem, sep, _ = str(p).partition("/.lake/build/ir/")
        if sep:
            lean_dirs.append(Path(stem) / ".lake/build/lib/lean")
    lean_dirs = list(dict.fromkeys(lean_dirs))
    elf_c = lua / ".lake/build/ir/Vsa/Elf.c"
    elf_olean = lua / ".lake/build/lib/lean/Vsa/Elf.olean"
    for p in [tc / "bin/lean", tc / "bin/leanc", tc / "bin/clang", elf_c, elf_olean]:
        if not p.is_file():
            raise ValueError(f"missing prebuilt input: {p}")
    version = subprocess.check_output([str(tc / "bin/lean"), "--version"], text=True).strip()
    if not version.startswith("Lean (version 4.34.0,") and not version.startswith("Lean 4.34.0,"):
        raise ValueError(f"expected Lean 4.34.0: {version}")
    out.mkdir(parents=True, exist_ok=True)
    source = out / "CaptureCut.lean"
    source.write_text(generate(lua))
    env = dict(os.environ, LEAN_PATH=os.pathsep.join(map(str, lean_dirs)))
    run([tc / "bin/lean", "--root", out, "-o", out / "CaptureCut.olean",
         "-c", out / "CaptureCut.c", source], env=env, cwd=ROOT)
    # Even this small imported module's native object is produced locally.
    # Its generated C and olean remain read-only in ship-your-lua.
    run([tc / "bin/leanc", "-O2", "-c", elf_c, "-o", out / "VsaElf.o"], cwd=ROOT)
    run([tc / "bin/leanc", "-O2", "-c", out / "CaptureCut.c", "-o", out / "CaptureCut.o"], cwd=ROOT)
    local_response = out / "dependencies.rsp"
    local_response.write_text("\n".join(json.dumps(t) for t in dependencies) + "\n")
    binary = out / "capture_cut"
    run([tc / "bin/clang", "-o", binary, out / "CaptureCut.o", out / "VsaElf.o",
         "@" + str(local_response)], cwd=ROOT)
    inputs = [elf_c, elf_olean, response, lua / "riscv-lean/Lean_RV64D_executable/LeanRV64DExecutable/Defs.lean"] + objects
    manifest = {"status": "native observation utility; not an execution certificate",
                "lean_version": version, "lua_root": str(lua),
                "response_project_prefix": old_root,
                "generator_sha256": sha(Path(__file__).with_name("gen_capture_cut.py")),
                "wrapper_sha256": sha(__file__), "generated_source_sha256": sha(source),
                "binary_sha256": sha(binary),
                "inputs": {str(p): sha(p) for p in inputs}}
    (out / "build.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(binary)


def symbols(nm, elf):
    rows = subprocess.check_output([str(nm), "--defined-only", str(elf)], text=True)
    syms = {}
    for row in rows.splitlines():
        parts = row.split()
        if len(parts) == 3:
            syms[parts[2]] = int(parts[0], 16)
    return syms


def elf_word(path, address):
    """Read an initialized uint64 from a mapped ELF64 little-endian segment."""
    raw = path.read_bytes()
    if raw[:6] != b"\x7fELF\x02\x01":
        raise ValueError("expected little-endian ELF64")
    phoff = struct.unpack_from("<Q", raw, 32)[0]
    entsize, count = struct.unpack_from("<HH", raw, 54)
    for i in range(count):
        off = phoff + i * entsize
        kind, _, fileoff, vaddr, _, filesz = struct.unpack_from("<IIQQQQ", raw, off)
        if kind == 1 and vaddr <= address and address + 8 <= vaddr + filesz:
            return struct.unpack_from("<Q", raw, fileoff + address - vaddr)[0]
    raise ValueError("embedded_flags is not backed by eight initialized ELF bytes")


def capture(args):
    out = inside_repo(args.out)
    binary = args.binary.resolve()
    elf, source = args.elf.resolve(), args.source.resolve()
    build_path = binary.parent / "build.json"
    build_manifest = json.loads(build_path.read_text())
    if sha(binary) != build_manifest["binary_sha256"]:
        raise ValueError("capture binary does not match build.json")
    syms = symbols(args.nm, elf)
    target = syms["tcc_baremetal_enter"]
    base, end = syms["__tccrun_start"], syms["__tccrun_end"]
    if (base, end) != (0x86400000, 0x86800000):
        raise ValueError("unexpected tcc arena extent")
    if elf_word(elf, syms["embedded_flags"]) != 0:
        raise ValueError("capture requires a FLAGS=0 ELF")
    elf_hash, source_hash = sha(elf), sha(source)
    out.mkdir(parents=True, exist_ok=False)
    # Preserve diagnostics if interrupted or unsuccessful. cut.json is emitted
    # only after successful completion and validation of every expected output.
    with (out / "native.stdout").open("wb") as stdout, (out / "native.stderr").open("wb") as stderr:
        run([binary, elf, target, base, end - base, out], stdout=stdout, stderr=stderr)
    if sha(elf) != elf_hash or sha(source) != source_hash:
        raise ValueError("source or ELF changed during capture")
    keys = ["pc", "steps", "tick", "cycles", "defined_registers", "memory_map_size", "arena_present_bytes"]
    values = list(map(int, (out / "cut.tsv").read_text().strip().split("\t")))
    if len(values) != len(keys):
        raise ValueError("invalid capture metadata")
    data = dict(zip(keys, values))
    if data["pc"] != target or not 0 <= data["steps"] <= LIMIT:
        raise ValueError("wrong cut or invalid step count")
    arena, presence = (out / "arena.bin").read_bytes(), (out / "arena.present.bin").read_bytes()
    if len(arena) != end - base or len(presence) != end - base:
        raise ValueError("incomplete arena capture")
    if any(v > 1 for v in presence) or sum(presence) != data["arena_present_bytes"]:
        raise ValueError("invalid presence map")
    if any(b for b, p in zip(arena, presence) if p == 0):
        raise ValueError("absent arena bytes must have total-read value zero")
    registers = (out / "registers.txt").read_text()
    if sum(line.startswith("⟨Register.") for line in registers.splitlines()) != data["defined_registers"]:
        raise ValueError("incomplete register dump")
    if (out / "console.txt").read_bytes() or (out / "console.repr.txt").read_text() != "#[]\n":
        raise ValueError("console is not empty at cut")
    data.update(status="native observation only; startup reachability remains open",
                boundary="first pre-instruction state at tcc_baremetal_enter",
                source=str(source), source_sha256=source_hash, elf=str(elf), elf_sha256=elf_hash,
                binary_sha256=sha(binary), build_manifest_sha256=sha(build_path),
                arena_base=base, arena_end=end, flags=0, step_limit=LIMIT,
                presence_encoding="one byte per arena address: 0 absent, 1 present",
                files={p.name: {"bytes": p.stat().st_size, "sha256": sha(p)}
                       for p in sorted(out.iterdir()) if p.is_file()})
    (out / "cut.json").write_text(json.dumps(data, indent=2) + "\n")
    print(json.dumps(data, indent=2))


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    sub = ap.add_subparsers(dest="command", required=True)
    b = sub.add_parser("build", help="compile helper against existing objects; never rebuild dependencies")
    b.add_argument("--lua-root", type=Path, default=LUA)
    b.add_argument("--toolchain", type=Path, default=TOOLCHAIN)
    b.add_argument("--build-dir", type=Path, default=OUT)
    c = sub.add_parser("capture", help="observe FLAGS=0 image, no startup certificate")
    c.add_argument("--elf", type=Path, required=True)
    c.add_argument("--source", type=Path, required=True)
    c.add_argument("--out", type=Path, required=True)
    c.add_argument("--binary", type=Path, default=OUT / "capture_cut")
    c.add_argument("--nm", type=Path, default=Path.home() / "toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin/riscv-none-elf-nm")
    args = ap.parse_args()
    try:
        (build if args.command == "build" else capture)(args)
    except (OSError, ValueError, KeyError, subprocess.CalledProcessError) as error:
        print(f"capture-cut: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
