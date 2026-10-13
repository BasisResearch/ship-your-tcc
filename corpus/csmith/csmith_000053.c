/* Csmith 0cdc710315cf seed 53, flattened by scripts/csmith_flatten.py */
// Options:   --seed 53 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
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
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
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
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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

union U0 {
   int32_t  f0;
   uint16_t  f1;
   uint64_t  f2;
};

union U1 {
   int8_t  f0;
   int64_t  f1;
   const uint16_t  f2;
};

union U2 {
   uint64_t  f0;
   int8_t  f1;
};

static int32_t g_7 = 0xAF5AF8B5;
static int32_t *g_6 = &g_7;
static const union U2 g_13 = {0xF6A715CC48B1647CLL};
static int64_t g_22 = 0x52487223B16AAC89LL;
static uint32_t g_25 = 0x829CBC21;
static uint32_t g_38 = 4294967295U;
static int16_t g_64 = (-5);
static uint64_t g_65[4] = {0xA916A8E8C0996BD0LL,0xA916A8E8C0996BD0LL,0xA916A8E8C0996BD0LL,0xA916A8E8C0996BD0LL};
static uint64_t g_70 = 0x1FBFE3ACA477CAF8LL;
static const int32_t g_85 = (-1);
static uint16_t g_95 = 0x2F80;
static uint16_t g_105 = 0xE5BE;
static uint16_t g_107 = 0xA5A2;
static int16_t g_118 = 7;
static int8_t g_119 = (-7);
static uint64_t g_120 = 0x9044C330A9AF7D07LL;
static uint8_t g_141 = 0xAF;
static union U1 g_147[2][3] = {{{0x3B},{0x3B},{0xE0}},{{0x3B},{0x3B},{0xE0}}};
static uint32_t *g_156 = (void*)0;
static uint32_t **g_155[4] = {&g_156,&g_156,&g_156,&g_156};

static int64_t  func_1(void);
static int32_t * const  func_3(int32_t * p_4, int32_t * p_5);
static int32_t * func_8(int32_t * p_9, uint8_t  p_10, const union U2  p_11, int16_t  p_12);

static int64_t  func_1(void)
{ 
    const int16_t l_2 = 0xC23A;
    int32_t *l_80 = &g_7;
    uint16_t *l_94 = &g_95;
    uint16_t *l_104 = &g_105;
    uint16_t *l_106 = &g_107;
    uint8_t l_142 = 0xE2;
    int32_t l_153 = 0;
    if ((0xAD >= l_2))
    { 
        int32_t **l_76 = &g_6;
        (*l_76) = func_3(g_6, func_8(&g_7, g_7, g_13, g_13.f0));
    }
    else
    { 
        int32_t l_78 = 1;
        const int32_t *l_84 = &g_85;
        const int32_t **l_83 = &l_84;
lbl_90:
        for (g_64 = 0; (g_64 <= 3); g_64 += 1)
        { 
            int64_t *l_79 = &g_22;
            for (g_25 = 0; (g_25 <= 3); g_25 += 1)
            { 
                int i;
                (*g_6) = 0xD7EC95B9;
                return g_65[g_64];
            }
            (*g_6) = ((l_2 > (g_13 , (+(((*l_79) &= l_78) < g_70)))) == g_25);
        }
        if ((((*l_83) = func_8(l_80, (safe_mul_func_int8_t_s_s((g_70 ^ (g_64 = (0 & (*l_80)))), g_65[2])), g_13, l_78)) == &g_85))
        { 
            int32_t *l_86[1][3];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 3; j++)
                    l_86[i][j] = &g_7;
            }
            (*l_83) = l_86[0][2];
        }
        else
        { 
            uint8_t l_87 = 0xAB;
            return l_87;
        }
        for (g_25 = (-11); (g_25 > 21); ++g_25)
        { 
            for (g_64 = 3; (g_64 >= 0); g_64 -= 1)
            { 
                int32_t **l_91 = &l_80;
                if (g_25)
                    goto lbl_90;
                (*l_91) = func_3(&g_7, l_80);
                if (g_25)
                    goto lbl_90;
            }
        }
    }
    if ((g_22 == ((safe_add_func_uint16_t_u_u(((*l_94)++), (safe_div_func_uint16_t_u_u(((*l_106) = ((*l_104) = ((safe_rshift_func_int16_t_s_u((*l_80), (safe_add_func_uint8_t_u_u((g_70 <= g_64), 0xB4)))) , (*l_80)))), g_65[2])))) == g_22)))
    { 
        int32_t * const l_108 = (void*)0;
        int32_t **l_109[1][1];
        int32_t **l_110 = (void*)0;
        int32_t **l_111 = &g_6;
        uint64_t l_116 = 0x64C99091BB24BDB3LL;
        int8_t l_117 = (-6);
        int32_t *l_129 = &g_7;
        union U1 l_130 = {-1};
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_109[i][j] = &l_80;
        }
        (*l_111) = l_108;
        if (((safe_add_func_uint32_t_u_u((4294967292U || ((safe_lshift_func_int8_t_s_u(g_22, (((0x10211DA0 & l_116) & g_65[0]) || g_70))) , l_117)), g_70)) || (*l_80)))
        { 
            union U0 l_125 = {0xC731693D};
            g_120++;
            for (g_119 = 0; (g_119 <= (-30)); g_119 = safe_sub_func_uint8_t_u_u(g_119, 1))
            { 
                (*l_80) &= (l_125 , ((safe_mod_func_uint8_t_u_u(g_107, g_65[2])) <= l_125.f0));
            }
            for (g_120 = 0; (g_120 <= 3); g_120 += 1)
            { 
                const int8_t l_128 = 0xBB;
                (*l_80) = l_128;
                (*l_111) = l_129;
            }
        }
        else
        { 
            int64_t *l_139 = &l_130.f1;
            int64_t *l_140 = &l_130.f1;
            const int32_t l_150 = 0;
            union U0 l_151[4][3] = {{{0x44EFACDB},{0x51B32B03},{0x51B32B03}},{{0x44EFACDB},{0x51B32B03},{0x51B32B03}},{{0x44EFACDB},{0x51B32B03},{0x51B32B03}},{{0x44EFACDB},{0x51B32B03},{0x51B32B03}}};
            int32_t l_152 = (-3);
            union U0 *l_154 = (void*)0;
            int i, j;
            if (((*l_80) = (l_130 , (safe_sub_func_uint32_t_u_u(0xC4C452B0, (((*l_139) = ((((safe_rshift_func_uint8_t_u_u((safe_lshift_func_uint32_t_u_u(((g_141 &= (safe_rshift_func_int8_t_s_s(g_22, 2))) , (*l_80)), (*l_80))), 4)) , l_142) , g_7) , (-1))) >= g_7))))))
            { 
                int8_t l_143 = 0xC4;
                int32_t l_144 = 0x455FDE6E;
                l_144 &= l_143;
                l_144 |= (g_13 , (safe_add_func_uint8_t_u_u(g_7, ((*l_80) & g_105))));
            }
            else
            { 
                (*l_129) = ((void*)0 == &g_22);
                l_153 &= (g_147[1][2] , (l_152 ^= (safe_mod_func_uint64_t_u_u(l_150, (l_151[1][2] , (*l_80))))));
            }
            l_154 = &l_151[1][2];
        }
        g_6 = &l_153;
    }
    else
    { 
        g_155[0] = (void*)0;
        return (*l_80);
    }
    return g_13.f0;
}

static int32_t * const  func_3(int32_t * p_4, int32_t * p_5)
{ 
    int32_t *l_68 = &g_7;
    int32_t *l_69[4][3] = {{&g_7,&g_7,&g_7},{(void*)0,(void*)0,(void*)0},{&g_7,&g_7,&g_7},{(void*)0,(void*)0,(void*)0}};
    uint8_t l_73 = 0U;
    int i, j;
    g_70++;
    --l_73;
    return &g_7;
}

static int32_t * func_8(int32_t * p_9, uint8_t  p_10, const union U2  p_11, int16_t  p_12)
{ 
    union U2 l_14[3] = {{18446744073709551609U},{18446744073709551609U},{18446744073709551609U}};
    int32_t l_15 = 5;
    int i;
    for (p_12 = 0; (p_12 <= 2); p_12 += 1)
    { 
        int32_t *l_16 = (void*)0;
        int32_t *l_17 = &l_15;
        int32_t *l_18 = (void*)0;
        int32_t *l_19 = &l_15;
        int32_t *l_20 = &g_7;
        int32_t *l_21 = &l_15;
        int32_t *l_23 = &l_15;
        int32_t *l_24[1];
        int i;
        for (i = 0; i < 1; i++)
            l_24[i] = (void*)0;
        ++g_25;
    }
    if ((safe_mul_func_int16_t_s_s(l_15, (!(l_15 ^ (safe_mul_func_int8_t_s_s((-9), 0x3E)))))))
    { 
        uint8_t l_47 = 4U;
        for (g_25 = (-30); (g_25 == 27); ++g_25)
        { 
            uint32_t *l_37 = &g_38;
            int32_t *l_41 = &g_7;
            int32_t **l_42 = &l_41;
            int32_t **l_43 = (void*)0;
            int64_t l_46 = 0x70D8AC507B3925B9LL;
            l_47 = (((safe_div_func_int32_t_s_s((((((*l_37)--) || ((((p_9 = ((*l_42) = l_41)) == &g_7) <= ((safe_mod_func_uint8_t_u_u((0xFBCA ^ 0x6E16), l_15)) >= l_46)) || g_13.f0)) > p_11.f0) ^ 0x29ADBD3D), p_10)) == 6) == g_25);
        }
    }
    else
    { 
        const uint64_t l_57 = 0xC22F747EAB1A6906LL;
        int64_t *l_58[3];
        int32_t l_59 = 0x8F381C4E;
        int32_t *l_60 = &g_7;
        int32_t *l_61 = (void*)0;
        int32_t *l_62 = &l_15;
        int32_t *l_63[2];
        int i;
        for (i = 0; i < 3; i++)
            l_58[i] = (void*)0;
        for (i = 0; i < 2; i++)
            l_63[i] = &l_15;
        l_15 = ((-10) < ((g_22 = (l_59 = (safe_mul_func_uint32_t_u_u((+(safe_mul_func_int16_t_s_s((((safe_lshift_func_int64_t_s_u(7, ((safe_lshift_func_int32_t_s_u(0x3335A02A, 16)) <= l_57))) > p_11.f0) >= (-3)), 1))), p_10)))) == l_15));
        g_65[0]--;
    }
    return p_9;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_13.f0, "g_13.f0", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_65[i], "g_65[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_95, "g_95", print_hash_value);
    transparent_crc(g_105, "g_105", print_hash_value);
    transparent_crc(g_107, "g_107", print_hash_value);
    transparent_crc(g_118, "g_118", print_hash_value);
    transparent_crc(g_119, "g_119", print_hash_value);
    transparent_crc(g_120, "g_120", print_hash_value);
    transparent_crc(g_141, "g_141", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_147[i][j].f0, "g_147[i][j].f0", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
