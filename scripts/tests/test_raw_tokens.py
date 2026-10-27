import sys
from pathlib import Path
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from raw_tokens import parse, ProtocolError, spec_bytes, e_encoding

class RawProtocol(unittest.TestCase):
    def good(self, middle=b'',n=0):
        return b'TCCPP 1\n'+middle+b'STATE 0 0 0 0\nEND '+str(n).encode()+b'\n'
    def test_explicit_empty(self):
        self.assertEqual(parse(self.good(),0).tokens,[])
    def test_arbitrary_bytes(self):
        self.assertEqual(parse(self.good(b'S 4 00ff095c\n',1),0).tokens,[('S',b'\0\xff\t\\')])
    def test_error(self):
        for kind,rc in [('refuse',1),('model',2)]:
            self.assertEqual(parse(('TCCPP 1\nERROR '+kind+' 6162\n').encode(),rc).error,(kind,b'ab'))
    def test_failures(self):
        bad=[b'',b'TCCPP 1\n',b'TCCPP 2\n',self.good()+b'\n',
             self.good(b'X 1 61\n',1),self.good(b'I 2 61\n',1),self.good(b'I 1 6g\n',1),
             self.good(b'I 1 6A\n',1),self.good(b'I 01 61\n',1),self.good(b'I 0 \n',1),
             self.good(b'I 1 61\n',2),self.good(b'[ 0 \n',1),self.good(b'] 0 \n',1),
             self.good(b'[ 1 61\n] 0 \n',2),self.good(b'ERROR refuse 61\n'),
             b'TCCPP 1\nERROR refuse 61\nERROR refuse 61\n',b'TCCPP 1\nERROR refuse 6\n',
             self.good().replace(b'STATE 0',b'STATE 8'),self.good().replace(b'STATE 0 0',b'STATE 0 3')]
        for out in bad:
            with self.subTest(out=out),self.assertRaises(ProtocolError): parse(out,0)
    def test_crashes_and_exit_disagreement(self):
        for rc in [-11,-6,1,2,124,139]:
            with self.subTest(rc=rc),self.assertRaises(ProtocolError): parse(self.good(),rc)
        for out,rc in [(b'TCCPP 1\nERROR refuse 61\n',0),(b'TCCPP 1\nERROR model 61\n',1)]:
            with self.assertRaises(ProtocolError):parse(out,rc)
    def test_never_decode_C_escapes(self):
        self.assertEqual(spec_bytes(r'"\\n\\x41\\?"'),b'"\\n\\x41\\?"')
        self.assertEqual(e_encoding([('S',b'"?"')],[('S',b'"\\?"')]),'printer-question-escape')
        for raw,printed in [(b'"A"',b'"\\x41"'),(b'"\n"',b'"\\n"'),(b'"?"',b'"\\077"')]:
            self.assertEqual(e_encoding([('S',raw)],[('S',printed)]),'different')
    def test_kind_and_pragma_position_matter(self):
        self.assertEqual(e_encoding([('I',b'x')],[('N',b'x')]),'different')
        self.assertEqual(e_encoding([('[',b''),('I',b'x'),(']',b'')],[('I',b'x'),('[',b''),(']',b'')]),'different')

if __name__=='__main__':unittest.main()
