#!/usr/bin/env python3
"""Install the datatype-derived visitor emitted by `lake build IEEEVisitor`.
The marked block in Unit/CoreQuoteTest.lean is generated; the surrounding
independent comparison and mutation controls are handwritten.
"""
from pathlib import Path
import argparse,re
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('log',type=Path);p.add_argument('--check',action='store_true');a=p.parse_args()
s=a.log.read_text();start='-- BEGIN DATATYPE VISITOR';end='-- END DATATYPE VISITOR'
if 'inventory drift:' in s:raise SystemExit('generator inventory drift; review and rerun the generator first')
i=s.index(start);j=s.index(end,i)+len(end)
block=s[i:j]
file=Path(__file__).resolve().parents[1]/'lean_frontend/test/Unit/CoreQuoteTest.lean'
old=file.read_text();x=old.index(start);y=old.index(end,x)+len(end)
# Inventory prose comes from the same generator, after its invariant check.
old=re.sub(r'\n-- \d+ model constructors;[^\n]*','',old)
x=old.index(start);y=old.index(end,x)+len(end)
new=old[:x]+block+old[y:]
if a.check:
 if new!=old:raise SystemExit('Core quotation visitor drift')
 print('Core quotation visitor matches generator')
else:file.write_text(new)
