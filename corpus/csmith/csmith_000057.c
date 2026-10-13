/* Csmith 0cdc710315cf seed 57, flattened by scripts/csmith_flatten.py */
// Options:   --seed 57 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
   int32_t  f0;
   int16_t  f1;
};

struct S1 {
   int64_t  f0;
   uint64_t  f1;
   uint8_t  f2;
};

union U2 {
   int32_t  f0;
};

union U3 {
   uint16_t  f0;
};

static union U2 g_5 = {0x2E5664DC};
static int32_t *g_19[3] = {(void*)0,(void*)0,(void*)0};
static int16_t g_23 = 0x752B;
static int64_t g_26 = 2;
static struct S1 g_39 = {0xD5D742CC3231A295LL,0xE3164C116104DBC0LL,0U};
static const struct S1 *g_45 = (void*)0;
static struct S0 g_60 = {-10,-10};
static int16_t g_69 = (-1);
static const union U2 *g_89 = &g_5;
static const union U2 **g_88 = &g_89;
static int32_t g_125 = 0x1A2B5784;
static union U3 g_129 = {0xF44F};

static union U3  func_1(void);
static int32_t * func_2(union U2  p_3, int32_t * p_4);
static int32_t * func_6(int32_t * p_7, int32_t * p_8, int8_t  p_9, int16_t  p_10, int32_t * p_11);

static union U3  func_1(void)
{ 
    int32_t *l_12 = (void*)0;
    int32_t l_13[1];
    int32_t **l_127 = &l_12;
    int32_t **l_128 = &g_19[2];
    int i;
    for (i = 0; i < 1; i++)
        l_13[i] = 0x7D8CF4C5;
    (*l_128) = ((*l_127) = func_2(g_5, func_6(l_12, l_12, l_13[0], g_5.f0, &l_13[0])));
    return g_129;
}

static int32_t * func_2(union U2  p_3, int32_t * p_4)
{ 
    const int16_t l_81 = 0;
    union U2 l_85 = {4};
    union U2 *l_87 = &l_85;
    union U2 **l_86 = &l_87;
    int32_t l_126 = 8;
    for (g_39.f0 = 17; (g_39.f0 >= (-21)); g_39.f0--)
    { 
        union U3 l_82 = {6U};
        int32_t l_90 = 0;
        int32_t *l_96 = &l_90;
        for (g_60.f1 = 0; (g_60.f1 > 27); g_60.f1 = safe_add_func_uint32_t_u_u(g_60.f1, 4))
        { 
            if (l_81)
                break;
            p_3.f0 = ((l_82 , (9U >= p_3.f0)) & p_3.f0);
            l_90 ^= (((safe_div_func_uint16_t_u_u((((l_85 , g_5) , (((l_86 == g_88) <= 2) | g_69)) && g_69), 65526U)) >= g_5.f0) == 0xD733);
        }
        p_3.f0 = (((1U && ((l_85.f0 & (~((safe_div_func_int64_t_s_s((g_26 & p_3.f0), 3)) >= g_26))) && 0x2B)) && (-10)) , l_85.f0);
        (*l_96) = ((((safe_div_func_uint16_t_u_u((g_5.f0 == ((p_3.f0 | p_3.f0) && l_82.f0)), 0xED7D)) | g_60.f1) <= g_39.f1) | 0x16214FB9B3D0AF17LL);
    }
    for (p_3.f0 = 0; (p_3.f0 <= 2); p_3.f0 += 1)
    { 
        uint64_t l_121 = 1U;
        uint16_t l_122 = 65535U;
        for (g_60.f0 = 0; (g_60.f0 <= 2); g_60.f0 += 1)
        { 
            int32_t *l_102 = (void*)0;
            int32_t *l_103 = &l_85.f0;
            int64_t *l_107 = &g_39.f0;
            int64_t *l_108 = &g_26;
            int32_t *l_123 = (void*)0;
            int32_t *l_124[2][3] = {{&g_125,&g_125,&g_125},{&g_125,&g_125,&g_125}};
            int i, j;
            (*l_103) &= (!(p_3.f0 ^ (safe_lshift_func_uint16_t_u_u((safe_mod_func_int32_t_s_s(0xB4319BF6, p_3.f0)), 1))));
            l_126 |= ((((*l_108) = ((*l_107) = (safe_lshift_func_int8_t_s_s((safe_unary_minus_func_int64_t_s(g_5.f0)), 5)))) >= (safe_div_func_int16_t_s_s((safe_rshift_func_uint32_t_u_s(((safe_lshift_func_int64_t_s_s((safe_add_func_int16_t_s_s(((((safe_add_func_uint32_t_u_u((safe_rshift_func_int8_t_s_u(l_121, (l_121 < l_122))), 0xE5B14AB0)) , g_5.f0) > g_39.f2) | p_3.f0), p_3.f0)), 32)) , l_122), l_121)), (*l_103)))) <= (-10));
        }
    }
    return &g_125;
}

static int32_t * func_6(int32_t * p_7, int32_t * p_8, int8_t  p_9, int16_t  p_10, int32_t * p_11)
{ 
    uint64_t l_21[4] = {0xE9F9034E1054D814LL,0xE9F9034E1054D814LL,0xE9F9034E1054D814LL,0xE9F9034E1054D814LL};
    int32_t l_24 = 0xFEFB659B;
    int8_t l_27 = 0x1D;
    const int16_t * const l_36 = &g_23;
    const struct S1 *l_43 = &g_39;
    int16_t *l_53[4] = {&g_23,&g_23,&g_23,&g_23};
    uint16_t l_58 = 0xDE22;
    int i;
    for (p_9 = (-30); (p_9 >= (-13)); p_9 = safe_add_func_int32_t_s_s(p_9, 3))
    { 
        int32_t **l_20[4];
        int16_t *l_22[4][2];
        int64_t *l_25 = &g_26;
        struct S1 *l_38 = &g_39;
        struct S0 l_41 = {0,1};
        const struct S1 **l_44[4];
        int i, j;
        for (i = 0; i < 4; i++)
            l_20[i] = &g_19[1];
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 2; j++)
                l_22[i][j] = &g_23;
        }
        for (i = 0; i < 4; i++)
            l_44[i] = &l_43;
        if ((((safe_div_func_int16_t_s_s(0xCCB9, ((+((*l_25) = ((0xFF || ((l_24 = (((((p_7 = g_19[1]) != (void*)0) || l_21[2]) , 0x8EF8A6EB) ^ 0xD0D7276C)) > 1)) && g_23))) , g_26))) <= l_21[2]) || g_5.f0))
        { 
            if (l_27)
                break;
            for (p_10 = 0; (p_10 == 21); p_10 = safe_add_func_uint8_t_u_u(p_10, 4))
            { 
                return p_8;
            }
        }
        else
        { 
            struct S1 l_32 = {0x1DDBC75B167D3D8ELL,0xCE43C7D757520C25LL,0xFA};
            int32_t l_40[1];
            int i;
            for (i = 0; i < 1; i++)
                l_40[i] = 0;
            for (l_24 = 0; (l_24 <= 2); l_24 = safe_add_func_int64_t_s_s(l_24, 3))
            { 
                struct S1 *l_33 = &l_32;
                (*l_33) = l_32;
            }
            for (l_32.f0 = 0; (l_32.f0 >= 16); l_32.f0 = safe_add_func_uint8_t_u_u(l_32.f0, 8))
            { 
                struct S1 *l_37 = &l_32;
                struct S0 *l_42 = &l_41;
                l_40[0] &= ((*p_11) = (((l_36 == &p_10) , l_37) == l_38));
                if (l_27)
                    break;
                (*l_42) = l_41;
            }
        }
        if ((*p_11))
            continue;
        g_45 = l_43;
    }
    for (l_24 = 0; (l_24 > (-7)); --l_24)
    { 
        int16_t *l_50 = &g_23;
        int16_t *l_52 = (void*)0;
        int16_t **l_51[2][2] = {{&l_52,&l_52},{&l_52,&l_52}};
        union U2 *l_56 = &g_5;
        int32_t l_76 = 0;
        int i, j;
        if (((p_9 == ((l_53[3] = (l_50 = &p_10)) != (void*)0)) > l_21[2]))
        { 
            uint16_t l_73 = 0x72FE;
            for (g_39.f1 = 25; (g_39.f1 < 7); g_39.f1 = safe_sub_func_uint64_t_u_u(g_39.f1, 7))
            { 
                union U2 **l_57 = &l_56;
                struct S0 *l_59 = &g_60;
                struct S0 **l_61 = &l_59;
                (*l_57) = l_56;
                (*p_11) &= (((**l_57) , ((void*)0 != &g_5)) , l_58);
                (*l_61) = l_59;
            }
            for (p_9 = (-10); (p_9 >= 9); p_9++)
            { 
                int32_t **l_66 = &g_19[1];
                (*p_11) &= ((65534U != ((safe_mod_func_uint32_t_u_u(g_39.f2, 1U)) | p_10)) != 0);
                (*l_66) = g_19[0];
            }
            for (p_10 = 0; (p_10 == (-30)); --p_10)
            { 
                int32_t *l_70 = &g_60.f0;
                int32_t *l_71 = (void*)0;
                int32_t *l_72[1][2];
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_72[i][j] = &g_60.f0;
                }
                --l_73;
                return p_11;
            }
        }
        else
        { 
            if (l_76)
                break;
        }
    }
    return g_19[2];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5.f0, "g_5.f0", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_39.f0, "g_39.f0", print_hash_value);
    transparent_crc(g_39.f1, "g_39.f1", print_hash_value);
    transparent_crc(g_39.f2, "g_39.f2", print_hash_value);
    transparent_crc(g_60.f0, "g_60.f0", print_hash_value);
    transparent_crc(g_60.f1, "g_60.f1", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_125, "g_125", print_hash_value);
    transparent_crc(g_129.f0, "g_129.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
