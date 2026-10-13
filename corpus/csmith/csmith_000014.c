/* Csmith 0cdc710315cf seed 14, flattened by scripts/csmith_flatten.py */
// Options:   --seed 14 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
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
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
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
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

static int16_t g_26 = 1;
static int32_t g_28 = 0x20348F4A;
static int32_t *g_27 = &g_28;
static int32_t g_44 = 0x2955D482;
static int32_t **g_46 = &g_27;
static uint32_t g_67 = 0x4743E03E;
static uint8_t g_95[2] = {0x9D,0x9D};
static uint32_t g_102 = 0xBA6A349B;
static int64_t g_119 = 0x0130CBD1ED58CDB4LL;
static int8_t g_120[3] = {(-1),(-1),(-1)};
static int64_t g_122 = 0x2A85F46675684669LL;
static int8_t g_123 = 0;
static uint64_t g_124 = 18446744073709551611U;
static int32_t g_129 = 0x2D82128C;
static int16_t g_131 = 0x1197;
static int32_t g_135 = 0x48C356ED;
static uint16_t g_173 = 7U;

static uint32_t  func_1(void);
static int16_t  func_12(uint64_t  p_13);
static const int16_t  func_19(int64_t  p_20, int8_t  p_21, uint64_t  p_22, int64_t  p_23, const int8_t  p_24);

static uint32_t  func_1(void)
{ 
    uint8_t l_14 = 0x63;
    int32_t *l_128 = &g_129;
    int16_t *l_130 = &g_131;
    int8_t *l_132 = (void*)0;
    int8_t *l_133 = &g_123;
    int32_t *l_134 = &g_135;
    (*l_134) |= (safe_sub_func_uint32_t_u_u((((*l_128) = (((*l_133) &= ((safe_lshift_func_int32_t_s_u((safe_sub_func_int64_t_s_s((safe_rshift_func_int16_t_s_s((safe_sub_func_int16_t_s_s(func_12(l_14), (((*l_130) = (!(l_128 != &g_129))) | g_120[0]))), 9)), (*l_128))), 11)) <= g_120[0])) >= 0x8F)) , 0x561CB1BB), g_120[2]));
    for (g_131 = 0; (g_131 <= (-28)); g_131--)
    { 
        uint64_t l_138 = 1U;
        (**g_46) &= l_138;
    }
    if ((*l_128))
    { 
        (*l_128) = (+(((**g_46) < (0xE611AB6490890113LL ^ (safe_mod_func_int8_t_s_s((((safe_mul_func_uint64_t_u_u(g_67, g_120[2])) , (void*)0) != (*g_46)), g_26)))) < 0x39D9));
    }
    else
    { 
        int16_t l_171 = 0xFF29;
        int32_t l_185 = 0;
        for (g_26 = 0; (g_26 <= (-27)); g_26--)
        { 
            int32_t l_172 = 0xDD90FB3C;
        }
        for (g_119 = 1; (g_119 >= 0); g_119 -= 1)
        { 
            int64_t *l_178 = &g_122;
            uint8_t *l_179 = (void*)0;
            uint8_t *l_180 = &g_95[1];
            int i;
            (*l_128) = (safe_mod_func_int16_t_s_s((safe_add_func_uint64_t_u_u((((((*l_178) = g_95[g_119]) , ((--(*l_180)) & g_173)) || (safe_lshift_func_int16_t_s_u((g_95[g_119] ^ (*g_27)), l_171))) >= 0x3B8F), l_185)), 0xE185));
            (*g_46) = l_128;
        }
    }
    return g_131;
}

static int16_t  func_12(uint64_t  p_13)
{ 
    int16_t l_25 = 0x2949;
    int16_t l_36 = (-1);
    int32_t l_37[4][3] = {{1,0x6C4F32B7,0x6C4F32B7},{1,0x6C4F32B7,0x6C4F32B7},{1,0x6C4F32B7,0x6C4F32B7},{1,0x6C4F32B7,0x6C4F32B7}};
    int32_t * const *l_81 = (void*)0;
    uint64_t l_92 = 0xF7ADB2349D53036CLL;
    int8_t l_115 = (-4);
    int i, j;
    if ((safe_mul_func_int32_t_s_s(((*g_27) = ((safe_mul_func_int16_t_s_s(func_19((l_25 , (p_13 , p_13)), l_25, l_25, g_26, g_26), 3U)) , p_13)), l_25)))
    { 
        int32_t l_59 = 0x76A17913;
        int32_t l_63 = 5;
        int32_t l_65 = (-6);
        int8_t l_66 = 0x5D;
        int64_t l_93[2][4];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 4; j++)
                l_93[i][j] = 0xBA410498C5D29228LL;
        }
        for (p_13 = 0; (p_13 >= 22); p_13 = safe_add_func_uint8_t_u_u(p_13, 6))
        { 
            int32_t **l_32 = &g_27;
            int32_t *l_35 = (void*)0;
            (*l_32) = &g_28;
            for (g_28 = 0; (g_28 != 15); g_28 = safe_add_func_int16_t_s_s(g_28, 5))
            { 
                l_35 = (*l_32);
                return l_36;
            }
        }
        for (g_26 = 0; (g_26 <= 2); g_26 += 1)
        { 
            int64_t l_45[1];
            int i;
            for (i = 0; i < 1; i++)
                l_45[i] = (-1);
            (*g_27) = ((safe_sub_func_int8_t_s_s((((safe_mod_func_int8_t_s_s((((g_44 = ((((safe_div_func_uint64_t_u_u(((*g_27) , ((void*)0 != &g_27)), 0x5F0E77DA19AE86A6LL)) < p_13) & l_37[3][0]) && 0x5C76)) , 0x0E74BB9CEC02DFCELL) || g_44), g_26)) && 0) ^ p_13), g_26)) <= l_45[0]);
            for (l_36 = 2; (l_36 >= 0); l_36 -= 1)
            { 
                int i, j;
                g_46 = &g_27;
                l_37[(g_26 + 1)][g_26] = (safe_div_func_int16_t_s_s(((safe_rshift_func_int32_t_s_u(l_37[l_36][g_26], 3)) || l_37[2][0]), ((1 != 1) || g_26)));
            }
        }
        if ((safe_mul_func_int16_t_s_s(((safe_sub_func_int64_t_s_s((safe_mod_func_uint8_t_u_u(p_13, p_13)), ((safe_add_func_int16_t_s_s((g_26 == 18446744073709551615U), 0xD083)) , l_36))) > l_59), l_59)))
        { 
            int64_t l_60 = (-7);
            int32_t l_64 = 0x0743585F;
            for (l_59 = 0; (l_59 <= 2); l_59 += 1)
            { 
                int32_t *l_61[4][2];
                int16_t l_62 = (-8);
                int i, j;
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_61[i][j] = &l_37[0][2];
                }
                ++g_67;
                (*g_27) = (safe_sub_func_int8_t_s_s(((safe_lshift_func_int32_t_s_s((safe_mod_func_int16_t_s_s(l_59, (safe_lshift_func_int32_t_s_u((safe_mod_func_uint8_t_u_u(p_13, (l_64 |= ((0U | g_44) , g_67)))), g_28)))), p_13)) > p_13), 1));
            }
        }
        else
        { 
            int32_t * const *l_80 = &g_27;
            int64_t l_101 = 0xC047C65310EE7DBBLL;
            if ((((l_81 = l_80) != &g_27) ^ (((safe_mul_func_uint32_t_u_u(4294967291U, (**l_80))) & p_13) == l_63)))
            { 
                uint8_t l_89[3][1];
                int8_t *l_94[2][1];
                int32_t *l_96 = &l_63;
                int i, j;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_89[i][j] = 0x0C;
                }
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_94[i][j] = &l_66;
                }
                (*l_96) |= ((safe_add_func_int16_t_s_s((((g_95[1] = (((safe_rshift_func_uint32_t_u_u((((+(p_13 ^ ((--l_89[1][0]) == ((l_59 |= (**l_80)) , (((l_92 , g_44) && 0x1200BAF9C35C0EDELL) , l_93[1][0]))))) <= (-1)) >= 0U), 18)) > (**l_80)) | 1)) , 0xB755) && 0x3B7D), (**l_81))) > p_13);
            }
            else
            { 
                int32_t *l_97 = &l_59;
                int32_t *l_98 = &l_63;
                int32_t *l_99 = (void*)0;
                int32_t *l_100[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_100[i] = &l_65;
                --g_102;
            }
        }
    }
    else
    { 
        int32_t *l_105[3][1];
        uint8_t l_106 = 254U;
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_105[i][j] = &l_37[3][2];
        }
        for (l_25 = 0; l_25 < 4; l_25 = l_25 + 1)
        {
            for (l_36 = 0; l_36 < 3; l_36 = l_36 + 1)
            {
                l_37[l_25][l_36] = 0x3996C8B9;
            }
        }
        l_106 ^= (*g_27);
    }
    for (g_44 = 0; (g_44 <= 1); g_44 += 1)
    { 
        uint16_t l_107 = 0x9ACA;
        int32_t l_116 = (-10);
        int32_t l_117 = 0;
        int32_t l_118 = (-4);
        int32_t l_121 = 0x7036B522;
        if (l_107)
        { 
            (**g_46) = (safe_add_func_uint32_t_u_u((safe_mul_func_uint8_t_u_u((((safe_unary_minus_func_int16_t_s(g_67)) ^ g_26) == (0U <= 0x34F8EFC5)), g_95[1])), 4294967288U));
        }
        else
        { 
            int32_t *l_113 = &l_37[2][0];
            int32_t *l_114[1];
            int i;
            for (i = 0; i < 1; i++)
                l_114[i] = &g_28;
            ++g_124;
        }
    }
    return p_13;
}

static const int16_t  func_19(int64_t  p_20, int8_t  p_21, uint64_t  p_22, int64_t  p_23, const int8_t  p_24)
{ 
    int32_t *l_29 = &g_28;
    l_29 = g_27;
    return (*l_29);
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_67, "g_67", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_95[i], "g_95[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_102, "g_102", print_hash_value);
    transparent_crc(g_119, "g_119", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_120[i], "g_120[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_122, "g_122", print_hash_value);
    transparent_crc(g_123, "g_123", print_hash_value);
    transparent_crc(g_124, "g_124", print_hash_value);
    transparent_crc(g_129, "g_129", print_hash_value);
    transparent_crc(g_131, "g_131", print_hash_value);
    transparent_crc(g_135, "g_135", print_hash_value);
    transparent_crc(g_173, "g_173", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
