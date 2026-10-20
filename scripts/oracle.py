#!/usr/bin/env python3
"""Run ONE oracle from scripts/oracles.toml on one program, uncached.

    scripts/oracle.py ORACLE prog.c

stdout and the exit code are the oracle's (see the interface in
scripts/oracles.toml); exit 125 if the oracle is absent. Used by
scripts/minimise.sh's interestingness test.
"""
import os
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from diff3 import ROOT, load_config, missing_completion_meta, oracle_outcome, run_process  # noqa: E402


def main():
    require_ok = sys.argv[1:2] == ['--require-ok']
    argv = sys.argv[2:] if require_ok else sys.argv[1:]
    name, prog = argv[0], os.path.abspath(argv[1])
    _, oracles = load_config(os.environ.get('ORACLES_TOML', os.path.join(ROOT, 'scripts', 'oracles.toml')))
    o = oracles[name]
    if o.get('requires') and not os.path.exists(os.path.join(ROOT, o['requires'])):
        print(f'oracle {name} is absent ({o["requires"]})', file=sys.stderr)
        sys.exit(125)
    cmd = o['cmd'].replace('{prog}', prog).replace('{root}', ROOT)
    # The reducer must enforce the same guest-completion contract as diff3.
    with tempfile.TemporaryDirectory(prefix='tcc-oracle-') as work:
        meta_path = os.path.join(work, 'oracle.meta')
        env = dict(os.environ, ORACLE_META=meta_path, **{k: str(v) for k, v in o['env'].items()})
        rc, _, _, killed = run_process(cmd, env, o['timeout'], capture_output=False)
        meta = {}
        if os.path.exists(meta_path):
            with open(meta_path) as source:
                for line in source:
                    if '=' in line:
                        key, value = line.rstrip('\n').split('=', 1)
                        meta[key] = value
    outcome = oracle_outcome(rc, o, killed)
    if outcome == 'timeout':
        sys.exit(124)
    if rc < 0 or (outcome == 'ok' and missing_completion_meta(o, meta)) or (require_ok and outcome != 'ok'):
        sys.exit(125)
    sys.exit(rc)


if __name__ == '__main__':
    main()
