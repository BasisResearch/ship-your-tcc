/* Csmith 0cdc710315cf seed 162, flattened by scripts/csmith_flatten.py */
// Options:   --seed 162 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

/* standalone LP64 prelude (no system headers): the same types as
   <stdint.h> on x86_64 glibc and riscv64 newlib */
int printf(const char *, ...);
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

/* ---- Csmith runtime (flattened from csmith.h / platform_generic.h) ---- */
static uint32_t crc32_tab[256];
static uint32_t crc32_context = 0xFFFFFFFFUL;

static void crc32_gentab(void) {
  uint32_t crc;
  const uint32_t poly = 0xEDB88320UL;
  int i, j;
  for (i = 0; i < 256; i++) {
    crc = i;
    for (j = 8; j > 0; j--) {
      if (crc & 1) {
        crc = (crc >> 1) ^ poly;
      } else {
        crc >>= 1;
      }
    }
    crc32_tab[i] = crc;
  }
}

static void crc32_byte(uint8_t b) {
  crc32_context = ((crc32_context >> 8) & 0x00FFFFFF) ^
                  crc32_tab[(crc32_context ^ b) & 0xFF];
}

static void crc32_8bytes(uint64_t val) {
  crc32_byte((val >> 0) & 0xff);
  crc32_byte((val >> 8) & 0xff);
  crc32_byte((val >> 16) & 0xff);
  crc32_byte((val >> 24) & 0xff);
  crc32_byte((val >> 32) & 0xff);
  crc32_byte((val >> 40) & 0xff);
  crc32_byte((val >> 48) & 0xff);
  crc32_byte((val >> 56) & 0xff);
}

static void transparent_crc(uint64_t val, char *vname, int flag) {
  crc32_8bytes(val);
  if (flag) {
    printf("...checksum after hashing %s : %lX\n", vname,
           crc32_context ^ 0xFFFFFFFFUL);
  }
}

static void platform_main_begin(void) { }

static void platform_main_end(uint32_t crc, int flag) {
  (void)flag;
  printf("checksum = %X\n", crc);
}
/* ---- end of Csmith runtime ---- */
/* ---- safe_math.h wrappers used by this program ---- */
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint16_t  f0;
   int64_t  f1;
};

struct S2 {
   int32_t  f0;
   uint32_t  f1;
   uint16_t  f2;
};

struct S5 {
   uint16_t  f0;
   int64_t  f1;
   uint32_t  f2;
   const int32_t  f3;
};

struct S6 {
   uint32_t  f0;
   int16_t  f1;
   struct S5  f2;
   int32_t  f3;
};

static struct S6 g_6[1][2] = {{{0x78ACC4B7,0x7008,{0U,7,0U,0xA964A90C},-8},{0x78ACC4B7,0x7008,{0U,7,0U,0xA964A90C},-8}}};
static const struct S0 g_12 = {65535U,0x10CBEDFA90EF874ALL};
static const struct S0 *g_11 = &g_12;
static const struct S0 **g_10 = &g_11;
static struct S0 g_16 = {65526U,0xB40284818A88ACC1LL};
static struct S2 g_20 = {1,4294967295U,0x62CF};
static int32_t g_31 = 5;
static int32_t *g_30 = &g_31;
static uint32_t g_36 = 0xBFBD42F9;

static uint8_t  func_1(void);
static const struct S0 * func_2(int8_t  p_3, struct S6  p_4);
static struct S0 ** func_8(const struct S0 ** p_9);

static uint8_t  func_1(void)
{ 
    uint16_t l_5 = 65530U;
    uint16_t l_39 = 0x0040;
    (*g_10) = func_2(l_5, g_6[0][0]);
    return l_39;
}

static const struct S0 * func_2(int8_t  p_3, struct S6  p_4)
{ 
    int32_t l_7[1];
    struct S0 *l_26 = &g_16;
    int32_t *l_32 = &g_20.f0;
    int32_t *l_33 = &g_20.f0;
    int32_t *l_34 = &g_20.f0;
    int32_t *l_35[1];
    int i;
    for (i = 0; i < 1; i++)
        l_7[i] = 4;
    for (i = 0; i < 1; i++)
        l_35[i] = &l_7[0];
    for (p_4.f0 = 0; (p_4.f0 <= 0); p_4.f0 += 1)
    { 
        struct S0 *l_15 = &g_16;
        struct S0 **l_14[1];
        struct S2 **l_17 = (void*)0;
        struct S2 *l_19 = &g_20;
        struct S2 **l_18 = &l_19;
        struct S2 *l_22 = &g_20;
        struct S2 **l_21 = &l_22;
        int i;
        for (i = 0; i < 1; i++)
            l_14[i] = &l_15;
        l_14[0] = func_8(g_10);
        (*l_21) = ((*l_18) = (void*)0);
        for (g_20.f0 = 0; (g_20.f0 >= 0); g_20.f0 -= 1)
        { 
            int32_t *l_25 = &l_7[g_20.f0];
            int32_t *l_27 = (void*)0;
            int32_t l_28[3][4];
            int32_t **l_29[1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 4; j++)
                    l_28[i][j] = 1;
            }
            for (i = 0; i < 1; i++)
                l_29[i] = (void*)0;
            l_28[1][3] = (safe_sub_func_int32_t_s_s(((*l_25) = l_7[g_20.f0]), (((void*)0 == l_26) & 0x15A0349B)));
            g_30 = ((((p_4.f2.f2 && g_20.f1) < (p_4.f3 , p_4.f2.f1)) ^ 0x61CAE467) , (void*)0);
        }
    }
    g_36--;
    return l_26;
}

static struct S0 ** func_8(const struct S0 ** p_9)
{ 
    struct S0 **l_13[1][2];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_13[i][j] = (void*)0;
    }
    return l_13[0][0];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_6[i][j].f0, "g_6[i][j].f0", print_hash_value);
            transparent_crc(g_6[i][j].f1, "g_6[i][j].f1", print_hash_value);
            transparent_crc(g_6[i][j].f2.f0, "g_6[i][j].f2.f0", print_hash_value);
            transparent_crc(g_6[i][j].f2.f1, "g_6[i][j].f2.f1", print_hash_value);
            transparent_crc(g_6[i][j].f2.f2, "g_6[i][j].f2.f2", print_hash_value);
            transparent_crc(g_6[i][j].f2.f3, "g_6[i][j].f2.f3", print_hash_value);
            transparent_crc(g_6[i][j].f3, "g_6[i][j].f3", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_12.f0, "g_12.f0", print_hash_value);
    transparent_crc(g_12.f1, "g_12.f1", print_hash_value);
    transparent_crc(g_16.f0, "g_16.f0", print_hash_value);
    transparent_crc(g_16.f1, "g_16.f1", print_hash_value);
    transparent_crc(g_20.f0, "g_20.f0", print_hash_value);
    transparent_crc(g_20.f1, "g_20.f1", print_hash_value);
    transparent_crc(g_20.f2, "g_20.f2", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_36, "g_36", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
