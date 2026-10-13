/* Csmith 0cdc710315cf seed 322, flattened by scripts/csmith_flatten.py */
// Options:   --seed 322 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
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
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
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
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
static int32_t
safe_unary_minus_func_int32_t_s(int32_t si )
{
  return
    (si==(-2147483647-1)) ?
    ((si)) :
    -si;
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
   uint16_t  f0;
   const int32_t  f1;
};

static int8_t g_15 = 0x9F;
static int32_t g_21 = (-9);
static struct S0 g_23 = {65526U,3};
static struct S0 *g_22[1] = {&g_23};
static int32_t *g_26 = &g_21;
static int32_t **g_25 = &g_26;
static uint64_t g_54 = 0xFAF230988A21DEC7LL;
static uint32_t g_58 = 4294967295U;
static uint64_t g_62 = 18446744073709551612U;
static uint16_t g_64[2][2] = {{65535U,65535U},{65535U,65535U}};
static uint32_t g_120 = 4294967286U;
static uint64_t g_123 = 18446744073709551615U;

static int32_t  func_1(void);
static int32_t  func_2(const int16_t  p_3);
static uint32_t  func_9(uint32_t  p_10);

static int32_t  func_1(void)
{ 
    uint64_t l_4[4] = {18446744073709551609U,18446744073709551609U,18446744073709551609U,18446744073709551609U};
    uint64_t *l_74 = (void*)0;
    uint64_t *l_75 = &l_4[2];
    int32_t *l_121 = (void*)0;
    int32_t *l_122[1][1];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_122[i][j] = &g_21;
    }
    g_123 = func_2(((((*l_75) = (l_4[0] , (safe_rshift_func_int64_t_s_s((safe_sub_func_uint32_t_u_u(func_9(((safe_lshift_func_int32_t_s_s(((safe_lshift_func_uint16_t_u_s(g_15, (safe_rshift_func_int16_t_s_s((safe_div_func_uint16_t_u_u(l_4[3], l_4[0])), l_4[3])))) && 9U), l_4[0])) != g_15)), l_4[0])), 32)))) > 0x13CF365AED0E3492LL) ^ g_23.f1));
    return g_21;
}

static int32_t  func_2(const int16_t  p_3)
{ 
    uint16_t l_83 = 0xE309;
    struct S0 l_96[1][1] = {{{0U,0}}};
    int32_t l_97 = 0xFB1927A6;
    uint8_t l_111 = 254U;
    int i, j;
    for (g_21 = 0; (g_21 <= (-22)); g_21--)
    { 
        uint32_t l_78 = 0xDC621C65;
        const int32_t *l_79[1][1];
        const int32_t **l_80 = &l_79[0][0];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_79[i][j] = &g_21;
        }
        if (l_78)
            break;
        (*l_80) = l_79[0][0];
    }
    if ((g_62 | ((safe_add_func_int8_t_s_s(3, (&g_26 == (void*)0))) <= l_83)))
    { 
        uint32_t *l_108[4];
        int32_t l_109[1];
        int32_t *l_110[4][2] = {{&l_97,&l_97},{&l_97,&l_97},{&l_97,&l_97},{&l_97,&l_97}};
        int i, j;
        for (i = 0; i < 4; i++)
            l_108[i] = (void*)0;
        for (i = 0; i < 1; i++)
            l_109[i] = 0xA3AC23FC;
        l_97 |= (safe_sub_func_uint32_t_u_u(l_83, (p_3 , (safe_mul_func_uint64_t_u_u((safe_sub_func_int64_t_s_s(((safe_rshift_func_int32_t_s_s((safe_sub_func_uint32_t_u_u(((safe_mul_func_int16_t_s_s((l_96[0][0] , l_96[0][0].f1), l_83)) , 4294967295U), 0xC75790E3)), 31)) , 1), g_64[1][1])), 0xE9E2562E968F5B28LL)))));
        for (g_54 = 0; (g_54 <= 0); g_54 += 1)
        { 
            uint64_t l_101 = 0x5ABFD0B78A1103C6LL;
            for (g_23.f0 = 0; (g_23.f0 <= 1); g_23.f0 += 1)
            { 
                int32_t *l_98 = &l_97;
                int32_t *l_99 = &g_21;
                int32_t *l_100 = &g_21;
                int i, j;
                g_21 = 9;
                if (g_64[(g_54 + 1)][g_23.f0])
                    continue;
                l_101++;
            }
            g_21 = (((safe_div_func_uint64_t_u_u((p_3 | (((safe_div_func_uint64_t_u_u((((l_108[2] = (void*)0) != (void*)0) & 0x079F8D1B), l_96[0][0].f0)) >= p_3) > p_3)), 18446744073709551610U)) || 0x37D68548) >= g_23.f1);
            if (l_97)
                continue;
        }
        l_111++;
    }
    else
    { 
        int8_t l_114 = 0x7D;
        struct S0 l_115 = {0x616C,0x9CF3F9EA};
        int8_t *l_116 = &g_15;
        l_97 &= (((l_114 & (((*l_116) = (l_114 <= ((l_115 , (void*)0) == (void*)0))) ^ p_3)) ^ g_21) , 3);
        for (g_21 = 0; (g_21 >= 0); g_21 -= 1)
        { 
            uint64_t *l_119 = &g_62;
            g_120 = ((18446744073709551612U < (g_54 = (safe_mod_func_uint64_t_u_u(18446744073709551615U, ((*l_119) = (g_62 || g_54)))))) , 1);
        }
    }
    return p_3;
}

static uint32_t  func_9(uint32_t  p_10)
{ 
    int32_t *l_20 = &g_21;
    struct S0 **l_24 = &g_22[0];
    int32_t l_63 = 0x6F7EBA05;
    (*l_20) = 4;
    (*l_24) = g_22[0];
    for (g_15 = 0; (g_15 <= 0); g_15 += 1)
    { 
        int32_t **l_27[4][2] = {{&l_20,&g_26},{&g_26,&l_20},{&g_26,&g_26},{&l_20,&g_26}};
        int i, j;
        if (g_23.f0)
        { 
            uint64_t l_30 = 18446744073709551613U;
            if (((0xD2044653 > g_21) | (1 | 247U)))
            { 
                l_27[3][0] = g_25;
            }
            else
            { 
                int16_t l_28 = 0xC6F7;
                int32_t l_29 = 1;
                l_30--;
            }
            if (((*l_20) = (g_23.f1 || g_23.f0)))
            { 
                if ((*g_26))
                    break;
                (*l_20) = (safe_lshift_func_uint8_t_u_u(((((((safe_mod_func_int32_t_s_s((((void*)0 == &l_20) != (g_23.f0 , p_10)), p_10)) , p_10) == (-4)) & (-8)) , p_10) <= 0), g_23.f0));
            }
            else
            { 
                (*l_20) = (safe_div_func_int32_t_s_s((*l_20), (((safe_div_func_uint16_t_u_u(g_21, (*l_20))) <= 65535U) | 0xAE0512ED)));
                (*g_25) = (void*)0;
                return p_10;
            }
        }
        else
        { 
            int8_t l_52 = 3;
            if ((**g_25))
            { 
                (**g_25) = 0x3B020B2D;
            }
            else
            { 
                uint64_t *l_53 = &g_54;
                uint32_t *l_56 = (void*)0;
                uint32_t *l_57 = &g_58;
                int32_t l_61 = 8;
                g_62 ^= (safe_sub_func_uint32_t_u_u((safe_unary_minus_func_int64_t_s(((safe_lshift_func_uint32_t_u_u(((safe_div_func_uint16_t_u_u((safe_lshift_func_uint32_t_u_s((((((safe_add_func_int16_t_s_s((g_21 >= (((*l_53) &= l_52) | (safe_unary_minus_func_int32_t_s((((((*l_57)--) , 0xBFBD) <= g_15) & 0x2FC308456A9E5FAFLL))))), (-2))) <= (*l_20)) , (-7)) || (*l_20)) >= 65535U), (*g_26))), g_23.f0)) & 0xBEC16066), l_61)) > g_23.f1))), (*g_26)));
                g_64[1][1]--;
            }
            for (g_62 = 0; (g_62 <= 0); g_62 += 1)
            { 
                int32_t l_67 = 0;
                uint64_t l_68 = 0xFCA599F3DF38E6A2LL;
                int i, j;
                --l_68;
                return (*l_20);
            }
        }
        (*l_20) = (safe_add_func_int8_t_s_s((~(((void*)0 != &l_63) <= (0xB9 ^ p_10))), p_10));
    }
    return p_10;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_23.f0, "g_23.f0", print_hash_value);
    transparent_crc(g_23.f1, "g_23.f1", print_hash_value);
    transparent_crc(g_54, "g_54", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_64[i][j], "g_64[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_120, "g_120", print_hash_value);
    transparent_crc(g_123, "g_123", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
