/* Csmith 0cdc710315cf seed 252, flattened by scripts/csmith_flatten.py */
// Options:   --seed 252 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
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
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   int64_t  f0;
};

union U1 {
   uint64_t  f0;
   uint8_t  f1;
};

static int32_t g_2 = 9;
static int32_t g_14 = (-1);
static union U0 g_15 = {0x4207647F1A969AC5LL};
static union U1 g_26 = {0xF51EF9953E80C654LL};
static uint8_t g_41 = 0x64;
static int32_t g_50 = 0xBA323A35;
static uint64_t g_69 = 0x40293E9FC65F31FDLL;
static uint64_t * const g_68[3] = {&g_69,&g_69,&g_69};
static union U0 g_74 = {-1};
static const int32_t *g_79 = &g_14;
static uint16_t g_90 = 0x4E30;
static int8_t g_93 = 0xBA;
static union U1 g_116 = {0xC41F2E98D3579F7DLL};
static union U1 *g_115 = &g_116;

static union U0  func_1(void);
static int32_t  func_20(int16_t  p_21, uint64_t  p_22, int32_t * const  p_23);
static int32_t * func_24(union U1  p_25);

static union U0  func_1(void)
{ 
    uint64_t l_5 = 1U;
    int16_t l_6[1];
    uint16_t l_76[2];
    union U1 l_80 = {0x297682755FCA9972LL};
    int32_t l_107 = 0x6AF0F771;
    union U0 l_120 = {1};
    int i;
    for (i = 0; i < 1; i++)
        l_6[i] = (-6);
    for (i = 0; i < 2; i++)
        l_76[i] = 65534U;
    for (g_2 = (-20); (g_2 != (-17)); g_2++)
    { 
        int64_t l_42 = 0x63B6910C33490CE6LL;
        const int32_t *l_78 = &g_50;
        if ((l_6[0] = l_5))
        { 
            uint8_t l_12 = 6U;
            int32_t *l_13 = &g_14;
            (*l_13) = (safe_mul_func_int8_t_s_s((safe_add_func_int64_t_s_s(0xF365653D42374B1BLL, (+l_12))), (g_2 == g_2)));
            for (l_12 = 0; (l_12 <= 0); l_12 += 1)
            { 
                return g_15;
            }
        }
        else
        { 
            int32_t *l_35 = &g_14;
            int32_t **l_34 = &l_35;
            uint8_t *l_40 = &g_41;
            int16_t l_75[3];
            int i;
            for (i = 0; i < 3; i++)
                l_75[i] = 1;
            if ((safe_div_func_uint16_t_u_u(((((safe_lshift_func_uint32_t_u_u((((func_20((((*l_34) = func_24(g_26)) != (((safe_sub_func_int64_t_s_s((safe_sub_func_uint8_t_u_u((((*l_40) = 0x62) > g_2), g_2)), l_6[0])) , l_42) , (void*)0)), g_2, &g_2) > g_2) , 65527U) , g_50), g_2)) && l_75[2]) & g_2) <= 1), l_76[1])))
            { 
                int32_t *l_77 = (void*)0;
                (*l_34) = l_77;
            }
            else
            { 
                g_79 = l_78;
            }
        }
        g_79 = func_24(l_80);
    }
    for (g_2 = 0; (g_2 > 9); g_2 = safe_add_func_uint8_t_u_u(g_2, 3))
    { 
        int64_t l_91 = 0xA8FA0B358DD5D93DLL;
        union U1 l_117[2][1] = {{{18446744073709551609U}},{{18446744073709551609U}}};
        int32_t *l_119 = (void*)0;
        int32_t **l_118 = &l_119;
        int i, j;
        for (g_15.f0 = 1; (g_15.f0 >= 0); g_15.f0 -= 1)
        { 
            uint8_t *l_83 = &g_26.f1;
            uint8_t *l_84 = (void*)0;
            uint8_t *l_85 = &g_41;
            int8_t *l_92 = &g_93;
            int i;
            g_50 |= (l_76[g_15.f0] < ((*l_92) = (((++(*l_85)) | g_2) ^ (safe_add_func_uint64_t_u_u((((((((g_15.f0 ^ g_69) , 0xB4A04EC1) ^ l_76[g_15.f0]) && g_90) != 0xD4616B4C8AB2937FLL) || l_76[0]) , 0U), l_91)))));
        }
        for (l_80.f1 = (-2); (l_80.f1 >= 21); l_80.f1++)
        { 
            int64_t *l_100 = &g_74.f0;
            int32_t l_105 = 0xF9042C8A;
            int32_t l_106 = 5;
            int32_t *l_109 = (void*)0;
            int32_t **l_108 = &l_109;
            l_107 = (safe_lshift_func_int8_t_s_s(((((g_69 != (safe_mul_func_int64_t_s_s(((*l_100) = l_91), ((safe_mul_func_int8_t_s_s((safe_lshift_func_int8_t_s_s(((l_105 |= ((l_100 != &g_69) == l_80.f1)) | g_41), l_106)), g_41)) || l_6[0])))) & g_50) > g_15.f0) >= 1), g_50));
            (*l_108) = &l_107;
            for (g_26.f0 = (-29); (g_26.f0 > 39); ++g_26.f0)
            { 
                union U1 **l_112 = (void*)0;
                union U1 *l_114 = &g_26;
                union U1 **l_113[2][3] = {{&l_114,&l_114,&l_114},{&l_114,&l_114,&l_114}};
                int i, j;
                (*l_108) = &g_2;
                g_115 = &g_26;
            }
        }
        (*l_118) = func_24(l_117[0][0]);
    }
    return l_120;
}

static int32_t  func_20(int16_t  p_21, uint64_t  p_22, int32_t * const  p_23)
{ 
    uint32_t l_58 = 0x008693C2;
    uint8_t l_62[2];
    const int32_t * const l_64 = &g_50;
    const int32_t * const *l_63 = &l_64;
    int i;
    for (i = 0; i < 2; i++)
        l_62[i] = 252U;
    for (g_14 = 9; (g_14 == (-9)); g_14 = safe_sub_func_uint16_t_u_u(g_14, 1))
    { 
        uint8_t *l_46[3];
        uint8_t **l_45 = &l_46[0];
        int i;
        for (i = 0; i < 3; i++)
            l_46[i] = &g_41;
        l_45 = l_45;
        for (g_15.f0 = (-27); (g_15.f0 != 25); ++g_15.f0)
        { 
            int32_t *l_49 = &g_50;
            (*l_49) = 0;
            if ((*l_49))
                continue;
            for (p_21 = 5; (p_21 >= 18); p_21 = safe_add_func_int16_t_s_s(p_21, 7))
            { 
                return (*p_23);
            }
        }
    }
    for (g_26.f0 = 0; (g_26.f0 == 26); g_26.f0 = safe_add_func_uint32_t_u_u(g_26.f0, 1))
    { 
        if (g_2)
        { 
            int32_t *l_57[1][2];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_57[i][j] = &g_14;
            }
            l_58 = ((safe_div_func_int64_t_s_s(g_26.f0, g_15.f0)) >= (*p_23));
        }
        else
        { 
            int32_t *l_59 = &g_50;
            (*l_59) = 1;
        }
    }
    for (g_15.f0 = (-10); (g_15.f0 != (-29)); g_15.f0 = safe_sub_func_uint64_t_u_u(g_15.f0, 1))
    { 
        int32_t *l_66 = &g_14;
        int32_t **l_65 = &l_66;
        uint64_t * const l_67 = &g_26.f0;
        uint64_t *l_70 = &g_69;
        for (g_50 = 1; (g_50 >= 0); g_50 -= 1)
        { 
            int i;
            return l_62[g_50];
        }
        if (((((*l_70) ^= (((l_63 != (g_26.f0 , l_65)) , l_67) == g_68[1])) & p_21) & 65529U))
        { 
            int32_t *l_72[4][1] = {{(void*)0},{&g_50},{(void*)0},{&g_50}};
            int32_t **l_71 = &l_72[1][0];
            int i, j;
            (*l_71) = ((*l_65) = (*l_65));
        }
        else
        { 
            union U0 *l_73 = &g_74;
            l_73 = (void*)0;
            (*l_65) = &g_50;
        }
    }
    return g_74.f0;
}

static int32_t * func_24(union U1  p_25)
{ 
    uint8_t l_27 = 0xCA;
    const uint32_t l_30 = 0x9AB83EB6;
    int64_t *l_31 = &g_15.f0;
    uint64_t *l_32[2];
    int32_t *l_33[1];
    int i;
    for (i = 0; i < 2; i++)
        l_32[i] = (void*)0;
    for (i = 0; i < 1; i++)
        l_33[i] = &g_14;
    g_14 = (((l_27 , (((g_26.f0 &= (l_27 || (((*l_31) = ((g_15.f0 ^ l_30) & 0U)) , l_30))) & l_30) ^ p_25.f0)) | g_14) , 0x92357994);
    return &g_2;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_15.f0, "g_15.f0", print_hash_value);
    transparent_crc(g_26.f0, "g_26.f0", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_74.f0, "g_74.f0", print_hash_value);
    transparent_crc(g_90, "g_90", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    transparent_crc(g_116.f0, "g_116.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
