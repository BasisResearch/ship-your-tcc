import importlib.util
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
spec=importlib.util.spec_from_file_location('pre_diff',Path(__file__).resolve().parents[1] / 'pre_diff.py')
p=importlib.util.module_from_spec(spec);spec.loader.exec_module(p)

class ProtocolTests(unittest.TestCase):
    def test_empty_success(self):
        self.assertEqual(p.parse_toks('',0),([],None))
    def test_error(self):
        self.assertEqual(p.parse_toks('!\tmacro\t/prog.c:1\tbad\n',1)[1],'macro\t/prog.c:1\tbad')
    def test_crash_cannot_be_empty_agreement(self):
        for rc in [-6,1,124,139]:
            with self.subTest(rc=rc),self.assertRaises(p.HarnessError):p.parse_toks('',rc)
    def test_exit_verdict_consistency(self):
        for out,rc in [('I\tx\n',1),('!\tbad\n',0),('I\tx\n!\tbad\n',1),('!\tbad\nI\tx\n',1),('!\tbad\n!\tbad\n',1)]:
            with self.subTest(out=out,rc=rc),self.assertRaises(p.HarnessError):p.parse_toks(out,rc)
    def test_unknown_records(self):
        for out in ['panic!\n','garbage\tstuff\n','I x\n','!\t\n']:
            with self.subTest(out=out),self.assertRaises(p.HarnessError):p.parse_toks(out,0)
    def test_lexer_failure_cannot_be_empty_agreement(self):
        with tempfile.TemporaryDirectory() as d:
            for rc,out in [(124,''),(1,'!\tbad\n'),(-6,'')]:
                with self.subTest(rc=rc),patch.object(p,'run',return_value=(rc,out,'')),self.assertRaises(p.HarnessError):p.lex_text('int x;',d)
    def test_missing_date_time_is_not_dummy_data(self):
        with tempfile.TemporaryDirectory() as d,patch.object(p,'run',return_value=(0,'','')),self.assertRaises(p.HarnessError):p.date_time(d)

if __name__=='__main__':unittest.main()
