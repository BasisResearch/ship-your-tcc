#!/usr/bin/env python3
"""Export completed combined-gate native captures for offline byte comparison."""
import json,re,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'scripts'))
from freeze_sail_arenas import freeze
from ghost_cabs_cmp import build_receipt

def main():
 build_receipt('assert-messages-full-1')
 log=(ROOT/'build/worker-build-assert-messages-full-1.log').read_text()
 dirs=sorted(set(re.findall(r'build/(?:assert-messages|unevaluated-syntax|array-decay)-gate\.[A-Za-z0-9]+',log)))
 reports=[]
 for directory in dirs:
  reports.extend(sorted((ROOT/directory).rglob('arenas.json')))
 assert len(reports)==5,len(reports)
 out=ROOT/'build/assert-messages-native';out.mkdir(exist_ok=False)
 rows=[]
 for index,report in enumerate(reports):
  target=out/str(index)
  inventory=freeze(report.resolve(),target.resolve())
  assert inventory['successes']==1
  rows.append(dict(report=str(report.relative_to(ROOT)),directory=str(index),steps=inventory['rows'][0]['comparison']['steps'],bytes=inventory['rows'][0]['comparison']['compared_bytes']))
 (out/'exports.json').write_text(json.dumps(rows,indent=2)+'\n')
 print('exported',len(rows),'complete native arena pairs')
if __name__=='__main__':main()
