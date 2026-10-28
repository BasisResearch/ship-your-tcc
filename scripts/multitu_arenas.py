#!/usr/bin/env python3
"""Independent fast RV64 arenas for the exact ordered multi-TU fixtures.

No new native Sail observation is claimed. Matching compiler diagnostics are
separate from successful arena equality and from independent raw Cabs success.
"""
import argparse,json,shutil,subprocess
from pathlib import Path
from ghost_cabs_cmp import ROOT,sha,source_pins,build_receipt
import model_diff as md

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--build-id',required=True);ap.add_argument('--comparison',type=Path,required=True);ap.add_argument('--out',type=Path,required=True);a=ap.parse_args()
 receipt=build_receipt(a.build_id);pins=source_pins();binary=sha(ROOT/'.lake/build/bin/tcc_model');runtime=md.runtime_key();symbols=sha(ROOT/'build/image.syms')
 data=json.loads(a.comparison.read_text());assert data['source_hashes']==pins
 assert all(sha(Path(p))==h for p,h in data['input_pins'].items())
 assert data['runtime_key']==runtime and data['image_symbols_sha256']==symbols
 out=a.out.resolve();out.mkdir(parents=True,exist_ok=False);rows=[]
 with md.invocation_lock():
  for case in data['rows']:
   units=case['expected_units'];main=units[-1]['source'];extras=[]
   files=Path(case['runs'][1]['command'][case['runs'][1]['command'].index('--ghost-out')+1]).parent/'files.json'
   # Use the exact same virtual files and physical bytes. They are retained
   # separately from the model and raw exports and never reconstructed from Cabs.
   directory=out/case['case'];directory.mkdir()
   for i,(virtual,contents) in enumerate(json.loads(files.read_text())):
    path=directory/f'file-{i}';path.write_bytes(contents.encode());extras.append((virtual,str(path)))
   args=['-w']+[u['filename'] for u in units[:-1]]
   invocation=dict(source=main,extra=extras,args=args,argv=['tcc','-std=c11',*args,'-run','/prog.c'])
   row=md.compare(main,'c',runtime,invocation=invocation)
   wanted='same-refusal' if case['category']=='diagnostic' else 'same'
   (directory/'comparison.json').write_text(json.dumps(row,indent=2)+'\n')
   assert row['result']==wanted,(case['case'],row)
   refstatus,refdata,lm=md.ref_run(main,extras,args,[],runtime)
   assert not lm,'unexpected implicit runtime library change'
   if wanted=='same':
    model=Path(md.WORK)/(md.sha(row['name'],'c')+'.arena')
    reference=Path(refdata)
    assert model.read_bytes()==reference.read_bytes()==Path(case['runs'][0]['command'][case['runs'][0]['command'].index('--arena-out')+1]).read_bytes()
    shutil.copyfile(model,directory/'model.arena');shutil.copyfile(reference,directory/'reference.arena')
   else:
    assert refstatus=='exit';(directory/'reference.console').write_bytes(refdata[1])
   row.update(case=case['case'],category=case['category']);rows.append(row)
   print(case['case'],wanted,flush=True)
 assert source_pins()==pins and sha(ROOT/'.lake/build/bin/tcc_model')==binary and md.runtime_key()==runtime and sha(ROOT/'build/image.syms')==symbols
 (out/'report.json').write_text(json.dumps(dict(build=receipt,source_hashes=pins,runtime_key=runtime,image_symbols_sha256=symbols,binary_sha256=binary,input_pins=data['input_pins'],comparison_sha256=sha(a.comparison),rows=rows),indent=2)+'\n')
if __name__=='__main__':main()
