from pathlib import Path
import json,hashlib,random,subprocess,os
r=Path(__file__).resolve().parents[1];w=r/'.claude/worktrees/sol-csem-repin';out=r/'build/bitfield-layout-edge-review';out.mkdir(exist_ok=False)
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();binary=w/'.lake/build/bin/csem_raw';bp=H(binary);rng=random.Random(28026);rows=[]
types=[('unsigned char',8),('unsigned short',16),('unsigned',32),('unsigned long long',64)]
for case in range(24):
 d=out/f'{case:02}';d.mkdir();decl=[];sets=[]
 for i in range(4):
  ty,bits=rng.choice(types);width=rng.choice([1,bits-1,bits]);decl.append(f'{ty} f{i}:{width};');sets.append(f's.f{i}={rng.getrandbits(width)}ULL;')
  if rng.randrange(4)==0:decl.append(rng.choice(['unsigned :0;','unsigned char :0;','unsigned short :3;']))
 tag='struct '+('__attribute__((packed)) ' if case%3==0 else '')+'S {'+' '.join(decl)+' char z; };'
 init='struct S s; unsigned char *p=(unsigned char*)&s; unsigned i; for(i=0;i<sizeof s;i++) p[i]=0; '+' '.join(sets)+' s.z=90;'
 reference='#include <stdio.h>\n'+tag+'\nint main(void){'+init+'printf("%u %u %u",(unsigned)sizeof s,(unsigned)_Alignof(struct S),(unsigned)((char*)&s.z-(char*)&s)); for(i=0;i<sizeof s;i++) printf(" %u",(unsigned)p[i]); return 0;}\n'
 (d/'reference.c').write_text(reference);cmd=['gcc','-std=gnu11','-O0',str(d/'reference.c'),'-o',str(d/'reference')];g=subprocess.run(cmd,capture_output=True);assert g.returncode==0,g.stderr;(d/'gcc.stderr').write_bytes(g.stderr)
 q=subprocess.run([str(d/'reference')],capture_output=True);assert q.returncode==0;(d/'gcc.stdout').write_bytes(q.stdout);vals=list(map(int,q.stdout.split()));n,a,off,*bs=vals;assert n==len(bs)
 checks=[f'sizeof s!={n}',f'_Alignof(struct S)!={a}',f'(char*)&s.z-(char*)&s!={off}']+[f'p[{i}]!={v}' for i,v in enumerate(bs)]
 source=tag+'\nint main(void){'+init+'return '+' || '.join(checks)+';}\n';p=d/'input.c';p.write_text(source)
 cmd=[str(binary),'--tcc','--fslist',str(w/'c/build/fs.list'),'--semantic',str(p)];z=subprocess.run(cmd,cwd=w,capture_output=True,timeout=120,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'});(d/'raw.stdout').write_bytes(z.stdout);(d/'raw.stderr').write_bytes(z.stderr);(d/'command.json').write_text(json.dumps(cmd)+'\n')
 sem=json.loads(z.stdout).get('semantics',[]) if z.returncode==0 else [];ok=bool(sem) and all(x.get('status')=='defined' and x.get('exit')==0 and x.get('stdout')=='' for x in sem)
 row=dict(case=case,packed=case%3==0,source_sha256=H(p),gcc=vals,raw_exit=z.returncode,semantics=sem,ok=ok);rows.append(row);print(case,ok,sem,flush=True)
assert H(binary)==bp
(out/'report.json').write_text(json.dumps(dict(seed=28026,binary_sha256=bp,driver_sha256=H(Path(__file__)),rows=rows),indent=2)+'\n')
