/* Csmith 0cdc710315cf seed 75, flattened by scripts/csmith_flatten.py */
// Options:   --seed 75 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_add_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 + si2);
}
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
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
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint32_t  f0;
};

static int32_t g_4 = 0x7689351F;
static uint32_t g_12 = 0x1C3A23FE;
static int32_t *g_19 = &g_4;
static int32_t * const *g_18 = &g_19;
static uint16_t g_24 = 0xB34C;
static int64_t g_35 = 0xFF79FACD4A797449LL;
static int8_t g_41 = 0x90;
static int32_t g_61[4][4] = {{3,3,3,3},{3,3,3,3},{3,3,3,3},{3,3,3,3}};

static int64_t  func_1(void);
static union U0  func_5(uint16_t  p_6, int32_t * p_7, int64_t  p_8, int32_t * const * p_9, int32_t * p_10);
static int8_t  func_15(int16_t  p_16);

static int64_t  func_1(void)
{ 
    int32_t *l_3 = &g_4;
    int32_t **l_2 = &l_3;
    uint32_t *l_11 = &g_12;
    int32_t *l_68 = &g_61[1][1];
    (*l_2) = (void*)0;
    (*l_68) ^= (func_5((((*l_11) |= g_4) && (safe_mul_func_int8_t_s_s(func_15((((void*)0 == (*l_2)) > 0x5DA2)), g_4))), &g_4, g_4, g_18, (*l_2)) , (**g_18));
    return g_61[0][3];
}

static union U0  func_5(uint16_t  p_6, int32_t * p_7, int64_t  p_8, int32_t * const * p_9, int32_t * p_10)
{ 
    int32_t **l_20 = &g_19;
    int32_t l_23 = (-4);
    int64_t *l_33 = (void*)0;
    int64_t *l_34 = &g_35;
    int8_t *l_40 = &g_41;
    int32_t l_57[3][2] = {{0,0},{5,0},{0,5}};
    int64_t l_62 = 1;
    uint32_t l_64 = 3U;
    union U0 l_67 = {0xEC541CD8};
    int i, j;
    (*l_20) = (*p_9);
    (*g_19) = (safe_mod_func_int16_t_s_s(l_23, ((g_24 = p_6) ^ ((safe_sub_func_int16_t_s_s((safe_rshift_func_int16_t_s_s((safe_add_func_int16_t_s_s((safe_mul_func_int64_t_s_s(((*l_34) = p_8), 0x4BE3D0E72EE72C34LL)), g_4)), g_12)), p_8)) == 0x5F5C9B8DB370CC32LL))));
    if (((((safe_mul_func_int32_t_s_s((**g_18), ((safe_div_func_int8_t_s_s(((*l_40) ^= (&p_10 != &p_7)), g_35)) , g_24))) <= 0xFB18) < (**l_20)) , 0))
    { 
        const uint32_t l_52 = 0xEFC518F8;
        int32_t *l_53 = &l_23;
        l_23 &= ((0x8045 ^ ((4294967295U || (safe_sub_func_uint16_t_u_u((safe_add_func_int32_t_s_s(((**g_18) = (((((((safe_sub_func_int32_t_s_s((safe_div_func_int8_t_s_s((safe_mul_func_uint16_t_u_u(((l_20 = &g_19) == (void*)0), 3U)), 0x90)), g_24)) , g_35) , g_4) <= 4294967295U) , 7U) == g_4) , (*g_19))), l_52)), l_52))) & l_52)) , (*g_19));
        (*l_53) = ((**g_18) = 0xAF36EAF0);
    }
    else
    { 
        int8_t l_55 = 0x88;
        int32_t l_56 = 1;
        int32_t *l_58 = (void*)0;
        int32_t *l_59 = &l_23;
        int32_t *l_60[2];
        int8_t l_63 = 7;
        int i;
        for (i = 0; i < 2; i++)
            l_60[i] = &l_57[2][1];
        l_56 ^= ((!0x47) , ((((void*)0 != l_40) && l_55) , 4));
        --l_64;
    }
    return l_67;
}

static int8_t  func_15(int16_t  p_16)
{ 
    int8_t l_17[2][4] = {{1,1,1,1},{1,0xDE,0xDE,1}};
    int i, j;
    return l_17[0][3];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_61[i][j], "g_61[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
