#!/usr/bin/env python3
"""Validate and assemble anonymous-member evidence against frozen sources."""
from pathlib import Path
from collections import Counter
import hashlib,json,subprocess
ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'results/csem/anonymous-members'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(name):return json.loads((OUT/name).read_text())
def verify_request(req,result):
    r=read(req);s=read(result)
    assert r['request_id']==s.get('request_id',s.get('request')),(req,result)
    assert s['exit']==0 and s.get('source_hashes_verified'),s
    for p,h in r['sha256'].items():assert sha(ROOT/p)==h,p
    return dict(request_id=r['request_id'],receipt=s)
def rows_current(rows,root=ROOT):
    for r in rows:
        p=root/r.get('source',r.get('name'))
        assert sha(p)==r.get('source_sha256',r.get('sha256')),p

def main():
    focused=read('focused.json');assert len(focused['rows'])==92
    assert all(r['ok'] for r in focused['rows'])
    for r in focused['rows']:
        assert sha(ROOT/'csem/tests/anonymous'/r['name'])==r['source_sha256']
        if not r['invalid']:assert r['core_ok']
    union=read('union.json');assert len(union['rows'])==34 and all(r['ok'] for r in union['rows'])
    for r in union['rows']:assert sha(ROOT/'csem/tests/union-init'/(r['name']+'.c'))==r['source_sha256']
    cont=read('continuation.json');rows_current(cont['rows']);assert len(cont['rows'])==58
    gaps=[r for r in cont['rows'] if r['result']!='same']
    assert len(gaps)==4
    for r in gaps:
        assert any(s in r['name'] for s in ['designator_padding_','designator_trailing_padding_'])
        assert 'bit-fields' in r['ocaml']['stderr'] and r['lean']['exit']!=0
    for group in [focused,union,cont]:
        for p,h in group.get('binaries',group.get('engines',{})).items():assert sha(p)==h,p
    cross=read('cross-sail.json');rows_current(cross['rows']);assert len(cross['rows'])==78
    assert sum(r['tcc_gap'] for r in cross['rows'])==4
    for p,h in cross['binaries'].items():assert sha(p)==h,p
    strict=read('strict-gcc-controls.json');rows_current(strict['rows']);assert len(strict['rows'])==92
    assert all(r['gcc_ok'] for r in strict['rows'])
    pristine=read('pristine.json');rows_current(pristine['rows']);assert sha(pristine['oracle'])==pristine['binary_sha256']
    residuals=read('minimized-and-residuals.json');rows_current(residuals['rows'])
    for p,h in residuals['binaries'].items():assert sha(p)==h,p
    for r in residuals['rows'][:4]:
        expected={'anonymous-continuation.c':3,'anonymous-designated.c':3,'anonymous-offset.c':12,'anonymous-size.c':16}[Path(r['source']).name]
        assert r['gcc']['exit']==expected
        for k in ['ocaml','lean']:assert r[k]['exit']==0 and f'Specified({expected})' in r[k]['stdout']
    parsers={}
    for f,want in [('parser-full.tsv',{'AGREE':614,'BOTH_REJECT':41,'CPP_FAIL':9}),('parser-focused.tsv',{'AGREE':93})]:
        c=Counter(l.split('\t')[2] for l in (OUT/f).read_text().splitlines()[1:])
        assert dict(c)==want,(f,c);parsers[f]=dict(c)
    receipts={
        'build':verify_request('build-07-request.json','build-07-result.json'),
        'oracle':verify_request('oracle-05-request.json','oracle-05-result.json'),
        'gate':verify_request('gate-02-request.json','gate-02-result.json'),
    }
    native=read('native.json')
    for r in native['rows']:
        assert sha(ROOT/r['source'])==r['source_sha256']
        assert r['exit']==r['expected'] and r['stdout']=='' and r['isa_exit']==0
        assert len(r['trace_rows'])==3 and 'TRACE-FUEL-OUT' not in r['trace']
    assert sha(OUT/'native-78.c')==sha(ROOT/'csem/tests/anonymous-batch.c')
    inputs=[ROOT/p for p in focused['shared_sources']]+[ROOT/'csem/lean_frontend/CerbMem.lean',ROOT/'csem/lean_frontend/CerbMem_lemMeasureProofs.lean',ROOT/'patches/csem/0001-union-layout-tags.patch']
    for p,h in focused['shared_sources'].items():assert sha(ROOT/p)==h
    protected=['PHASES.md','STATUS.md','Tcc/CSem/Supported.lean','Tcc/Ccp/Sum','Tcc/Capture/Sum','csem/lean_frontend/CerbFloat.lean']
    subprocess.run(['git','diff','--exit-code','--',*protected],cwd=ROOT,check=True,capture_output=True)
    source_pins={str(p.relative_to(ROOT)):sha(p) for p in inputs}
    gate_log=ROOT/receipts['gate']['receipt']['log']
    text=gate_log.read_text()
    assert 'csem_gates: OK' in text and 'check: OK' in text
    result=dict(pins=(ROOT/'vendor/CSEM_REV').read_text(),sources=source_pins,
        focused=dict(positive=78,invalid=14,passed=92,core_text_parses=78),union=dict(passed=34,conforming=33,inherited_excess_control=1),
        continuation=dict(attempted=58,defined_agreements=54,bitfield_gaps=[r['name'] for r in gaps]),
        compiler=dict(cases=78,sail_agreements=74,qualifier_counterexamples=[r['source'] for r in cross['rows'] if r['tcc_gap']]),
        parser=parsers,receipts=receipts,native=native,
        binaries=focused['binaries'],parser_binary={str(ROOT/'.lake/build/bin/csem_cparse'):sha(ROOT/'.lake/build/bin/csem_cparse')},
        inventory_sha256=sha(ROOT/'build/anonymous-proof-inventory.txt'),
        claim='Finite observations and kernel-checked fuel proof adaptation only; no compiler/model refinement or all-C11 coverage claim.')
    (OUT/'validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print('anonymous validation: frozen sources, paired outcomes, parser, full gate and retained gaps verified')
if __name__=='__main__':main()
