/* Csmith 0cdc710315cf seed 301, flattened by scripts/csmith_flatten.py */
// Options:   --seed 301 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int32_t  f0;
   uint16_t  f1;
   uint16_t  f2;
   int64_t  f3;
};

union U1 {
   const uint16_t  f0;
   int8_t  f1;
};

static const uint16_t g_14 = 1U;
static const int32_t g_17 = 0xB6F65E79;
static const int32_t *g_16 = &g_17;
static int32_t g_20 = (-1);
static int32_t * const g_19 = &g_20;
static uint64_t g_23[1] = {0xD0C11E043EEF7D26LL};
static struct S0 g_24 = {0x54E5878E,0x7C3B,0xC39C,3};
static int32_t *g_40 = &g_20;
static union U1 g_46 = {65535U};
static uint64_t g_48 = 18446744073709551607U;
static struct S0 g_54 = {-2,0x2EE7,7U,9};
static struct S0 *g_63 = &g_54;
static struct S0 **g_62 = &g_63;
static uint8_t g_78 = 0x6C;
static int32_t *g_83 = &g_20;
static int16_t g_88 = (-7);
static uint32_t g_92[2] = {0U,0U};
static uint32_t g_115 = 0xAF309D66;
static union U1 *g_126 = &g_46;
static union U1 **g_125 = &g_126;

static uint16_t  func_1(void);
static int32_t * func_2(int32_t * p_3, int32_t  p_4, int32_t * p_5, int8_t  p_6, const uint16_t  p_7);
static int32_t * func_8(const int8_t  p_9, int32_t  p_10, const int32_t * p_11, const uint16_t  p_12, int32_t * const  p_13);

static uint16_t  func_1(void)
{ 
    const int32_t *l_15 = (void*)0;
    const int32_t l_18 = 0x735B263D;
    int32_t **l_41 = (void*)0;
    int32_t l_47 = 9;
    struct S0 l_53 = {0xB476018F,65535U,0U,1};
    int8_t l_91 = (-1);
    l_15 = func_2(func_8(g_14, g_14, (g_16 = l_15), l_18, g_19), (*g_19), &g_20, g_14, g_17);
    for (g_24.f2 = 0; (g_24.f2 > 39); g_24.f2 = safe_add_func_uint64_t_u_u(g_24.f2, 5))
    { 
        int8_t l_44[2][2];
        int32_t *l_45[1];
        int64_t l_66 = 0xA3E88205D61439ABLL;
        int8_t l_74 = 0xEA;
        int32_t *l_84 = &l_47;
        int32_t **l_106[1];
        struct S0 l_108 = {0x8B080850,0x123E,65532U,0};
        uint16_t l_114 = 0x7BB9;
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
                l_44[i][j] = 0x56;
        }
        for (i = 0; i < 1; i++)
            l_45[i] = &g_20;
        for (i = 0; i < 1; i++)
            l_106[i] = &g_83;
    }
    g_24.f0 |= ((*g_19) ^= ((((*g_62) = (*g_62)) == (void*)0) && (safe_rshift_func_uint16_t_u_s((g_125 == &g_126), g_88))));
    return g_24.f2;
}

static int32_t * func_2(int32_t * p_3, int32_t  p_4, int32_t * p_5, int8_t  p_6, const uint16_t  p_7)
{ 
    struct S0 *l_26 = &g_24;
    int16_t l_37 = 0xD446;
    for (p_4 = 0; (p_4 <= 0); p_4 += 1)
    { 
        int8_t l_38 = 0x25;
        for (g_20 = 0; (g_20 >= 0); g_20 -= 1)
        { 
            struct S0 *l_25 = &g_24;
            int i;
            if (g_23[g_20])
                break;
            (*l_25) = g_24;
        }
        for (g_24.f2 = 0; (g_24.f2 <= 0); g_24.f2 += 1)
        { 
            uint64_t l_28[1];
            int32_t *l_31 = &g_20;
            struct S0 * const l_36 = &g_24;
            int i;
            for (i = 0; i < 1; i++)
                l_28[i] = 0x3F0AEACB2D505BC9LL;
            for (g_24.f1 = 0; (g_24.f1 <= 0); g_24.f1 += 1)
            { 
                struct S0 **l_27 = &l_26;
                int32_t *l_30 = &g_24.f0;
                int32_t **l_29[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_29[i] = &l_30;
                (*l_27) = l_26;
                l_31 = func_8(g_23[g_24.f2], ((-10) || l_28[0]), &g_17, g_24.f3, &g_20);
                if ((*l_31))
                    continue;
            }
            if ((safe_div_func_uint8_t_u_u((((((p_4 || p_7) | ((safe_rshift_func_uint64_t_u_s(((l_36 == &g_24) != (*l_31)), 16)) | p_6)) , p_4) == l_37) && p_6), l_38)))
            { 
                int32_t **l_39[4][2] = {{&l_31,&l_31},{&l_31,&l_31},{&l_31,&l_31},{&l_31,&l_31}};
                int i, j;
                g_40 = &p_4;
                g_40 = (void*)0;
            }
            else
            { 
                (*g_19) = (*p_3);
            }
        }
    }
    return &g_20;
}

static int32_t * func_8(const int8_t  p_9, int32_t  p_10, const int32_t * p_11, const uint16_t  p_12, int32_t * const  p_13)
{ 
    struct S0 l_21 = {1,0xE76E,65535U,0xF4E79753589FE57FLL};
    struct S0 *l_22 = &l_21;
    (*l_22) = l_21;
    return &g_20;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_23[i], "g_23[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_24.f0, "g_24.f0", print_hash_value);
    transparent_crc(g_24.f1, "g_24.f1", print_hash_value);
    transparent_crc(g_24.f2, "g_24.f2", print_hash_value);
    transparent_crc(g_24.f3, "g_24.f3", print_hash_value);
    transparent_crc(g_46.f0, "g_46.f0", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_54.f0, "g_54.f0", print_hash_value);
    transparent_crc(g_54.f1, "g_54.f1", print_hash_value);
    transparent_crc(g_54.f2, "g_54.f2", print_hash_value);
    transparent_crc(g_54.f3, "g_54.f3", print_hash_value);
    transparent_crc(g_78, "g_78", print_hash_value);
    transparent_crc(g_88, "g_88", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_92[i], "g_92[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_115, "g_115", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
