from pathlib import Path
import subprocess,json,hashlib
r=Path.cwd();out=r/'build/conditional-bitfield-promotion-review-corrected';out.mkdir(exist_ok=False)
H=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();tcc=r/'build/test-c11/host/tcc';assert tcc.exists()
exprs={'gnu-type':'_Generic((s.x ?: 0),int:0,default:1)','ordinary-type':'_Generic((s.x ? s.x : 0),int:0,default:1)','arithmetic-type':'_Generic((s.x + 0),int:0,default:1)','unary-type':'_Generic(+s.x,int:0,default:1)','gnu-negative':'((s.x ?: -1)<0 ? 0:1)','ordinary-negative':'((s.x ? s.x : -1)<0 ? 0:1)'}
rows=[]
for name,expr in exprs.items():
 src=out/(name+'.c');src.write_text('int main(void){struct S{unsigned int x:3;}s={0}; return '+expr+';}\n');row=dict(name=name,source_sha256=H(src),runs=[])
 for engine,cc in [('gcc','gcc'),('patched-host-tcc',str(tcc))]:
  cmd=[cc,'-std=c11']+(['-B'+str(tcc.parent)] if engine=='patched-host-tcc' else [])+[str(src),'-o',str(out/(name+'-'+engine))];p=subprocess.run(cmd,capture_output=True);d=dict(engine=engine,command=cmd,compile_exit=p.returncode);(out/(name+'-'+engine+'.compile.stderr')).write_bytes(p.stderr)
  if p.returncode==0:
   v=subprocess.run([cmd[-1]],capture_output=True);d['exit']=v.returncode;(out/(name+'-'+engine+'.stdout')).write_bytes(v.stdout);(out/(name+'-'+engine+'.stderr')).write_bytes(v.stderr)
  row['runs'].append(d)
 rows.append(row);print(name,[(x['engine'],x.get('exit')) for x in row['runs']],flush=True)
(out/'report.json').write_text(json.dumps(dict(note='Host-only minimization of the separate native GNU conditional batch mismatch. No new Sail or proof claim. Ordinary C11 ternary controls are retained independently of GNU syntax.',tcc_binary_sha256=H(tcc),tccgen_sha256=H(r/'build/tcc-src/tccgen.c'),driver_sha256=H(__file__),rows=rows),indent=2)+'\n')
