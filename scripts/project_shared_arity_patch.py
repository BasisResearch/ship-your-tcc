#!/usr/bin/env python3
"""Omit exactly the adopted arity hunks already present in shared Lem sources.

Verify each complete hunk's post-image against the shared source first. No
reverse/force application, fuzz, or change to any completed cache is allowed.
"""
import argparse,re
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('patch',type=Path);p.add_argument('root',type=Path);p.add_argument('output',type=Path);a=p.parse_args()
skip={'frontend/model/ail/ailTypesAux.lem','frontend/model/ail/genTypesAux.lem'}
blocks=re.split(r'(?=^--- a/)',a.patch.read_text(),flags=re.M);out=[];seen=set()
for block in blocks:
 if not block:continue
 path=block.splitlines()[0].removeprefix('--- a/')
 if path not in skip:out.append(block);continue
 source=(a.root/path).read_text();seen.add(path)
 for hunk in re.split(r'^@@[^\n]*\n',block,flags=re.M)[1:]:
  post=''.join(line[1:] for line in hunk.splitlines(True) if line.startswith((' ','+')))
  assert post and post in source,f'shared source lacks exact post-image: {path}'
assert seen==skip,seen
a.output.write_text(''.join(out));print('Verified and omitted two already shared arity sections')
