from pathlib import Path
import hashlib,json,subprocess,os
r=Path(__file__).resolve().parents[1];w=r/'.claude/worktrees/sol-csem-repin';out=r/'build/bitfield-conversion-review';out.mkdir(exist_ok=False);b=w/'.lake/build/bin/csem_raw';H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pin=H(b)
cases={
'float-unsigned':'struct S{unsigned a:3;};int main(void){struct S s={0};double d=3.75;int x=(s.a=d);return x!=3 || s.a!=3;}',
'float-signed':'struct S{int a:3;};int main(void){struct S s={0};double d=-2.75;int x=(s.a=d);return x!= -2 || s.a!= -2;}',
'float-bool':'struct S{_Bool a:1;};int main(void){struct S s={0};double d=0.5;int x=(s.a=d);return x!=1 || s.a!=1;}',
'float-compound':'struct S{unsigned a:3;};int main(void){struct S s={1};double d=2.75;int x=(s.a+=d);return x!=3 || s.a!=3;}',
'conditional-promotion':'struct S{unsigned a:3;};int main(void){struct S s={1};return (1?s.a:s.a)-2 != -1;}',
'comma-promotion':'struct S{unsigned a:3;};int main(void){struct S s={1};return +(0,s.a)-2 != -1;}',
'cast-clears-width':'struct S{unsigned a:3;};int main(void){struct S s={1};unsigned x=(unsigned)s.a;return x-2 !=4294967295u;}',
'volatile-compound-callback':'struct S{volatile unsigned a:3;};struct S s={1};int n;struct S *f(void){++n;return &s;}int main(void){f()->a+=2;return n!=1 || s.a!=3;}',
}
rows=[]
for name,src in cases.items():
 d=out/name;d.mkdir();p=d/'input.c';p.write_text(src+'\n');cmd=[str(b),'--tcc','--fslist',str(w/'c/build/fs.list'),'--semantic',str(p)];z=subprocess.run(cmd,cwd=w,capture_output=True,timeout=120,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'});(d/'raw.stdout').write_bytes(z.stdout);(d/'raw.stderr').write_bytes(z.stderr);(d/'command.json').write_text(json.dumps(cmd)+'\n')
 g=subprocess.run(['gcc','-std=c11','-O0',str(p),'-o',str(d/'gcc')],capture_output=True);(d/'gcc.stderr').write_bytes(g.stderr);v=subprocess.run([str(d/'gcc')],capture_output=True) if g.returncode==0 else None
 sem=json.loads(z.stdout).get('semantics',[]) if z.returncode==0 else [];row=dict(name=name,source_sha256=H(p),raw_exit=z.returncode,semantics=sem,gcc_compile=g.returncode,gcc_exit=v.returncode if v else None);rows.append(row);print(row,flush=True)
assert H(b)==pin;(out/'report.json').write_text(json.dumps(dict(binary_sha256=pin,driver_sha256=H(Path(__file__)),rows=rows),indent=2)+'\n')
