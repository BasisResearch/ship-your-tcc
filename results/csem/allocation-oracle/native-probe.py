from pathlib import Path
import importlib.util,subprocess,json,os
root=Path.cwd();out=root/'results/csem/allocation-oracle'
spec=importlib.util.spec_from_file_location('cfg_probes',root/'scripts/cfg_probes.py');cfg=importlib.util.module_from_spec(spec);spec.loader.exec_module(cfg)
cfg.OUT=out
subprocess.run(['make','-s','-C','c','BUILD=build','FLAGS=0','ELF=build/base.elf','build/base.elf'],check=True)
os.environ['TCC_ARGS']='"-w","-std=c11",'
pins=cfg.pins()
cfg.measure(root/'csem/tests/allocation/global-address.c','tcc',pins,900)
r=json.loads((out/'global-address.tcc.json').read_text())
assert pins==cfg.pins(), 'runtime inputs changed during native observation'
assert r['status']=='defined' and r['exit']==0, r['status']
(out/'global-address.sail.stdout').write_text(r['raw_stdout'])
(out/'global-address.sail.stderr').write_text(r['stderr'])
(out/'global-address.sail.exit').write_text(str(r['exit'])+'\n')
(out/'global-address.sail.meta').write_text('\n'.join(r['meta'])+'\n')
