/* Csmith 0cdc710315cf seed 68, flattened by scripts/csmith_flatten.py */
// Options:   --seed 68 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (4294967295U >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
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
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   uint16_t  f1;
   uint8_t  f2;
};

struct S1 {
   int64_t  f0;
   const uint16_t  f1;
};

struct S2 {
   uint16_t  f0;
};

struct S3 {
   uint32_t  f0;
};

static int32_t g_2 = 0x5683C8FC;
static struct S2 g_16 = {65535U};
static int32_t g_22 = 0;
static int64_t g_27 = 0x4D8D955A61940C9BLL;
static uint32_t g_29[2][1] = {{1U},{1U}};
static const struct S1 g_44[2][1] = {{{0x3C489718B2F1036CLL,0x418E}},{{0x3C489718B2F1036CLL,0x418E}}};
static uint16_t g_68 = 0x17ED;
static uint16_t g_77 = 65532U;
static int8_t g_83 = 0;
static uint32_t g_92 = 6U;
static struct S0 g_104 = {-1,0xBB6B,0xE6};
static int64_t *g_107 = &g_27;
static int64_t **g_106 = &g_107;
static uint64_t g_121 = 1U;
static struct S0 *g_124 = &g_104;
static uint32_t g_145[3] = {0U,0U,0U};
static struct S3 g_149 = {0xBD42CCF7};
static struct S3 *g_148 = &g_149;
static struct S1 g_153 = {-1,0x58D7};
static uint8_t * const *g_163 = (void*)0;

static int16_t  func_1(void);
static int32_t  func_5(const struct S3  p_6, int64_t  p_7, uint64_t  p_8, const struct S1  p_9, int64_t  p_10);
static const struct S3  func_11(struct S2  p_12, uint32_t  p_13, uint8_t  p_14, struct S1  p_15);

static int16_t  func_1(void)
{ 
    struct S1 l_18 = {0xD2B5F7919ADB8213LL,0x2F3D};
    int32_t l_143 = 0xABD02100;
    int32_t l_144 = 3;
    int64_t *l_168 = &g_153.f0;
    struct S3 l_172 = {4294967295U};
    struct S1 *l_174 = &l_18;
    struct S1 **l_173 = &l_174;
    for (g_2 = (-29); (g_2 >= (-22)); ++g_2)
    { 
        struct S2 *l_17 = &g_16;
        int32_t *l_141 = &g_22;
        struct S1 *l_152[4][3] = {{&g_153,&g_153,&g_153},{&g_153,&l_18,(void*)0},{&g_153,&l_18,&g_153},{&l_18,&g_153,(void*)0}};
        int8_t l_171[1];
        int i, j;
        for (i = 0; i < 1; i++)
            l_171[i] = 0;
    }
    (*l_173) = (void*)0;
    return g_68;
}

static int32_t  func_5(const struct S3  p_6, int64_t  p_7, uint64_t  p_8, const struct S1  p_9, int64_t  p_10)
{ 
    struct S1 l_45 = {-3,0xE0AD};
    int64_t l_46 = 0xB5C50EC31937DA87LL;
    int32_t l_55 = 0x90D92204;
    int32_t l_56 = 5;
    struct S2 *l_90 = &g_16;
    uint32_t *l_125 = (void*)0;
    int32_t *l_140 = &l_56;
    if ((l_46 = (p_8 ^ (l_45 , (6 < g_16.f0)))))
    { 
        int64_t l_47 = 0xEB613644CAD70698LL;
        int64_t *l_54 = &l_45.f0;
        int32_t l_119 = 0x341DA9CD;
        uint32_t *l_126[1];
        int i;
        for (i = 0; i < 1; i++)
            l_126[i] = &g_92;
        l_47 = 1;
        if (((l_56 = (7 > (safe_sub_func_uint32_t_u_u((safe_rshift_func_uint16_t_u_s(((l_55 = ((*l_54) = (!(((g_16 , (~p_7)) , 0x5B4C) , l_47)))) , l_47), p_9.f1)), g_22)))) , (-8)))
        { 
            uint16_t *l_67 = &g_16.f0;
            uint32_t *l_69 = &g_29[1][0];
            int8_t l_74 = 1;
            int32_t *l_75 = &g_22;
            uint16_t *l_76 = &g_77;
            if (((safe_add_func_int64_t_s_s(0xD870105C6596AAD9LL, (safe_mod_func_uint32_t_u_u((safe_rshift_func_int16_t_s_u((safe_unary_minus_func_uint8_t_u((~(((*l_76) &= (((safe_lshift_func_uint16_t_u_s((g_68 = ((*l_67) = p_9.f1)), 11)) & (--(*l_69))) != ((*l_75) ^= (safe_lshift_func_int64_t_s_s(0xAA3630DD35930CB0LL, l_74))))) , l_45.f0)))), 5)), 0x300FFBEB)))) , g_2))
            { 
                int32_t *l_82 = &g_22;
                (*l_82) |= ((((safe_lshift_func_int32_t_s_s(((safe_lshift_func_uint32_t_u_s(g_68, 0)) , ((g_77 ^= ((g_83 ^= ((l_82 != &g_22) && g_2)) & g_29[1][0])) == p_9.f0)), g_68)) && g_68) >= (-4)) & 0xC8DFB1A916490A06LL);
            }
            else
            { 
                uint32_t *l_91 = &g_92;
                int32_t l_105 = 7;
                (*l_75) |= (safe_rshift_func_int64_t_s_s(((safe_mod_func_int32_t_s_s((safe_mod_func_uint64_t_u_u((g_29[1][0] <= 0x15481186C96E7F8ELL), (((*l_91) &= ((void*)0 != l_90)) , p_9.f0))), 0xCACAAAB2)) ^ 0x3B6D3E8DF325EC3ELL), 63));
                (*l_75) = ((((*l_69) = ((~((safe_mod_func_int8_t_s_s((safe_div_func_int16_t_s_s((7 < (((((safe_rshift_func_int32_t_s_u((safe_div_func_uint8_t_u_u((253U <= (safe_rshift_func_int16_t_s_u((g_92 && 18446744073709551612U), p_7))), g_16.f0)), g_2)) <= l_47) , g_104) , l_105) , p_6.f0)), 0x787C)), 0x3D)) < 0)) ^ 0x3ABB)) && g_29[0][0]) >= l_47);
            }
            g_106 = &l_54;
        }
        else
        { 
            uint32_t l_116 = 0xB2300A90;
            for (g_27 = 0; (g_27 >= 0); g_27 -= 1)
            { 
                int32_t *l_108 = (void*)0;
                int32_t *l_109 = &l_56;
                int32_t *l_110 = &l_56;
                int32_t *l_111 = &l_55;
                int32_t *l_112 = &l_56;
                int32_t *l_113 = &l_55;
                int32_t *l_114 = &l_55;
                int32_t *l_115[4] = {&g_22,&g_22,&g_22,&g_22};
                int i;
                l_116--;
                l_119 = ((**g_106) > ((l_56 = 0x7690A484C52037B1LL) <= l_116));
                return p_9.f1;
            }
        }
        for (g_22 = 0; (g_22 <= 0); g_22 += 1)
        { 
            int32_t *l_120[4][3];
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 3; j++)
                    l_120[i][j] = &l_55;
            }
            g_121--;
            for (g_68 = 0; (g_68 <= 0); g_68 += 1)
            { 
                int32_t l_127 = 0;
                int i, j;
                g_124 = &g_104;
                l_127 |= (&g_29[(g_68 + 1)][g_22] != (l_126[0] = l_125));
                if (l_56)
                    break;
            }
        }
    }
    else
    { 
        for (l_56 = 0; (l_56 != (-17)); l_56 = safe_sub_func_uint16_t_u_u(l_56, 1))
        { 
            for (p_7 = 0; (p_7 < (-4)); p_7 = safe_sub_func_uint64_t_u_u(p_7, 9))
            { 
                uint8_t *l_134 = (void*)0;
                uint8_t *l_135 = &g_104.f2;
                int32_t *l_136 = (void*)0;
                int32_t *l_137 = (void*)0;
                int32_t l_138 = 0x7D739AC8;
                if (p_9.f1)
                    break;
                l_138 = (g_77 && (((*l_135) &= (safe_lshift_func_int16_t_s_s(1, 11))) | p_10));
            }
        }
    }
    (*l_140) |= (!g_44[1][0].f1);
    return p_7;
}

static const struct S3  func_11(struct S2  p_12, uint32_t  p_13, uint8_t  p_14, struct S1  p_15)
{ 
    int64_t l_24 = 0x523DD972C40A98CELL;
    int32_t l_28[2];
    int8_t l_42 = (-1);
    const struct S3 l_43[3][3] = {{{0U},{0U},{0U}},{{0U},{0U},{0U}},{{0U},{0U},{0U}}};
    int i, j;
    for (i = 0; i < 2; i++)
        l_28[i] = 0x5FC1C575;
    for (p_13 = 0; (p_13 != 32); p_13++)
    { 
        int32_t *l_21 = &g_22;
        int32_t *l_23 = &g_22;
        int32_t l_25 = 3;
        int32_t *l_26[2];
        struct S2 *l_35 = &g_16;
        struct S2 **l_34 = &l_35;
        int i;
        for (i = 0; i < 2; i++)
            l_26[i] = &l_25;
        g_29[1][0]--;
        for (g_16.f0 = 0; (g_16.f0 <= 1); g_16.f0 += 1)
        { 
            struct S2 *l_33 = &g_16;
            struct S2 **l_32 = &l_33;
            for (l_25 = 1; (l_25 >= 0); l_25 -= 1)
            { 
                int i;
                l_26[l_25] = &l_28[g_16.f0];
                l_34 = l_32;
            }
        }
        l_42 ^= (safe_mod_func_uint64_t_u_u(((safe_rshift_func_int16_t_s_u(p_14, g_27)) , (safe_sub_func_int16_t_s_s((*l_23), 0x4ACA))), 18446744073709551615U));
    }
    return l_43[2][2];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_16.f0, "g_16.f0", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_29[i][j], "g_29[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_44[i][j].f0, "g_44[i][j].f0", print_hash_value);
            transparent_crc(g_44[i][j].f1, "g_44[i][j].f1", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_68, "g_68", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_83, "g_83", print_hash_value);
    transparent_crc(g_92, "g_92", print_hash_value);
    transparent_crc(g_104.f0, "g_104.f0", print_hash_value);
    transparent_crc(g_104.f1, "g_104.f1", print_hash_value);
    transparent_crc(g_104.f2, "g_104.f2", print_hash_value);
    transparent_crc(g_121, "g_121", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_145[i], "g_145[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_149.f0, "g_149.f0", print_hash_value);
    transparent_crc(g_153.f0, "g_153.f0", print_hash_value);
    transparent_crc(g_153.f1, "g_153.f1", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
