#!/usr/bin/env python3
"""GNU syntax arena controls, with refusals and ghost erasure kept separate."""
import argparse, gzip, json, os, subprocess
from pathlib import Path
from gen_gnu_expression_fixtures import fixtures, HEADER, batch
from ghost_cabs_cmp import sha
import model_diff as md

ROOT=Path(__file__).resolve().parents[1]

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--out',type=Path,required=True)
    ap.add_argument('--sail-capture',type=Path);a=ap.parse_args()
    out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
    inputs=out/'inputs';inputs.mkdir();(inputs/'gnu-syntax.h').write_text(HEADER)
    sources=fixtures()|{'batch':(batch(),'valid')}
    paths={}
    for name,(src,category) in sources.items():
        if name!='batch':
            src+='\nint main(void){return 0;}\n'
            if 'int g(void);' in src:src+='int g(void){return 0;}\n'
        p=inputs/(name+'.c');p.write_text(src);paths[name]=p
    pins={str(p):sha(p) for p in [Path(__file__),ROOT/'scripts/gen_gnu_expression_fixtures.py',ROOT/'.lake/build/bin/tcc_model',Path(md.CROSS),*inputs.iterdir()]}
    compiler_rows=[]
    for name,p in paths.items():
        category=sources[name][1]
        for engine,compiler in [('gcc',['gcc','-std=gnu11']),('cross-tcc',[md.CROSS,'-std=c11'])]:
            command=compiler+['-I'+str(inputs),'-c',str(p),'-o',str(out/'compile.o')]
            q=subprocess.run(command,cwd=ROOT,capture_output=True,timeout=60)
            # The two known constraints must remain diagnosed by GCC and
            # accepted by the actual compiler, outside positive arena counts.
            accepted=category=='valid' or (engine=='cross-tcc' and category=='compiler-constraint-gap')
            compiler_rows.append(dict(case=name,category=category,engine=engine,command=command,
                                      exit=q.returncode,stderr=q.stderr.decode()))
            (out/'compiler-matrix.json').write_text(json.dumps(compiler_rows,indent=2)+'\n')
            assert (q.returncode==0)==accepted,(name,engine,q.returncode,q.stderr.decode())
    rtkey=md.runtime_key()
    cmd=['python3','scripts/model_diff.py','--no-report','--require-success',str(sum(c=='valid' for _,c in sources.values())),
         '--json-out',str(out/'fast.json'),'-j','4',*map(str,paths.values())]
    r=subprocess.run(cmd,cwd=ROOT,capture_output=True,timeout=7200)
    (out/'fast.stdout').write_bytes(r.stdout);(out/'fast.stderr').write_bytes(r.stderr)
    (out/'fast.command.json').write_text(json.dumps(cmd)+'\n')
    data=json.loads((out/'fast.json').read_text());rows=[x for xs in data['results'].values() for x in xs]
    assert r.returncode==0, data['totals']
    exact=[];diagnostics=[];constraints=[];erasure=[]
    with md.invocation_lock():
        for name,p in paths.items():
            row=next(x for x in rows if (ROOT/x['name']).resolve()==p)
            category=sources[name][1]
            expected='same-refusal' if category=='diagnostic' else 'same'
            assert row['result']==expected,(name,row)
            dst=out/name;dst.mkdir()
            extra=[(v,str(h)) for v,h in md.fixture_entries(str(p))]
            if expected=='same':
                st,ref,lm=md.ref_run(str(p),extra,['-w'],[],rtkey)
                assert st=='ran' and not lm
                tag=md.sha(os.path.relpath(str(p),str(ROOT)),'c')
                model=Path(md.WORK)/(tag+'.arena')
                assert Path(ref).read_bytes()==model.read_bytes()
                for engine,arena in [('reference',Path(ref)),('model',model)]:
                    (dst/(engine+'.arena.gz')).write_bytes(gzip.compress(arena.read_bytes(),mtime=0))
                (exact if category=='valid' else constraints).append(name)
            else:diagnostics.append(name)
            records=[]
            for mode in ['off','on']:
                stem=dst/mode
                command=[md.MODEL,'--fs',md.FSLIST,'--syms',md.SYMS,'--prog',str(p),'--arg','-w',
                         '--arena-out',str(stem)+'.arena','--console-out',str(stem)+'.console']
                for v,h in extra:command+=['--extra',v+'='+str(Path(h).resolve())]
                if mode=='on':command+=['--ghost-out',str(stem)+'.ghost.json']
                q=subprocess.run(command,cwd=ROOT,capture_output=True,timeout=600)
                for ext,raw in [('stdout',q.stdout),('stderr',q.stderr)]:Path(str(stem)+'.'+ext).write_bytes(raw)
                records.append(dict(command=command,exit=q.returncode,stdout=q.stdout.decode(),stderr=q.stderr.decode(),
                    arena=sha(Path(str(stem)+'.arena')) if Path(str(stem)+'.arena').exists() else None,
                    console=sha(Path(str(stem)+'.console'))))
            equal=all(records[0][k]==records[1][k] for k in ['exit','stdout','stderr','arena','console'])
            erasure.append(dict(case=name,equal=equal,runs=records));assert equal,name
    native=None
    if a.sail_capture:
        command=['python3','scripts/unevaluated_native_check.py','--source',str(paths['batch']),
                 '--sail-capture',str(a.sail_capture.resolve()),'--out',str(out/'native')]
        q=subprocess.run(command,cwd=ROOT,capture_output=True,timeout=7200)
        (out/'native.stdout').write_bytes(q.stdout);(out/'native.stderr').write_bytes(q.stderr)
        native=dict(command=command,exit=q.returncode);assert q.returncode==0,q.stderr.decode()
    assert all(sha(Path(p))==h for p,h in pins.items()) and rtkey==md.runtime_key()
    (out/'report.json').write_text(json.dumps(dict(pins=pins,runtime_key=rtkey,exact=exact,diagnostics=diagnostics,compiler_constraints=constraints,compiler_rows=compiler_rows,erasure=erasure,native=native),indent=2)+'\n')
    print(len(exact),'independent fast arenas;',len(diagnostics),'separate refusals;',len(constraints),'separate compiler constraint acceptances;',len(erasure),'ghost pairs; native=',native is not None)
if __name__=='__main__':main()
