/* Csmith 0cdc710315cf seed 269, flattened by scripts/csmith_flatten.py */
// Options:   --seed 269 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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

static uint16_t g_2 = 0xC8B1;
static int16_t g_4 = (-6);
static int32_t g_12 = 0x5B14F4E7;
static int32_t *g_11 = &g_12;
static uint8_t g_23[2][4] = {{1U,0U,1U,1U},{0U,0U,0x68,0U}};
static uint16_t g_46 = 3U;
static uint32_t g_48 = 0x8453370C;
static int32_t *g_57 = &g_12;
static const int32_t * const g_58 = (void*)0;
static int32_t *g_59 = (void*)0;
static int32_t *g_61[2][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0}};

static int8_t  func_1(void);
static int32_t * func_5(int16_t  p_6, int8_t  p_7, int32_t * p_8, int32_t * p_9);
static int32_t * func_15(int32_t * p_16, int32_t * p_17, int32_t  p_18);

static int8_t  func_1(void)
{ 
    int32_t *l_3[4];
    int32_t l_10[3][3] = {{0x0E864896,0x0E864896,0x0E864896},{0xE85A7B05,0x35B263D1,0xE85A7B05},{0x0E864896,0x0E864896,0x0E864896}};
    int32_t **l_60[1][1];
    int i, j;
    for (i = 0; i < 4; i++)
        l_3[i] = (void*)0;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_60[i][j] = &g_57;
    }
    g_4 &= g_2;
    g_61[1][1] = func_5((0x471D | g_4), l_10[0][0], g_11, &g_12);
    (*g_57) = (*g_59);
    return g_48;
}

static int32_t * func_5(int16_t  p_6, int8_t  p_7, int32_t * p_8, int32_t * p_9)
{ 
    int32_t *l_19 = &g_12;
    int32_t **l_55 = &l_19;
    int32_t **l_56[3];
    int i;
    for (i = 0; i < 3; i++)
        l_56[i] = &g_11;
    (*p_9) = ((safe_mul_func_uint8_t_u_u((((((((g_57 = ((*l_55) = func_15(l_19, l_19, (*g_11)))) != g_58) , p_7) < p_7) , p_7) , (*l_55)) == (void*)0), g_4)) , (*p_9));
    g_59 = ((*l_55) = func_15(((*l_55) = func_15(func_15(p_9, p_8, (*p_8)), p_8, p_7)), g_57, g_4));
    return &g_12;
}

static int32_t * func_15(int32_t * p_16, int32_t * p_17, int32_t  p_18)
{ 
    int32_t *l_20 = (void*)0;
    int32_t *l_21 = &g_12;
    int32_t *l_22[2];
    int32_t **l_32[3];
    int8_t l_43 = 1;
    int i;
    for (i = 0; i < 2; i++)
        l_22[i] = &g_12;
    for (i = 0; i < 3; i++)
        l_32[i] = (void*)0;
    ++g_23[0][3];
    if ((p_18 ^= (safe_mul_func_int8_t_s_s((safe_mul_func_int64_t_s_s(((safe_lshift_func_int16_t_s_u(((g_11 = &p_18) != (void*)0), (g_2 |= (((g_23[0][3] < g_23[0][1]) ^ (*p_16)) == 0xA3)))) & 0U), g_4)), g_23[0][3]))))
    { 
        return &g_12;
    }
    else
    { 
        int8_t l_35 = (-5);
        uint64_t l_44 = 18446744073709551615U;
        uint16_t *l_54[1][2];
        uint16_t **l_53 = &l_54[0][1];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_54[i][j] = &g_46;
        }
        if (((*l_21) = (safe_mul_func_uint32_t_u_u(p_18, (0x51 <= l_35)))))
        { 
            uint16_t *l_45[4][2] = {{&g_46,&g_2},{&g_46,&g_2},{&g_46,&g_2},{&g_46,&g_2}};
            int32_t l_47 = 0xDC071782;
            int i, j;
            g_48 ^= (g_12 | (0 ^ (((l_47 &= (((((!(safe_rshift_func_int32_t_s_u((safe_add_func_int32_t_s_s((safe_sub_func_int64_t_s_s((((p_18 , g_23[0][3]) & l_43) < 0xEEB3), 0x67D625C2A3DA7981LL)), 4294967295U)), l_44))) == 0x1A3A4AA3E85184BFLL) || 4) && g_2) && g_23[0][3])) , g_2) > l_44)));
        }
        else
        { 
            l_53 = ((safe_div_func_int32_t_s_s((safe_add_func_int16_t_s_s((g_23[0][3] > (*g_11)), (p_18 >= p_18))), 0x73301997)) , (void*)0);
            for (g_12 = 0; (g_12 <= 1); g_12 += 1)
            { 
                (*g_11) = (*g_11);
                (*g_11) ^= (*p_16);
            }
        }
    }
    return &g_12;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_23[i][j], "g_23[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_46, "g_46", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
