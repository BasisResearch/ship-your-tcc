/* Csmith 0cdc710315cf seed 129, flattened by scripts/csmith_flatten.py */
// Options:   --seed 129 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (4294967295U >> ((int)right)))) ?
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
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint32_t
safe_unary_minus_func_uint32_t_u(uint32_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   uint8_t  f1;
};

static struct S0 g_6 = {0xA1836CCB03629AA8LL,0x0D};
static int32_t g_8 = (-1);
static int32_t * const g_7[3] = {&g_8,&g_8,&g_8};
static int32_t *g_30[1][1] = {{&g_8}};
static int32_t g_32 = 0xF851F08A;
static uint32_t g_55 = 0x33B115E5;
static uint64_t g_59 = 0xA65522591552D49ALL;
static int8_t g_61 = 0;
static struct S0 g_64 = {0xC3D8A5C089A473F1LL,0x02};
static int8_t *g_76 = &g_61;
static int8_t ** const g_75 = &g_76;
static int32_t *g_80 = &g_32;
static int32_t **g_90 = &g_80;
static uint16_t g_93 = 0x5A4B;
static uint8_t g_95 = 0x66;

static int8_t  func_1(void);
static int32_t * func_2(struct S0  p_3, int32_t * const  p_4, int64_t  p_5);
static uint8_t  func_11(int32_t * p_12, int32_t  p_13, int8_t  p_14);

static int8_t  func_1(void)
{ 
    int32_t **l_79 = (void*)0;
    struct S0 l_83 = {1,0x6A};
    int32_t **l_89[1];
    int8_t l_91 = 0x29;
    int64_t l_92 = 0xDD35AB2A6A61002FLL;
    int64_t l_94 = 0x4BCFCB35CAECAF6BLL;
    int i;
    for (i = 0; i < 1; i++)
        l_89[i] = &g_30[0][0];
    g_80 = (g_30[0][0] = func_2(g_6, g_7[0], g_8));
    for (g_64.f1 = 0; (g_64.f1 < 51); g_64.f1 = safe_add_func_int16_t_s_s(g_64.f1, 1))
    { 
        struct S0 l_84 = {0x535ACB962FAD3287LL,255U};
        l_84 = l_83;
    }
    l_94 = (safe_add_func_int8_t_s_s((safe_lshift_func_int16_t_s_u(((g_90 = l_89[0]) != (l_79 = ((0xDFF3 && (l_91 < l_92)) , (void*)0))), g_64.f1)), g_93));
    return g_95;
}

static int32_t * func_2(struct S0  p_3, int32_t * const  p_4, int64_t  p_5)
{ 
    int32_t *l_15 = (void*)0;
    int32_t *l_19 = &g_8;
    int32_t **l_18 = &l_19;
    int32_t l_20 = 0x198D72A6;
    int32_t *l_31 = &g_32;
    int32_t *l_44[4][3] = {{&g_8,&g_8,&l_20},{&g_8,&g_8,&l_20},{&g_8,&g_8,&l_20},{&g_8,&g_8,&l_20}};
    int i, j;
    if ((safe_div_func_int32_t_s_s(((*l_31) &= ((func_11(l_15, ((safe_lshift_func_uint64_t_u_s(p_3.f0, ((((*l_18) = l_15) == &g_8) > l_20))) & 5), g_6.f1) | g_6.f1) , (*p_4))), g_6.f1)))
    { 
        uint16_t l_43 = 0x5481;
        struct S0 *l_71 = &g_64;
        int32_t *l_73[1][1];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_73[i][j] = (void*)0;
        }
        (*l_18) = (*l_18);
        for (g_6.f0 = (-11); (g_6.f0 > 21); ++g_6.f0)
        { 
            uint64_t l_49 = 0x31AC6914F0912742LL;
            int32_t *l_72 = &g_8;
            l_44[1][2] = ((safe_div_func_int8_t_s_s((safe_lshift_func_int16_t_s_s((((safe_mod_func_int16_t_s_s(((p_3.f0 || ((safe_mod_func_int32_t_s_s((((p_5 & (0x2069 != p_5)) , 0xCC72) ^ (-8)), 0x25B78C6A)) ^ g_8)) < p_5), (-1))) >= p_3.f1) != (*p_4)), l_43)), g_8)) , p_4);
            if ((0x2B7283D3 ^ (((safe_add_func_int64_t_s_s(((g_32 == l_43) ^ (-5)), l_49)) == l_49) != l_43)))
            { 
                uint16_t *l_54[3];
                uint64_t *l_56 = &l_49;
                uint64_t *l_57 = (void*)0;
                uint64_t *l_58 = &g_59;
                int8_t *l_60 = &g_61;
                int32_t l_62 = 0x19F56C0C;
                int i;
                for (i = 0; i < 3; i++)
                    l_54[i] = &l_43;
                (*p_4) = (safe_lshift_func_uint32_t_u_s(((((*l_60) = (safe_unary_minus_func_uint32_t_u(((((*l_58) = (((*l_56) = ((((!(g_55 = p_5)) < 0x01) ^ p_3.f1) > p_3.f0)) || 3)) ^ 0xB27F9695FF7D1D41LL) ^ 3)))) >= g_6.f0) , l_62), (*p_4)));
            }
            else
            { 
                struct S0 *l_63[4][1];
                int i, j;
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_63[i][j] = &g_64;
                }
                p_3 = g_6;
            }
            if ((safe_mul_func_int8_t_s_s((safe_mod_func_int8_t_s_s(((g_59 = (l_71 != &p_3)) && (((*l_18) = l_72) == p_4)), p_3.f1)), 0)))
            { 
                return l_73[0][0];
            }
            else
            { 
                int32_t **l_74 = &l_15;
                int8_t **l_77 = &g_76;
                (*l_18) = ((*l_31) , &g_8);
                (*l_74) = p_4;
                l_77 = g_75;
            }
        }
    }
    else
    { 
        int32_t *l_78 = (void*)0;
        return l_78;
    }
    return &g_8;
}

static uint8_t  func_11(int32_t * p_12, int32_t  p_13, int8_t  p_14)
{ 
    uint64_t l_25 = 0xE2E4ED383DE467B4LL;
    int32_t *l_26 = &g_8;
    if ((safe_add_func_int32_t_s_s(0, (p_14 , ((safe_sub_func_int32_t_s_s(l_25, l_25)) != g_6.f0)))))
    { 
        int32_t **l_27 = (void*)0;
        int32_t **l_28 = &l_26;
        (*l_28) = l_26;
    }
    else
    { 
        int32_t **l_29[4] = {&l_26,&l_26,&l_26,&l_26};
        int i;
        (*l_26) ^= 0x3CEA2FF3;
        g_30[0][0] = &p_13;
        (*l_26) = ((*l_26) != 1U);
    }
    l_26 = l_26;
    return g_6.f0;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6.f0, "g_6.f0", print_hash_value);
    transparent_crc(g_6.f1, "g_6.f1", print_hash_value);
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_64.f0, "g_64.f0", print_hash_value);
    transparent_crc(g_64.f1, "g_64.f1", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    transparent_crc(g_95, "g_95", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
