#!/usr/bin/env python3
"""Defined C11 batch: ghost erasure, full native Sail arena and execution."""
import argparse, hashlib, json, os, subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def run(cmd,out,env=None):
 p=subprocess.run(cmd,cwd=ROOT,capture_output=True,env=env,timeout=1800)
 out.with_suffix('.stdout').write_bytes(p.stdout);out.with_suffix('.stderr').write_bytes(p.stderr)
 return dict(command=cmd,exit=p.returncode,stdout_sha256=sha(out.with_suffix('.stdout')),stderr_sha256=sha(out.with_suffix('.stderr')))
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--source',type=Path,required=True);ap.add_argument('--out',type=Path,required=True);ap.add_argument('--sail-capture',type=Path,required=True);a=ap.parse_args()
 src=a.source.resolve();out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
 binary=ROOT/'.lake/build/bin/tcc_model';pins={str(p):sha(p) for p in [src,binary,a.sail_capture.resolve(),a.sail_capture.resolve().with_name('build.json'),Path(__file__)]}
 runs=[]
 for mode in ['off','on']:
  base=out/mode
  cmd=[str(binary),'--prog',str(src),'--arg','-w','--arg','-std=c11','--arena-out',str(base)+'.arena','--console-out',str(base)+'.console']
  if mode=='on':cmd+=['--ghost-out',str(base)+'.ghost.json']
  r=run(cmd,base);r['arena']=sha(str(base)+'.arena');r['console']=sha(str(base)+'.console');runs.append(r)
 erasure=all(runs[0][k]==runs[1][k] for k in ['exit','stdout_sha256','stderr_sha256','arena','console']) and runs[0]['exit']==0
 cmd=['python3','scripts/model_diff.py','--no-report','--require-success','1','--json-out',str(out/'arenas.json'),'--sail-capture',str(a.sail_capture.resolve()),'-j','1',str(src)]
 arenas=run(cmd,out/'native-arena')
 # Full execution is a distinct observation from the handoff arena.
 env={**os.environ,'TMPDIR':str(out),'KEEP_WORK':'1','ORACLE_META':str(out/'execution.meta'),'MAX_STEPS':'100000000','TCC_ARGS':'"-w","-std=c11",'}
 execution=run(['systemd-run','--user','--scope','-p','MemoryMax=12G','bash','scripts/run_tcc_sail.sh',str(src)],out/'execution',env)
 assert all(sha(p)==h for p,h in pins.items()),'native inputs changed'
 report=dict(input_pins=pins,erasure=erasure,model_runs=runs,native_arena=arenas,execution=execution)
 (out/'comparison.json').write_text(json.dumps(report,indent=2)+'\n')
 assert erasure and arenas['exit']==0 and execution['exit']==0,report
 assert (out/'execution.stdout').read_bytes()==b'', 'unexpected defined batch output'
 print('batch: identical ghost on/off, complete native Sail arena and exit 0')
if __name__=='__main__':main()
