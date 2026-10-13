/* Csmith 0cdc710315cf seed 300, flattened by scripts/csmith_flatten.py */
// Options:   --seed 300 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
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
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
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
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static int64_t
safe_unary_minus_func_int64_t_s(int64_t si )
{
  return
    (si==(-9223372036854775807L-1)) ?
    ((si)) :
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int16_t  f0;
   uint8_t  f1;
   int32_t  f2;
};

struct S1 {
   uint64_t  f0;
   struct S0  f1;
};

static uint16_t g_2[4] = {1U,1U,1U,1U};
static int32_t g_3 = 1;
static int32_t g_16 = 1;
static struct S0 g_22 = {0x0385,0x06,1};
static struct S0 g_24 = {0xDF11,0xAF,0xE2CD8491};
static int16_t g_30[3] = {0,0,0};
static struct S1 g_42 = {0x64544A95A4792D8CLL,{-1,0U,0x4BCB8161}};

static struct S1  func_1(void);
static struct S1  func_4(uint8_t  p_5, int16_t  p_6, uint16_t  p_7, int16_t  p_8, int8_t  p_9);
static const int8_t  func_18(struct S1  p_19);

static struct S1  func_1(void)
{ 
    uint8_t l_49 = 2U;
    struct S1 l_54 = {5U,{0,0x4E,0x3343DEF5}};
    for (g_3 = 0; (g_3 <= 3); g_3 += 1)
    { 
        int64_t l_10 = (-6);
        struct S1 *l_41[3][2] = {{&g_42,&g_42},{&g_42,&g_42},{&g_42,&g_42}};
        int32_t *l_53 = &g_42.f1.f2;
        int i, j;
        g_42 = func_4((g_2[0] ^ g_3), l_10, g_3, g_3, g_3);
        (*l_53) = ((safe_sub_func_int16_t_s_s((safe_mul_func_int16_t_s_s((safe_rshift_func_int8_t_s_s((l_49 || l_49), (safe_div_func_uint8_t_u_u((!1U), l_49)))), 0xC31B)), 4U)) > g_42.f1.f2);
    }
    return l_54;
}

static struct S1  func_4(uint8_t  p_5, int16_t  p_6, uint16_t  p_7, int16_t  p_8, int8_t  p_9)
{ 
    int8_t l_27 = 0x2E;
    int32_t *l_39 = &g_16;
    struct S1 l_40 = {0U,{1,5U,0}};
    for (p_6 = 0; (p_6 <= 28); p_6 = safe_add_func_int64_t_s_s(p_6, 1))
    { 
        int32_t *l_15 = &g_16;
        int32_t l_17 = 9;
        struct S1 l_20 = {0x1C1D99EA5D840DFELL,{-1,0x64,2}};
        int32_t *l_26[1][1];
        uint8_t *l_37 = &l_20.f1.f1;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_26[i][j] = &l_17;
        }
        l_27 = ((((safe_sub_func_uint8_t_u_u((((*l_15) &= p_9) < l_17), func_18(l_20))) || g_22.f0) < p_8) <= 4U);
        if (p_7)
            break;
        g_24.f2 = ((safe_lshift_func_uint64_t_u_u(0x5DBFE6B87C2657A7LL, 2)) & (((((*l_37) = ((g_30[0] & (safe_mod_func_int64_t_s_s((safe_sub_func_int16_t_s_s((safe_lshift_func_uint16_t_u_u(l_27, 9)), 0xB602)), g_24.f2))) & g_24.f1)) , g_22.f2) ^ g_16) <= p_7));
    }
    (*l_39) &= ((safe_unary_minus_func_int64_t_s(p_8)) >= g_30[0]);
    return l_40;
}

static const int8_t  func_18(struct S1  p_19)
{ 
    struct S0 *l_21 = &g_22;
    struct S0 *l_23 = &g_24;
    const uint8_t l_25 = 250U;
    (*l_23) = ((*l_21) = p_19.f1);
    return l_25;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_22.f0, "g_22.f0", print_hash_value);
    transparent_crc(g_22.f1, "g_22.f1", print_hash_value);
    transparent_crc(g_22.f2, "g_22.f2", print_hash_value);
    transparent_crc(g_24.f0, "g_24.f0", print_hash_value);
    transparent_crc(g_24.f1, "g_24.f1", print_hash_value);
    transparent_crc(g_24.f2, "g_24.f2", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_30[i], "g_30[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_42.f0, "g_42.f0", print_hash_value);
    transparent_crc(g_42.f1.f0, "g_42.f1.f0", print_hash_value);
    transparent_crc(g_42.f1.f1, "g_42.f1.f1", print_hash_value);
    transparent_crc(g_42.f1.f2, "g_42.f1.f2", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
