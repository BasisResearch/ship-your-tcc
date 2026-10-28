#!/usr/bin/env python3
"""Strict GCC and freshly patched host/cross compiler diagnostic matrix."""
from pathlib import Path

def check(rows, compilers, run, out):
    observations=[]
    for source,category in rows.items():
        for engine,compiler in compilers.items():
            # GNU consumers test unchanged tcc policy, not strict C syntax.
            if engine=='gcc' and category.startswith('ordinary-'): continue
            q=run(compiler+['-c',source,'-o',str(Path(out)/'assert-message.o')],expected=None)
            accept = category in ('valid','ordinary-consumer') or (category=='extension' and engine!='gcc')
            assert (q.returncode==0)==accept, (source,category,engine,q.returncode,q.stderr)
            if not accept: assert q.stderr,(source,engine,'missing diagnostic')
            if category=='false' and engine!='gcc':
                assert b'before' in q.stderr and b'after' in q.stderr,(source,engine,q.stderr)
                if '-zero' not in source and '-unicode' not in source:
                    assert b'before middle after' in q.stderr,(source,engine,q.stderr)
            observations.append(dict(source=source,category=category,engine=engine,exit=q.returncode,
                                     stderr=q.stderr.decode('utf8','replace')))
    return observations
