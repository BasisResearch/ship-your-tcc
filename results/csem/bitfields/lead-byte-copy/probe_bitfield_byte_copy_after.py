from pathlib import Path
import hashlib,json,subprocess,os
r=Path(__file__).resolve().parents[1];w=r/'.claude/worktrees/sol-csem-repin';out=r/'build/bitfield-byte-copy-review-after';out.mkdir(exist_ok=False)
b=w/'.lake/build/bin/csem_raw';h=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pin=h(b)
body='struct S { unsigned a:3; }; int main(void) { struct S s={5}, t={0}; unsigned char *p=(unsigned char*)&s,*q=(unsigned char*)&t; unsigned i; for(i=0;i<sizeof s;i++) q[i]=p[i]; return t.a!=5; }\n'
controls={'padding':body,'full-byte':body.replace('unsigned a:3;','unsigned a:8;'),'assignment':body.replace('for(i=0;i<sizeof s;i++) q[i]=p[i];','t=s;')}
rows=[]
for name,src in controls.items():
 d=out/name;d.mkdir();p=d/'input.c';p.write_text(src)
 cmd=[str(b),'--tcc','--fslist',str(w/'c/build/fs.list'),'--semantic',str(p)];a=subprocess.run(cmd,cwd=w,env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'},capture_output=True,timeout=120)
 (d/'raw.stdout').write_bytes(a.stdout);(d/'raw.stderr').write_bytes(a.stderr);(d/'raw.command.json').write_text(json.dumps(cmd)+'\n')
 gcmd=['gcc','-std=c11','-pedantic-errors','-O0',str(p),'-o',str(d/'gcc')];g=subprocess.run(gcmd,capture_output=True);(d/'gcc.stdout').write_bytes(g.stdout);(d/'gcc.stderr').write_bytes(g.stderr);(d/'gcc.command.json').write_text(json.dumps(gcmd)+'\n');run=subprocess.run([str(d/'gcc')],capture_output=True) if g.returncode==0 else None
 row=dict(case=name,source_sha256=h(p),raw_exit=a.returncode,semantics=json.loads(a.stdout).get('semantics') if a.returncode==0 else None,gcc_compile=g.returncode,gcc_exit=run.returncode if run else None);rows.append(row);print(row,flush=True)
assert h(b)==pin
(out/'report.json').write_text(json.dumps(dict(binary=str(b),binary_sha256=pin,driver_sha256=h(Path(__file__)),rows=rows),indent=2)+'\n')
