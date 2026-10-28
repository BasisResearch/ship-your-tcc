#!/usr/bin/env python3
"""Project exact shared type-query definitions into the isolated OCaml source.

Only this section is shared: unrelated local IEEE translation constructors have
no pristine OCaml counterpart. The build pins both full sources, this generator,
and the projected result. No syntax, binding identity or floating code is erased.
"""
import argparse
from pathlib import Path

def section(s):
    start=s.index('val ctype_of:')
    end=s.index('val is_lvalue:',start)
    chunk=s[start:end]
    assert chunk.count('let ctype_of ')==chunk.count('let qualified_ctype_of ')==1
    return start,end,chunk

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('source',type=Path);ap.add_argument('target',type=Path)
    a=ap.parse_args();s=a.source.read_text();t=a.target.read_text()
    _,_,shared=section(s);start,end,_=section(t)
    result=t[:start]+shared+t[end:]
    a.target.write_text(result)
    assert section(a.target.read_text())[2]==shared
    print('Projected two exact shared alignment-query definitions')
if __name__=='__main__':main()
