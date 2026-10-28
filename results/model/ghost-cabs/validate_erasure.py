#!/usr/bin/env python3
"""Finite ordinary CLI ghost-on/off arena and diagnostic comparison, not a proof."""
import hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    exe=ROOT/'.lake/build/bin/tcc_model';binary=sha(exe)
    manifest=json.loads((ROOT/'build/ghost-byte-inputs/manifest.json').read_text())
    dest=ROOT/'build/ghost-erasure-final';dest.mkdir(exist_ok=True)
    rows=[]
    for rel,h in manifest.items():
        src=ROOT/rel;assert sha(src)==h
        name=src.stem;captures={};commands=[]
        for observe in [False,True]:
            tag=dest/(name+('.on' if observe else '.off'))
            cmd=[str(exe),'--fs','c/build/fs.list','--syms','build/image.syms','--prog',str(src),
                 '--arena-out',str(tag)+'.arena','--console-out',str(tag)+'.console']
            if observe:cmd+=['--ghost-out',str(tag)+'.ghost.json']
            p=subprocess.run(cmd,cwd=ROOT,capture_output=True,text=True,timeout=90,
                             env={**os.environ,'LEAN_ABORT_ON_PANIC':'1'})
            commands.append(cmd)
            (Path(str(tag)+'.stdout')).write_text(p.stdout)
            (Path(str(tag)+'.stderr')).write_text(p.stderr)
            assert p.returncode==0 and p.stdout.strip()=='ran',(name,p.returncode,p.stdout,p.stderr)
            captures[observe]=dict(arena_sha256=sha(Path(str(tag)+'.arena')),
                console_sha256=sha(Path(str(tag)+'.console')),stdout=p.stdout,stderr=p.stderr)
        ghost=json.loads((dest/(name+'.on.ghost.json')).read_text())
        equal=captures[True]==captures[False]
        row=dict(source=rel,source_sha256=h,equal=equal,captures=captures,commands=commands,
            ghost_status=ghost['status'],typed_fragments=sum(f['syntax'] is not None for f in ghost['audit']),
            token_errors=len(ghost['token_errors']),pragma_effects=ghost['pragma_effects'])
        rows.append(row);print(name,'equal' if equal else 'DIFFERENT',flush=True)
        assert sha(src)==h
    assert sha(exe)==binary,'binary changed during comparison'
    result=dict(model_sha256=binary,driver_sha256=sha(Path(__file__)),
        source_manifest_sha256=sha(ROOT/'build/ghost-byte-inputs/manifest.json'),rows=rows)
    (dest/'comparison.json').write_text(json.dumps(result,indent=2)+'\n')
    return int(any(not r['equal'] for r in rows))
if __name__=='__main__':raise SystemExit(main())
