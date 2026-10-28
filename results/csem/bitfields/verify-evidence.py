"""Verify retained captures, completed build records and actual native images."""
from pathlib import Path
import hashlib,json,lzma,re
ROOT=Path(__file__).resolve().parent
H=lambda b:hashlib.sha256(b).hexdigest()
manifest=json.loads((ROOT/'evidence-sha256.json').read_text())
for name,h in manifest.items():assert H((ROOT/name).read_bytes())==h,name
builds=json.loads((ROOT/'builds/manifest.json').read_text())
for name,r in builds.items():
 data=(ROOT/'builds'/name).read_bytes();assert H(data)==r['archive_sha256'];raw=lzma.decompress(data);assert H(raw)==r['sha256'] and len(raw)==r['bytes'],name
recorded=set()
def collect(v):
 if isinstance(v,dict):
  for x in v.values():collect(x)
 elif isinstance(v,list):
  for x in v:collect(x)
 elif isinstance(v,str) and re.fullmatch('[0-9a-f]{64}',v):recorded.add(v)
for p in ROOT.rglob('*.json'):
 if p.name!='evidence-sha256.json':collect(json.loads(p.read_text()))
images=list(ROOT.rglob('prog.elf.xz'))
for p in images:
 raw=lzma.decompress(p.read_bytes());assert raw.startswith(b'\x7fELF') and H(raw) in recorded,p
for name,h in json.loads((ROOT/'lead-byte-copy/archive-sha256.json').read_text()).items():assert H((ROOT/'lead-byte-copy'/name).read_bytes())==h,name
lead=ROOT/'lead-final-review'
for name,h in json.loads((lead/'archive-sha256.json').read_text()).items():assert H((lead/name).read_bytes())==h,name
for name,r in json.loads((lead/'compressed-originals.json').read_text()).items():
 raw=lzma.decompress((lead/r['archive']).read_bytes());assert H(raw)==r['sha256'] and len(raw)==r['bytes'],name
print('verified',len(manifest),'evidence files,',len(builds),'archived build records and',len(images),'actual native ELFs')
