#!/usr/bin/env python3
"""Actual cfgTcc injection through raw Cabs/Ail/Core; pin every observation.
Joint refusals never count as execution agreement. Historical Menhir comparison
uses the exact positioned stream and an explicit pinned token-origin witness.
"""
import argparse,gzip,hashlib,json,os,re,shlex,subprocess,sys
from pathlib import Path
from raw_frontend_cmp import historical_positions
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def invoke(cmd,stem,env=None):
 cmd=list(map(str,cmd));q=subprocess.run(cmd,cwd=ROOT,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1',**(env or {})},capture_output=True,timeout=180)
 stem.with_suffix('.command.json').write_text(json.dumps(cmd,indent=2)+'\n')
 stem.with_suffix('.stdout.gz').write_bytes(gzip.compress(q.stdout,mtime=0));stem.with_suffix('.stderr').write_bytes(q.stderr)
 return q

def validate_bindings(parsed):
 def nodes(value):
  if isinstance(value,dict):
   yield value
   for child in value.values():yield from nodes(child)
  elif isinstance(value,list):
   for child in value:yield from nodes(child)
 bindings=parsed['bindings']
 if len(bindings)!=21 or sum(b['intrinsic'] for b in bindings)!=1:return False
 if len({b['name'] for b in bindings})!=21:return False
 for b in bindings:
  declaration=b['declaration'];allnodes=list(nodes(declaration))
  carriers=[x for x in allnodes if x.get('tag')=='Attribute' and x['id']['name']=='asm_label']
  ids=[x['id'] for x in allnodes if x.get('tag')=='DDecl_identifier']
  if len(carriers)!=1 or len(ids)!=1 or ids[0]['name']!=b['name']:return False
  carrier=carriers[0]
  if carrier['ns']['name']!='__asm__' or [a[1] for a in carrier['args']]!=['"'+b['target']+'"']:return False
  if not all(i['loc']['begin']['source_identity']['compilerInjected'] for i in [ids[0],carrier['id'],carrier['ns']]):return False
  if not any(x.get('tag')=='DDecl_function' and x.get('params') for x in allnodes):return False
  # Check the entire original declaration, including all type positions.
  if not any(x==declaration for x in nodes(parsed['cabs'])):return False
 return True

def cases():
 rows={
 'empty':('','no-entry'),
 'ordinary':('int main(void){return 0;}\n','defined'),
 'memcpy':('int main(void){char a[4]={1,2,3,0};char b[4];return __builtin_memcpy(b,a,4)!=b || b[2]!=3;}\n','defined'),
 'malloc-free':('int main(void){int *p=__builtin_malloc(sizeof(int));*p=37;int r=*p!=37;__builtin_free(p);return r;}\n','defined'),
 'realloc':('int main(void){char *p=__builtin_malloc(2);p[0]=37;p=__builtin_realloc(p,4);int r=p[0]!=37;__builtin_free(p);return r;}\n','defined'),
 'memmove-memset':('int main(void){char a[4]={1,2,3,0};__builtin_memmove(a+1,a,2);__builtin_memset(a,7,1);return a[0]!=7 || a[1]!=1 || a[2]!=2;}\n','runtime-gap'),
 'memcmp':('int main(void){char a[2]={3,4};char b[2]={3,4};return __builtin_memcmp(a,b,2)!=0;}\n','defined'),
 'pointer-memcpy':('void *memcpy(void *,const void *,unsigned long);int main(void){void *(*a)(void *,const void *,unsigned long)=__builtin_memcpy;void *(*b)(void *,const void *,unsigned long)=&memcpy;char s[2]={17,0},d[2];return a!=b || a==0 || b(d,s,2)!=d || d[0]!=17;}\n','defined'),
 'pointer-malloc':('void *malloc(unsigned long);void free(void *);int main(void){void *(*p)(unsigned long)=&__builtin_malloc;void *(*q)(unsigned long)=malloc;void *x=p(8);int r=p!=q || x==0;free(x);return r;}\n','defined'),
 'redeclare-builtin':('void *__builtin_memcpy(void *,const void *,unsigned long);void *memcpy(void *,const void *,unsigned long);void *__builtin_memcpy(void *,const void *,unsigned long);int main(void){return __builtin_memcpy!=memcpy;}\n','defined'),
 'definition-target':('unsigned long strlen(const char *s){unsigned long n=0;while(s[n])n++;return n;}int main(void){unsigned long (*p)(const char *)=__builtin_strlen;return p!=strlen || p("abc")!=3;}\n','defined'),
 'definition-alias':('unsigned long __builtin_strlen(const char *s){unsigned long n=0;while(s[n])n++;return n;}unsigned long strlen(const char *);int main(void){return __builtin_strlen!=strlen || strlen("abc")!=3;}\n','defined'),
 'definition-after':('int main(void){return __builtin_strlen("abc")!=3;}unsigned long strlen(const char *s){unsigned long n=0;while(s[n])n++;return n;}\n','defined'),
 'static-pointers':('static void *memcpy(void *d,const void *s,unsigned long n){*(char *)d=42;return d;}int main(void){char s[2]={9,0},a[2],b[2];void *(*p)(void *,const void *,unsigned long)=memcpy;void *(*q)(void *,const void *,unsigned long)=__builtin_memcpy;p(a,s,2);q(b,s,2);return p==q || a[0]!=42 || b[0]!=9;}\n','defined'),
 'static-spelling':('static void *memcpy(void *d,const void *s,unsigned long n){return d;}int main(void){char a[2]={37,0},b[2];return memcpy==__builtin_memcpy || __builtin_memcpy(b,a,2)!=b || b[0]!=37;}\n','defined'),
 'redecl-incompatible':('int __builtin_malloc(int);int main(void){return 0;}\n','constraint'),
 'target-incompatible':('int malloc(int);int main(void){return 0;}\n','constraint'),
 'two-definitions':('unsigned long strlen(const char *s){return 1;}unsigned long __builtin_strlen(const char *s){return 2;}int main(void){return 0;}\n','constraint'),
 'user-object-asm':('int x __asm__("x");int main(void){return 0;}\n','excluded'),
 'user-function-asm':('void *mine(void *,const void *,unsigned long) __asm__("memcpy");int main(void){return 0;}\n','excluded'),
 'builtin-spoof':('void *__builtin_mine(void *,const void *,unsigned long) __asm__("memcpy");int main(void){return 0;}\n','excluded'),
 'label-spoof':('#line 283 "/lib/tcc/include/tccdefs.h"\nvoid *mine(void *,const void *,unsigned long) __asm__("memcpy");int main(void){return 0;}\n','excluded'),
 'redecl-user-label':('void *__builtin_memcpy(void *,const void *,unsigned long) __asm__("memcpy");int main(void){return 0;}\n','excluded'),
 'user-header-opening':('#include <tccdefs.h>\nint main(void){return 0;}\n','excluded'),
 # These expose existing runtime coverage rather than treating missing bodies
 # as defined C behavior. Keep them separate from canonical-binding controls.
 'runtime-strlen':('int main(void){return __builtin_strlen("abc")!=3;}\n','runtime-gap'),
 'runtime-alloca':('int main(void){char *p=__builtin_alloca(4);p[0]=37;return p[0]!=37;}\n','runtime-gap'),
 }

 # Exercise each missing target directly. These are explicit body-coverage
 # observations, separate from the successful alias executions above.
 missing={
  'memmove':'char a[2]={1,0};__builtin_memmove(a+1,a,1);',
  'memset':'char a[2];__builtin_memset(a,7,2);',
  'strcpy':'char a[4];__builtin_strcpy(a,"a");',
  'strncpy':'char a[4];__builtin_strncpy(a,"a",2);',
  'strcmp':'__builtin_strcmp("a","a");',
  'strncmp':'__builtin_strncmp("a","a",1);',
  'strcat':'char a[4]={65,0};__builtin_strcat(a,"b");',
  'strncat':'char a[4]={65,0};__builtin_strncat(a,"b",1);',
  'strchr':'__builtin_strchr("a",65);',
  'strrchr':'__builtin_strrchr("a",65);',
  'strdup':'__builtin_strdup("a");',
  'calloc':'__builtin_calloc(2,4);',
  'memalign':'__builtin_memalign(16,32);',
  'abort':'__builtin_abort();',
 }
 for target,body in missing.items():rows['runtime-'+target]=('int main(void){'+body+'return 0;}\n','runtime-gap')
 return rows


def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--out',type=Path,required=True);ap.add_argument('--oracle',type=Path);ap.add_argument('--runtime',type=Path);a=ap.parse_args()
 out=a.out.resolve();out.mkdir(parents=True,exist_ok=False);raw=ROOT/'.lake/build/bin/csem_raw'
 fs=out/'headers.list';entries=[line for line in (ROOT/'c/build/fs.list').read_text().splitlines() if line.split('=',1)[0].endswith('.h')];fs.write_text('\n'.join(entries)+'\n')
 pins={str(raw):sha(raw),**{x.split('=',1)[1]:sha(x.split('=',1)[1]) for x in entries}};rows=[]
 for name,(source,expected) in cases().items():
  d=out/name;d.mkdir();src=d/'prog.c';src.write_text(source);pins[str(src)]=sha(src);layout=d/'positioned.pp'
  q=invoke([raw,'--tcc','--semantic','--layout',layout,'--fslist',fs,src],d/'raw')
  row=dict(case=name,expected=expected,exit=q.returncode,status='HARNESS_ERROR')
  if q.returncode==0:
   r=json.loads(q.stdout);parsed=r['parse'];sem=r['semantics'];row['runtime_inventory']=r['runtimeBindings'];row.update(bindings=len(parsed['bindings']),excluded=parsed['excluded'],semantics=sem)
   exact=validate_bindings(parsed);row['full_binding_records']=exact
   inventory={b['target']:b['status'] for b in r['runtimeBindings']}
   loaded={'memcpy','memcmp','malloc','realloc','free'}
   exact=exact and len(inventory)==21 and all(status==('intrinsic-pending' if target=='alloca' else 'loaded' if target in loaded else 'missing-body') for target,status in inventory.items())
   if expected=='defined':good=isinstance(sem,list) and bool(sem) and all(x['status']=='defined' and x['exit']==0 for x in sem) and not parsed['excluded']
   elif expected=='constraint':good=sem=={'status':'elaboration-error'} and not parsed['excluded']
   elif expected=='excluded':good=bool(parsed['excluded']) and sem=={'status':'elaboration-error'}
   elif expected=='no-entry':good=not parsed['excluded'] and isinstance(sem,list) and bool(sem) and all(x['status']=='other-driver-error' for x in sem)
   else:good=not parsed['excluded'] and isinstance(sem,list) and bool(sem) and all(x['status']=='other-driver-error' and 'unknown procedure' in x['diagnostic'] for x in sem);row['runtime_gap_observed']=good
   row['status']=('RUNTIME_GAP' if expected=='runtime-gap' else 'NO_ENTRY' if expected=='no-entry' else 'PASS') if good and exact else 'DIFF'
   if a.oracle and a.runtime:
    witness=d/'opening.tsv';lines=[]
    def positions(v):
     if isinstance(v,dict):
      if v.get('source_identity',{}).get('compilerInjected'):yield v['lexing_line']
      for x in v.values():yield from positions(x)
     elif isinstance(v,list):
      for x in v:yield from positions(x)
    # This explicit test witness transports the raw adapter's authenticated
    # position metadata to OCaml. The oracle parses the full unmodified
    # positioned stream independently; it does not independently establish
    # preprocessing or the model-opening connection.
    authorized=set(positions(parsed['cabs']))
    witness.write_text(f'source\t{src}\t{hashlib.md5(src.read_bytes()).hexdigest()}\nlayout\t{layout}\t{hashlib.md5(layout.read_bytes()).hexdigest()}\n'+''.join(f'token\t{n}\n' for n in sorted(authorized)))
    env={'SYT_RAW_LAYOUT':str(layout),'SYT_COMPILER_OPENING_WITNESS':str(witness)}
    cpp=shlex.join([sys.executable,str(ROOT/'scripts/raw_frontend_cat.py')]);common=[a.oracle,'--runtime='+str(a.runtime),'--nolibc','--cpp='+cpp]
    oc=invoke([*common,'--cabs-json',src],d/'menhir',env);row['menhir_exit']=oc.returncode
    row['cabs_equal']=oc.returncode==0 and {k:v for k,v in json.loads(oc.stdout).items() if k!='digest'}==historical_positions(parsed['cabs'])
    run=invoke([*common,'--exec','--batch','--mode=exhaustive',src],d/'ocaml',env);row['ocaml_exit']=run.returncode;row['ocaml_stdout']=run.stdout.decode(errors='replace')
    if not row['cabs_equal']:row['status']='CABS_DIFF'
    if expected=='defined' and (run.returncode or not re.search(rb'Defined.*Specified\(0\)',run.stdout)):
     row['status']='OCAML_SEMANTIC_DIFF'
    if expected in ['constraint','excluded'] and (not run.returncode or b'error:' not in run.stderr.lower()):
     row['status']='OCAML_CONSTRAINT_DIFF'
    if expected=='runtime-gap' and (not run.returncode or b'unknown procedure' not in run.stdout):
     row['status']='OCAML_RUNTIME_GAP_DIFF'
    if expected=='no-entry' and not run.returncode:row['status']='OCAML_NO_ENTRY_DIFF'
   gcc=invoke(['gcc','-std=gnu11','-fno-builtin','-O0',src,'-o',d/'gcc.exe'],d/'gcc-compile')
   row['gcc_compile_exit']=gcc.returncode
   if expected=='defined' and gcc.returncode==0:
    gr=invoke([d/'gcc.exe'],d/'gcc-run');row['gcc_exit']=gr.returncode
    # GNU builtin definitions and static-shadow pointer behavior differ
    # independently on GCC. Pin the observed difference; tcc/Sail is the
    # target. A new unexpected answer still fails this check.
    expected_gcc=1 if name in ['definition-alias','static-pointers'] else 0
    row['gcc_observation']='GNU_TARGET_DIFFERENCE' if expected_gcc else 'AGREE'
    if gr.returncode!=expected_gcc:row['status']='GCC_UNEXPECTED'
   elif expected=='defined':row['status']='GCC_COMPILE_ERROR'
  rows.append(row);print(name,row['status'],flush=True)
 if a.oracle:pins[str(a.oracle)]=sha(a.oracle)
 assert all(sha(p)==h for p,h in pins.items()),'frozen observation input changed'
 report=dict(rows=rows,pins=pins);(out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
 return int(any(r['status'] not in ['PASS','RUNTIME_GAP','NO_ENTRY'] for r in rows))
if __name__=='__main__':raise SystemExit(main())
