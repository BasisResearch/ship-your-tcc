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
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DIGEST = ROOT / 'results/p3/ccp-digest.json'
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
        self.written_pages = {(addr + j) >> PS for _, addr, w, _ in self.stores for j in range(w)}


class DigestTrace:
    """The part of the native trace the generated files depend on, apart from
    the checkpoint states' page deltas (those are checked by hash): the
    executed PCs and words, the written pages, the checkpoints' PC and GPRs."""

    def __init__(self, dg):
        self.pcs = [(int(pc, 16), int(w, 16)) for pc, w in dg['pcs']]
        self.ckpt = {int(k): (int(pc, 16), [int(x, 16) for x in regs])
                     for k, (pc, regs) in dg['ckpt'].items()}
        self.stores = None
        self.written_pages = {int(x, 16) for x in dg['written_pages']}


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


class Emitter:
    """Writes generated files, or (check mode) compares them with what is on
    disk; `hashed` files (the checkpoint states, whose deltas need the native
    trace) are compared by their SHA-256 in the digest."""

    def __init__(self, check=False, st_sha=None):
        self.check = check
        self.st_sha = st_sha or {}
        self.drift = []
        self.hashes = {}
        self.count = 0

    def emit(self, path, text):
        path = Path(path)
        self.count += 1
        self.hashes[str(path)] = hashlib.sha256(text.encode()).hexdigest()
        if self.check:
            if not path.exists() or path.read_text() != text:
                self.drift.append(str(path))
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)

    def hashed(self, path, key):
        path = Path(path)
        self.count += 1
        want = self.st_sha.get(key)
        got = hashlib.sha256(path.read_bytes()).hexdigest() if path.exists() else None
        if want is None or got != want:
            self.drift.append(str(path))


EMIT = Emitter()


HEADER = '''/-! GENERATED by scripts/gen_ccp.py (CompileCut pilot). Do not edit: change the generator. -/
'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--trace', help="scripts/compile_trace.py's output directory")
    ap.add_argument('--out', default='Tcc/Ccp/Sum')
    ap.add_argument('--digest', default=str(DIGEST),
                    help='the committed digest of the trace (PCs, words, written pages, checkpoint '
                         'registers, the checkpoint states\' hashes)')
    ap.add_argument('--write-digest', action='store_true',
                    help='with --trace: also write the digest (records the options)')
    ap.add_argument('--check', action='store_true',
                    help='without the trace: regenerate everything from the digest (the states by '
                         'hash) with the digest\'s options and fail on any difference')
    ap.add_argument('--ns', default='Tcc.Ccp.Sum')
    ap.add_argument('--chunk', action='append', default=[])
    ap.add_argument('--chain', default=None,
                    help='K:N: checkpoint states every K steps up to N (St/S<i>.lean, each '
                         'a delta on the previous) and one chunk theorem per segment '
                         '(Seg/C<i>.lean), under --out')
    ap.add_argument('--pin-proof', action='store_true',
                    help='emit Pin/M<j>.lean, Pin/C<j>.lean and Pin/All.lean: the reset memory and the '
                         'code table are the bytes TccSumElf pins (kernel decides, 8 blocks per file)')
    ap.add_argument('--table-proof', action='store_true',
                    help='emit Table/T<k>.lean (each block of the code table satisfies EntryOK) '
                         'and Table/All.lean (TableOK code)')
    ap.add_argument('--form', choices=['rfl', 'beq'], default='rfl',
                    help='chunk statement: run equation by kernel_rfl, or chunkOK by decide')
    a = ap.parse_args()
    global EMIT
    if a.check:
        dg = json.loads(Path(a.digest).read_text())
        tr = DigestTrace(dg)
        a.out, a.chain = dg['out'], dg['chain']
        a.table_proof, a.pin_proof, a.chunk = dg['table_proof'], dg['pin_proof'], []
        EMIT = Emitter(check=True, st_sha=dg['st_sha'])
    else:
        if not a.trace:
            raise SystemExit('gen_ccp: --trace or --check')
        tr = Trace(a.trace)
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    pins = parse_pins()
    code_bytes = set()
    for pc, w in tr.pcs:
        for j in range(4):
            code_bytes.add(pc + j)
    lo, hi = min(code_bytes), max(code_bytes) + 1
    for step, addr, w, data in tr.stores or []:
        if not (addr + w <= lo or hi <= addr):
            raise SystemExit(f'gen_ccp: store into the code range at {addr:#x}')
    written_pages = tr.written_pages
    universe = sorted({a >> PS for a in pins} | written_pages)
    mod = a.ns
    # code
    words = dict(tr.pcs)
    items = [(pc, decode(pc, w)) for pc, w in tr.pcs]
    ct = tree_defs('table', 'CT', items,
                   lambda pc, i: f'(CT.leaf {pc:#x} {words[pc]:#x} (Ins{i}))')
    EMIT.emit(out / 'Code.lean', 
        'import Tcc.Ccp.Eval\n\n' + HEADER + '\nnamespace ' + mod + '\nopen Tcc.Ccp\n\n'
        f'/-- The {len(items)} executed instructions of the compile phase. -/\n'
        f'{ct}\n'
        f'def code : Code := ⟨table, {lo:#x}, {hi:#x}⟩\n\nend {mod}\n')
    pages0 = pages_of(pins)
    EMIT.emit(out / 'Mem0.lean', 
        'import Tcc.Ccp.Eval\n\n' + HEADER + '\nnamespace ' + mod + '\nopen Tcc.Ccp\n\n'
        f'/-- The memory at reset over the {len(universe)} pages the run touches. -/\n'
        f'{mt_text("mem0", universe, pages0)}\nend {mod}\n')
    print(f'code: {len(items)} instrs, range {lo:#x}..{hi:#x}; pages {len(universe)}; '
          f'pins {len(pins)}', file=sys.stderr)
    ckpts = [0]
    if a.chain:
        ckpts = gen_chain(a, tr, pins, universe, pages0, out, mod)
    if a.table_proof:
        gen_table_proof(len(items), out, mod)
    if a.pin_proof:
        gen_pin_proof(len(items), len(universe), out, mod)
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
        EMIT.emit(out / f'Chunk{s0}_{s1}.lean', '\n'.join(txt))
    if a.check:
        if EMIT.drift:
            print(f'gen_ccp: drift in {len(EMIT.drift)} of {EMIT.count} files: ' +
                  ', '.join(EMIT.drift[:10]) + (' …' if len(EMIT.drift) > 10 else ''))
            sys.exit(1)
        print(f'gen_ccp: up to date ({EMIT.count} files; checkpoint states by hash)')
        return
    if a.write_digest:
        st_sha = {Path(k).stem: v for k, v in EMIT.hashes.items()
                  if Path(k).parent.name == 'St' and Path(k).stem != 'S0'}
        dg = {'note': 'scripts/gen_ccp.py --check input; from scripts/compile_trace.py',
              'out': a.out, 'chain': a.chain, 'table_proof': a.table_proof, 'pin_proof': a.pin_proof,
              'pcs': [[f'{pc:x}', f'{w:x}'] for pc, w in tr.pcs],
              'written_pages': [f'{x:x}' for x in sorted(written_pages)],
              'ckpt': {str(k): [f'{tr.ckpt[k][0]:x}', [f'{x:x}' for x in tr.ckpt[k][1]]] for k in ckpts},
              'st_sha': st_sha}
        Path(a.digest).write_text(json.dumps(dg, separators=(',', ':')) + '\n')
        print(f'gen_ccp: wrote {a.digest}', file=sys.stderr)


def gen_pin_proof(ncode, npages, out, mod, block=64, per=8):
    """`mem0_pinned` and `code_pinned`: every known byte of the reset memory is
    the pinned ELF's (or an absent one, zero), every table word is the pinned
    bytes at its PC. One kernel decide per block of 64 pages / entries, `per`
    blocks per module, joined along the block trees of `tree_defs`."""
    (out / 'Pin').mkdir(exist_ok=True)
    P = 'pinB Tcc.Capture.Sum.Elf.elfAbsentB Tcc.Capture.Sum.Elf.elfImage'
    C = 'codeB Tcc.Capture.Sum.Elf.elfAbsentB Tcc.Capture.Sum.Elf.elfImage'
    hdr = (f'import {mod}.Mem0\nimport {mod}.Code\nimport Tcc.Ccp.Sound\nimport Tcc.Capture.Sum.Elf\n\n'
           + HEADER + f'\nnamespace {mod}\nopen Tcc.Ccp\n\n')
    mods = []
    nm = (npages + block - 1) // block
    nc = (ncode + block - 1) // block
    for kind, n, mk in (('M', nm, lambda k: f'theorem mem0_{k}_pin : MT.allKnown ({P}) mem0_{k} = true := by\n  decide +kernel\n'),
                        ('C', nc, lambda k: f'theorem table_{k}_pin : CT.allW ({C}) table_{k} = true := by\n  decide +kernel\n')):
        for j in range(0, n, per):
            body = '\n'.join(mk(k) for k in range(j, min(n, j + per)))
            name = f'{kind}{j // per}'
            EMIT.emit(out / 'Pin' / f'{name}.lean', hdr + body + f'\nend {mod}\n')
            mods.append(name)
    mem_pins = ', '.join(f'mem0_{k}_pin' for k in range(nm))
    tab_pins = ', '.join(f'table_{k}_pin' for k in range(nc))
    EMIT.emit(out / 'Pin' / 'All.lean', 
        '\n'.join(f'import {mod}.Pin.{m}' for m in mods) + '\n\n' + HEADER +
        f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
        f'/-- Every byte the reset memory knows is the pinned ELF\'s (absent ones zero). -/\n'
        f'theorem mem0_pinned : MT.allKnown ({P}) mem0 = true := by\n'
        f'  simp only [mem0, MT.allKnown, {mem_pins}, Bool.and_self]\n\n'
        f'/-- Every code-table word is the pinned ELF\'s bytes at its PC. -/\n'
        f'theorem code_pinned : CT.allW ({C}) code.table = true := by\n'
        f'  simp only [code, table, CT.allW, {tab_pins}, Bool.and_self]\n\nend {mod}\n')


def gen_table_proof(n, out, mod, block=64):
    """`TableOK code`, one module per block of 64 entries (each entry: the
    fetch facts by `decide`, the decode fact by `decodeW`, through `ccp_entry`)
    and a join following the block tree of `tree_defs`."""
    (out / 'Table').mkdir(exist_ok=True)
    nb = (n + block - 1) // block
    for k in range(nb):
        EMIT.emit(out / 'Table' / f'T{k}.lean', 
            f'import {mod}.Code\nimport Tcc.Ccp.Sound\n\n' + HEADER + f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
            f'theorem table_{k}_ok : CT.All (EntryOK code) table_{k} := by\n'
            f'  simp only [table_{k}, CT.All]\n  ccp_entry\n\nend {mod}\n')
    def top(lo, hi):
        if hi - lo == 1:
            return f'table_{lo}_ok'
        mid = (lo + hi) // 2
        return f'⟨{top(lo, mid)}, {top(mid, hi)}⟩'
    EMIT.emit(out / 'Table' / 'All.lean', 
        '\n'.join(f'import {mod}.Table.T{k}' for k in range(nb)) + '\n\n' + HEADER +
        f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
        f'/-- Every entry of the code table decodes, under Sail, to what the evaluator runs. -/\n'
        f'theorem tableOK : TableOK code := tableOK_of_all (by\n'
        f'  show CT.All (EntryOK code) table\n  simp only [table, CT.All]\n  exact {top(0, nb)})\n\nend {mod}\n')


def gen_chain(a, tr, pins, universe, pages0, out, mod):
    """Checkpoint states `st<i>` every K steps (the last one at N, which need
    not be a multiple of K), each a delta on its predecessor, in a chain of
    light modules `St/S<i>.lean`; the chunk theorems `seg<i> : run code k_i st<i>
    = some st<i+1>` in `Seg/C<i>.lean`, each importing only its two states (so
    they build in parallel); and `Seg/Join.lean`, the prefixes `upto<i> : run
    code (steps to i) st0 = some st<i>` joined by `run_trans`."""
    K, N = (int(x) for x in a.chain.split(':'))
    assert K % 1000 == 0 and N in tr.ckpt
    bounds = list(range(K, N, K)) + [N]
    (out / 'St').mkdir(exist_ok=True)
    (out / 'Seg').mkdir(exist_ok=True)
    pc0, regs0 = tr.ckpt[0]
    EMIT.emit(out / 'St' / 'S0.lean', 
        f'import {mod}.Mem0\n\n' + HEADER + f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
        f'/-- The evaluator state at reset. -/\n'
        f'def st0 : St := ⟨{pc0:#x}, {regs_nat(regs0):#x}, mem0⟩\n\nend {mod}\n')
    pk = dict(pages0)
    si = 0
    sizes = []
    prevk = 0
    for i, k in enumerate(bounds, start=1):
        touched = set()
        while tr.stores is not None and si < len(tr.stores) and tr.stores[si][0] < k:
            step, addr, w, data = tr.stores[si]
            for j in range(w):
                x = addr + j
                d, m = pk.get(x >> PS, (0, 0))
                off = x & (PB - 1)
                b = (data >> (8 * j)) & 0xff
                d = (d & ~(0xff << (8 * off))) | (b << (8 * off))
                pk[x >> PS] = (d, m | (1 << off))
                touched.add(x >> PS)
            si += 1
        if tr.stores is None:
            EMIT.hashed(out / 'St' / f'S{i}.lean', f'S{i}')
        else:
            changed = {p: pk[p] for p in touched}
            dt, defs = dt_text(f'st{i}_d', universe, changed)
            pc, regs = tr.ckpt[k]
            body = (f'import {mod}.St.S{i - 1}\n\n' + HEADER + f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
                    + '\n'.join(defs) +
                    f'\n/-- Step {k}: {len(changed)} pages written since step {prevk}. -/\n'
                    f'def st{i} : St := ⟨{pc:#x}, {regs_nat(regs):#x},\n st{i - 1}.mem.overlay {dt}⟩\n'
                    f'\nend {mod}\n')
            EMIT.emit(out / 'St' / f'S{i}.lean', body)
            sizes.append(len(body))
        EMIT.emit(out / 'Seg' / f'C{i - 1}.lean', 
            f'import {mod}.Code\nimport {mod}.St.S{i}\nimport Tcc.Ccp.KernelRfl\n\n' + HEADER +
            f'\nnamespace {mod}\nopen Tcc.Ccp\n\n'
            f'/-- Steps {prevk} to {k}. -/\n'
            f'theorem seg{i - 1} : run code {k - prevk} st{i - 1} = some st{i} := by\n'
            f'  kernel_rfl\n\nend {mod}\n')
        prevk = k
    n = len(bounds)
    lines = [f'import {mod}.Seg.C{i}' for i in range(n)] + ['import Tcc.Ccp.Lemmas', '', HEADER,
             f'namespace {mod}', 'open Tcc.Ccp', '',
             f'theorem upto1 : run code {bounds[0]} st0 = some st1 := seg0', '']
    for i in range(1, n):
        lines += [f'theorem upto{i + 1} : run code {bounds[i]} st0 = some st{i + 1} := by',
                  f'  have h := run_trans upto{i} seg{i}',
                  f'  rwa [show ({bounds[i - 1]} + {bounds[i] - bounds[i - 1]} : Nat) = {bounds[i]} from rfl] at h',
                  '']
    lines += [f'/-- The evaluator runs the {N} steps from reset without a fault to the',
              f'native trace\'s state at step {N}. -/',
              f'theorem run_all : run code {N} st0 = some st{n} := upto{n}', '', f'end {mod}', '']
    EMIT.emit(out / 'Seg' / 'Join.lean', '\n'.join(lines))
    if sizes:
        print(f'chain: {n} segments to step {N}; state sources {sum(sizes)} bytes '
              f'(max {max(sizes)})', file=sys.stderr)
    return [0] + bounds


if __name__ == '__main__':
    main()
