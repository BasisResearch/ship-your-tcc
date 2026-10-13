/* Csmith 0cdc710315cf seed 44, flattened by scripts/csmith_flatten.py */
// Options:   --seed 44 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int8_t  f0;
   uint64_t  f1;
   int32_t  f2;
};

struct S1 {
   const uint64_t  f0;
   struct S0  f1;
   const int16_t  f2;
   int64_t  f3;
};

union U2 {
   uint64_t  f0;
   int64_t  f1;
   uint8_t  f2;
};

union U3 {
   uint8_t  f0;
};

static int32_t g_2 = 0x6580849F;
static union U2 g_10 = {0xAFE8B0D193E0566ALL};
static uint16_t g_15[4][2] = {{0xAD81,0xAD81},{0xAD81,0xAD81},{0xAD81,0xAD81},{0xAD81,0xAD81}};
static int32_t g_23 = 0;
static int32_t g_32 = 3;
static int8_t g_36[3] = {0x7D,0x7D,0x7D};
static uint32_t g_45 = 3U;
static uint32_t *g_44 = &g_45;
static int32_t g_54 = 1;
static uint64_t g_55 = 6U;
static int32_t *g_63 = &g_23;
static union U3 g_64 = {0x60};
static struct S1 g_93 = {0xF6B910F8796DE133LL,{1,0U,3},0xC55C,-9};
static struct S0 g_100 = {7,0U,0xD2E80B3E};
static struct S0 *g_99 = &g_100;
static struct S0 *g_102 = &g_100;

static int32_t  func_1(void);
static struct S0  func_5(union U2  p_6, int16_t  p_7, const uint8_t  p_8, uint8_t  p_9);
static union U3  func_11(union U2  p_12, struct S0  p_13);

static int32_t  func_1(void)
{ 
    const int16_t l_105 = 0;
    uint8_t l_116 = 1U;
    struct S1 *l_118 = &g_93;
    struct S1 **l_117 = &l_118;
    for (g_2 = 0; (g_2 <= (-18)); g_2 = safe_sub_func_int8_t_s_s(g_2, 1))
    { 
        struct S0 l_14 = {0x21,0x124DC41FD9D08905LL,-6};
        int32_t *l_110[1];
        int64_t *l_111 = &g_10.f1;
        struct S1 **l_119 = &l_118;
        int i;
        for (i = 0; i < 1; i++)
            l_110[i] = &g_54;
        g_93.f1 = func_5(g_10, (func_11(g_10, l_14) , g_15[2][0]), l_14.f2, g_36[1]);
        for (g_23 = 0; (g_23 != (-29)); g_23--)
        { 
            for (l_14.f1 = (-17); (l_14.f1 == 12); l_14.f1++)
            { 
                struct S0 **l_101 = &g_99;
                g_102 = ((*l_101) = g_99);
            }
        }
        if ((safe_mul_func_uint32_t_u_u((l_105 > (((*l_111) = (safe_rshift_func_int32_t_s_u((l_105 & ((g_54 ^= ((*g_63) = ((safe_mul_func_uint32_t_u_u(0x3B5E53D9, l_105)) != g_93.f1.f1))) >= (*g_44))), 11))) > g_64.f0)), (*g_44))))
        { 
            int32_t **l_112 = (void*)0;
            int32_t **l_113 = (void*)0;
            int32_t **l_114 = (void*)0;
            int32_t **l_115 = &l_110[0];
            (*l_115) = &g_54;
            (*g_63) &= l_116;
        }
        else
        { 
            int32_t *l_120 = &g_32;
            int32_t **l_121 = &l_110[0];
            l_119 = l_117;
            if ((*g_63))
                continue;
            (*l_121) = (g_93 , l_120);
        }
    }
    (*g_102) = (*g_102);
    (*g_63) ^= (safe_rshift_func_int8_t_s_s((-7), 1));
    return l_105;
}

static struct S0  func_5(union U2  p_6, int16_t  p_7, const uint8_t  p_8, uint8_t  p_9)
{ 
    uint32_t *l_65 = &g_45;
    int32_t l_66 = 1;
    uint16_t *l_79 = &g_15[0][0];
    int64_t *l_80[1];
    struct S1 *l_92[2];
    int i;
    for (i = 0; i < 1; i++)
        l_80[i] = &g_10.f1;
    for (i = 0; i < 2; i++)
        l_92[i] = &g_93;
    l_66 ^= ((void*)0 == l_65);
    (*g_63) &= (safe_lshift_func_uint16_t_u_u((safe_lshift_func_int64_t_s_s(((safe_mul_func_int64_t_s_s((l_66 & (safe_add_func_int8_t_s_s((safe_mul_func_int64_t_s_s((l_66 || (p_6.f1 = (safe_add_func_uint16_t_u_u(((*l_79) &= l_66), l_66)))), l_66)), (-1)))), (-1))) || p_7), g_36[1])), 2));
    for (g_23 = 0; (g_23 == (-3)); g_23--)
    { 
        const int32_t *l_90 = (void*)0;
        const int32_t **l_89 = &l_90;
        int32_t *l_91 = &g_32;
        struct S1 **l_94 = &l_92[1];
        (*l_91) ^= (safe_mod_func_int8_t_s_s(((safe_add_func_uint64_t_u_u((safe_mul_func_int64_t_s_s((l_89 != (p_9 , &g_63)), g_54)), p_8)) <= l_66), 1));
        (*l_94) = l_92[1];
    }
    return g_93.f1;
}

static union U3  func_11(union U2  p_12, struct S0  p_13)
{ 
    uint64_t l_29 = 18446744073709551607U;
    for (p_13.f0 = 0; (p_13.f0 <= 1); p_13.f0 += 1)
    { 
        uint32_t l_21 = 0U;
        union U3 l_24 = {0x81};
        const int32_t *l_25 = &g_23;
        int32_t l_28 = 0x2CD8491F;
        for (p_13.f1 = 0; (p_13.f1 <= 1); p_13.f1 += 1)
        { 
            union U3 l_20 = {0U};
            int i, j;
            if (g_15[(p_13.f0 + 1)][p_13.f1])
            { 
                union U2 l_16 = {1U};
                int32_t *l_22 = &g_23;
                int i, j;
                (*l_22) &= ((l_16 , ((g_15[(p_13.f0 + 2)][p_13.f0] || (+(((safe_div_func_int16_t_s_s(g_15[(p_13.f1 + 2)][p_13.f0], (-1))) , l_20) , g_15[(p_13.f1 + 1)][p_13.f1]))) , 65535U)) != l_21);
            }
            else
            { 
                return l_24;
            }
        }
        l_28 = (p_13.f2 = (l_25 == ((safe_add_func_uint32_t_u_u(0x39C01058, g_2)) , (void*)0)));
    }
    if (l_29)
    { 
        int32_t *l_30 = &g_23;
        int32_t *l_31 = &g_32;
        (*l_31) ^= (l_29 , ((*l_30) &= l_29));
    }
    else
    { 
        uint32_t *l_35[1];
        int32_t l_37[1];
        uint64_t l_38[2][2] = {{3U,3U},{3U,3U}};
        int32_t *l_41 = &g_32;
        union U2 l_43 = {4U};
        uint32_t **l_46 = &g_44;
        int32_t **l_58[2][4] = {{&l_41,&l_41,&l_41,&l_41},{&l_41,&l_41,&l_41,&l_41}};
        int i, j;
        for (i = 0; i < 1; i++)
            l_35[i] = (void*)0;
        for (i = 0; i < 1; i++)
            l_37[i] = 0x455DBFE6;
        (*l_41) ^= (((safe_mul_func_int32_t_s_s((-3), (++l_38[1][0]))) & (p_13.f1 < (0xBC || l_29))) ^ 65535U);
        if ((~(l_35[0] != ((*l_46) = (l_43 , g_44)))))
        { 
            for (p_12.f0 = 0; p_12.f0 < 1; p_12.f0 = p_12.f0 + 1)
            {
                l_35[p_12.f0] = &g_45;
            }
        }
        else
        { 
            int32_t l_51 = 0x42075D7B;
            int32_t *l_52 = &l_37[0];
            int32_t *l_53[1][3];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 3; j++)
                    l_53[i][j] = &g_54;
            }
            g_55 ^= ((*l_52) |= ((((safe_mod_func_int64_t_s_s(((*l_41) &= ((0U != (!(safe_unary_minus_func_uint8_t_u(255U)))) != p_13.f1)), l_51)) ^ p_13.f0) >= g_23) || p_13.f1));
            for (g_23 = 0; (g_23 == 3); g_23 = safe_add_func_int32_t_s_s(g_23, 7))
            { 
                int16_t l_62 = 0xFFBB;
                l_58[0][0] = l_58[0][0];
                (*l_52) |= ((*l_41) = (safe_add_func_int32_t_s_s((+l_62), p_13.f2)));
            }
            for (l_43.f0 = 0; (l_43.f0 <= 1); l_43.f0 += 1)
            { 
                g_63 = &g_23;
                if ((*g_63))
                    break;
            }
        }
    }
    return g_64;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_15[i][j], "g_15[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_36[i], "g_36[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_54, "g_54", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_64.f0, "g_64.f0", print_hash_value);
    transparent_crc(g_93.f0, "g_93.f0", print_hash_value);
    transparent_crc(g_93.f1.f0, "g_93.f1.f0", print_hash_value);
    transparent_crc(g_93.f1.f1, "g_93.f1.f1", print_hash_value);
    transparent_crc(g_93.f1.f2, "g_93.f1.f2", print_hash_value);
    transparent_crc(g_93.f2, "g_93.f2", print_hash_value);
    transparent_crc(g_93.f3, "g_93.f3", print_hash_value);
    transparent_crc(g_100.f0, "g_100.f0", print_hash_value);
    transparent_crc(g_100.f1, "g_100.f1", print_hash_value);
    transparent_crc(g_100.f2, "g_100.f2", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
