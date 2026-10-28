#!/usr/bin/env python3
"""Retain complete independent raw-CSem and strict-GCC bitfield observations."""
import argparse,hashlib,json,lzma,os,subprocess,tempfile,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(cmd):
 p=subprocess.run(list(map(str,cmd)),cwd=ROOT,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'},capture_output=True,text=True,timeout=180)
 return dict(command=list(map(str,cmd)),exit=p.returncode,stdout=p.stdout,stderr=p.stderr)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--out',type=Path,required=True);ap.add_argument('--strict-gnu',action='store_true');a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=True);assert not (a.out/'report.json').exists(),'retain previous report in a separate directory'
 binary=ROOT/'.lake/build/bin/csem_raw';binary_pin=sha(binary);rows=[]
 for e in json.loads((ROOT/'c/tests/bitfields/manifest.json').read_text()):
  src=ROOT/e['path'];assert sha(src)==e['sha256'];raw=run([binary,'--tcc','--fslist',ROOT/'c/build/fs.list','--semantic',src]);obj=json.loads(raw['stdout']) if raw['exit']==0 else None
  sem=obj['semantics'] if obj else None
  with tempfile.TemporaryDirectory(prefix='bitfield-gcc.') as td:
   gcc=run(['gcc','-std=c11','-pedantic-errors','-O0',src,'-o',Path(td)/'prog']);g=run([Path(td)/'prog']) if gcc['exit']==0 else None
  valid=e['kind']!='constraint'
  defined=isinstance(sem,list) and bool(sem) and all(x.get('status')=='defined' and x.get('exit')==0 and x.get('stdout')=='' for x in sem)
  constraint=isinstance(sem,dict) and sem.get('status')=='elaboration-error' or isinstance(sem,list) and bool(sem) and all(x.get('status')=='elaboration-failure' for x in sem)
  ok=(gcc['exit']==0 and g['exit']==0 and defined) if valid else (gcc['exit']!=0 and constraint)
  console=raw.pop('stdout');name=src.stem+'.stdout.xz'
  with lzma.open(a.out/name,'wt',preset=0) as archive:archive.write(console)
  raw.update(stdout_archive=name,stdout_sha256=hashlib.sha256(console.encode()).hexdigest())
  row=dict(e,raw=raw,semantics=sem,gcc_compile=gcc,gcc=g,ok=bool(ok),required=a.strict_gnu or e['kind']!='gnu-defined');rows.append(row);print('PASS' if ok else 'FAIL',src.name,flush=True)
 assert sha(binary)==binary_pin,'semantic executable changed during measurements'
 report=dict(binary_sha256=binary_pin,source_revision=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),measured_utc=time.strftime('%Y-%m-%dT%H:%M:%SZ',time.gmtime()),rows=rows)
 (a.out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print('bitfields',sum(x['ok'] for x in rows),'/',len(rows),'required',sum(x['ok'] for x in rows if x['required']),'/',sum(x['required'] for x in rows),'GNU nonsuccesses',sum(not x['ok'] for x in rows if x['kind']=='gnu-defined'));return int(not all(x['ok'] for x in rows if x['required']))
if __name__=='__main__':raise SystemExit(main())
