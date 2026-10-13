/* Csmith 0cdc710315cf seed 116, flattened by scripts/csmith_flatten.py */
// Options:   --seed 116 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
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
   uint64_t  f0;
   uint8_t  f1;
   uint64_t  f2;
};

static int32_t g_2 = 1;
static int8_t g_11[1] = {0xF8};
static uint8_t g_39 = 255U;
static int8_t g_41 = 1;
static int32_t g_44 = 1;
static int8_t g_47 = 0x5D;
static uint64_t g_54 = 0xEDB9CC5119B5F814LL;
static struct S0 g_55 = {0x3C4855F075F97E6ELL,251U,1U};
static struct S0 g_59 = {0xD4ED1E22517922C1LL,0x97,0x0CE5FA5B821BFDB9LL};
static struct S0 g_61 = {7U,0xF6,18446744073709551615U};
static struct S0 g_63 = {0x28D85E26160D4A06LL,0xEE,0x324E311B86553ECFLL};
static struct S0 *g_70 = (void*)0;
static struct S0 **g_69 = &g_70;
static int32_t *g_76 = &g_44;
static int32_t *g_86 = &g_44;
static uint16_t g_116 = 0xFAB5;

static uint64_t  func_1(void);
static int32_t  func_5(const int16_t  p_6, uint16_t  p_7, int8_t  p_8, int8_t  p_9);
static int8_t  func_16(int16_t  p_17, struct S0  p_18);

static uint64_t  func_1(void)
{ 
    uint32_t l_23 = 0x8AAF0A9B;
    const struct S0 *l_95 = (void*)0;
    const struct S0 **l_94 = &l_95;
    int32_t **l_108 = &g_86;
    uint16_t l_122 = 1U;
    for (g_2 = 0; (g_2 <= (-16)); g_2--)
    { 
        const int8_t l_10 = 0x91;
        struct S0 l_24[1][3] = {{{18446744073709551607U,0xBE,0xA1E1C5A3D7F19125LL},{18446744073709551607U,0xBE,0xA1E1C5A3D7F19125LL},{18446744073709551607U,0xBE,0xA1E1C5A3D7F19125LL}}};
        int32_t * const l_72 = (void*)0;
        struct S0 **l_93 = &g_70;
        int i, j;
    }
    return (**l_108);
}

static int32_t  func_5(const int16_t  p_6, uint16_t  p_7, int8_t  p_8, int8_t  p_9)
{ 
    struct S0 *l_56 = &g_55;
    struct S0 *l_57 = (void*)0;
    struct S0 *l_58 = &g_59;
    struct S0 *l_60 = &g_61;
    struct S0 *l_62[2][4];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
            l_62[i][j] = (void*)0;
    }
    g_63 = ((*l_60) = ((*l_58) = ((*l_56) = g_55)));
    (*l_56) = ((*l_60) = g_63);
    return p_8;
}

static int8_t  func_16(int16_t  p_17, struct S0  p_18)
{ 
    uint8_t *l_38 = &g_39;
    int8_t *l_40 = &g_41;
    int32_t l_42 = 1;
    int32_t *l_43 = &g_44;
    uint64_t l_46 = 0x1731D497C242EA6ALL;
    int32_t *l_53[2][1];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_53[i][j] = (void*)0;
    }
    (*l_43) ^= ((safe_lshift_func_uint64_t_u_u((!((g_11[0] < (safe_rshift_func_uint64_t_u_u((safe_sub_func_int32_t_s_s(((safe_lshift_func_int32_t_s_u((safe_rshift_func_uint16_t_u_u((((p_18.f0 , ((*l_38) &= (p_18.f1--))) & (((((*l_40) = 0) > 0xC4) && 1) <= (-1))) && p_18.f2), l_42)), 21)) >= l_42), 0x0A3418EB)), 43))) != 0x39150040B319BD12LL)), l_42)) & p_18.f2);
    g_47 |= (+(((p_18.f0 >= (g_11[0] > (((l_46 & p_18.f2) && 9) & (*l_43)))) > (*l_43)) != g_11[0]));
    g_54 |= ((safe_rshift_func_int16_t_s_s((safe_div_func_int8_t_s_s((!((((p_17 && ((((*l_43) = (((0x66C0953B != p_17) ^ 0) & (*l_43))) <= l_46) <= 0x5D82)) , p_18.f1) , &l_42) == &g_2)), g_41)), p_18.f2)) , p_18.f0);
    return g_11[0];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_11[i], "g_11[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_54, "g_54", print_hash_value);
    transparent_crc(g_55.f0, "g_55.f0", print_hash_value);
    transparent_crc(g_55.f1, "g_55.f1", print_hash_value);
    transparent_crc(g_55.f2, "g_55.f2", print_hash_value);
    transparent_crc(g_59.f0, "g_59.f0", print_hash_value);
    transparent_crc(g_59.f1, "g_59.f1", print_hash_value);
    transparent_crc(g_59.f2, "g_59.f2", print_hash_value);
    transparent_crc(g_61.f0, "g_61.f0", print_hash_value);
    transparent_crc(g_61.f1, "g_61.f1", print_hash_value);
    transparent_crc(g_61.f2, "g_61.f2", print_hash_value);
    transparent_crc(g_63.f0, "g_63.f0", print_hash_value);
    transparent_crc(g_63.f1, "g_63.f1", print_hash_value);
    transparent_crc(g_63.f2, "g_63.f2", print_hash_value);
    transparent_crc(g_116, "g_116", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
