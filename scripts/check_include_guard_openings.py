#!/usr/bin/env python3
"""Independent include-opening and positioned-token observations.

Cases retain the counterexamples measured before the target cache repair.
No fixture supplies a model/spec opening graph. Host observations are not Sail.
"""
from pathlib import Path
import argparse,hashlib,json,lzma,os,subprocess,time
from ghost_origins_cmp import compare
ROOT=Path(__file__).resolve().parents[1]
H=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
base='#ifndef H\n#define H\nextern int retained;\n#endif\n'
cases={
'simple':(base,'',''),
'leading-blank':('\n\n'+base,'',''),
'leading-comment':('/* c */\n'+base,'',''),
'leading-null':('#\n'+base,'',''),
'leading-define':('#define BEFORE 1\n'+base,'',''),
'leading-line':('#line 80 "virtual.h"\n'+base,'',''),
'if-not-defined':(base.replace('#ifndef H','#if !defined(H)'),'',''),
'outer-else':(base.replace('#endif','#else\nextern int alternate;\n#endif'),'',''),
'outer-elif':(base.replace('#endif','#elif 0\nextern int alternate;\n#endif'),'',''),
'nested-else':(base.replace('extern int retained;','#if 1\nextern int retained;\n#else\nextern int alternate;\n#endif'),'',''),
'trailing-null':(base+'#\n','',''),
'trailing-define':(base+'#define AFTER 1\n','',''),
'trailing-comment':(base+'/* c */\n','',''),
'trailing-token':(base+'extern int after;\n','',''),
'predefined-guard':(base,'#define H\n',''),
'undefined-between':(base,'','#undef H\n'),
'redefined-between':(base,'','#undef H\n#define H 3\n'),
'not-defining-guard':(base.replace('#define H\n',''),'',''),
'undefined-inside':(base.replace('extern int retained;','extern int retained;\n#undef H'),'',''),
'leading-null-predefined':('#\n'+base,'#define H\n',''),
}
cases.update({
'leading-null-comment':('# /* null */\n'+base,'',''),
'leading-inactive':('#if 0\nextern int ignored;\n#endif\n'+base,'',''),
'leading-empty-macro':('EMPTY\n'+base,'#define EMPTY\n',''),
'leading-spliced-guard':(base.replace('#ifndef','#ifn\\\ndef'),'',''),
'leading-trigraph-guard':(base.replace('#','??='),'',''),
'crlf':(base.replace('\n','\r\n'),'',''),
'no-final-newline':(base.rstrip('\n'),'',''),
'no-final-newline-crlf':(base.replace('\n','\r\n').rstrip('\r\n'),'',''),
'no-final-newline-splice':(base.rstrip('\n').replace('#endif','#en\\\ndif'),'',''),
'no-final-newline-trigraph':(base.rstrip('\n').replace('#','??='),'',''),
'null-at-eof':(base+'#','',''),
'line-inside-guard':(base.replace('extern int retained;','#line 80 "virtual.h"\nextern int retained;'),'',''),
'empty-macro-after-guard':(base+'EMPTY\n','#define EMPTY\n',''),
'inactive-after-guard':(base+'#if 0\nextern int ignored;\n#endif\n','',''),
'recursive-guard':(base.replace('extern int retained;','#include "header.h"\nextern int retained;'),'',''),
})

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--out',type=Path,required=True);ap.add_argument('--diagnostic',action='store_true');ap.add_argument('--host',type=Path);a=ap.parse_args()
 out=a.out.resolve();out.mkdir(exist_ok=False,parents=True)
 tcc=(a.host or ROOT/'build/test-c11/host/tcc').resolve();ghost=ROOT/'.lake/build/bin/tcc_ghost'
 pins={str(tcc):H(tcc),str(ghost):H(ghost),str(Path(__file__).resolve()):H(__file__)};rows=[]
 for name,(header,prefix,middle) in cases.items():
  d=out/name;d.mkdir();src=d/'main.c';src.write_text(prefix+'#include "header.h"\n'+middle+'#include "header.h"\nint main(void){return 0;}\n');(d/'header.h').write_text(header);fs=d/'files.json';fs.write_text(json.dumps([['/header.h',header]])+'\n')
  row=dict(name=name,source_sha256=H(src),header_sha256=H(d/'header.h'),runs={});observed={}
  for mode,cmd in [('host',[str(tcc),'-std=c11','-B'+str(tcc.parent),'-vv','-E',str(src)]),('raw-model',[str(ghost),'raw-model',str(src),'--files',str(fs)]),('origin-spec',[str(ghost),'origin-spec',str(src),'--files',str(fs)])]:
   start=time.time()
   try:p=subprocess.run(cmd,capture_output=True,timeout=90,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'});code=p.returncode;stdout=p.stdout;stderr=p.stderr
   except subprocess.TimeoutExpired as e:code=124;stdout=e.stdout or b'';stderr=(e.stderr or b'')+b'\nHARNESS timeout90s\n'
   with lzma.open(d/(mode+'.stdout.xz'),'wb',preset=0) as f:f.write(stdout)
   (d/(mode+'.stderr')).write_bytes(stderr);v=dict(command=cmd,exit=code,seconds=time.time()-start,stdout_sha256=hashlib.sha256(stdout).hexdigest())
   if mode=='host':
    lines=stdout.decode(errors='replace').splitlines();v['header_opens']=sum(x.startswith('-> ') and x.endswith('/header.h') for x in lines);v['cached_skips']=sum(x.startswith('=> ') and x.endswith('/header.h') for x in lines)
   elif code==0:
    try:obj=json.loads(stdout)
    except json.JSONDecodeError:obj=dict(status='malformed-output')
    observed[mode]=obj;v['status']=obj['status'];v['header_opens']=sum(bytes(x['opened_as'])==b'/header.h' for x in obj.get('sources',[]))
   row['runs'][mode]=v
  if len(observed)==2:
   issue,difference,start=compare(observed['raw-model'],observed['origin-spec']);row.update(issue=issue,difference=difference,model_user_token_start=start)
  else:row.update(issue='missing observer result',difference=None)
  row['exact']=all(v['exit']==0 for v in row['runs'].values()) and all(row['runs'][m].get('status')=='ok' for m in ['raw-model','origin-spec']) and row['issue'] is None and row['difference'] is None and len({v.get('header_opens') for v in row['runs'].values()})==1
  rows.append(row);print(name,'exact' if row['exact'] else 'MISMATCH',[(k,v.get('header_opens'),v.get('cached_skips')) for k,v in row['runs'].items()],row['issue'],row['difference'],flush=True)
 assert all(H(p)==h for p,h in pins.items())
 report=dict(note='Finite host/model/independent-spec opening and full positioned-token/origin/source observations. No new native Sail or universal proof claim.',pins=pins,rows=rows,exact=sum(x['exact'] for x in rows))
 (out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print('Exact',report['exact'],'/',len(rows),flush=True)
 return 0 if a.diagnostic else int(report['exact']!=len(rows))
if __name__=='__main__':raise SystemExit(main())
