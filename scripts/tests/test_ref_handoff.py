#!/usr/bin/env python3
"""Handoff regressions; --image also runs the real C11 tcc image/harness.

No Lean or Sail runs; emulator invocations are serial. Temporary ELF fixtures
do not change the pinned sum compile-cut artifacts.
"""
import argparse
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
BASE = 0x80000000


def fixture(path, ending="exit", omit=None, bad_bounds=False, pretrap=False):
    """Tiny RV64 ELF with distinct run/enter markers and a writable arena."""
    # auipc/addi pairs set a0 (entry), a4 (code), t0 (tohost).
    words = [0x00000517, 0x30450513, 0x00000717, 0x30870713,
             0x00400793, 0x00000297, 0x3ec28293, 0x00f00313]
    run = BASE + 4 * len(words)
    words += [0x00000713]  # clobber a4: metadata must be saved at run
    enter = BASE + 4 * len(words)
    words += {"exit": [0x0062b023], "loop": [0x0000006f], "trap": [0]}[ending]
    if pretrap:
        words[0] = 0
    mem = bytearray(0x500)
    struct.pack_into("<" + "I" * len(words), mem, 0, *words)
    mem[0x300:0x320] = bytes(range(32))
    names = bytearray(b"\0")
    symbols = bytearray(24)
    for name, value in {
        "tohost": BASE + 0x400, "tcc_baremetal_run": run,
        "tcc_baremetal_enter": enter, "__tccrun_start": BASE + 0x300,
        "__tccrun_end": BASE + (0x200 if bad_bounds else 0x380),
    }.items():
        if name == omit:
            continue
        off = len(names)
        names += name.encode() + b"\0"
        symbols += struct.pack("<IBBHQQ", off, 0x10, 0, 1, value, 0)
    data_off = 0x100
    sym_off = data_off + len(mem)
    str_off = sym_off + len(symbols)
    sh_off = (str_off + len(names) + 7) & ~7
    hdr = struct.pack("<16sHHIQQQIHHHHHH", b"\x7fELF\x02\x01\x01" + bytes(9),
                      2, 243, 1, BASE, 64, sh_off, 0, 64, 56, 1, 64, 3, 0)
    ph = struct.pack("<IIQQQQQQ", 1, 7, data_off, BASE, BASE, len(mem), len(mem), 4)
    blob = bytearray(hdr + ph)
    blob += bytes(data_off - len(blob)) + mem + symbols + names
    blob += bytes(sh_off - len(blob)) + bytes(64)
    blob += struct.pack("<IIQQQQIIQQ", 0, 2, 0, 0, sym_off, len(symbols), 2, 1, 8, 24)
    blob += struct.pack("<IIQQQQIIQQ", 0, 3, 0, 0, str_off, len(names), 0, 0, 1, 0)
    path.write_bytes(blob)
    return (run - BASE) // 4, (enter - BASE) // 4


def invoke(cmd, code, **kwargs):
    r = subprocess.run([str(x) for x in cmd], capture_output=True, timeout=120, **kwargs)
    assert r.returncode == code, (cmd, r.returncode, r.stdout, r.stderr)
    return r


def arena(path):
    data = path.read_bytes()
    fields = data.split(b"\n", 5)
    assert len(fields) == 6 and fields[0] == b"TCCARENA 1", fields[:5]
    assert len(fields[5]) == int(fields[4].split()[1], 16)
    return data


def emulator_checks(work, emu):
    for ending, rc in [("exit", 7), ("loop", 124), ("trap", 126)]:
        elf = work / (ending + ".elf")
        run, enter = fixture(elf, ending)
        full, cut = work / "full.arena", work / "cut.arena"
        r = invoke([emu, elf, "--arena-out", full, "--max-steps", 100], rc)
        assert re.search(rb"compile_steps=8 exit=\d+ status=(exit|limit|trap)", r.stderr)
        r = invoke([emu, elf, "--arena-out", cut, "--stop-at-handoff"], 0)
        assert f"steps={enter} compile_steps={run} exit=- status=handoff\n".encode() in r.stderr
        assert arena(full) == arena(cut)
        print(f"fixture {ending}: handoff at {enter}; legacy rc={rc}; arenas equal")
    for limit in [run - 1, enter]:
        out = work / "limited.arena"
        r = invoke([emu, elf, "--arena-out", out, "--stop-at-handoff", "--max-steps", limit], 124)
        assert not out.exists() and b"status=handoff" not in r.stderr
    for options, rc in [({"pretrap": True}, 126), ({"omit": "tcc_baremetal_enter"}, 2),
                        ({"omit": "tcc_baremetal_run"}, 2), ({"bad_bounds": True}, 2)]:
        fixture(elf, **options)
        out = work / "failed.arena"
        r = invoke([emu, elf, "--arena-out", out, "--stop-at-handoff"], rc)
        assert not out.exists() and b"status=handoff" not in r.stderr
    fixture(elf)
    for out in [work / "missing" / "out.arena", Path("/dev/full")]:
        r = invoke([emu, elf, "--arena-out", out, "--stop-at-handoff"], 2)
        assert b"status=handoff" not in r.stderr
        if out == Path("/dev/full"):
            assert b"arena write failed" in r.stderr  # buffered fclose failure
    print("fixture errors: pre-handoff limits/trap, missing markers, arena bounds/open/write all fail")


def image_checks(work, emu):
    env = dict(os.environ, TCC_LIBS=" ", MAX_STEPS="400000000", TCC_ARGS='"-w",',
               EXTRA_FILES="", ELF_OUT="")
    for name, source, legacy in [
        ("terminating", 'int main(void) { return 7; }\n', 7),
        ("infinite", 'int main(void) { for (;;) {} }\n', 124),
        # Deliberate execution fault, not a UB-free Supported witness.
        ("trapping", 'int main(void) { return *(volatile int *)0; }\n', 126),
    ]:
        src, elf = work / (name + ".c"), work / (name + ".elf")
        src.write_text(source)
        cut, full = work / (name + ".cut"), work / (name + ".full")
        r = invoke([ROOT / "scripts/ref_arena.sh", src, cut], 0,
                   env=dict(env, ELF_OUT=str(elf)))
        m = re.search(rb"steps=(\d+) compile_steps=(\d+) exit=- status=handoff", r.stderr)
        assert m, r.stderr
        steps = int(m[1])
        legacy_steps = steps + 100000
        old = invoke([emu, elf, "--arena-out", full, "--max-steps", legacy_steps], legacy)
        assert arena(cut) == arena(full)
        print(f"C11 {name}: {r.stderr.decode().strip()}; legacy limit={legacy_steps}: {old.stderr.decode().strip()}")
        r = invoke([ROOT / "scripts/ref_arena.sh", src, cut], 124,
                   env=dict(env, MAX_STEPS=str(steps)))
        assert not cut.exists(), "stale arena accepted after a limit"
    bad = work / "compile-error.c"
    bad.write_text("int main( {\n")
    r = invoke([ROOT / "scripts/ref_arena.sh", bad, work / "bad.arena"], 125, env=env)
    assert b"compile_steps=-" in r.stderr and b"status=exit" in r.stderr
    math = work / "libm.c"
    math.write_text("extern double sin(double); int main(void) { return sin(0.0) != 0.0; }\n")
    invoke([ROOT / "scripts/ref_arena.sh", math, work / "libm-failed.arena"], 125, env=env)
    retry_env = dict(env)
    retry_env.pop("TCC_LIBS")
    r = invoke([ROOT / "scripts/ref_arena.sh", math, work / "libm.arena"], 0, env=retry_env)
    arena(work / "libm.arena")
    assert b"exit=- status=handoff" in r.stderr
    print("harness: unresolved libm reference refused without retry; automatic -lm retry reaches handoff")

    # Real shell harness/linker with a controlled emulator in a temporary root.
    fake = work / "harness"
    (fake / "scripts").mkdir(parents=True)
    (fake / "build").mkdir()
    (fake / "c").symlink_to(ROOT / "c", target_is_directory=True)
    shutil.copy(ROOT / "scripts/ref_arena.sh", fake / "scripts/ref_arena.sh")
    shutil.copy(ROOT / "scripts/rv64emu.c", fake / "scripts/rv64emu.c")
    stub = fake / "build/rv64emu"
    stub.write_text('''#!/bin/bash
while [ "$#" -gt 0 ]; do
  if [ "$1" = --arena-out ]; then shift; out=$1; fi
  shift
done
printf 'partial arena' > "$out"
case "$STUB_MODE" in
  limit) echo 'rv64emu: steps=10 compile_steps=8 exit=124 status=limit' >&2; exit 124 ;;
  trap) echo 'rv64emu: steps=10 compile_steps=8 exit=126 status=trap' >&2; exit 126 ;;
  missing) exit 0 ;;
  legacy) echo 'rv64emu: steps=10 compile_steps=8 exit=0' >&2; exit 0 ;;
  wrongrc) echo 'rv64emu: steps=10 compile_steps=8 exit=- status=handoff' >&2; exit 1 ;;
  duplicate) for i in 1 2; do echo 'rv64emu: steps=10 compile_steps=8 exit=- status=handoff' >&2; done ;;
  noarena) rm "$out"; echo 'rv64emu: steps=10 compile_steps=8 exit=- status=handoff' >&2 ;;
  partial) echo 'rv64emu: steps=10 compile_steps=8 exit=- status=handoff' >&2 ;;
  pretrap) rm "$out"; echo 'rv64emu: access fault' >&2; exit 126 ;;
esac
''')
    stub.chmod(0o755)
    for mode, rc in [("limit", 124), ("trap", 126), ("missing", 2), ("legacy", 2),
                     ("wrongrc", 2), ("duplicate", 2), ("noarena", 2), ("partial", 2), ("pretrap", 126)]:
        out = work / "stale.arena"
        out.write_text("stale arena")
        invoke([fake / "scripts/ref_arena.sh", src, out], rc, env=dict(env, STUB_MODE=mode))
        assert not out.exists(), mode
    print("harness: compile refusal and 9 incomplete/failure records classified correctly; stale arenas removed")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--image", action="store_true", help="build/run real tcc images and shell harness checks")
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix="ref-handoff-test-") as tmp:
        work = Path(tmp)
        emu = work / "rv64emu"
        invoke(["gcc", "-std=c11", "-O2", "-Wall", "-Wextra", "-Werror", "-o", emu,
                ROOT / "scripts/rv64emu.c"], 0)
        emulator_checks(work, emu)
        if args.image:
            image_checks(work, emu)
    print("ref handoff checks: PASS")


if __name__ == "__main__":
    main()

