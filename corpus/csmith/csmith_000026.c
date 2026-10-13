/* Csmith 0cdc710315cf seed 26, flattened by scripts/csmith_flatten.py */
// Options:   --seed 26 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
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
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
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
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint16_t  f0;
   const uint8_t  f1;
};

union U1 {
   int32_t  f0;
   uint32_t  f1;
   struct S0  f2;
};

union U3 {
   struct S0  f0;
};

union U4 {
   uint64_t  f0;
   int32_t  f1;
   struct S0  f2;
};

static int32_t g_2[1] = {0};
static int32_t g_3 = 0x6B9DD952;
static int32_t g_4 = (-8);
static int32_t g_5 = 1;
static int32_t g_7 = 0xDAD5A519;
static uint16_t g_25 = 1U;
static uint32_t g_35 = 0xA9172C2E;
static int32_t *g_59 = &g_7;
static int32_t **g_58 = &g_59;
static int16_t g_61[3][4] = {{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)}};
static int16_t g_64 = 0xC99C;
static uint8_t g_76[3] = {0x18,0x18,0x18};
static int32_t g_86 = 3;
static int16_t g_89 = (-4);
static uint16_t g_91 = 1U;
static int8_t g_102 = 0x2D;
static union U1 g_134 = {0xDD7BE084};
static union U1 *g_133 = &g_134;
static uint64_t g_197 = 18446744073709551615U;

static int16_t  func_1(void);
static int32_t ** const  func_8(int32_t * p_9, uint32_t  p_10, int8_t  p_11, int64_t  p_12, uint64_t  p_13);
static int8_t  func_14(int32_t * p_15, int32_t ** p_16, int32_t ** p_17);

static int16_t  func_1(void)
{ 
    int32_t *l_132 = &g_5;
    struct S0 l_144 = {0U,0xA2};
    union U1 **l_145 = &g_133;
    int32_t l_149[4][4] = {{0x540276F7,0x540276F7,1,0x540276F7},{0x540276F7,0x7C34602A,0x7C34602A,0x540276F7},{0x7C34602A,0x540276F7,0x7C34602A,0x7C34602A},{0x540276F7,0x540276F7,1,0x540276F7}};
    int8_t l_157 = 0xD6;
    union U4 l_177 = {0x2EBA6A255FDD9B1ELL};
    int32_t *l_179 = &g_5;
    int32_t *l_180 = &g_3;
    int32_t *l_181 = &g_134.f0;
    int32_t *l_182 = &g_134.f0;
    int32_t *l_183 = &g_3;
    int32_t *l_184 = &g_134.f0;
    int32_t *l_185 = &g_134.f0;
    int32_t *l_186 = &g_86;
    int32_t *l_187 = &g_3;
    int32_t *l_188 = &g_3;
    int32_t *l_189 = &g_134.f0;
    int32_t *l_190 = &g_86;
    int32_t *l_191 = &l_149[2][0];
    uint64_t l_192 = 0x4B86BEE1F3E7179ALL;
    int i, j;
    for (g_3 = 0; (g_3 >= 0); g_3 -= 1)
    { 
        uint8_t *l_139 = &g_76[0];
        union U4 l_146[2][3] = {{{0U},{9U},{9U}},{{0U},{9U},{9U}}};
        int32_t l_150 = (-5);
        int32_t l_151 = 3;
        int32_t l_152 = (-6);
        int8_t l_155 = 0;
        uint64_t l_158 = 0U;
        uint8_t l_168 = 0x09;
        int i, j;
        for (g_4 = 0; (g_4 <= 0); g_4 += 1)
        { 
            int32_t **l_18 = (void*)0;
            union U1 **l_135 = (void*)0;
            union U1 **l_136 = &g_133;
            for (g_5 = 0; (g_5 <= 0); g_5 += 1)
            { 
                int32_t *l_6 = &g_7;
                uint64_t l_103 = 0xBE94C56F68EFB8CCLL;
                int32_t **l_131[3][4] = {{&l_6,&g_59,&g_59,&l_6},{&g_59,&l_6,&g_59,&g_59},{&g_59,&g_59,&l_6,&g_59}};
                int i, j;
                (*l_6) |= (g_2[g_3] = 0x0657689A);
            }
            (*l_136) = g_133;
        }
    }
    ++l_192;
    (*l_132) ^= (g_197 ^ ((((*l_188) = (*l_185)) && (safe_rshift_func_int64_t_s_s((g_61[1][2] && (*l_181)), 16))) <= 0x495F73CD));
    return (*l_184);
}

static int32_t ** const  func_8(int32_t * p_9, uint32_t  p_10, int8_t  p_11, int64_t  p_12, uint64_t  p_13)
{ 
    uint64_t l_108[3][4] = {{1U,1U,1U,1U},{1U,1U,1U,1U},{1U,1U,1U,1U}};
    uint32_t *l_115 = &g_35;
    int32_t l_121[3];
    int32_t l_123 = 0xE2A70171;
    int32_t l_125 = 8;
    uint64_t l_127 = 0x56C3BA57B682B91DLL;
    int32_t ** const l_130 = (void*)0;
    int i, j;
    for (i = 0; i < 3; i++)
        l_121[i] = 0xFB27AD41;
    if ((*g_59))
    { 
        int32_t *l_107 = &g_2[0];
        int32_t **l_106 = &l_107;
        g_7 ^= (safe_lshift_func_uint16_t_u_u((0xE8 || (((*g_58) = (*g_58)) != ((*l_106) = (void*)0))), g_61[2][2]));
    }
    else
    { 
        l_108[0][3]++;
    }
    for (p_12 = 2; (p_12 >= 0); p_12 -= 1)
    { 
        int32_t l_116[1];
        int i;
        for (i = 0; i < 1; i++)
            l_116[i] = (-1);
        (*p_9) &= (l_116[0] = ((**g_58) = (safe_add_func_uint8_t_u_u((safe_sub_func_uint64_t_u_u(g_76[p_12], (18446744073709551615U && (l_115 != (void*)0)))), l_108[0][3]))));
        for (g_25 = 0; (g_25 <= 0); g_25 += 1)
        { 
            int i;
            for (g_64 = 0; (g_64 <= 2); g_64 += 1)
            { 
                return &g_59;
            }
            g_2[g_25] |= 1;
        }
    }
    for (g_102 = 0; (g_102 < 26); g_102++)
    { 
        int32_t *l_119 = &g_7;
        int32_t *l_120[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
        int64_t l_122 = 0xAE1FFEC589AC8C39LL;
        int16_t l_124 = 0x1AAA;
        int32_t l_126 = (-1);
        int i;
        l_127--;
        (*p_9) ^= ((**g_58) = 0x327B72E6);
    }
    return l_130;
}

static int8_t  func_14(int32_t * p_15, int32_t ** p_16, int32_t ** p_17)
{ 
    int32_t *l_19 = &g_7;
    int32_t *l_20 = &g_7;
    int32_t *l_21 = &g_7;
    int32_t *l_22 = &g_7;
    int32_t l_23 = 0x0CAAD901;
    int32_t *l_24[1][2];
    union U3 l_31 = {{0xF2AB,255U}};
    uint32_t *l_34 = &g_35;
    uint16_t l_36 = 0xA1E1;
    const uint64_t l_37 = 1U;
    int32_t l_87 = 0x5E1620D8;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_24[i][j] = &l_23;
    }
    (*l_19) = (l_19 != &g_7);
    ++g_25;
    if (((safe_rshift_func_uint16_t_u_s((!0x87A9B65C), (l_31 , (((((*l_19) = (safe_mod_func_int16_t_s_s((((*l_34) = ((((*l_22) > g_5) && 0x087B332E) <= g_4)) && g_2[0]), g_7))) | g_25) && (*l_20)) < l_36)))) & l_37))
    { 
        int64_t l_48[3][2] = {{0x6C9F93B76F056E99LL,(-2)},{(-2),0x6C9F93B76F056E99LL},{(-2),(-2)}};
        int32_t l_71 = 7;
        int32_t l_74 = 3;
        int32_t l_75 = 0;
        const union U3 l_80 = {{0x5BD1,255U}};
        int i, j;
        (*l_22) = 0x6F9F1111;
        if (((safe_rshift_func_uint32_t_u_s((((safe_lshift_func_int64_t_s_u((safe_mul_func_uint32_t_u_u(((*l_21) < (safe_add_func_int64_t_s_s((safe_div_func_int32_t_s_s(g_5, ((&g_5 == &g_5) ^ (*l_20)))), (*l_19)))), 0U)), 55)) ^ l_48[0][1]) > (*l_20)), l_48[2][1])) & 0xFC5446ED))
        { 
            int16_t l_55 = 0x020A;
            int32_t *l_68 = &g_3;
            int32_t l_72 = 0x0213A2BB;
            int32_t l_73[2][2] = {{(-8),(-8)},{(-8),(-8)}};
            const struct S0 l_79 = {0xE375,0xFD};
            int i, j;
            if ((((8 != (safe_mod_func_uint16_t_u_u(0U, (*l_20)))) <= (*l_22)) && 0))
            { 
                int32_t **l_51 = (void*)0;
                int32_t **l_52 = &l_20;
                int8_t *l_60[3][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0}};
                int16_t *l_62 = (void*)0;
                int16_t *l_63 = &g_64;
                int i, j;
                (*l_52) = &l_23;
                (*g_59) = ((safe_div_func_int16_t_s_s(((*l_63) &= (l_55 && (g_61[2][2] = (safe_lshift_func_uint32_t_u_u((((&p_15 == (g_58 = g_58)) <= g_7) , (*l_22)), 12))))), g_35)) || 0xD959);
            }
            else
            { 
                int8_t l_69 = 1;
                int32_t l_70[1][4] = {{0xB24C01D5,0xB24C01D5,0xB24C01D5,0xB24C01D5}};
                int i, j;
                (*l_21) &= (safe_div_func_int8_t_s_s((~(((*l_34) ^= ((void*)0 == l_68)) > 0xE8010CA1)), 0xF4));
                --g_76[0];
            }
            l_75 &= (((l_31.f0 , (((l_79 , 0U) , l_80) , (*g_59))) & (*l_22)) > (*l_68));
        }
        else
        { 
            const int16_t l_83 = 0xCF1A;
            int32_t *l_84 = &l_75;
            for (g_25 = 4; (g_25 == 28); g_25++)
            { 
                if (l_83)
                    break;
            }
            l_84 = ((*g_58) = (*g_58));
        }
    }
    else
    { 
        int32_t l_85 = (-1);
        int32_t l_88 = 0x2BF2F772;
        int32_t l_90 = 0xDB194A10;
        union U3 *l_101 = &l_31;
        union U3 **l_100 = &l_101;
        ++g_91;
        l_85 ^= (safe_div_func_int8_t_s_s((l_90 ^ (((safe_mod_func_int16_t_s_s(l_88, (((safe_rshift_func_int8_t_s_s((((((0 > 0x828E4EA47852D388LL) , (*l_19)) , (*l_22)) == (*l_22)) <= (-1)), l_90)) , l_88) ^ 0U))) <= l_90) == g_35)), (*l_22)));
        (*l_100) = &l_31;
    }
    return (*l_22);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_61[i][j], "g_61[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_64, "g_64", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_76[i], "g_76[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_86, "g_86", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    transparent_crc(g_91, "g_91", print_hash_value);
    transparent_crc(g_102, "g_102", print_hash_value);
    transparent_crc(g_134.f0, "g_134.f0", print_hash_value);
    transparent_crc(g_197, "g_197", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
