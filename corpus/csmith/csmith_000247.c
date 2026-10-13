/* Csmith 0cdc710315cf seed 247, flattened by scripts/csmith_flatten.py */
// Options:   --seed 247 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   const uint8_t  f0;
   const int64_t  f1;
};

union U2 {
   int32_t  f0;
   int64_t  f1;
   int16_t  f2;
};

union U4 {
   int32_t  f0;
   int32_t  f1;
};

static int32_t g_10 = (-1);
static uint16_t g_19 = 0xB445;
static uint8_t g_23[3] = {0x65,0x65,0x65};
static int32_t g_26 = 0x1D686A37;
static int32_t *g_25 = &g_26;
static union U2 g_27[2] = {{0xA2F617A9},{0xA2F617A9}};
static union U0 g_33 = {0x4F};
static uint64_t g_45 = 18446744073709551608U;
static uint16_t g_47 = 0x43D4;
static uint64_t g_50 = 0U;
static union U4 g_52 = {0x0A341A46};
static union U4 *g_51[4] = {&g_52,&g_52,&g_52,&g_52};
static uint32_t g_89 = 2U;
static uint8_t g_136 = 0U;

static union U2  func_1(void);
static int32_t * func_2(uint64_t  p_3, union U4  p_4, int32_t * p_5, union U2  p_6, union U0  p_7);
static int32_t * const  func_29(union U4  p_30);

static union U2  func_1(void)
{ 
    uint16_t *l_17 = (void*)0;
    uint16_t *l_18 = &g_19;
    uint32_t l_22 = 0U;
    union U4 l_24 = {0xA42BBD4A};
    union U0 l_28 = {1U};
    int32_t **l_140 = &g_25;
    union U2 l_141 = {-1};
    (*l_140) = func_2((safe_mul_func_int32_t_s_s((g_10 = g_10), (safe_mod_func_int64_t_s_s(((safe_lshift_func_uint32_t_u_u((safe_mod_func_int32_t_s_s((((*l_18)--) || (((4294967295U < l_22) ^ 0x37C5B1510AFBD13DLL) || 0x5378470ED99EA7EBLL)), l_22)), g_23[1])) , l_22), g_23[1])))), l_24, g_25, g_27[0], l_28);
    (*l_140) = (*l_140);
    return l_141;
}

static int32_t * func_2(uint64_t  p_3, union U4  p_4, int32_t * p_5, union U2  p_6, union U0  p_7)
{ 
    int32_t *l_139[3][1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_139[i][j] = (void*)0;
    }
    l_139[0][0] = func_29(p_4);
    return p_5;
}

static int32_t * const  func_29(union U4  p_30)
{ 
    const uint8_t l_34 = 3U;
    const uint16_t *l_48 = (void*)0;
    int8_t l_57 = (-9);
    const int32_t *l_58 = &g_10;
    uint16_t l_78 = 65535U;
    int32_t **l_82 = &g_25;
    int64_t *l_85[4];
    int32_t l_86 = 0xFA87DC7F;
    int8_t *l_88[4];
    uint16_t l_90 = 0x69AA;
    int i;
    for (i = 0; i < 4; i++)
        l_85[i] = &g_27[0].f1;
    for (i = 0; i < 4; i++)
        l_88[i] = &l_57;
    if ((safe_div_func_int16_t_s_s((((g_33 , g_33.f0) >= p_30.f0) <= l_34), g_23[1])))
    { 
        int32_t *l_43 = (void*)0;
        for (g_10 = 0; (g_10 <= (-21)); --g_10)
        { 
            uint16_t *l_39 = (void*)0;
            uint16_t *l_40 = &g_19;
            uint64_t *l_44 = &g_45;
            uint16_t *l_46 = &g_47;
            uint64_t *l_49 = &g_50;
            const int32_t **l_59 = (void*)0;
            const int32_t **l_60 = &l_58;
            if (((((*l_40)--) , ((*l_44) ^= (&g_26 == l_43))) | ((*l_49) = (((((*l_46) = (((4U || 4294967295U) & g_27[0].f0) <= p_30.f0)) , l_40) == l_48) <= l_34))))
            { 
                g_51[2] = g_51[2];
                (*g_25) &= ((safe_sub_func_uint8_t_u_u((safe_lshift_func_uint16_t_u_u(l_57, l_34)), g_47)) | p_30.f0);
            }
            else
            { 
                (*g_25) ^= 0x30F0C867;
            }
            (*l_60) = l_58;
        }
    }
    else
    { 
        int32_t l_72 = (-1);
        for (l_57 = 0; (l_57 >= 28); l_57 = safe_add_func_uint64_t_u_u(l_57, 4))
        { 
            union U4 l_69[4] = {{0x7D86C3CE},{0x7D86C3CE},{0x7D86C3CE},{0x7D86C3CE}};
            int32_t *l_74 = (void*)0;
            int32_t *l_75 = (void*)0;
            int32_t *l_76 = &l_69[1].f0;
            int32_t *l_77[4];
            int32_t **l_81 = (void*)0;
            int i;
            for (i = 0; i < 4; i++)
                l_77[i] = &l_72;
            for (g_26 = 0; (g_26 != (-13)); g_26--)
            { 
                int32_t *l_73 = &g_52.f0;
                if (p_30.f0)
                    break;
                (*l_73) |= (safe_add_func_uint32_t_u_u((((((safe_mul_func_int32_t_s_s(((l_69[1] , (safe_add_func_int16_t_s_s(g_10, p_30.f0))) >= l_72), 1U)) ^ l_69[1].f0) == p_30.f0) ^ l_69[1].f0) != g_27[0].f0), p_30.f0));
                if (l_72)
                    break;
            }
            l_78++;
            l_75 = l_75;
        }
    }
    (*l_82) = &g_10;
    if ((((*g_25) ^= 0) & (l_90 = (g_52.f1 = (safe_add_func_int8_t_s_s((g_89 &= ((l_86 ^= p_30.f0) < ((~(((18446744073709551607U > p_30.f0) ^ p_30.f0) & 0xE87A905B)) > p_30.f0))), p_30.f0))))))
    { 
        uint64_t l_111 = 18446744073709551608U;
        uint64_t l_114[3][1];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_114[i][j] = 6U;
        }
        for (g_26 = 0; (g_26 > 15); g_26 = safe_add_func_uint8_t_u_u(g_26, 4))
        { 
            return &g_10;
        }
        if ((0x7F0BBFFD >= (safe_rshift_func_uint8_t_u_s((safe_rshift_func_uint16_t_u_s(0x13E6, (safe_add_func_int8_t_s_s((safe_sub_func_int16_t_s_s(((~((void*)0 == &l_57)) ^ (*l_58)), 0)), 0x5B)))), g_45))))
        { 
            int32_t *l_102 = &g_27[0].f0;
            uint16_t *l_103[2][1];
            int32_t l_104 = 0x783B0794;
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_103[i][j] = &g_47;
            }
            (*l_102) = ((*g_25) &= p_30.f0);
            (*g_25) &= ((*l_102) &= ((g_47 = (g_19--)) ^ 65535U));
            for (l_86 = 0; (l_86 <= 3); l_86 += 1)
            { 
                uint16_t **l_107 = &l_103[1][0];
                int8_t *l_109 = &l_57;
                l_107 = &l_103[1][0];
                (**l_82) &= (+((void*)0 == l_109));
            }
        }
        else
        { 
            int32_t *l_110[2];
            int i;
            for (i = 0; i < 2; i++)
                l_110[i] = &l_86;
            l_111--;
            l_114[1][0]--;
        }
    }
    else
    { 
        int32_t l_125 = (-1);
        int32_t l_126 = 0;
        int32_t l_129 = 0x40900504;
        const int32_t *l_133 = (void*)0;
        int32_t l_135 = 3;
        for (g_47 = 0; (g_47 <= 1); g_47 += 1)
        { 
            int32_t *l_117 = &g_10;
            int32_t *l_118 = &g_52.f1;
            int32_t *l_119 = &g_52.f0;
            int32_t l_120 = 0xB372D1EC;
            int32_t *l_121 = &g_26;
            int32_t *l_122 = &g_10;
            int32_t *l_123 = &l_86;
            int32_t *l_124 = &l_86;
            int32_t *l_127 = (void*)0;
            int32_t *l_128[2];
            uint16_t l_130 = 0x9CA4;
            int i;
            for (i = 0; i < 2; i++)
                l_128[i] = &g_52.f1;
            (*l_82) = (*l_82);
            --l_130;
            for (l_125 = 1; (l_125 >= 0); l_125 -= 1)
            { 
                int32_t l_134[4][2] = {{1,1},{1,1},{1,1},{1,1}};
                int i, j;
                l_129 = (((l_133 == (void*)0) , &g_50) == &g_50);
                ++g_136;
            }
        }
    }
    return (*l_82);
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_23[i], "g_23[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_26, "g_26", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_27[i].f0, "g_27[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_33.f0, "g_33.f0", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    transparent_crc(g_52.f1, "g_52.f1", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    transparent_crc(g_136, "g_136", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
