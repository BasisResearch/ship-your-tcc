#!/usr/bin/env python3
"""GNU layout checks: independent GCC observations, raw Cabs and execution.
Pristine Menhir's rejection is a classification, never successful AST equality.
"""
import argparse,hashlib,json,os,subprocess
from pathlib import Path
from gen_gnu_attribute_cases import generate,POSITIVE,DIAGNOSTIC,RESIDUAL
from raw_frontend_cmp import historical_positions
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def run(cmd,stem,env=None):
 cmd=list(map(str,cmd));p=subprocess.run(cmd,cwd=ROOT,capture_output=True,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1',**(env or {})},timeout=120)
 stem.with_suffix('.command.json').write_text(json.dumps(cmd,indent=2)+'\n');stem.with_suffix('.stdout').write_bytes(p.stdout);stem.with_suffix('.stderr').write_bytes(p.stderr)
 return p

def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args();d=a.out.resolve();d.mkdir(parents=True,exist_ok=False);generate(d/'sources')
 fs=d/'fs.list';header=ROOT/'csem/runtime/libc/include/stddef.h';fs.write_text('/runtime/libc/include/stddef.h='+str(header)+'\n')
 raw=ROOT/'.lake/build/bin/csem_raw';pins={str(raw):sha(raw),str(ROOT/'.lake/build/bin/csem_cparse'):sha(ROOT/'.lake/build/bin/csem_cparse'),str(header):sha(header)};rows=[]
 for name in [*POSITIVE,*DIAGNOSTIC,*RESIDUAL]:
  src=d/'sources'/(name+'.c');pins[str(src)]=sha(src);sub=d/name;sub.mkdir()
  p=run([raw,'--gnu','--checked','--semantic','--layout',sub/'layout.pp','--include','/runtime/libc/include','--library','/runtime/libc/include/stddef.h','--fslist',fs,src],sub/'raw')
  row=dict(case=name,raw_exit=p.returncode)
  if p.returncode:row.update(status='RAW_REJECT',diagnostic=p.stderr.decode(errors='replace'))
  else:
   obj=json.loads(p.stdout);sem=obj['semantics'];row['semantics']=sem
   defined=isinstance(sem,list) and bool(sem) and all(x.get('status')=='defined' and x.get('exit')==0 and x.get('stdout')=='' for x in sem)
   row['status']='PASS' if defined else 'SEMANTIC_DIAGNOSTIC'
   # Retain the semantic parser's actual whole Cabs, not model construction.
   (sub/'cabs.json').write_text(json.dumps(dict(digest=obj['parse']['digest'],**obj['parse']['cabs']))+'\n')
   view=sub/'text.pp';data=(sub/'layout.pp').read_bytes();view.write_bytes(data[:-1] if data.endswith(b'\n') else data)
   text=run([ROOT/'.lake/build/bin/csem_cparse','--gnu','--json',view,src],sub/'text')
   row['full_text_cabs']=text.returncode==0 and json.loads(text.stdout)==historical_positions(obj['parse']['cabs'])
   if not row['full_text_cabs']:row['status']='FULL_TEXT_CABS_DIFF'
   if name=='keyword-alias':
    def names(value):
     if isinstance(value,dict):
      if value.get('tag')=='Attribute':yield value['id']['name']
      for child in value.values():yield from names(child)
     elif isinstance(value,list):
      for child in value:yield from names(child)
    row['original_keyword_alias']=('__const__' in list(names(obj['parse']['cabs'])) and any(t['spelling']=='__const__' for t in obj['parse']['phase4']))
    if not row['original_keyword_alias']:row['status']='KEYWORD_ATTRIBUTE_ALIAS_LOST'


   if name in DIAGNOSTIC and defined:row['status']='UNSOUND_ATTRIBUTE_ERASURE'
  gcc=run(['gcc','-std=c11','-O0',src,'-o',sub/'gcc'],sub/'gcc-compile') if name not in DIAGNOSTIC else run(['gcc','-std=c11','-fsyntax-only',src],sub/'gcc-compile')
  row['gcc_compile_exit']=gcc.returncode
  if gcc.returncode==0 and name not in DIAGNOSTIC:
   q=run([sub/'gcc'],sub/'gcc-run');row['gcc_exit']=q.returncode;row['gcc_stdout']=q.stdout.decode(errors='replace')
  row['required_positive']=name in POSITIVE
  if row['required_positive'] and (gcc.returncode or row.get('gcc_exit')!=0 or row.get('gcc_stdout')!=''):row['status']='GCC_ORACLE_ERROR'
  if name in DIAGNOSTIC and row['status']=='SEMANTIC_DIAGNOSTIC':row['status']='DIAGNOSTIC'
  rows.append(row);print(name,row['status'],flush=True)
 assert all(sha(p)==h for p,h in pins.items()),'inputs or executable changed during comparison'
 (d/'report.json').write_text(json.dumps(dict(rows=rows,pins=pins),indent=2)+'\n')
 return int(any(r['required_positive'] and r['status']!='PASS' or r['case'] in DIAGNOSTIC and r['status']!='DIAGNOSTIC' for r in rows))
if __name__=='__main__':raise SystemExit(main())
