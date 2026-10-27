"""Golden spellings/positions/effects, in addition to differential equality."""
import sys
from pathlib import Path
import tempfile
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import pre_tokens_diff as h
from gen_pre_token_cases import generate, CASES, OBSERVER_REGRESSIONS

@unittest.skipUnless(h.MODEL.exists() and h.REF.exists(), 'build tcc_pre_tokens and pre_tokens_ref first')
class RawIntegration(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp=tempfile.TemporaryDirectory(prefix='raw_golden_')
        cls.cases=Path(cls.tmp.name)/'cases';generate(cls.cases)
    @classmethod
    def tearDownClass(cls):cls.tmp.cleanup()
    def observe(self,name,plain=False):
        p=self.cases/(name+'.c')
        with tempfile.TemporaryDirectory() as t:
            d=Path(t);listing=h.materialize(p,d)
            cmd=[h.REF,'--root',d]+(['--plain'] if plain else [])
            rc,out,_=h.run(cmd);ref=h.parse(out,rc)
            rc,out,_=h.run([h.MODEL,'--prog',p,'--fs',listing]);model=h.parse(out,rc)
            if not plain:
                self.assertEqual(model.error,ref.error)
                self.assertEqual(model.tokens,ref.tokens)
                self.assertEqual(model.state,ref.state)
                rc,out,_=h.run([h.SPEC,'--cfg','run','--fslist',listing,p])
                spec,err=h.spec_obs(rc,out)
                if ref.error is None:
                    self.assertIsNone(err)
                    self.assertEqual(spec,ref.tokens)
                else:
                    self.assertIsNotNone(err)
            return model,ref
    def test_macro_self_include_fixture(self):
        (self.cases/'self.c').write_text('#ifndef SELF_SEEN\n#define SELF_SEEN 1\n'
            '#define SELF_NAME "self.c"\n#include SELF_NAME\n#else\nselected_self\n#endif\n')
        m,_=self.observe('self')
        self.assertEqual(m.tokens[-1:], [('I',b'selected_self')])
    def test_original_directory_include_fixture(self):
        (self.cases/'relative_header.h').write_text('27\n')
        (self.cases/'relative.c').write_text('#include "../cases/relative_header.h"\n')
        m,_=self.observe('relative')
        self.assertEqual(m.tokens[-1:], [('N',b'27')])
    def test_once_opened_path_policy(self):
        for operator,pragma in [(False,'#pragma once'),(True,'_Pragma("once")')]:
            with self.subTest(operator=operator):
                (self.cases/'once_path.h').write_text(pragma+'\n__COUNTER__\n')
                (self.cases/'once_path.c').write_text('#include "once_path.h"\n'
                    '#include "./once_path.h"\n#include "once_path.h"\n'
                    '#include "../cases/once_path.h"\n')
                m,_=self.observe('once_path')
                self.assertEqual(m.tokens[-3:], [('N',b'0'),('N',b'1'),('N',b'2')])
                with tempfile.TemporaryDirectory() as t:
                    d=Path(t);listing=h.materialize(self.cases/'once_path.c',d)
                    rc,out,_=h.run([h.SPEC,'--cfg','E','--fslist',listing,self.cases/'once_path.c'])
                    tokens,err=h.spec_obs(rc,out)
                    self.assertIsNone(err)
                    self.assertEqual(tokens[-2:], [('N',b'0'),('N',b'1')])
    def test_prefixes_and_no_concat(self):
        m,_=self.observe('prefixes')
        expected=[('S',b'L"wide"'),('S',b'u8"utf8"'),('S',b'u"utf16"'),('S',b'U"utf32"'),
                  ('C',b"L'a'"),('C',b"u'b'"),('C',b"U'c'"),('S',b'"plain"')]
        self.assertEqual(m.tokens[-8:],expected)
        m,_=self.observe('adjacent-strings')
        self.assertEqual(m.tokens[-6:],[('S',s) for s in [b'"a"',b'"b"',b'u8"c"',b'"d"',b'L"e"',b'"f"']])
    def test_literals_are_not_decoded(self):
        m,_=self.observe('literal-escapes')
        self.assertEqual(m.tokens[-5:],[('S',s) for s in [br'"\n"',br'"\x41"',br'"\101"',br'"\?"',br'"\\n"']])
        m,_=self.observe('splice-trigraph-literal')
        self.assertEqual(m.tokens[-3:],[('S',b'"??="'),('S',b'"??/"'),('C',b"'??='")])
        m,_=self.observe('stringify-trigraph')
        self.assertEqual(m.tokens[-2:],[('S',b'"??="'),('S',b'"??/"')])
    def test_digraph_and_ucn_spellings(self):
        m,_=self.observe('digraphs')
        self.assertTrue({'<:',':>','<%','%>'}.issubset({s.decode() for k,s in m.tokens if k=='P'}))
        m,_=self.observe('ucn-spellings')
        self.assertEqual([s for k,s in m.tokens if s.startswith(b'\\')],[br'\u00E9',br'\U000000E9'])
    def test_pragma_positions_and_effects(self):
        expected=[('I',b'before'),('[',b''),('I',b'pack'),('P',b'('),('I',b'push'),
                  ('P',b','),('N',b'1'),('P',b')'),(']',b''),('I',b'after'),('I',b'end')]
        for name in ['pragma-in-macro','pragma-in-argument','pragma-operator']:
            m,_=self.observe(name)
            # pragma-operator has a second pop; the macro tests pin the exact
            # event location and the still-active final pack effect.
            if name!='pragma-operator':
                self.assertEqual(m.tokens[-11:],expected);self.assertEqual(m.state[:2],(1,1))
        m,_=self.observe('pragma-pop-rescan')
        self.assertEqual(m.tokens[-2:],[('N',b'11'),('N',b'11')])
        self.assertFalse(any(k in '[]' for k,b in m.tokens))
        for name,token in [('pragma-stringify',('S',b'"word"')),('pragma-paste',('I',b'word'))]:
            m,_=self.observe(name)
            self.assertEqual(m.tokens[-2],token)
            self.assertEqual(sum(k=='[' for k,b in m.tokens),1)
        m,_=self.observe('pragma-two-in-macro')
        self.assertEqual(sum(k=='[' for k,b in m.tokens),2)
        m,_=self.observe('pragma-argument-twice')
        self.assertEqual(sum(k=='[' for k,b in m.tokens),1)
        self.assertEqual(m.state[:2],(1,1))
        m,_=self.observe('pack-nested');self.assertEqual(m.state[:2],(1,2))
        m,plain=self.observe('pragma-ignored-counter',plain=True)
        self.assertEqual(m.tokens[-1],('N',b'0'));self.assertEqual(m.state,plain.state)
    def test_pragma_compositions(self):
        for name in CASES:
            if (name.startswith('pragma-') or name.startswith('ordinary-')):
                with self.subTest(name=name):self.observe(name)
        for name in ['pragma-paste-two','pragma-stringify-two']:
            m,_=self.observe(name)
            payload=[b for k,b in m.tokens if b in (b'left',b'right')]
            self.assertEqual(payload,[b'right',b'left'])
        m,_=self.observe('pragma-repeated-nonempty')
        self.assertEqual(sum(k=='[' for k,b in m.tokens),1)
        self.assertEqual(m.state[:2],(1,1))
        for name in ['pragma-once-operator','pragma-once-prescan']:
            m,_=self.observe(name)
            self.assertEqual(m.tokens[-2:],[('N',b'0'),('N',b'1')])
        m,_=self.observe('pragma-pop-argument')
        self.assertEqual(m.tokens[-2:],[('N',b'11'),('N',b'11')])

    def test_include_and_line_state(self):
        for name in ['include','line-file','line-splice','line-macro','counter']:
            self.observe(name)
        m,_=self.observe('include')
        self.assertIn(('S',b'"/inside.h"'),m.tokens)
        self.assertEqual(m.state[2],3)
    def test_negative_inputs_are_refusals(self):
        for name in CASES:
            if name.startswith('error-'):
                with self.subTest(name=name):
                    m,r=self.observe(name);self.assertEqual(m.error[0],'refuse')
    def test_observer_regressions_preserve_plain_execution(self):
        for name in sorted(OBSERVER_REGRESSIONS):
            with self.subTest(name=name):
                row=h.one(self.cases/(name+'.c'))
                self.assertEqual(row['raw'],'exact')
                self.assertEqual(row['spec'],'exact')
                self.assertTrue(row['observer_preserves_effects'])

    def test_comparator_default_accepts_repaired_pragma_and_rejects_refusal(self):
        for name,expected in [('prefixes',0),('error-comment',1),('pragma-unknown',0)]:
            with self.subTest(name=name),tempfile.TemporaryDirectory() as t:
                rc,out,err=h.run([sys.executable,h.ROOT/'scripts/pre_tokens_diff.py',
                                 '--out',Path(t)/'report.json',self.cases/(name+'.c')])
                self.assertEqual(rc,expected,(out,err))
    def test_comparator_default_rejects_spec_residual(self):
        with tempfile.TemporaryDirectory() as t:
            p=Path(t)/'overflow.c'
            p.write_text('#if 9223372036854775807 + 1\nyes\n#endif\n')
            rc,out,err=h.run([sys.executable,h.ROOT/'scripts/pre_tokens_diff.py',
                             '--out',Path(t)/'report.json',p])
            self.assertEqual(rc,1,(out,err))

    def test_driver_failure_does_not_produce_success(self):
        for args in [[],['--unknown'],['--prog','/does/not/exist'],['--prog',str(self.cases/'empty.c'),'--fs','/does/not/exist']]:
            rc,out,_=h.run([h.MODEL,*args]);self.assertNotEqual(rc,0)
            with self.assertRaises(h.ProtocolError):h.parse(out,rc)

if __name__=='__main__':unittest.main()
