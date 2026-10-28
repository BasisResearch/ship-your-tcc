#!/usr/bin/env python3
"""Project the exact shared single-evaluation compound-assignment lowering."""
import argparse
from pathlib import Path
def branch(s):
 start=s.index('      | A.AilEcompoundAssign e1 aop e2 ->');end=s.index('\n      | A.AilEcall',start)
 return start,end,s[start:end]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('source',type=Path);p.add_argument('target',type=Path);a=p.parse_args()
s=a.source.read_text();t=a.target.read_text();_,_,shared=branch(s);start,end,_=branch(t)
a.target.write_text(t[:start]+shared+t[end:]);assert branch(a.target.read_text())[2]==shared
print('Projected exact shared compound-assignment lowering')
