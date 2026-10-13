/* Csmith 0cdc710315cf seed 157, flattened by scripts/csmith_flatten.py */
// Options:   --seed 157 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
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
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int8_t g_2[4] = {(-1),(-1),(-1),(-1)};
static int32_t g_3[2] = {6,6};
static int32_t g_4 = 7;
static int32_t g_20 = 0;
static int32_t *g_19 = &g_20;
static uint16_t g_35 = 0x81E1;
static uint32_t g_41 = 4294967287U;
static int64_t g_45[1] = {0x5BD639B6AF4A9B83LL};
static int16_t g_80[4] = {(-2),(-2),(-2),(-2)};
static uint64_t g_110 = 0x612EDF4FAD408300LL;
static int8_t g_119 = 0x38;
static int64_t g_141[4] = {0x632A4309C40C0AAELL,0x632A4309C40C0AAELL,0x632A4309C40C0AAELL,0x632A4309C40C0AAELL};
static uint32_t g_145 = 0U;

static uint16_t  func_1(void);
static int32_t  func_5(uint64_t  p_6);
static uint64_t  func_7(const uint64_t  p_8, int8_t  p_9, int32_t  p_10);

static uint16_t  func_1(void)
{ 
    for (g_3[1] = 3; (g_3[1] >= 0); g_3[1] -= 1)
    { 
        uint32_t l_126[4];
        int32_t *l_146[2][3] = {{&g_3[1],&g_3[1],&g_3[1]},{&g_4,&g_4,&g_4}};
        int32_t **l_147 = &g_19;
        int i, j;
        for (i = 0; i < 4; i++)
            l_126[i] = 0xBB3B6D22;
        for (g_4 = 0; (g_4 <= 3); g_4 += 1)
        { 
            uint8_t l_127 = 255U;
            int i;
            g_145 = func_5((((func_7(((255U >= 0xE2) == 4294967295U), g_2[g_3[1]], g_2[g_4]) == l_126[0]) >= 4U) != l_127));
            return l_126[0];
        }
        (*l_147) = l_146[1][2];
    }
    return g_110;
}

static int32_t  func_5(uint64_t  p_6)
{ 
    uint64_t *l_128 = &g_110;
    const int32_t l_129 = 0x5268D47C;
    int32_t *l_130 = (void*)0;
    int32_t *l_131 = (void*)0;
    int8_t *l_134 = (void*)0;
    int8_t *l_135 = (void*)0;
    int8_t *l_136[1];
    int32_t l_137 = 0x892D9D7C;
    int32_t *l_138 = &g_20;
    int32_t *l_139 = &g_20;
    int32_t *l_140[3];
    uint8_t l_142[3];
    int i;
    for (i = 0; i < 1; i++)
        l_136[i] = &g_119;
    for (i = 0; i < 3; i++)
        l_140[i] = &l_137;
    for (i = 0; i < 3; i++)
        l_142[i] = 250U;
    g_20 ^= ((l_128 == &p_6) , (((-4) && l_129) , 1));
    l_137 &= ((g_119 = (l_129 <= (safe_sub_func_uint16_t_u_u(65535U, ((1U | (-9)) >= p_6))))) >= p_6);
    ++l_142[1];
    return (*l_138);
}

static uint64_t  func_7(const uint64_t  p_8, int8_t  p_9, int32_t  p_10)
{ 
    int32_t *l_18 = &g_3[1];
    const int32_t *l_24[1];
    const int32_t **l_23 = &l_24[0];
    int32_t l_33 = (-5);
    const uint16_t *l_78 = (void*)0;
    int32_t l_79 = (-1);
    uint32_t l_117 = 0x4B258E78;
    uint16_t l_120 = 0x4036;
    uint32_t l_121 = 4294967295U;
    int i;
    for (i = 0; i < 1; i++)
        l_24[i] = &g_3[1];
    if (p_9)
    { 
        uint8_t l_11 = 0x27;
        return l_11;
    }
    else
    { 
        int32_t *l_12 = &g_3[1];
        int32_t **l_13 = &l_12;
        uint32_t l_21 = 0xE40424B9;
        int32_t l_26 = 1;
        (*l_13) = l_12;
        if (p_9)
            goto lbl_25;
lbl_25:
        for (p_10 = 0; (p_10 >= 4); ++p_10)
        { 
            for (p_9 = (-10); (p_9 < (-2)); p_9++)
            { 
                int32_t *l_22 = &g_20;
                (*l_13) = l_18;
                (*l_22) ^= (l_21 = ((((*l_13) = &g_4) != (g_19 = &g_3[1])) > 4294967295U));
                l_23 = (void*)0;
            }
        }
        if ((l_26 <= (((p_8 == (g_20 > g_3[1])) & p_8) <= 0x967684ABF0A64418LL)))
        { 
            (*l_13) = &p_10;
        }
        else
        { 
            uint64_t *l_32[2];
            uint16_t *l_34 = &g_35;
            int32_t l_36 = (-1);
            int32_t **l_37[1][3];
            int64_t l_38[3];
            int i, j;
            for (i = 0; i < 2; i++)
                l_32[i] = (void*)0;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 3; j++)
                    l_37[i][j] = (void*)0;
            }
            for (i = 0; i < 3; i++)
                l_38[i] = 0xB3B6AC8918CB0B9DLL;
            l_38[0] |= (&g_19 == ((((+(safe_sub_func_uint16_t_u_u(((*l_34) = (((l_33 = ((safe_mul_func_uint16_t_u_u(g_2[3], g_3[1])) ^ g_2[2])) ^ 0xE86DE1F1717C264FLL) , (*l_12))), l_36))) && 0x198FE831) ^ 0xA6) , l_37[0][2]));
            for (p_10 = 0; p_10 < 1; p_10 = p_10 + 1)
            {
                for (l_21 = 0; l_21 < 3; l_21 = l_21 + 1)
                {
                    l_37[p_10][l_21] = &l_18;
                }
            }
            g_19 = &p_10;
        }
    }
    if ((safe_div_func_int32_t_s_s((((g_41 != (p_10 = (*g_19))) , p_10) & p_9), p_9)))
    { 
        int32_t **l_42 = &l_18;
        int32_t *l_43 = (void*)0;
        int32_t *l_44 = &g_20;
        int32_t *l_46[1];
        uint16_t l_47 = 0U;
        uint32_t *l_62 = &g_41;
        int i;
        for (i = 0; i < 1; i++)
            l_46[i] = &l_33;
        (*l_42) = &p_10;
        --l_47;
        if ((((safe_lshift_func_int16_t_s_s(((((safe_add_func_int16_t_s_s(p_9, (((safe_lshift_func_uint32_t_u_s(((*l_62) = ((safe_sub_func_int8_t_s_s(((safe_rshift_func_uint16_t_u_s((safe_mod_func_int64_t_s_s((g_3[0] && g_20), 0xFD363BF156DE04B3LL)), (**l_42))) , g_3[1]), 0xE0)) , 4294967289U)), 23)) | (*l_44)) != (*g_19)))) || 0x08) != g_20) > (*l_44)), 14)) == (*g_19)) == g_35))
        { 
            const int32_t *l_67 = (void*)0;
            int32_t l_81 = 0x00A8D5E9;
            for (p_9 = 0; (p_9 < 15); p_9 = safe_add_func_int64_t_s_s(p_9, 6))
            { 
                int32_t **l_68 = &l_43;
                int64_t *l_69 = (void*)0;
                int64_t *l_70 = &g_45[0];
                (*l_44) = (safe_add_func_int64_t_s_s(((*l_70) |= ((l_67 = (*l_42)) != ((*l_68) = &l_33))), (p_9 & 5)));
            }
            g_80[2] |= (((safe_div_func_uint32_t_u_u(0U, (safe_add_func_uint64_t_u_u((safe_mul_func_int64_t_s_s((+(&g_35 != l_78)), g_3[1])), p_8)))) <= l_79) , (*g_19));
            for (l_47 = 0; (l_47 <= 0); l_47 += 1)
            { 
                int i;
                l_81 |= (((*g_19) , &l_24[0]) != (p_9 , (void*)0));
                g_19 = ((*l_42) = &g_3[1]);
                if ((*g_19))
                    continue;
            }
        }
        else
        { 
            uint8_t l_84 = 0xEF;
            int8_t l_87 = 0xD6;
            const int64_t l_91[3][1] = {{0xF1449E4C9CD7D838LL},{0xF1449E4C9CD7D838LL},{0xF1449E4C9CD7D838LL}};
            int32_t *l_92[2][1];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_92[i][j] = &g_20;
            }
            if (((**l_42) ^= (((safe_add_func_uint16_t_u_u((++l_84), l_87)) < p_9) , (((safe_sub_func_int64_t_s_s((safe_unary_minus_func_uint64_t_u((0xF6 && l_91[0][0]))), p_9)) & g_35) & g_4))))
            { 
                l_42 = &l_18;
                l_92[1][0] = &g_3[1];
            }
            else
            { 
                int8_t *l_93 = &g_2[1];
                (*l_44) = (*g_19);
                (*l_44) ^= (p_10 & ((*l_93) = p_10));
                (*l_42) = &p_10;
            }
        }
    }
    else
    { 
        uint64_t *l_109 = &g_110;
        uint64_t **l_111 = &l_109;
        uint64_t *l_113 = (void*)0;
        uint64_t **l_112 = &l_113;
        int32_t l_114[2];
        uint64_t *l_115 = (void*)0;
        uint64_t *l_116 = &g_110;
        int8_t *l_118 = &g_119;
        int32_t *l_122 = (void*)0;
        int32_t *l_123 = &g_20;
        int i;
        for (i = 0; i < 2; i++)
            l_114[i] = 0;
        (*l_123) = ((safe_sub_func_int64_t_s_s((g_2[2] < (&g_41 != ((safe_add_func_int64_t_s_s(((((safe_mul_func_uint16_t_u_u((((((*l_118) = (((((*l_116) = (~((safe_sub_func_uint32_t_u_u((((safe_div_func_int32_t_s_s((safe_mod_func_uint16_t_u_u((safe_sub_func_int8_t_s_s(((((*l_112) = ((*l_111) = l_109)) != &p_8) & 0xAA), l_114[0])), p_10)), (*g_19))) | g_80[3]) >= g_41), 0xCAE20C25)) <= p_9))) < 0x968035757FEC6B6BLL) <= (*l_18)) & l_117)) || l_120) > p_9) != p_8), p_8)) > p_10) <= 0xC347B9C4) & p_10), p_8)) , (void*)0))), l_121)) != p_9);
        for (l_120 = 0; l_120 < 1; l_120 = l_120 + 1)
        {
            g_45[l_120] = 0xB948F0CD08EF1BB2LL;
        }
        for (g_35 = 0; (g_35 <= 1); g_35 += 1)
        { 
            int32_t **l_124 = (void*)0;
            int32_t **l_125 = &l_123;
            int i;
            (*l_125) = &l_114[g_35];
        }
    }
    return p_10;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_3[i], "g_3[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_45[i], "g_45[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_80[i], "g_80[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_110, "g_110", print_hash_value);
    transparent_crc(g_119, "g_119", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_141[i], "g_141[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_145, "g_145", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
