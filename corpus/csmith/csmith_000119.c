/* Csmith 0cdc710315cf seed 119, flattened by scripts/csmith_flatten.py */
// Options:   --seed 119 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
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
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
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
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 9;
static int16_t g_38 = (-7);
static uint16_t g_42 = 65526U;
static int16_t g_43 = 0x0D61;
static uint16_t g_52 = 5U;
static int64_t g_56 = (-4);
static uint64_t g_69 = 0xE2AA969AAE9A1548LL;
static uint32_t g_71 = 0x110BDBE7;
static const int16_t g_83 = 0x5547;
static int64_t g_106[1] = {0x9D91FBEB5A7EB3ECLL};
static int64_t g_112 = 7;
static int32_t g_117 = 0x52157428;
static uint16_t g_125 = 0x3683;
static int32_t *g_130 = (void*)0;
static int32_t **g_129 = &g_130;
static uint32_t g_147 = 0x3C832F20;

static int64_t  func_1(void);
static int32_t * func_5(int32_t * const  p_6, int8_t  p_7, uint32_t  p_8);
static int16_t  func_11(uint16_t  p_12, int8_t  p_13, uint8_t  p_14, int32_t * p_15, int32_t * const  p_16);

static int64_t  func_1(void)
{ 
    int32_t * const l_9 = &g_2;
    int64_t l_17[4] = {0xDDA37BB00868BB4ALL,0xDDA37BB00868BB4ALL,0xDDA37BB00868BB4ALL,0xDDA37BB00868BB4ALL};
    int32_t *l_18 = (void*)0;
    int i;
    for (g_2 = 0; (g_2 >= (-17)); --g_2)
    { 
        int32_t l_10 = (-1);
        uint32_t *l_114 = &g_71;
        (*g_129) = func_5(l_9, l_10, ((*l_114) = (func_11(l_17[2], l_10, g_2, l_18, &g_2) || l_10)));
        if (l_10)
            break;
        (*g_129) = (*g_129);
    }
    return g_38;
}

static int32_t * func_5(int32_t * const  p_6, int8_t  p_7, uint32_t  p_8)
{ 
    int64_t l_123 = 0xC8C07AB760D964AFLL;
    int32_t l_124 = 0x7F70597A;
    uint16_t * const l_136[3] = {(void*)0,(void*)0,(void*)0};
    int32_t *l_139 = &l_124;
    int32_t l_140 = (-6);
    int32_t *l_141 = &g_117;
    int32_t *l_142 = (void*)0;
    int32_t *l_143 = (void*)0;
    int32_t *l_144 = &g_117;
    int32_t *l_145[4] = {&l_124,&l_124,&l_124,&l_124};
    int8_t l_146[4];
    int i;
    for (i = 0; i < 4; i++)
        l_146[i] = 0x07;
    if (g_106[0])
    { 
        int32_t *l_115 = (void*)0;
        int32_t *l_116 = &g_117;
        int32_t *l_118 = &g_117;
        int32_t *l_119 = &g_117;
        int32_t *l_120 = &g_117;
        int32_t *l_121 = (void*)0;
        int32_t *l_122[2][2];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
                l_122[i][j] = &g_117;
        }
        --g_125;
        l_124 ^= (((safe_unary_minus_func_uint16_t_u((((void*)0 == g_129) >= ((*l_116) = (p_7 , (-1)))))) , 0xFB) ^ l_123);
        (*g_129) = (*g_129);
    }
    else
    { 
        uint32_t *l_135 = &g_71;
        uint16_t * const l_137 = &g_42;
        int32_t l_138 = 0x527E66CD;
        l_138 &= (safe_add_func_uint32_t_u_u(((p_7 && ((*l_135) = (p_8++))) , (l_136[2] != l_137)), 9));
    }
    ++g_147;
    return (*g_129);
}

static int16_t  func_11(uint16_t  p_12, int8_t  p_13, uint8_t  p_14, int32_t * p_15, int32_t * const  p_16)
{ 
    uint64_t l_25[1][1];
    int16_t l_39 = 0xC095;
    int32_t * const l_40[3][3] = {{&g_2,(void*)0,&g_2},{&g_2,(void*)0,&g_2},{&g_2,(void*)0,&g_2}};
    const int64_t l_105 = (-1);
    int8_t l_110[1][3];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_25[i][j] = 18446744073709551615U;
    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_110[i][j] = 0xCC;
    }
lbl_86:
    for (p_12 = (-26); (p_12 > 4); p_12++)
    { 
        int32_t *l_22 = &g_2;
        int32_t **l_21 = &l_22;
        int32_t l_47 = 0xE0C2DF10;
        (*l_21) = &g_2;
        l_25[0][0] = ((*l_22) != ((safe_div_func_uint8_t_u_u(g_2, (0x9EA7EB8E || g_2))) ^ p_14));
        for (p_14 = 0; (p_14 < 34); ++p_14)
        { 
            int16_t *l_37[2];
            uint16_t *l_41 = &g_42;
            int32_t l_44 = 0x2576953C;
            int32_t l_45 = 0x2376568C;
            int32_t l_46 = 4;
            int16_t l_55 = 0;
            int i;
            for (i = 0; i < 2; i++)
                l_37[i] = &g_38;
            if ((safe_lshift_func_uint16_t_u_s((safe_mod_func_uint64_t_u_u((safe_div_func_uint8_t_u_u((((((!((*l_41) = (safe_sub_func_int8_t_s_s((&g_2 != ((l_39 |= 0x9078) , l_40[1][2])), p_12)))) ^ p_13) >= g_43) || g_43) || p_14), (-2))), 0x1CC492975A1448AFLL)), 0)))
            { 
                uint16_t l_48 = 65535U;
                l_48--;
                return g_42;
            }
            else
            { 
                int32_t l_51 = 0xAB6322A5;
                if (g_42)
                    break;
                --g_52;
                (*l_21) = (void*)0;
            }
            l_45 = (l_55 >= ((void*)0 != &g_38));
            g_56 |= (g_38 , (-4));
        }
    }
    if ((safe_rshift_func_uint8_t_u_u(g_56, ((p_14 < ((p_13 & p_13) && p_14)) || (-1)))))
    { 
        uint8_t l_61 = 250U;
        for (g_56 = 5; (g_56 == (-10)); --g_56)
        { 
            return l_61;
        }
        for (g_42 = 0; (g_42 == 40); g_42 = safe_add_func_int8_t_s_s(g_42, 6))
        { 
            int16_t l_64[4] = {1,1,1,1};
            uint8_t *l_70[4][2] = {{&l_61,&l_61},{&l_61,&l_61},{&l_61,&l_61},{&l_61,&l_61}};
            int32_t l_77[2];
            int i, j;
            for (i = 0; i < 2; i++)
                l_77[i] = (-3);
            l_64[1] = (&l_40[0][0] == &p_16);
            if ((p_14 , (safe_add_func_uint32_t_u_u(((((g_71 = (g_52 ^ (safe_rshift_func_uint64_t_u_u(((g_69 < 1U) , 0x34BCD0122BF4B248LL), 63)))) | 0xE0) > g_2) && l_64[3]), l_64[1]))))
            { 
                if ((*p_16))
                    break;
                if (l_64[1])
                    break;
            }
            else
            { 
                return p_13;
            }
            for (l_61 = 0; (l_61 != 33); l_61++)
            { 
                uint8_t *l_76 = &l_61;
                const int16_t **l_80 = (void*)0;
                const int16_t *l_82[2][3] = {{&g_83,(void*)0,&g_83},{&g_83,(void*)0,&g_83}};
                const int16_t **l_81 = &l_82[1][2];
                int i, j;
                l_77[1] = (((((safe_rshift_func_uint64_t_u_u(g_42, (l_70[3][1] != l_76))) | 1) || 18446744073709551615U) & g_71) <= g_69);
                l_77[0] = (g_42 | (safe_mod_func_int8_t_s_s((((*l_81) = &l_64[2]) == &g_83), 0xF1)));
            }
        }
    }
    else
    { 
        uint16_t *l_104[2][4];
        int32_t l_107 = 0x2A7AC8E6;
        int16_t *l_108 = (void*)0;
        int16_t *l_109 = &g_38;
        int64_t *l_111 = &g_112;
        int32_t l_113 = 0x77CE632A;
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 4; j++)
                l_104[i][j] = &g_42;
        }
        for (p_12 = 27; (p_12 != 31); p_12 = safe_add_func_int64_t_s_s(p_12, 1))
        { 
            if (g_2)
                goto lbl_86;
        }
        l_113 &= (safe_sub_func_uint64_t_u_u(g_71, (((l_107 = (((((safe_rshift_func_int64_t_s_s(((*l_111) ^= ((safe_mul_func_uint64_t_u_u((((safe_sub_func_int16_t_s_s(((*l_109) = (((((safe_sub_func_int32_t_s_s((g_106[0] = (safe_rshift_func_uint64_t_u_u((((safe_mod_func_int16_t_s_s((~(((safe_sub_func_int16_t_s_s(g_83, (g_52 = (g_69 & p_13)))) || g_2) >= l_105)), 0xCA48)) , g_52) >= g_38), p_12))), g_2)) , (void*)0) == (void*)0) != g_56) < l_107)), g_69)) , 0x391FFBB2) != l_110[0][0]), g_71)) | l_107)), g_71)) > g_42) > 65535U) | l_107) < p_14)) >= g_83) >= g_2)));
    }
    return g_112;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_71, "g_71", print_hash_value);
    transparent_crc(g_83, "g_83", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_106[i], "g_106[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_112, "g_112", print_hash_value);
    transparent_crc(g_117, "g_117", print_hash_value);
    transparent_crc(g_125, "g_125", print_hash_value);
    transparent_crc(g_147, "g_147", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
