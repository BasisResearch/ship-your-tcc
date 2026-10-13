/* Csmith 0cdc710315cf seed 192, flattened by scripts/csmith_flatten.py */
// Options:   --seed 192 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
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
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
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
};

union U1 {
   uint16_t  f0;
   const uint16_t  f1;
   uint32_t  f2;
};

union U3 {
   uint8_t  f0;
};

union U4 {
   int16_t  f0;
   int64_t  f1;
};

union U5 {
   int32_t  f0;
   const int64_t  f1;
};

static uint8_t g_2 = 252U;
static int32_t g_5 = 0x0707DFE4;
static int64_t g_22 = 0x39E071A05132D7AFLL;
static int16_t g_32 = (-1);
static int32_t **g_40 = (void*)0;
static union U5 g_44 = {0x4CA67B11};
static uint8_t g_67 = 0x34;
static uint16_t g_73 = 0x8108;
static uint32_t g_74 = 0x1338462C;
static union U4 g_81 = {-1};
static int32_t g_96 = 1;
static int32_t *g_95 = &g_96;
static union U3 *g_97 = (void*)0;
static uint64_t g_108 = 18446744073709551609U;
static const uint8_t g_143[3] = {255U,255U,255U};
static union U1 g_153 = {65535U};
static union U1 * const g_152 = &g_153;
static union U1 g_155 = {65534U};
static union U1 *g_154[2] = {&g_155,&g_155};

static union U1  func_1(void);
static uint64_t  func_10(union U4  p_11, union U5  p_12, int32_t  p_13, int32_t * p_14);
static union U4  func_15(union U3  p_16, int64_t  p_17, int8_t  p_18);

static union U1  func_1(void)
{ 
    const uint8_t l_23 = 0x04;
    union U5 l_94 = {0xA9CADD72};
    uint16_t l_142 = 0x199D;
    union U1 l_156[2] = {{0U},{0U}};
    int i;
    if (g_2)
    { 
        int16_t l_3 = 0x0444;
        int32_t *l_4 = &g_5;
        (*l_4) = l_3;
    }
    else
    { 
        union U3 l_19[3] = {{4U},{4U},{4U}};
        int64_t *l_21 = &g_22;
        uint64_t l_24 = 7U;
        int32_t *l_144 = (void*)0;
        int32_t *l_145 = (void*)0;
        int32_t *l_146 = (void*)0;
        int32_t *l_147 = &g_5;
        int i;
        (*l_147) = ((*g_95) = ((safe_div_func_uint8_t_u_u((safe_mod_func_uint64_t_u_u(func_10(func_15(l_19[0], (((((-1) > ((*l_21) = (~g_5))) && g_5) >= l_23) >= g_5), l_24), l_94, l_19[0].f0, g_95), l_142)), g_143[2])) | g_143[2]));
        for (g_44.f0 = (-20); (g_44.f0 == 12); ++g_44.f0)
        { 
            for (l_24 = 0; (l_24 < 13); l_24 = safe_add_func_uint8_t_u_u(l_24, 4))
            { 
                g_5 &= (*g_95);
            }
            (*l_147) ^= (*g_95);
            for (l_142 = 0; l_142 < 3; l_142 = l_142 + 1)
            {
                union U3 tmp = {1U};
                l_19[l_142] = tmp;
            }
        }
        g_154[0] = g_152;
    }
    (*g_95) = (-5);
    return l_156[1];
}

static uint64_t  func_10(union U4  p_11, union U5  p_12, int32_t  p_13, int32_t * p_14)
{ 
    int16_t l_109 = 7;
    int32_t l_123[2];
    int i;
    for (i = 0; i < 2; i++)
        l_123[i] = 0x22FC4CEA;
    if ((*p_14))
    { 
        union U3 **l_98 = &g_97;
        (*l_98) = g_97;
    }
    else
    { 
        uint64_t l_102 = 9U;
        uint8_t *l_113 = &g_67;
        uint64_t *l_132 = &g_108;
        int64_t *l_135 = &g_22;
        if ((*g_95))
        { 
            uint64_t *l_101 = (void*)0;
            int32_t l_105 = 0xBAEB33E7;
            int16_t l_106 = (-1);
            uint64_t *l_107 = &g_108;
            (*g_95) = ((((*l_107) = ((safe_sub_func_uint64_t_u_u((l_102 = 0xF4C626C4802A3EDBLL), (safe_add_func_uint16_t_u_u((g_73 || l_105), 65532U)))) <= l_106)) <= g_44.f0) <= l_109);
        }
        else
        { 
            int32_t *l_110 = &g_96;
            union U0 l_120 = {0xCFBC8947};
            int64_t *l_121 = &g_81.f1;
            uint16_t l_122 = 7U;
            l_110 = &g_96;
            for (g_2 = 0; (g_2 != 40); ++g_2)
            { 
                (*l_110) = ((void*)0 != l_113);
            }
            l_123[0] &= ((((safe_mul_func_int8_t_s_s((safe_div_func_uint64_t_u_u(((safe_div_func_uint8_t_u_u((l_120 , ((*l_113) |= (0xB1C03A75F9311F6BLL <= ((*l_121) = l_109)))), g_74)) == 0xE9), p_12.f0)), l_102)) >= (*l_110)) != l_109) , l_122);
        }
        g_44.f0 = (safe_mul_func_int64_t_s_s(((*l_135) = (safe_mod_func_uint64_t_u_u((safe_mul_func_int16_t_s_s((safe_sub_func_int32_t_s_s(((((*l_113) = (((*l_132)--) && ((1 && ((*p_14) = (((((l_102 != g_44.f0) && g_2) >= 0xDA819422B6A99605LL) <= p_11.f0) ^ l_123[1]))) ^ p_12.f0))) | l_123[0]) ^ g_44.f0), 0xB6043366)), 0xB581)), 0xB0F311A8DCBC2B75LL))), l_123[0]));
        (*g_95) = (safe_div_func_int64_t_s_s((safe_rshift_func_uint64_t_u_u(((safe_rshift_func_int16_t_s_u(p_12.f0, 10)) ^ ((*l_132) = (l_102 ^ (*p_14)))), 4)), l_123[0]));
    }
    return g_44.f0;
}

static union U4  func_15(union U3  p_16, int64_t  p_17, int8_t  p_18)
{ 
    uint32_t l_41 = 3U;
    int32_t l_93 = 0x08B99CB9;
    for (p_16.f0 = 24; (p_16.f0 == 47); ++p_16.f0)
    { 
        union U3 l_29 = {0U};
        int16_t *l_30 = (void*)0;
        int16_t *l_31[2];
        int32_t l_33[2];
        int64_t *l_34[2];
        int32_t l_35 = 0x65FBC01D;
        int64_t l_57 = 1;
        int i;
        for (i = 0; i < 2; i++)
            l_31[i] = &g_32;
        for (i = 0; i < 2; i++)
            l_33[i] = 0xBE62699F;
        for (i = 0; i < 2; i++)
            l_34[i] = &g_22;
        l_35 |= (((l_33[1] ^= (safe_mul_func_uint16_t_u_u((((((p_17 < (p_18 > (l_29 , 0xAD))) && 0x8BBE58ED0BD2F23DLL) <= 0xD2DA) || l_29.f0) > 0x62), p_18))) , &p_17) == l_34[1]);
        for (l_29.f0 = 0; (l_29.f0 <= 3); l_29.f0++)
        { 
            const uint32_t l_56 = 0xEC8B24CE;
            for (l_35 = 1; (l_35 >= 0); l_35 -= 1)
            { 
                int32_t *l_39[4][2] = {{&l_33[0],&l_33[0]},{&l_33[0],&l_33[0]},{&l_33[0],&l_33[0]},{&l_33[0],&l_33[0]}};
                int32_t **l_38[2];
                int8_t l_55[2][4];
                int i, j;
                for (i = 0; i < 2; i++)
                    l_38[i] = &l_39[3][0];
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 4; j++)
                        l_55[i][j] = 0x77;
                }
                g_40 = l_38[0];
                l_41 ^= (2U || g_2);
                l_57 = ((g_5 &= ((((safe_lshift_func_int16_t_s_u(((&l_33[1] != ((g_44 , (safe_rshift_func_uint16_t_u_s((((safe_add_func_uint32_t_u_u((safe_sub_func_uint32_t_u_u(((((safe_rshift_func_uint8_t_u_u((safe_lshift_func_int32_t_s_u(9, l_55[0][1])), g_2)) | g_22) & 0x047C1471C2AA0BDELL) || l_35), p_17)), p_17)) || (-1)) <= l_56), 8))) , (*g_40))) != p_17), p_16.f0)) > 4294967295U) && 0x5B) , 1)) <= l_33[1]);
            }
            for (l_41 = 0; (l_41 > 4); l_41 = safe_add_func_int64_t_s_s(l_41, 1))
            { 
                uint8_t *l_64 = &g_2;
                uint8_t *l_65 = (void*)0;
                uint8_t *l_66 = &g_67;
                uint16_t *l_72 = &g_73;
                const uint32_t l_75 = 4294967295U;
                int32_t *l_76[1][3];
                uint64_t l_77 = 0x7DF0EBE340CFCE32LL;
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 3; j++)
                        l_76[i][j] = (void*)0;
                }
                l_77 &= (safe_mod_func_uint64_t_u_u((safe_rshift_func_uint8_t_u_u(((*l_66) = ((*l_64) = g_5)), p_18)), (safe_mod_func_uint16_t_u_u((g_74 = ((safe_mul_func_uint16_t_u_u(((l_33[1] = (((*l_72) &= l_57) || 3)) >= p_18), l_41)) < p_17)), l_75))));
            }
        }
    }
    for (g_22 = (-6); (g_22 > 14); g_22++)
    { 
        int16_t l_80[4][4];
        int32_t l_82[1][2];
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
                l_80[i][j] = 0xAFBA;
        }
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_82[i][j] = 0x0774A768;
        }
        if (p_16.f0)
            break;
        for (p_17 = 0; (p_17 <= 3); p_17 += 1)
        { 
            int32_t *l_83 = (void*)0;
            int32_t *l_84 = (void*)0;
            int32_t *l_85 = &g_5;
            int i, j;
            for (g_32 = 3; (g_32 >= 0); g_32 -= 1)
            { 
                return g_81;
            }
            l_82[0][1] ^= (-10);
            (*l_85) = l_80[p_17][p_17];
        }
        if (l_41)
            continue;
    }
    for (g_2 = 13; (g_2 <= 25); g_2 = safe_add_func_uint8_t_u_u(g_2, 1))
    { 
        int32_t *l_88 = (void*)0;
        int32_t *l_89 = &g_5;
        (*l_89) ^= p_16.f0;
        for (g_5 = 20; (g_5 > 21); g_5 = safe_add_func_uint32_t_u_u(g_5, 7))
        { 
            int32_t *l_92 = &g_44.f0;
            l_93 = ((*l_92) = 0x76030DEE);
        }
        return g_81;
    }
    return g_81;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_44.f0, "g_44.f0", print_hash_value);
    transparent_crc(g_67, "g_67", print_hash_value);
    transparent_crc(g_73, "g_73", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_96, "g_96", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_143[i], "g_143[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_153.f0, "g_153.f0", print_hash_value);
    transparent_crc(g_155.f0, "g_155.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
