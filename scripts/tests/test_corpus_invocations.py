"""Protect shared input bytes/order and rejection of stale or conflicting manifests."""
from pathlib import Path
import hashlib,json,sys,tempfile,unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from source_fixtures import load_invocation

class InvocationInputs(unittest.TestCase):
    def setup_manifest(self, root):
        for n,c in [('main.c','int main(void){return f();}'),('companion.c','int f(void){return 0;}'),('Makefile','test: companion.c main.c')]:
            (root/n).write_text(c)
        h=lambda n:hashlib.sha256((root/n).read_bytes()).hexdigest()
        return dict(version=1,source='main.c',source_sha256=h('main.c'),before_run=[dict(image_path='/other.c',source='companion.c',sha256=h('companion.c'))],provenance=dict(path='Makefile',sha256=h('Makefile')))
    def read(self,root,d):
        p=root/'invocation.json';p.write_text(json.dumps(d));return load_invocation(p,root)
    def test_distinct_translation_units_and_order(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp).resolve();d=self.setup_manifest(root);r=self.read(root,d)
            self.assertEqual(r['argv'],['tcc','-std=c11','-w','/other.c','-run','/prog.c'])
            self.assertEqual(r['source'],str(root/'main.c'))
            self.assertEqual(dict(r['extra'])['/other.c'],str(root/'companion.c'))
            self.assertIn(str(root/'companion.c'),r['pins'])
    def test_changed_companion_cannot_reuse_pin(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp).resolve();d=self.setup_manifest(root);(root/'companion.c').write_text('int f(void){return 1;}')
            with self.assertRaisesRegex(ValueError,'hash mismatch'):self.read(root,d)
    def test_conflicting_image_paths_and_reserved_main(self):
        for image_path in ['/main.c','/prog.c','/bad".c','/../other.c']:
            with self.subTest(image_path=image_path),tempfile.TemporaryDirectory() as tmp:
                root=Path(tmp).resolve();d=self.setup_manifest(root);d['before_run'][0]['image_path']=image_path
                with self.assertRaises(ValueError):self.read(root,d)
    def test_duplicate_companion_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp).resolve();d=self.setup_manifest(root);d['before_run']*=2
            with self.assertRaisesRegex(ValueError,'duplicate'):self.read(root,d)
if __name__=='__main__':unittest.main()
