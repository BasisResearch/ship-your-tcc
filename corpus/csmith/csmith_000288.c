/* Csmith 0cdc710315cf seed 288, flattened by scripts/csmith_flatten.py */
// Options:   --seed 288 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int8_t  f0;
};

union U1 {
   const int64_t  f0;
   const uint16_t  f1;
   struct S0  f2;
};

static struct S0 g_13 = {0x8A};
static uint16_t g_20 = 0xFAA3;
static union U1 g_38 = {1};
static int8_t g_42 = 0x39;

static int64_t  func_1(void);
static struct S0  func_2(struct S0  p_3, union U1  p_4, union U1  p_5, int16_t  p_6);
static struct S0  func_7(int8_t  p_8, const struct S0  p_9, const struct S0  p_10, uint32_t  p_11, union U1  p_12);

static int64_t  func_1(void)
{ 
    uint32_t l_19 = 4294967295U;
    const struct S0 l_21[4][4] = {{{7},{7},{7},{7}},{{7},{7},{7},{7}},{{7},{7},{7},{7}},{{7},{7},{7},{7}}};
    union U1 l_22 = {0x1A05132D7AF2C9A1LL};
    union U1 l_43 = {0x649690B2811FFBE6LL};
    int8_t l_44[1][2];
    struct S0 *l_45 = &l_43.f2;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_44[i][j] = 0xDE;
    }
    (*l_45) = func_2((func_7((g_13 , (safe_mul_func_uint8_t_u_u((safe_mul_func_int64_t_s_s((!l_19), (((l_19 | g_13.f0) <= g_20) , 1U))), l_19))), l_21[3][0], l_21[3][0], g_13.f0, l_22) , g_13), g_38, l_43, l_44[0][1]);
    return l_21[3][0].f0;
}

static struct S0  func_2(struct S0  p_3, union U1  p_4, union U1  p_5, int16_t  p_6)
{ 
    return p_3;
}

static struct S0  func_7(int8_t  p_8, const struct S0  p_9, const struct S0  p_10, uint32_t  p_11, union U1  p_12)
{ 
    uint8_t l_27 = 1U;
    uint32_t *l_39[4];
    int32_t l_40 = 0x9031EA06;
    int32_t *l_41[2][4];
    int i, j;
    for (i = 0; i < 4; i++)
        l_39[i] = (void*)0;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
            l_41[i][j] = (void*)0;
    }
    g_42 = (safe_lshift_func_int32_t_s_u((safe_mod_func_uint32_t_u_u(((l_27 , ((safe_div_func_int8_t_s_s((safe_mod_func_uint64_t_u_u((safe_rshift_func_uint8_t_u_s((safe_add_func_uint16_t_u_u((g_38 , ((l_40 = (p_9.f0 , l_27)) > 0x0A7C6DD3)), 7U)), 5)), 0xBBE58ED0BD2F23D3LL)), p_8)) == 0)) ^ 0xAA), l_27)), 23));
    return g_13;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_13.f0, "g_13.f0", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_38.f0, "g_38.f0", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
