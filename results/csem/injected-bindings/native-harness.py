from pathlib import Path
import hashlib,json,subprocess,os,re,shutil
root=Path.cwd();out=root/'build/bindings-next-native-2';out.mkdir(exist_ok=False)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
rows=[]
fixtures={'batch':(root/'build/bindings-resume-batch.c').read_text(), 'definition-alias':'unsigned long __builtin_strlen(const char *s){unsigned long n=0;while(s[n])n++;return n;} unsigned long strlen(const char *);int main(void){return __builtin_strlen!=strlen || strlen("abc")!=3;}\n', 'static-pointers':'static void *memcpy(void *d,const void *s,unsigned long n){*(char *)d=42;return d;}int main(void){char s[2]={9,0},a[2],b[2];void *(*p)(void *,const void *,unsigned long)=memcpy;void *(*q)(void *,const void *,unsigned long)=__builtin_memcpy;p(a,s,2);q(b,s,2);return p==q || a[0]!=42 || b[0]!=9;}\n'}
host=Path('/data/home/kirancodes/Documents/code/ship-your-tcc/build/test-c11/host/tcc')
for name,source in fixtures.items():
 d=out/name;d.mkdir();src=d/'prog.c';src.write_text(source);row={'case':name,'source':str(src),'source_sha256':sha(src),'observations':[]}
 for engine,cmd,env in [('gcc',['gcc','-std=c11','-O0','-fno-builtin',str(src),'-o',str(d/'gcc')],{}),('tcc-host',[str(host),'-std=c11','-B'+str(host.parent),'-I'+str(host.parent.parent/'src/include'),str(src),'-o',str(d/'tcc-host')],{}),('tcc-sail',['bash',str(root/'scripts/run_tcc_sail.sh'),str(src)],{'KEEP_WORK':'1','ORACLE_META':str(d/'sail.meta'),'TCC_ARGS':'"-w","-std=c11",'})]:
  p=subprocess.run(cmd,env={**os.environ,**env},capture_output=True,timeout=900);(d/(engine+'.stdout')).write_bytes(p.stdout);(d/(engine+'.stderr')).write_bytes(p.stderr)
  item=dict(engine=engine,command=cmd,env=env,exit=p.returncode)
  if engine!='tcc-sail' and not p.returncode:
   q=subprocess.run([str(d/engine)],capture_output=True);item['program_exit']=q.returncode;item['binary_sha256']=sha(d/engine)
  if engine=='tcc-sail':
   m=re.search(rb'work: ([^\n]+)',p.stderr)
   if m:
    work=Path(os.fsdecode(m[1]));shutil.copy2(work/'prog.elf',d/'prog.elf');shutil.copy2(work/'trace',d/'trace');item['elf_sha256']=sha(d/'prog.elf')
   item['runtime_pins']={str(p):sha(p) for p in [root/'c/build/base.elf',root/'c/build/tcc.o',root/'c/build/main.o',root/'c/build/htif.o',root/'c/build/crt0.o']}
  row['observations'].append(item);print(name,engine,item['exit'],item.get('program_exit'),flush=True)
 rows.append(row);(out/'report.json').write_text(json.dumps(rows,indent=2)+'\n')
