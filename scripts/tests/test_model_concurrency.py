"""Regressions for shared scratch/cache races and duplicated observations."""
from pathlib import Path
import select
import subprocess
import sys
import tempfile
import unittest

SCRIPTS = Path(__file__).resolve().parents[1]
CHILD = '''
import sys
sys.path.insert(0, sys.argv[1])
import model_diff as m
m.WORK = sys.argv[2]
with m.invocation_lock():
    print('acquired', flush=True)
    if sys.argv[3] == 'hold':
        input()
        raise SystemExit(int(sys.argv[4]))
'''


class ComparisonConcurrency(unittest.TestCase):
    def line(self, stream):
        self.assertTrue(select.select([stream], [], [], 10)[0], 'child stalled')
        return stream.readline().strip()

    def test_contender_waits_and_lock_releases_on_success_or_error(self):
        for exit_code in [0, 7]:
            with self.subTest(exit_code=exit_code), tempfile.TemporaryDirectory() as d:
                children = []
                try:
                    def launch(mode):
                        p = subprocess.Popen(
                            [sys.executable, '-c', CHILD, str(SCRIPTS), d, mode, str(exit_code)],
                            stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, text=True)
                        children.append(p)
                        return p
                    first = launch('hold')
                    self.assertEqual(self.line(first.stdout), 'acquired')
                    second = launch('enter')
                    self.assertIn('waiting', self.line(second.stderr))
                    self.assertFalse(select.select([second.stdout], [], [], 0)[0])
                    first.stdin.write('\n')
                    first.stdin.flush()
                    self.assertEqual(first.wait(timeout=10), exit_code)
                    self.assertEqual(self.line(second.stdout), 'acquired')
                    self.assertEqual(second.wait(timeout=10), 0)
                finally:
                    for p in children:
                        if p.poll() is None:
                            p.kill()
                        p.communicate(timeout=10)

    def test_duplicate_source_is_rejected_before_comparison(self):
        with tempfile.TemporaryDirectory() as d:
            source = Path(d) / 'duplicate.c'
            source.write_text('int main(void){return 0;}\n')
            p = subprocess.run([sys.executable, str(SCRIPTS / 'model_diff.py'),
                                str(source), str(source), '--no-report'],
                               capture_output=True, text=True, timeout=10)
            self.assertEqual(p.returncode, 2)
            self.assertIn('duplicate input:', p.stderr)


if __name__ == '__main__':
    unittest.main()
