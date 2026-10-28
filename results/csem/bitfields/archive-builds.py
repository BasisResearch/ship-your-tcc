"""Retain completed receipts/manifests/logs, never reinterpret failed builds."""
from pathlib import Path
import hashlib,json,lzma
ROOT=Path.cwd();OUT=ROOT/'results/csem/bitfields/builds';OUT.mkdir(exist_ok=True)
H=lambda b:hashlib.sha256(b).hexdigest()
paths=[]
for p in sorted((ROOT/'build').glob('worker-build.bitfields-*.result')):
 r=json.loads(p.read_text());assert r.get('source_hashes_verified'),p
 req=ROOT/'build'/('worker-build.'+r['request_id']+'.request');q=json.loads(req.read_text());assert q['request_id']==r['request_id']
 paths.extend([p,req,ROOT/r['log']])
for p in sorted((ROOT/'build').glob('bitfields-oracle-*.result')):
 r=json.loads(p.read_text())
 if not r.get('source_hashes_verified'):
  verify=p.with_suffix('.verify.json');v=json.loads(verify.read_text());assert v['request_id']==r['request_id'] and v['source_hashes_verified'];paths.append(verify)
 paths.extend([p,ROOT/'build'/('bitfields-oracle-'+r['request_id'].split('-')[-1]+'.request')])
 if not Path(r['log']).is_absolute():paths.append(ROOT/r['log'])
for name in ['bitfields-lua-inventory.txt','bitfields-static-1.log','bitfields-static-final.log','bitfields-regen-character.log','bitfields-regen-character-events.log','bitfields-regen-profile.log']:
 p=ROOT/'build'/name
 if p.exists():paths.append(p)
manifest={}
for p in sorted(set(paths)):
 assert p.is_file(),p
 data=p.read_bytes();name=p.name+'.xz';target=OUT/name
 compressed=lzma.compress(data,preset=1)
 if target.exists():assert lzma.decompress(target.read_bytes())==data,'completed record changed: '+str(p)
 else:target.write_bytes(compressed)
 assert H(lzma.decompress(target.read_bytes()))==H(data)
 manifest[name]=dict(original=str(p.relative_to(ROOT)),sha256=H(data),bytes=len(data),archive_sha256=H(target.read_bytes()))
(OUT/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print('archived and verified',len(manifest),'completed evidence files')
