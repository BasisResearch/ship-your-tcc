#!/usr/bin/env python3
"""Actual image headers through ordinary --tcc raw parsing; no header stripping."""
import argparse,gzip,hashlib,json,os,subprocess,importlib.util,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def attrs(v):
 if isinstance(v,dict):
  if v.get('tag')=='Attribute':yield v
  for x in v.values():yield from attrs(x)
 elif isinstance(v,list):
  for x in v:yield from attrs(x)
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args();d=a.out.resolve();d.mkdir(parents=True,exist_ok=False)
 entries=[line.split('=',1) for line in (ROOT/'c/build/fs.list').read_text().splitlines() if line.split('=',1)[0].endswith('.h')]
 actual=dict(entries);header=Path(actual['/lib/tcc/include/tccdefs.h'])
 spec=importlib.util.spec_from_file_location('strip_image_headers',ROOT/'c/src/strip_headers.py');stripper=importlib.util.module_from_spec(spec);spec.loader.exec_module(stripper)
 assert stripper.strip((ROOT/'vendor/tinycc/include/tccdefs.h').read_text()).encode()==header.read_bytes(),'image header differs from its pinned generator'
 table=(ROOT/'Spec/Pre/Tcc/ImageFS.lean').read_text();match=re.search(r'/-- `/lib/tcc/include/tccdefs.h` -/\ndef hdr\d+ : String := ("[^\n]*")',table)
 assert match and json.loads(match[1]).encode()==header.read_bytes(),'spec image header differs from actual FS'

 binary=ROOT/'.lake/build/bin/csem_raw';pins={str(binary):sha(binary),**{p:sha(p) for _,p in entries}};rows=[]
 cases={'ordinary':'int main(void){return 0;}\n','macro':'#define ATTR __attribute__((unused,aligned(16)))\nATTR int x;\nint main(void){return 0;}\n','stdio':'#include <stdio.h>\nint main(void){return 0;}\n','setjmp':'#include <setjmp.h>\nint main(void){return 0;}\n','relabel-tccdefs':'int main(void){return 0;}\n','relabel-runtime':'#include <setjmp.h>\nint main(void){return 0;}\n'}
 for name,body in cases.items():
  sub=d/name;sub.mkdir();src=sub/'prog.c';src.write_text(body);pins[str(src)]=sha(src);mapping=dict(entries);library=[]
  if name.startswith('relabel-'):
   image='/lib/tcc/include/tccdefs.h' if name=='relabel-tccdefs' else '/include/setjmp.h'
   alias=sub/'relabel.h';alias.write_bytes(b'#line 701 "ordinary-header-alias.h"\n'+Path(mapping[image]).read_bytes());mapping[image]=str(alias);pins[str(alias)]=sha(alias);library=['--library',image]
  fs=sub/'fs.list';fs.write_text(''.join(f'{i}={p}\n' for i,p in mapping.items()));cmd=[str(binary),'--tcc','--fslist',str(fs),*library,str(src)]
  p=subprocess.run(cmd,capture_output=True,timeout=120,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'});(sub/'command.json').write_text(json.dumps(cmd,indent=2)+'\n');(sub/'stdout.json.gz').write_bytes(gzip.compress(p.stdout,mtime=0));(sub/'stderr').write_bytes(p.stderr)
  row=dict(case=name,exit=p.returncode,stdout_sha256=hashlib.sha256(p.stdout).hexdigest(),status='PARSE_DIAGNOSTIC',diagnostic=p.stderr.decode(errors='replace'))
  if p.returncode==0:
   r=json.loads(p.stdout);aa=list(attrs(r['cabs']));asm=[x for x in aa if x['id']['name']=='asm_label'];aligned=[x for x in aa if x['id']['name']=='__aligned__']
   row.update(status='PARSE_PASS',declarations=len(r['cabs']['decls']),tokens=len(r['phase4']),asm_bindings=len(asm),injected_aligned=len(aligned),resolved_relabel=sum(x['actualLibrary'] and not x['presumedLibrary'] for x in r['libraryOrigins']))
   assert len(asm)==21 and aligned,'actual injected declarations/attributes were lost'
   assert all(x['ns']['name']=='__asm__' and x['args'] for x in asm),'asm syntax carrier drift'
   if name.startswith('relabel-'):assert row['resolved_relabel']>0,'resolved source identity lost under #line'
  rows.append(row);print(name,row['status'],row.get('declarations'),flush=True)
 assert all(sha(p)==h for p,h in pins.items()),'header/executable/input changed during observation'
 (d/'report.json').write_text(json.dumps(dict(rows=rows,pins=pins),indent=2)+'\n')
 return int(any(r['status']!='PARSE_PASS' for r in rows))
if __name__=='__main__':raise SystemExit(main())
