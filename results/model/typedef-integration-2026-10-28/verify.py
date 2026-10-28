#!/usr/bin/env python3
"""Verify retained typedef integration evidence; --sources checks its source snapshot."""
from pathlib import Path
import argparse,collections,gzip,hashlib,json,tarfile

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--sources',action='store_true');a=ap.parse_args();here=Path(__file__).resolve().parent;root=here.parents[2]
 def read(n):return json.loads((here/(n+'.json')).read_text())
 pins=read('pins');assert all(sha(here/p)==h for p,h in pins['artifacts'].items())
 with tarfile.open(here/'captures.tar.gz') as t:actual={m.name:hashlib.sha256(t.extractfile(m).read()).hexdigest() for m in t.getmembers() if m.isfile()}
 assert actual==pins['archive_members']
 q=json.loads(gzip.decompress((here/'gate.request.json.gz').read_bytes()));r=json.loads(gzip.decompress((here/'gate.result.json.gz').read_bytes()));supp=json.loads(gzip.decompress((here/'supplementary.json.gz').read_bytes()))
 assert q['request_id']==r['request_id'] and r['exit']==0 and r['source_hashes_verified']
 if a.sources:
  for d in [q,supp]:assert all(sha(root/p)==h for p,h in d['sha256'].items())
 assert read('fixtures')['summary']=={'exact':45,'explicit-nonsuccess':11}
 assert read('raw-ghost')['summary']=={'exact':46,'explicit-nonsuccess':10}
 assert read('raw-ghost')['acceptance']['accepted']
 assert read('origins')['summary']=={'exact':17,'explicit_differences':2}
 assert all(read('origins')['harness_checks'].values())
 assert read('arenas')['totals']=={'n':7,'same':7}
 assert len(read('erasure')['rows'])==7 and all(x['equal'] for x in read('erasure')['rows'])
 assert read('worker-native')['totals']=={'n':2,'same':2}
 assert len(read('native')['rows'])==2 and all(x['comparison']['status']=='equal' and x['comparison']['compared_bytes']==4194304 for x in read('native')['rows'])
 corpus=read('historical-corpus');assert corpus['totals']=={'n':1815,'same':1345}
 counts=collections.Counter(r['result'] for rows in corpus['results'].values() for r in rows)
 assert counts=={'same':1345,'same-refusal':463,'unsupported-asm':7},counts
 for name in ['fixtures','raw-ghost','origins']:assert read(name)['binary_sha256']==pins['binary_pins']['.lake/build/bin/tcc_ghost']
 print(f'Verified {len(actual)} captured files, {len(q["sha256"])} gate inputs, {len(supp["sha256"])} unchanged supplementary inputs; obligations remain open')
if __name__=='__main__':main()
