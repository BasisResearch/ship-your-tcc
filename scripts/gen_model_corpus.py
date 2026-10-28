#!/usr/bin/env python3
"""Generated programs for the F3 differential (TccModel vs tcc, byte for
byte). The programs are compiled only (the arena is compared), so they need
not be free of undefined behaviour; they should reach as much of tcc as
possible.

    scripts/gen_model_corpus.py csmith N [--seed S]   build/model_corpus/csmith/
    scripts/gen_model_corpus.py cgen N [--seed S]     build/model_corpus/cgen/
    scripts/gen_model_corpus.py mutants N [--seed S]  build/model_corpus/mutants/

csmith: Csmith (build/csmith/install, scripts/CSMITH_REV) with its default
feature set (bit-fields, unions, structs, pointers, ...), seeds S.., half of
them flattened without headers and half with <stdio.h>/<stdint.h>
(scripts/csmith_flatten.py).

cgen: a small random C generator aimed at tcc's own features that Csmith
does not produce: float/double/long double arithmetic and conversions
(constant-folded and not), _Bool, enums, bit-fields of every width and
packing, unions, designated and nested initializers, string literals,
compound literals, statement expressions, typeof, _Generic, case ranges,
computed sizes, VLAs, varargs (stdarg.h), function pointers, goto/labels,
switch, do/while/for, the comma and ?: operators, __builtin_* calls,
attributes (aligned, packed), static locals, struct return and passing.

mutants: corpus programs (c/tests/tcc, corpus/*) with one random token-level
edit (delete, duplicate, swap or replace a token): mostly invalid programs,
so tcc's diagnostics (console bytes and exit status) are compared.

The output is a function of (generator, N, seed) only.
"""
import os
import random
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, 'build', 'model_corpus')


# ---------------------------------------------------------------- csmith
def gen_csmith(n, seed):
    csmith = os.path.join(ROOT, 'build', 'csmith', 'install', 'bin', 'csmith')
    inc = os.path.join(ROOT, 'build', 'csmith', 'install', 'include')
    d = os.path.join(OUT, 'csmith')
    os.makedirs(d, exist_ok=True)
    made = 0
    s = seed
    while made < n:
        raw = os.path.join(d, 'raw_%d.c' % s)
        out = os.path.join(d, 'csmith_%d.c' % s)
        r = subprocess.run([csmith, '--seed', str(s), '--no-argc', '--max-funcs', '8', '-o', raw],
                           capture_output=True, timeout=60)
        s += 1
        if r.returncode != 0:
            continue
        args = [sys.executable, '-I', os.path.join(ROOT, 'scripts', 'csmith_flatten.py'), inc, raw, out,
                '--header', 'Csmith seed %d (default features), scripts/gen_model_corpus.py' % (s - 1)]
        if (s - 1) % 2:
            args.append('--includes')
        r = subprocess.run(args, capture_output=True)
        os.unlink(raw)
        if r.returncode == 0:
            made += 1


# ---------------------------------------------------------------- cgen
class G:
    def __init__(self, rnd):
        self.r = rnd
        self.structs = []    # (name, [(type, field, bits)])
        self.globals = []    # (type, name)
        self.funcs = []      # (ret, name, params)
        self.n = 0

    def fresh(self, p):
        self.n += 1
        return '%s%d' % (p, self.n)

    SCALARS = ['char', 'signed char', 'unsigned char', 'short', 'unsigned short', 'int', 'unsigned',
               'long', 'unsigned long', 'long long', 'unsigned long long', '_Bool',
               'float', 'double', 'long double']
    INTS = ['char', 'signed char', 'unsigned char', 'short', 'unsigned short', 'int', 'unsigned',
            'long', 'unsigned long', 'long long', 'unsigned long long']

    def scalar(self):
        return self.r.choice(self.SCALARS)

    def lit(self, t):
        r = self.r
        if t in ('float', 'double', 'long double'):
            v = r.choice(['0.0', '1.0', '-2.5', '3.25', '1e10', '1e-5', '0.1', '123.456', '-0.0', '6.02e23',
                          '0x1.8p3', '1.0/3.0', '2.718281828459045'])
            suf = {'float': 'f', 'double': '', 'long double': 'L'}[t]
            if '/' in v:
                a, b = v.split('/')
                return '(%s%s/%s%s)' % (a, suf, b, suf)
            return v + suf
        if t == '_Bool':
            return r.choice(['0', '1'])
        v = r.choice([0, 1, -1, 2, 7, 100, 255, 256, 32767, -32768, 65535, 2147483647, -2147483647,
                      4294967295, 1 << 40, -(1 << 50), 0x7fffffffffffffff, 12345])
        s = str(v)
        if v > 2147483647 or v < -2147483648:
            s += 'LL'
        if t.startswith('unsigned') and v >= 0:
            s += 'U' if 'L' not in s else ''
        return s if v >= 0 else '(%s)' % s

    def struct_def(self):
        name = self.fresh('S')
        fields = []
        for i in range(self.r.randint(1, 6)):
            t = self.r.choice(self.INTS + ['float', 'double', 'char', 'int'])
            bits = None
            if t in self.INTS and self.r.random() < 0.4:
                size = {'char': 8, 'signed char': 8, 'unsigned char': 8, 'short': 16, 'unsigned short': 16,
                        'int': 32, 'unsigned': 32, 'long': 64, 'unsigned long': 64,
                        'long long': 64, 'unsigned long long': 64}[t]
                bits = self.r.randint(0 if i else 1, size - (1 if size == 64 else 0))
                if bits == 0:
                    fields.append((t, '', 0))
                    continue
            fields.append((t, 'f%d' % i, bits))
        kind = 'union' if self.r.random() < 0.2 else 'struct'
        attr = ''
        if self.r.random() < 0.15:
            attr = ' __attribute__((packed))'
        elif self.r.random() < 0.1:
            attr = ' __attribute__((aligned(16)))'
        self.structs.append((name, kind, fields))
        body = ''.join('    %s %s%s;\n' % (t, f, (' : %d' % b) if b is not None else '') for t, f, b in fields)
        return '%s%s %s {\n%s};\n' % (kind, attr, name, body)

    def expr(self, vars, t, depth=0):
        r = self.r
        if depth > 3 or r.random() < 0.3:
            cands = [v for v, vt in vars if vt == t]
            if cands and r.random() < 0.7:
                return r.choice(cands)
            return self.lit(t)
        k = r.random()
        isf = t in ('float', 'double', 'long double')
        if k < 0.35:
            op = r.choice(['+', '-', '*'] + ([] if isf else ['/', '%', '&', '|', '^', '<<', '>>']) + (['/'] if isf else []))
            a = self.expr(vars, t, depth + 1)
            b = self.expr(vars, t, depth + 1)
            if op in ('/', '%') and not isf:
                b = '((%s) | 1)' % b
            if op in ('<<', '>>'):
                b = '(%s & 7)' % b
            return '(%s %s %s)' % (a, op, b)
        if k < 0.45:
            return '(%s)%s' % (t, self.expr(vars, self.scalar(), depth + 1))
        if k < 0.55:
            return '(%s ? %s : %s)' % (self.expr(vars, 'int', depth + 1), self.expr(vars, t, depth + 1),
                                       self.expr(vars, t, depth + 1))
        if k < 0.62:
            o = self.scalar()
            return '(%s)(%s %s %s)' % (t, self.expr(vars, o, depth + 1), r.choice(['<', '<=', '==', '!=', '>', '>=', '&&', '||']),
                                       self.expr(vars, o, depth + 1))
        if k < 0.68:
            return '(%s)(-(%s))' % (t, self.expr(vars, t, depth + 1))
        if k < 0.72 and not isf:
            return '(%s)(~%s)' % (t, self.expr(vars, t, depth + 1))
        if k < 0.76:
            return '(%s)sizeof(%s)' % (t, r.choice(self.SCALARS + ['struct ' + s[0] if s[1] == 'struct' else 'union ' + s[0] for s in self.structs] or ['int']))
        if k < 0.80:
            return '({ %s tmp = %s; tmp; })' % (t, self.expr(vars, t, depth + 1))
        if k < 0.84 and self.funcs:
            f = r.choice(self.funcs)
            args = ', '.join(self.expr(vars, pt, depth + 1) for pt in f[2])
            return '(%s)%s(%s)' % (t, f[1], args)
        if k < 0.88:
            return '(%s)(%s, %s)' % (t, self.expr(vars, 'int', depth + 1), self.expr(vars, t, depth + 1))
        if k < 0.92:
            return '_Generic(%s, int: (%s)1, double: (%s)2, default: (%s)3)' % (self.expr(vars, self.scalar(), depth + 1), t, t, t)
        return '((%s[]){%s, %s})[%d]' % (t, self.expr(vars, t, depth + 1), self.expr(vars, t, depth + 1), r.randint(0, 1))

    def printv(self, v, t):
        if t in ('float', 'double'):
            return '    printf("%%g\\n", (double)%s);\n' % v
        if t == 'long double':
            return '    printf("%%g\\n", (double)%s);\n' % v
        return '    printf("%%lld\\n", (long long)%s);\n' % v

    def stmt(self, vars, depth, ret):
        r = self.r
        k = r.random()
        if depth > 2 or k < 0.35:
            v, t = r.choice(vars)
            op = r.choice(['=', '+=', '-=', '*='] + ([] if t in ('float', 'double', 'long double', '_Bool') else ['|=', '&=', '^=', '<<=']))
            e = self.expr(vars, t)
            if op == '<<=':
                e = '(%s & 3)' % e
            return '    %s %s %s;\n' % (v, op, e)
        if k < 0.45:
            return '    if (%s) {\n%s    } else {\n%s    }\n' % (self.expr(vars, 'int'), self.stmt(vars, depth + 1, ret),
                                                             self.stmt(vars, depth + 1, ret))
        if k < 0.55:
            i = self.fresh('i')
            return '    for (int %s = 0; %s < %d; %s++) {\n%s    }\n' % (i, i, r.randint(1, 5), i,
                                                                    self.stmt(vars + [(i, 'int')], depth + 1, ret))
        if k < 0.62:
            i = self.fresh('w')
            return '    { int %s = %d; while (%s-- > 0) {\n%s    if (%s == 2) continue; if (%s == 7) break; } }\n' % (
                i, r.randint(1, 6), i, self.stmt(vars + [(i, 'int')], depth + 1, ret), i, i)
        if k < 0.68:
            i = self.fresh('d')
            return '    { int %s = 0; do {\n%s    } while (++%s < %d); }\n' % (i, self.stmt(vars + [(i, 'int')], depth + 1, ret), i, r.randint(1, 4))
        if k < 0.76:
            cases = ''
            used = set()
            for _ in range(r.randint(1, 5)):
                c = r.randint(-3, 20)
                if c in used:
                    continue
                used.add(c)
                if r.random() < 0.2 and not ({c + 1, c + 2} & used):
                    cases += '    case %d ... %d:\n%s        break;\n' % (c, c + 2, self.stmt(vars, depth + 1, ret))
                    used |= {c + 1, c + 2}
                else:
                    cases += '    case %d:\n%s%s' % (c, self.stmt(vars, depth + 1, ret), '        break;\n' if r.random() < 0.7 else '')
            return '    switch ((int)(%s)) {\n%s    default: break;\n    }\n' % (self.expr(vars, 'int'), cases)
        if k < 0.80:
            l = self.fresh('L')
            return '    goto %s;\n%s    %s: ;\n' % (l, self.stmt(vars, depth + 1, ret), l)
        if k < 0.84:
            n = self.fresh('vla')
            return '    { int %s_n = %d; int %s[%s_n]; for (int j = 0; j < %s_n; j++) %s[j] = j * 3; %s = (%s)%s[%s_n - 1]; }\n' % (
                n, r.randint(1, 8), n, n, n, n, vars[0][0], vars[0][1], n, n)
        if k < 0.88:
            v, t = r.choice(vars)
            return self.printv(v, t)
        if k < 0.92:
            n = self.fresh('st')
            return '    { static %s %s = %s; %s += 1; %s = (%s)%s; }\n' % ('int', n, self.lit('int'), n, vars[0][0], vars[0][1], n)
        v, t = r.choice(vars)
        return '    { typeof(%s) tt = %s; %s = tt; }\n' % (v, self.expr(vars, t), v)

    def func(self):
        r = self.r
        ret = self.scalar()
        name = self.fresh('fn')
        params = [self.scalar() for _ in range(r.randint(0, 5))]
        vars = [('p%d' % i, t) for i, t in enumerate(params)]
        body = ''
        for i in range(r.randint(1, 4)):
            t = self.scalar()
            v = 'l%d' % i
            body += '    %s %s = %s;\n' % (t, v, self.expr(vars, t) if vars else self.lit(t))
            vars.append((v, t))
        for _ in range(r.randint(2, 8)):
            body += self.stmt(vars, 0, ret)
        body += '    return (%s)%s;\n' % (ret, self.expr(vars, ret))
        ps = ', '.join('%s p%d' % (t, i) for i, t in enumerate(params)) or 'void'
        self.funcs.append((ret, name, params))
        st = 'static ' if r.random() < 0.3 else ''
        return '%s%s %s(%s)\n{\n%s}\n' % (st, ret, name, ps, body)

    def varargs_func(self):
        name = self.fresh('va')
        self.va = name
        return ('static long double %s(int n, ...)\n{\n    va_list ap; long double s = 0; int i;\n'
                '    va_start(ap, n);\n    for (i = 0; i < n; i++) {\n'
                '        switch (i %% 4) {\n        case 0: s += va_arg(ap, int); break;\n'
                '        case 1: s += va_arg(ap, double); break;\n        case 2: s += va_arg(ap, long double); break;\n'
                '        default: s += va_arg(ap, long long); break;\n        }\n    }\n    va_end(ap);\n    return s;\n}\n') % name

    def program(self):
        r = self.r
        out = '/* generated by scripts/gen_model_corpus.py cgen */\n#include <stdio.h>\n#include <stdarg.h>\n#include <string.h>\n\n'
        for _ in range(r.randint(0, 3)):
            out += self.struct_def()
        out += 'enum E { E0, E1 = 5, E2, E3 = -1 };\n'
        for i in range(r.randint(1, 6)):
            t = self.scalar()
            init = (' = ' + self.lit(t)) if r.random() < 0.7 else ''
            out += '%s%s g%d%s;\n' % ('static ' if r.random() < 0.2 else '', t, i, init)
            self.globals.append(('g%d' % i, t))
        for s, kind, fields in self.structs:
            named = [f for f in fields if f[1]]
            if named and r.random() < 0.8:
                f = r.choice(named)
                out += '%s %s gs_%s = { .%s = %s };\n' % (kind, s, s, f[1], self.lit(f[0]))
                if r.random() < 0.5:
                    out += '%s %s ga_%s[3] = { [1] = { .%s = %s }, [2].%s = %s };\n' % (
                        kind, s, s, f[1], self.lit(f[0]), f[1], self.lit(f[0]))
        out += 'char gstr[] = "gen\\tstr\\x41\\101";\nconst char *gp = "ptr" "cat";\nint garr[] = { 1, 2, [5] = 9, 3 };\n'
        out += 'double gd[2][3] = { { 1.5, 2.5 }, { [2] = 3.5 } };\n'
        out += self.varargs_func()
        for _ in range(r.randint(1, 4)):
            out += self.func()
        out += 'int main(void)\n{\n'
        vars = list(self.globals)
        for s, kind, fields in self.structs:
            named = [f for f in fields if f[1]]
            if named:
                v = 'ls_' + s
                out += '    %s %s %s = {0};\n' % (kind, s, v)
                for t, f, b in named:
                    out += '    %s.%s = %s;\n' % (v, f, self.lit(t))
                    out += self.printv('%s.%s' % (v, f), t)
        for _ in range(r.randint(3, 10)):
            out += self.stmt(vars, 0, 'int')
        for f in self.funcs:
            args = ', '.join(self.expr(vars, pt) for pt in f[2])
            out += self.printv('%s(%s)' % (f[1], args), f[0])
        out += '    printf("%%g\\n", (double)%s(5, 1, 2.0, 3.0L, 4LL, 5));\n' % self.va
        out += '    printf("%s %d %g\\n", gstr, garr[5], gd[1][2]);\n'
        for v, t in vars:
            out += self.printv(v, t)
        out += '    return 0;\n}\n'
        return out


def gen_cgen(n, seed):
    d = os.path.join(OUT, 'cgen')
    os.makedirs(d, exist_ok=True)
    for i in range(n):
        s = seed + i
        g = G(random.Random(s))
        open(os.path.join(d, 'cgen_%d.c' % s), 'w').write(g.program())


# ---------------------------------------------------------------- mutants
TOKEN = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[A-Za-z_]\w*|\d[\w.]*|\.\.\.|->|\+\+|--|<<=|>>=|[<>=!&|+\-*/%^]=|&&|\|\||<<|>>|##|\S')


def gen_mutants(n, seed):
    import glob
    srcs = sorted(glob.glob(os.path.join(ROOT, 'c/tests/tcc/*.c')) + glob.glob(os.path.join(ROOT, 'corpus/compcert/*.c'))
                  + glob.glob(os.path.join(ROOT, 'corpus/tcc/*.c')))
    d = os.path.join(OUT, 'mutants')
    os.makedirs(d, exist_ok=True)
    r = random.Random(seed)
    junk = ['(', ')', '{', '}', '[', ']', ';', ',', '*', '&', '=', '+', 'int', 'struct', 'return', 'if',
            'else', 'x', '0', '1.5', '"s"', "'c'", '#', '...', 'typedef', 'static', 'void', '?', ':', '->', '.']
    for i in range(n):
        src = r.choice(srcs)
        text = open(src, encoding='latin-1').read()
        toks = [(m.start(), m.end()) for m in TOKEN.finditer(text)]
        # mutate within the program body (skip a leading comment/includes region)
        start = min(len(toks) - 1, len(toks) // 10)
        k = r.randint(start, len(toks) - 1)
        a, b = toks[k]
        op = r.choice(['del', 'dup', 'swap', 'rep'])
        if op == 'del':
            text = text[:a] + ' ' + text[b:]
        elif op == 'dup':
            text = text[:b] + ' ' + text[a:b] + text[b:]
        elif op == 'swap' and k + 1 < len(toks):
            c, e = toks[k + 1]
            text = text[:a] + text[c:e] + text[b:c] + text[a:b] + text[e:]
        else:
            text = text[:a] + r.choice(junk) + text[b:]
        hdr = '/* mutant %d of %s (%s token %d), scripts/gen_model_corpus.py */\n' % (i, os.path.relpath(src, ROOT), op, k)
        open(os.path.join(d, 'mut_%d.c' % (seed + i)), 'w', encoding='latin-1').write(hdr + text)


def gen_dtsplit(n, seed):
    """tinycc's tests2 programs run with -dt (one run per `#if defined
    test_X` case): one program per case, with `#define test_X` in front."""
    d = os.path.join(OUT, 'dtsplit')
    os.makedirs(d, exist_ok=True)
    t2 = os.path.join(ROOT, 'build', 'tcc-src', 'tests', 'tests2')
    for f in ['60_errors_and_warnings.c', '96_nodata_wanted.c', '139_arm64_errors.c', '125_atomic_misc.c',
              '128_run_atexit.c']:
        p = os.path.join(t2, f)
        if not os.path.exists(p):
            continue
        text = open(p, encoding='latin-1').read()
        names = []
        for m in re.finditer(r'defined\s+(test_\w+)', text):
            if m.group(1) not in names:
                names.append(m.group(1))
        for nm in names:
            out = os.path.join(d, '%s__%s.c' % (f[:-2], nm))
            open(out, 'w', encoding='latin-1').write('#define %s\n%s' % (nm, text))


def main():
    kind, n = sys.argv[1], int(sys.argv[2])
    if kind == 'dtsplit':
        return gen_dtsplit(n, 0)
    seed = int(sys.argv[sys.argv.index('--seed') + 1]) if '--seed' in sys.argv else 1
    {'csmith': gen_csmith, 'cgen': gen_cgen, 'mutants': gen_mutants}[kind](n, seed)


if __name__ == '__main__':
    main()
