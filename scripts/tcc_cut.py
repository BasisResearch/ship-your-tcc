#!/usr/bin/env python3
"""Capture the cut of `tcc -run P` at the jump into the generated code.

    scripts/tcc_cut.py --elf build/p3/sum.elf --out build/p3/sum-cut

Runs the executable Sail model's emulator with `--trace-all` on a FLAGS=0 tcc
image (c/Makefile) and streams its trace (LeanRiscv.traceLoop: one row per
step with the registers before the step and the 8 bytes at a load/store's
address before and after it). Nothing is stored but what the cut needs:

* the compile phase (step 0 up to the first step at `tcc_baremetal_enter`):
  the memory is the ELF's loaded image (the emulator's `initializeMemory`:
  every program header's file bytes at its virtual address) overlaid with
  every store. Every load's observed value is checked against this
  reconstruction, so the reconstruction is the model's memory on every byte
  the run observes. The bytes the compile phase reads before writing them are
  counted (`compile_reads`), not kept.
* the cut: the step number, the 31 GPRs, the console length (must be 0);
* the program phase (from the cut to the `tohost` store in `_exit`,
  inclusive): every byte fetched or loaded before the program phase writes
  it, with its value at the cut (the view), and the phase's rows
  (`program.tsv`) for the segment generators. Every byte of the view must be
  present at the cut (loaded from the ELF or stored by the compile phase).

Outputs in --out (must not exist unless --force): cut.json, view.tsv
(addr, byte, source: elf|store), program.tsv (the raw rows), console.txt,
compile_reads.tsv (the bytes the compile phase reads before writing them).
gen_tcc_cut.py turns them into Lean.
"""
import argparse
import hashlib
import json
import os
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
XB = Path.home() / 'toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin'
EMU = Path.home() / 'Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator'
# everything but .embed (c/tests/run.sh runtime_sha): the image after the errno fix
RUNTIME_SHA = '9e1142dc96d8db10'
# tcc's static code arena (c/src/link.ld `__tccrun`)
ARENA, ARENA_SIZE = 0x86400000, 4 << 20


def tool(name):
    return str(XB / f'riscv-none-elf-{name}')


def runtime_sha(elf):
    """c/tests/run.sh's runtime_sha: allocated sections but .embed, and their bytes."""
    tmp = str(elf) + '.rt.bin'
    subprocess.run([tool('objcopy'), '-O', 'binary', '-R', '.embed', str(elf), tmp], check=True)
    sections = subprocess.run([tool('readelf'), '-SW', str(elf)], check=True,
                              capture_output=True, text=True).stdout
    rows = []
    for line in sections.splitlines():
        line = line.strip()
        if not line.startswith('['):
            continue
        rest = line.split(']', 1)[1].split()
        # name type addr off size es flg ...
        if len(rest) >= 7 and 'A' in rest[6] and rest[0] != '.embed':
            rows.append(' '.join([rest[0], rest[1], rest[2], rest[4], rest[6]]))
    h = hashlib.sha256(('\n'.join(rows) + '\n').encode() + Path(tmp).read_bytes())
    os.unlink(tmp)
    return h.hexdigest()[:16]


def symbols(elf):
    out = subprocess.run([tool('nm'), str(elf)], check=True, capture_output=True, text=True).stdout
    syms = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) == 3:
            syms.setdefault(p[2], int(p[0], 16))
    return syms


def exit_store(elf):
    out = subprocess.run([tool('objdump'), '-d', '--disassemble=_exit', str(elf)], check=True,
                         capture_output=True, text=True).stdout
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 3 and p[2] == 'sd':
            return int(p[0].rstrip(':'), 16)
    raise SystemExit('tcc_cut: no sd in _exit')


def elf_image(elf):
    """The emulator's initializeMemory on the RAM range: each program header's
    file bytes at p_vaddr (bss is NOT present until stored)."""
    data = Path(elf).read_bytes()
    assert data[:4] == b'\x7fELF' and data[4] == 2 and data[5] == 1
    phoff, = struct.unpack_from('<Q', data, 0x20)
    phentsize, phnum = struct.unpack_from('<HH', data, 0x36)
    segs = []
    for i in range(phnum):
        (ptype, flags, off, vaddr, paddr, filesz, memsz,
         align) = struct.unpack_from('<IIQQQQQQ', data, phoff + i * phentsize)
        if filesz and vaddr >= 0x80000000:
            segs.append((vaddr, data[off:off + filesz]))
    return segs


class Image:
    def __init__(self, segs):
        self.segs = segs

    def get(self, a):
        for base, body in self.segs:
            if base <= a < base + len(body):
                return body[a - base]
        return None


def le_bytes(word_hex, n):
    w = int(word_hex, 16)
    return [(w >> (8 * j)) & 0xff for j in range(n)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--elf', required=True)
    ap.add_argument('--out', required=True)
    ap.add_argument('--emu', default=str(EMU))
    ap.add_argument('--max-steps', type=int, default=20_000_000)
    ap.add_argument('--force', action='store_true')
    ap.add_argument('--arena', action='store_true',
                    help='also write arena.tsv: every byte of the 4 MiB code arena '
                         '(0x86400000) the compile phase stored, at the cut')
    a = ap.parse_args()
    elf = Path(a.elf).resolve()
    out = Path(a.out)
    if out.exists() and not a.force:
        raise SystemExit(f'tcc_cut: {out} exists')
    out.mkdir(parents=True, exist_ok=True)

    rt = runtime_sha(elf)
    if rt != RUNTIME_SHA:
        raise SystemExit(f'tcc_cut: runtime sha {rt} != pinned {RUNTIME_SHA}')
    syms = symbols(elf)
    enter = syms['tcc_baremetal_enter']
    tohost = syms['tohost']
    exitsd = exit_store(elf)
    segs = elf_image(elf)
    img = Image(segs)
    fl = syms['embedded_flags']
    flags = int.from_bytes(bytes(img.get(fl + j) for j in range(8)), 'little')
    if flags != 0:
        raise SystemExit(f'tcc_cut: embedded_flags = {flags}, need a FLAGS=0 image')

    written = {}          # addr -> byte, every store so far
    compile_reads = set()  # read before written in the compile phase
    view = {}             # program phase: addr -> (byte, source)
    pwritten = set()      # written in the program phase
    cut = None
    console = bytearray()
    prog_rows = []
    nrows = 0
    end_step = None

    def cur(addr):
        if addr in written:
            return written[addr], 'store'
        b = img.get(addr)
        return (b, 'elf') if b is not None else (None, 'absent')

    def read_prog(addr, observed):
        if addr in pwritten or addr in view:
            return
        b, src = cur(addr)
        if b is None:
            raise SystemExit(f'tcc_cut: program phase reads absent byte {addr:#x}')
        if observed is not None and observed != b:
            raise SystemExit(f'tcc_cut: reconstruction {b:#x} != observed {observed:#x} at {addr:#x}')
        view[addr] = (b, src)

    proc = subprocess.Popen([a.emu, str(elf), '--trace-all', '--max-steps', str(a.max_steps)],
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=1 << 20)
    for raw in proc.stderr:
        if not raw.startswith(b'T\t'):
            if raw.startswith(b'TRACE-FUEL-OUT'):
                raise SystemExit('tcc_cut: step budget exhausted')
            continue
        nrows += 1
        f = raw.decode().rstrip('\n').split('\t')
        step = int(f[1])
        pc = int(f[2], 16)
        if cut is None and pc == enter:
            if a.arena:
                with open(out / 'arena.tsv', 'w') as fh:
                    fh.write('# addr\tbyte: the arena bytes stored by the compile phase, at the cut\n')
                    for addr in sorted(x for x in written if ARENA <= x < ARENA + ARENA_SIZE):
                        fh.write(f'{addr:x}\t{written[addr]:02x}\n')
            regs = [int(x, 16) for x in f[4:35]]
            oi = f.index('O')
            cut = {'step': step, 'pc': pc, 'regs': regs, 'console_chunks': int(f[oi + 1])}
        mem = None
        if len(f) > 35 and f[35] != 'O':
            mem = (f[35][0], int(f[35][1:]), int(f[36], 16), f[37], f[38])
        if cut is None:
            # compile phase
            for j in range(4):
                if pc + j not in written:
                    compile_reads.add(pc + j)
            if mem:
                kind, w, addr, pre, post = mem
                if kind == 'L':
                    for j, b in enumerate(le_bytes(pre, w)):
                        cb, _ = cur(addr + j)
                        if (cb if cb is not None else 0) != b:
                            raise SystemExit(f'tcc_cut: compile load mismatch at {addr + j:#x} step {step}')
                        if addr + j not in written:
                            compile_reads.add(addr + j)
                else:
                    for j, b in enumerate(le_bytes(post, w)):
                        written[addr + j] = b
            continue
        # program phase
        prog_rows.append(raw.decode())
        for j in range(4):
            read_prog(pc + j, None)
        if mem:
            kind, w, addr, pre, post = mem
            if kind == 'L':
                for j, b in enumerate(le_bytes(pre, w)):
                    read_prog(addr + j, b)
            else:
                for j, b in enumerate(le_bytes(post, w)):
                    pwritten.add(addr + j)
        oi = f.index('O')
        if f[oi + 3] != '256':
            console.append(int(f[oi + 3]))
        elif f[oi + 1] != f[oi + 2]:
            raise SystemExit(f'tcc_cut: unexpected console change at step {step}')
        if pc == exitsd:
            end_step = step
            break
    proc.kill()
    proc.wait()
    if cut is None or end_step is None:
        raise SystemExit('tcc_cut: did not reach the cut and the exit store')
    if cut['console_chunks'] != 0:
        raise SystemExit('tcc_cut: console not empty at the cut')

    with open(out / 'view.tsv', 'w') as fh:
        fh.write('# addr\tbyte\tsource\n')
        for addr in sorted(view):
            b, src = view[addr]
            fh.write(f'{addr:x}\t{b:02x}\t{src}\n')
    (out / 'program.tsv').write_text(''.join(prog_rows))
    with open(out / 'compile_reads.tsv', 'w') as fh:
        fh.write('# addr\tbyte (the ELF image byte, 00 when absent)\n')
        for addr in sorted(compile_reads):
            b = img.get(addr)
            fh.write(f'{addr:x}\t{(b or 0):02x}\n')
    (out / 'console.txt').write_bytes(bytes(console))
    meta = {
        'elf': str(elf.relative_to(ROOT)) if elf.is_relative_to(ROOT) else str(elf),
        'elf_sha256': hashlib.sha256(elf.read_bytes()).hexdigest(),
        'runtime_sha': rt,
        'enter': enter, 'tohost': tohost, 'exit_store': exitsd,
        'cut_step': cut['step'], 'regs': cut['regs'],
        'end_step': end_step, 'program_steps': end_step - cut['step'] + 1,
        'view_bytes': len(view),
        'view_from_store': sum(1 for v in view.values() if v[1] == 'store'),
        'compile_written_bytes': len(written),
        'compile_reads_bytes': len(compile_reads),
        'console': console.decode('latin-1'),
        'segments': [[base, len(body)] for base, body in segs],
        'status': 'observation of the emulator; no execution claim',
    }
    (out / 'cut.json').write_text(json.dumps(meta, indent=1) + '\n')
    print(json.dumps({k: v for k, v in meta.items() if k not in ('regs', 'segments')}, indent=1))


if __name__ == '__main__':
    main()
