/* Csmith 0cdc710315cf seed 45, flattened by scripts/csmith_flatten.py */
// Options:   --seed 45 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
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

union U0 {
   uint16_t  f0;
   int16_t  f1;
   int8_t  f2;
};

static int32_t g_3 = 0xF34FDA0E;
static uint8_t g_5 = 0x6E;
static int8_t g_22[2][4] = {{(-6),(-6),(-6),(-6)},{(-6),(-6),(-6),(-6)}};
static union U0 g_23 = {0xE13C};
static int32_t *g_32 = (void*)0;
static uint16_t g_72 = 0xEECC;
static uint16_t * const g_71[2][3] = {{(void*)0,(void*)0,(void*)0},{&g_72,&g_72,&g_72}};
static int64_t g_76 = (-1);
static int32_t g_81 = 0xED293A85;
static union U0 *g_86 = &g_23;
static int8_t g_98 = (-2);
static int64_t g_99 = 0x77E0797F44D61F69LL;
static int16_t g_100 = 0x5243;
static uint32_t g_101 = 0x3DDFAC93;
static int64_t g_115 = 0x04C7A4504AE8C7AELL;
static uint64_t g_116[2][1] = {{0x2EA708A12FBA8185LL},{0x2EA708A12FBA8185LL}};
static int32_t g_121 = (-7);
static int32_t *g_120 = &g_121;
static int32_t *g_122 = &g_121;
static uint16_t g_142[4] = {0xB2A5,0xB2A5,0xB2A5,0xB2A5};

static int8_t  func_1(void);
static int32_t * func_8(int32_t * p_9, int32_t * p_10, int32_t * p_11);
static int32_t * func_12(union U0  p_13, union U0  p_14, union U0  p_15);

static int8_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t *l_4[3][4] = {{&g_3,&g_3,&g_3,&g_3},{&g_3,&g_3,&g_3,&g_3},{&g_3,&g_3,&g_3,&g_3}};
    union U0 l_24 = {0x4E58};
    union U0 l_25 = {0xC39C};
    int32_t **l_140 = &g_120;
    uint64_t l_141 = 0x40A721C5E8018D86LL;
    uint64_t *l_151 = &g_116[1][0];
    uint8_t *l_152[4];
    int32_t **l_153 = &l_2;
    int i, j;
    for (i = 0; i < 4; i++)
        l_152[i] = &g_5;
    g_5--;
    g_142[2] |= ((&g_3 != ((*l_140) = func_8(func_12(((safe_mul_func_uint32_t_u_u(g_3, (g_22[0][0] = ((safe_rshift_func_int64_t_s_u(((safe_lshift_func_int16_t_s_u((g_5 <= 0U), 4)) && (*l_2)), 37)) & g_3)))) , g_23), l_24, l_25), g_120, g_122))) < l_141);
    (*l_153) = ((*l_140) = ((((**l_140) != (g_5 = ((safe_div_func_uint16_t_u_u(((((safe_div_func_uint32_t_u_u((((*l_151) = g_100) ^ ((g_23.f1 = (-3)) != g_100)), 0x3D79CAFD)) || 0U) == (*l_2)) , (*l_2)), g_98)) ^ g_115))) || 0xE6) , &g_121));
    return g_22[0][1];
}

static int32_t * func_8(int32_t * p_9, int32_t * p_10, int32_t * p_11)
{ 
    uint64_t l_133[4][3] = {{7U,0x6B3A0AE147BCB459LL,0x6B3A0AE147BCB459LL},{7U,0x6B3A0AE147BCB459LL,0x6B3A0AE147BCB459LL},{7U,0x6B3A0AE147BCB459LL,0x6B3A0AE147BCB459LL},{7U,0x6B3A0AE147BCB459LL,0x6B3A0AE147BCB459LL}};
    int32_t l_137 = (-1);
    int i, j;
    for (g_121 = 0; (g_121 >= 0); g_121 -= 1)
    { 
        uint16_t *l_125 = &g_23.f0;
        int32_t l_128[3][1];
        int32_t *l_136 = &g_3;
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_128[i][j] = 0x870E6F01;
        }
        (*l_136) = (safe_add_func_uint32_t_u_u((((l_128[0][0] ^= (--(*l_125))) , 8) , (safe_rshift_func_int16_t_s_s(((safe_rshift_func_uint16_t_u_u((((l_133[2][0] , (safe_rshift_func_uint8_t_u_s((((void*)0 != l_125) , 0x98), g_98))) != (-9)) && (*p_10)), l_128[0][0])) , 7), 0))), g_76));
        for (g_5 = 0; (g_5 <= 0); g_5 += 1)
        { 
            int32_t l_139 = (-6);
            int i, j;
            l_137 |= (-1);
            if (g_116[(g_5 + 1)][g_5])
                break;
            l_139 ^= (+g_116[g_5][g_5]);
        }
    }
    (*p_11) = (*g_120);
    return &g_81;
}

static int32_t * func_12(union U0  p_13, union U0  p_14, union U0  p_15)
{ 
    int32_t *l_29 = &g_3;
    int32_t *l_31 = (void*)0;
    int32_t l_69 = 0x980504C9;
    int32_t l_96 = 0;
    int32_t *l_107 = &g_3;
    int32_t *l_108 = &g_81;
    int32_t *l_109 = &g_81;
    int32_t *l_110 = &l_69;
    int32_t *l_111 = &g_3;
    int32_t *l_112 = (void*)0;
    int32_t *l_113 = (void*)0;
    int32_t *l_114[3][4] = {{&g_3,&g_3,&g_3,&g_3},{&g_3,&g_3,&g_3,&g_3},{&g_3,&g_3,&g_3,&g_3}};
    union U0 l_119[2][2] = {{{0U},{0U}},{{0U},{0U}}};
    int i, j;
    if (g_3)
    { 
        int32_t *l_26[1];
        int32_t **l_30[3];
        union U0 *l_36 = &g_23;
        int32_t *l_63 = &g_3;
        int i;
        for (i = 0; i < 1; i++)
            l_26[i] = (void*)0;
        for (i = 0; i < 3; i++)
            l_30[i] = &l_26[0];
        g_3 = g_22[0][3];
        if (((*l_29) = (((safe_rshift_func_int8_t_s_s((-1), 0)) < (l_29 != (g_32 = (l_31 = l_29)))) , 0xCBDE6B78)))
        { 
            int64_t l_41 = (-4);
            int32_t *l_62 = (void*)0;
lbl_38:
            for (p_13.f2 = (-5); (p_13.f2 <= 3); p_13.f2 = safe_add_func_int64_t_s_s(p_13.f2, 4))
            { 
                int32_t *l_35 = (void*)0;
                return l_35;
            }
            if ((*l_29))
            { 
                union U0 **l_37 = &l_36;
                (*l_37) = l_36;
                (*l_29) = 0xAEC70140;
                if (g_3)
                    goto lbl_38;
            }
            else
            { 
                (*g_32) = ((g_22[0][0] | (safe_rshift_func_int16_t_s_u(l_41, g_22[1][0]))) && 0xC7DFFC5A258D3B3ELL);
            }
            for (p_14.f2 = 0; (p_14.f2 < 23); p_14.f2 = safe_add_func_int64_t_s_s(p_14.f2, 1))
            { 
                uint16_t *l_52 = &g_23.f0;
                int32_t l_58 = (-7);
                uint8_t l_59 = 0U;
                (*g_32) = (safe_lshift_func_int32_t_s_u((((safe_add_func_uint8_t_u_u((safe_div_func_uint32_t_u_u(4294967295U, (safe_div_func_uint16_t_u_u(((*l_52) ^= g_3), (safe_lshift_func_int64_t_s_s((!((safe_mod_func_uint16_t_u_u((0x5184BF4D80A99235LL <= g_22[1][2]), 0x8E87)) & p_14.f2)), 43)))))), p_15.f0)) && 1) >= l_58), 31));
                ++l_59;
                l_63 = l_62;
            }
        }
        else
        { 
            uint64_t l_64 = 0xB15E7CE1BF9D45AALL;
            --l_64;
        }
    }
    else
    { 
        int16_t *l_73[2];
        int32_t l_74 = 0xE9ABA4A7;
        int64_t *l_75 = &g_76;
        uint16_t *l_77 = (void*)0;
        uint16_t *l_78 = &g_72;
        int32_t l_79 = 0;
        int32_t *l_80 = &g_81;
        uint8_t l_87 = 0xF8;
        int32_t l_94[3][1];
        int i, j;
        for (i = 0; i < 2; i++)
            l_73[i] = (void*)0;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_94[i][j] = (-10);
        }
        (*l_80) |= (((((p_15.f0 < ((*l_78) = (safe_lshift_func_int64_t_s_s(((((*l_75) = (((l_74 = (l_69 , (+(((g_71[1][2] == &g_72) < p_15.f0) || p_13.f0)))) > p_14.f0) < 1U)) <= 1) , 1), 38)))) > (*l_29)) , p_14.f0) <= l_79) , p_14.f0);
        if ((safe_sub_func_int32_t_s_s((safe_sub_func_int64_t_s_s(((void*)0 == g_86), g_72)), l_87)))
        { 
            for (g_72 = 0; (g_72 <= 1); g_72 += 1)
            { 
                return &g_81;
            }
            (*l_80) |= (safe_unary_minus_func_int64_t_s(p_13.f0));
        }
        else
        { 
            int8_t l_93[4][1];
            int32_t l_95 = 0x39A49A9C;
            int32_t l_97 = 0x4C2F892C;
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 1; j++)
                    l_93[i][j] = 0x8E;
            }
            for (p_14.f1 = 6; (p_14.f1 >= 1); p_14.f1--)
            { 
                int32_t *l_91 = &l_79;
                int32_t *l_92[2][3] = {{&g_3,&g_3,&g_3},{(void*)0,(void*)0,(void*)0}};
                int i, j;
                g_101++;
                g_86 = &p_13;
                if ((*l_80))
                    break;
            }
            for (l_79 = (-2); (l_79 > 12); l_79 = safe_add_func_uint16_t_u_u(l_79, 4))
            { 
                int32_t **l_106[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_106[i] = &g_32;
                l_31 = ((*l_29) , &g_3);
            }
        }
        (*l_29) = ((*l_80) = 0x6D33D981);
    }
    g_116[1][0]++;
    (*l_109) = ((((l_119[1][0] , 18446744073709551606U) ^ (p_15.f0 > (*l_110))) && (*l_109)) , p_15.f0);
    return &g_3;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_22[i][j], "g_22[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_23.f1, "g_23.f1", print_hash_value);
    transparent_crc(g_72, "g_72", print_hash_value);
    transparent_crc(g_76, "g_76", print_hash_value);
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_98, "g_98", print_hash_value);
    transparent_crc(g_99, "g_99", print_hash_value);
    transparent_crc(g_100, "g_100", print_hash_value);
    transparent_crc(g_101, "g_101", print_hash_value);
    transparent_crc(g_115, "g_115", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_116[i][j], "g_116[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_121, "g_121", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_142[i], "g_142[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
