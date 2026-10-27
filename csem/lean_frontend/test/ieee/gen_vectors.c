/* CerbIEEE differential vectors from the host (x86-64 glibc, gcc libgcc,
   libquadmath): results/csem/ieee-2026-10-27.md.

     gen_vectors f128   N SEED   binary128 ops through libgcc's soft-fp
                                  (__addtf3 … __trunctfsf2) and sqrtq
     gen_vectors strto  N SEED   literal strings through strtof/strtod/strtof128
     gen_vectors printf N SEED   glibc printf (double) and quadmath_snprintf
                                  (binary128) outputs

   Output lines are tab-separated; hex fields are the IEEE bit patterns
   (binary128 as 32 hex digits, high half first). Build:
     gcc -O1 -o gen_vectors gen_vectors.c -lquadmath -lm */
#define _GNU_SOURCE
#include <quadmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

typedef unsigned __int128 u128;
static uint64_t S;
static uint64_t rnd(void) { S ^= S << 13; S ^= S >> 7; S ^= S << 17; return S; }

static u128 qbits(__float128 x) { u128 u; memcpy(&u, &x, 16); return u; }
static __float128 qfrom(u128 u) { __float128 x; memcpy(&x, &u, 16); return x; }
static uint64_t dbits(double x) { uint64_t u; memcpy(&u, &x, 8); return u; }
static double dfrom(uint64_t u) { double x; memcpy(&x, &u, 8); return x; }
static uint32_t fbits(float x) { uint32_t u; memcpy(&u, &x, 4); return u; }
static float ffrom(uint32_t u) { float x; memcpy(&x, &u, 4); return x; }
static void pq(u128 u) { printf("%016llx%016llx", (unsigned long long)(u >> 64), (unsigned long long)u); }

/* ---------- binary128 operands ---------- */
#define QEXP(u) ((int)((u >> 112) & 0x7fff))
static u128 mk(int neg, int ex, u128 frac) {
  return ((u128)(neg != 0) << 127) | ((u128)(ex & 0x7fff) << 112) | (frac & (((u128)1 << 112) - 1));
}
static u128 rfrac(void) { return ((u128)rnd() << 64) | rnd(); }
static const u128 *edges_q(int *n) {
  static u128 e[40]; int k = 0;
  e[k++] = mk(0, 0, 0); e[k++] = mk(1, 0, 0);
  e[k++] = mk(0, 0, 1); e[k++] = mk(1, 0, 1);
  e[k++] = mk(0, 0, ((u128)1 << 112) - 1);
  e[k++] = mk(0, 1, 0); e[k++] = mk(1, 1, 0);
  e[k++] = mk(0, 0x3fff, 0); e[k++] = mk(1, 0x3fff, 0); e[k++] = mk(0, 0x3fff, 1);
  e[k++] = mk(0, 0x3ffe, ((u128)1 << 112) - 1);
  e[k++] = mk(0, 0x7ffe, ((u128)1 << 112) - 1); e[k++] = mk(1, 0x7ffe, ((u128)1 << 112) - 1);
  e[k++] = mk(0, 0x7fff, 0); e[k++] = mk(1, 0x7fff, 0);
  e[k++] = mk(0, 0x7fff, (u128)1 << 111); e[k++] = mk(1, 0x7fff, ((u128)1 << 111) | 5);
  e[k++] = mk(0, 0x7fff, 1);
  e[k++] = mk(0, 0x4000, 0); e[k++] = mk(0, 0x3ffd, 0);
  e[k++] = qbits(0.1Q); e[k++] = qbits(-3.0Q); e[k++] = qbits(1e4000Q); e[k++] = qbits(1e-4940Q);
  e[k++] = mk(0, 0x403e, 0); e[k++] = mk(1, 0x403e, 0); e[k++] = mk(0, 0x401e, 0);
  e[k++] = mk(0, 0x403d, ((u128)1 << 112) - 1);
  *n = k; return e;
}
static u128 rq(void) {
  int n; const u128 *e = edges_q(&n);
  switch (rnd() % 6) {
  case 0: return rfrac() | ((u128)rnd() << 64 & ((u128)0xffff << 112));      /* random bits */
  case 1: return e[rnd() % n];
  case 2: { int ex = rnd() % 2 ? (int)(rnd() % 8) : 0x7ff7 + (int)(rnd() % 9);
            return mk(rnd() & 1, ex, rfrac()); }
  case 3: return mk(rnd() & 1, 0x3fff + (int)(rnd() % 200) - 100, rfrac() & rfrac() & rfrac());
  default: return mk(rnd() & 1, 0x3fff + (int)(rnd() % 4000) - 2000, rfrac());
  }
}
static u128 near(u128 x) {   /* an operand whose exponent is close to x's (ties, cancellation) */
  int ex = QEXP(x) + (int)(rnd() % 240) - 120;
  if (ex < 0) ex = 0;
  if (ex > 0x7ffe) ex = 0x7ffe;
  switch (rnd() % 3) {
  case 0: return mk(rnd() & 1, ex, rfrac());
  case 1: return mk(rnd() & 1, QEXP(x) - 113 > 0 ? QEXP(x) - 113 : 0, 0);  /* half an ulp */
  default: return x ^ ((u128)1 << 127) ^ (rnd() & 0xff);               /* near-cancellation */
  }
}

static int f128(long n) {
  for (long i = 0; i < n; i++) {
    u128 a = rq(), b = (rnd() % 2) ? near(a) : rq();
    __float128 x = qfrom(a), y = qfrom(b);
    printf("add\t"); pq(a); printf("\t"); pq(b); printf("\t"); pq(qbits(x + y)); printf("\n");
    printf("sub\t"); pq(a); printf("\t"); pq(b); printf("\t"); pq(qbits(x - y)); printf("\n");
    printf("mul\t"); pq(a); printf("\t"); pq(b); printf("\t"); pq(qbits(x * y)); printf("\n");
    printf("div\t"); pq(a); printf("\t"); pq(b); printf("\t"); pq(qbits(x / y)); printf("\n");
    printf("sqrt\t"); pq(a); printf("\t0\t"); pq(qbits(sqrtq(x))); printf("\n");
    printf("cmp\t"); pq(a); printf("\t"); pq(b); printf("\t%d%d%d%d%d%d\n",
           x == y, x != y, x < y, x <= y, x > y, x >= y);
    /* conversions: __fixtfdi/__fixtfsi (in range only), __floatditf, __floatsitf,
       __extenddftf2/__trunctfdf2, __extendsftf2/__trunctfsf2 */
    if (!isnanq(x) && fabsq(x) < 9.2e18Q) printf("fixdi\t"), pq(a), printf("\t0\t%lld\n", (long long)x);
    if (!isnanq(x) && fabsq(x) < 2.1e9Q) printf("fixsi\t"), pq(a), printf("\t0\t%d\n", (int)x);
    long long ll = (long long)rnd() >> (rnd() % 64);
    printf("floatdi\t%lld\t0\t", ll); pq(qbits((__float128)ll)); printf("\n");
    int ii = (int)rnd() >> (rnd() % 32);
    printf("floatsi\t%d\t0\t", ii); pq(qbits((__float128)ii)); printf("\n");
    uint64_t d = (rnd() % 3) ? rnd() : dbits((double)x);
    printf("extenddf\t%016llx\t0\t", (unsigned long long)d); pq(qbits((__float128)dfrom(d))); printf("\n");
    printf("truncdf\t"); pq(a); printf("\t0\t%016llx\n", (unsigned long long)dbits((double)x));
    uint32_t f = (uint32_t)rnd();
    printf("extendsf\t%08x\t0\t", f); pq(qbits((__float128)ffrom(f))); printf("\n");
    printf("truncsf\t"); pq(a); printf("\t0\t%08x\n", fbits((float)x));
  }
  return 0;
}

/* ---------- literal strings ---------- */
static void emit_lit(const char *s) {
  printf("lit\t%s\t%08x\t%016llx\t", s, fbits(strtof(s, NULL)),
         (unsigned long long)dbits(strtod(s, NULL)));
  pq(qbits((__float128)strtof128(s, NULL))); printf("\n");
}
static void rdigits(char *p, int n) { for (int i = 0; i < n; i++) p[i] = '0' + rnd() % 10; p[n] = 0; }
static int strto(long n) {
  char buf[1200], d1[900], d2[64];
  for (long i = 0; i < n; i++) {
    int k = rnd() % 8;
    if (k == 0) {        /* short decimal, wide exponent */
      rdigits(d1, 1 + rnd() % 20);
      snprintf(buf, sizeof buf, "%s%se%d", d1, (rnd() & 1) ? ".5" : "", (int)(rnd() % 10000) - 5000);
    } else if (k == 1) { /* fraction form */
      rdigits(d1, 1 + rnd() % 6); rdigits(d2, 1 + rnd() % 30);
      snprintf(buf, sizeof buf, "%s.%s", d1, d2);
    } else if (k == 2) { /* midpoint of two adjacent doubles, exact (long double holds it) */
      double a = dfrom(rnd() & 0x7fefffffffffffffULL);
      long double m = ((long double)a + (long double)nextafter(a, INFINITY)) / 2;
      snprintf(buf, sizeof buf, "%.*Le", 780, m);
      /* trim trailing zeros of the mantissa to keep the string exact but short */
      char *e = strchr(buf, 'e'); char *z = e; while (z[-1] == '0') z--;
      memmove(z, e, strlen(e) + 1);
      if (rnd() % 3 == 0) { /* perturb: just above / below the tie */
        char *e2 = strchr(buf, 'e'); memmove(e2 + 1, e2, strlen(e2) + 1); *e2 = (rnd() & 1) ? '1' : '9';
      }
    } else if (k == 3) { /* midpoint of two adjacent floats, exact in double */
      float a = ffrom((uint32_t)rnd() & 0x7f7fffff);
      double m = ((double)a + (double)nextafterf(a, INFINITY)) / 2;
      snprintf(buf, sizeof buf, "%.*e", 200, m);
      char *e = strchr(buf, 'e'); char *z = e; while (z[-1] == '0') z--;
      memmove(z, e, strlen(e) + 1);
    } else if (k == 4) { /* subnormal range */
      rdigits(d1, 1 + rnd() % 25);
      int which = rnd() % 3;
      int ex = which == 0 ? -(int)(300 + rnd() % 30) : which == 1 ? -(int)(36 + rnd() % 12) : -(int)(4920 + rnd() % 50);
      snprintf(buf, sizeof buf, "0.%se%d", d1, ex);
    } else if (k == 5) { /* hex */
      snprintf(buf, sizeof buf, "0x%llx.%llxp%d", (unsigned long long)(rnd() >> (rnd() % 64)),
               (unsigned long long)rnd(), (int)(rnd() % 34000) - 17000);
    } else if (k == 6) { /* hex, subnormal and near overflow */
      int ex = (rnd() & 1) ? -(int)(1070 + rnd() % 60) : (int)(1000 + rnd() % 60);
      if (rnd() % 3 == 0) ex = (rnd() & 1) ? -(int)(16440 + rnd() % 60) : (int)(16340 + rnd() % 60);
      if (rnd() % 3 == 0) ex = (rnd() & 1) ? -(int)(145 + rnd() % 30) : (int)(100 + rnd() % 30);
      snprintf(buf, sizeof buf, "0X%llX.%llxP%+d", (unsigned long long)(rnd() >> (rnd() % 64)),
               (unsigned long long)(rnd() >> (rnd() % 64)), ex);
    } else {             /* long mantissa */
      rdigits(d1, 1 + rnd() % 800);
      snprintf(buf, sizeof buf, "%c.%se%d", '1' + (int)(rnd() % 9), d1, (int)(rnd() % 700) - 350);
    }
    emit_lit(buf);
  }
  const char *fixed[] = {"0", "0.0", "1", "1.", ".5", "0x1p0", "0x.8p1", "1e-400", "1e400",
    "2.2250738585072011e-308", "2.4703282292062327e-324", "2.4703282292062328e-324",
    "4.9406564584124654e-324", "1.7976931348623157e308", "1.7976931348623158e308",
    "3.4028235677973366e38", "1.4012984643248170e-45", "7.0064923216240854e-46",
    "1.18973149535723176508575932662800702e4932", "6.475175119438025110924438958227646552e-4966",
    "0x1.fffffffffffff8p1023", "0x1p-1075", "0x1.00000000000008p0", "0x1.000001p0",
    "123456789012345678901234567890", "1.0000000596046447753906250000008271806",
    "1.000000059604644775390625", "0.1", "0.2", "0.3", "1e23", "8.98846567431158e307", NULL};
  for (int i = 0; fixed[i]; i++) emit_lit(fixed[i]);
  return 0;
}

/* ---------- printf ---------- */
static const char *specs[] = {"%f", "%e", "%g", "%a", "%F", "%E", "%G", "%A", "%.0f", "%.0e",
  "%.0g", "%.1g", "%.3f", "%.17g", "%.20e", "%#.0f", "%#.0e", "%#g", "%#.3g", "%+e", "% f",
  "%-14.5g|", "%014.4e", "%+012.3f", "%30.20g", "%.40f", "%.13a", "%.3a", "%.0a", "%#a",
  "%08a", "%-+20.10e|", "%.60e", "%.100g", "%5.1f", "%#.8G", "% 012g", NULL};
static double rd(void) {
  switch (rnd() % 6) {
  case 0: return dfrom(rnd());
  case 1: return dfrom(rnd() & 0x800fffffffffffffULL);                /* subnormal */
  case 2: return (double)(long long)(rnd() >> (rnd() % 64)) / (double)(1ULL << (rnd() % 20));
  case 3: return (double)(rnd() % 100000) / 1000.0 * ((rnd() & 1) ? 1 : -1);
  case 4: { double v[] = {0.0, -0.0, 0.5, 1.5, 2.5, 0.125, 9.5, 99.5, 0.05, 1e22, 1e-5, 1e-4,
                          123456.5, 1e300, INFINITY, -INFINITY, NAN, 5e-324, 2.2250738585072014e-308};
            return v[rnd() % (sizeof v / sizeof v[0])]; }
  default: return ldexp((double)(rnd() >> 11), (int)(rnd() % 200) - 100 - 53);
  }
}
static int pf(long n) {
  char out[6000], qspec[64];
  for (long i = 0; i < n; i++) {
    double x = rd();
    for (int s = 0; specs[s]; s++) {
      snprintf(out, sizeof out, specs[s], x);
      if (isnan(x) && signbit(x)) continue;
      printf("pd\t%s\t%016llx\t%s\n", specs[s], (unsigned long long)dbits(x), out);
    }
    if (i % 4 == 0) {   /* binary128 through libquadmath */
      __float128 q = qfrom(rq());
      if (rnd() % 2) q = (__float128)x * (1 + 1e-25Q * (rnd() % 1000));
      for (int s = 0; specs[s]; s++) {
        /* turn "%…c" into "%…Qc" */
        size_t L = strlen(specs[s]); const char *bar = strchr(specs[s], '|');
        size_t cpos = bar ? (size_t)(bar - specs[s]) - 1 : L - 1;
        memcpy(qspec, specs[s], cpos); qspec[cpos] = 'Q'; qspec[cpos+1] = specs[s][cpos]; qspec[cpos+2] = 0;
        int rc = quadmath_snprintf(out, sizeof out, qspec, q);
        if (rc < 0 || rc >= (int)sizeof out) { fprintf(stderr, "quadmath format failed: %s\n", qspec); return 1; }
        if (isnanq(q)) continue;
        printf("pq\t%s\t", specs[s]); pq(qbits(q)); printf("\t%s%s\n", out, specs[s] + cpos + 1);
      }
    }
  }
  return 0;
}

/* ---------- a C program for the rv64 target (Sail) ----------
   The operands are generated here; the program computes every operation with
   the target's own libgcc (and newlib's sqrtl) and prints lines in the f128
   vector format, which `ieee-test vectors` checks. */
static void cq(u128 u) { printf("{0x%016llxULL,0x%016llxULL}", (unsigned long long)u, (unsigned long long)(u >> 64)); }
static int sailc(long n) {
  printf("/* generated by gen_vectors sailc %ld: binary128 through the rv64 libgcc */\n", n);
  printf("#include <stdio.h>\n#include <string.h>\n#include <math.h>\n");
  printf("typedef unsigned long long u64;\nstatic const u64 A[][2] = {\n");
  u128 *as = malloc(n * sizeof(u128)), *bs = malloc(n * sizeof(u128));
  for (long i = 0; i < n; i++) { as[i] = rq(); bs[i] = (rnd() % 2) ? near(as[i]) : rq(); }
  for (long i = 0; i < n; i++) { cq(as[i]); printf(",\n"); }
  printf("};\nstatic const u64 B[][2] = {\n");
  for (long i = 0; i < n; i++) { cq(bs[i]); printf(",\n"); }
  printf("};\n");
  printf("static const long long I[] = {");
  for (long i = 0; i < n; i++) printf("%lldLL,", (long long)rnd() >> (rnd() % 64));
  printf("};\nstatic const u64 D[] = {");
  for (long i = 0; i < n; i++) printf("0x%016llxULL,", (unsigned long long)rnd());
  printf("};\nstatic const unsigned F[] = {");
  for (long i = 0; i < n; i++) printf("0x%08xU,", (unsigned)rnd());
  printf("};\n");
  printf("%s",
    "static long double L(const u64 *p) { long double x; memcpy(&x, p, 16); return x; }\n"
    "static void pq(long double x) { u64 u[2]; memcpy(u, &x, 16); printf(\"%016llx%016llx\", u[1], u[0]); }\n"
    "static void pa(const u64 *p) { printf(\"%016llx%016llx\", p[1], p[0]); }\n"
    "#define BIN(name, expr) do { printf(name \"\\t\"); pa(A[i]); printf(\"\\t\"); pa(B[i]); printf(\"\\t\"); pq(expr); printf(\"\\n\"); } while (0)\n"
    "int main(void) {\n"
    "  int n = (int)(sizeof A / sizeof A[0]);\n"
    "  for (int i = 0; i < n; i++) {\n"
    "    long double x = L(A[i]), y = L(B[i]);\n"
    "    BIN(\"add\", x + y); BIN(\"sub\", x - y); BIN(\"mul\", x * y); BIN(\"div\", x / y);\n"
    "    printf(\"sqrt\\t\"); pa(A[i]); printf(\"\\t0\\t\"); pq(sqrtl(x)); printf(\"\\n\");\n"
    "    printf(\"cmp\\t\"); pa(A[i]); printf(\"\\t\"); pa(B[i]);\n"
    "    printf(\"\\t%d%d%d%d%d%d\\n\", x == y, x != y, x < y, x <= y, x > y, x >= y);\n"
    "    if (x == x && x < 9.2e18L && x > -9.2e18L) { printf(\"fixdi\\t\"); pa(A[i]); printf(\"\\t0\\t%lld\\n\", (long long)x); }\n"
    "    if (x == x && x < 2.1e9L && x > -2.1e9L) { printf(\"fixsi\\t\"); pa(A[i]); printf(\"\\t0\\t%d\\n\", (int)x); }\n"
    "    printf(\"floatdi\\t%lld\\t0\\t\", I[i]); pq((long double)I[i]); printf(\"\\n\");\n"
    "    printf(\"floatsi\\t%d\\t0\\t\", (int)I[i]); pq((long double)(int)I[i]); printf(\"\\n\");\n"
    "    { double d; u64 db; memcpy(&d, &D[i], 8); printf(\"extenddf\\t%016llx\\t0\\t\", D[i]); pq((long double)d); printf(\"\\n\");\n"
    "      d = (double)x; memcpy(&db, &d, 8); printf(\"truncdf\\t\"); pa(A[i]); printf(\"\\t0\\t%016llx\\n\", db); }\n"
    "    { float f; unsigned fb; memcpy(&f, &F[i], 4); printf(\"extendsf\\t%08x\\t0\\t\", F[i]); pq((long double)f); printf(\"\\n\");\n"
    "      f = (float)x; memcpy(&fb, &f, 4); printf(\"truncsf\\t\"); pa(A[i]); printf(\"\\t0\\t%08x\\n\", fb); }\n"
    "  }\n  return 0;\n}\n");
  return 0;
}

/* ---------- a printf program for tcc on Sail (newlib's printf) ----------
   Prints "pd"/"pq" lines in the printf vector format for N doubles and N/2
   long doubles under every spec in `specs`. */
static int sailp(long n) {
  printf("/* generated by gen_vectors sailp %ld: newlib printf under tcc -run */\n", n);
  printf("#include <stdio.h>\n#include <string.h>\ntypedef unsigned long long u64;\n");
  printf("static const u64 D[] = {");
  for (long i = 0; i < n; i++) {
    double x = rd();
    if (isnan(x)) x = 1.25;
    printf("0x%016llxULL,", (unsigned long long)dbits(x));
  }
  printf("};\nstatic const u64 Q[][2] = {");
  for (long i = 0; i < n / 2; i++) {
    __float128 q = qfrom(rq());
    if (isnanq(q)) q = 3.5Q;
    if (rnd() % 2) q = (__float128)rd() * (1 + 1e-25Q * (rnd() % 1000));
    if (isnanq(q)) q = -2.75Q;
    u128 u = qbits(q);
    printf("{0x%016llxULL,0x%016llxULL},", (unsigned long long)u, (unsigned long long)(u >> 64));
  }
  printf("};\nstatic const char *S[] = {");
  for (int s = 0; specs[s]; s++) printf("\"%s\",", specs[s]);
  printf("0};\nstatic const char *SL[] = {");
  for (int s = 0; specs[s]; s++) {
    size_t L = strlen(specs[s]); const char *bar = strchr(specs[s], '|');
    size_t cpos = bar ? (size_t)(bar - specs[s]) - 1 : L - 1;
    printf("\"%.*sL%s\",", (int)cpos, specs[s], specs[s] + cpos);
  }
  printf("0};\n%s",
    "int main(void) {\n"
    "  char out[6000];\n"
    "  for (int i = 0; i < (int)(sizeof D / sizeof D[0]); i++) {\n"
    "    double x; memcpy(&x, &D[i], 8);\n"
    "    for (int s = 0; S[s]; s++) { int rc = snprintf(out, sizeof out, S[s], x); if (rc < 0 || rc >= (int)sizeof out) return 2; printf(\"pd\\t%s\\t%016llx\\t%s\\n\", S[s], D[i], out); }\n"
    "  }\n"
    "  for (int i = 0; i < (int)(sizeof Q / sizeof Q[0]); i++) {\n"
    "    long double x; memcpy(&x, Q[i], 16);\n"
    "    for (int s = 0; SL[s]; s++) { int rc = snprintf(out, sizeof out, SL[s], x); if (rc < 0 || rc >= (int)sizeof out) return 2; printf(\"pq\\t%s\\t%016llx%016llx\\t%s\\n\", S[s], Q[i][1], Q[i][0], out); }\n"
    "  }\n  return 0;\n}\n");
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 4) { fprintf(stderr, "usage: gen_vectors f128|strto|printf|sailc|sailp N SEED\n"); return 2; }
  if (!strcmp(argv[1], "sailp")) { S = strtoull(argv[3], NULL, 10) * 2654435761ULL + 88172645463325252ULL; return sailp(atol(argv[2])); }
  if (!strcmp(argv[1], "sailc")) { S = strtoull(argv[3], NULL, 10) * 2654435761ULL + 88172645463325252ULL; return sailc(atol(argv[2])); }
  long n = atol(argv[2]); S = strtoull(argv[3], NULL, 10) * 2654435761ULL + 88172645463325252ULL;
  if (!strcmp(argv[1], "f128")) return f128(n);
  if (!strcmp(argv[1], "strto")) return strto(n);
  if (!strcmp(argv[1], "printf")) return pf(n);
  return 2;
}
