#!/usr/bin/env python3
"""Generate the CompileCut pilot's Lean inputs (Tcc/Ccp/Eval.lean's data).

    scripts/gen_ccp.py --trace build/ccp/trace --out <dir> [--chunk A:B ...]

From scripts/compile_trace.py's output (`pcs.tsv`, `ckpt.tsv`, `stores.bin`)
and the ELF pin of the captured cut (`Tcc/Capture/Sum/Elf/P*.lean` chunks and
`elfAbsent`, the bytes `TccSumElf` pins: the evaluator's memory at reset is
exactly those bytes, the absent ones as 0):

* `Code.lean`: the pre-decoded code table (`Tcc.Ccp.Code`) of every executed
  PC, a balanced search tree;
* `Mem0.lean`: the memory at reset over every page the run touches (pinned
  bytes known, everything else unknown);
* for each `--chunk A:B` (steps, multiples of the checkpoint stride), a file
  `ChunkA_B.lean` with the states at A and B (as page deltas against the
  reset memory) and the kernel check `chunkOK code (B - A) sA sB = true`.

The states come from the native trace (registers at checkpoints, memory from
the pins and every store), never from the Lean evaluator, so a passing check
is a cross-check of the evaluator against the emulator.
"""
import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PS = 8
PB = 1 << PS
W = 1 << 64


def sext(v, bits):
    v &= (1 << bits) - 1
    return v - (1 << bits) if v >> (bits - 1) else v


def m64(v):
    return v % W


def decode(pc, w):
    op = w & 0x7f
    rd = (w >> 7) & 31
    f3 = (w >> 12) & 7
    rs1 = (w >> 15) & 31
    rs2 = (w >> 20) & 31
    f7 = w >> 25
    immI = sext(w >> 20, 12)
    immS = sext(((w >> 25) << 5) | ((w >> 7) & 31), 12)
    immB = sext((((w >> 31) & 1) << 12) | (((w >> 7) & 1) << 11) | (((w >> 25) & 0x3f) << 5)
                | (((w >> 8) & 0xf) << 1), 13)
    immJ = sext((((w >> 31) & 1) << 20) | (((w >> 12) & 0xff) << 12) | (((w >> 20) & 1) << 11)
                | (((w >> 21) & 0x3ff) << 1), 21)
    immU = sext(w & 0xfffff000, 32)
    if op == 0x37:
        return f'.li {rd} {m64(immU)}'
    if op == 0x17:
        return f'.li {rd} {m64(pc + immU)}'
    if op == 0x6f:
        return f'.jal {rd} {m64(pc + immJ)}'
    if op == 0x67 and f3 == 0:
        return f'.jalr {rd} {rs1} {m64(immI)}'
    if op == 0x63:
        n = {0: 'beq', 1: 'bne', 4: 'blt', 5: 'bge', 6: 'bltu', 7: 'bgeu'}[f3]
        return f'.{n} {rs1} {rs2} {m64(pc + immB)}'
    if op == 0x03:
        n = ['lb', 'lh', 'lw', 'ld', 'lbu', 'lhu', 'lwu'][f3]
        return f'.{n} {rd} {rs1} {m64(immI)}'
    if op == 0x23:
        n = ['sb', 'sh', 'sw', 'sd'][f3]
        return f'.{n} {rs1} {rs2} {m64(immS)}'
    if op == 0x13:
        if f3 == 1:
            assert (w >> 26) == 0
            return f'.slli {rd} {rs1} {(w >> 20) & 63}'
        if f3 == 5:
            n = 'srai' if (w >> 26) == 0x10 else 'srli'
            assert (w >> 26) in (0, 0x10)
            return f'.{n} {rd} {rs1} {(w >> 20) & 63}'
        n = {0: 'addi', 2: 'slti', 3: 'sltiu', 4: 'xori', 6: 'ori', 7: 'andi'}[f3]
        return f'.{n} {rd} {rs1} {m64(immI)}'
    if op == 0x1b:
        if f3 == 0:
            return f'.addiw {rd} {rs1} {m64(immI)}'
        assert f7 in (0, 0x20)
        n = {1: 'slliw', 5: 'sraiw' if f7 == 0x20 else 'srliw'}[f3]
        return f'.{n} {rd} {rs1} {(w >> 20) & 31}'
    if op == 0x33:
        assert f7 in (0, 0x20), hex(w)
        n = {(0, 0): 'add', (0, 0x20): 'sub', (1, 0): 'sll', (2, 0): 'slt', (3, 0): 'sltu',
             (4, 0): 'xor', (5, 0): 'srl', (5, 0x20): 'sra', (6, 0): 'or', (7, 0): 'and'}[(f3, f7)]
        return f'.{n} {rd} {rs1} {rs2}'
    if op == 0x3b:
        n = {(0, 0): 'addw', (0, 0x20): 'subw', (1, 0): 'sllw', (5, 0): 'srlw',
             (5, 0x20): 'sraw'}[(f3, f7)]
        return f'.{n} {rd} {rs1} {rs2}'
    if op == 0x0f:
        return '.fence'
    raise SystemExit(f'gen_ccp: unsupported word {w:08x} at {pc:#x}')


def parse_pins():
    """The bytes TccSumElf pins: elfImage chunks and elfAbsent (as 0)."""
    pins = {}
    chunk = re.compile(r'⟨0x([0-9a-f]+), (\d+), 0x([0-9a-f]+)⟩')
    for p in sorted((ROOT / 'Tcc/Capture/Sum/Elf').glob('P*.lean')):
        for base, ln, packed in chunk.findall(p.read_text()):
            base, ln, packed = int(base, 16), int(ln), int(packed, 16)
            for j in range(ln):
                pins.setdefault(base + j, (packed >> (8 * j)) & 0xff)
    src = (ROOT / 'Tcc/Capture/Sum/Elf.lean').read_text()
    absent = re.search(r'def elfAbsent[^\[]*\[(.*?)\]', src, re.S).group(1)
    for base, ln in re.findall(r'\(0x([0-9a-f]+), (\d+)\)', absent):
        for j in range(int(ln)):
            pins.setdefault(int(base, 16) + j, 0)
    return pins


def balanced(items, leaf, node):
    """A balanced search tree over sorted (key, payload) items."""
    def go(lo, hi):
        if hi - lo == 1:
            return leaf(*items[lo])
        mid = (lo + hi) // 2
        return node(items[mid][0], go(lo, mid), go(mid, hi))
    return go(0, len(items))


def tree_defs(name, ty, items, leaf, block=64):
    """`balanced`, split into one definition per block of `block` leaves (the
    elaborator's recursion depth bounds the nesting of one term) and a top
    definition `name` over the blocks."""
    node = lambda k, l, r: f'({ty}.node {k:#x}\n {l}\n {r})'
    out = []
    blocks = []
    for b in range(0, len(items), block):
        part = items[b:b + block]
        bn = f'{name}_{b // block}'
        out.append(f'def {bn} : {ty} :=\n {balanced(part, leaf, node)}\n')
        blocks.append((part[0][0], bn))
    out.append(f'def {name} : {ty} :=\n {balanced(blocks, lambda k, n: n, node)}\n')
    return '\n'.join(out)


class Trace:
    def __init__(self, d):
        d = Path(d)
        self.pcs = []
        for line in (d / 'pcs.tsv').read_text().splitlines():
            pc, w, n, m = line.split('\t')
            self.pcs.append((int(pc, 16), int(w, 16)))
        self.ckpt = {}
        for line in (d / 'ckpt.tsv').read_text().splitlines():
            f = line.split('\t')
            self.ckpt[int(f[0])] = (int(f[1], 16), [int(x, 16) for x in f[2:33]])
        raw = (d / 'stores.bin').read_bytes()
        sz = struct.calcsize('<IQBQ')
        self.stores = [struct.unpack_from('<IQBQ', raw, o) for o in range(0, len(raw), sz)]


def regs_nat(regs):
    r = 0
    for i, v in enumerate(regs):
        r |= v << (64 * (i + 1))
    return r


def pages_of(mem):
    pg = {}
    for a, b in mem.items():
        d, m = pg.get(a >> PS, (0, 0))
        off = a & (PB - 1)
        pg[a >> PS] = (d | (b << (8 * off)), m | (1 << off))
    return pg


def mem_at(pins, stores, k):
    mem = dict(pins)
    for step, addr, w, data in stores:
        if step >= k:
            break
        for j in range(w):
            mem[addr + j] = (data >> (8 * j)) & 0xff
    return mem


def mt_text(name, universe, pages):
    items = [(p, pages.get(p, (0, 0))) for p in universe]
    return tree_defs(name, 'MT', items, lambda p, dm: f'(MT.leaf {p:#x} {dm[0]:#x} {dm[1]:#x})')


def dt_text(name, universe, changed, block=64):
    """The delta (`DT`) turning the tree over `universe` into one with the
    `changed` pages, in the shape `tree_defs` gives the tree: one definition
    per changed block, `keep` for unchanged subtrees."""
    def go(items, lo, hi):
        if not any(items[j][0] in changed for j in range(lo, hi)):
            return 'DT.keep'
        if hi - lo == 1:
            d, m = changed[items[lo][0]]
            return f'(DT.leaf {d:#x} {m:#x})'
        mid = (lo + hi) // 2
        return f'(DT.node {go(items, lo, mid)}\n {go(items, mid, hi)})'
    items = [(p, None) for p in universe]
    blocks = [(b, min(b + block, len(items))) for b in range(0, len(items), block)]
    defs = []
    names = []
    for j, (lo, hi) in enumerate(blocks):
        t = go(items, lo, hi)
        if t == 'DT.keep':
            names.append(('DT.keep', False))
        else:
            defs.append(f'def {name}{j} : DT :=\n {t}\n')
            names.append((f'{name}{j}', True))
    def top(lo, hi):
        if not any(names[j][1] for j in range(lo, hi)):
            return 'DT.keep'
        if hi - lo == 1:
            return names[lo][0]
        mid = (lo + hi) // 2
        return f'(DT.node {top(lo, mid)} {top(mid, hi)})'
    return top(0, len(names)), defs


HEADER = '''/-! GENERATED by scripts/gen_ccp.py (CompileCut pilot). Do not edit: change the generator. -/
'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--trace', required=True)
    ap.add_argument('--out', required=True)
    ap.add_argument('--ns', default='Tcc.Ccp.Sum')
    ap.add_argument('--chunk', action='append', default=[])
    ap.add_argument('--chain', default=None,
                    help='K:N: checkpoint states every K steps up to N (St/S<i>.lean, each '
                         'a delta on the previous) and one chunk theorem per segment '
                         '(Seg/C<i>.lean), under --out')
    ap.add_argument('--form', choices=['rfl', 'beq'], default='rfl',
                    help='chunk statement: run equation by kernel_rfl, or chunkOK by decide')
    a = ap.parse_args()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    tr = Trace(a.trace)
    pins = parse_pins()
    code_bytes = set()
    for pc, w in tr.pcs:
        for j in range(4):
            code_bytes.add(pc + j)
    lo, hi = min(code_bytes), max(code_bytes) + 1
    for step, addr, w, data in tr.stores:
        if not (addr + w <= lo or hi <= addr):
            raise SystemExit(f'gen_ccp: store into the code range at {addr:#x}')
    written_pages = {(addr + j) >> PS for _, addr, w, _ in tr.stores for j in range(w)}
    universe = sorted({a >> PS for a in pins} | written_pages)
    mod = a.ns
    # code
    words = dict(tr.pcs)
    items = [(pc, decode(pc, w)) for pc, w in tr.pcs]
    ct = tree_defs('table', 'CT', items,
                   lambda pc, i: f'(CT.leaf {pc:#x} {words[pc]:#x} (Ins{i}))')
    (out / 'Code.lean').write_text(
        'import Tcc.Ccp.Eval\n\n' + HEADER + '\nnamespace ' + mod + '\nopen Tcc.Ccp\n\n'
        f'/-- The {len(items)} executed instructions of the compile phase. -/\n'
        f'{ct}\n'
        f'def code : Code := ⟨table, {lo:#x}, {hi:#x}⟩\n\nend {mod}\n')
    pages0 = pages_of(pins)
    (out / 'Mem0.lean').write_text(
        'import Tcc.Ccp.Eval\n\n' + HEADER + '\nnamespace ' + mod + '\nopen Tcc.Ccp\n\n'
        f'/-- The memory at reset over the {len(universe)} pages the run touches. -/\n'
        f'{mt_text("mem0", universe, pages0)}\nend {mod}\n')
    print(f'code: {len(items)} instrs, range {lo:#x}..{hi:#x}; pages {len(universe)}; '
          f'pins {len(pins)}; stores {len(tr.stores)}', file=sys.stderr)
    if a.chain:
        gen_chain(a, tr, pins, universe, pages0, out, mod)
    for spec in a.chunk:
        s0, s1 = (int(x) for x in spec.split(':'))
        txt = [f'import {mod}.Code\nimport {mod}.Mem0\nimport Tcc.Ccp.KernelRfl\n', HEADER,
               f'namespace {mod}\nopen Tcc.Ccp\n']
        for k in (s0, s1):
            pc, regs = tr.ckpt[k]
            pk = pages_of(mem_at(pins, tr.stores, k))
            delta = [(p, pk[p]) for p in universe if pk.get(p, (0, 0)) != pages0.get(p, (0, 0))]
            changed = {p: dm for p, dm in delta}
            dt, defs = dt_text(f's{k}_d', universe, changed)
            txt.extend(defs)
            txt.append(f'/-- Step {k}: {len(delta)} pages differ from reset. -/\n'
                       f'def s{k} : St := ⟨{pc:#x}, {regs_nat(regs):#x},\n mem0.overlay {dt}⟩\n')
        if a.form == 'beq':
            txt.append(f'theorem chunk_{s0}_{s1} : chunkOK code {s1 - s0} s{s0} s{s1} = true := by\n'
                       f'  decide +kernel\n\nend {mod}\n')
        else:
            txt.append(f'theorem chunk_{s0}_{s1} : run code {s1 - s0} s{s0} = some s{s1} := by\n'
                       f'  kernel_rfl\n\nend {mod}\n')
        (out / f'Chunk{s0}_{s1}.lean').write_text('\n'.join(txt))


def gen_chain(a, tr, pins, universe, pages0, out, mod):
    """Checkpoint states `st<i>` (step i*K) as deltas on their predecessor, in a
    chain of light modules, and the chunk theorems `seg<i> : run code K st<i> =
    some st<i+1>` in modules that only import the two states (built in parallel)."""
    K, N = (int(x) for x in a.chain.split(':'))
    assert N % K == 0 and K % 1000 == 0
    (out / 'St').mkdir(exist_ok=True)
    (out / 'Seg').mkdir(exist_ok=True)
    pc0, regs0 = tr.ckpt[0]
    (out / 'St' / 'S0.lean').write_text(
        f'import {mod}.Mem0\n\n' + HEADER + f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
        f'/-- The evaluator state at reset. -/\n'
        f'def st0 : St := ⟨{pc0:#x}, {regs_nat(regs0):#x}, mem0⟩\n\nend {mod}\n')
    prev = pages0
    mem = dict(pins)
    si = 0
    sizes = []
    for i in range(1, N // K + 1):
        k = i * K
        while si < len(tr.stores) and tr.stores[si][0] < k:
            step, addr, w, data = tr.stores[si]
            for j in range(w):
                mem[addr + j] = (data >> (8 * j)) & 0xff
            si += 1
        pk = pages_of(mem)
        changed = {p: pk[p] for p in universe if pk.get(p, (0, 0)) != prev.get(p, (0, 0))}
        dt, defs = dt_text(f'st{i}_d', universe, changed)
        pc, regs = tr.ckpt[k]
        body = (f'import {mod}.St.S{i - 1}\n\n' + HEADER + f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
                + '\n'.join(defs) +
                f'\n/-- Step {k}: {len(changed)} pages changed since step {k - K}. -/\n'
                f'def st{i} : St := ⟨{pc:#x}, {regs_nat(regs):#x},\n st{i - 1}.mem.overlay {dt}⟩\n'
                f'\nend {mod}\n')
        (out / 'St' / f'S{i}.lean').write_text(body)
        sizes.append(len(body))
        (out / 'Seg' / f'C{i - 1}.lean').write_text(
            f'import {mod}.Code\nimport {mod}.St.S{i}\nimport Tcc.Ccp.KernelRfl\n\n' + HEADER +
            f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
            f'/-- Steps {k - K} to {k}. -/\n'
            f'theorem seg{i - 1} : run code {K} st{i - 1} = some st{i} := by\n  kernel_rfl\n\nend {mod}\n')
        prev = pk
    print(f'chain: {N // K} segments of {K}; state sources {sum(sizes)} bytes '
          f'(max {max(sizes)})', file=sys.stderr)


if __name__ == '__main__':
    main()
