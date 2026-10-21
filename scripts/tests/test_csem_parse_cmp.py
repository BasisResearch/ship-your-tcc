#!/usr/bin/env python3
"""Fail-closed regressions for F1 parser evidence (no Lean build required)."""
import concurrent.futures
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location('csem_parse_cmp', Path(__file__).resolve().parents[1] / 'csem_parse_cmp.py')
cmp = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = cmp
spec.loader.exec_module(cmp)


class Outcomes(unittest.TestCase):
    def test_oracle_rejection_requires_known_diagnostic_and_exit(self):
        diagnostic = b'/tmp/x.c:3:4: error: unexpected token after x\nsource\n^\n'
        self.assertEqual(cmp.oracle_status(cmp.Run(1, err=diagnostic)), 'ORACLE_REJECT')
        for r in [cmp.Run(-11, err=diagnostic), cmp.Run(2, err=diagnostic),
                  cmp.Run(1), cmp.Run(124), cmp.Run(None, timeout=True),
                  cmp.Run(1, err=b'Fatal error: exception Stack_overflow'),
                  cmp.Run(1, out=b'{}', err=diagnostic),
                  cmp.Run(1, err=diagnostic + b'Fatal error: exception Failure\n'),
                  cmp.Run(1, err=b'x.c:1:1: error: internal failure')]:
            self.assertNotEqual(cmp.oracle_status(r), 'ORACLE_REJECT')

    def test_oracle_success_requires_json_object(self):
        for payload in [b'', b'null', b'[]', b'{}', b'{}\n{}', b'broken', b'{"tag":"TUnit","decls":[]}']:
            self.assertEqual(cmp.oracle_status(cmp.Run(0, payload)), 'ORACLE_PROTOCOL')
        self.assertEqual(cmp.oracle_status(cmp.Run(0, json.dumps({'tag': 'TUnit', 'decls': [], 'digest': '0'*32}).encode())), 'ORACLE_OK')

    def test_all_lean_status_exit_combinations(self):
        for status, rc in cmp.CMP_EXIT.items():
            accepted = status not in ('BOTH_REJECT', 'LEAN_ONLY')
            good = cmp.Run(rc, f'{status}\tdetail\n'.encode())
            self.assertEqual(cmp.compare_status(good, accepted), (status, 'detail'))
            self.assertEqual(cmp.compare_status(good, not accepted)[0], 'LEAN_PROTOCOL')
            for bad in [cmp.Run(1-rc, good.out), cmp.Run(-9, good.out),
                        cmp.Run(139, good.out), cmp.Run(0, good.out, timeout=True),
                        cmp.Run(rc, good.out, b'unexpected stderr'),
                        cmp.Run(rc, good.out + b'extra\n')]:
                self.assertNotEqual(cmp.compare_status(bad, accepted)[0], status)

    def test_unexpected_lean_output(self):
        for out in [b'', b'?', b'UNKNOWN\tx\n', b'AGREE\n', b'x\nAGREE\tx\n']:
            self.assertEqual(cmp.compare_status(cmp.Run(0, out), True)[0], 'LEAN_PROTOCOL')

    def test_timeout_is_distinct_from_exit_124(self):
        with patch.object(subprocess, 'run', side_effect=subprocess.TimeoutExpired(['cmd'], 1)):
            self.assertTrue(cmp.run(['cmd']).timeout)
        with patch.object(subprocess, 'run', return_value=subprocess.CompletedProcess(['cmd'], 124, b'', b'')):
            self.assertFalse(cmp.run(['cmd']).timeout)

    def test_executable_missing(self):
        with patch.object(subprocess, 'run', side_effect=FileNotFoundError('missing executable')):
            self.assertEqual(cmp.oracle_status(cmp.run(['cmd'])), 'ORACLE_CRASH')

    def test_never_compare_after_oracle_crash(self):
        with tempfile.TemporaryDirectory() as tmp:
            with patch.object(cmp, 'run', side_effect=[cmp.Run(0, b'pp\n'), cmp.Run(-11)]) as run:
                row = cmp.one('x', '/x/a.c', 'oracle', 'rt', 'lean', False, Path(tmp))
                self.assertEqual(row[2], 'ORACLE_CRASH')
                self.assertEqual(run.call_count, 2)

    def test_parallel_same_basename_keeps_distinct_inputs(self):
        with tempfile.TemporaryDirectory() as tmp:
            def invoke(cmd, timeout):
                if cmd[0] == 'cc':
                    return cmp.Run(0, cmd[-1].encode() + b'\n')
                if cmd[0] == 'oracle':
                    return cmp.Run(0, json.dumps({'tag': 'TUnit', 'decls': [], 'digest': '0'*32}).encode())
                self.assertEqual(Path(cmd[2]).read_text(), cmd[3])
                return cmp.Run(0, b'AGREE\tok\n')
            with patch.object(cmp, 'run', side_effect=invoke):
                with concurrent.futures.ThreadPoolExecutor(2) as pool:
                    rows = list(pool.map(lambda f: cmp.one('adhoc', f, 'oracle', 'rt', 'lean', False, Path(tmp)), ['/x/same.c', '/y/same.c']))
            self.assertEqual([r[2] for r in rows], ['AGREE', 'AGREE'])
            self.assertEqual(len(list(Path(tmp).glob('adhoc/*/input.i'))), 2)

    def main_exit(self, args):
        with patch.object(sys, 'argv', ['cmp'] + args), contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit) as e:
                cmp.main()
            return e.exception.code

    def test_empty_and_unknown_corpora(self):
        with patch.object(cmp, 'CORPORA', {'empty': []}):
            self.assertEqual(self.main_exit(['--corpus', 'empty']), 2)
            self.assertEqual(self.main_exit(['--corpus', 'unknown']), 2)

    def test_report_and_gate_failures(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src = root / 'x.c'
            src.write_text('int x;')
            report = root / 'report.tsv'
            for status in ['AGREE', 'BOTH_REJECT', 'CPP_FAIL', 'LEAN_ONLY', 'LEAN_CRASH', 'ORACLE_TIMEOUT', 'ORACLE_CRASH', 'ROUNDTRIP', 'UNKNOWN']:
                row = ('adhoc', str(src), status, ['detail'])
                with patch.object(cmp, 'CACHE', root), patch.object(cmp, 'env_paths', return_value=('oracle', 'rt')), patch.object(cmp, 'one', return_value=row):
                    self.assertEqual(self.main_exit(['--report', str(report), str(src)]), 0 if status in cmp.PASS else 1)
                self.assertIn(status, report.read_text())


if __name__ == '__main__':
    unittest.main()
