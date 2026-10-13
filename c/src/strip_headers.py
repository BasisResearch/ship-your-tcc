#!/usr/bin/env python3
"""strip_headers.py SRC DST: copy the *.h tree under SRC (without c++/) to
DST with comments, leading indentation and trailing blanks removed; every
line stays on its line (a comment spanning lines leaves its newlines), so
__LINE__ and tcc's diagnostics are unchanged. Nothing else changes: string
and character literals are copied as they are, and a line following a
backslash-newline keeps its indentation.

Why: tcc -run on the Sail model spends ~35 steps per header byte, and
newlib's headers are about a third comments. tests/check_headers.sh checks
that gcc preprocesses programs to the same tokens and macros with the
stripped and the original trees."""
import os
import sys


def strip(src):
    out = []
    i, n = 0, len(src)
    while i < n:
        c = src[i]
        if c == '/' and i + 1 < n and src[i + 1] == '*':
            j = src.find('*/', i + 2)
            j = n if j < 0 else j + 2
            nl = src.count('\n', i, j)
            out.append('\n' * nl if nl else ' ')
            i = j
        elif c == '/' and i + 1 < n and src[i + 1] == '/':
            # to the end of the line; a backslash-newline continues it
            j = i
            while j < n and src[j] != '\n':
                if src[j] == '\\' and j + 1 < n and src[j + 1] == '\n':
                    j += 2
                    out.append('\n')
                else:
                    j += 1
            i = j
        elif c in '"\'':
            j = i + 1
            while j < n and src[j] != c and src[j] != '\n':
                j += 2 if src[j] == '\\' else 1
            j = min(j + 1, n)
            out.append(src[i:j])
            i = j
        else:
            out.append(c)
            i += 1
    lines = ''.join(out).split('\n')
    res, cont = [], False
    for line in lines:
        s = line.rstrip(' \t')
        if not cont:
            s = s.lstrip(' \t')
        cont = s.endswith('\\')
        res.append(s)
    return '\n'.join(res)


def main():
    src, dst = sys.argv[1], sys.argv[2]
    for root, dirs, files in os.walk(src):
        rel = os.path.relpath(root, src)
        if rel == 'c++' or rel.startswith('c++' + os.sep):
            continue
        for f in files:
            if not f.endswith('.h'):
                continue
            d = os.path.join(dst, rel)
            os.makedirs(d, exist_ok=True)
            with open(os.path.join(root, f), encoding='latin-1') as fi:
                text = fi.read()
            with open(os.path.join(d, f), 'w', encoding='latin-1') as fo:
                fo.write(strip(text))


if __name__ == '__main__':
    main()
