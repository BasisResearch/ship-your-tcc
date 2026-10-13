/* Csmith 0cdc710315cf seed 42, flattened by scripts/csmith_flatten.py */
// Options:   --seed 42 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
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
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
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
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   const uint32_t  f0;
   int64_t  f1;
};

union U1 {
   const uint32_t  f0;
   const int8_t  f1;
};

static int32_t g_2 = 0x3CDF98A5;
static int32_t g_6 = 0xDA622A14;
static union U0 g_8 = {4294967294U};
static int8_t g_18 = 1;
static int32_t g_21 = 0x090AE735;
static int32_t g_40 = 0x7F464F61;
static int8_t g_41 = (-2);
static int32_t g_45 = 0xC2DA50E2;
static int8_t *g_54 = &g_41;
static int8_t **g_53 = &g_54;
static int16_t g_65 = 0;
static uint8_t g_67[2] = {0x48,0x48};
static int32_t *g_76 = &g_40;
static int32_t g_87[4][4] = {{(-6),(-6),(-6),(-6)},{(-6),(-6),(-6),(-6)},{(-6),(-6),(-6),(-6)},{(-6),(-6),(-6),(-6)}};
static uint32_t g_129 = 4294967295U;

static const int8_t  func_1(void);
static uint64_t  func_9(union U1  p_10, uint16_t  p_11, union U1  p_12);
static int32_t  func_23(int32_t * p_24);

static const int8_t  func_1(void)
{ 
    union U1 l_13 = {0x592758E9};
    int32_t l_105 = (-6);
    int32_t l_108 = (-7);
    int32_t l_110 = 0x04AF86A2;
    int32_t l_111 = 3;
    int32_t l_113 = (-5);
    int32_t l_115 = 0x04F2ECF3;
    int32_t l_117 = 1;
    int32_t l_118 = 0xD3BA7024;
    int16_t l_123 = (-10);
    uint32_t *l_128[1];
    int64_t l_130 = 0xF4BCE8A264C33588LL;
    int32_t *l_133[1][2];
    int i, j;
    for (i = 0; i < 1; i++)
        l_128[i] = &g_129;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_133[i][j] = &g_2;
    }
    for (g_2 = 0; (g_2 != 0); g_2 = safe_add_func_uint64_t_u_u(g_2, 8))
    { 
        uint64_t l_5[3];
        int32_t l_104[3][4];
        int32_t l_112 = 0xE0213BFE;
        int64_t l_114 = (-1);
        int64_t l_116[1][3];
        uint8_t l_119 = 0x0E;
        int i, j;
        for (i = 0; i < 3; i++)
            l_5[i] = 0xC3FBB4EBB0CB8CE3LL;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 4; j++)
                l_104[i][j] = 0x1AF0014B;
        }
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_116[i][j] = 0x6BCD61EF5433B4ECLL;
        }
        for (g_6 = 2; (g_6 >= 0); g_6 -= 1)
        { 
            uint8_t l_93[4] = {0U,0U,0U,0U};
            int32_t *l_94 = &g_45;
            int32_t l_106 = 0xCAF053B5;
            int32_t l_107[3];
            int i;
            for (i = 0; i < 3; i++)
                l_107[i] = 7;
        }
    }
    (*g_76) ^= (((+(0xE6 >= l_123)) == (safe_lshift_func_int8_t_s_u((((l_130 = ((((safe_sub_func_uint64_t_u_u(g_8.f0, 0x642EFCB15699354FLL)) , g_21) > 0x5D8BB331) && g_67[0])) < 0) || 0x537E), 1))) >= l_123);
    g_21 |= ((l_118 = (safe_div_func_int8_t_s_s((**g_53), (((*g_76) != (*g_76)) , l_105)))) , 6);
    return (*g_54);
}

static uint64_t  func_9(union U1  p_10, uint16_t  p_11, union U1  p_12)
{ 
    int16_t l_19 = 0xF59E;
    int8_t *l_51 = &g_41;
    int8_t **l_50 = &l_51;
    int32_t l_57 = 0xED293E35;
    int32_t l_58 = (-1);
    int32_t l_64 = 0;
    int32_t l_66[2][4];
    uint8_t l_80 = 9U;
    const int8_t *l_86[4];
    const int8_t ** const l_85 = &l_86[1];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
            l_66[i][j] = 0x0944B0CE;
    }
    for (i = 0; i < 4; i++)
        l_86[i] = &g_41;
    if (l_19)
    { 
        int32_t *l_20 = &g_21;
        int64_t l_22[2][4];
        int8_t *l_52 = (void*)0;
        int32_t l_56 = 1;
        int32_t l_59 = 4;
        int32_t l_61 = 0xAC46587A;
        int32_t l_63 = 0x1D8B2D29;
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 4; j++)
                l_22[i][j] = 4;
        }
        (*l_20) = p_11;
        if ((p_11 > (*l_20)))
        { 
            uint64_t l_46[3][2];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 2; j++)
                    l_46[i][j] = 18446744073709551608U;
            }
            for (g_21 = 0; (g_21 <= 1); g_21 += 1)
            { 
                int32_t *l_43 = &g_40;
                int32_t *l_44[3][4] = {{&g_45,&g_45,&g_21,&g_45},{&g_45,(void*)0,(void*)0,&g_45},{(void*)0,&g_45,(void*)0,(void*)0}};
                int i, j;
                l_46[2][1] ^= ((*l_43) = func_23(&g_21));
            }
        }
        else
        { 
            int16_t l_60[3][2] = {{0,0},{8,0},{0,8}};
            int32_t l_62[2][1];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_62[i][j] = 0;
            }
            if ((~((l_50 == (void*)0) | (l_52 != &g_18))))
            { 
                g_53 = (void*)0;
            }
            else
            { 
                int32_t *l_55[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_55[i] = (void*)0;
                g_67[0]++;
            }
            return g_40;
        }
        (*l_20) = (safe_lshift_func_uint16_t_u_u(((&l_59 != &l_63) > (safe_mul_func_uint32_t_u_u((0x22C9C813C75F9279LL > (-6)), 0x668E5470))), 12));
    }
    else
    { 
        int32_t *l_74 = &l_58;
        int32_t **l_75 = (void*)0;
        int32_t **l_77 = (void*)0;
        int32_t **l_78 = &g_76;
        int32_t *l_79[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
        int i;
        g_76 = l_74;
        (*l_78) = &l_64;
        ++l_80;
    }
    l_66[1][0] ^= (0x9B || (((((((65532U > (safe_mod_func_int16_t_s_s(p_12.f0, p_11))) ^ p_11) >= 0) != (*g_54)) != 0xBD11537C) , l_85) == &l_86[1]));
    return g_87[0][0];
}

static int32_t  func_23(int32_t * p_24)
{ 
    int64_t l_25 = 0xD423CB8E00457A23LL;
    int8_t *l_37 = &g_18;
    if ((l_25 , (((safe_sub_func_int64_t_s_s((((safe_add_func_int8_t_s_s((safe_mul_func_uint8_t_u_u(l_25, l_25)), l_25)) & 0x1E34F9A325CF162CLL) & l_25), l_25)) <= l_25) <= g_8.f0)))
    { 
        const int32_t l_34 = (-4);
        int8_t **l_38[2];
        int32_t *l_39 = &g_40;
        int i;
        for (i = 0; i < 2; i++)
            l_38[i] = &l_37;
        g_41 = ((*l_39) = ((((safe_rshift_func_uint64_t_u_s(l_34, 9)) , (-3)) < (safe_mod_func_int32_t_s_s((((l_37 = l_37) != (void*)0) != g_8.f0), l_34))) <= g_21));
    }
    else
    { 
        uint64_t l_42 = 0U;
        return l_42;
    }
    return (*p_24);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_8.f0, "g_8.f0", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_67[i], "g_67[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_87[i][j], "g_87[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_129, "g_129", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
