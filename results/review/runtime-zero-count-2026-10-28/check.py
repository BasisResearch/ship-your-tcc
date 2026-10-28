from pathlib import Path
import subprocess,json,hashlib
root=Path.cwd();out=root/'build/runtime-source-review';out.mkdir(exist_ok=True)
src=root/'csem/runtime/libc/src/string.c';text=src.read_text();defs=[]
for name in ['strncpy','strncmp']:
 start=text.index(('char *' if name=='strncpy' else 'int ')+name+'(') if name=='strncmp' else text.index('char *strncpy (')
 stop=text.index('\n}',start)+2;defs.append(text[start:stop])
subject=out/'subject.h';subject.write_text('#include <stddef.h>\n#define strncpy subject_strncpy\n#define strncmp subject_strncmp\n'+'\n\n'.join(defs)+'\n#undef strncpy\n#undef strncmp\n')
main='''#include <string.h>
#ifdef SUBJECT
#include "subject.h"
#define strncpy subject_strncpy
#define strncmp subject_strncmp
#endif
int main(void) {
#if CASE == 1
 char dst[4]={'X','Y','Z',0}; char src[1]={0};
 char *r=strncpy(dst,src,0);
 return r!=dst || dst[0]!='X' || dst[1]!='Y' || dst[2]!='Z';
#elif CASE == 2
 char a[2]={'a',0},b[2]={'b',0};
 return strncmp(a,b,0)!=0;
#elif CASE == 3
 char dst[4]={'X','Y','Z',0}; char src[2]={'a',0};
 char *r=strncpy(dst,src,1);
 return r!=dst || dst[0]!='a' || dst[1]!='Y';
#elif CASE == 4
 char a[2]={'a',0},b[2]={'b',0};
 return !(strncmp(a,b,1)<0);
#else
#error missing case
#endif
}
'''
(out/'probe.c').write_text(main);runs=[]
for mode in ['subject','libc']:
 for case in range(1,5):
  cmd=['gcc','-std=c11','-O0','-fno-builtin','-funsigned-char','-DCASE='+str(case)]
  if mode=='subject':cmd+=['-DSUBJECT']
  exe=out/f'{mode}-{case}';cmd += [str(out/'probe.c'),'-o',str(exe)]
  p=subprocess.run(cmd,capture_output=True);assert p.returncode==0,p.stderr
  r=subprocess.run([str(exe)],capture_output=True,timeout=5)
  runs.append(dict(mode=mode,case=case,command=cmd,compile_exit=p.returncode,exit=r.returncode,stdout=r.stdout.decode(),stderr=r.stderr.decode(),binary_sha256=hashlib.sha256(exe.read_bytes()).hexdigest()))
assert [r['exit'] for r in runs]==[1,1,0,0,0,0,0,0],runs
pinned=root/'build/csem-704c4e69-e6d8db56/cerberus-lean'
# Compare directly with the immutable adopted upstream checkout when present.
assert (pinned/'runtime/libc/src/string.c').read_bytes()==src.read_bytes()
rev=subprocess.check_output(['git','rev-parse','HEAD'],cwd=pinned,text=True).strip()
record=dict(source=str(src.relative_to(root)),source_sha256=hashlib.sha256(src.read_bytes()).hexdigest(),upstream_revision=rev,upstream_source_identical=True,scope='Exact extracted C source bodies compiled with GCC; not Lean/Core/runtime-loader execution or fresh upstream binary rebuild.',compiler=subprocess.check_output(['gcc','--version'],text=True).splitlines()[0],runs=runs)
(out/'report.json').write_text(json.dumps(record,indent=2)+'\n');print('PASS: two source-runtime defects; two nonzero-count controls; libc references all zero')
