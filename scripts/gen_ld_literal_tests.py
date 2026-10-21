#!/usr/bin/env python3
"""Exact decimal binary128 regression vectors. No compiler/reference inputs.

Build rational test inputs, spell their decimal text using integer arithmetic,
and calculate the expected IEEE bits by exact quotient/remainder rounding.
The image uses newlib integer bignums; the Lean model uses SF.roundRat.
Regenerate c/tests/model/ld_boundary.c with this script; --check checks drift.
--stress-out FILE emits the deterministic 156-case supplementary stress input.
"""
from fractions import Fraction as F
from pathlib import Path
import argparse

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'c/tests/model/ld_boundary.c'


def pow2(e):
    return F(2 ** e) if e >= 0 else F(1, 2 ** -e)


def decimal(x, exact=False, direction=0):
    """Positive x, terminating exact decimal or 160-digit decimal bracket."""
    if not x:
        return '0.0'
    if exact:
        n, d = x.numerator, x.denominator
        k = d.bit_length() - 1
        assert d == 2 ** k
        text = str(n * 5 ** k).rjust(k + 1, '0')
        return (text[:-k] + '.' + text[-k:]) if k else text + '.0'
    n, d = x.numerator, x.denominator
    e = len(str(n)) - len(str(d))
    scale = F(10 ** e) if e >= 0 else F(1, 10 ** -e)
    if x < scale:
        e -= 1
    shift = 159 - e
    y = x * (10 ** shift if shift >= 0 else F(1, 10 ** -shift))
    q, r = divmod(y.numerator, y.denominator)
    if direction > 0 and r:
        q += 1
    return str(q) + 'e' + str(e - 159)


def bits(text):
    """Round the decimal rational directly to its binary128 quantum."""
    x = F(text)
    if text.startswith('-'):
        return (1 << 127) | bits(text[1:])
    if not x:
        return 0
    n, d = x.numerator, x.denominator
    e = n.bit_length() - d.bit_length()
    if x < pow2(e):
        e -= 1
    quantum = max(e - 112, -16494)
    y = x / pow2(quantum)
    q, r = divmod(y.numerator, y.denominator)
    if 2 * r > y.denominator or (2 * r == y.denominator and q % 2):
        q += 1
    if q == 2 ** 113:
        q //= 2
        quantum += 1
    if q < 2 ** 112:
        return q
    exponent = quantum + 16495
    if exponent >= 32767:
        return 32767 << 112
    return (exponent << 112) | (q - 2 ** 112)


def vectors():
    cases = [('zero', '0.0'), ('simple', '1.5'), ('tenth', '0.1'),
             ('large exponent', '1e4932'), ('overflow', '1e4933'),
             ('tiny', '1e-5000'), ('huge exponent', '1e999999999'),
             ('tiny exponent', '1e-999999999')]
    for name, x in [('beyond binary64', 1 + pow2(-100)),
                    ('beyond x87', 1 + pow2(-80))]:
        cases.append((name, decimal(x, exact=True)))
    for name, mid in [('even tie at one', 1 + pow2(-113)),
                      ('odd tie at one', 1 + 3 * pow2(-113)),
                      ('carry tie at two', 2 - pow2(-113))]:
        for suffix, delta in [('below', -pow2(-180)), ('tie', F(0)), ('above', pow2(-180))]:
            cases.append((name + ' ' + suffix, decimal(mid + delta, exact=True)))
    # Exact midpoint spellings at these extremes exceed TinyCC's 1024-byte
    # token limit. Use decimal rationals on both sides, never a rounded tie.
    for name, x in [('half minimum subnormal', pow2(-16495)),
                    ('odd subnormal midpoint', 3 * pow2(-16495)),
                    ('minimum subnormal', pow2(-16494)),
                    ('normal/subnormal midpoint', pow2(-16382) - pow2(-16495)),
                    ('minimum normal', pow2(-16382)),
                    ('maximum finite', (2 - pow2(-112)) * pow2(16383)),
                    ('overflow midpoint', (2 - pow2(-113)) * pow2(16383))]:
        for direction in [-1, 1]:
            cases.append((name + (' below' if direction < 0 else ' above'), decimal(x, direction=direction)))
    # Pinned strtodg's 114-bit directed path returned short significands
    # for these long decimal expansions just below a dyadic value.
    for mantissa, exponent in [(29, -15906), (35, 4621), (17, 10265)]:
        cases.append((f'gdtoa short significand {mantissa}p{exponent}',
                      decimal(F(mantissa) * pow2(exponent))))
    cases += [('negative zero', '-0.0'), ('negative finite', '-0.1'),
              ('negative tiny', '-1e-5000'), ('negative overflow', '-1e4933')]
    return cases


def stress_vectors():
    import random
    cases = []
    for e in [-16382, -1022, -100, -10, 0, 1, 10, 100, 1024, 16000, 16383]:
        for side, mid in [('lower', pow2(e) - pow2(e - 114)),
                          ('upper', pow2(e) + pow2(e - 113))]:
            for direction in [-1, 1]:
                cases.append((f'binade {e} {side} {direction}',
                              decimal(mid, direction=direction)))
            if -500 <= e <= 100:
                cases.append((f'binade {e} {side} tie', decimal(mid, exact=True)))
    rng = random.Random(20261021)
    for i in range(100):
        x = F(rng.randrange(1, 10 ** rng.randrange(2, 180))) * pow2(rng.randrange(-16500, 16000))
        cases.append((f'random {i}', decimal(x)))
    return cases


def render(cases=None):
    if cases is None:
        cases = vectors()
    lines = ['/* Generated by scripts/gen_ld_literal_tests.py; edit the generator.',
             '   Decimal strings and expected binary128 bits come from exact rationals.',
             '   No reference compiler, model arena or stack data is used. */',
             '#include <stdio.h>', '#include <string.h>',
             'static const struct { long double value; unsigned long long lo, hi; } cases[] = {']
    for name, text in cases:
        # Huge exponents have a trivial mathematical answer without constructing
        # a billion-digit integer in Fraction.
        b = (32767 << 112) if text == '1e999999999' else 0 if text == '1e-999999999' else bits(text)
        lines += ['    /* ' + name + ' */',
                  f'    {{ {text}L, 0x{b % 2**64:016x}ULL, 0x{b >> 64:016x}ULL }},']
    lines += ['};', 'int main(void) {', '    unsigned i, failures = 0;',
              '    for (i = 0; i < sizeof cases / sizeof cases[0]; ++i) {',
              '        unsigned long long words[2];',
              '        memcpy(words, &cases[i].value, sizeof words);',
              '        if (words[0] != cases[i].lo || words[1] != cases[i].hi) {',
              '            printf("binary128 case %u failed: %llx %llx expected %llx %llx\\n", i, words[1], words[0], cases[i].hi, cases[i].lo);',
              '            ++failures;', '        }', '    }',
              '    printf("binary128 %u/%u\\n", i - failures, i);',
              '    return failures != 0;', '}', '']
    return '\n'.join(lines)


def main():
    # Python limits decimal printing by default; boundary numerators need 5k.
    import sys
    sys.set_int_max_str_digits(20000)
    ap = argparse.ArgumentParser()
    ap.add_argument('--check', action='store_true')
    ap.add_argument('--stress-out', type=Path)
    args = ap.parse_args()
    if args.stress_out:
        args.stress_out.write_text(render(stress_vectors()))
        return
    content = render()
    if args.check:
        assert OUT.read_text() == content, f'{OUT}: regenerate with {Path(__file__).name}'
    else:
        OUT.write_text(content)


if __name__ == '__main__':
    main()
