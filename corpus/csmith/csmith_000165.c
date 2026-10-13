/* Csmith 0cdc710315cf seed 165, flattened by scripts/csmith_flatten.py */
// Options:   --seed 165 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = 0xF0B96CA2;
static int64_t g_4 = 0xCAD2086BD085C9E5LL;
static int16_t g_6 = 0xA32A;
static int64_t g_7 = 4;
static uint8_t g_8 = 0xFD;
static uint64_t g_18 = 18446744073709551614U;
static uint64_t g_22 = 0xDFE924F63ABEFA02LL;
static uint64_t *g_21[3][1] = {{&g_22},{&g_22},{&g_22}};
static int32_t g_26 = 1;
static int8_t g_29[4][1] = {{0x6F},{(-1)},{0x6F},{(-1)}};
static int32_t g_33 = 0x535B5B06;
static uint16_t g_35 = 1U;
static int32_t *g_57 = &g_3;
static int32_t **g_56 = &g_57;

static int64_t  func_1(void);
static int32_t  func_13(uint64_t  p_14, int8_t  p_15, int32_t * p_16);
static int32_t  func_19(int16_t  p_20);

static int64_t  func_1(void)
{ 
    int32_t *l_2[4][2] = {{&g_3,&g_3},{&g_3,&g_3},{&g_3,&g_3},{&g_3,&g_3}};
    int8_t l_5 = 0xF9;
    int i, j;
    g_8--;
    for (g_3 = (-5); (g_3 > 4); g_3 = safe_add_func_int32_t_s_s(g_3, 1))
    { 
        uint64_t *l_17[4][3];
        uint64_t **l_23 = &g_21[0][0];
        int16_t *l_24[1];
        int32_t l_60 = 0x9E206048;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 3; j++)
                l_17[i][j] = &g_18;
        }
        for (i = 0; i < 1; i++)
            l_24[i] = &g_6;
        g_33 = func_13((g_7 , (g_18 = 18446744073709551615U)), (func_19((g_6 = (((*l_23) = g_21[0][0]) == (void*)0))) || l_60), &l_60);
    }
    return g_18;
}

static int32_t  func_13(uint64_t  p_14, int8_t  p_15, int32_t * p_16)
{ 
    int64_t l_65[2][4] = {{0xF4763CD51DC99B0BLL,0xD1A38AD7B5901663LL,0xF4763CD51DC99B0BLL,0xF4763CD51DC99B0BLL},{0xD1A38AD7B5901663LL,0xD1A38AD7B5901663LL,0xF359A1248F7C4FBBLL,0xD1A38AD7B5901663LL}};
    int i, j;
    (*p_16) = (safe_lshift_func_int64_t_s_u((safe_mul_func_int64_t_s_s(p_14, l_65[1][1])), ((-3) == p_14)));
    return (*g_57);
}

static int32_t  func_19(int16_t  p_20)
{ 
    int16_t l_27 = 0;
    int32_t l_30 = 0x17854C3E;
    int32_t l_31 = 0xED994AD4;
    int32_t l_32 = 0x85024D8F;
    int32_t l_34 = 0x25AC89D6;
    uint64_t **l_46 = &g_21[0][0];
    int32_t **l_58 = &g_57;
    if (g_7)
    { 
        int32_t *l_25 = &g_26;
        int32_t *l_28[4][1];
        uint64_t **l_47[4];
        int32_t **l_48 = &l_28[0][0];
        int32_t **l_49 = &l_25;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_28[i][j] = &g_26;
        }
        for (i = 0; i < 4; i++)
            l_47[i] = (void*)0;
        g_35++;
        (*l_49) = ((*l_48) = ((safe_mod_func_uint64_t_u_u(((p_20 , &l_30) != ((safe_mul_func_int64_t_s_s((safe_mul_func_uint32_t_u_u((safe_div_func_int64_t_s_s(((l_46 = &g_21[1][0]) != l_47[3]), l_30)), p_20)), l_34)) , &g_33)), (-1))) , &l_31));
    }
    else
    { 
        int16_t l_50 = 0xF1A7;
        int32_t *l_51 = &l_34;
        int32_t *l_59 = &l_30;
        (*l_51) ^= l_50;
        (*l_59) &= ((safe_mod_func_int8_t_s_s(((g_35 || ((g_33 , g_56) == l_58)) , g_4), g_35)) , (*l_51));
        (*l_58) = (*l_58);
    }
    return (**g_56);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_29[i][j], "g_29[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
