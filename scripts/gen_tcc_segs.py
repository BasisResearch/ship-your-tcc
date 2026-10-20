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
OUT = ROOT / 'Tcc/Prog/Sum/Machine'
PER_PART = 40
CHUNK_MAX = 6  # instructions per block item
PIN_MAX = 28   # byte pins (4 per fetch, the width per load) per block item
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


def rule_kind(w):
    """One-instruction rules of Tcc/Boot/Step.lean."""
    f = fields(w)
    if f['op'] == 0x67 and f['f3'] == 0 and f['rd'] != 0:
        return 'jalr'
    if f['op'] == 0x6f and f['rd'] != 0:
        return 'jal'
    if w == 0x00000013:
        return 'nop'
    if f['op'] == 0x33 and f['f3'] == 3 and f['f7'] == 0 and f['rd'] != 0:
        return 'sltu'
    return None


def capture(trace):
    """Cut the trace into items (blocks and one-instruction rules); stop at the
    first step with neither."""
    _, mem = load_view()
    rows = []
    for line in Path(trace).read_text().splitlines():
        f = line.split('\t')
        regs = [int(x, 16) for x in f[4:35]]
        m = None
        if f[35] != 'O':
            m = [f[35][0], int(f[35][1:]), int(f[36], 16), int(f[37], 16), int(f[38], 16)]
        oi = f.index('O')
        rows.append({'pc': int(f[2], 16), 'npc': int(f[3], 16), 'regs': regs, 'mem': m,
                     'outb': int(f[oi + 3])})
    items = []
    cur = None
    stop = None

    def close(end_pc, term=None):
        nonlocal cur
        if cur is not None and (cur['body'] or term):
            cur['term'] = term
            cur['end'] = end_pc
            items.append(cur)
        cur = None

    for r in rows:
        w = word_at(mem, r['pc'])
        tk, bk, rk = term_kind(w), body_kind(w), rule_kind(w)
        reason = 'no rule for this instruction'
        if bk and r['mem'] and r['mem'][0] == 'S' and TOHOST - 8 < r['mem'][2] < TOHOST + 16:
            bk, reason = None, 'a store into the HTIF mailbox'
            kind_, width, addr, pre, post = r['mem']
            rs2 = (w >> 20) & 31
            data = r['regs'][rs2 - 1] if rs2 else 0
            if addr == TOHOST and width == 8 and data >> 48 == 0x0101:
                rk, r['data'] = 'putchar', data
            elif addr == TOHOST and width == 8 and data & 1 and data >> 47 == 0:
                rk, r['data'] = 'exit', data
        if rk:
            close(r['pc'])
            items.append({'kind': rk, 'pc': r['pc'], 'regs': r['regs'], 'word': w,
                          'end': r['npc'], 'stores': [], 'outb': r['outb'],
                          'data': r.get('data', 0)})
            if rk == 'exit':
                break
            continue
        if cur is None:
            cur = {'kind': 'block', 'pc': r['pc'], 'regs': r['regs'], 'body': [], 'lds': [],
                   'stores': [], 'outb': 256}
        if tk:
            close(r['npc'], [r['pc'], w, tk, r['npc'] != r['pc'] + 4])
        elif bk:
            cost = 4 + (r['mem'][1] if r['mem'] and r['mem'][0] == 'L' else 0)
            if len(cur['body']) == CHUNK_MAX or cur.get('cost', 0) + cost > PIN_MAX:
                # a long body is several items: per-declaration work stays bounded
                close(r['pc'])
                cur = {'kind': 'block', 'pc': r['pc'], 'regs': r['regs'], 'body': [], 'lds': [],
                       'stores': [], 'outb': 256}
            cur['body'].append([r['pc'], w])
            cur['cost'] = cur.get('cost', 0) + cost
            if r['mem']:
                kind, width, addr, pre, post = r['mem']
                if kind == 'L':
                    cur['lds'].append([(pre >> (8 * j)) & 0xff for j in range(width)])
                else:
                    rs2 = (w >> 20) & 31
                    cur['stores'].append([addr, width, r['regs'][rs2 - 1] if rs2 else 0])
        else:
            stop = {'pc': r['pc'], 'word': w, 'reason': reason}
            close(r['pc'])
            break
    steps = sum(len(i['body']) + (1 if i['term'] else 0) if i['kind'] == 'block' else 1
                for i in items)
    halted = bool(items) and items[-1]['kind'] == 'exit'
    inp = {'items': items, 'stop': stop, 'halted': halted,
           'stop_regs': rows[steps]['regs'] if steps < len(rows) else None,
           'steps': steps, 'program_steps': len(rows)}
    INPUT.write_text(json.dumps(inp) + '\n')
    return inp


def pins_lit(regs):
    return '[' + ', '.join(f'({i + 1}, 0x{v:x}#64)' for i, v in enumerate(regs)) + ']'


def entries_lit(log):
    return '[' + ', '.join(f'(0x{a:x}, {w}, 0x{d:x}#64)' for a, w, d in log) + ']'


def lds_lit(lds):
    return '[' + ', '.join('[' + ', '.join(f'0x{b:02x}#8' for b in l) + ']' for l in lds) + ']'


def hdr(text, imports):
    return (''.join(f'import {m}\n' for m in imports) +
            '\n/-! GENERATED by scripts/gen_tcc_segs.py from results/p3/sum-replay.json\n'
            "(scripts/tcc_cut.py's program-phase trace). Do not edit: change the generator.\n\n"
            f'{text} -/\n\n')


NS = 'Tcc.Prog.Sum.Machine'
OPENS = 'open LeanRV64DExecutable Vsa Vsa.Machine Vsa.Sim Tcc.Boot Tcc.Capture.Sum'
DK = 'by decide +kernel'
INSTR_AT = '⟨' + ', '.join([DK] * 9) + ', fun s h1 h2 h3 => Vsa.Sim.decodeW s h1 h2 h3⟩'


def esc(s):
    return s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')


def render(inp):
    items = inp['items']
    exit_it = items[-1] if inp.get('halted') else None
    if exit_it:
        items = items[:-1]
    n = len(items)
    files = {}
    rows_pc = [it['pc'] for it in items] + [exit_it['pc'] if exit_it else items[-1]['end']]
    rows_regs = [it['regs'] for it in items] + [exit_it['regs'] if exit_it else inp['stop_regs']]
    outs = ['']
    for it in items:
        o = outs[-1]
        if it['kind'] == 'putchar':
            o = o + chr(it['data'] & 0xff)
        outs.append(o)
    # defs: pins and logs; a row whose pins (a nop) or log (no store) did not
    # change reuses the previous row's name, so consecutive rows unify syntactically
    defs = []
    pname = ['(cutPins Tcc.Capture.Sum.regs)']
    for k in range(1, n + 1):
        if items[k - 1]['kind'] in ('nop', 'putchar'):
            pname.append(pname[-1])
        else:
            defs.append(f'@[irreducible] def pins{k} : GRegs := {pins_lit(rows_regs[k])}')
            pname.append(f'pins{k}')
    lname = ['([] : List WEntry)']
    for k, it in enumerate(items):
        if it['stores']:
            # newest first: this item's stores, last store first, before the older log
            defs.append(f'@[irreducible] def log{k + 1} : List WEntry := '
                        f'{entries_lit(it["stores"][::-1])} ++ {lname[-1]}')
            lname.append(f'log{k + 1}')
        else:
            lname.append(lname[-1])
    files['Defs.lean'] = (hdr('The replay rows: pin lists and write logs, irreducible so that '
                              'unification never unfolds them (the kernel `decide`s do).', ['Tcc.Boot.Run', 'Tcc.Capture.Sum.View']) +
                          f'namespace {NS}\n\n{OPENS}\n\n' + '\n'.join(defs) + f'\n\nend {NS}\n')
    # static blocks
    static = {}
    derive = []
    for it in items:
        if it['kind'] != 'block':
            continue
        t = it['term']
        key = (it['pc'], len(it['body']), tuple(t[2:]) if t else None)
        if key in static:
            continue
        name = f'blk_{it["pc"]:x}_{len(static)}'
        static[key] = name
        body = ',\n   '.join(f'(0x{pc:x}#64, 0x{w:08x}#32)' for pc, w in it['body'])
        txt = f'#derive_case {name} chain\n  [{body}]'
        if t:
            txt += f'\n  terminator {term_lit(t[0], t[1], t[2], t[3])}'
        derive.append(txt)
    files['Blocks.lean'] = (
        hdr('One `#derive_case` per static block of the replay.',
            ['Tcc.Boot.Run', 'Vsa.Sim.DeriveCase', 'Tcc.Capture.Sum.View']) +
        '-- discipline: allow(R9-handrolled-fn-assembly) generated, one #derive_case per static block\n' +
        f'namespace {NS}\n\n{OPENS}\n\n' + '\n\n'.join(derive) + f'\n\nend {NS}\n')

    def row(k):
        return f'RunRow view 0x{rows_pc[k]:x}#64 {pname[k]} {lname[k]} "{esc(outs[k])}"'

    sims = []
    for k, it in enumerate(items):
        if it['kind'] == 'block':
            t = it['term']
            name = static[(it['pc'], len(it['body']), tuple(t[2:]) if t else None)]
            steps = len(it['body']) + (1 if t else 0)
            proof = (f'row_step {name} {lds_lit(it["lds"])} ({DK}) '
                     f'(fun _ hv => by tcc_facts hv)\n    ({DK}) ({DK}) ({DK}) ({DK})')
            what = f'block `{name}`'
        elif it['kind'] == 'jalr':
            w = it['word']
            rd, rs1, imm = (w >> 7) & 31, (w >> 15) & 31, (w >> 20) & 0xfff
            steps = 1
            proof = (f'row_jalr (w := 0x{w:08x}#32) (imm := 0x{imm:03x}#12) (rd := {rd}) (rs1 := {rs1})\n'
                     f'    {INSTR_AT}\n'
                     f'    (by decide) ({DK}) ({DK}) ({DK}) ({DK}) ({DK})')
            what = f'call `jalr x{rd}, {imm}(x{rs1})`'
        elif it['kind'] == 'jal':
            w = it['word']
            rd = (w >> 7) & 31
            imm = ((w >> 31) & 1) << 20 | ((w >> 12) & 0xff) << 12 | ((w >> 20) & 1) << 11 | ((w >> 21) & 0x3ff) << 1
            steps = 1
            proof = (f'row_jal (w := 0x{w:08x}#32) (imm := 0x{imm:06x}#21) (rd := {rd})\n'
                     f'    {INSTR_AT}\n'
                     f'    (by decide) ({DK}) ({DK}) ({DK}) ({DK})')
            what = f'call `jal x{rd}, {imm}`'
        elif it['kind'] == 'putchar':
            w = it['word']
            rs1, rs2 = (w >> 15) & 31, (w >> 20) & 31
            imm = ((w >> 25) << 5) | ((w >> 7) & 31)
            steps = 1
            proof = (f'row_putchar (w := 0x{w:08x}#32) (imm := 0x{imm:03x}#12) (rs1 := {rs1}) '
                     f'(rs2 := {rs2}) (c := 0x{it["data"] & 0xff:02x}#8)\n'
                     f'    {INSTR_AT}\n'
                     f'    ({DK}) ({DK}) ({DK}) ({DK}) ({DK})')
            what = f'console byte `{it["data"] & 0xff:#04x}` to `tohost`'
        elif it['kind'] == 'nop':
            steps = 1
            proof = f'row_nop (w := 0x00000013#32)\n    {INSTR_AT}\n    ({DK}) ({DK}) ({DK})'
            what = 'nop'
        elif it['kind'] == 'sltu':
            w = it['word']
            rd, rs1, rs2 = (w >> 7) & 31, (w >> 15) & 31, (w >> 20) & 31
            steps = 1
            proof = (f'row_sltu (w := 0x{w:08x}#32) (rd := {rd}) (rs1 := {rs1}) (rs2 := {rs2})\n'
                     f'    {INSTR_AT}\n'
                     f'    (by decide) ({DK}) ({DK}) ({DK}) ({DK})')
            what = f'`sltu x{rd}, x{rs1}, x{rs2}`'
        sims.append((k, f'/-- Item {k}: {what}, `0x{rows_pc[k]:x}` to `0x{rows_pc[k + 1]:x}` '
                        f'({steps} steps). -/\n'
                        f'theorem sim_B{k} : Leads ({row(k)}) ({row(k + 1)}) :=\n  {proof}'))
    parts = [sims[i:i + PER_PART] for i in range(0, len(sims), PER_PART)]
    part_thms = []
    for p, ps in enumerate(parts):
        k0, k1 = ps[0][0], ps[-1][0] + 1
        chain = f'sim_B{k0}'
        for k in range(k0 + 1, k1):
            chain = f'Leads.trans ({chain}) sim_B{k}'
        body = '\n\n'.join(t for _, t in ps)
        files[f'Part{p}.lean'] = (
            hdr(f'Replay items {k0}..{k1 - 1}.',
                [f'{NS}.Defs', f'{NS}.Blocks', 'Tcc.Boot.Step', 'Tcc.Capture.Sum.View', 'Tcc.Capture.Sum.Regs']) +
            f'namespace {NS}\n\n{OPENS}\n\n{body}\n\n'
            f'theorem part{p} : Leads ({row(k0)}) ({row(k1)}) :=\n  {chain}\n\nend {NS}\n')
        part_thms.append(f'part{p}')
    chain = part_thms[0]
    for t in part_thms[1:]:
        chain = f'Leads.trans ({chain}) {t}'
    stop = inp['stop']
    stop_txt = (f'the instruction `0x{stop["word"]:08x}` at `0x{stop["pc"]:x}` ({stop["reason"]})'
                if stop else 'the end of the trace')
    files['Replay.lean'] = (
        hdr(f'The replay of the program phase from the captured cut: {n} items (blocks by '
            f'`row_step`, one-instruction rules of `Tcc/Boot/Step.lean`), the first '
            f'{inp["steps"]} of the {inp["program_steps"]} steps, up to {stop_txt}.',
            [f'{NS}.Part{p}' for p in range(len(parts))] + ['Tcc.Capture.Sum.Cut']) +
        f'namespace {NS}\n\n{OPENS}\n\n'
        f'/-- The replayed prefix: from the captured cut, {inp["steps"]} steps to '
        f'`0x{rows_pc[n]:x}`. -/\n'
        f'theorem replay : Leads TccSumCut ({row(n)}) :=\n  fun c h => ({chain}) c h.row\n\n'
        + (exit_txt(exit_it, row(n), outs[n]) if exit_it else '') +
        f'end {NS}\n')
    return files


def exit_txt(it, row_n, out):
    w = it['word']
    rs1, rs2 = (w >> 15) & 31, (w >> 20) & 31
    imm = ((w >> 25) << 5) | ((w >> 7) & 31)
    e = it['data'] >> 1
    return (f'/-- The exit store at `0x{it["pc"]:x}`: `tohost := ({e} <<< 1) ||| 1`. -/\n'
            f'theorem sim_exit : ∀ c, ({row_n}) c → Halts c "{esc(out)}" (0x{e:x}#64).toNat :=\n'
            f'  row_exit (w := 0x{w:08x}#32) (imm := 0x{imm:03x}#12) (rs1 := {rs1}) (rs2 := {rs2})\n'
            f'    {INSTR_AT}\n'
            f'    ({DK}) ({DK}) ({DK}) ({DK}) ({DK})\n\n'
            f'/-- **The program phase from the captured cut**: `tcc -run sum.c` prints '
            f'`{esc(out)}` and exits with status {e}. -/\n'
            f'theorem sum_halts : SumHalts_Statement := by\n'
            f'  intro c h\n'
            f'  obtain ⟨c1, hs1, hr⟩ := replay c h\n'
            f'  obtain ⟨c2, σf, hs2, hh, ho⟩ := sim_exit c1 hr\n'
            f'  exact ⟨c2, σf, Steps.trans hs1 hs2, hh, ho⟩\n\n')


def ledger(inp):
    return ('# case\thand_lines\tsource  (scripts/gen_tcc_segs.py: generated, no hand lines)\n' +
            ''.join(f'B{k}\t0\tTcc/Prog/Sum/Machine/Part{k // PER_PART}.lean\n'
                    for k in range(len(inp['items']))))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--trace')
    ap.add_argument('--check', action='store_true')
    a = ap.parse_args()
    inp = capture(a.trace) if a.trace else json.loads(INPUT.read_text())
    files = {OUT / k: v for k, v in render(inp).items()}
    files[LEDGER] = ledger(inp)
    stale = [p for p in OUT.glob('*.lean') if p not in files]
    if a.check:
        bad = [str(p.relative_to(ROOT)) for p, t in files.items()
               if not p.exists() or p.read_text() != t]
        bad += [str(p.relative_to(ROOT)) + ' (stale)' for p in stale]
        if bad:
            sys.exit('gen_tcc_segs: drift in ' + ', '.join(bad))
        print('gen_tcc_segs: up to date')
        return
    for p in stale:
        p.unlink()
    for p, t in files.items():
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(t)
    stop = inp['stop']
    print(f'gen_tcc_segs: {len(inp["items"])} items, {inp["steps"]} of {inp["program_steps"]} steps;'
          + (f' stopped at 0x{stop["pc"]:x} (0x{stop["word"]:08x}): {stop["reason"]}' if stop else ''))


if __name__ == '__main__':
    main()
