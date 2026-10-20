#!/usr/bin/env python3
"""Recover HTIF bytes from the trace-mode emulator's UTF-8 Lean String.

Trace-mode diagnostics go to stderr. Every stdout character belongs to the
program, including strings such as SUCCESS and TODO: cancel_reservation.
"""
import argparse
from pathlib import Path
import re
import sys


def decode_console(data):
    try:
        text = data.decode('utf-8')
        if all(ord(c) < 256 for c in text):
            return text.encode('latin-1')
    except UnicodeDecodeError:
        pass
    return data


def split_code_dump(data):
    """Consume only the size-framed initial dump; later @@ text is program output."""
    header, separator, rest = data.partition(b'\n')
    match = re.fullmatch(rb'@@TCCBM code (0x[0-9a-fA-F]+) ([0-9a-fA-F]+)', header)
    if not match or not separator:
        raise ValueError('missing initial TCC code dump')
    size = int(match[2], 16)
    code = bytearray()
    while len(code) < size:
        line, separator, rest = rest.partition(b'\n')
        if not separator or not re.fullmatch(rb'@@(?:[0-9a-fA-F]{2})+', line):
            raise ValueError('malformed or truncated TCC code dump')
        code.extend(bytes.fromhex(line[2:].decode('ascii')))
    if len(code) != size:
        raise ValueError('TCC code dump exceeds declared size')
    return bytes(code), rest


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('raw')
    parser.add_argument('--code-out')
    args = parser.parse_args()
    data = Path(args.raw).read_bytes()
    if args.code_out:
        try:
            code, data = split_code_dump(data)
        except ValueError as error:
            parser.error(str(error))
        Path(args.code_out).write_bytes(code)
    sys.stdout.buffer.write(decode_console(data))
