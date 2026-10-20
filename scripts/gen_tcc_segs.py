#!/usr/bin/env python3
"""Generate the replay of the program phase from the captured cut (PHASES 3b).

    scripts/gen_tcc_segs.py --trace build/p3/sum-cut/program.tsv   # write input + Lean
    scripts/gen_tcc_segs.py --check                                 # drift check

The program phase from `TccSumCut` is concrete: every register and every
loaded byte is in the trace (scripts/tcc_cut.py's program.tsv). This script
cuts the trace into dynamic basic blocks (a body of segment-layer
instructions ending at a branch, `j` or `jr`), emits one `#derive_case` per
static block and, per dynamic block, one row-to-row theorem `sim_B<k>` by
`Tcc.Boot.row_step` (every side condition a kernel `decide`, the
`ChainFacts` by `tcc_facts`), and chains them from the cut. It stops at the
first step the segment layer has no rule for (code at or above
`tohostAddr`, which is all of tcc's arena; a call `jal`/`jalr` with a link
register; `addi x0` (nop); a store below `tohostAddr + 16`; an opcode
outside `Vsa.Sim.decodeM`), and reports it.

Writes results/p3/sum-replay.json (the committed input: the replayed rows),
Tcc/Prog/Sum/Machine/Replay.lean (GENERATED) and the ledger
abstractions/ledger/p3-codegen-segment.tsv (each generated case's hand
cost: 0 lines).
"""
import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CUT = ROOT / 'results/p3/sum-cut.json'
INPUT = ROOT / 'results/p3/sum-replay.json'
OUT = ROOT / 'Tcc/Prog/Sum/Machine/Replay.lean'
LEDGER = ROOT / 'abstractions/ledger/p3-codegen-segment.tsv'
TOHOST = 0x8007cec0
BOP = {0: 'BEQ', 1: 'BNE', 4: 'BLT', 5: 'BGE', 6: 'BLTU', 7: 'BGEU'}


def fields(w):
    return dict(op=w & 0x7f, rd=(w >> 7) & 31, f3=(w >> 12) & 7, rs1=(w >> 15) & 31,
                rs2=(w >> 20) & 31, f7=(w >> 25) & 0x7f, f6=(w >> 26) & 0x3f)


def body_kind(w):
    """Vsa.Sim.decodeM's kind, or None (mirrors Vsa/Sim/BlockDecode.lean)."""
    f = fields(w)
    op, f3, f7, f6 = f['op'], f['f3'], f['f7'], f['f6']
    k = None
    if op == 0x13:
        k = {0: 'addi', 2: 'slti', 1: 'slli', 4: 'xori', 7: 'andi', 6: 'ori'}.get(f3)
        if f3 == 5:
            k = {0x00: 'srli', 0x10: 'srai'}.get(f6)
    elif op == 0x33:
        k = {(0, 0): 'add', (0, 0x20): 'sub', (6, 0): 'or', (7, 0): 'and', (5, 0): 'srl',
             (4, 0): 'xor', (1, 0): 'sll'}.get((f3, f7))
        if f3 == 2:
            k = 'slt'
    elif op == 0x03:
        k = {2: 'lw', 3: 'ld', 4: 'lbu', 1: 'lh', 5: 'lhu', 6: 'lwu'}.get(f3)
    elif op == 0x23:
        k = {2: 'sw', 3: 'sd', 0: 'sb', 1: 'sh'}.get(f3)
    elif op == 0x1b:
        k = {0: 'addiw', 1: 'slliw'}.get(f3)
        if f3 == 5:
            k = {0x00: 'srliw', 0x20: 'sraiw'}.get(f7)
    elif op == 0x3b:
        k = {(0, 0): 'addw', (0, 0x20): 'subw', (1, 0): 'sllw', (5, 0): 'srlw',
             (5, 0x20): 'sraw'}.get((f3, f7))
    elif op == 0x17:
        k = 'auipc'
    elif op == 0x37:
        k = 'lui'
    if k and k not in ('sw', 'sd', 'sb', 'sh') and f['rd'] == 0:
        return None  # KindOK needs 1 ≤ rd (a nop has no rule)
    return k


def term_kind(w):
    f = fields(w)
    if f['op'] == 0x63 and f['f3'] in BOP:
        return 'br'
    if f['op'] == 0x6f and f['rd'] == 0:
        return 'j'
    if f['op'] == 0x67 and f['rd'] == 0 and f['f3'] == 0:
        return 'jr'
    return None


def term_lit(pc, w, kind, taken):
    b = ', '.join(f'0x{(w >> (8 * i)) & 0xff:02x}#8' for i in range(4))
    rs1, rs2 = (w >> 15) & 31, (w >> 20) & 31
    if kind == 'br':
        imm = ((w >> 31) & 1) << 12 | ((w >> 7) & 1) << 11 | ((w >> 25) & 0x3f) << 5 | ((w >> 8) & 0xf) << 1
        return (f'⟨0x{pc:x}#64, 0x{w:08x}#32, {b}, .br bop.{BOP[(w >> 12) & 7]} '
                f'{"true" if taken else "false"}, {rs1}, {rs2}, 0x{imm:03x}#13, 0#21, 0#12⟩')
    if kind == 'j':
        imm = ((w >> 31) & 1) << 20 | ((w >> 12) & 0xff) << 12 | ((w >> 20) & 1) << 11 | ((w >> 21) & 0x3ff) << 1
        return f'⟨0x{pc:x}#64, 0x{w:08x}#32, {b}, .j, 0, 0, 0#13, 0x{imm:06x}#21, 0#12⟩'
    return f'⟨0x{pc:x}#64, 0x{w:08x}#32, {b}, .jr, {rs1}, 0, 0#13, 0#21, 0x{(w >> 20) & 0xfff:03x}#12⟩'


def load_view():
    cut = json.loads(CUT.read_text())
    mem = {}
    for base, hx, _ in cut['view']:
        b = int(base, 16)
        for i, x in enumerate(bytes.fromhex(hx)):
            mem[b + i] = x
    return cut, mem


def word_at(mem, pc):
    return int.from_bytes(bytes(mem[pc + j] for j in range(4)), 'little')


def capture(trace):
    """Cut the trace into dynamic blocks; stop at the first unsupported step."""
    _, mem = load_view()
    rows = []
    for line in Path(trace).read_text().splitlines():
        f = line.split('\t')
        regs = [int(x, 16) for x in f[4:35]]
        m = None
        if f[35] != 'O':
            m = [f[35][0], int(f[35][1:]), int(f[36], 16), int(f[37], 16), int(f[38], 16)]
        rows.append({'pc': int(f[2], 16), 'npc': int(f[3], 16), 'regs': regs, 'mem': m})
    blocks = []
    cur = None
    stop = None
    for r in rows:
        w = word_at(mem, r['pc'])
        if cur is None:
            cur = {'pc': r['pc'], 'regs': r['regs'], 'body': [], 'lds': [], 'stores': []}
        tk = term_kind(w)
        bk = body_kind(w)
        reason = 'no rule for this instruction'
        if bk and r['mem'] and r['mem'][0] == 'S' and r['mem'][2] < TOHOST + 16:
            bk, reason = None, 'a store below tohostAddr + 16 (the HTIF window, or code)'
        if r['pc'] + 4 > TOHOST:
            tk, bk = None, None
            reason = ('code above tohostAddr: the segment layer places code below the HTIF '
                      'mailbox and every store above it (InstrOKM), but tcc runs P from its '
                      'arena at 0x86400000')
        if tk:
            taken = r['npc'] != r['pc'] + 4
            cur['term'] = [r['pc'], w, tk, taken]
            cur['end'] = r['npc']
            blocks.append(cur)
            cur = None
        elif bk:
            cur['body'].append([r['pc'], w])
            if r['mem']:
                kind, width, addr, pre, post = r['mem']
                if kind == 'L':
                    cur['lds'].append([(pre >> (8 * j)) & 0xff for j in range(width)])
                else:
                    rs2 = (w >> 20) & 31
                    cur['stores'].append([addr, width, r['regs'][rs2 - 1] if rs2 else 0])
        else:
            stop = {'pc': r['pc'], 'word': w, 'reason': reason}
            if cur['body']:
                cur['term'] = None
                cur['end'] = r['pc']
                blocks.append(cur)
            break
    end = {'pc': stop['pc'], 'regs': rows[[r['pc'] for r in rows].index(stop['pc'])]['regs']} if stop else None
    # the registers after the last block: the row at the stop
    idx = sum(len(b['body']) + (1 if b['term'] else 0) for b in blocks)
    inp = {'blocks': blocks, 'stop': stop, 'stop_regs': rows[idx]['regs'],
           'steps': idx, 'program_steps': len(rows)}
    INPUT.write_text(json.dumps(inp) + '\n')
    return inp


def pins_lit(regs):
    return '[' + ', '.join(f'({i + 1}, 0x{v:x}#64)' for i, v in enumerate(regs)) + ']'


def log_lit(log):
    return '[' + ', '.join(f'(0x{a:x}, {w}, 0x{d:x}#64)' for a, w, d in log) + ']'


def lds_lit(lds):
    return '[' + ', '.join('[' + ', '.join(f'0x{b:02x}#8' for b in l) + ']' for l in lds) + ']'


def render(inp):
    blocks = inp['blocks']
    static = {}
    derive = []
    for b in blocks:
        t = b['term']
        key = (b['pc'], len(b['body']), tuple(t[2:]) if t else None)
        if key in static:
            continue
        name = f'blk_{b["pc"]:x}_{len(static)}'
        static[key] = name
        body = ',\n   '.join(f'(0x{pc:x}#64, 0x{w:08x}#32)' for pc, w in b['body'])
        txt = f'#derive_case {name} chain\n  [{body}]'
        if t:
            txt += f'\n  terminator {term_lit(t[0], t[1], t[2], t[3])}'
        derive.append(txt)
    sims = []
    log = []
    rows = []  # (pc, pins term, log term)
    for k, b in enumerate(blocks):
        rows.append((b['pc'], 'cutPins Tcc.Capture.Sum.regs' if k == 0 else f'pins{k}', f'log{k}'))
    nxt_regs = [blocks[k + 1]['regs'] for k in range(len(blocks) - 1)] + [inp['stop_regs']]
    defs = []
    for k, b in enumerate(blocks):
        defs.append(f'def log{k} : List WEntry := {log_lit(log)}')
        if k:
            defs.append(f'def pins{k} : GRegs := {pins_lit(b["regs"])}')
        log = log + b['stores']
    n = len(blocks)
    defs.append(f'def log{n} : List WEntry := {log_lit(log)}')
    defs.append(f'def pins{n} : GRegs := {pins_lit(inp["stop_regs"])}')
    rows.append((blocks[-1]['end'], f'pins{n}', f'log{n}'))
    for k, b in enumerate(blocks):
        t = b['term']
        name = static[(b['pc'], len(b['body']), tuple(t[2:]) if t else None)]
        (pc0, L0, g0), (pc1, L1, g1) = rows[k], rows[k + 1]
        sims.append(f'''/-- Block {k}: `0x{pc0:x}` to `0x{pc1:x}` ({len(b['body']) + (1 if t else 0)} steps). -/
theorem sim_B{k} : ∀ c, RunRow view 0x{pc0:x}#64 ({L0}) {g0} c →
    ∃ c', Steps c c' ∧ RunRow view 0x{pc1:x}#64 {L1} {g1} c' :=
  row_step {name} {lds_lit(b['lds'])} (by decide +kernel) (fun _ hv => by tcc_facts hv)
    (by decide +kernel) (by decide +kernel) (by decide +kernel) (by decide +kernel)''')
    chain = 'sim_B0'
    for k in range(1, n):
        chain = f'RunRow.trans ({chain}) sim_B{k}'
    stop = inp['stop']
    stop_txt = (f'the instruction `0x{stop["word"]:08x}` at `0x{stop["pc"]:x}` ({stop["reason"]})'
                if stop else 'the end of the trace')
    steps = inp['steps']
    return f'''import Tcc.Boot.Run
import Tcc.Capture.Sum.Cut
import Vsa.Sim.DeriveCase

/-! GENERATED by scripts/gen_tcc_segs.py from results/p3/sum-replay.json
(scripts/tcc_cut.py's program-phase trace). Do not edit: change the generator.

The replay of the program phase from the captured cut, one `#derive_case`
per static block and one `sim_B<k>` per dynamic block, chained: the first
{steps} of the {inp['program_steps']} steps, up to {stop_txt}. -/

namespace Tcc.Prog.Sum.Machine

open LeanRV64DExecutable Vsa Vsa.Machine Vsa.Sim Tcc.Boot Tcc.Capture.Sum

{chr(10).join(defs)}

{(chr(10) * 2).join(derive)}

{(chr(10) * 2).join(sims)}

/-- The replayed prefix: from the captured cut, {steps} steps to `0x{rows[-1][0]:x}`. -/
theorem replay : ∀ c, TccSumCut c →
    ∃ c', Steps c c' ∧ RunRow view 0x{rows[-1][0]:x}#64 pins{n} log{n} c' :=
  fun c h => ({chain}) c h.row

end Tcc.Prog.Sum.Machine
'''


def ledger(inp):
    return ('# case\thand_lines\tsource  (scripts/gen_tcc_segs.py: generated, no hand lines)\n' +
            ''.join(f'B{k}\t0\tTcc/Prog/Sum/Machine/Replay.lean\n' for k in range(len(inp['blocks']))))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--trace')
    ap.add_argument('--check', action='store_true')
    a = ap.parse_args()
    inp = capture(a.trace) if a.trace else json.loads(INPUT.read_text())
    files = {OUT: render(inp), LEDGER: ledger(inp)}
    if a.check:
        bad = [str(p.relative_to(ROOT)) for p, t in files.items() if not p.exists() or p.read_text() != t]
        if bad:
            sys.exit('gen_tcc_segs: drift in ' + ', '.join(bad))
        print('gen_tcc_segs: up to date')
        return
    for p, t in files.items():
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(t)
    stop = inp['stop']
    print(f'gen_tcc_segs: {len(inp["blocks"])} blocks, {inp["steps"]} of {inp["program_steps"]} steps;'
          + (f' stopped at 0x{stop["pc"]:x} (0x{stop["word"]:08x}): {stop["reason"]}' if stop else ''))


if __name__ == '__main__':
    main()
