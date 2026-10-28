#!/usr/bin/env python3
"""Verify the portable array-constraint observations without running engines."""
from pathlib import Path
import hashlib,json,tarfile
p=Path(__file__).resolve().parent
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
m=json.loads((p/'pins.json').read_text());assert all(sha(p/n)==h for n,h in m['artifacts'].items())
with tarfile.open(p/'captures.tar.gz') as t:actual={f.name:hashlib.sha256(t.extractfile(f).read()).hexdigest() for f in t.getmembers() if f.isfile()}
assert actual==m['archive_members'];assert m['upstream_worktree_status']==''
r=json.loads((p/'report.json').read_text());rows={x['case']:x['engines'] for x in r['rows']};assert len(rows)==10
for n in ['function-array-typedef','void-array-typedef']:
 e=rows[n];assert e['raw']['semantics']==[{'exit':0,'status':'defined','stdout':''}]
 assert e['pristine-ocaml']['verdicts']==['Defined {value: "Specified(0)", stdout: "", stderr: "", blocked: "false"}']
 assert e['gcc']['exit']==e['tcc']['exit']==1
for n in ['function-pointer-array','int-array','incomplete-extern']:assert all(e['exit']==0 for e in rows[n].values())
assert len(actual)==173
print('Verified 173 captures and ten classified cases; invalid inputs add no defined-C coverage')
