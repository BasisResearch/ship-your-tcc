#!/usr/bin/env python3
"""Dev helper for the hand-written TccModel sources: a structure instance
`{ x with f := a,` whose next field continues on a following line left of
`f` does not parse in Lean 4 (fields must not start left of the first
one). Move the first field to its own line, at the continuation's
indentation.  Usage: scripts/lean_fix_structinst.py FILE..."""
import re
import sys

for path in sys.argv[1:]:
    lines = open(path).read().split('\n')
    out = []
    i = 0
    changed = 0
    while i < len(lines):
        l = lines[i]
        m = None
        if l.rstrip().endswith(',') and i + 1 < len(lines):
            # last '{ <expr> with ' on the line
            for mm in re.finditer(r"\{ ?(?:[\w.!\[\]]+ with )?(?=[\w']+ :=)", l):
                m = mm
        if m:
            nxt = lines[i + 1]
            k = len(nxt) - len(nxt.lstrip())
            first_col = m.end()
            if k < first_col:
                out.append(l[:m.end()].rstrip())
                out.append(' ' * k + l[m.end():].lstrip())
                changed += 1
                i += 1
                continue
        out.append(l)
        i += 1
    if changed:
        open(path, 'w').write('\n'.join(out))
        print(f'{path}: {changed}')
