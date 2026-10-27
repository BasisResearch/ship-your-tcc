#!/usr/bin/env python3
"""Recheck the same probe sources across the two CSem pins, without modifying images.
Sail records remain their original dated evidence; changed compiler fingerprints
are reported and never silently recertified by this comparison.
"""
from pathlib import Path
import sys,os,json,hashlib,subprocess,concurrent.futures,re,time
ROOT=Path.cwd();sys.path.insert(0,str(ROOT/'scripts'))
import cfg_probes as cfg
import fs_diff as fs
TAG=sys.argv[1];OUT=ROOT/'results/csem/repin-2026-10-27'/TAG;OUT.mkdir(exist_ok=True)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
refs=ROOT/'results/csem';main=Path(subprocess.check_output(['git','rev-parse','--path-format=absolute','--git-common-dir'],text=True).strip()).parent
frozen={str(p.relative_to(ROOT)):sha(p) for p in (ROOT/'csem/runtime').rglob('*') if p.is_file()}
frozen['.lake/build/bin/csem_run']=sha(ROOT/'.lake/build/bin/csem_run')

def run(cmd):
 t=time.monotonic();p=subprocess.run(cmd,cwd=ROOT,capture_output=True,timeout=180,env=dict(os.environ,CSEM_IMAGE_FS_LIST=str(main/'c/build/fs.list')))
 return p.returncode,p.stdout,p.stderr,round(time.monotonic()-t,3)

def config(src):
 old=json.loads((refs/'cfg-probes'/(src.stem+'.csem.json')).read_text())
 assert sha(src)==old['source_sha256'],str(src)
 fresh=ROOT/'results/csem/repin-2026-10-27/baseline-focused'/(src.stem+'.json')
 if TAG!='baseline-focused' and fresh.exists():
  old=json.loads(fresh.read_text());old['status']='defined' if old['defined'] else 'residual'
 cmd=[str(ROOT/'scripts/csem_run.sh')]
 if src.stem=='p09_stdin':cmd+=['--libc']
 cmd+=['--impl','tcc-rv64','--address-space-top','18446744073709551615',str(src)]
 rc,out,err,wall=run(cmd)
 lines=[x for x in out.decode().splitlines() if not x.startswith('EXECUTION ')]
 parsed=[cfg.DEFINED.fullmatch(x) for x in lines]
 defined=rc==0 and bool(parsed) and all(parsed)
 outcomes=sorted({(cfg.unescape(m[2]),int(m[1])%2**32) for m in parsed if m})
 status=('SAME_OUTCOMES' if defined and outcomes==sorted(map(tuple,old['outcomes'])) else 'CHANGED_OUTCOMES') if old['status']=='defined' else ('RESIDUAL' if not defined else 'IMPROVED')
 row=dict(kind='config',probe=src.stem,status=status,old_status=old['status'],command=cmd,exit=rc,wall_seconds=wall,source_sha256=sha(src),defined=bool(defined),outcomes=outcomes)
 (OUT/(src.stem+'.json')).write_text(json.dumps(row,indent=2)+'\n');(OUT/(src.stem+'.out')).write_bytes(out);(OUT/(src.stem+'.err')).write_bytes(err)
 print(src.stem,status,flush=True);return row

def imagefs(src):
 old=json.loads((refs/'fs-probes'/(src.stem+'.json')).read_text());assert sha(src)==old['source_sha256']
 observed=refs/'fs-probes'/(src.stem+'.sail.out');meta=refs/'fs-probes'/(src.stem+'.sail.meta')
 cmd=[str(ROOT/'scripts/csem_run_image.sh'),'--first',str(src)]
 rc,out,err,wall=run(cmd);lines=[x for x in out.decode().splitlines() if not x.startswith('EXECUTION ')]
 m=fs.DEFINED.fullmatch(lines[0]) if len(lines)==1 else None
 status='CHANGED'
 if old['status']=='AGREE_ONE_BRANCH' and 'program_steps=' in meta.read_text() and rc==0 and m and fs.unescape(m[2])==observed.read_bytes() and fs.unescape(m[3])==b'' and int(m[1])%256==old['sail_exit']:status='SAME_SAIL_OBSERVATION'
 elif old['status']=='MISSING_LIBRARY' and rc!=0 and b'CerbFS refusal' in err:status='MISSING_LIBRARY'
 row=dict(kind='filesystem',probe=src.stem,status=status,command=cmd,exit=rc,wall_seconds=wall,source_sha256=sha(src),sail_stdout_sha256=sha(observed),sail_meta_sha256=sha(meta))
 (OUT/('fs-'+src.stem+'.json')).write_text(json.dumps(row,indent=2)+'\n');(OUT/('fs-'+src.stem+'.out')).write_bytes(out);(OUT/('fs-'+src.stem+'.err')).write_bytes(err)
 print('fs-'+src.stem,status,flush=True);return row

# Verify the retained FS image/table/embedded files before and after all runs.
pins=json.loads((refs/'fs-probes/pins.json').read_text());fsfiles=[p for p in pins if str(main/'c/build') in p]
for name in fsfiles:assert sha(Path(name))==pins[name],name
frozen.update({name:sha(Path(name)) for name in fsfiles})
with concurrent.futures.ThreadPoolExecutor(4) as pool:
 futs=[pool.submit(config,p) for p in sorted((ROOT/'c/tests/cfg').glob('*.c'))]+[pool.submit(imagefs,p) for p in sorted((ROOT/'c/tests/fs').glob('*.c'))]
 rows=[f.result() for f in futs]
for name,h in frozen.items():assert sha(Path(name))==h,name
(OUT/'summary.json').write_text(json.dumps(dict(rows=rows,sha256=frozen,comparison='Old/new CSem outcomes for identical source. FS also compared to hash-verified retained Sail bytes. Config Sail evidence is historical, not recertified.'),indent=2)+'\n')
raise SystemExit(any(r['status'] in ['CHANGED','CHANGED_OUTCOMES'] for r in rows))
