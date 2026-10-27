#!/usr/bin/env python3
"""Validate final frozen build/comparison receipts and split residual evidence."""
from pathlib import Path
import hashlib
import json
import re

ROOT=Path(__file__).resolve().parents[1]

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    request=json.loads((ROOT/'build/worker-build.request').read_text())
    result=json.loads((ROOT/'build/worker-build.result').read_text())
    assert request['request_id']==result['request_id']
    assert result['exit']==0 and result['source_hashes_verified']
    assert all(sha(ROOT/p)==h for p,h in request['sha256'].items())
    assert 'MemoryMax=12G' in result['command'] and result['command'][0]=='flock'
    log=ROOT/result['log'];text=log.read_text()
    assert 'Build completed successfully' in text
    for name in ['preprocess','PreprocessCorrect_Statement','PragmaObservationEffects_Statement','PreprocessObservationNoninterference_Statement']:
        assert "'TccModel."+name+"' depends on axioms: [propext, Classical.choice, Quot.sound]" in text
    tests=(ROOT/'build/token-tests.log').read_text()
    static=(ROOT/'build/token-static.log').read_text()
    assert '\nOK\n' in tests and 'skipped' not in tests
    methods=int(re.search(r'Ran ([0-9]+) tests',tests)[1])
    assert 'check: static stages OK' in static
    summaries={};errors={};residuals={}
    report_paths=['results/pre/token-focused.json','results/pre/token-corpus.json']
    for path in report_paths:
        r=json.loads((ROOT/path).read_text())
        assert all(sha(ROOT/p)==h for p,h in r['sha256'].items())
        assert all(sha(p)==h for p,h in {**r['input_sha256'],**r['header_sha256']}.items())
        assert not any(x['raw'] in ['harness-error','observer-effect-difference'] for x in r['rows'])
        assert all(x['observer_preserves_effects'] for x in r['rows'])
        assert all(x['raw'] in ['exact','both-refused'] for x in r['rows']),r['summary']
        summaries[path]=dict(attempted=len(r['rows']),**r['summary'])
        errors[path]=[x for x in r['rows'] if x['model_error'] or x['reference_error'] or x['spec_error']]
        residuals[path]=[x for x in r['rows'] if x['spec']!='exact' and not x['spec_error'] and not x['model_error']]
    dest=ROOT/'results/pre'
    (dest/'token-builds/tests.log').write_text(tests)
    (dest/'token-builds/static.log').write_text(static)
    final=dict(request=request,result=result,build_log_sha256=sha(log),test_methods=methods,summaries=summaries,
               sha256={p:sha(ROOT/p) for p in report_paths+['scripts/freeze_pre_tokens.py','results/pre/token-builds/tests.log','results/pre/token-builds/static.log']},
               limitations=['Finite host internal-token observation, not a Sail refinement proof.',
                            'Spec/model equality and whole-run observation noninterference remain typed Statements.',
                            'Supported and the frozen sum artifacts are unchanged.'])
    (dest/'token-build.json').write_text(json.dumps(final,indent=2)+'\n')
    (dest/'token-errors.json').write_text(json.dumps(errors,indent=2)+'\n')
    (dest/'token-residuals.json').write_text(json.dumps(residuals,indent=2)+'\n')
    (dest/'token-builds'/('request-'+request['request_id'].split('-')[-1]+'.json')).write_text(json.dumps(request,indent=2)+'\n')
    (dest/'token-builds'/('result-'+request['request_id'].split('-')[-1]+'.json')).write_text(json.dumps(result,indent=2)+'\n')
    (dest/'token-builds'/('build-'+request['request_id'].split('-')[-1]+'.log')).write_text(text)
    print(json.dumps(summaries,indent=2))

if __name__=='__main__':main()
