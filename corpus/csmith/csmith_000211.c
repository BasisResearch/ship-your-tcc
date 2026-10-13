/* Csmith 0cdc710315cf seed 211, flattened by scripts/csmith_flatten.py */
// Options:   --seed 211 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
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
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static int32_t
safe_unary_minus_func_int32_t_s(int32_t si )
{
  return
    (si==(-2147483647-1)) ?
    ((si)) :
    -si;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const int8_t  f0;
   int8_t  f1;
   const uint64_t  f2;
};

struct S1 {
   int8_t  f0;
   struct S0  f1;
   struct S0  f2;
   uint16_t  f3;
};

static uint64_t g_2 = 6U;
static int32_t g_5 = 0x1A0734F0;
static int32_t *g_6 = &g_5;
static struct S0 g_14 = {0x0C,5,0xC1DAB9737E0D1B71LL};
static uint64_t g_81 = 0x308FB828C2071398LL;
static int32_t *g_89 = (void*)0;
static struct S0 *g_110 = (void*)0;
static uint16_t g_113 = 0x918A;
static int16_t g_115 = 0x4B68;
static uint16_t *g_118 = &g_113;
static uint16_t **g_117 = &g_118;
static int64_t g_121 = 1;
static uint8_t g_124 = 0x88;
static int16_t *g_159 = &g_115;
static struct S1 g_166 = {-4,{0x6A,7,0xD8CAD771BCF00ED8LL},{-1,0,1U},0x1F76};
static struct S1 *g_165 = &g_166;

static int8_t  func_1(void);
static int32_t  func_9(struct S0  p_10, const int32_t * p_11, uint32_t  p_12, struct S0  p_13);
static int32_t * func_15(uint32_t  p_16, uint64_t  p_17, int16_t  p_18, int64_t  p_19, uint8_t  p_20);

static int8_t  func_1(void)
{ 
    uint64_t l_3[1];
    int32_t *l_4 = &g_5;
    const int16_t l_25 = (-4);
    int8_t l_163[1][2];
    int i, j;
    for (i = 0; i < 1; i++)
        l_3[i] = 0x543BEB7161AB6EC7LL;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_163[i][j] = 0;
    }
    (*l_4) &= (l_3[0] = g_2);
    g_6 = l_4;
    if ((0x7E08A110 != ((safe_sub_func_uint64_t_u_u((func_9(g_14, func_15((safe_add_func_uint8_t_u_u((safe_mod_func_uint32_t_u_u(((g_14.f0 & 0x3E01) || 0xB88DCFD8), (*g_6))), l_25)), g_14.f0, g_2, g_14.f2, (*l_4)), g_14.f2, g_14) >= l_3[0]), l_25)) && (*l_4))))
    { 
        uint8_t l_164 = 0x21;
        (*l_4) &= l_163[0][1];
        (*l_4) &= l_164;
        g_165 = g_165;
    }
    else
    { 
        int16_t l_173 = 0xB2B3;
        uint64_t *l_177 = (void*)0;
        uint64_t **l_176 = &l_177;
        for (g_2 = 0; (g_2 > 3); g_2 = safe_add_func_uint16_t_u_u(g_2, 9))
        { 
            for (g_113 = (-19); (g_113 == 38); g_113++)
            { 
                int8_t l_172 = 0x48;
                uint64_t *l_175 = &g_2;
                uint64_t **l_174 = &l_175;
                l_173 |= (!l_172);
                l_174 = (l_176 = l_174);
            }
        }
    }
    return (*l_4);
}

static int32_t  func_9(struct S0  p_10, const int32_t * p_11, uint32_t  p_12, struct S0  p_13)
{ 
    int8_t l_161 = 0x21;
    int32_t l_162 = (-1);
    l_162 = l_161;
    return (*p_11);
}

static int32_t * func_15(uint32_t  p_16, uint64_t  p_17, int16_t  p_18, int64_t  p_19, uint8_t  p_20)
{ 
    uint8_t l_42 = 8U;
    int32_t *l_50 = &g_5;
    struct S0 *l_56 = (void*)0;
    int32_t l_58 = 0;
    int32_t l_67 = 0xA30FB9C0;
    int32_t l_69 = 1;
    int32_t l_78 = 0xBC551464;
    int32_t **l_88[3];
    int32_t l_90 = 6;
    int32_t l_160 = 0xFB28E062;
    int i;
    for (i = 0; i < 3; i++)
        l_88[i] = &g_6;
lbl_45:
    if ((safe_mod_func_int64_t_s_s((safe_mod_func_int64_t_s_s((safe_sub_func_int32_t_s_s(0x88AD88CA, ((void*)0 != &g_5))), 0x716E3113783312EFLL)), p_17)))
    { 
        int32_t **l_32[3];
        int i;
        for (i = 0; i < 3; i++)
            l_32[i] = &g_6;
        l_32[1] = l_32[1];
    }
    else
    { 
        int64_t l_39 = 0x09CC0F90B8A442F2LL;
        for (p_19 = 0; (p_19 <= 12); p_19++)
        { 
            for (g_2 = 1; (g_2 <= 19); ++g_2)
            { 
                int32_t *l_37 = (void*)0;
                int32_t *l_38 = (void*)0;
                int32_t *l_40 = &g_5;
                int32_t *l_41 = &g_5;
                l_42--;
            }
        }
        if (l_42)
            goto lbl_45;
    }
    for (p_18 = (-10); (p_18 == (-12)); p_18 = safe_sub_func_uint64_t_u_u(p_18, 8))
    { 
        int8_t l_57 = 0xE3;
        int32_t *l_59 = &g_5;
        int32_t *l_60 = (void*)0;
        int32_t *l_61 = &g_5;
        int32_t *l_62 = (void*)0;
        int32_t l_63 = 0x56995E14;
        int32_t *l_64 = &g_5;
        int32_t *l_65 = &l_63;
        int32_t *l_66 = &l_58;
        int32_t *l_68 = &l_63;
        int32_t *l_70 = &l_67;
        int32_t *l_71 = &l_63;
        int32_t *l_72 = &l_58;
        int32_t *l_73 = &l_69;
        int32_t *l_74 = &l_69;
        int32_t *l_75 = &l_67;
        int32_t *l_76 = &g_5;
        int32_t *l_77 = &l_63;
        int32_t *l_79 = &g_5;
        int32_t *l_80[1];
        int i;
        for (i = 0; i < 1; i++)
            l_80[i] = (void*)0;
        for (p_17 = 0; (p_17 != 26); ++p_17)
        { 
            (*g_6) ^= (l_50 != &g_5);
        }
        for (p_17 = 0; (p_17 > 18); p_17 = safe_add_func_int64_t_s_s(p_17, 9))
        { 
            struct S0 *l_55 = (void*)0;
            for (g_2 = 0; (g_2 > 44); ++g_2)
            { 
                l_56 = l_55;
            }
            g_6 = &g_5;
        }
        --g_81;
    }
    if (((safe_rshift_func_uint64_t_u_s((((g_6 = &l_58) != (g_89 = (void*)0)) != (0U & l_90)), p_20)) || g_14.f0))
    { 
        uint8_t l_102 = 0xC5;
        (*l_50) ^= (safe_rshift_func_uint64_t_u_u(((*g_6) ^ ((&g_89 == (void*)0) , 1U)), p_18));
        (*g_6) = (safe_mul_func_uint8_t_u_u((safe_unary_minus_func_int32_t_s((safe_unary_minus_func_int32_t_s((*g_6))))), ((safe_lshift_func_uint32_t_u_u(((safe_rshift_func_uint16_t_u_u(p_18, (safe_unary_minus_func_uint64_t_u(l_102)))) | 1), 7)) > p_17)));
    }
    else
    { 
        uint16_t **l_119[1][2];
        int32_t l_122[3][3] = {{0x8AD0AF09,0x8AD0AF09,0x8AD0AF09},{0x7058F739,0x42098501,0x7058F739},{0x8AD0AF09,0x8AD0AF09,0x8AD0AF09}};
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_119[i][j] = &g_118;
        }
        for (p_17 = 0; (p_17 >= 37); ++p_17)
        { 
            const int64_t l_111 = 1;
            uint16_t *l_112 = &g_113;
            int16_t *l_114 = &g_115;
            int32_t l_142 = (-1);
            if (((*g_6) |= (safe_div_func_uint32_t_u_u(((p_18 ^= ((*l_114) = (((*l_112) = ((!(safe_sub_func_int64_t_s_s(((g_110 = (p_20 , &g_14)) == (void*)0), p_20))) | l_111)) == 0x073B))) <= g_14.f1), g_14.f2))))
            { 
                uint16_t **l_116 = &l_112;
                g_89 = &g_5;
                l_119[0][1] = (g_117 = l_116);
            }
            else
            { 
                return &g_5;
            }
            for (l_58 = 0; (l_58 <= 2); l_58 += 1)
            { 
                int32_t l_120 = 0x9701DEE0;
                int32_t l_123 = (-1);
                uint64_t *l_136 = (void*)0;
                uint64_t *l_137[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_137[i] = &g_2;
                g_89 = &g_5;
                --g_124;
                (*g_89) = ((-1) || (l_122[0][0] = ((safe_sub_func_uint16_t_u_u(((*g_118)++), (((safe_mul_func_uint16_t_u_u((0x4F || (safe_mul_func_uint32_t_u_u((safe_unary_minus_func_uint64_t_u((g_81++))), ((safe_lshift_func_uint16_t_u_u((p_20 <= g_121), 9)) != p_17)))), 0x5CA7)) , l_142) , 1))) && p_17)));
            }
            (*g_6) = (g_2 , ((((**g_117) = ((safe_rshift_func_uint64_t_u_u(l_122[0][2], 4)) != 0x0601)) != p_18) >= g_14.f2));
        }
        for (l_90 = 18; (l_90 > (-28)); --l_90)
        { 
            for (p_17 = (-1); (p_17 <= 45); ++p_17)
            { 
                const int32_t *l_149 = &g_5;
                const int32_t **l_150 = &l_149;
                (*l_150) = l_149;
            }
        }
        (*g_6) ^= ((18446744073709551615U & (safe_div_func_uint8_t_u_u((safe_mod_func_uint64_t_u_u((safe_add_func_uint64_t_u_u((safe_mod_func_uint8_t_u_u(((void*)0 != g_159), 0x23)), l_160)), p_16)), p_17))) , 0x8FA5C3DA);
    }
    return &g_5;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_14.f0, "g_14.f0", print_hash_value);
    transparent_crc(g_14.f1, "g_14.f1", print_hash_value);
    transparent_crc(g_14.f2, "g_14.f2", print_hash_value);
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_113, "g_113", print_hash_value);
    transparent_crc(g_115, "g_115", print_hash_value);
    transparent_crc(g_121, "g_121", print_hash_value);
    transparent_crc(g_124, "g_124", print_hash_value);
    transparent_crc(g_166.f0, "g_166.f0", print_hash_value);
    transparent_crc(g_166.f1.f0, "g_166.f1.f0", print_hash_value);
    transparent_crc(g_166.f1.f1, "g_166.f1.f1", print_hash_value);
    transparent_crc(g_166.f1.f2, "g_166.f1.f2", print_hash_value);
    transparent_crc(g_166.f2.f0, "g_166.f2.f0", print_hash_value);
    transparent_crc(g_166.f2.f1, "g_166.f2.f1", print_hash_value);
    transparent_crc(g_166.f2.f2, "g_166.f2.f2", print_hash_value);
    transparent_crc(g_166.f3, "g_166.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
