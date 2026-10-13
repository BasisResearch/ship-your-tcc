/* Csmith 0cdc710315cf seed 221, flattened by scripts/csmith_flatten.py */
// Options:   --seed 221 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
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
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
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
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint32_t
safe_unary_minus_func_uint32_t_u(uint32_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int8_t g_27 = 0xD0;
static uint64_t g_46 = 9U;
static uint32_t g_48 = 1U;
static uint64_t *g_60 = &g_46;
static int32_t g_64 = 0;
static int32_t *g_63 = &g_64;
static uint8_t g_74 = 0U;
static int16_t g_77 = 0x276A;
static uint32_t g_94 = 4294967286U;
static int32_t **g_95 = &g_63;
static int32_t **g_96 = &g_63;
static uint64_t g_120 = 0x757FEC6B6B0D4880LL;
static uint32_t g_133 = 0x54BB3B6D;
static int64_t g_134[2][1] = {{3},{3}};
static uint16_t g_147 = 65528U;
static int8_t g_157 = 0xB1;

static uint16_t  func_1(void);
static const uint32_t  func_5(const int16_t  p_6, uint32_t  p_7, uint64_t  p_8, int16_t  p_9);
static uint64_t  func_17(int64_t  p_18, int8_t  p_19, uint8_t  p_20, uint64_t  p_21, uint8_t  p_22);

static uint16_t  func_1(void)
{ 
    int16_t l_2[1];
    uint64_t l_28[3][4] = {{0xC6088B92280643B8LL,0xC6088B92280643B8LL,0xC6088B92280643B8LL,0xC6088B92280643B8LL},{0xC6088B92280643B8LL,0xC6088B92280643B8LL,0xC6088B92280643B8LL,0xC6088B92280643B8LL},{0xC6088B92280643B8LL,0xC6088B92280643B8LL,0xC6088B92280643B8LL,0xC6088B92280643B8LL}};
    uint16_t l_29 = 0x7857;
    int64_t l_30 = 0xF67AB6CE1A812C00LL;
    const int8_t l_31 = (-1);
    uint16_t l_32[3];
    const uint32_t *l_107 = &g_48;
    int16_t l_142 = 0x3B28;
    int32_t l_152 = 0x48E4E026;
    uint32_t l_162 = 0x690FF0A8;
    int i, j;
    for (i = 0; i < 1; i++)
        l_2[i] = (-1);
    for (i = 0; i < 3; i++)
        l_32[i] = 0x4418;
    if ((l_2[0] , (safe_div_func_int64_t_s_s((func_5((~(safe_mod_func_int16_t_s_s(((safe_rshift_func_int16_t_s_s((safe_lshift_func_uint64_t_u_s(func_17((safe_mul_func_uint16_t_u_u((((l_2[0] != (((((safe_mul_func_uint64_t_u_u(((g_27 == l_2[0]) , g_27), l_28[2][0])) , 0U) <= l_2[0]) <= (-1)) , 0x49968CB667F5AEE0LL)) || l_29) , g_27), g_27)), l_2[0], g_27, g_27, l_2[0]), l_30)), l_2[0])) , l_31), l_29))), l_2[0], l_28[1][1], l_32[2]) <= (*g_63)), 0xF3DD08C68BE14B93LL))))
    { 
        int16_t *l_82 = &l_2[0];
        int16_t *l_83 = &g_77;
        int32_t l_99[2];
        int i;
        for (i = 0; i < 2; i++)
            l_99[i] = 0;
        g_63 = (void*)0;
        if (((4 >= ((((((*l_83) = ((*l_82) = g_27)) | (safe_div_func_int8_t_s_s((safe_sub_func_int8_t_s_s(0x23, g_46)), (-1)))) , g_27) == g_64) && (-4))) , 0x2E755E18))
        { 
            uint32_t *l_92 = &g_48;
            uint32_t *l_93 = &g_94;
            g_96 = (g_95 = ((((*l_93) |= ((*l_92) = (((*g_60)--) && (((0x0988C8CF6E8DA2FBLL > (g_48 >= (l_31 > g_48))) ^ g_27) ^ 0xA93D2D6DA1B12C12LL)))) , (-6)) , &g_63));
            return g_77;
        }
        else
        { 
            for (g_27 = 0; (g_27 <= 0); g_27 += 1)
            { 
                int i;
                return l_2[g_27];
            }
        }
        for (l_29 = (-26); (l_29 <= 2); l_29 = safe_add_func_int8_t_s_s(l_29, 4))
        { 
            return l_99[1];
        }
    }
    else
    { 
        const uint8_t l_100 = 1U;
        int32_t l_101 = (-2);
        uint64_t *l_119[4][1];
        const uint64_t l_121 = 1U;
        int8_t l_124 = (-9);
        uint16_t *l_131 = (void*)0;
        uint16_t *l_132[4][3] = {{&l_29,&l_29,&l_32[0]},{&l_32[1],&l_32[0],&l_32[0]},{&l_32[0],&l_32[2],&l_29},{&l_32[1],&l_32[2],&l_32[1]}};
        int32_t *l_135 = (void*)0;
        int32_t *l_136 = &l_101;
        uint8_t l_138 = 1U;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_119[i][j] = &g_120;
        }
        l_101 = l_100;
        if (g_94)
            goto lbl_108;
        if (((((g_77 >= (+(-3))) , (((safe_mod_func_int32_t_s_s(((safe_add_func_uint16_t_u_u(((((void*)0 == l_107) == 0xC8546551E483D774LL) && 0U), l_2[0])) <= g_46), (**g_96))) == g_77) >= 0x57241B9A)) & g_74) < 0x5E98))
        { 
            if (l_100)
            { 
                return l_100;
            }
            else
            { 
lbl_108:
                (*g_95) = (*g_95);
                (*g_95) = ((((safe_add_func_int16_t_s_s((safe_div_func_int8_t_s_s((safe_div_func_int8_t_s_s((l_28[2][0] ^ (((safe_lshift_func_uint64_t_u_u((g_120 &= (--(*g_60))), 48)) || 1U) > l_121)), (-10))), g_77)), g_94)) , &g_63) != &g_63) , (void*)0);
            }
            (*l_136) ^= (safe_mul_func_uint64_t_u_u(((*g_60) |= l_124), (safe_mod_func_uint16_t_u_u((0xDB64 <= ((((safe_mul_func_uint64_t_u_u((safe_add_func_uint16_t_u_u((g_133 = 65534U), 1U)), g_74)) , 1) >= l_124) <= g_134[1][0])), g_94))));
            (*l_136) = (safe_unary_minus_func_uint32_t_u(l_138));
        }
        else
        { 
            int32_t *l_141 = &g_64;
            (*g_96) = (*g_96);
            for (l_29 = 0; (l_29 <= 2); l_29 += 1)
            { 
                int32_t l_143 = 0x30126A6C;
                int i;
                (**g_96) |= l_32[l_29];
                (*g_95) = (*g_96);
                l_143 = (&l_32[l_29] != ((safe_lshift_func_int64_t_s_s((((((((((*g_95) = l_141) == &l_101) , g_46) , g_46) ^ l_142) == g_48) < g_134[1][0]) <= 0xE01F0FAE), (*l_136))) , (void*)0));
            }
            (**g_95) &= (-4);
        }
        for (g_48 = 0; (g_48 <= 2); g_48 += 1)
        { 
            int i;
            g_64 ^= l_32[g_48];
            for (g_27 = 2; (g_27 >= 0); g_27 -= 1)
            { 
                int32_t *l_144 = &l_101;
                int32_t *l_145 = &l_101;
                int32_t *l_146 = &l_101;
                ++g_147;
            }
        }
    }
    if ((safe_div_func_int16_t_s_s((7 >= ((0xCD2E != (l_152 ^= ((0xA4C82FA94D608905LL && g_134[0][0]) , 0xEA6A))) , l_28[2][1])), g_147)))
    { 
        uint32_t l_154 = 0U;
        int16_t *l_155 = (void*)0;
        int8_t *l_156 = &g_157;
        int64_t *l_158 = &l_30;
        int32_t *l_159 = (void*)0;
        int32_t *l_160 = &l_152;
        (*l_160) = ((((*l_158) = ((+g_46) < ((*l_156) ^= (g_27 &= ((l_154 , l_155) == (void*)0))))) >= l_2[0]) ^ g_46);
    }
    else
    { 
        int32_t *l_161[1];
        int i;
        for (i = 0; i < 1; i++)
            l_161[i] = &l_152;
        (*g_96) = l_161[0];
    }
    return l_162;
}

static const uint32_t  func_5(const int16_t  p_6, uint32_t  p_7, uint64_t  p_8, int16_t  p_9)
{ 
    int32_t *l_35 = (void*)0;
    int32_t l_44 = 0x9C894628;
    uint64_t *l_45 = &g_46;
    uint64_t l_47 = 2U;
    uint64_t *l_59 = &g_46;
    int32_t *l_67 = &l_44;
    for (p_8 = 0; (p_8 >= 39); p_8 = safe_add_func_uint16_t_u_u(p_8, 1))
    { 
        int32_t **l_36 = &l_35;
        int32_t l_37 = (-7);
        (*l_36) = l_35;
        l_37 = (-1);
        for (p_7 = 0; (p_7 < 18); ++p_7)
        { 
            int32_t *l_40 = (void*)0;
            int32_t *l_41 = &l_37;
            (*l_41) = (-8);
            return g_27;
        }
    }
    g_48 |= ((g_27 >= (((safe_sub_func_uint64_t_u_u(((*l_45) = (l_44 ^ (&l_35 != (void*)0))), 0x1E341F56A6D983A5LL)) == p_6) != l_47)) <= l_47);
    for (g_27 = 10; (g_27 < 11); g_27++)
    { 
        int32_t l_62 = 0;
        int32_t *l_68 = &g_64;
        int32_t l_72 = 0xE8BA66F8;
        int32_t **l_81[2];
        int i;
        for (i = 0; i < 2; i++)
            l_81[i] = &l_67;
    }
    return p_7;
}

static uint64_t  func_17(int64_t  p_18, int8_t  p_19, uint8_t  p_20, uint64_t  p_21, uint8_t  p_22)
{ 
    return g_27;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_94, "g_94", print_hash_value);
    transparent_crc(g_120, "g_120", print_hash_value);
    transparent_crc(g_133, "g_133", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_134[i][j], "g_134[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_147, "g_147", print_hash_value);
    transparent_crc(g_157, "g_157", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
