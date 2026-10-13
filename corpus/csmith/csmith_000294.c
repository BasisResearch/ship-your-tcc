/* Csmith 0cdc710315cf seed 294, flattened by scripts/csmith_flatten.py */
// Options:   --seed 294 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
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
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
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
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
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
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
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
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
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

static uint16_t g_5[1] = {7U};
static int32_t g_6 = 0x555B7CEF;
static int64_t g_9 = 0xD744D0289A695768LL;
static int32_t g_17 = 1;
static uint64_t g_26 = 0xADCA1A1F050678E0LL;
static int32_t g_28 = 1;
static uint32_t g_40 = 0xF7A0211E;
static uint16_t g_56[1][3] = {{0x8168,0x8168,0x8168}};
static uint8_t g_75 = 0U;
static int16_t g_87 = 0x777E;
static uint64_t g_88 = 0x360B9F0A6C5FA607LL;
static uint32_t g_138 = 0x592CB3A5;

static int16_t  func_1(void);
static int32_t * func_29(uint32_t  p_30, uint8_t  p_31);
static int64_t * func_32(int32_t * p_33, uint32_t  p_34, uint16_t  p_35);

static int16_t  func_1(void)
{ 
    int8_t l_2 = 0x78;
    uint8_t l_153 = 0xD2;
    int32_t *l_158 = (void*)0;
    int32_t **l_159 = &l_158;
    int16_t *l_165 = &g_87;
    if (l_2)
    { 
        for (l_2 = 10; (l_2 == 27); l_2 = safe_add_func_uint16_t_u_u(l_2, 1))
        { 
            for (g_6 = 0; (g_6 <= 0); g_6 += 1)
            { 
                int64_t *l_7 = (void*)0;
                int64_t *l_8 = &g_9;
                int32_t *l_14 = (void*)0;
                int32_t *l_15 = (void*)0;
                int32_t *l_16 = &g_17;
                int i;
                (*l_16) = (((*l_8) &= g_5[g_6]) && (safe_lshift_func_uint16_t_u_s((++g_5[g_6]), 0)));
            }
            return g_5[0];
        }
    }
    else
    { 
        uint16_t l_24 = 0x0E26;
        uint64_t *l_25 = &g_26;
        int32_t *l_27 = &g_28;
        int64_t *l_151 = &g_9;
        int64_t **l_150 = &l_151;
        uint8_t *l_152 = &g_75;
        (*l_27) &= (safe_div_func_int64_t_s_s(g_17, ((*l_25) |= (safe_div_func_int16_t_s_s(((g_9 = g_5[0]) <= (safe_add_func_int16_t_s_s(((g_5[0] | 5U) , l_24), g_6))), l_2)))));
        l_158 = func_29((((*l_152) = ((*l_27) , (((*l_150) = func_32(&g_17, (g_40 = ((((safe_mul_func_int32_t_s_s((safe_div_func_int16_t_s_s((-2), (*l_27))), g_6)) , g_26) && (*l_27)) <= 0xBA09A164E62B5009LL)), (*l_27))) != &g_9))) , l_153), g_5[0]);
    }
    (*l_159) = &g_28;
    (**l_159) = (safe_unary_minus_func_int64_t_s((safe_div_func_uint32_t_u_u(((g_56[0][1] & g_26) <= ((safe_mul_func_int64_t_s_s((((*l_165) ^= (0x31 & 0xA1)) | (**l_159)), g_138)) ^ (**l_159))), 1U))));
    return (*l_158);
}

static int32_t * func_29(uint32_t  p_30, uint8_t  p_31)
{ 
    int32_t l_155[3];
    int32_t *l_156[3][1];
    int32_t **l_157 = &l_156[0][0];
    int i, j;
    for (i = 0; i < 3; i++)
        l_155[i] = 0x7285EB11;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_156[i][j] = (void*)0;
    }
    g_6 = (safe_unary_minus_func_int64_t_s(l_155[0]));
    l_157 = &l_156[2][0];
    return &g_6;
}

static int64_t * func_32(int32_t * p_33, uint32_t  p_34, uint16_t  p_35)
{ 
    uint8_t l_48 = 247U;
    int32_t l_80 = 7;
    int32_t l_81 = 0x0179E4C0;
    int32_t l_82 = (-5);
    int32_t l_85 = (-10);
    int32_t l_86 = (-1);
    uint64_t *l_97 = (void*)0;
    if (((p_35 & 4) || g_40))
    { 
        int32_t *l_41[3];
        int i;
        for (i = 0; i < 3; i++)
            l_41[i] = &g_17;
        l_41[1] = p_33;
    }
    else
    { 
        int32_t l_58 = 1;
        uint32_t *l_72 = &g_40;
        int32_t l_83 = 0x7A58E81D;
        int32_t l_84[2][2] = {{9,9},{9,9}};
        int32_t *l_104 = &g_28;
        int i, j;
lbl_78:
        for (g_6 = (-4); (g_6 != 3); g_6 = safe_add_func_uint16_t_u_u(g_6, 1))
        { 
            uint32_t *l_49 = &g_40;
            int32_t l_69[3];
            int i;
            for (i = 0; i < 3; i++)
                l_69[i] = 0;
            if ((safe_rshift_func_uint16_t_u_u((safe_mul_func_uint64_t_u_u(((((((*l_49) = l_48) <= ((-5) <= p_35)) ^ l_48) < (-2)) , p_35), g_17)), p_34)))
            { 
                uint64_t l_57 = 1U;
                uint64_t *l_59 = &g_26;
                g_17 &= (((safe_add_func_uint64_t_u_u(l_48, ((*l_59) = (p_35 ^ (safe_lshift_func_int16_t_s_u(((safe_mul_func_int8_t_s_s((g_56[0][2] = ((0x0E98 <= g_28) == p_34)), l_57)) | l_58), 4)))))) && g_40) >= l_58);
            }
            else
            { 
                uint32_t l_68 = 0x7BC07E7F;
                (*p_33) &= (safe_add_func_int16_t_s_s((safe_mul_func_int64_t_s_s(p_35, ((safe_mul_func_uint64_t_u_u(g_40, (((safe_rshift_func_uint8_t_u_u((l_68 && g_40), 4)) != p_34) == l_69[2]))) > p_35))), l_69[2]));
                (*p_33) ^= l_48;
                if (l_68)
                    continue;
            }
            return &g_9;
        }
        if ((safe_sub_func_int16_t_s_s((&p_34 == l_72), l_48)))
        { 
            (*p_33) = (safe_lshift_func_int16_t_s_s((((g_75 = 1) || (l_58 == ((safe_add_func_int16_t_s_s(0x5052, g_28)) , 0x6F60D197A07E09FALL))) ^ (*p_33)), 13));
        }
        else
        { 
            int32_t *l_79[4] = {&g_6,&g_6,&g_6,&g_6};
            int i;
            if (g_6)
                goto lbl_78;
            g_88++;
            for (l_85 = (-29); (l_85 < 4); l_85 = safe_add_func_int16_t_s_s(l_85, 5))
            { 
                const int8_t l_98 = 0x93;
                int32_t l_99[1][2];
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_99[i][j] = 5;
                }
                (*p_33) = (l_99[0][1] |= ((g_6 = (safe_add_func_uint64_t_u_u((safe_sub_func_int32_t_s_s(((void*)0 != l_97), 0xAEF9AD35)), l_98))) ^ (*p_33)));
            }
        }
        (*l_104) &= (safe_rshift_func_int32_t_s_u(l_82, (l_84[1][0] < (safe_lshift_func_uint32_t_u_u((((g_17 != l_80) > 0xA9) && p_34), p_35)))));
    }
    if (g_75)
    { 
        int32_t l_113 = 0x66D82068;
        int32_t *l_114 = (void*)0;
        g_28 &= (safe_rshift_func_int8_t_s_s((safe_rshift_func_uint32_t_u_u(((safe_mod_func_uint16_t_u_u((p_35 ^ ((((8 < (*p_33)) && 0) == p_35) , l_113)), g_56[0][2])) & 1U), 0)), g_6));
    }
    else
    { 
        int32_t *l_115 = &g_6;
        int32_t *l_116 = &l_80;
        int32_t *l_117 = &l_85;
        int32_t *l_118 = (void*)0;
        int32_t *l_119 = &l_82;
        int32_t *l_120 = &l_82;
        int32_t l_121 = 0x9AC72693;
        int32_t *l_122 = &l_82;
        int32_t *l_123 = &l_85;
        int32_t *l_124 = &l_81;
        int32_t l_125 = 6;
        int32_t *l_126 = &g_28;
        int32_t *l_127 = (void*)0;
        int32_t *l_128 = &l_86;
        int32_t *l_129 = (void*)0;
        int32_t *l_130 = &l_85;
        int32_t *l_131 = &g_6;
        int32_t l_132 = 0;
        int32_t *l_133 = &l_86;
        int32_t *l_134 = (void*)0;
        int32_t *l_135 = &l_80;
        int32_t *l_136 = (void*)0;
        int32_t *l_137[1];
        uint32_t *l_145[3];
        int64_t *l_149 = &g_9;
        int i;
        for (i = 0; i < 1; i++)
            l_137[i] = &l_121;
        for (i = 0; i < 3; i++)
            l_145[i] = &g_40;
        g_138++;
        if (g_88)
            goto lbl_148;
        if (l_82)
            goto lbl_148;
lbl_148:
        (*l_122) = (((safe_rshift_func_int8_t_s_s(((safe_sub_func_uint32_t_u_u((g_40 = ((void*)0 != p_33)), (safe_lshift_func_int16_t_s_s(((p_34 == g_17) < g_88), 15)))) , 0x1C), (*l_117))) , l_86) != g_28);
        return l_149;
    }
    return &g_9;
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
        transparent_crc(g_5[i], "g_5[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_56[i][j], "g_56[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_75, "g_75", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    transparent_crc(g_88, "g_88", print_hash_value);
    transparent_crc(g_138, "g_138", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
