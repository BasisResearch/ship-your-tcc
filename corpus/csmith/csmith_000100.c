/* Csmith 0cdc710315cf seed 100, flattened by scripts/csmith_flatten.py */
// Options:   --seed 100 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
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

static uint64_t g_4[3] = {18446744073709551615U,18446744073709551615U,18446744073709551615U};
static uint64_t g_14 = 0xA86BA47C9D5FAC12LL;
static int16_t g_16 = 0x2AEE;
static int16_t g_18[2][1] = {{(-9)},{(-9)}};
static int32_t g_20 = (-1);
static const int32_t g_42 = 1;
static uint16_t g_50 = 0xC201;
static int32_t g_52 = (-8);
static int32_t *g_51 = &g_52;
static int64_t g_77 = 0xB415C855A4044A2BLL;
static int64_t * const g_76 = &g_77;
static const int32_t *g_78 = &g_52;
static uint32_t g_126 = 0x7E429369;
static int64_t g_136 = 0x4325E2F7FA0514B5LL;
static int64_t g_137 = (-3);
static int32_t g_138 = (-5);
static int16_t g_140 = 9;
static uint8_t g_141 = 249U;

static int16_t  func_1(void);
static uint8_t  func_5(int32_t  p_6, int8_t  p_7, const uint64_t  p_8, int64_t  p_9);
static int32_t  func_10(uint64_t  p_11);

static int16_t  func_1(void)
{ 
    int64_t l_12[1];
    uint64_t *l_13 = &g_14;
    int16_t *l_15[3];
    int32_t l_17 = (-10);
    const uint32_t l_79 = 4294967288U;
    int16_t l_106 = 8;
    int16_t l_121 = (-1);
    int32_t l_135 = (-1);
    int16_t l_139[2];
    uint16_t l_146[2];
    int32_t *l_147 = &g_20;
    int i;
    for (i = 0; i < 1; i++)
        l_12[i] = 0x7B8F5F4990069F30LL;
    for (i = 0; i < 3; i++)
        l_15[i] = &g_16;
    for (i = 0; i < 2; i++)
        l_139[i] = 2;
    for (i = 0; i < 2; i++)
        l_146[i] = 0x4EA8;
    if ((safe_sub_func_int8_t_s_s(g_4[1], func_5(func_10(((l_17 = ((g_4[1] & ((*l_13) = l_12[0])) == 0xDFA831EB2391CB4ELL)) == l_12[0])), g_4[1], l_79, (*g_76)))))
    { 
        int8_t l_103 = 0x52;
        int32_t *l_113 = (void*)0;
        int32_t *l_122 = &l_17;
        if (((g_16 &= (((safe_add_func_uint32_t_u_u(((((0x3B10 || l_103) , (safe_sub_func_int8_t_s_s((l_106 | 0x1DA9CD7CB0FD4E32LL), l_12[0]))) , (void*)0) == (void*)0), l_103)) , l_103) , g_18[1][0])) , 0x087D55B8))
        { 
lbl_117:
            for (g_50 = 0; (g_50 <= 2); g_50 += 1)
            { 
                uint64_t l_111 = 0x95AFD930511C70F4LL;
                int32_t *l_112 = &g_20;
                int32_t **l_114 = &g_51;
                int i;
                (*l_112) = ((safe_add_func_uint32_t_u_u((safe_rshift_func_int32_t_s_s(l_111, (l_15[g_50] != &g_16))), (*g_78))) == (-1));
                (*l_114) = l_113;
                (*l_112) &= (safe_mod_func_int32_t_s_s((*g_78), 1U));
            }
        }
        else
        { 
            if (l_79)
                goto lbl_117;
        }
        l_122 = ((((*l_13) = ((safe_mul_func_uint16_t_u_u(g_20, ((safe_unary_minus_func_int64_t_s(l_12[0])) < ((l_121 = 0xB619) && g_16)))) || g_4[1])) <= 18446744073709551611U) , (void*)0);
    }
    else
    { 
        uint32_t *l_125[2];
        int64_t *l_127 = &l_12[0];
        int64_t **l_128 = (void*)0;
        int64_t *l_130[2][4] = {{&l_12[0],&l_12[0],&l_12[0],&l_12[0]},{&l_12[0],&l_12[0],&l_12[0],&l_12[0]}};
        int64_t **l_129 = &l_130[0][0];
        int32_t l_131 = 0x4DF62E77;
        int32_t *l_132 = &g_20;
        int32_t *l_133 = &l_17;
        int32_t *l_134[1];
        int i, j;
        for (i = 0; i < 2; i++)
            l_125[i] = &g_126;
        for (i = 0; i < 1; i++)
            l_134[i] = &g_20;
        l_131 &= (safe_sub_func_uint32_t_u_u((g_126 = l_121), (l_127 != ((*l_129) = (((*g_76) & (*g_76)) , &g_77)))));
        g_52 ^= l_12[0];
        --g_141;
    }
    (*l_147) ^= (g_52 > ((safe_add_func_uint64_t_u_u(0U, l_146[0])) & 7U));
    return (*l_147);
}

static uint8_t  func_5(int32_t  p_6, int8_t  p_7, const uint64_t  p_8, int64_t  p_9)
{ 
    uint8_t l_100 = 0U;
    for (p_9 = 0; (p_9 >= 13); p_9++)
    { 
        int16_t *l_89 = &g_18[1][0];
        int32_t l_90 = 0x75427816;
        if (((safe_mul_func_uint32_t_u_u(((((safe_mod_func_uint32_t_u_u((safe_add_func_int64_t_s_s((+(((l_89 == l_89) || (((0x3C ^ 0x34) , &g_78) == &g_51)) == 0x1C2D9EA4)), l_90)), (*g_78))) && 0xCCD07D1F) != p_9) < l_90), 1U)) ^ l_90))
        { 
            uint64_t l_99 = 0U;
            for (p_7 = 0; (p_7 >= (-25)); p_7 = safe_sub_func_uint8_t_u_u(p_7, 6))
            { 
                int16_t *l_97 = &g_18[1][0];
                int32_t *l_98[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_98[i] = &g_20;
                g_52 = (((((safe_mul_func_int8_t_s_s((safe_lshift_func_uint8_t_u_u(0x25, p_7)), ((void*)0 == l_97))) && g_52) && 0x3BB5) , &g_4[1]) == (void*)0);
                l_99 |= (&g_4[1] == (void*)0);
            }
        }
        else
        { 
            return g_50;
        }
    }
    return l_100;
}

static int32_t  func_10(uint64_t  p_11)
{ 
    int16_t l_23 = 3;
    int32_t l_24 = 7;
    int64_t l_48 = 0xB3FA3CE724D257E7LL;
    for (g_16 = 0; (g_16 >= 0); g_16 -= 1)
    { 
        uint16_t l_25 = 65527U;
        int64_t l_35 = 0;
        int32_t l_38[3][2] = {{0x8F15EA37,0x8F15EA37},{0,0x8F15EA37},{0x8F15EA37,0}};
        int i, j;
        if (g_4[2])
        { 
            int32_t *l_19 = &g_20;
            int32_t *l_21 = &g_20;
            int32_t *l_22[4][3];
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 3; j++)
                    l_22[i][j] = &g_20;
            }
            --l_25;
        }
        else
        { 
            int32_t *l_29[1][3];
            int32_t **l_28 = &l_29[0][0];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 3; j++)
                    l_29[i][j] = &l_24;
            }
            (*l_28) = &g_20;
            for (g_14 = 0; (g_14 <= 0); g_14 += 1)
            { 
                uint8_t l_34 = 0x79;
                (**l_28) = (safe_add_func_int64_t_s_s((safe_mul_func_uint8_t_u_u((((((void*)0 == &p_11) , (void*)0) == (void*)0) != p_11), 1U)), p_11));
                (**l_28) = (l_34 &= l_24);
                if (l_35)
                    break;
            }
            for (l_25 = 0; (l_25 <= 0); l_25 += 1)
            { 
                uint64_t l_39 = 18446744073709551607U;
                const int32_t *l_41 = &g_42;
                const int32_t **l_40 = &l_41;
                uint64_t *l_49 = &l_39;
                g_20 |= (((safe_add_func_int64_t_s_s(((1 > l_38[1][1]) > l_39), 0x438E213633E7A329LL)) ^ p_11) && g_4[1]);
                g_50 |= ((((&l_24 == ((*l_40) = (*l_28))) , ((!((*l_49) = (safe_lshift_func_uint8_t_u_u((safe_rshift_func_int64_t_s_s((*l_41), l_48)), 5)))) <= 0x6E0AEE48)) , (void*)0) == &l_38[0][1]);
            }
        }
        for (g_20 = 0; (g_20 <= 0); g_20 += 1)
        { 
            int32_t *l_54 = &g_52;
            int32_t **l_53 = &l_54;
            (*l_53) = (g_51 = &g_20);
            if (p_11)
                break;
        }
        for (l_25 = 0; (l_25 <= 0); l_25 += 1)
        { 
            int32_t **l_59 = &g_51;
            for (g_14 = 0; (g_14 <= 0); g_14 += 1)
            { 
                (*g_51) = (-2);
            }
            (**l_59) = (safe_div_func_uint32_t_u_u(((((p_11 & ((!(+p_11)) | ((l_59 != (void*)0) | l_25))) && g_42) == p_11) != l_24), (*g_51)));
        }
    }
    if (l_48)
    { 
        int16_t l_73 = 1;
        for (g_50 = 0; (g_50 != 12); g_50 = safe_add_func_int64_t_s_s(g_50, 3))
        { 
            int32_t **l_62 = (void*)0;
            int32_t **l_63 = &g_51;
            int32_t l_66 = 0x96E547EF;
            (*l_63) = &l_24;
            (**l_63) = 0xD72D30A9;
            for (l_24 = 0; (l_24 == (-26)); l_24 = safe_sub_func_uint8_t_u_u(l_24, 2))
            { 
                l_66 &= p_11;
            }
        }
        (*g_51) = (((safe_mul_func_int64_t_s_s((safe_mul_func_int32_t_s_s(l_23, p_11)), (((safe_sub_func_int32_t_s_s(p_11, p_11)) != 255U) >= (-9)))) , l_73) || p_11);
    }
    else
    { 
        int16_t l_74 = 8;
        int64_t *l_75 = &l_48;
        l_74 = ((-1) >= p_11);
        (*g_51) |= (((((((*l_75) = l_24) , ((void*)0 != g_76)) ^ p_11) < 0x4F9076B3EC5F6624LL) >= 0) , 0);
        g_78 = &g_42;
    }
    return p_11;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_4[i], "g_4[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_18[i][j], "g_18[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_126, "g_126", print_hash_value);
    transparent_crc(g_136, "g_136", print_hash_value);
    transparent_crc(g_137, "g_137", print_hash_value);
    transparent_crc(g_138, "g_138", print_hash_value);
    transparent_crc(g_140, "g_140", print_hash_value);
    transparent_crc(g_141, "g_141", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
