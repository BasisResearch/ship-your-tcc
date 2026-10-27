"""Protocol checks using the retained real Sail capture, without rerunning Sail."""
import gzip
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

SCRIPTS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SCRIPTS))
import compare_sail_arena as capture
import model_diff

ROOT = SCRIPTS.parent
FIXTURE = ROOT / 'results/model/trigraph-sail-capture'


class SailCaptureProtocol(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.cut = self.root / 'cut'
        shutil.copytree(FIXTURE, self.cut)
        self.model = self.root / 'model.arena'
        self.model.write_bytes(gzip.decompress((FIXTURE / 'model.arena.gz').read_bytes()))

    def test_complete_retained_capture(self):
        result = capture.compare(self.cut, self.model, ROOT / 'results/pre/trigraphs-sail.trace')
        self.assertEqual(result['compared_bytes'], 4194304)
        self.assertEqual(result['steps'], 3390238)

    def test_trailing_byte_is_checked(self):
        data = bytearray(gzip.decompress((self.cut / 'arena.bin.gz').read_bytes()))
        data[-1] = 1
        (self.cut / 'arena.bin.gz').write_bytes(gzip.compress(data))
        with self.assertRaisesRegex(ValueError, 'capture file changed'):
            capture.compare(self.cut, self.model)

    def test_model_byte_is_checked(self):
        data = bytearray(self.model.read_bytes())
        data[-1] ^= 1
        self.model.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'arena mismatch'):
            capture.compare(self.cut, self.model)

    def test_metadata_is_checked(self):
        self.model.write_bytes(self.model.read_bytes().replace(b'entry 8640021c', b'entry 86400220'))
        with self.assertRaisesRegex(ValueError, 'handoff metadata differs'):
            capture.compare(self.cut, self.model)

    def test_trace_step_is_checked(self):
        trace = self.root / 'trace'
        trace.write_text((ROOT / 'results/pre/trigraphs-sail.trace').read_text().replace('3390238', '3390239'))
        with self.assertRaisesRegex(ValueError, 'trace step differs'):
            capture.compare(self.cut, self.model, trace)

    def test_capture_failure_cannot_publish_success(self):
        source = self.root / 'prog.c'
        source.write_text('int main(void){return 0;}')
        def fake_reference(cmd, **kwargs):
            Path(cmd[2]).write_bytes(self.model.read_bytes())
            Path(cmd[3]).write_bytes(b'')
            return subprocess.CompletedProcess(cmd, 0, b'',
                b'rv64emu: steps=20 compile_steps=10 exit=- status=handoff\n')
        with patch.object(model_diff, 'CACHE', str(self.root)), \
             patch.object(model_diff.subprocess, 'run', side_effect=fake_reference), \
             patch.object(model_diff, 'capture_sail', side_effect=RuntimeError('capture failed')):
            with self.assertRaisesRegex(RuntimeError, 'capture failed'):
                model_diff.ref_run(str(source), [], ['-w'], [], 'test', {'test': True})
        self.assertEqual(list(self.root.glob('*.status')), [])

    def test_cached_capture_is_revalidated(self):
        base = str(self.root / 'cache')
        shutil.copyfile(self.model, base + '.arena')
        shutil.copytree(self.cut, base + '.sail')
        result = capture.compare(Path(base + '.sail'), Path(base + '.arena'))
        result['capture_settings'] = {'test': True}
        Path(base + '.sail.json').write_text(json.dumps(result))
        self.assertEqual(model_diff.validate_sail_cache(base, {'test': True})['status'], 'equal')
        Path(base + '.sail/arena.bin.gz').write_bytes(gzip.compress(b'bad'))
        with self.assertRaisesRegex(ValueError, 'capture file changed'):
            model_diff.validate_sail_cache(base, {'test': True})


if __name__ == '__main__':
    unittest.main()
