/* Csmith 0cdc710315cf seed 128, flattened by scripts/csmith_flatten.py */
// Options:   --seed 128 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
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
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = 0x3185299D;
static uint32_t g_7 = 0U;
static uint8_t g_31 = 253U;
static int8_t g_36 = 0;
static int32_t g_39 = 0x4A1D2DAF;
static int8_t *g_40 = &g_36;
static int32_t **g_56 = (void*)0;
static int32_t g_57[4] = {8,8,8,8};

static uint16_t  func_1(void);
static int32_t * func_10(int16_t  p_11, int32_t * p_12);
static const uint32_t  func_17(uint64_t  p_18, int32_t * p_19);

static uint16_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t *l_4 = &g_3;
    int32_t *l_5 = &g_3;
    int32_t *l_6[2];
    uint16_t l_26 = 5U;
    int32_t **l_46 = &l_4;
    int i;
    for (i = 0; i < 2; i++)
        l_6[i] = &g_3;
    --g_7;
    (*l_46) = func_10((safe_add_func_int64_t_s_s((safe_sub_func_uint16_t_u_u((func_17((((safe_lshift_func_int32_t_s_u((safe_sub_func_uint32_t_u_u((safe_mod_func_int16_t_s_s(((void*)0 != l_6[0]), l_26)), 1)), (*l_4))) != g_7) > (*l_2)), l_2) && (*l_4)), (-1))), (*l_5))), &g_3);
    g_57[3] &= ((*l_4) = (((safe_mod_func_int8_t_s_s((safe_add_func_uint64_t_u_u((((safe_mul_func_int16_t_s_s(((*l_2) = (~4U)), 0U)) , (*l_46)) == (void*)0), g_7)), (*g_40))) ^ g_39) ^ 0x4CA67B11082E70D5LL));
    return (*l_2);
}

static int32_t * func_10(int16_t  p_11, int32_t * p_12)
{ 
    int8_t *l_41 = &g_36;
    int32_t l_44 = 1;
    uint32_t *l_45 = &g_7;
    (*p_12) &= (((g_40 = g_40) == l_41) != ((*l_45) |= (safe_lshift_func_uint32_t_u_u((l_44 & g_39), 14))));
    return &g_39;
}

static const uint32_t  func_17(uint64_t  p_18, int32_t * p_19)
{ 
    int16_t l_29 = (-4);
    uint8_t *l_30 = &g_31;
    int8_t *l_34 = (void*)0;
    int8_t *l_35[3][4];
    int32_t l_37 = 0;
    int32_t *l_38[3][1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
            l_35[i][j] = &g_36;
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_38[i][j] = &g_39;
    }
    g_39 |= (((*l_30) = l_29) && (g_3 , (((l_37 = ((safe_mod_func_uint64_t_u_u(g_7, g_3)) > 0x9964)) >= 0xE5) == 0)));
    return g_3;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_36, "g_36", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_57[i], "g_57[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
