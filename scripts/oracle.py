#!/usr/bin/env python3
"""Run ONE oracle from scripts/oracles.toml on one program, uncached.

    scripts/oracle.py ORACLE prog.c

stdout and the exit code are the oracle's (see the interface in
scripts/oracles.toml); exit 125 if the oracle is absent. Used by
scripts/minimise.sh's interestingness test.
"""
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from diff3 import ROOT, load_config  # noqa: E402


def main():
    name, prog = sys.argv[1], os.path.abspath(sys.argv[2])
    _, oracles = load_config(os.environ.get('ORACLES_TOML', os.path.join(ROOT, 'scripts', 'oracles.toml')))
    o = oracles[name]
    if o.get('requires') and not os.path.exists(os.path.join(ROOT, o['requires'])):
        print(f'oracle {name} is absent ({o["requires"]})', file=sys.stderr)
        sys.exit(125)
    cmd = o['cmd'].replace('{prog}', prog).replace('{root}', ROOT)
    env = dict(os.environ, **{k: str(v) for k, v in o['env'].items()})
    try:
        r = subprocess.run(['bash', '-c', cmd], cwd=ROOT, env=env, stdin=subprocess.DEVNULL,
                           timeout=o['timeout'])
        sys.exit(r.returncode)
    except subprocess.TimeoutExpired:
        sys.exit(124)


if __name__ == '__main__':
    main()
