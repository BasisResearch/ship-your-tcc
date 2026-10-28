#!/usr/bin/env python3
"""Independent supplemental typeof/VLA syntax and host evaluation controls."""
import argparse,gzip,json,os,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'scripts'))
from ghost_cabs_cmp import sha,first_diff,source_pins
from injected_origins_cmp import compare

FORMS={
 'vla-expression':'int main(void){int n=2;typeof(*(int(*)[n++])0) a;return n!=3 || sizeof(a)!=8;}',
 'vla-type':'int main(void){int n=2;typeof(int[n++]) a;return n!=3 || sizeof(a)!=8;}',
 'vla-existing':'int main(void){int n=2;int a[n++];typeof(a) b;return n!=3 || sizeof(b)!=8;}',
 'nonvla-expression':'int main(void){int n=2;typeof(n++) a=1;return n!=2 || a!=1;}',
 'vla-pointer-type':'int main(void){int n=2;typeof(int(*)[n++]) p=0;return n!=3 || sizeof(p)!=8;}',
 'vla-unevaluated-sizeof':'int main(void){int n=2;int s=sizeof(typeof(int[n++]));return n!=3 || s!=8;}',
}

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
 out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
 exe=ROOT/'.lake/build/bin/tcc_ghost';host=ROOT/'build/test-c11/host/tcc'
 pins=source_pins()|{str(p):sha(p) for p in [exe,host,Path(__file__)]};rows=[]
 for name,src in FORMS.items():
  d=out/name;d.mkdir();p=d/'prog.c';p.write_text(src+'\n');captures={};engines=[]
  for engine,cc in [('gcc',['gcc','-std=gnu11','-O0']),('host-tcc',[str(host),'-B'+str(host.parent),'-std=c11'])]:
   dest=d/engine;cmd=cc+[str(p),'-o',str(dest)];q=subprocess.run(cmd,capture_output=True)
   (d/(engine+'.stderr')).write_bytes(q.stderr);assert q.returncode==0,q.stderr
   r=subprocess.run([str(dest)],capture_output=True);assert r.returncode==0 and not r.stdout
   engines.append(dict(engine=engine,command=cmd,compile_exit=q.returncode,exit=r.returncode))
  for mode in ['model','spec','raw-model','raw-spec','independent-raw']:
   cmd=[str(exe),mode,str(p)];q=subprocess.run(cmd,capture_output=True,timeout=120,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
   (d/(mode+'.json.gz')).write_bytes(gzip.compress(q.stdout,mtime=0));(d/(mode+'.stderr')).write_bytes(q.stderr)
   assert q.returncode==0;captures[mode]=json.loads(q.stdout)
  scopes={}
  for scope,m,s in [('shared','model','spec'),('component','raw-model','raw-spec'),('independent','raw-model','independent-raw')]:
   model,spec=captures[m],captures[s]
   assert model['status']==spec['status']=='ok',(name,scope,model.get('gap'),spec.get('error'))
   scopes[scope]=first_diff(model['cabs'],spec['cabs'])
   if scope=='independent' and scopes[scope] is None:scopes[scope]=compare(model,spec)
   assert scopes[scope] is None,(name,scope,scopes[scope])
  rows.append(dict(case=name,source_sha256=sha(p),comparisons=scopes,engines=engines));print(name,'exact',flush=True)
 assert all(sha(Path(p) if Path(p).is_absolute() else ROOT/p)==h for p,h in pins.items())
 # One executable for a separately serviced native capture/run.
 bodies=[s.replace('int main(void)',f'static int check{i}(void)') for i,s in enumerate(FORMS.values())]
 (out/'batch.c').write_text('\n'.join(bodies)+'\nint main(void){return '+' || '.join(f'check{i}()' for i in range(len(bodies)))+';}\n')
 (out/'report.json').write_text(json.dumps(dict(pins=pins,rows=rows),indent=2)+'\n')
if __name__=='__main__':main()
