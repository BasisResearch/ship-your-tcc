import importlib.util
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

SCRIPTS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SCRIPTS))
spec = importlib.util.spec_from_file_location('model_diff', SCRIPTS / 'model_diff.py')
m = importlib.util.module_from_spec(spec)
spec.loader.exec_module(m)
ARENA = b'TCCARENA 1\nbase 86400000\nentry 86400000\ncode 86400000 4\nsize 4\nABCD'
HANDOFF = b'rv64emu: steps=20 compile_steps=10 exit=- status=handoff\n'
REFUSAL = b'rv64emu: steps=20 compile_steps=- exit=1 status=exit\n'


class ReferenceProtocol(unittest.TestCase):
    def invoke(self, rc, record, arena=ARENA):
        with tempfile.TemporaryDirectory() as d:
            source = Path(d) / 'probe.c'
            source.write_text('int main(void){return 0;}')
            def fake(cmd, **kwargs):
                if arena is not None:
                    Path(cmd[2]).write_bytes(arena)
                Path(cmd[3]).write_bytes(b'')
                return subprocess.CompletedProcess(cmd, rc, b'', record)
            with patch.object(m, 'CACHE', d), patch.object(m.subprocess, 'run', side_effect=fake):
                result = m.ref_run(str(source), [], ['-w'], [], 'test')
                return result[0]

    def test_handoff(self):
        self.assertEqual(self.invoke(0, HANDOFF), 'ran')

    def test_refusal(self):
        self.assertEqual(self.invoke(125, REFUSAL, None), 'exit')

    def test_harness_status_cannot_be_refusal(self):
        for rc in [2, 124, 126, -6]:
            with self.subTest(rc=rc), self.assertRaises(RuntimeError):
                self.invoke(rc, REFUSAL)

    def test_completion_record_required(self):
        for record in [b'', HANDOFF+HANDOFF, b'rv64emu: steps=20 compile_steps=10 exit=0\n', REFUSAL]:
            with self.subTest(record=record), self.assertRaises(RuntimeError):
                self.invoke(0, record)

    def test_payload_required(self):
        for arena in [b'', ARENA[:-1], ARENA.replace(b'TCCARENA 1', b'TCCARENA 2')]:
            with self.subTest(arena=arena), self.assertRaises(RuntimeError):
                self.invoke(0, HANDOFF, arena)

    def test_cache_validates_payload(self):
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / 'arena'
            path.write_bytes(ARENA)
            self.assertEqual(m.parse_arena(str(path))[1], b'ABCD')
            path.write_bytes(ARENA[:-1])
            with self.assertRaises(RuntimeError):
                m.parse_arena(str(path))


if __name__ == '__main__':
    unittest.main()
