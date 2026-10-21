/* 6.10.1: controlling expressions. */
#if '\377' < 0
char_signed
#else
char_unsigned
#endif
#if 'ab' == 0x6162
multichar_bigendian
#endif
#if L'\xff' == 255
wide_ff
#endif
#if 0 && (1 / 0)
no
#else
short_circuit_ok
#endif
#if 1
one
#elif 1 / 0
never
#endif
#if (-1 >> 1) == -1
asr
#endif
#if 0x7fffffffffffffff == 9223372036854775807 && 0xffffffffffffffff == -1
wrap_u
#endif
#if defined(__STDC__) && __STDC__ == 1 && __STDC_VERSION__ == 199901L && __STDC_HOSTED__
stdc
#endif
#define D defined(D)
#if D
defined_from_macro
#endif
#if true
true_is_one
#else
true_is_zero
#endif
#if 0b101 == 5
binary
#endif
#if (2 || 1/0) && (0 ? 1/0 : 1)
cond_ok
#endif
#if 18446744073709551615u == -1
umax
#endif
