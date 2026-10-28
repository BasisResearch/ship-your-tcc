#!/usr/bin/env python3
"""Verify the inherited compound-assignment bug captures and minimal control."""
from pathlib import Path
import hashlib,json,tarfile
p=Path(__file__).resolve().parent;H=lambda b:hashlib.sha256(b).hexdigest()
pins=json.loads((p/'pins.json').read_text())
assert all(H((p/n).read_bytes())==h for n,h in pins['files'].items())
with tarfile.open(p/'captures.tar.gz') as t:
 actual={m.name:H(t.extractfile(m).read()) for m in t.getmembers() if m.isfile()}
assert actual==pins['members'] and len(actual)==140
meta=json.loads((p/'provenance.json').read_text());assert meta['upstream_status']=='' and meta['upstream_revision']=='704c4e697845127bced66c93fa0763eb51527ca4'
rows=json.loads((p/'report.json').read_text())['rows'];assert len(rows)==6
for row in rows:
 e=row['engines'];assert e['gcc']['exit']==e['tcc']['exit']==e['gcc']['run_exit']==e['tcc']['run_exit']==0
 sem=e['raw']['semantics'];assert sem
 if row['case'] in ['index-add','index-multiply','pointer-postincrement']:
  assert all(x['status']=='undefined' and 'UB035_unsequenced_race' in x['diagnostic'] for x in sem)
  assert 'UB035_unsequenced_race' in e['pristine']['verdicts']
 else:
  expected=0 if row['case']=='index-assign-control' else 1
  assert all(x['status']=='defined' and x['exit']==expected for x in sem)
  assert f'Specified({expected})' in e['pristine']['verdicts']
x=json.loads((p/'minimal-report.json').read_text())['rows'];assert len(x)==1;e=x[0]['engines']
assert e['gcc']['run_exit']==e['tcc']['run_exit']==1
assert all(x['status']=='defined' and x['exit']==2 for x in e['raw']['semantics'])
assert e['pristine']['verdicts'].count('Specified(2)')==5
print('Verified140 captures: pristine inheritance, three spurious-UB cases, two wrong defined results, ordinary assignment control and minimal twice-versus-once counter')
