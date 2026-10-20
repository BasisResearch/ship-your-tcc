#!/usr/bin/env python3
"""Generate the printf layout of one `tcc -run` image (Tcc/Printf/Sum/*.lean).

    scripts/gen_printf_layout.py [--elf build/p3/sum.elf] \
        [--arena results/p3/sum-arena.tsv] [--replay results/p3/sum-replay.json] [--check]

Writes (never edit them: change this generator):

* `Tcc/Printf/Sum/Layout.lean`: `layout : Tcc.Printf.StdioLayout` (printf's
  entry; the program's `_reent` and `stdout` FILE; the struct offsets, read
  from the cross compiler by compiling an `offsetof` probe; the static data
  printf may write; the heap; the stack bound; the code), `init` (the arena's
  data as at the cut), `room` (heap room above the host's break), `sym` (the
  machine addresses of P's globals that printf receives);
* `Tcc/Printf/Sum/Code.lean`, `Tcc/Printf/Sum/Init.lean`: the chunk lists.

Where each value comes from:

* `printf`, the closure functions: placed in the arena by their archive bytes
  (`printf_inventory.place`);
* `reent`, `stdoutFile`: `a0`/`a1` at `_vfprintf_r`'s entry in the replay
  (`printf` passes `_REENT` and `_stdout_r(_REENT)`); checked against the
  word `_impure_data._stdout` holds in the arena;
* `statics`: every data object a placed function of printf's direct-call
  closure (and the stdio functions it reaches through pointers: `__swrite`,
  `__ascii_mbtowc`, the `*_r` system-call wrappers) addresses by
  `auipc`+`%pcrel_lo`, with its size from the archive's symbol table; plus the
  host's `brk.0` and `errno` words;
* `stack`: the largest sum of frame sizes (`addi sp,sp,-N`) along a call
  chain of the closure, rounded up to 4096;
* `code`: the arena's text up to the end of tcc's PLT, the PLT's GOT words,
  and the host functions the replay's printf call executes;
* `init`: the arena bytes between the text and the GOT (the libraries' and
  P's data) as the compile phase left them.
"""
import argparse
import collections
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import printf_inventory as inv  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / 'Tcc/Printf/Sum'
CC = str(inv.XB / 'riscv-none-elf-gcc')

# reached through function pointers, not direct calls (stdio's FILE hooks,
# the locale's mbtowc, the system-call wrappers tcc resolves to the host)
INDIRECT = ['__swrite', '__sread', '__sseek', '__sclose', '__ascii_mbtowc',
            '_write_r', '_fstat_r', '_isatty_r', '_sbrk_r', '_lseek_r', '_read_r', '_close_r',
            '__retarget_lock_acquire_recursive', '__retarget_lock_release_recursive',
            '__malloc_lock', '__malloc_unlock', '_localeconv_r', '__locale_mb_cur_max']

PROBE = r'''
#include <stdio.h>
#include <reent.h>
#include <stddef.h>
long off_cleanup = offsetof(struct _reent, __cleanup);
long off_stdout = offsetof(struct _reent, _stdout);
long off_p = offsetof(struct __sFILE, _p);
long off_bf = offsetof(struct __sFILE, _bf);
long off_base = offsetof(struct __sbuf, _base);
long size_file = sizeof(struct __sFILE);
long size_reent = sizeof(struct _reent);
'''


def offsets():
    d = tempfile.mkdtemp(prefix='printf_off_')
    src = os.path.join(d, 'probe.c')
    Path(src).write_text(PROBE)
    asm = inv.run([CC, '-march=rv64i', '-mabi=lp64', '-S', '-o', '-', src])
    vals, cur = {}, None
    for line in asm.splitlines():
        m = re.match(r'^(\w+):$', line)
        if m:
            cur = m.group(1)
            continue
        m = re.match(r'^\s+\.dword\s+(-?\d+)', line)
        if m and cur:
            vals[cur] = int(m.group(1))
            cur = None
        elif re.match(r'^\s+\.zero\s+8', line) and cur:  # a zero lands in .bss
            vals[cur] = 0
            cur = None
    return vals


def sext(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def pcrel_targets(words, addr, body):
    """(symbol+addend, absolute address) for each auipc of a placed function
    whose %pcrel_hi relocation names a symbol and whose next instruction
    completes it with %pcrel_lo off the same register."""
    out = []
    for i, x in enumerate(body):
        hi = [s for k, s in x[4] if k in ('R_RISCV_PCREL_HI20', 'R_RISCV_GOT_HI20')]
        if not hi or i + 1 >= len(body):
            continue
        w = words[addr + 4 * i]
        if w & 0x7f != 0x17:
            continue
        rd = (w >> 7) & 31
        nxt = words[addr + 4 * i + 4]
        op = nxt & 0x7f
        rs1 = (nxt >> 15) & 31
        if rs1 != rd:
            continue
        if op in (0x13, 0x03, 0x1b):  # addi / loads / addiw: I-type
            lo = sext(nxt >> 20, 12)
        elif op == 0x23:  # stores: S-type
            lo = sext(((nxt >> 25) << 5) | ((nxt >> 7) & 31), 12)
        else:
            continue
        target = (addr + 4 * i + sext(w & 0xfffff000, 32) + lo) & ((1 << 64) - 1)
        out.append((hi[0], target))
    return out


def object_sizes(obj):
    """name -> size of the OBJECT symbols of one member."""
    sizes = {}
    for line in inv.run([inv.tool('readelf'), '-sW', str(obj)]).splitlines():
        p = line.split()
        if len(p) >= 8 and p[3] == 'OBJECT':
            sizes[p[7]] = int(p[2], 0)
    return sizes


def chunks(mem, lo, hi):
    """(base, len, packed) chunks of up to 256 present bytes in [lo, hi)."""
    out, a = [], lo
    while a < hi:
        if a not in mem:
            a += 1
            continue
        b = a
        while b < hi and b in mem and b - a < 256:
            b += 1
        packed = 0
        for k in range(b - 1, a - 1, -1):
            packed = (packed << 8) | mem[k]
        out.append((a, b - a, packed))
        a = b
    return out


def elf_bytes(elf, lo, hi):
    segs = []
    data = Path(elf).read_bytes()
    import struct
    phoff, = struct.unpack_from('<Q', data, 0x20)
    phentsize, phnum = struct.unpack_from('<HH', data, 0x36)
    for i in range(phnum):
        (ptype, flags, off, vaddr, paddr, filesz, memsz,
         align) = struct.unpack_from('<IIQQQQQQ', data, phoff + i * phentsize)
        if filesz:
            segs.append((vaddr, data[off:off + filesz]))
    mem = {}
    for a in range(lo, hi):
        for base, body in segs:
            if base <= a < base + len(body):
                mem[a] = body[a - base]
    return mem


def lean_chunks(name, cs, doc):
    lines = [f'/-- {doc} -/', f'def {name} : List Tcc.Boot.Chunk := [']
    for k, (b, n, p) in enumerate(cs):
        sep = ',' if k + 1 < len(cs) else ''
        lines.append(f'  ⟨{b:#x}, {n}, {p:#x}⟩{sep}')
    lines.append(']')
    return '\n'.join(lines)


HEADER = '''/-! GENERATED by scripts/gen_printf_layout.py from {src}.
Do not edit: change the generator. -/
'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--arena', default=str(ROOT / 'results/p3/sum-arena.tsv'))
    ap.add_argument('--elf', default=str(ROOT / 'build/p3/sum.elf'),
                    help='the FLAGS=0 image (sha256 as in results/p3/sum-cut.json)')
    ap.add_argument('--replay', default=str(ROOT / 'results/p3/sum-replay.json'))
    ap.add_argument('--check', action='store_true', help='fail if the outputs differ')
    a = ap.parse_args()
    import hashlib
    want = json.loads((ROOT / 'results/p3/sum-cut.json').read_text())['elf_sha256']
    if not Path(a.elf).exists():
        if a.check:
            print(f'gen_printf_layout: skipped (no ELF at {a.elf})')
            return
        raise SystemExit(f'gen_printf_layout: no ELF at {a.elf}')
    if hashlib.sha256(Path(a.elf).read_bytes()).hexdigest() != want:
        raise SystemExit('gen_printf_layout: the ELF is not the captured image')

    mem, words, funcs, placement, aliases, tmp = inv.place(a.arena)
    rep = json.load(open(a.replay))
    items = rep['items']

    # printf, _vfprintf_r and the replay's registers at their entries
    pf = placement['printf'][0]
    vf = placement['_vfprintf_r'][0]
    k_pf = next(k for k, it in enumerate(items) if it['pc'] == pf)
    k_vf = next(k for k, it in enumerate(items) if it['pc'] == vf)
    reent, stdout_file = items[k_vf]['regs'][9], items[k_vf]['regs'][10]
    fmt_addr = items[k_pf]['regs'][9]

    off = offsets()
    word = lambda x: sum(mem[x + j] << (8 * j) for j in range(8))
    assert word(reent + off['off_stdout']) == stdout_file, 'stdout FILE mismatch'

    # the closure: direct calls from printf, plus the INDIRECT entry points
    calls = {n: sorted({x[2] for x in body if x[2]}) for n, (_, body) in funcs.items()}
    closure, todo = set(), ['printf'] + INDIRECT
    while todo:
        f = todo.pop()
        if f in closure or f not in funcs:
            continue
        closure.add(f)
        todo.extend(calls.get(f, []))
    placed_closure = sorted(f for f in closure if f in placement)

    # statics: data objects the closure addresses
    sizes_by_obj = {}
    objs = {o.name: o for o in Path(tmp).rglob('*.o')}
    statics = {}
    text_hi = max(placement[f][0] + 4 * placement[f][2] for f in placement)
    for f in placed_closure:
        addr, obj, n = placement[f]
        body = funcs[f][1]
        if obj not in sizes_by_obj:
            sizes_by_obj[obj] = object_sizes(objs[obj])
        for symadd, target in pcrel_targets(words, addr, body):
            sym = symadd.split('+')[0].split('-')[0]
            if target < text_hi:
                continue  # code or read-only data inside the text
            addend = target - (target if '+' not in symadd else 0)
            if '+' in symadd:
                base = target - int(symadd.split('+')[1], 0)
            else:
                base = target
            size = sizes_by_obj[obj].get(sym)
            if size is None:
                # defined in another member: look it up there
                for o in objs.values():
                    if o.name not in sizes_by_obj:
                        sizes_by_obj[o.name] = object_sizes(o)
                    if sym in sizes_by_obj[o.name]:
                        size = sizes_by_obj[o.name][sym]
                        break
            if size is None or base < inv.ARENA:
                continue
            statics[base] = max(statics.get(base, 0), size, 8)
    host = {}
    for line in inv.run([inv.tool('nm'), '-S', a.elf]).splitlines():
        p = line.split()
        if len(p) == 4:
            host[p[3]] = (int(p[0], 16), int(p[1], 16))
        elif len(p) == 3:
            host.setdefault(p[2], (int(p[0], 16), 0))
    for h in ('brk.0', 'errno'):
        statics[host[h][0]] = max(host[h][1], 8)
    # the program's `_impure_data`: reached through `_impure_ptr` (a data
    # relocation), not by a pc-relative reference of the code
    statics[reent] = max(statics.get(reent, 0), off['size_reent'])
    # merge overlapping ranges
    ranges = []
    for b in sorted(statics):
        e = b + statics[b]
        if ranges and b <= ranges[-1][1]:
            ranges[-1][1] = max(ranges[-1][1], e)
        else:
            ranges.append([b, e])
    heap = (host['_end'][0], host['__heap_end'][0])

    # the text's end: the last archive function, then tcc's PLT (a header of
    # eight words starting with `auipc t2`, then stubs `auipc t3; ld t3;
    # jalr t1,t3; nop`); the GOT words the PLT loads
    t = text_hi
    got = set()
    if words.get(t, 0) & 0xfff == 0x397:  # auipc t2
        hdr_hi = sext(words[t] & 0xfffff000, 32)
        got.update(range(t + hdr_hi + sext(words[t + 8] >> 20, 12), t + hdr_hi + sext(words[t + 8] >> 20, 12) + 16))
        t += 32
    while words.get(t, 0) & 0xfff == 0xe17 and words.get(t + 12) == 0x13:  # auipc t3 ... nop
        g = t + sext(words[t] & 0xfffff000, 32) + sext(words[t + 4] >> 20, 12)
        got.update(range(g, g + 8))
        t += 16
    text_end = t
    got_lo, got_hi = min(got), max(got) + 1

    # host functions the printf call executes
    end = next(k for k in range(k_pf + 1, len(items)) if items[k]['pc'] == items[k_pf]['regs'][0])
    hpcs = set()
    for it in items[k_pf:end]:
        if it['kind'] == 'block':
            hpcs.update(pc for pc, _ in it['body'] if pc < inv.ARENA)
        elif it['pc'] < inv.ARENA:
            hpcs.add(it['pc'])
    hfuncs = sorted({(b, s, n) for n, (b, s) in host.items() if s and any(b <= pc < b + s for pc in hpcs)})
    host_mem = {}
    for b, s, n in hfuncs:
        host_mem.update(elf_bytes(a.elf, b, b + s))

    # stack: frames along call chains of the closure (static), host frames too
    def frame(addr):
        w = words.get(addr) if addr >= inv.ARENA else None
        if w is None:
            bs = host_mem
            w = sum(bs.get(addr + j, 0) << (8 * j) for j in range(4))
        if w & 0xfffff == 0x10113:  # addi sp,sp,imm
            return max(0, -sext(w >> 20, 12))
        return 0
    graph = {f: [g for g in calls.get(f, []) if g in placement] for f in placed_closure}
    graph['_vfprintf_r'] = graph.get('_vfprintf_r', []) + ['__ascii_mbtowc']
    graph['__sflush_r'] = graph.get('__sflush_r', []) + ['__swrite']
    host_frame = max((frame(b) for b, s, n in hfuncs), default=0)
    memo = {}

    def depth(f, seen=()):
        if f in memo:
            return memo[f]
        if f in seen:
            return 0
        d = frame(placement[f][0]) + max([depth(g, seen + (f,)) for g in graph.get(f, [])
                                          if g in placement] + [host_frame])
        memo[f] = d
        return d
    static_depth = depth('printf')
    stack = max(4096, (static_depth + 4095) // 4096 * 4096)

    code = chunks(mem, inv.ARENA, text_end) + chunks(mem, got_lo, got_hi) + \
        chunks(host_mem, min(host_mem), max(host_mem) + 1)
    init = chunks(mem, text_end, got_lo)

    rel = lambda x: os.path.relpath(Path(x).resolve(), ROOT)
    src = f'{rel(a.arena)} (scripts/tcc_cut.py --arena), {rel(a.replay)} and the image ELF'
    layout = '''import Tcc.Printf.Spec
import Tcc.Printf.Sum.Code
import Tcc.Printf.Sum.Init
import Tcc.Prog.Sum.Clight

''' + HEADER.format(src=src) + f'''
/-! The printf layout of `tcc -run sum.c` (the `FLAGS=0` image of
`results/p3/sum-cut.json`): printf at `{pf:#x}`, the program's `_reent` at
`{reent:#x}`, its `stdout` FILE at `{stdout_file:#x}`. Static frame bound along
printf's call chains: {static_depth} bytes (rounded to `stack`); measured on
P's call: {items[k_pf]['regs'][1] - min(it['regs'][1] for it in items[k_pf:end])} bytes. Host functions in `code`:
{', '.join(n for b, s, n in hfuncs)}. -/

namespace Tcc.Printf.Sum

open Tcc.Printf

/-- The program's newlib in the arena. -/
def layout : StdioLayout where
  printf := {pf:#x}
  reent := {reent:#x}
  cleanupOff := {off['off_cleanup']}
  stdoutFile := {stdout_file:#x}
  pOff := {off['off_p']}
  baseOff := {off['off_bf'] + off['off_base']}
  statics := [{', '.join(f'({b:#x}, {e - b})' for b, e in ranges)}]
  heap := ({heap[0]:#x}, {heap[1]:#x})
  stack := {stack}
  code := codeChunks

/-- The host's break (`_sbrk`'s `brk`) leaves 1 MiB of heap. -/
def room (σ : Vsa.Machine.MState) : Prop :=
  layout.heap.1 ≤ rdWord σ {host['brk.0'][0]:#x} ∧ rdWord σ {host['brk.0'][0]:#x} + 1048576 ≤ layout.heap.2

/-- The machine addresses of P's globals that reach printf: its format
string (`__stringlit_1`, printf's `a0` in the replay). -/
def sym (id : CC.Ident) : Option Nat :=
  if id = Tcc.Prog.Sum.___stringlit_1 then some {fmt_addr:#x} else none

end Tcc.Printf.Sum
'''
    code_lean = 'import Tcc.Boot.View\n\n' + HEADER.format(src=src) + '\nnamespace Tcc.Printf.Sum\n\n' + \
        lean_chunks('codeChunks', code,
                    f'The code printf may run: the arena text `{inv.ARENA:#x}..{text_end:#x}` '
                    f'(the relocated libc/libgcc members and tcc\'s PLT), the GOT words '
                    f'`{got_lo:#x}..{got_hi:#x}` and the host system calls.') + \
        '\n\nend Tcc.Printf.Sum\n'
    init_lean = 'import Tcc.Boot.View\n\n' + HEADER.format(src=src) + '\nnamespace Tcc.Printf.Sum\n\n' + \
        lean_chunks('init', init,
                    f'The arena data `{text_end:#x}..{got_lo:#x}` as the compile phase left it.') + \
        '\n\nend Tcc.Printf.Sum\n'
    outs = {OUT / 'Layout.lean': layout, OUT / 'Code.lean': code_lean, OUT / 'Init.lean': init_lean}
    if a.check:
        bad = [str(p) for p, s in outs.items() if not p.exists() or p.read_text() != s]
        if bad:
            raise SystemExit('gen_printf_layout: out of date: ' + ', '.join(bad))
        print('gen_printf_layout: up to date')
        return
    OUT.mkdir(parents=True, exist_ok=True)
    for p, s in outs.items():
        p.write_text(s)
    print(json.dumps({'printf': hex(pf), 'reent': hex(reent), 'stdout': hex(stdout_file),
                      'offsets': off, 'statics': [[hex(b), e - b] for b, e in ranges],
                      'heap': [hex(heap[0]), hex(heap[1])], 'stack': stack,
                      'static_depth': static_depth, 'text_end': hex(text_end),
                      'got': [hex(got_lo), hex(got_hi)], 'host': [n for b, s, n in hfuncs],
                      'code_chunks': len(code), 'init_chunks': len(init)}, indent=1))


if __name__ == '__main__':
    main()
