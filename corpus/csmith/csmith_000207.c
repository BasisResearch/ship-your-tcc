/* Csmith 0cdc710315cf seed 207, flattened by scripts/csmith_flatten.py */
// Options:   --seed 207 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
};

union U2 {
   int32_t  f0;
   int16_t  f1;
   struct S0  f2;
};

union U4 {
   struct S0  f0;
   uint8_t  f1;
   struct S0  f2;
};

static const uint8_t g_2 = 255U;
static int8_t g_12 = 0;
static union U2 g_13[3][1] = {{{0xA4E86524}},{{0xA4E86524}},{{0xA4E86524}}};
static const int32_t g_20 = 0x55A63A74;
static const int32_t *g_19 = &g_20;
static int64_t g_44 = 0x063446B7D869E772LL;
static uint8_t g_46 = 0xD6;
static int32_t g_48 = 0xADD890AA;
static uint64_t g_56 = 0x1AB372C247AFAE86LL;
static struct S0 g_58 = {251U};

static const uint32_t  func_1(void);
static uint32_t  func_5(int8_t  p_6, union U2  p_7);
static int32_t * func_14(const int32_t * p_15, int16_t  p_16, uint8_t  p_17, union U4  p_18);

static const uint32_t  func_1(void)
{ 
    uint16_t l_10[2][2] = {{0U,0U},{0U,0U}};
    int8_t *l_11 = &g_12;
    int16_t *l_55 = (void*)0;
    int32_t *l_57 = &g_13[1][0].f0;
    struct S0 *l_59 = &g_58;
    int i, j;
    (*l_57) = (g_2 == (g_56 &= (safe_div_func_uint32_t_u_u(func_5(((*l_11) |= ((safe_rshift_func_int64_t_s_u(l_10[1][0], 23)) <= (g_2 <= g_2))), g_13[1][0]), g_20))));
    (*l_59) = g_58;
    return g_13[1][0].f0;
}

static uint32_t  func_5(int8_t  p_6, union U2  p_7)
{ 
    int8_t *l_23 = &g_12;
    int32_t l_24 = 0x90660D64;
    union U4 l_25 = {{1U}};
    int32_t *l_54 = &l_24;
    int32_t **l_53 = &l_54;
    (*l_53) = func_14(g_19, (safe_add_func_uint16_t_u_u((l_23 == &g_12), l_24)), g_2, l_25);
    return g_20;
}

static int32_t * func_14(const int32_t * p_15, int16_t  p_16, uint8_t  p_17, union U4  p_18)
{ 
    int64_t l_30[1][4];
    int8_t *l_42 = &g_12;
    int64_t *l_43 = &g_44;
    uint8_t *l_45 = &g_46;
    int32_t *l_47 = &g_48;
    int32_t *l_52 = &g_13[1][0].f0;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_30[i][j] = 0;
    }
    (*l_47) &= (safe_mul_func_uint32_t_u_u((((safe_div_func_int8_t_s_s(l_30[0][1], (((*l_45) &= ((((safe_mul_func_int32_t_s_s((safe_lshift_func_int32_t_s_s((*g_19), ((safe_unary_minus_func_uint64_t_u((safe_lshift_func_int16_t_s_u(((((*l_42) = (((*l_43) ^= ((safe_lshift_func_uint64_t_u_s((safe_lshift_func_int64_t_s_s((&g_12 == l_42), 19)), 2)) && 0U)) != g_13[1][0].f0)) == l_30[0][1]) <= 1), g_13[1][0].f0)))) > p_16))), p_18.f0.f0)) || 0x90F5F2FE868E0133LL) < 0x1F7977F45A323196LL) < 0x14F5)) | p_17))) || (-1)) >= (-6)), (*g_19)));
    (*l_52) = ((((safe_sub_func_int8_t_s_s(((*l_47) = (((~((((((g_12 < (*l_47)) < ((void*)0 != l_45)) >= p_16) > g_48) ^ p_17) < p_17)) < (*l_47)) == g_46)), g_12)) >= p_16) || 0U) < 0U);
    return &g_48;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_13[i][j].f0, "g_13[i][j].f0", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_58.f0, "g_58.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
