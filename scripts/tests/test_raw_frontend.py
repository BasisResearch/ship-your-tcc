"""Semantic golden checks. Missing new executable is an error, never a skip."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from gen_raw_frontend_cases import generate, CASES, INVALID

ROOT = Path(__file__).resolve().parents[2]
BIN = ROOT/'.lake/build/bin/csem_raw'


class RawFrontend(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not BIN.exists():
            raise RuntimeError('Build csem_raw under the shared lock and cap first')
        cls.tmp = tempfile.TemporaryDirectory(prefix='raw-frontend-')
        cls.root = Path(cls.tmp.name)
        cls.listing = generate(cls.root)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def invoke(self, name, *extra):
        return subprocess.run([BIN, '--fslist', self.listing, *extra, self.root/(name+'.c')],
                              capture_output=True, text=True, timeout=120)

    def observe(self, name, *extra):
        p = self.invoke(name, *extra)
        self.assertEqual(p.returncode, 0, (name, p.stderr))
        return json.loads(p.stdout)

    def test_positive_matrix_and_digest(self):
        for name in CASES:
            with self.subTest(name=name):
                r = self.observe(name, *(['--tcc'] if name in ['ucn-utf8','ucn-macros','utf8-typedef'] else []))
                self.assertEqual(r['digest'], hashlib.md5((self.root/(name+'.c')).read_bytes()).hexdigest())
                self.assertTrue(all(t['site'] is not None for t in r['phase4']))

    @staticmethod
    def identifiers(value):
        if isinstance(value, dict):
            if 'name' in value and 'loc' in value:
                yield value.get('name')
            for child in value.values():
                yield from RawFrontend.identifiers(child)
        elif isinstance(value, list):
            for child in value:
                yield from RawFrontend.identifiers(child)

    def test_ucn_utf8_identity_and_typedef_feedback(self):
        for name in ['ucn', 'ucn-utf8', 'ucn-macros', 'utf8-typedef']:
            r = self.observe(name, '--tcc')
            encoded = json.dumps(r['cabs'])
            self.assertNotIn('U000000E9', encoded)
            self.assertNotIn('U000003B1', encoded)
            self.assertNotIn('u03B1', encoded)
            # All non-ASCII NAMEs use canonical lower-case UCN spellings.
            names = list(self.identifiers(r['cabs']))
            self.assertTrue(names, r['cabs'])
            self.assertTrue(any('\\u' in n for n in names))
        r = self.observe('ucn-utf8', '--tcc')
        spellings = [t['spelling'] for t in r['phase4']]
        self.assertIn('é'.encode().decode('latin1'), spellings)
        self.assertIn('\\u00E9', spellings)
        self.assertIn('\\U000000E9', spellings)
        self.assertEqual(list(self.identifiers(r['cabs'])).count('\\u00e9'), 3)
        widths = {t['site']['physical']['stop']['offset'] - t['site']['physical']['start']['offset']
                  for t in r['phase4'] if t['spelling'] in ['\\u00E9','\\U000000E9','é'.encode().decode('latin1')]}
        self.assertEqual(widths, {2,6,10})

    def test_checked_semantic_entry_diagnoses_pending_pack_and_library(self):
        for name in ['pack', 'pack-active']:
            p = self.invoke(name, '--tcc', '--checked')
            self.assertEqual(p.returncode, 1)
            self.assertIn('unimplemented-pack-layout', p.stderr)
        p = self.invoke('line-library-plant', '--checked')
        self.assertEqual(p.returncode, 1)
        self.assertIn('library-identity-mismatch', p.stderr)
        p = self.invoke('basic', '--checked')
        self.assertEqual(p.returncode, 0, p.stderr)

    def test_invalid_phase7_is_diagnosed(self):
        for name in INVALID:
            with self.subTest(name=name):
                p = self.invoke(name, *(['--tcc'] if name == 'pack-invalid' else []))
                self.assertEqual(p.returncode, 1, (name, p.stdout, p.stderr))
                self.assertTrue(p.stderr)

    def test_include_identity_and_ancestry(self):
        r = self.observe('include')
        included = next(t for t in r['phase4'] if t['spelling'] == 'included')['site']
        nested = next(t for t in r['phase4'] if t['spelling'] == 'nested')['site']
        self.assertEqual(included['physical']['identity'], ['include', 'a.h'])
        self.assertEqual(nested['physical']['identity'], ['include', 'b.h'])
        self.assertEqual(len(included['includes']), 1)
        self.assertEqual(len(nested['includes']), 2)

    def test_macro_invocation_arguments_and_paste(self):
        r = self.observe('macro-locations')
        val = next(t for t in r['phase4'] if t['spelling'] == 'val')
        arg = next(t for t in r['phase4'] if t['spelling'] == '2')
        string = next(t for t in r['phase4'] if t['spelling'] == '"foo + bar"')
        self.assertIn('paste', val['derivation'])
        self.assertEqual(val['site']['line'], 90)
        self.assertIn('argument', arg['derivation'])
        self.assertEqual(arg['site']['line'], 91)
        self.assertIn('stringify', string['derivation'])
        self.assertEqual(string['site']['line'], 92)

    def test_trigraph_span_and_splice(self):
        r = self.observe('trigraph-splice')
        t = next(t for t in r['phase4'] if t['spelling'] == 'value')
        self.assertEqual(t['site']['physical']['start']['line'], 2)
        self.assertEqual(t['site']['physical']['stop']['line'], 3)
        self.assertTrue(any(t['spelling'] == '"\\n"' for t in r['phase4']))

    def test_no_second_physical_translation(self):
        r = self.observe('no-rephase')
        spellings = [t['spelling'] for t in r['phase4']]
        self.assertIn('"??="', spellings)
        self.assertIn('"??/"', spellings)

    def test_line_labels_leave_identity_unchanged(self):
        r = self.observe('line')
        t = next(t for t in r['phase4'] if t['spelling'] == '400')
        self.assertEqual(t['site']['presumed'], 'presumed.c')
        self.assertEqual(t['site']['line'], 400)
        self.assertEqual(t['site']['physical']['start']['line'], 2)
        self.assertEqual(t['site']['physical']['openedAs'], str(self.root/'line.c'))

    def test_library_identity_is_independent_of_line_names(self):
        member = '/runtime/libc/include/h.h'
        r = self.observe('library', '--library', member)
        self.assertTrue(any(o['actualLibrary'] and o['presumedLibrary'] for o in r['libraryOrigins']))
        r = self.observe('line-library-plant', '--library', member)
        self.assertTrue(any(not o['actualLibrary'] and o['presumedLibrary'] for o in r['libraryOrigins']))
        alias = '/runtime/libc/include/alias.h'
        r = self.observe('library-line-alias', '--library', alias)
        self.assertTrue(any(o['actualLibrary'] and not o['presumedLibrary'] for o in r['libraryOrigins']))

    def test_pragma_schedule_is_retained(self):
        r = self.observe('pack', '--tcc')
        self.assertEqual(r['finalPack'], [0])
        active = self.observe('pack-active', '--tcc')
        self.assertEqual(active['finalPack'], [1, 0])
        self.assertEqual(len(r['pragmas']), 2)
        self.assertEqual([t['spelling'] for t in r['pragmas'][0]['payload']], ['pack', '(', 'push', ',', '1', ')'])
        self.assertGreater(r['pragmas'][1]['beforeToken'], r['pragmas'][0]['beforeToken'])
        r = self.observe('pragma-adjacent')
        self.assertEqual(len(r['pragmas']), 1)
        self.assertEqual([t['spelling'] for t in r['phase4'] if 'strLit' in t['kind']], ['"a"', '"b"'])


if __name__ == '__main__':
    unittest.main()
