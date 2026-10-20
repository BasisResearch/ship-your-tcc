#!/usr/bin/env python3
"""Fast regressions for oracle outcome, cache and process ownership."""
import contextlib
import importlib.util
import io
import json
import os
from pathlib import Path
import shlex
import signal
import subprocess
import sys
import tempfile
import time
import unittest
from unittest.mock import patch

SCRIPTS = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('diff3', SCRIPTS / 'diff3.py')
diff3 = importlib.util.module_from_spec(spec)
spec.loader.exec_module(diff3)
console_spec = importlib.util.spec_from_file_location('sail_console', SCRIPTS / 'sail_console.py')
console = importlib.util.module_from_spec(console_spec)
console_spec.loader.exec_module(console)


class OracleRegression(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix='tcc-diff3-test-')
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        for key, value in [('ROOT', self.tmp.name),
                           ('OUT', str(self.root / 'results')),
                           ('CACHE', str(self.root / 'cache'))]:
            p = patch.object(diff3, key, value)
            p.start()
            self.addCleanup(p.stop)

    def oracle(self, cmd, **extra):
        return dict(cmd=cmd, env={}, heavy=False, timeout=5, **extra)

    def test_console_preserves_diagnostic_looking_program_bytes(self):
        raw = b'PC = 0x123\nTODO: cancel_reservation\n\xff\x00\nSUCCESS\n'
        encoded = raw.decode('latin-1').encode('utf-8')
        self.assertEqual(console.decode_console(encoded), raw)

    def test_signal_is_incomparable_but_nonzero_main_return_is_ok(self):
        failed = diff3.run_oracle('signal', self.oracle('kill -TERM $$'),
                                 'unused', 'signal', diff3._Null(), False)
        self.assertEqual(failed['outcome'], 'n/a')
        self.assertEqual(failed['exit'], -signal.SIGTERM)
        ordinary = diff3.run_oracle('return', self.oracle('exit 6'),
                                   'unused', 'return', diff3._Null(), False)
        self.assertEqual((ordinary['outcome'], ordinary['exit']), ('ok', 6))
        self.assertEqual(diff3.compare(failed, failed), 'incomparable')

    def test_missing_guest_exit_metadata_is_not_a_program_result(self):
        o = self.oracle('exit 2', completion_meta=['steps'])
        result = diff3.run_oracle('sail', o, 'unused', 'missing-meta', diff3._Null(), False)
        self.assertEqual(result['outcome'], 'n/a')
        self.assertIn('completion metadata', result['stderr_tail'])

    def test_timeout_stops_descendant_before_it_can_write(self):
        marker = self.root / 'survived'
        child = f'import time; time.sleep(0.8); open({str(marker)!r}, "w").write("orphan")'
        cmd = f'{shlex.quote(sys.executable)} -c {shlex.quote(child)} & wait'
        rc, out, err, killed = diff3.run_process(cmd, os.environ.copy(), 0.15)
        self.assertTrue(killed)
        self.assertEqual(diff3.oracle_outcome(rc, {}, killed), 'timeout')
        time.sleep(0.9)
        self.assertFalse(marker.exists(), 'timed-out oracle left a live descendant')

    def test_cache_version_tracks_patch_changes_and_status_mappings(self):
        patches = self.root / 'patches'
        patches.mkdir()
        source = patches / 'one.patch'
        source.write_text('first')
        o = self.oracle('true', version_files=['patches/*.patch'])
        first = diff3.oracle_version('test', o)
        source.write_text('second')
        second = diff3.oracle_version('test', o)
        self.assertNotEqual(first, second)
        o['codes'] = {'126': 'stuck'}
        self.assertNotEqual(second, diff3.oracle_version('test', o))

    def test_changed_source_drops_unselected_old_oracles(self):
        corpus = self.root / 'corpus/csmith'
        corpus.mkdir(parents=True)
        (corpus / 'one.c').write_text('int main(void) { return 0; }\n')
        output = self.root / 'results/csmith'
        output.mkdir(parents=True)
        record = output / 'one.json'
        record.write_text(json.dumps({'sha256': 'previous source',
                                      'oracles': {'stale': {'outcome': 'ok'}}}))
        config = self.root / 'oracles.toml'
        config.write_text('[oracle.gcc_native]\ncmd = "printf fresh"\n')
        args = ['diff3.py', '--config', str(config), '--corpus', 'csmith',
                '--oracles', 'gcc_native', '--no-cache', '--jobs', '1']
        with patch.object(sys, 'argv', args), contextlib.redirect_stdout(io.StringIO()), \
                contextlib.redirect_stderr(io.StringIO()):
            diff3.main()
        result = json.loads(record.read_text())
        self.assertEqual(set(result['oracles']), {'gcc_native'})
        self.assertEqual(result['oracles']['gcc_native']['stdout'], 'fresh')

    def test_reducer_cli_rejects_configured_stuck_status(self):
        config = self.root / 'oracle.toml'
        config.write_text('[oracle.test]\ncmd = "exit 126"\ncodes = {126 = "stuck"}\n')
        env = dict(os.environ, ORACLES_TOML=str(config))
        default = subprocess.run([sys.executable, str(SCRIPTS / 'oracle.py'),
                                  'test', 'unused.c'], env=env)
        checked = subprocess.run([sys.executable, str(SCRIPTS / 'oracle.py'),
                                  '--require-ok', 'test', 'unused.c'], env=env)
        self.assertEqual(default.returncode, 126)
        self.assertEqual(checked.returncode, 125)


if __name__ == '__main__':
    unittest.main()
