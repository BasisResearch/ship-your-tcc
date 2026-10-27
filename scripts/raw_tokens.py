"""Closed byte-preserving TCCPP 1 protocol. This decodes serialization hex,
never C literal escapes. Missing/truncated/error output cannot be equality."""
from dataclasses import dataclass
from collections import deque
import re

class ProtocolError(RuntimeError):
    pass

@dataclass
class Observation:
    tokens: list
    state: tuple | None = None
    error: tuple | None = None


def parse(data, rc):
    if not isinstance(data, bytes) or not data.endswith(b'\n'):
        raise ProtocolError('missing final newline')
    lines = deque(data.split(b'\n')[:-1])
    if not lines or lines.popleft() != b'TCCPP 1':
        raise ProtocolError('missing/unknown header')
    if lines and lines[0].startswith(b'ERROR '):
        m = re.fullmatch(rb'ERROR (refuse|model) ([0-9a-f]+)', lines[0])
        if len(lines) != 1 or not m or len(m[2]) % 2:
            raise ProtocolError('malformed/mixed error')
        expected = 1 if m[1] == b'refuse' else 2
        if rc != expected: raise ProtocolError('error/exit disagreement')
        return Observation([], error=(m[1].decode(), bytes.fromhex(m[2].decode())))
    if rc != 0: raise ProtocolError('success/exit disagreement: '+str(rc))
    tokens, depth = [], 0
    while lines and not lines[0].startswith(b'STATE '):
        line = lines.popleft()
        m = re.fullmatch(rb'([INCSPO\[\]]) (0|[1-9][0-9]*) ([0-9a-f]*)', line)
        if not m: raise ProtocolError('unknown/malformed token record')
        kind, n, h = m.groups()
        if len(h) % 2 or len(h) != 2 * int(n): raise ProtocolError('spelling length disagreement')
        spell = bytes.fromhex(h.decode())
        if kind in (b'[',b']'):
            if spell: raise ProtocolError('nonempty pragma marker')
            depth += 1 if kind == b'[' else -1
            if depth < 0: raise ProtocolError('unbalanced pragma')
        elif not spell: raise ProtocolError('empty ordinary token')
        tokens.append((kind.decode(),spell))
    if depth: raise ProtocolError('unterminated pragma')
    if len(lines) != 2: raise ProtocolError('missing/extra completion record')
    m = re.fullmatch(rb'STATE (0|[1-9][0-9]*) (-?(?:0|[1-9][0-9]*)) (-?(?:0|[1-9][0-9]*)) (0|[1-9][0-9]*)', lines[0])
    end = re.fullmatch(rb'END (0|[1-9][0-9]*)', lines[1])
    if not m or not end or int(end[1]) != len(tokens): raise ProtocolError('invalid state/count')
    state = tuple(map(int,m.groups()))
    if not 0 <= state[0] < 8 or state[1] not in (0,1,2,4,8,16) or not -2**31 <= state[2] < 2**31:
        raise ProtocolError('state outside tcc limits')
    return Observation(tokens,state)


def spec_bytes(text):
    """Inverse of pre_spec's escBytes serialization, not parse_escape_string."""
    out = bytearray(); i = 0
    while i < len(text):
        c = text[i]
        if c == '\\':
            if text[i:i+2] == '\\\\': out.append(92); i += 2
            elif re.fullmatch(r'\\x[0-9a-f]{2}',text[i:i+4]):
                out.append(int(text[i+2:i+4],16)); i += 4
            else: raise ProtocolError('invalid spec byte serialization')
        elif 32 <= ord(c) < 127: out.append(ord(c)); i += 1
        else: raise ProtocolError('unescaped spec byte')
    return bytes(out)


def e_encoding(raw, printed):
    """Auxiliary classification only. Remove ONLY printer-added \\? before a
question mark, and only to test that hypothesis. Never an equality rule for
raw comparison, and never a general C literal decoder."""
    if raw == printed: return 'exact'
    if len(raw) == len(printed):
        changes = 0
        for (k,a),(j,b) in zip(raw,printed):
            if (k,a) == (j,b): continue
            if k != j or k not in ('S','C') or b.replace(b'\\?',b'?') != a:
                break
            changes += 1
        else:
            if changes: return 'printer-question-escape'
    return 'different'
