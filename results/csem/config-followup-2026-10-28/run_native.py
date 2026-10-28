import sys,concurrent.futures,subprocess,json
from pathlib import Path
sys.path.insert(0,'scripts');import cfg_probes as c
c.OUT=Path('build/cfg-followup')
subprocess.run(['make','-s','-C','c','BUILD=build','FLAGS=0','ELF=build/base.elf','build/base.elf'],check=True)
pins=c.pins();probes=[next(Path('c/tests/cfg').glob(n+'*.c')).resolve() for n in ['p04','p09','p10']]
with concurrent.futures.ThreadPoolExecutor(3) as pool:
 for f in [pool.submit(c.measure,p,'tcc',pins,1800) for p in probes]:f.result()
assert c.pins()==pins
c.summarize(probes,pins)
