#!/usr/bin/env python3
"""Anonymous-member C11 results, with separately pinned pristine observations."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib,json,os,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parent.parent

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def run(cmd):
    cmd=list(map(str,cmd))
    p=subprocess.run(cmd,cwd=ROOT,capture_output=True,text=True,timeout=180,
                     env={**os.environ,'LEAN_ABORT_ON_PANIC':'1','NO_COLOR':'1','TERM':'dumb'})
    return dict(command=cmd,exit=p.returncode,stdout=p.stdout,stderr=p.stderr)
def specified(r,value=0):
    want=f'Defined {{value: "Specified({value})", stdout: "", stderr: "", blocked: "false"}}'
    lines=[s for s in r['stdout'].splitlines() if not s.startswith('EXECUTION ')]
    return r['exit']==0 and bool(lines) and all(s==want for s in lines)
def rejected(r):
    # A refused complete source needs an actual diagnostic, never just a crash,
    # harness error, missing output or exhaustion. Keep the raw stop envelope.
    s=r['stdout']+r['stderr']
    return r['exit'] in (1,2,125) and ('error:' in s or 'Error {msg: "desugaring failed at ' in s or 'Error {msg: "typechecking failed at ' in s) and 'Defined {' not in s and 'fuel' not in s.lower() and 'panic' not in s.lower()
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--report',type=Path,default=ROOT/'results/csem/anonymous-members/focused.json')
    ap.add_argument('--pristine',action='store_true');ap.add_argument('--without-lean',action='store_true')
    a=ap.parse_args()
    env=json.loads(subprocess.check_output(['bash','-c','source scripts/csem_env.sh; python3 -c \'import os,json; print(json.dumps({k:os.environ[k] for k in ["CSEM_ORACLE","CSEM_RUNTIME","CSEM_LEAN_BIN"]}))\''],text=True))
    fork=Path(subprocess.check_output([ROOT/'scripts/csem_fork_oracle.sh','--path'],text=True).strip())
    oracle=fork/'_build/default/backend/driver/main.exe'; runtime=fork/'_build/install/default'; lean=Path(env['CSEM_LEAN_BIN'])
    def one(src):
        invalid=src.name.startswith('invalid_')
        row=dict(name=src.name,source_sha256=sha(src),invalid=invalid)
        with tempfile.TemporaryDirectory(prefix='anonymous-') as td:
            td=Path(td)
            row['gcc_compile']=run(['gcc','-std=c11','-pedantic-errors','-O0','-funsigned-char',src,'-o',td/'prog'])
            if not invalid and row['gcc_compile']['exit']==0:row['gcc']=run([td/'prog'])
            row['ocaml']=run([oracle,f'--runtime={runtime}','--nolibc','--exec','--batch','--mode=exhaustive',src])
            if not a.without_lean:
                export=run([oracle,f'--runtime={runtime}','--nolibc','--cabs-json',src])
                j=td/'input.json';j.write_text(export['stdout'])
                export['stdout_sha256']=hashlib.sha256(export.pop('stdout').encode()).hexdigest();row['export']=export
                row['lean']=run([lean,'--batch',f'--runtime={runtime}',j])
                if not invalid:
                    dump=run([oracle,f'--runtime={runtime}','--nolibc','--pp=core',src])
                    core=td/'input.core';core.write_text(dump['stdout'])
                    row['core_parse']=run([lean,'--parse-core',core])
                    row['core_ok']=dump['exit']==0 and bool(dump['stdout']) and row['core_parse']['exit']==0
                    dump['stdout_sha256']=hashlib.sha256(dump.pop('stdout').encode()).hexdigest();row['core_dump']=dump
            if a.pristine:
                row['pristine']=run([env['CSEM_ORACLE'],f'--runtime={env["CSEM_RUNTIME"]}','--nolibc','--exec','--batch','--mode=exhaustive',src])
            engines=['ocaml']+([] if a.without_lean else ['lean'])
            row['ok']=(row['gcc_compile']['exit']!=0 and all(rejected(row[e]) for e in engines)) if invalid else (row['gcc_compile']['exit']==0 and row['gcc']['exit']==0 and row['gcc']['stdout']=='' and all(specified(row[e]) for e in engines))
            if not invalid and not a.without_lean:row['ok'] &= row['core_ok']
            print('PASS' if row['ok'] else 'FAIL',src.name,flush=True)
        return row
    files=sorted((ROOT/'csem/tests/anonymous').glob('*.c'))
    if len(files)!=92:raise SystemExit('expected 78 positive and 14 invalid complete probes')
    with ThreadPoolExecutor(max_workers=3) as pool:rows=list(pool.map(one,files))
    bins=[oracle]+([] if a.without_lean else [lean])+([Path(env['CSEM_ORACLE'])] if a.pristine else [])
    sources=[ROOT/p for p in (fork/'.shared-sources').read_text().splitlines()]
    report=dict(rows=rows,binaries={str(p):sha(p) for p in bins},shared_sources={str(p.relative_to(ROOT)):sha(p) for p in sources},pins=(ROOT/'vendor/CSEM_REV').read_text())
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(f'anonymous members: {sum(r["ok"] for r in rows)}/{len(rows)}')
    return int(not all(r['ok'] for r in rows))
if __name__=='__main__':raise SystemExit(main())
