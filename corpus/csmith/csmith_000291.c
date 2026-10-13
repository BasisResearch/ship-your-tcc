/* Csmith 0cdc710315cf seed 291, flattened by scripts/csmith_flatten.py */
// Options:   --seed 291 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
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
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
static int8_t
safe_unary_minus_func_int8_t_s(int8_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint16_t  f0;
};

static uint32_t g_2 = 4294967295U;
static int32_t g_6 = 0xFE6E3245;
static int32_t g_7 = 0xE923E64F;
static int16_t g_9 = 0;
static uint16_t g_10 = 8U;
static uint8_t g_24[1] = {250U};
static const int32_t * const *g_45 = (void*)0;
static const int32_t g_53 = 0;
static const int32_t g_55 = 0x384F7A32;
static int8_t g_56[4][3] = {{1,0,0x71},{1,1,0},{0xBC,0,0},{0,0,0x71}};
static int32_t *g_62 = (void*)0;
static union U0 g_94 = {65527U};
static int32_t **g_107 = (void*)0;
static int32_t **g_108 = &g_62;
static uint16_t g_137 = 1U;
static int8_t g_151 = (-5);
static int8_t g_152 = 0x39;

static int64_t  func_1(void);
static int32_t  func_17(int32_t * p_18, union U0  p_19, int32_t * p_20);
static int32_t * func_29(int64_t  p_30, int8_t  p_31, uint64_t  p_32, int64_t  p_33, uint8_t  p_34);

static int64_t  func_1(void)
{ 
    int32_t *l_5[2];
    int8_t l_8[3];
    int8_t l_89 = 0;
    int32_t **l_106[1];
    int i;
    for (i = 0; i < 2; i++)
        l_5[i] = &g_6;
    for (i = 0; i < 3; i++)
        l_8[i] = 0;
    for (i = 0; i < 1; i++)
        l_106[i] = &g_62;
    g_2--;
lbl_110:
    g_10--;
    if (g_10)
    { 
        int32_t *l_23 = &g_7;
        for (g_7 = 0; (g_7 == (-12)); g_7 = safe_sub_func_uint64_t_u_u(g_7, 8))
        { 
            return g_10;
        }
        for (g_9 = (-19); (g_9 == (-14)); ++g_9)
        { 
            int32_t *l_21 = &g_7;
            union U0 l_22[1] = {{1U}};
            int i;
            (*l_23) = func_17(l_21, l_22[0], l_23);
            for (g_7 = 1; (g_7 >= 0); g_7 -= 1)
            { 
                int i;
                l_89 ^= (*l_23);
            }
            for (g_7 = 2; (g_7 >= (-15)); --g_7)
            { 
                int8_t l_97 = 0xDD;
                int8_t *l_98 = &g_56[1][1];
                int32_t **l_99 = &g_62;
                g_6 |= (safe_add_func_int8_t_s_s((g_94 , (safe_mul_func_uint8_t_u_u((*l_21), ((*l_98) = l_97)))), g_94.f0));
                (*l_99) = (void*)0;
            }
        }
    }
    else
    { 
        int32_t l_104 = 1;
        int32_t **l_105 = &g_62;
        int8_t *l_109 = &g_56[0][1];
        int64_t l_120 = 0;
        int32_t l_153 = 0;
        int32_t l_154[2][3] = {{0x333B9D5F,0x333B9D5F,0x333B9D5F},{(-1),(-1),(-1)}};
        int i, j;
        if ((((safe_mul_func_uint64_t_u_u((safe_div_func_uint8_t_u_u((((*l_109) = (l_104 != ((g_107 = (l_106[0] = l_105)) != (g_108 = l_105)))) , g_56[3][1]), g_2)), 0xCE4C8D1EA5100093LL)) , &g_62) != (void*)0))
        { 
            if (l_89)
                goto lbl_110;
        }
        else
        { 
            uint16_t l_111 = 65530U;
            int32_t l_129 = (-2);
            int8_t l_145 = (-1);
            --l_111;
            for (l_104 = 0; (l_104 <= 2); l_104 += 1)
            { 
                uint32_t l_128 = 4294967295U;
                int64_t *l_130 = &l_120;
                int i;
                l_129 = ((safe_rshift_func_int16_t_s_u((safe_mul_func_int64_t_s_s((safe_mul_func_uint16_t_u_u(l_120, ((~((l_8[l_104] ^= (safe_lshift_func_int64_t_s_s((safe_add_func_int8_t_s_s((-6), (safe_lshift_func_uint16_t_u_u(g_55, g_56[3][2])))), l_128))) && 249U)) == g_94.f0))), g_53)), 3)) , l_111);
                (*l_105) = (*l_105);
                g_7 |= (l_129 = ((((((*l_130) &= 0xA30E59352247AAA1LL) < (safe_add_func_uint8_t_u_u(((safe_mul_func_uint8_t_u_u((0x10F1 == (safe_sub_func_int8_t_s_s((65535U >= l_129), 0x06))), g_2)) & l_128), g_55))) > 0U) || g_137) && 0xFBA1));
            }
            for (g_137 = 6; (g_137 >= 16); g_137++)
            { 
                uint16_t l_140 = 0xFAEA;
                const uint32_t l_148[1] = {0xD493DC33};
                int32_t l_149[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_149[i] = 5;
                l_140++;
                l_149[0] = ((safe_add_func_int16_t_s_s((l_145 <= (safe_mod_func_uint64_t_u_u(g_24[0], (((g_10 = ((g_2 ^ g_24[0]) || l_148[0])) , 1) || 4294967288U)))), g_94.f0)) , l_148[0]);
                return g_9;
            }
        }
        if (g_24[0])
        { 
            return g_53;
        }
        else
        { 
            int32_t l_150 = 1;
            uint8_t l_155 = 0x28;
            --l_155;
            (*g_108) = func_29((g_6 < (safe_add_func_uint16_t_u_u((g_137 = l_155), (((void*)0 == (*g_107)) < g_151)))), l_150, g_6, g_55, g_9);
        }
    }
    return g_2;
}

static int32_t  func_17(int32_t * p_18, union U0  p_19, int32_t * p_20)
{ 
    int64_t l_37[3];
    const int32_t *l_54 = &g_55;
    int32_t l_57 = 1;
    int32_t **l_79 = &g_62;
    int i;
    for (i = 0; i < 3; i++)
        l_37[i] = (-1);
    for (g_10 = 0; (g_10 <= 0); g_10 += 1)
    { 
        const int32_t *l_52 = &g_53;
        const int32_t **l_51[4];
        int32_t l_80 = 0x9B52FFB4;
        int32_t **l_84 = &g_62;
        int i;
        for (i = 0; i < 4; i++)
            l_51[i] = &l_52;
        l_57 |= ((safe_sub_func_uint64_t_u_u((g_56[1][1] = ((safe_rshift_func_int8_t_s_s(((l_54 = func_29(g_24[g_10], g_24[0], ((((safe_lshift_func_uint8_t_u_s((g_24[g_10] < 0x69FF9E6B99BF0BB9LL), 6)) , g_6) < 1U) , g_2), p_19.f0, l_37[2])) == p_20), l_37[1])) & p_19.f0)), 0x3D96456CFC4FF2D4LL)) , (*l_52));
        if (((safe_unary_minus_func_int16_t_s((p_19.f0 > (safe_unary_minus_func_int8_t_s((safe_div_func_uint8_t_u_u(0xB5, (((*p_18) = g_6) & g_53)))))))) != g_6))
        { 
            g_62 = (void*)0;
            for (g_2 = 0; (g_2 != 40); g_2 = safe_add_func_int16_t_s_s(g_2, 8))
            { 
                l_57 &= (((safe_add_func_int8_t_s_s((safe_mul_func_uint8_t_u_u(((((safe_sub_func_uint64_t_u_u(p_19.f0, (safe_rshift_func_uint16_t_u_u(((safe_mod_func_uint32_t_u_u((safe_add_func_int32_t_s_s((p_19.f0 || ((void*)0 != &l_54)), g_9)), p_19.f0)) && (-1)), 12)))) | 0xAA) <= 0xA35EB253) ^ 0xB3), 0xC4)), (*l_54))) , 247U) <= p_19.f0);
                l_80 = ((*p_18) = (safe_add_func_uint16_t_u_u(g_55, (((l_79 = &g_62) == &p_18) | (-1)))));
            }
            l_79 = (void*)0;
        }
        else
        { 
            for (l_57 = 0; (l_57 > (-6)); l_57 = safe_sub_func_int32_t_s_s(l_57, 9))
            { 
                int32_t **l_83 = (void*)0;
                l_54 = &g_53;
                l_84 = l_83;
            }
        }
        (*p_18) = ((&g_62 != &l_52) > (safe_add_func_uint8_t_u_u((safe_mul_func_int16_t_s_s(p_19.f0, 0x945F)), g_9)));
    }
    (*p_18) = 0;
    (*p_20) = (-2);
    return (*l_54);
}

static int32_t * func_29(int64_t  p_30, int8_t  p_31, uint64_t  p_32, int64_t  p_33, uint8_t  p_34)
{ 
    int32_t *l_38 = (void*)0;
    int32_t **l_39 = &l_38;
    int32_t *l_40 = &g_7;
    (*l_40) |= (((*l_39) = l_38) != (void*)0);
    for (p_30 = 6; (p_30 > (-5)); p_30 = safe_sub_func_int8_t_s_s(p_30, 1))
    { 
        int32_t *l_48 = &g_6;
        for (p_34 = 0; (p_34 > 39); p_34 = safe_add_func_uint16_t_u_u(p_34, 1))
        { 
            int32_t *l_46 = &g_7;
            if (((void*)0 != g_45))
            { 
                return &g_7;
            }
            else
            { 
                int32_t *l_47 = &g_7;
                l_47 = l_46;
                (*l_39) = l_48;
            }
        }
        (*l_39) = l_48;
        for (p_33 = (-10); (p_33 >= 26); ++p_33)
        { 
            (*l_39) = l_48;
        }
    }
    (*l_39) = (*l_39);
    return &g_6;
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
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_10, "g_10", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_24[i], "g_24[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_53, "g_53", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_56[i][j], "g_56[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_94.f0, "g_94.f0", print_hash_value);
    transparent_crc(g_137, "g_137", print_hash_value);
    transparent_crc(g_151, "g_151", print_hash_value);
    transparent_crc(g_152, "g_152", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
