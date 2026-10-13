/* Csmith 0cdc710315cf seed 117, flattened by scripts/csmith_flatten.py */
// Options:   --seed 117 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
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
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   int8_t  f0;
   uint64_t  f1;
};

union U1 {
   const uint64_t  f0;
};

static int64_t g_14 = 0;
static uint64_t g_19 = 0U;
static uint32_t g_25 = 0x5EE4C20F;
static union U1 g_29 = {18446744073709551615U};
static int64_t g_42 = 6;
static uint32_t g_46 = 6U;
static int32_t g_48 = 0x6369DDA3;
static int32_t *g_47 = &g_48;
static uint8_t g_56 = 255U;

static uint32_t  func_1(void);
static int32_t * func_2(int32_t * p_3, int32_t * p_4, uint32_t  p_5);
static int32_t * func_6(union U0  p_7, int64_t  p_8, uint16_t  p_9, const int8_t  p_10);

static uint32_t  func_1(void)
{ 
    uint8_t l_13 = 255U;
    uint64_t *l_17 = (void*)0;
    uint64_t *l_18 = &g_19;
    union U0 l_20 = {0x37};
    int32_t *l_49 = &g_48;
    int64_t *l_60 = (void*)0;
    int64_t **l_59[3][2] = {{&l_60,&l_60},{&l_60,&l_60},{&l_60,&l_60}};
    int i, j;
    l_49 = func_2(func_6(((safe_add_func_int8_t_s_s((((*l_18) ^= (l_13 || (g_14 ^ (((safe_rshift_func_int16_t_s_s(0xAE7B, 9)) ^ g_14) , l_13)))) <= g_14), 0xF4)) , l_20), l_20.f0, g_14, g_14), l_49, (*l_49));
    l_59[1][1] = l_59[1][1];
    return g_14;
}

static int32_t * func_2(int32_t * p_3, int32_t * p_4, uint32_t  p_5)
{ 
    int32_t *l_50 = &g_48;
    int32_t l_51[3][2];
    int32_t *l_52 = &l_51[1][1];
    int32_t *l_53 = &l_51[2][1];
    int32_t *l_54[3][1];
    int64_t l_55 = 0;
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_51[i][j] = 0x26E16511;
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_54[i][j] = &l_51[2][1];
    }
    g_56--;
    return p_3;
}

static int32_t * func_6(union U0  p_7, int64_t  p_8, uint16_t  p_9, const int8_t  p_10)
{ 
    int64_t l_23 = (-1);
    uint32_t *l_24 = &g_25;
    union U1 *l_28[3][4] = {{&g_29,&g_29,&g_29,&g_29},{&g_29,&g_29,(void*)0,&g_29},{&g_29,&g_29,&g_29,&g_29}};
    union U1 *l_31 = &g_29;
    union U1 **l_30 = &l_31;
    union U0 l_32[1][4] = {{{0x3F},{0x3F},{0x3F},{0x3F}}};
    int64_t *l_41 = &g_42;
    int64_t *l_43 = &g_14;
    uint64_t *l_44 = &g_19;
    int32_t *l_45[2];
    int i, j;
    for (i = 0; i < 2; i++)
        l_45[i] = (void*)0;
    (*l_30) = ((safe_add_func_int32_t_s_s(l_23, ((*l_24)++))) , (l_28[0][0] = l_28[0][0]));
    g_46 = (((l_32[0][1] , ((*l_44) = (l_23 < (safe_rshift_func_uint16_t_u_s((safe_mod_func_int64_t_s_s(((*l_43) = (safe_sub_func_int64_t_s_s((((safe_mod_func_int64_t_s_s(((*l_41) &= (p_8 = ((g_14 < 0U) , 0x2589D38951FE511BLL))), g_14)) , g_14) >= g_29.f0), p_10))), g_29.f0)), 10))))) <= l_23) == 2);
    return g_47;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_29.f0, "g_29.f0", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
