#!/usr/bin/env python3
"""The import closure of the machine layer copied from ship-your-lua.

    scripts/port_closure.py [--src ~/Documents/code/ship-your-lua] [--copy] [--check]

SEEDS are the modules the Phase 3 machine lane uses (Config/Step/Halts,
the segment layer, HTIF step observations, decode, frames, densification).
Their transitive `import` closure inside Vsa/ is listed; the closure cannot
drop ship-your-interpreter's pinned modules (`Vsa.Sim.Code.*`, `*Sites`,
`DecodeTable.Batch*`, …) without editing copied files, because the segment
layer imports them (e.g. `BlockMem → ValueSites → Code.Value_*`).

--copy   copies the closure into this checkout (never writes the source).
--check  fails unless every closure file is here and, outside MODIFIED,
         byte-identical to the source; and experiments/port/copied.txt lists
         exactly the unmodified files.
MODIFIED files are copies with a change (ATTRIBUTION.md says which); they
are not in copied.txt, so the discipline gate scans them.
"""
import argparse
import filecmp
import os
import re
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = Path.home() / 'Documents/code/ship-your-lua'
SEEDS = ['Vsa.Machine', 'Vsa.Elf', 'Vsa.Triple', 'Vsa.Meta.SimpNF', 'Vsa.Densify',
         'Vsa.Sim.DeriveCase', 'Vsa.Sim.DeriveCaseRow', 'Vsa.Sim.SegToTripleFramed',
         'Vsa.Sim.StepCount', 'Vsa.Sim.FnSummary', 'Vsa.Sim.HtifStepObs', 'Vsa.Sim.DecodeNF',
         'Vsa.Sim.FrameMeta', 'Vsa.Sim.StepObs', 'Vsa.Sim.TripleCat', 'Vsa.Sim.DeriveCallSeg',
         'Vsa.Sim.DeriveLoop']
MODIFIED = {'Vsa/Sim/InitValues.lean'}
COPIED = ROOT / 'experiments/port/copied.txt'
HEADER = """# Lean files copied verbatim from ship-your-lua (Vsa/: the import closure of the
# machine lane's seeds, scripts/port_closure.py; source commit in ATTRIBUTION.md).
# Exempt from scripts/check_discipline.py. A copied file that is changed leaves
# this list (scripts/port_closure.py MODIFIED).
"""


def mpath(root, m):
    return Path(root, *m.replace('«', '').replace('»', '').split('.')).with_suffix('.lean')


def imports(p):
    txt = re.sub(r'/-.*?-/', '', p.read_text(), flags=re.S)
    out = []
    for line in txt.splitlines():
        s = line.strip()
        if not s or s.startswith('--'):
            continue
        m = re.match(r'^(public\s+)?(meta\s+)?import\s+(.*)', s)
        if m:
            out += m.group(3).split()
        elif not s.startswith('module') and s != 'prelude':
            break
    return out


def closure(src):
    seen, stack = set(), list(SEEDS)
    while stack:
        m = stack.pop()
        if m in seen:
            continue
        p = mpath(src, m)
        if not p.exists():
            sys.exit(f'port_closure: {m} not in {src}')
        seen.add(m)
        stack += [x for x in imports(p) if x.split('.')[0] == 'Vsa']
    return sorted(str(mpath(src, m).relative_to(src)) for m in seen)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--src', default=str(SRC))
    ap.add_argument('--copy', action='store_true')
    ap.add_argument('--check', action='store_true')
    a = ap.parse_args()
    src = Path(a.src)
    # without the source checkout the closure is computed here (same imports)
    files = closure(src if src.exists() else ROOT)
    if a.copy:
        for rel in files:
            if rel in MODIFIED and (ROOT / rel).exists():
                continue
            (ROOT / rel).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(src / rel, ROOT / rel)
        COPIED.write_text(HEADER + ''.join(f + '\n' for f in files if f not in MODIFIED))
    if a.check:
        bad = []
        for rel in files:
            if not (ROOT / rel).exists():
                bad.append(f'missing {rel}')
            elif rel not in MODIFIED and src.exists() and not filecmp.cmp(src / rel, ROOT / rel, shallow=False):
                bad.append(f'changed {rel} (add it to MODIFIED, or restore it)')
        here = sorted(str(p.relative_to(ROOT)) for p in (ROOT / 'Vsa').rglob('*.lean'))
        extra = set(here) - set(files)
        bad += [f'not in the closure: {r}' for r in sorted(extra)]
        listed = [l for l in COPIED.read_text().splitlines() if l and not l.startswith('#')]
        if listed != [f for f in files if f not in MODIFIED]:
            bad.append('experiments/port/copied.txt is not the unmodified closure (run --copy)')
        if bad:
            print('\n'.join(bad))
            sys.exit(1)
    lines = sum(len((ROOT / f).read_text().splitlines()) for f in files if (ROOT / f).exists())
    print(f'port_closure: {len(files)} modules, {lines} lines')


if __name__ == '__main__':
    main()
