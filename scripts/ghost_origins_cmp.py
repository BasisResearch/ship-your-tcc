#!/usr/bin/env python3
"""Independent exact origin checks for the /prog.c source suffix.

The model still compiles its real injected header. Its genuine source-opening
identity locates the /prog.c suffix. The independent spec processes that source
without predefined text. No Cabs equality or complete-TU acceptance is claimed.
"""
import argparse,copy,json,os,subprocess
from pathlib import Path
from ghost_cabs_cmp import ROOT,sha,source_pins,build_receipt,first_diff
CASES={
 'direct':'int x=1+2; int f(void){return x;}',
 'object':'#define N 3\nint x=N;\n',
 'argument':'#define ADD(x) ((x)+1)\nint f(int x){return ADD(x);}\n',
 'reuse':'#define D(x) ((x)+(x))\n#define N 2\nint x=D(N);\n',
 'paste':'#define C(a,b) a##b\nint C(na,me)=2;\n',
 'stringify':'#define S(x) #x\nchar *s=S(word + token);\n',
 'strings':r'char *s="\x41" "1" "\101";',
 'builtin':'#line 200 "virtual.c"\nint n=__LINE__+__COUNTER__; char *s=__FILE__;\n',
 'ucn':r'#define ID(x) x'+'\n'+r'int \u00e9; int f(void){return ID(é);}',
 'refill':'/*'+' '*8186+'*/int x;',
 'incomplete':'int a[]={1,2,3};',
 'include-nested':'#include "outer.h"\nint n=OUT;\n',
 'include-repeat-declarations':'#include "decl.h"\n#include "decl.h"\nint n=1;\n',
 'include-line':'#include "line.h"\nint n=VALUE;\n',
 'splice':'int lo\\\nngname=1;\n',
 'trigraph':'int x=1 ??! 2;\n',
 'include-repeat':'#include "empty.h"\n#include "again.h"\n#include "again.h"\nint n=VAL;\n',
 'include-alias':'#include "dir/../leaf.h"\nint n=VALUE;\n',
 'varargs':'#define V(x,...) x , ##__VA_ARGS__\nint a[]={V(1),V(2,3)};\n',
}
FILES={
 'include-repeat-declarations':[('/decl.h','extern int repeated;\n')],
 'include-line':[('/line.h','#line 700 "virtual.h"\n#define VALUE 11\nextern int line_name;\n')],
 'include-nested':[('/outer.h','#include "inner.h"\n#define OUT INNER\nextern int outer;\n'),('/inner.h','#define INNER 7\ntypedef int T;\n')],
 'include-repeat':[('/empty.h',''),('/again.h','extern int repeated;\n#define VAL 9\n')],
 'include-alias':[('/dir/empty.h',''),('/leaf.h','#define VALUE 11\nextern int leaf;\n')],
}
# Retained, diagnosed coverage failures, never counted as exact controls.
DIAGNOSTIC={'splice','trigraph'}
def source_view(s):
 return {k:s[k] for k in ['opened_as','identity','contents','included_at']}
def compare(m,s):
 first=None;issue=None;start=0
 physical=[x for x in m.get('sources',[]) if not x.get('generated')]
 # Only the actual first implicit image-header opening is outside this
 # component comparison. All other model openings must be compared, even if
 # the independent spec did not open them or they emitted no language tokens.
 if len(physical)<2 or bytes(physical[0]['opened_as'])!=b'/prog.c' or bytes(physical[1]['opened_as'])!=b'/lib/tcc/include/tccdefs.h' or physical[1].get('included_at') is None:
  return 'missing identified main/implicit-header opening',None,0
 header=physical[1]['id'];model_sources=[x for x in physical if x['id']!=header]
 sources={x['id'] for x in model_sources}
 spec_sources=s.get('sources',[])
 if not spec_sources or bytes(spec_sources[0]['opened_as'])!=b'<command line>' or spec_sources[0]['contents'] or spec_sources[0]['included_at'] is not None:
  return 'independent no-predefs fixture has unexpected source prefix',None,0
 if m.get('token_errors'):issue='model has incomplete origins'
 elif s['status']!='ok':issue='spec has no complete origin fixture'
 elif len(m['model_origins'])!=len(m['tokens']):issue='model anchor table is not parallel'
 else:
  start=next((i for i,o in enumerate(m['model_origins']) if o['source'] in sources),len(m['tokens']))
  if any(o['source']!=header for o in m['model_origins'][:start]):issue='unidentified model token in excluded prefix'
  elif any(o['source'] not in sources for o in m['model_origins'][start:]):issue='non-user source inside user suffix'
  else:first=first_diff({'tokens':m['tokens'][start:],'origins':m['origins'][start:],'sources':[source_view(x) for x in model_sources]},
    {'tokens':s['tokens'],'origins':s['origins'],'sources':[source_view(x) for x in spec_sources[1:]]})
 return issue,first,start

def omission_checks(m,s):
 # Perturb actual captures; these are harness checks, never Cabs/origin
 # success cases and never inputs to either compiler or independent parser.
 checks={}
 x=copy.deepcopy(m);fake=copy.deepcopy(next(f for f in x['sources'] if not f.get('generated')))
 fake.update(id=max(f['id'] for f in x['sources'])+1,opened_as=list(b'/unexpected.h'),identity=[list(b'unexpected.h')],contents=list(b'extern int unexpected;'),included_at='unexpected opening')
 x['sources'].append(fake)
 issue,diff,_=compare(x,s);checks['unmatched_empty_opening_rejected']=bool(issue or diff)
 # An unmatched model-only header emitting tokens before the main source
 # must not disappear into the excluded injected-header prefix either.
 x['tokens'].insert(0,x['tokens'][0]);x['origins'].insert(0,x['origins'][0])
 anchor=copy.deepcopy(x['model_origins'][0]);anchor['source']=fake['id'];x['model_origins'].insert(0,anchor)
 issue,diff,_=compare(x,s);checks['unmatched_prefix_tokens_rejected']=bool(issue or diff)
 x=copy.deepcopy(m);x['sources']=[]
 issue,diff,_=compare(x,s);checks['missing_model_openings_rejected']=bool(issue or diff)
 assert all(checks.values()),checks
 return checks

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--build-id',required=True);ap.add_argument('--out',type=Path,required=True);ap.add_argument('--case',action='append');a=ap.parse_args()
 if a.case and set(a.case)-set(CASES):ap.error('unknown case')
 receipt=build_receipt(a.build_id);pins=source_pins();driver=sha(Path(__file__))
 exe=ROOT/'.lake/build/bin/tcc_ghost';binary=sha(exe);dest=a.out.resolve();dest.mkdir(parents=True,exist_ok=True);rows=[];harness_checks={}
 for name,source in CASES.items():
  if a.case and name not in a.case:continue
  src=dest/(name+'.c');src.write_text(source+'\n');h=sha(src);data={}
  files=dest/(name+'.files.json');files.write_text(json.dumps(FILES.get(name,[]))+'\n');fh=sha(files)
  for mode in ['raw-model','origin-spec']:
   p=subprocess.run([str(exe),mode,str(src),"--files",str(files)],capture_output=True,text=True,timeout=90,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
   (dest/f'{name}.{mode}.json').write_text(p.stdout);(dest/f'{name}.{mode}.stderr').write_text(p.stderr)
   assert p.returncode==0,(mode,p.returncode,p.stderr)
   data[mode]=json.loads(p.stdout)
  m,s=data['raw-model'],data['origin-spec'];issue,first,start=compare(m,s)
  exact=not issue and first is None
  row=dict(case=name,required_exact=name not in DIAGNOSTIC,source_sha256=h,files_sha256=fh,origin_exact=exact,issue=issue,first_difference=first,cabs_status=m['status'],token_errors=len(m.get('token_errors',[])))
  if not issue:row['uncompared_header_prefix_tokens']=start
  rows.append(row)
  if name=='direct':harness_checks=omission_checks(m,s)
  assert sha(src)==h and sha(files)==fh
  print(name,'origin-exact' if exact else 'DIFFERENT',issue or first or '',flush=True)
 assert sha(exe)==binary and source_pins()==pins and sha(Path(__file__))==driver,'input changed during comparison'
 required=[r for r in rows if r['required_exact']]
 exact=sum(r['origin_exact'] for r in rows)
 accepted=bool(required) and all(r['origin_exact'] for r in required)
 report=dict(harness_checks=harness_checks,acceptance=dict(accepted=accepted,required_exact=[r['case'] for r in required]),summary=dict(exact=exact,explicit_differences=len(rows)-exact),scope='exact user-source suffix and opening graph located by model opening IDs; injected header retained but not compared here',build=receipt,binary_sha256=binary,driver_sha256=driver,source_hashes=pins,rows=rows)
 (dest/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
 return int(not accepted)
if __name__=='__main__':raise SystemExit(main())
