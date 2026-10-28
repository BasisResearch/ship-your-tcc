from pathlib import Path
import json,hashlib,subprocess,sys
root=Path('/data/home/kirancodes/Documents/code/ship-your-tcc');wt=root/'.claude/worktrees/sol-csem-repin';dest=wt/'build/ghost-integration-identity/native-recomparison';dest.mkdir(exist_ok=True)
sys.path.insert(0,str(root/'scripts'))
from source_fixtures import fixture_entries
from compare_sail_arena import compare
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
evidence=root/'results/model/corpus-fixtures-2026-10-28';runtime=json.loads((evidence/'runtime-inputs.json').read_text());assert all(sha(root/p)==h for p,h in runtime.items())
exe=wt/'.lake/build/bin/tcc_model';exehash=sha(exe);syms=root/'build/image.syms';symhash=sha(syms);rows=[]
for case in json.loads((evidence/'capture-index.json').read_text()):
 source=root/case['source'];name=source.stem;arena=dest/(name+'.arena');console=dest/(name+'.console')
 previous=json.loads((evidence/case['capture']/'reference.sail.json').read_text());assert sha(source)==previous['source_sha256']
 fixtures=fixture_entries(source);pins={str(p):sha(p) for _,p in fixtures}
 cmd=[str(exe),'--fs',str(root/'c/build/fs.list'),'--syms',str(syms),'--prog',str(source),'--arena-out',str(arena),'--console-out',str(console),'--arg','-w']
 for virtual,p in fixtures:cmd+=['--extra',virtual+'='+str(p)]
 p=subprocess.run(cmd,capture_output=True,timeout=600)
 (dest/(name+'.stdout')).write_bytes(p.stdout);(dest/(name+'.stderr')).write_bytes(p.stderr)
 assert p.returncode==0 and p.stdout.strip()==b'ran',(name,p.returncode,p.stdout,p.stderr)
 compared=compare(evidence/case['capture']/'sail',arena);assert compared['status']=='equal' and compared['compared_bytes']==4194304
 assert all(sha(p)==h for p,h in pins.items());rows.append(dict(source=case['source'],command=cmd,source_and_fixture_pins=pins,retained_capture=case['capture'],comparison=compared));print(name,'complete retained Sail arena equal',flush=True)
assert sha(exe)==exehash and sha(syms)==symhash;assert all(sha(root/p)==h for p,h in runtime.items())
(dest/'comparison.json').write_text(json.dumps(dict(rows=rows,model_sha256=exehash,symbols_sha256=symhash,driver_sha256=sha(__file__),native_observations='Reused unchanged complete captures from corpus-fixtures-2026-10-28; model execution is fresh, Sail was not rerun.'),indent=2)+'\n')
