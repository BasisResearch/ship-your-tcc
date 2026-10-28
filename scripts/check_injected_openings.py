#!/usr/bin/env python3
"""Compiler opening authority follows exact bytes and include edges.
The negative configurations deliberately differ from the fixed image. Ordinary
library membership, presumed labels, and same-path bytes grant no authority.
"""
import argparse,gzip,hashlib,importlib.util,json,os,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
 out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
 entries=dict(line.split('=',1) for line in (ROOT/'c/build/fs.list').read_text().splitlines() if line.split('=',1)[0].endswith('.h'))
 image='/lib/tcc/include/tccdefs.h';header=Path(entries[image]);original=header.read_bytes()
 spec=importlib.util.spec_from_file_location('strip_image_headers',ROOT/'c/src/strip_headers.py');stripper=importlib.util.module_from_spec(spec);spec.loader.exec_module(stripper)
 assert stripper.strip((ROOT/'vendor/tinycc/include/tccdefs.h').read_text()).encode()==original
 table=ROOT/'Spec/Pre/Tcc/ImageFS.lean';m=re.search(r'/-- `/lib/tcc/include/tccdefs.h` -/\ndef hdr\d+ : String := ("[^\n]*")',table.read_text())
 assert m and json.loads(m[1]).encode()==original
 raw=ROOT/'.lake/build/bin/csem_raw';pins={str(raw):sha(raw),str(table):sha(table),str(ROOT/'vendor/tinycc/include/tccdefs.h'):sha(ROOT/'vendor/tinycc/include/tccdefs.h'),str(ROOT/'c/src/strip_headers.py'):sha(ROOT/'c/src/strip_headers.py'),**{p:sha(p) for p in entries.values()}}
 rows=[]
 cases={'pinned':(original,False),'relabel-content':(b'#line 701 "ordinary-header.h"\n'+original,False),'changed-same-path':(original+b'\n',False),'library-membership':(b'#line 701 "ordinary-header.h"\n'+original,True),'forged-signature':(original.replace(b'__BOTH(void*, memcpy,',b'__BOTH(int, memcpy,'),False),'forged-target':(original.replace(b'__RENAME(#name);',b'__RENAME("memcpy");'),False)}
 for name,(body,library) in cases.items():
  d=out/name;d.mkdir();src=d/'prog.c';src.write_text('int main(void){return 0;}\n');h=d/'header.h';h.write_bytes(body);fs=d/'headers.list';fs.write_text(''.join(f'{i}={h if i==image else p}\n' for i,p in entries.items()))
  pins.update({str(src):sha(src),str(h):sha(h),str(fs):sha(fs)})
  cmd=[str(raw),'--tcc','--semantic','--fslist',str(fs),*(['--library',image] if library else []),str(src)]
  p=subprocess.run(cmd,capture_output=True,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'},timeout=120)
  (d/'command.json').write_text(json.dumps(cmd,indent=2)+'\n');(d/'stdout.json.gz').write_bytes(gzip.compress(p.stdout,mtime=0));(d/'stderr').write_bytes(p.stderr)
  row=dict(case=name,exit=p.returncode,header_sha256=sha(h),status='FAIL')
  if p.returncode==0:
   r=json.loads(p.stdout);parsed=r['parse'];sem=r['semantics'];row.update(bindings=len(parsed['bindings']),excluded=parsed['excluded'],semantics=sem)
   good=(len(parsed['bindings'])==21 and not parsed['excluded'] and sem==[{'exit':0,'status':'defined','stdout':''}]) if name=='pinned' else (not parsed['bindings'] and len(parsed['excluded'])==21 and sem=={'status':'elaboration-error'})
   row['status']='PASS' if good else 'FAIL'
  rows.append(row);print(name,row['status'],flush=True)
 assert all(sha(p)==h for p,h in pins.items()),'input/binary changed during observation'
 (out/'report.json').write_text(json.dumps(dict(rows=rows,pins=pins),indent=2)+'\n')
 return int(any(r['status']!='PASS' for r in rows))
if __name__=='__main__':raise SystemExit(main())
