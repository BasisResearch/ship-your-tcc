#!/usr/bin/env python3
"""Project the exact bitfield configuration query into the default OCaml oracle.

That oracle remains cerberus-default (bitfields unsupported); cfgTcc's target
bitfield semantics are independently checked using raw Lean, GCC and native Sail.
No OCaml bitfield execution agreement is claimed by this compatibility overlay.
"""
import argparse
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('source',type=Path);p.add_argument('target',type=Path);a=p.parse_args()
s=a.source.read_text();start=s.index('val bitfields_supported:');end=s.index('val integerImpl:',start);chunk=s[start:end]
t=a.target.read_text();i=t.index('val integerImpl:');assert 'val bitfields_supported:' not in t
a.target.write_text(t[:i]+chunk+t[i:]);assert chunk in a.target.read_text()
print('Projected default-configuration bitfield query')
