/* Csmith 0cdc710315cf seed 190, flattened by scripts/csmith_flatten.py */
// Options:   --seed 190 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
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
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
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
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   const int16_t  f1;
   int8_t  f2;
   uint32_t  f3;
};

union U1 {
   const int16_t  f0;
   uint8_t  f1;
   uint16_t  f2;
};

static int32_t g_2[2][2] = {{0x11738AB6,0x11738AB6},{0x11738AB6,0x11738AB6}};
static int32_t g_6 = 0;
static uint32_t g_12 = 6U;
static int32_t g_29[2] = {0x7BA3F300,0x7BA3F300};
static struct S0 g_30 = {-1,4,0x09,2U};
static int16_t g_63 = 0;
static int32_t g_77 = 0;
static int32_t *g_76 = &g_77;
static int32_t g_89 = 1;
static uint64_t g_104 = 0x1C42294EC24AAD4DLL;
static union U1 g_114 = {0xC04B};
static union U1 * const g_113 = &g_114;
static uint16_t g_161 = 3U;

static uint16_t  func_1(void);
static int32_t * func_15(const union U1  p_16, uint8_t  p_17, int32_t * p_18, int8_t  p_19);
static int32_t * func_22(const int32_t * p_23, uint8_t  p_24, uint16_t  p_25, int32_t  p_26, int32_t * p_27);

static uint16_t  func_1(void)
{ 
    int16_t l_11 = 0xFFAF;
    int32_t *l_148 = &g_29[0];
    union U1 *l_159 = &g_114;
    uint16_t *l_160[2][4];
    int32_t *l_162 = &g_89;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
            l_160[i][j] = &g_161;
    }
    for (g_2[1][0] = (-29); (g_2[1][0] < 8); g_2[1][0] = safe_add_func_int64_t_s_s(g_2[1][0], 7))
    { 
        int32_t *l_5 = &g_6;
        int32_t *l_7 = (void*)0;
        int32_t *l_8 = &g_6;
        int32_t *l_9 = (void*)0;
        int32_t *l_10[4] = {&g_6,&g_6,&g_6,&g_6};
        const union U1 l_20 = {0};
        int i;
        ++g_12;
        for (l_11 = 0; (l_11 <= 3); l_11 += 1)
        { 
            int32_t l_21 = (-1);
            uint8_t l_28 = 0x4F;
            for (g_12 = 0; (g_12 <= 3); g_12 += 1)
            { 
                int32_t **l_147[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_147[i] = &l_5;
                l_148 = func_15(l_20, l_21, (l_10[g_12] = func_22(l_10[g_12], g_2[1][0], l_28, l_21, &g_2[0][0])), g_30.f1);
            }
        }
    }
    (*g_76) = (safe_lshift_func_uint64_t_u_u(g_6, 39));
    (*l_162) &= ((safe_mod_func_int32_t_s_s((safe_add_func_uint64_t_u_u((safe_add_func_int64_t_s_s(0x7CF680477419847BLL, g_114.f0)), (((safe_mod_func_uint16_t_u_u((g_161 |= (((l_159 == (void*)0) > g_104) == g_29[0])), (*l_148))) , 7U) > (*l_148)))), g_30.f3)) & 5);
    return g_30.f2;
}

static int32_t * func_15(const union U1  p_16, uint8_t  p_17, int32_t * p_18, int8_t  p_19)
{ 
    uint16_t l_96 = 0xB39D;
    uint64_t *l_103 = &g_104;
    int32_t *l_105 = &g_6;
    int32_t l_107[3];
    uint32_t *l_142 = &g_30.f3;
    int i;
    for (i = 0; i < 3; i++)
        l_107[i] = 0x10E39645;
    if (((((safe_sub_func_int16_t_s_s(((safe_lshift_func_uint16_t_u_u(l_96, 8)) <= (0x9EF2 <= (safe_add_func_uint64_t_u_u(((safe_lshift_func_int32_t_s_s((safe_sub_func_uint64_t_u_u(((*l_103) = p_17), g_30.f0)), 30)) <= p_17), 18446744073709551611U)))), p_17)) <= g_29[1]) || 0U) , l_96))
    { 
        int32_t **l_106[3][1];
        uint32_t l_108[3];
        union U1 *l_127[4][2];
        union U1 **l_126 = &l_127[0][0];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_106[i][j] = &l_105;
        }
        for (i = 0; i < 3; i++)
            l_108[i] = 0U;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 2; j++)
                l_127[i][j] = (void*)0;
        }
        g_76 = l_105;
        --l_108[0];
        for (g_6 = 16; (g_6 >= 3); g_6--)
        { 
            int64_t l_118 = 0x3918EFA3C6A65346LL;
            if ((*p_18))
            { 
                union U1 **l_115 = (void*)0;
                union U1 *l_117 = &g_114;
                union U1 **l_116 = &l_117;
                (*l_116) = g_113;
            }
            else
            { 
                union U1 *l_122 = (void*)0;
                union U1 ** const l_121 = &l_122;
                l_118 = (*g_76);
                g_29[0] &= (safe_mul_func_int32_t_s_s((l_121 == (((safe_mul_func_uint64_t_u_u((+(&g_63 == (void*)0)), 0x6C07334A22360892LL)) != 8U) , l_126)), 3U));
                l_107[2] = (-9);
            }
        }
    }
    else
    { 
        const uint64_t *l_134 = &g_104;
        for (g_104 = (-7); (g_104 <= 47); ++g_104)
        { 
            int8_t l_136 = 1;
            int32_t *l_137 = &g_89;
            (*l_137) |= (safe_sub_func_uint32_t_u_u((((safe_div_func_uint32_t_u_u(((l_134 == l_134) || (~(((l_136 != l_136) & 0x7E04) && (*g_76)))), 6U)) != g_2[1][0]) , p_17), (*p_18)));
            if ((*p_18))
                break;
        }
    }
    (*p_18) = (p_16.f0 == (safe_add_func_int8_t_s_s(p_19, g_30.f2)));
    (*l_105) = (g_29[1] , (safe_sub_func_uint8_t_u_u((((++(*l_142)) || (l_107[1] ^= (1 > (safe_mul_func_int32_t_s_s(((*g_113) , (*g_76)), p_17))))) >= p_16.f0), g_104)));
    return &g_2[1][0];
}

static int32_t * func_22(const int32_t * p_23, uint8_t  p_24, uint16_t  p_25, int32_t  p_26, int32_t * p_27)
{ 
    const int32_t *l_31 = &g_29[0];
    int32_t l_44 = (-1);
    uint32_t l_45 = 0xB9FE29F1;
    int16_t l_54 = 0x4A3C;
lbl_78:
    for (p_26 = 1; (p_26 >= 0); p_26 -= 1)
    { 
        int16_t *l_58[3][3] = {{&l_54,&l_54,&l_54},{&l_54,(void*)0,&l_54},{&l_54,&l_54,&l_54}};
        int i, j;
        if ((((g_30 , &g_29[p_26]) == l_31) & p_25))
        { 
            uint8_t l_32[3];
            int32_t *l_33 = &g_29[0];
            int32_t *l_34 = (void*)0;
            int32_t *l_35 = &g_29[1];
            int32_t *l_36 = &g_29[p_26];
            int32_t *l_37 = &g_29[p_26];
            int32_t *l_38 = &g_29[p_26];
            int32_t *l_39 = &g_29[p_26];
            int32_t *l_40 = &g_29[p_26];
            int32_t *l_41 = &g_6;
            int32_t *l_42 = &g_6;
            int32_t *l_43[3][3] = {{&g_29[0],&g_29[0],&g_6},{&g_29[0],&g_29[0],&g_6},{&g_29[0],&g_29[0],&g_6}};
            int i, j;
            for (i = 0; i < 3; i++)
                l_32[i] = 252U;
            for (g_30.f0 = 1; (g_30.f0 >= 0); g_30.f0 -= 1)
            { 
                if (l_32[2])
                    break;
            }
            l_45--;
            if (g_30.f0)
            { 
                int16_t l_52 = 0x8378;
                int16_t *l_53 = &l_52;
                (*l_40) = (9U > ((safe_add_func_int8_t_s_s((safe_div_func_int8_t_s_s((((*l_53) = l_52) || ((((((0 & l_54) >= 4294967289U) , g_30.f3) != 7) == (*l_41)) && 0x285828E8)), p_24)), 0xB7)) || (-10)));
                return &g_6;
            }
            else
            { 
                int16_t *l_65 = &l_54;
                int16_t **l_64 = &l_65;
                if ((*l_31))
                    break;
                g_6 &= (safe_mod_func_int64_t_s_s(p_26, ((!(l_58[1][0] != ((*l_64) = ((safe_sub_func_int64_t_s_s((safe_sub_func_uint8_t_u_u((g_63 = g_29[p_26]), (-1))), p_24)) , (void*)0)))) , (*l_31))));
            }
        }
        else
        { 
            int32_t l_68 = 0x1CDDC186;
            for (g_30.f0 = (-30); (g_30.f0 < 6); g_30.f0 = safe_add_func_uint8_t_u_u(g_30.f0, 1))
            { 
                int32_t *l_71 = &l_44;
                if (l_68)
                    break;
                g_6 ^= ((safe_lshift_func_int32_t_s_u(((((*l_71) = g_30.f1) || ((((*l_71) < ((((safe_sub_func_int8_t_s_s(g_30.f0, 0x65)) , (void*)0) == (void*)0) , g_30.f0)) >= (-1)) >= (*l_71))) & 0x75BC6281B62C3DCCLL), g_63)) < 18446744073709551615U);
            }
        }
    }
    for (g_6 = 0; (g_6 != (-21)); --g_6)
    { 
        for (l_45 = 0; (l_45 <= 1); l_45 += 1)
        { 
            int i;
            g_76 = &g_29[l_45];
            g_29[l_45] = 0x341B0B2A;
            if (g_30.f0)
                goto lbl_78;
        }
        for (l_44 = 0; (l_44 <= 1); l_44 += 1)
        { 
            int32_t *l_88[4][1] = {{&g_89},{&g_29[l_44]},{&g_89},{&g_29[l_44]}};
            int i, j;
            g_89 &= (safe_add_func_uint8_t_u_u(g_29[l_44], (!((safe_rshift_func_uint8_t_u_s((safe_sub_func_int16_t_s_s(p_26, (safe_mul_func_uint8_t_u_u(p_26, 0x17)))), p_24)) >= (*g_76)))));
            return &g_29[1];
        }
    }
    return &g_6;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_2[i][j], "g_2[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_29[i], "g_29[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_30.f0, "g_30.f0", print_hash_value);
    transparent_crc(g_30.f1, "g_30.f1", print_hash_value);
    transparent_crc(g_30.f2, "g_30.f2", print_hash_value);
    transparent_crc(g_30.f3, "g_30.f3", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    transparent_crc(g_104, "g_104", print_hash_value);
    transparent_crc(g_114.f0, "g_114.f0", print_hash_value);
    transparent_crc(g_161, "g_161", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
