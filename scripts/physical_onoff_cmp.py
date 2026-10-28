#!/usr/bin/env python3
"""Frozen on/off arena, console and status controls for physical annotations."""
import sys,json,hashlib,subprocess,os,argparse
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from ghost_cabs_cmp import sha,build_receipt,source_pins
from model_diff import runtime_key
from ghost_origins_cmp import at
ap=argparse.ArgumentParser();ap.add_argument('--build-id',required=True);ap.add_argument('--out',type=Path,required=True);args=ap.parse_args()
root=Path(__file__).resolve().parents[1];os.chdir(root);out=args.out.resolve();out.mkdir(parents=True,exist_ok=True)
exe=root/'.lake/build/bin/tcc_model'
fixtures=sorted((root/'build/ghost-byte-inputs').glob('*.c'))
assert len(fixtures)==7
old=json.loads((root/'build/ghost-byte-inputs/manifest.json').read_text())
assert all(sha(root/p)==h for p,h in old.items())
programs={
 'physical-lf':'#define NA\\\nME(x) x+\\\n1\nint lo\\\nngname=2; int main(void){return NAME(longname)-3;}\n',
 'physical-crlf':'int lo\\\r\nngname=2; int main(void){return longname-2;}\n',
 'physical-literal':'char *s="ab\\\ncd"; int main(void){return s[2]-99;}\n',
 'physical-trigraphs':"??=define N 2\nint a??(2??)=??<1,N??>; int main(void)??<return (??-1 ??! (2 ??' 3)) + 1;??>\n",
 'physical-trisplice':at(8190,'??=define N 2\nint lo??/\r\nngname=N; int main(void){return longname-2;}\n'),
 'physical-boundary-lf':at(8191-6,'int lo\\\nngname=2; int main(void){return longname-2;}\n'),
 'physical-boundary-crlf':at(8191-6,'int lo\\\r\nngname=2; int main(void){return longname-2;}\n'),
 'physical-tail':'int main(void){return 0;} // ??',
}
for n,s in programs.items():
 p=out/(n+'.c');p.write_text(s);fixtures.append(p)
# Sources generated before observation, and immutable for all comparisons.
inputs={str(p.relative_to(root)):sha(p) for p in fixtures}
(out/'input-manifest.json').write_text(json.dumps(inputs,indent=2)+'\n')
receipt=build_receipt(args.build_id);binary=sha(exe);rows=[]
pins=source_pins();runtime=runtime_key();symbols=sha(root/'build/image.syms');driver=sha(Path(__file__))
for p in fixtures:
 records=[]
 for mode in ['off','on']:
  base=out/(p.stem+'.'+mode)
  cmd=[str(exe),'--prog',str(p),'--arg','-w','--arg','-std=c11','--arena-out',str(base)+'.arena','--console-out',str(base)+'.console']
  if mode=='on':cmd+=['--ghost-out',str(base)+'.ghost.json']
  r=subprocess.run(cmd,capture_output=True,timeout=180,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
  Path(str(base)+'.stdout').write_bytes(r.stdout);Path(str(base)+'.stderr').write_bytes(r.stderr)
  records.append(dict(command=cmd,exit=r.returncode,stdout=r.stdout.decode(),stderr=r.stderr.decode(),arena=sha(Path(str(base)+'.arena')) if Path(str(base)+'.arena').exists() else None,console=sha(Path(str(base)+'.console'))))
 a,b=records
 exact=all(a[k]==b[k] for k in ['exit','stdout','stderr','arena','console']) and a['stdout']=='ran\n' and a['arena'] is not None
 rows.append(dict(source=str(p.relative_to(root)),exact=exact,runs=records))
 print(p.stem,'exact' if exact else 'DIFFERENT',flush=True)
assert binary==sha(exe) and all(sha(root/p)==h for p,h in inputs.items())
assert source_pins()==pins and runtime_key()==runtime and sha(root/'build/image.syms')==symbols and sha(Path(__file__))==driver
(out/'erasure.json').write_text(json.dumps(dict(build=receipt,binary_sha256=binary,driver_sha256=driver,source_hashes=pins,runtime_key=runtime,image_symbols_sha256=symbols,inputs=inputs,rows=rows),indent=2)+'\n')
assert all(r['exact'] for r in rows)
