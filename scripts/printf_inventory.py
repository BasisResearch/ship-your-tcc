#!/usr/bin/env python3
"""Feasibility inventory for PHASES `PrintfSpec_Statement` (printf's machine spec).

    scripts/printf_inventory.py --arena build/p3/sum-arena/arena.tsv \
        [--replay results/p3/sum-replay.json] [--out results/p3/printf-inventory.json]

Inputs: the code arena of `tcc -run sum.c` at the cut (`scripts/tcc_cut.py
--arena`: every byte the compile phase stored in 0x86400000..+4 MiB), the
program phase's replay trace, the toolchain's rv64i/lp64 `libc.a` and
`libgcc.a`, and (read only) ship-your-lua's ELF.

What it checks and prints (all observations, no proof claims):

* placement: every function of `libc.a`/`libgcc.a` whose instruction words
  occur in the arena (relocated fields masked: an instruction a relocation
  applies to is compared on its opcode bits only), with its address and size;
  every arena function must match its archive bytes on every unrelocated
  instruction, so tcc's relocated code differs from the archive only in
  relocated immediates;
* the printf closure: the functions reachable from `printf` through the
  archive's call relocations (direct calls), and the indirect calls the
  replay observed (stdio's function pointers);
* execution: how many distinct instructions of each function P's one
  printf call executes;
* reuse: for each closure function, its size in ship-your-lua's ELF; GNU ld
  relaxes `call` (auipc+jalr) to `jal` there, tcc does not, so a size
  difference means no instruction-identical copy (and no reusable at-lemma).
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

ROOT = Path(__file__).resolve().parent.parent
XB = Path.home() / 'toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/bin'
LIBDIR = Path.home() / 'toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/riscv-none-elf/lib/rv64i/lp64'
LIBGCC = Path.home() / 'toolchains/xpack-riscv-none-elf-gcc-15.2.0-1/lib/gcc/riscv-none-elf/15.2.0/rv64i/lp64/libgcc.a'
LUA_ELF = Path.home() / 'Documents/code/ship-your-lua/c/lua-riscv-htif.elf'
ARENA = 0x86400000


def tool(name):
    return str(XB / f'riscv-none-elf-{name}')


def run(args, cwd=None):
    return subprocess.run(args, check=True, capture_output=True, text=True, cwd=cwd).stdout


def parse_objdump(path):
    """Functions of an object: name -> list of (word, relocated, reloc_symbol_or_None)."""
    out = run([tool('objdump'), '-d', '-r', str(path)])
    funcs = collections.OrderedDict()
    cur = None
    for line in out.splitlines():
        m = re.match(r'^[0-9a-f]+ <(.*)>:', line)
        if m:
            if not m.group(1).startswith('.L'):
                cur = m.group(1)
                funcs[cur] = []
            continue
        if cur is None:
            continue
        m = re.match(r'^\s+([0-9a-f]+):\s+([0-9a-f]{8})\s', line)
        if m:
            # word, relocated, call target, jalr of a call pair, relocations
            funcs[cur].append([int(m.group(2), 16), False, None, False, []])
            continue
        m = re.match(r'^\s+([0-9a-f]+):\s+(R_RISCV_\S+)\s+(\S+)', line)
        if m and funcs[cur]:
            kind, sym = m.group(2), m.group(3)
            if kind in ('R_RISCV_RELAX',):
                continue
            funcs[cur][-1][1] = True
            funcs[cur][-1][4].append((kind, sym))
            if kind in ('R_RISCV_CALL', 'R_RISCV_CALL_PLT'):
                funcs[cur][-1][2] = sym.split('+')[0]
                # the jalr of the pair is relocated too
                funcs[cur][-1][3] = True
    return funcs


def extract(archive, into):
    os.makedirs(into, exist_ok=True)
    run([tool('ar'), 'x', str(archive)], cwd=into)
    return sorted(Path(into).glob('*.o'))


def load_arena(path):
    """`scripts/tcc_cut.py --arena`'s runs: `base<TAB>hex bytes`."""
    mem = {}
    for line in open(path):
        if line.startswith('#'):
            continue
        a, b = line.split()
        base = int(a, 16)
        for j, x in enumerate(bytes.fromhex(b)):
            mem[base + j] = x
    return mem


def word_at(mem, a):
    bs = [mem.get(a + j) for j in range(4)]
    if None in bs:
        return None
    return bs[0] | bs[1] << 8 | bs[2] << 16 | bs[3] << 24


def place(arena_path):
    """Words of the arena, archive functions, and their placement in the arena."""
    mem = load_arena(arena_path)
    words = {}
    for addr in range(ARENA, ARENA + (4 << 20), 4):
        w = word_at(mem, addr)
        if w is not None:
            words[addr] = w
    by_word = collections.defaultdict(list)
    for addr, w in words.items():
        by_word[w].append(addr)

    tmp = tempfile.mkdtemp(prefix='printf_inv_')
    objs = extract(LIBDIR / 'libc.a', os.path.join(tmp, 'libc')) + \
        extract(LIBGCC, os.path.join(tmp, 'libgcc'))
    funcs = {}
    for o in objs:
        for name, body in parse_objdump(o).items():
            if body:
                funcs.setdefault(name, (o.name, body))

    # placement: anchor on the first unrelocated word, verify the whole body
    placed = {}
    for name, (obj, body) in funcs.items():
        anchor = next((i for i, x in enumerate(body) if not x[1]), None)
        if anchor is None:
            continue
        for cand in by_word.get(body[anchor][0], []):
            start = cand - 4 * anchor
            ok = True
            for i, x in enumerate(body):
                w = words.get(start + 4 * i)
                if w is None:
                    ok = False
                    break
                relocated = x[1] or (i > 0 and body[i - 1][3])
                if relocated:
                    if (w & 0x7f) != (x[0] & 0x7f):
                        ok = False
                        break
                elif w != x[0]:
                    ok = False
                    break
            if ok:
                placed.setdefault(name, []).append(start)
    # a function placed at several addresses is ambiguous (short stubs); at
    # one address the longest body wins, equal bodies are aliases
    at = collections.defaultdict(list)
    for n, v in placed.items():
        if len(v) == 1:
            at[v[0]].append(n)
    placement, aliases = {}, {}
    for addr, names in at.items():
        best = max(len(funcs[n][1]) for n in names)
        keep = sorted(n for n in names if len(funcs[n][1]) == best)
        if best < 8 and len(keep) > 1:
            continue  # a short stub many functions share: not identifiable by bytes
        main = 'printf' if 'printf' in keep else keep[0]
        placement[main] = (addr, funcs[main][0], best)
        aliases[main] = [n for n in keep if n != main]
    return mem, words, funcs, placement, aliases, tmp


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--arena', required=True)
    ap.add_argument('--replay', default=str(ROOT / 'results/p3/sum-replay.json'))
    ap.add_argument('--out', default=None)
    ap.add_argument('--all', action='store_true', help='list functions P does not execute too')
    a = ap.parse_args()
    mem, words, funcs, placement, aliases, tmp = place(a.arena)

    # closure from printf over direct calls
    calls = {n: sorted({x[2] for x in body if x[2]}) for n, (_, body) in funcs.items()}
    closure, todo = set(), ['printf']
    while todo:
        f = todo.pop()
        if f in closure or f not in funcs:
            continue
        closure.add(f)
        todo.extend(calls.get(f, []))

    # executed instructions of P's printf call (replay items 52 .. 633)
    rep = json.load(open(a.replay))
    items = rep['items']
    start = next(k for k, it in enumerate(items) if it['pc'] == placement.get('printf', (0,))[0])
    ret = items[start]['regs'][0]  # ra at printf's entry
    end = next(k for k in range(start + 1, len(items)) if items[k]['pc'] == ret)
    executed = set()
    for it in items[start:end]:
        if it['kind'] == 'block':
            executed.update(pc for pc, _ in it['body'])
            if it.get('term'):
                executed.add(it['term'][0])
        else:
            executed.add(it['pc'])
    host_exec = sorted(pc for pc in executed if pc < ARENA)
    steps = sum(it.get('cost', 1) for it in items[start:end])

    lua, lua_addr = {}, {}
    for line in run([tool('nm'), '-S', str(LUA_ELF)]).splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in 'tT':
            lua.setdefault(p[3], int(p[1], 16) // 4)
            lua_addr.setdefault(p[3], int(p[0], 16))
    lua_text = {}
    for line in run([tool('objdump'), '-d', str(LUA_ELF)]).splitlines():
        m = re.match(r'^\s+([0-9a-f]+):\s+([0-9a-f]{8})\s', line)
        if m:
            lua_text[int(m.group(1), 16)] = int(m.group(2), 16)

    def lua_identical(name, addr, n):
        # the same instruction words, word for word (a relocated immediate
        # differs between the images unless the displacement coincides)
        if lua.get(name) != n:
            return False
        la = lua_addr[name]
        return all(lua_text.get(la + 4 * i) == words.get(addr + 4 * i) for i in range(n))

    rows = []
    for name, (addr, obj, n) in sorted(placement.items(), key=lambda kv: kv[1][0]):
        ex = sum(1 for i in range(n) if addr + 4 * i in executed)
        rows.append({'name': name, 'aliases': aliases.get(name, []), 'addr': addr, 'obj': obj,
                     'insns': n,
                     'executed_by_P_call': ex, 'in_closure': name in closure,
                     'lua_insns': lua.get(name), 'lua_identical': lua_identical(name, addr, n)})
    arena_exec = sorted(pc for pc in executed if pc >= ARENA)
    covered = set()
    for r in rows:
        covered.update(range(r['addr'], r['addr'] + 4 * r['insns'], 4))
    unmatched = [pc for pc in arena_exec if pc not in covered]
    # unmatched executed pcs as ranges
    ranges = []
    for pc in unmatched:
        if ranges and pc - ranges[-1][1] <= 64:
            ranges[-1][1] = pc
        else:
            ranges.append([pc, pc])

    report = {
        'printf_call_items': [start, end], 'printf_call_cost_units': steps,
        'executed_distinct_insns': len(executed), 'executed_host_insns': len(host_exec),
        'executed_arena_unmatched_ranges': [[hex(x), hex(y)] for x, y in ranges],
        'closure': sorted(closure),
        'closure_insns': sum(len(funcs[f][1]) for f in closure),
        'placed': rows,
        'status': 'observation; no proof claim',
    }
    print(f"printf call: items {start}..{end - 1}, {len(executed)} distinct instructions "
          f"({len(host_exec)} host), arena pcs not matched to an archive function: {len(unmatched)}")
    print(f"direct-call closure of printf: {len(closure)} functions, "
          f"{report['closure_insns']} instructions")
    print(f"{'function':28} {'addr':>10} {'insns':>6} {'exec':>5} {'clos':>4} {'lua':>6} ident")
    print('executed arena code not identified by bytes:',
          ', '.join(f'{x:#x}..{y:#x}' for x, y in ranges))
    for r in rows:
        if r['executed_by_P_call'] == 0 and not a.all:
            continue
        print(f"{r['name']:28} {r['addr']:#10x} {r['insns']:6} {r['executed_by_P_call']:5} "
              f"{'y' if r['in_closure'] else '':>4} {str(r['lua_insns']):>6} "
              f"{'yes' if r['lua_identical'] else 'no'}")
    if a.out:
        Path(a.out).write_text(json.dumps(report, indent=1) + '\n')


if __name__ == '__main__':
    main()
