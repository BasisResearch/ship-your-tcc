/* Csmith 0cdc710315cf seed 154, flattened by scripts/csmith_flatten.py */
// Options:   --seed 154 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
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
   const uint8_t  f0;
   int16_t  f1;
};

struct S1 {
   int16_t  f0;
   uint8_t  f1;
};

union U2 {
   uint64_t  f0;
};

static int32_t g_4 = 0x748F69F7;
static int32_t g_6 = 0x14B733C6;
static uint16_t g_21 = 0x45C1;
static uint64_t g_27 = 18446744073709551615U;
static int32_t g_31 = (-6);
static union U2 g_33 = {0xEC0021FBB9CF7D16LL};
static union U2 *g_34 = (void*)0;
static struct S1 g_35 = {0x1826,1U};
static struct S1 g_37 = {5,0xE8};
static int8_t g_71 = 0x59;
static uint32_t g_72[1] = {0x80B142B9};
static struct S1 *g_76 = &g_35;
static const struct S1 *g_87 = (void*)0;
static const struct S1 **g_86 = &g_87;
static uint16_t *g_89 = &g_21;
static uint16_t **g_88 = &g_89;
static uint8_t g_138 = 1U;

static int16_t  func_1(void);
static const struct S1 ** func_38(uint16_t * const  p_39);
static int64_t  func_43(struct S0  p_44, uint32_t  p_45, uint64_t  p_46, uint64_t * p_47);

static int16_t  func_1(void)
{ 
    int8_t l_2 = 0x9F;
    int32_t *l_3 = &g_4;
    int32_t *l_5 = &g_6;
    union U2 * const l_32 = &g_33;
    uint16_t * const l_40[3][4] = {{&g_21,&g_21,&g_21,&g_21},{&g_21,&g_21,&g_21,&g_21},{&g_21,&g_21,&g_21,&g_21}};
    const struct S1 **l_133 = &g_87;
    int32_t *l_134 = (void*)0;
    int32_t *l_135 = &g_4;
    int32_t *l_136[3];
    int16_t l_137 = (-9);
    int i, j;
    for (i = 0; i < 3; i++)
        l_136[i] = &g_4;
    if (((*l_5) &= ((*l_3) = l_2)))
    { 
        uint32_t l_22 = 0x3CC47D1A;
        struct S1 *l_36 = &g_37;
        for (g_6 = 0; (g_6 < 25); g_6++)
        { 
            uint16_t *l_17 = (void*)0;
            uint16_t *l_18 = (void*)0;
            uint16_t *l_19 = (void*)0;
            uint16_t *l_20 = &g_21;
            uint64_t *l_25 = (void*)0;
            uint64_t *l_26 = &g_27;
            int32_t *l_30 = &g_31;
            l_22 = (safe_rshift_func_int32_t_s_u((g_4 = 1), (!(safe_div_func_int64_t_s_s((~((*l_5) == ((*l_20) = (safe_mul_func_int16_t_s_s(g_6, 0xD5A5))))), (*l_5))))));
            (*l_30) ^= (((safe_mul_func_int8_t_s_s((*l_3), (((*l_26)--) && g_4))) != 0xFF60) <= 2);
            g_34 = l_32;
        }
        (*l_36) = (g_35 = g_35);
        (*l_3) = g_21;
    }
    else
    { 
        l_133 = func_38(l_40[0][0]);
    }
    g_138++;
    return g_72[0];
}

static const struct S1 ** func_38(uint16_t * const  p_39)
{ 
    struct S0 l_48[3] = {{0xC2,0},{0xC2,0},{0xC2,0}};
    union U2 *l_49 = &g_33;
    union U2 *l_50 = &g_33;
    uint64_t *l_51 = &g_27;
    struct S1 l_79 = {-4,255U};
    const struct S1 *l_83 = &g_37;
    const struct S1 **l_82 = &l_83;
    int32_t *l_99[1];
    int i;
    for (i = 0; i < 1; i++)
        l_99[i] = &g_6;
lbl_119:
    if ((safe_div_func_int64_t_s_s(func_43(l_48[1], (((l_49 != l_50) != l_48[1].f1) || g_6), g_37.f1, l_51), g_35.f0)))
    { 
        const struct S1 **l_84 = &l_83;
        for (g_4 = 0; (g_4 <= 19); g_4 = safe_add_func_uint16_t_u_u(g_4, 3))
        { 
            (*g_76) = l_79;
        }
        for (g_33.f0 = 7; (g_33.f0 < 12); ++g_33.f0)
        { 
            const struct S1 **l_85 = &l_83;
            return g_86;
        }
        if (g_35.f0)
            goto lbl_119;
    }
    else
    { 
        int16_t l_94[2][1];
        int32_t l_97 = 0xAF05E375;
        struct S1 l_98 = {-8,0x98};
        const struct S1 **l_118 = &l_83;
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_94[i][j] = 0x26E3;
        }
        if (g_31)
        { 
            uint16_t **l_90 = (void*)0;
            l_90 = g_88;
        }
        else
        { 
            uint16_t l_91 = 65535U;
            struct S1 l_95[3][4] = {{{0xAF1E,246U},{0xAF1E,246U},{0xAF1E,246U},{0xAF1E,246U}},{{0xAF1E,246U},{0xAF1E,246U},{0xAF1E,246U},{0xAF1E,246U}},{{0xAF1E,246U},{0xAF1E,246U},{0xAF1E,246U},{0xAF1E,246U}}};
            int32_t *l_96[3][1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_96[i][j] = &g_31;
            }
            l_95[1][2] = ((l_91 || (safe_add_func_uint32_t_u_u(l_79.f1, ((4294967288U != l_94[1][0]) < l_79.f0)))) , (*g_76));
            l_97 &= g_4;
        }
        l_99[0] = ((l_98 , (*p_39)) , &g_31);
        for (g_35.f0 = 0; (g_35.f0 == (-26)); g_35.f0 = safe_sub_func_int8_t_s_s(g_35.f0, 6))
        { 
            uint8_t l_116 = 255U;
            int32_t l_117 = 5;
            for (g_27 = 0; (g_27 < 20); ++g_27)
            { 
                uint64_t *l_106 = &g_33.f0;
                int32_t l_109 = 0x50B83611;
                l_117 = (safe_rshift_func_int16_t_s_s((((*l_106)++) || (l_109 , (!(~((safe_div_func_int16_t_s_s((g_33 , l_109), l_116)) || 0))))), l_98.f0));
                g_4 = g_72[0];
            }
            return &g_87;
        }
    }
    for (l_79.f1 = 0; (l_79.f1 <= 0); l_79.f1 += 1)
    { 
        int32_t **l_122 = (void*)0;
        int32_t **l_123 = &l_99[0];
        struct S1 l_131 = {0,255U};
        int i;
        g_4 ^= g_72[l_79.f1];
        (*l_123) = (((*p_39) , ((safe_div_func_int64_t_s_s(g_37.f0, (1U || g_4))) > 1U)) , l_99[0]);
        for (g_4 = 0; (g_4 <= 0); g_4 += 1)
        { 
            int8_t l_130 = 0x1C;
            for (g_37.f0 = 0; (g_37.f0 <= 0); g_37.f0 += 1)
            { 
                int16_t l_129 = 4;
                struct S1 *l_132 = &l_131;
                g_6 ^= ((safe_rshift_func_uint32_t_u_u(g_37.f0, ((safe_unary_minus_func_uint32_t_u(((safe_lshift_func_uint64_t_u_u(((g_72[0] >= (g_4 ^ l_129)) & l_130), 47)) > g_71))) == 65534U))) != g_35.f0);
                (*l_123) = &g_31;
                (*l_132) = l_131;
            }
            if ((**l_123))
                break;
        }
    }
    return &g_87;
}

static int64_t  func_43(struct S0  p_44, uint32_t  p_45, uint64_t  p_46, uint64_t * p_47)
{ 
    uint16_t l_58 = 0x6AC1;
    int32_t *l_66 = &g_4;
    int32_t *l_67 = &g_31;
    int32_t *l_68 = (void*)0;
    int32_t *l_69 = (void*)0;
    int32_t *l_70[2];
    struct S1 *l_75 = &g_37;
    int i;
    for (i = 0; i < 2; i++)
        l_70[i] = &g_31;
    for (g_33.f0 = 0; (g_33.f0 >= 27); g_33.f0++)
    { 
        uint32_t l_56 = 9U;
        for (g_21 = 8; (g_21 < 48); g_21 = safe_add_func_uint32_t_u_u(g_21, 5))
        { 
            uint32_t l_62 = 0x64F7D9FD;
            int32_t *l_65 = &g_6;
            if ((g_37 , l_56))
            { 
                return l_56;
            }
            else
            { 
                int32_t *l_57[4][1];
                int i, j;
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_57[i][j] = &g_31;
                }
                ++l_58;
                g_4 ^= (~(l_56 && 1U));
                l_62--;
            }
            l_65 = l_65;
            g_4 |= p_44.f0;
        }
    }
    g_72[0]--;
    g_76 = (p_44 , l_75);
    return g_33.f0;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_33.f0, "g_33.f0", print_hash_value);
    transparent_crc(g_35.f0, "g_35.f0", print_hash_value);
    transparent_crc(g_35.f1, "g_35.f1", print_hash_value);
    transparent_crc(g_37.f0, "g_37.f0", print_hash_value);
    transparent_crc(g_37.f1, "g_37.f1", print_hash_value);
    transparent_crc(g_71, "g_71", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_72[i], "g_72[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_138, "g_138", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
