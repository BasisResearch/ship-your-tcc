/* Csmith 0cdc710315cf seed 112, flattened by scripts/csmith_flatten.py */
// Options:   --seed 112 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
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
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint8_t  f0;
};

struct S1 {
   const int16_t  f0;
   int8_t  f1;
};

union U5 {
   uint8_t  f0;
};

static uint8_t g_13 = 8U;
static union U5 g_17 = {0xEF};
static union U5 *g_18 = (void*)0;
static int32_t g_20 = 0x7E80D68F;
static int8_t g_22 = (-3);
static uint8_t g_23 = 255U;
static const union U5 g_31 = {0x8B};
static const union U5 *g_30 = &g_31;
static int64_t g_33 = 1;
static uint64_t g_36 = 0xEC1E183A0997736ELL;
static int8_t g_37[4] = {0x18,0x18,0x18,0x18};
static int32_t g_39 = 1;
static int16_t g_40 = (-7);
static uint16_t g_41[2] = {0x1700,0x1700};
static int8_t g_87[4][3] = {{0x67,0x67,0x67},{0x98,0x98,0x98},{0x67,0x67,0x67},{0x98,0x98,0x98}};
static int16_t g_88 = (-1);
static int8_t g_91 = 0x91;
static int64_t g_92 = 0x3E82D738A40F9476LL;
static int32_t g_93[3][2] = {{0x120E7427,2},{2,0x120E7427},{2,2}};
static struct S0 g_103 = {253U};
static struct S0 *g_102 = &g_103;
static struct S0 g_110 = {0x8F};

static int8_t  func_1(void);
static int64_t  func_2(int16_t  p_3, uint8_t  p_4, union U5  p_5, struct S1  p_6);
static union U5  func_8(uint8_t  p_9);

static int8_t  func_1(void)
{ 
    int8_t l_7[4] = {(-9),(-9),(-9),(-9)};
    uint8_t *l_12 = &g_13;
    struct S1 l_44 = {1,0xF6};
    uint8_t l_54 = 0x59;
    int32_t l_90 = 0;
    int8_t l_99 = 0;
    int32_t l_121 = 0x739EBEB9;
    int32_t l_126 = 0x3B4235E9;
    int32_t l_130 = 0x211E8D53;
    int32_t l_131[4][1];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
            l_131[i][j] = (-6);
    }
    if ((func_2(l_7[3], l_7[2], func_8((safe_lshift_func_uint64_t_u_u(((--(*l_12)) != 0x81), l_7[3]))), l_44) < l_54))
    { 
        const int16_t l_58 = 1;
        struct S1 l_59[3] = {{0xE2CB,0x03},{0xE2CB,0x03},{0xE2CB,0x03}};
        int32_t l_80 = 0;
        int i;
        if ((safe_lshift_func_int16_t_s_s((safe_unary_minus_func_uint64_t_u((((g_36 | l_58) != (l_59[1] , l_44.f1)) < 1))), l_44.f1)))
        { 
            uint64_t l_73[3];
            int8_t *l_77 = (void*)0;
            int8_t *l_78 = &l_44.f1;
            int32_t *l_79 = &g_39;
            int32_t *l_81 = &g_20;
            int i;
            for (i = 0; i < 3; i++)
                l_73[i] = 18446744073709551615U;
lbl_76:
            for (g_33 = 3; (g_33 >= 0); g_33 -= 1)
            { 
                int32_t *l_66 = &g_39;
                (*l_66) &= (safe_div_func_uint8_t_u_u((safe_sub_func_uint32_t_u_u(4294967295U, (safe_sub_func_int32_t_s_s(g_40, g_20)))), g_41[1]));
                (*l_66) = (safe_add_func_uint8_t_u_u(((safe_rshift_func_uint64_t_u_s((safe_mul_func_int64_t_s_s(((l_73[1] == (safe_add_func_int64_t_s_s(g_37[0], (((g_39 == g_17.f0) < l_73[2]) >= g_37[3])))) | l_44.f0), l_73[0])), l_73[1])) != (*l_66)), 0x5A));
            }
            for (g_33 = 0; (g_33 <= 3); g_33 += 1)
            { 
                if (g_17.f0)
                    goto lbl_76;
            }
            (*l_81) = (l_80 = ((*l_79) = (((*l_78) = g_36) && 4)));
        }
        else
        { 
            int8_t *l_83 = &g_22;
            int32_t *l_84 = (void*)0;
            int32_t l_85 = 0x31143146;
            int32_t *l_86[4][4] = {{(void*)0,&l_80,(void*)0,&l_80},{(void*)0,&l_80,(void*)0,&l_80},{(void*)0,&l_80,(void*)0,&l_80},{(void*)0,&l_80,(void*)0,&l_80}};
            int16_t l_89 = 3;
            uint16_t l_94 = 0xBC66;
            int i, j;
            l_85 ^= ((+g_20) | ((*l_83) = g_41[1]));
            l_94++;
        }
    }
    else
    { 
        struct S0 *l_109 = &g_110;
        int32_t *l_111 = &g_93[1][0];
        int32_t *l_112 = &g_20;
        int32_t *l_113 = &g_39;
        int32_t *l_114 = &g_20;
        int32_t *l_115 = &g_93[0][0];
        int32_t *l_116 = &l_90;
        int32_t *l_117 = &g_20;
        int32_t *l_118 = &g_93[2][0];
        int32_t *l_119 = &g_93[2][0];
        int32_t *l_120 = &g_20;
        int32_t *l_122 = &g_20;
        int32_t *l_123 = (void*)0;
        int32_t *l_124 = (void*)0;
        int32_t *l_125 = &g_93[2][0];
        int32_t *l_127 = &l_90;
        int32_t *l_128 = &l_126;
        int32_t *l_129[3];
        uint64_t l_132 = 18446744073709551615U;
        const int32_t *l_136[1][2];
        const int32_t **l_135 = &l_136[0][1];
        int i, j;
        for (i = 0; i < 3; i++)
            l_129[i] = &g_20;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_136[i][j] = &l_131[0][0];
        }
        if (l_44.f0)
        { 
            return g_36;
        }
        else
        { 
            for (g_23 = 0; (g_23 < 4); g_23 = safe_add_func_int16_t_s_s(g_23, 1))
            { 
                return l_99;
            }
            for (l_44.f1 = 25; (l_44.f1 >= 22); l_44.f1 = safe_sub_func_int64_t_s_s(l_44.f1, 1))
            { 
                struct S0 **l_104 = (void*)0;
                struct S0 **l_105 = &g_102;
                struct S0 **l_106 = (void*)0;
                struct S0 *l_108 = &g_103;
                struct S0 **l_107[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_107[i] = &l_108;
                l_109 = ((*l_105) = g_102);
            }
        }
        --l_132;
        (*l_135) = (void*)0;
    }
    return g_88;
}

static int64_t  func_2(int16_t  p_3, uint8_t  p_4, union U5  p_5, struct S1  p_6)
{ 
    int32_t *l_49[2][2];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
            l_49[i][j] = &g_39;
    }
    for (g_23 = (-1); (g_23 <= 18); g_23 = safe_add_func_uint8_t_u_u(g_23, 3))
    { 
        int32_t *l_48 = &g_20;
        int32_t **l_47[2][4] = {{(void*)0,&l_48,&l_48,(void*)0},{&l_48,(void*)0,&l_48,&l_48}};
        int i, j;
        l_49[0][1] = &g_39;
        for (g_39 = 0; (g_39 <= 1); g_39 += 1)
        { 
            union U5 **l_50 = (void*)0;
            union U5 **l_51 = &g_18;
            (*l_51) = &p_5;
            (*l_48) = (safe_mod_func_uint32_t_u_u((9U || (-7)), p_6.f0));
            for (p_6.f1 = 3; (p_6.f1 >= 0); p_6.f1 -= 1)
            { 
                int i;
                return g_37[g_39];
            }
        }
    }
    return g_20;
}

static union U5  func_8(uint8_t  p_9)
{ 
    union U5 *l_16 = &g_17;
    int32_t *l_19 = &g_20;
    int32_t *l_21[2];
    int i;
    for (i = 0; i < 2; i++)
        l_21[i] = &g_20;
    g_18 = l_16;
    --g_23;
    for (g_20 = 0; (g_20 == 27); g_20++)
    { 
        const union U5 *l_28[4][2] = {{&g_17,&g_17},{&g_17,&g_17},{&g_17,&g_17},{&g_17,&g_17}};
        int32_t l_38 = 0x7D900D29;
        int i, j;
        for (g_13 = 0; (g_13 <= 1); g_13 += 1)
        { 
            const union U5 **l_29[4];
            int i;
            for (i = 0; i < 4; i++)
                l_29[i] = &l_28[0][0];
            g_33 = (((((g_30 = l_28[0][0]) == l_16) <= (safe_unary_minus_func_uint64_t_u((l_16 == (void*)0)))) ^ g_31.f0) && p_9);
            for (g_23 = 0; (g_23 <= 1); g_23 += 1)
            { 
                union U5 **l_34 = (void*)0;
                int32_t l_35 = (-3);
                g_36 = (l_35 |= ((void*)0 == l_34));
                return (*g_30);
            }
        }
        ++g_41[1];
        for (g_40 = 1; (g_40 >= 0); g_40 -= 1)
        { 
            return (*g_30);
        }
    }
    return (*l_16);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_17.f0, "g_17.f0", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_31.f0, "g_31.f0", print_hash_value);
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_36, "g_36", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_37[i], "g_37[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_41[i], "g_41[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_87[i][j], "g_87[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_88, "g_88", print_hash_value);
    transparent_crc(g_91, "g_91", print_hash_value);
    transparent_crc(g_92, "g_92", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_93[i][j], "g_93[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_103.f0, "g_103.f0", print_hash_value);
    transparent_crc(g_110.f0, "g_110.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
