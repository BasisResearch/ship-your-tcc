/* Csmith 0cdc710315cf seed 168, flattened by scripts/csmith_flatten.py */
// Options:   --seed 168 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
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
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
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
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
static int32_t
safe_unary_minus_func_int32_t_s(int32_t si )
{
  return
    (si==(-2147483647-1)) ?
    ((si)) :
    -si;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint64_t  f0;
};

struct S1 {
   int16_t  f0;
   uint64_t  f1;
};

struct S2 {
   const uint16_t  f0;
   const uint64_t  f1;
   uint8_t  f2;
};

union U3 {
   int64_t  f0;
   const int8_t  f1;
   uint8_t  f2;
};

static int32_t g_3[3][2] = {{0xE96B779C,0x31D26EC1},{0x31D26EC1,0xE96B779C},{0x31D26EC1,0x31D26EC1}};
static int32_t g_17 = (-2);
static int64_t g_20 = 0x38E5161E33ECDC0FLL;
static int32_t g_21 = 0x33D73C12;
static int32_t g_22 = 0x3395CC9E;
static uint8_t g_23 = 1U;
static uint8_t g_26 = 0U;
static int16_t g_28 = 1;
static struct S1 g_46 = {0x0450,0x9A24791A13C53D17LL};
static int32_t *g_55 = &g_21;
static uint32_t g_61 = 4294967295U;
static int8_t g_73 = 1;
static uint32_t *g_81 = &g_61;
static uint32_t **g_80 = &g_81;
static struct S0 g_89 = {1U};

static int16_t  func_1(void);
static int16_t  func_11(const uint32_t  p_12, union U3  p_13);
static union U3  func_35(const struct S2  p_36);

static int16_t  func_1(void)
{ 
    struct S1 l_2[4][4] = {{{7,0x4B4B13281D21F1EBLL},{0xD4EA,0x801FC3EEE2FDE2BBLL},{7,0x4B4B13281D21F1EBLL},{7,0x4B4B13281D21F1EBLL}},{{0xD4EA,0x801FC3EEE2FDE2BBLL},{0xD4EA,0x801FC3EEE2FDE2BBLL},{0,3U},{0xD4EA,0x801FC3EEE2FDE2BBLL}},{{0xD4EA,0x801FC3EEE2FDE2BBLL},{7,0x4B4B13281D21F1EBLL},{7,0x4B4B13281D21F1EBLL},{0xD4EA,0x801FC3EEE2FDE2BBLL}},{{7,0x4B4B13281D21F1EBLL},{0xD4EA,0x801FC3EEE2FDE2BBLL},{7,0x4B4B13281D21F1EBLL},{7,0x4B4B13281D21F1EBLL}}};
    int32_t l_29 = 0x3CC114FF;
    const struct S2 l_37 = {0xC5CC,0x9CF48B058B506EE4LL,0x12};
    union U3 l_112 = {-1};
    uint8_t l_125[3][3] = {{0xCE,0xCE,0xCE},{252U,252U,252U},{0xCE,0xCE,0xCE}};
    int32_t **l_130[3];
    int i, j;
    for (i = 0; i < 3; i++)
        l_130[i] = &g_55;
    for (g_3[2][1] = 0; (g_3[2][1] <= 3); g_3[2][1] += 1)
    { 
        uint16_t l_6 = 0x73A4;
        const uint16_t l_14 = 0x4131;
        union U3 l_15[4][2] = {{{1},{1}},{{1},{1}},{{1},{1}},{{1},{1}}};
        int16_t *l_27 = &g_28;
        int32_t *l_30[3][1];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_30[i][j] = &g_22;
        }
        g_21 ^= (((safe_add_func_int32_t_s_s((l_6 , (((*l_27) ^= (safe_lshift_func_uint16_t_u_s((safe_rshift_func_uint16_t_u_s(0x9333, func_11(l_14, l_15[0][1]))), g_26))) >= l_14)), l_29)) & 0) | g_26);
        for (g_26 = 0; (g_26 <= 3); g_26 += 1)
        { 
            int8_t l_111 = 0x82;
        }
        if (l_37.f1)
        { 
            uint16_t l_114[2];
            int i;
            for (i = 0; i < 2; i++)
                l_114[i] = 1U;
            return l_114[0];
        }
        else
        { 
            uint8_t l_123[3];
            uint8_t *l_124 = &l_112.f2;
            int i;
            for (i = 0; i < 3; i++)
                l_123[i] = 9U;
            g_17 &= (safe_rshift_func_uint64_t_u_s((0x99 < ((safe_unary_minus_func_int32_t_s((safe_div_func_int32_t_s_s((g_73 ^ (((((l_125[2][0] |= ((safe_unary_minus_func_uint64_t_u(g_73)) || l_123[1])) & l_29) , &l_123[1]) == (void*)0) == g_23)), (**g_80))))) == g_21)), g_23));
        }
        for (g_89.f0 = 0; (g_89.f0 <= 3); g_89.f0 += 1)
        { 
            uint64_t *l_128 = &g_46.f1;
            int32_t l_129 = 0xA7943A2B;
            for (l_112.f0 = 0; l_112.f0 < 3; l_112.f0 = l_112.f0 + 1)
            {
                for (g_20 = 0; g_20 < 3; g_20 = g_20 + 1)
                {
                    l_125[l_112.f0][g_20] = 0U;
                }
            }
            l_129 &= ((safe_add_func_uint64_t_u_u(g_23, ((*l_128) = ((g_26 >= (-1)) , g_21)))) | l_125[0][0]);
            for (g_46.f0 = 0; (g_46.f0 <= 3); g_46.f0 += 1)
            { 
                if ((*g_55))
                    break;
            }
        }
    }
    g_55 = &g_21;
    g_55 = &g_21;
    return g_26;
}

static int16_t  func_11(const uint32_t  p_12, union U3  p_13)
{ 
    int32_t *l_16 = &g_17;
    int32_t *l_18 = &g_17;
    int32_t *l_19[3][1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_19[i][j] = &g_17;
    }
    (*l_16) &= g_3[2][1];
    g_23++;
    return p_13.f0;
}

static union U3  func_35(const struct S2  p_36)
{ 
    uint64_t l_38 = 18446744073709551615U;
    struct S1 *l_45 = &g_46;
    int32_t l_65 = (-8);
    struct S0 * const l_88 = &g_89;
    struct S0 *l_91 = &g_89;
    struct S0 **l_90 = &l_91;
    int32_t *l_97[2];
    union U3 l_98 = {-5};
    int i;
    for (i = 0; i < 2; i++)
        l_97[i] = &g_21;
    if ((l_38 > (safe_sub_func_uint32_t_u_u(g_22, (g_28 != l_38)))))
    { 
        int32_t *l_42 = &g_22;
        struct S1 *l_48 = &g_46;
        (*l_42) = (!1);
        for (g_22 = 19; (g_22 <= (-7)); g_22 = safe_sub_func_uint64_t_u_u(g_22, 9))
        { 
            struct S1 **l_47[4];
            int i;
            for (i = 0; i < 4; i++)
                l_47[i] = &l_45;
            l_48 = l_45;
        }
    }
    else
    { 
        uint16_t l_51 = 0U;
        int32_t *l_52 = &g_21;
        uint64_t l_53[4][1] = {{0U},{18446744073709551608U},{0U},{18446744073709551608U}};
        int64_t l_59 = 1;
        int32_t l_67 = 0x7D6112DD;
        uint32_t *l_79[2];
        uint32_t **l_78[4] = {&l_79[0],&l_79[0],&l_79[0],&l_79[0]};
        int i, j;
        for (i = 0; i < 2; i++)
            l_79[i] = &g_61;
        (*l_52) ^= (l_38 && (safe_rshift_func_int16_t_s_s((g_46 , ((l_38 , g_46.f1) < 0x3599)), l_51)));
        for (g_28 = 0; (g_28 >= 0); g_28 -= 1)
        { 
            uint32_t l_56 = 4294967288U;
            int32_t l_77 = 0x93F460CB;
            for (l_38 = 0; (l_38 <= 0); l_38 += 1)
            { 
                int32_t *l_54 = (void*)0;
                g_55 = l_54;
            }
            if (l_56)
            { 
                uint32_t *l_60[1];
                int32_t l_64[3];
                int32_t *l_66[2];
                uint32_t **l_68 = &l_60[0];
                uint32_t **l_69 = (void*)0;
                uint32_t *l_71[3];
                uint32_t **l_70[1][3];
                uint32_t *l_72 = (void*)0;
                int i, j;
                for (i = 0; i < 1; i++)
                    l_60[i] = &g_61;
                for (i = 0; i < 3; i++)
                    l_64[i] = (-1);
                for (i = 0; i < 2; i++)
                    l_66[i] = &g_17;
                for (i = 0; i < 3; i++)
                    l_71[i] = &l_56;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 3; j++)
                        l_70[i][j] = &l_71[0];
                }
                l_67 ^= ((l_65 = ((safe_lshift_func_uint32_t_u_u((((((l_59 <= (((((*l_52) = ((g_61 ^= l_56) || ((safe_mul_func_int32_t_s_s(((-1) == g_61), p_36.f1)) , 0x2454177E))) , l_64[1]) || 250U) >= l_56)) < p_36.f0) || p_36.f1) != p_36.f0) , 0U), g_3[2][1])) ^ g_22)) != g_23);
                (*l_52) = ((l_52 != (l_72 = ((*l_68) = &g_61))) != p_36.f1);
                g_73 = l_56;
            }
            else
            { 
                int32_t *l_76 = &g_3[0][0];
                l_77 |= (0x7432CFE0 == (safe_lshift_func_uint32_t_u_u((&l_67 != l_76), p_36.f2)));
                g_80 = l_78[2];
            }
            for (g_22 = 0; (g_22 <= 0); g_22 += 1)
            { 
                int i, j;
                if (l_53[(g_28 + 2)][g_22])
                    break;
            }
        }
        for (g_46.f1 = 0; (g_46.f1 <= 3); g_46.f1 += 1)
        { 
            for (l_65 = 1; (l_65 >= 0); l_65 -= 1)
            { 
                struct S1 *l_82 = (void*)0;
                l_82 = l_45;
            }
            for (g_20 = 1; (g_20 >= 0); g_20 -= 1)
            { 
                int32_t *l_87 = &l_65;
                int i;
                (*l_87) &= ((*l_52) = (&l_79[g_20] != ((0x67C2D953 > (safe_lshift_func_uint8_t_u_s((safe_rshift_func_int32_t_s_s(p_36.f0, 6)), g_28))) , &g_81)));
            }
        }
    }
    (*l_90) = l_88;
    g_21 &= (~(((g_17 = ((g_22 = (safe_sub_func_uint64_t_u_u(0xA7A75AA8E5449298LL, (safe_lshift_func_uint64_t_u_s(0x23BFC9F35A7B369ALL, l_65))))) >= 0xF1AF8A64)) || p_36.f0) , 0xC10F06FB));
    return l_98;
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
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_3[i][j], "g_3[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_46.f0, "g_46.f0", print_hash_value);
    transparent_crc(g_46.f1, "g_46.f1", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_73, "g_73", print_hash_value);
    transparent_crc(g_89.f0, "g_89.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
