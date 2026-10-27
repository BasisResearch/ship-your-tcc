#!/usr/bin/env python3
"""Exact model/internal-tcc/spec comparison. -E observations are auxiliary.

A success requires byte-for-byte token equality, including kind and pragma
positions, plus equal final pack/counter/library state for the raw reference.
Every refusal, spec error, model limitation, timeout and harness failure is
retained separately. --diagnostic records residuals but never accepts a
harness failure. No C literal decoding or spelling normalisation is used.
"""
import argparse
from collections import Counter
import concurrent.futures
import hashlib
import json
import os
import re
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

import pre_diff as old
from raw_tokens import parse, spec_bytes, e_encoding, ProtocolError
ROOT=Path(__file__).resolve().parents[1]
MODEL=ROOT/'.lake/build/bin/tcc_pre_tokens'
REF=ROOT/'build/pre-tokens-ref/pre_tokens_ref'
SPEC=ROOT/'.lake/build/bin/pre_spec'


def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def run(cmd,timeout=120):
    try:
        p=subprocess.run(list(map(str,cmd)),capture_output=True,timeout=timeout)
        return p.returncode,p.stdout,p.stderr
    except subprocess.TimeoutExpired as e:
        raise ProtocolError('timeout: '+str(cmd[0])) from e


def materialize(path,d):
    """Same virtual names for both raw observers. Header bytes are independent
    image files, never spec/model outputs. First fs.list entry wins."""
    entries=[];seen=set()
    for row in Path(old.FSLIST).read_text().splitlines():
        if not row:continue
        p,h=row.split('=',1)
        if p.endswith(('.a','.o')) or p in seen:continue
        seen.add(p);entries.append((p,Path(h)))
    names=re.findall(rb'#\s*include\s*"([^"/]+)"',path.read_bytes())
    locals_=sorted({n.decode() for n in names if (path.parent/n.decode()).is_file()})
    for name in locals_:
        entries.append(('/'+name,path.parent/name))
    # Headers may themselves include a local header; retain sibling headers
    # for focused once/include fixtures and tests2 multilevel include tests.
    for h in sorted(path.parent.glob('*.h')):
        if '/'+h.name not in {p for p,_ in entries}:entries.append(('/'+h.name,h))
    for p,h in entries:
        target=d/p.lstrip('/')
        target.parent.mkdir(parents=True,exist_ok=True)
        shutil.copyfile(h,target)
    shutil.copyfile(path,d/'prog.c')
    listing=d/'fs.list'
    listing.write_text(''.join(p+'='+str(d/p.lstrip('/'))+'\n' for p,h in entries))
    return listing


def spec_obs(rc,out):
    ts,err=old.parse_toks(out.decode('utf-8'),rc)
    return [(k,spec_bytes(s)) for k,s in ts],err


def context(a,b):
    i=next((i for i,(x,y) in enumerate(zip(a,b)) if x!=y),min(len(a),len(b)))
    def show(xs):return [(k,s.hex()) for k,s in xs[max(0,i-3):i+4]]
    return dict(index=i,left=show(a),right=show(b))


def lex_printed(text,tmp):
    """Tokenize printed phase-4 bytes with phase 3 alone. Repeating physical
    phase 1 on generated literal spellings is a distinct encoding error."""
    lines=[]
    for line in text.split('\n'):
        if re.match(r'\s*#\s*(line\s+)?\d+(\s|$)',line):continue
        m=re.match(r'\s*#\s*pragma\b(.*)$',line)
        lines.append('__PRAGMA_BEGIN__ '+m[1]+' __PRAGMA_END__' if m else line)
    path=tmp/'printed.i';path.write_bytes(('\n'.join(lines)+'\n').encode('latin-1'))
    rc,out,err=run([SPEC,'--lex-printed',path]);ts,se=spec_obs(rc,out)
    if se:raise ProtocolError('printed-token lexer error: '+se)
    return [('[',b'') if t==('I',b'__PRAGMA_BEGIN__') else (']',b'') if t==('I',b'__PRAGMA_END__') else t for t in ts]


def auxiliary(path,d,tmp):
    # Auxiliary evidence uses cfgTccE: tccdefs.h emits parser declarations in
    # the raw -run stream, but suppresses them in -E. These are separate lanes.
    r=old.one(str(path),str(d),str(tmp),True)
    prog=d/'prog.c'
    date,time=old.date_time(str(d))
    rc,out,_=run([SPEC,'--cfg','E','--fslist',d/'fs.list','--date',date,'--time',time,prog])
    ts,err=spec_obs(rc,out)
    enc={};contexts={}
    inc=['-I'+str(d/'lib/tcc/include'),'-I'+str(d/'include')]
    commands={'tcc':[old.TCC,'-std=c11','-E','-nostdinc',*inc,prog],
              'gcc':[old.XGCC,'-std=c11','-E','-undef','-nostdinc',*inc,'-include',d/'predefs.h',prog]}
    tri=re.compile(rb"\?\?[=/'()!<>-]")
    for name,cmd in commands.items():
        rc,out,diag=run(cmd)
        if rc not in (0,1):raise ProtocolError('unexpected auxiliary exit: '+str(rc))
        if err or rc:
            enc[name]='both-error' if err and rc else ('spec-error' if err else 'printer-error')
        else:
            tt=lex_printed(out.decode('latin-1'),tmp)
            # Host path rewriting is confined to this established auxiliary
            # lane. Neither raw observation ever receives host path strings.
            tt=[(k,b.replace(str(d).encode(),b'')) if k=='S' else (k,b) for k,b in tt]
            enc[name]=e_encoding(ts,tt)
            if enc[name]=='exact' and any(k in ('S','C') and tri.search(b) for k,b in tt):
                enc[name]='printer-unprotected-trigraph'
            if enc[name]=='different':contexts[name]=context(ts,tt)
    return {'legacy_phases123_tcc':r['tcc'],'legacy_phases123_gcc':r['gcc'],'encoding':enc,
            'contexts':contexts,'tcc_error':r.get('tcc_err',''),'gcc_error':r.get('gcc_err','')}


def one(path,aux=False):
    with tempfile.TemporaryDirectory(prefix='pre_tokens_') as tmp:
        d=Path(tmp)/'root';d.mkdir()
        listing=materialize(path,d)
        mrc,mout,merr=run([MODEL,'--prog',path,'--fs',listing])
        rrc,rout,rerr=run([REF,'--root',d])
        m,r=parse(mout,mrc),parse(rout,rrc)
        prc,pout,perr=run([REF,'--root',d,'--plain'])
        plain=parse(pout,prc)
        pmrc,pmout,pmerr=run([MODEL,'--prog',path,'--fs',listing,'--plain'])
        plain_model=parse(pmout,pmrc)
        def erase_events(ts):
            depth=0;out=[]
            for k,b in ts:
                if k=='[':depth+=1
                elif k==']':depth-=1
                elif depth==0:out.append((k,b))
            return out
        preserved = ((r.error is not None and plain.error is not None) or
                     (r.error is None and plain.error is None and
                      erase_events(r.tokens)==plain.tokens and r.state==plain.state))
        model_preserved=((m.error is not None and plain_model.error is not None) or
                         (m.error is None and plain_model.error is None and
                          erase_events(m.tokens)==plain_model.tokens and m.state==plain_model.state))
        plain_agree=(plain_model.error is not None and plain.error is not None) or (
            plain_model.error is None and plain.error is None and plain_model.tokens==plain.tokens and plain_model.state==plain.state)
        preserved=preserved and model_preserved and plain_agree
        src,sout,serr=run([SPEC,'--cfg','run','--fslist',listing,path])
        s,se=spec_obs(src,sout)
        row=dict(file=str(path.relative_to(ROOT)) if path.is_relative_to(ROOT) else str(path),
                 source_sha256=digest(path),model_tokens=len(m.tokens),reference_tokens=len(r.tokens),
                 spec_tokens=len(s),observer_preserves_effects=preserved,model_error=m.error,reference_error=r.error,spec_error=se,
                 model_diagnostics=merr.decode('latin-1'),reference_diagnostics=rerr.decode('latin-1'))
        if not preserved:
            row['raw']='observer-effect-difference';row['observer_state']=r.state;row['plain_state']=plain.state
        elif m.error or r.error:
            row['raw']='both-refused' if m.error and r.error and m.error[0]==r.error[0]=='refuse' else (
                'model-error' if m.error and m.error[0]=='model' else 'acceptance-difference')
        elif m.tokens!=r.tokens:
            row['raw']='different';row['raw_context']=context(m.tokens,r.tokens)
        elif m.state!=r.state:
            row['raw']='effect-difference';row['model_state']=m.state;row['reference_state']=r.state
        else:row['raw']='exact';row['state']=m.state
        if se: row['spec']='spec-error'
        elif m.error:row['spec']='compiler-refusal' if m.error[0]=='refuse' else 'model-error'
        elif s==m.tokens:row['spec']='exact'
        else:row['spec']='different';row['spec_context']=context(s,m.tokens)
        # Keep serialization hashes of both observations and the spec, even
        # when diagnostic mode leaves a disagreement unresolved.
        row['observations_sha256']={k:hashlib.sha256(v).hexdigest() for k,v in [('model',mout),('reference',rout),('plain_reference',pout),('plain_model',pmout),('spec',sout)]}
        if aux:
            # Auxiliary uses its established isolated root, never the raw root.
            e=Path(old.make_root(tmp));row['auxiliary']=auxiliary(path,e,Path(tmp))
        return row


def json_default(x):
    if isinstance(x,bytes):return {'hex':x.hex()}
    raise TypeError(type(x).__name__)


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('files',nargs='*',type=Path)
    ap.add_argument('--corpus',action='store_true')
    ap.add_argument('--auxiliary',action='store_true')
    ap.add_argument('--jobs',type=int,default=4)
    ap.add_argument('--diagnostic',action='store_true')
    ap.add_argument('--out',type=Path,required=True)
    a=ap.parse_args()
    if not 1<=a.jobs<=8:ap.error('jobs must be 1..8')
    files=list(a.files)
    if a.corpus:
        for sub in ['compcert','csmith','tcc']:
            if not list((ROOT/'corpus'/sub).glob('*.c')):ap.error('missing corpus/'+sub)
        files += [Path(p) for p in old.default_files(str(ROOT/'corpus'))]
    files=sorted(set(p.resolve() for p in files))
    if not files:ap.error('empty selection')
    pins=[MODEL,REF,SPEC,ROOT/'scripts/pre_tokens_diff.py',ROOT/'scripts/raw_tokens.py',
          ROOT/'scripts/pre_tokens_ref.c',ROOT/'scripts/build_pre_tokens_ref.py',ROOT/'scripts/gen_pre_token_cases.py']
    pins += list((ROOT/'Tcc/Model').glob('*.lean'))+list((ROOT/'Spec/Pre').rglob('*.lean'))
    pins += [ROOT/'Tcc/ModelPreMain.lean',ROOT/'Spec/PreSpec.lean',ROOT/'lakefile.toml',ROOT/'lean-toolchain']
    pins += [ROOT/'scripts/pre_diff.py',ROOT/'scripts/tests/test_raw_tokens.py',ROOT/'scripts/tests/test_pre_tokens_integration.py']
    hashes={str(p.relative_to(ROOT)):digest(p) for p in pins}
    input_paths=set(files)
    for p in files:input_paths.update(p.parent.glob('*.h'))
    input_hashes={str(p):digest(p) for p in sorted(input_paths)}
    fs_pins={h:digest(h) for l in Path(old.FSLIST).read_text().splitlines() if l for p,h in [l.split('=',1)] if p.endswith('.h')}
    ref_build=json.loads((ROOT/'build/pre-tokens-ref/build.json').read_text())
    if digest(REF)!=ref_build['executable_sha256'] or any(digest(ROOT/p)!=h for p,h in ref_build['sha256'].items()):
        raise RuntimeError('reference build is stale')
    def task(p):
        try:return one(p,a.auxiliary)
        except Exception as e:return dict(file=str(p),raw='harness-error',spec='harness-error',error=str(e))
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:rows=list(ex.map(task,files))
    if any(digest(ROOT/p)!=h for p,h in hashes.items()):raise RuntimeError('input changed during run')
    summary={k:dict(Counter(r[k] for r in rows)) for k in ['raw','spec']}
    if a.auxiliary:
        summary['auxiliary']={name:dict(Counter(r.get('auxiliary',{}).get('encoding',{}).get(name,'harness-error') for r in rows)) for name in ['tcc','gcc']}
    if any(digest(p)!=h for p,h in {**input_hashes,**fs_pins}.items()):raise RuntimeError('source/header changed during run')
    report=dict(command=sys.argv,summary=summary,sha256=hashes,input_sha256=input_hashes,header_sha256=fs_pins,
                reference_build=ref_build,rows=rows)
    a.out.parent.mkdir(parents=True,exist_ok=True)
    a.out.write_text(json.dumps(report,indent=2,default=json_default)+'\n')
    print(json.dumps(summary,indent=2))
    if any(r['raw'] in ('harness-error','observer-effect-difference') for r in rows):return 1
    if a.diagnostic:return 0
    return 0 if all(r['raw']==r['spec']=='exact' for r in rows) else 1

if __name__=='__main__':sys.exit(main())
