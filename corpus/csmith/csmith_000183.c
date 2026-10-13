/* Csmith 0cdc710315cf seed 183, flattened by scripts/csmith_flatten.py */
// Options:   --seed 183 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   int32_t  f0;
   int32_t  f1;
   int16_t  f2;
};

union U1 {
   int16_t  f0;
   int32_t  f1;
};

union U5 {
   int16_t  f0;
   const int8_t  f1;
};

union U6 {
   int32_t  f0;
   const int32_t  f1;
   int16_t  f2;
};

union U7 {
   int64_t  f0;
};

union U8 {
   const int32_t  f0;
   int16_t  f1;
};

union U9 {
   uint32_t  f0;
   int32_t  f1;
};

union U10 {
   const uint64_t  f0;
   int64_t  f1;
   uint32_t  f2;
};

static int32_t g_3[4] = {(-7),(-7),(-7),(-7)};
static int32_t g_4 = 1;
static int32_t g_7 = 1;
static int32_t *g_22 = &g_4;
static int32_t **g_21 = &g_22;
static int32_t g_25 = 1;
static int32_t g_26[1][4] = {{1,1,1,1}};
static uint32_t g_29 = 4294967295U;
static uint16_t g_59 = 65530U;
static uint8_t g_70 = 0U;
static uint16_t *g_87 = &g_59;
static const union U6 *g_94[2] = {(void*)0,(void*)0};
static union U5 g_99 = {-6};
static union U5 * const g_98 = &g_99;
static union U9 *g_110 = (void*)0;
static union U0 g_116 = {5};
static union U6 g_126 = {-1};
static int32_t g_133 = 0x33DDCFDB;
static union U8 g_136 = {0x0C1D3939};
static union U7 g_138 = {0x5BE5548FEC598316LL};
static union U7 *g_137 = &g_138;

static union U6  func_1(void);
static int32_t  func_8(union U10  p_9, int32_t ** p_10, int32_t * const * p_11, int64_t  p_12, union U8  p_13);
static union U10  func_14(int8_t  p_15, const union U1  p_16, int32_t ** p_17, union U8  p_18, int32_t  p_19);

static union U6  func_1(void)
{ 
    int16_t l_2[3][1];
    union U8 l_23 = {7};
    int16_t l_74[3][4] = {{1,1,0,1},{1,0xFD8E,0xFD8E,1},{0xFD8E,1,0xFD8E,0xFD8E}};
    uint16_t *l_78[2];
    uint16_t **l_77 = &l_78[1];
    uint32_t *l_97 = &g_29;
    uint64_t l_115 = 1U;
    union U6 l_147 = {0x6FE6C662};
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_2[i][j] = 0;
    }
    for (i = 0; i < 2; i++)
        l_78[i] = (void*)0;
    for (g_3[3] = 0; (g_3[3] >= 0); g_3[3] -= 1)
    { 
        int32_t * const *l_33 = &g_22;
        union U8 l_34 = {0};
        int32_t l_68 = 0x284727EF;
        uint16_t *l_86 = &g_59;
        union U6 l_96 = {0xB1135B97};
        for (g_4 = 0; (g_4 <= 0); g_4 += 1)
        { 
            int32_t *l_5 = &g_3[3];
            int32_t **l_6 = &l_5;
            uint16_t *l_73 = (void*)0;
            int i, j;
            (*l_6) = l_5;
            if (l_2[g_3[3]][g_3[3]])
                break;
            for (g_7 = 0; (g_7 >= 0); g_7 -= 1)
            { 
                const union U1 l_20 = {0x2A42};
                uint8_t *l_69 = &g_70;
                l_68 = func_8(func_14(l_2[0][0], l_20, g_21, l_23, l_20.f0), &l_5, l_33, g_25, l_34);
                g_26[0][2] &= (l_74[2][2] = ((((*l_69) = g_29) == ((**g_21) <= (l_73 == &g_59))) <= l_20.f0));
            }
        }
        for (g_29 = 0; (g_29 <= 0); g_29 += 1)
        { 
            uint16_t *l_76 = &g_59;
            uint16_t **l_75[4] = {&l_76,&l_76,&l_76,&l_76};
            uint8_t *l_81 = &g_70;
            int32_t l_92 = 0;
            int i;
            for (g_25 = 0; (g_25 <= 0); g_25 += 1)
            { 
                (**l_33) ^= 0;
            }
            l_77 = l_75[0];
            if ((((safe_div_func_uint8_t_u_u((--(*l_81)), (safe_sub_func_uint8_t_u_u(249U, (((*l_77) = l_86) == (g_87 = &g_59)))))) & g_29) | l_74[2][1]))
            { 
                int32_t *l_93[3][2] = {{&g_26[0][2],&g_26[0][2]},{&g_26[0][2],&g_26[0][2]},{&g_26[0][2],&g_26[0][2]}};
                int i, j;
                l_68 = ((**l_33) = ((safe_lshift_func_int64_t_s_s(((((safe_add_func_uint16_t_u_u(((void*)0 != &g_29), (((0x1D1A91C28AAA867CLL & 0xF394D215D55B7793LL) || 0x3C) == (-5)))) , g_3[0]) < g_29) | l_92), g_7)) > 0));
            }
            else
            { 
                const union U6 **l_95 = &g_94[0];
                (*l_95) = g_94[0];
                return l_96;
            }
        }
    }
    if ((l_97 == &g_29))
    { 
        union U5 *l_101[1][2];
        union U5 **l_100 = &l_101[0][0];
        int16_t *l_108 = &g_99.f0;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_101[i][j] = &g_99;
        }
        (*g_22) |= 0xD94E14C3;
        (*l_100) = g_98;
        (*g_22) = (safe_div_func_uint8_t_u_u((safe_add_func_uint16_t_u_u(l_74[2][2], (*g_87))), (safe_mod_func_int16_t_s_s(((*l_108) = 0x2717), (*g_87)))));
    }
    else
    { 
        const int8_t l_109[4][1] = {{0x4F},{0x4F},{0x4F},{0x4F}};
        int i, j;
        (**g_21) = l_109[2][0];
        (*g_21) = l_97;
        g_110 = (void*)0;
    }
    if ((safe_div_func_int64_t_s_s(((safe_mul_func_int8_t_s_s(((void*)0 == &g_94[0]), g_99.f0)) & l_115), l_2[0][0])))
    { 
        union U0 *l_117[4];
        union U6 *l_125 = &g_126;
        int32_t l_132 = 8;
        int i;
        for (i = 0; i < 4; i++)
            l_117[i] = &g_116;
        (**g_21) = ((g_116 = g_116) , ((safe_sub_func_int8_t_s_s((safe_div_func_int32_t_s_s(((void*)0 == l_97), 1)), g_25)) > (*g_22)));
lbl_127:
        (*g_22) = 2;
        for (g_116.f0 = 28; (g_116.f0 <= 24); g_116.f0--)
        { 
            const union U1 l_134 = {0x6F2B};
            if (((+((g_26[0][3] == (((void*)0 == l_125) <= l_2[1][0])) | 0xA549CE4963A11E95LL)) || g_59))
            { 
                (*g_22) ^= 5;
            }
            else
            { 
                int32_t **l_135 = &g_22;
                if (g_7)
                    goto lbl_127;
                (**g_21) = ((safe_add_func_int64_t_s_s((0xA33AE1349419EC0FLL >= (func_14((safe_sub_func_int64_t_s_s((g_133 = l_132), g_29)), l_134, l_135, g_136, (*g_22)) , l_132)), 0U)) ^ 0x0D);
                g_137 = g_137;
            }
        }
    }
    else
    { 
        int8_t l_144 = 0;
        for (g_25 = 0; (g_25 < (-15)); g_25 = safe_sub_func_uint8_t_u_u(g_25, 2))
        { 
            const int32_t l_146[2] = {4,4};
            int i;
            if ((**g_21))
            { 
                (*g_22) &= (g_26[0][2] , 0xC73AE716);
                if ((*g_22))
                    break;
            }
            else
            { 
                union U7 *l_141 = &g_138;
                union U6 *l_145 = &g_126;
                if ((*g_22))
                    break;
                (*g_22) = ((((((&g_138 == l_141) && (safe_lshift_func_uint16_t_u_s((*g_87), l_115))) > l_144) , l_145) == l_145) && (*g_87));
                if (l_144)
                    continue;
            }
            (**g_21) = l_146[0];
        }
    }
    return l_147;
}

static int32_t  func_8(union U10  p_9, int32_t ** p_10, int32_t * const * p_11, int64_t  p_12, union U8  p_13)
{ 
    uint32_t *l_35 = &g_29;
    int32_t l_38[4][3];
    int32_t l_39 = (-8);
    uint32_t l_61 = 1U;
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
            l_38[i][j] = 0;
    }
lbl_42:
    l_39 ^= (((*l_35)--) , l_38[3][0]);
    for (l_39 = 0; (l_39 <= (-20)); l_39 = safe_sub_func_int32_t_s_s(l_39, 9))
    { 
        uint16_t l_45[1];
        int32_t *l_56 = &l_38[3][0];
        uint16_t *l_57 = &l_45[0];
        uint16_t *l_58 = &g_59;
        int i;
        for (i = 0; i < 1; i++)
            l_45[i] = 0x212A;
        if (l_39)
            goto lbl_42;
        for (g_25 = 0; (g_25 <= 2); g_25 += 1)
        { 
            int32_t *l_43 = &l_38[(g_25 + 1)][g_25];
            int32_t *l_44[1];
            int i, j;
            for (i = 0; i < 1; i++)
                l_44[i] = &g_26[0][2];
            l_45[0]--;
        }
        if ((((safe_rshift_func_uint64_t_u_s((safe_lshift_func_uint16_t_u_s((g_26[0][2] , ((*l_58) = (((*l_57) = ((-1) && (((*l_56) = (*g_22)) != g_7))) != (-1)))), g_3[0])), l_39)) > 0) >= p_13.f0))
        { 
            return (**g_21);
        }
        else
        { 
            int32_t *l_60 = &l_38[2][1];
            uint16_t *l_66 = &l_45[0];
            uint16_t **l_67 = &l_66;
            l_61--;
            if ((**p_10))
                break;
            (*l_60) = (safe_add_func_int64_t_s_s(l_38[3][0], ((((*l_67) = l_66) != &l_45[0]) <= 0)));
        }
    }
    return (**p_11);
}

static union U10  func_14(int8_t  p_15, const union U1  p_16, int32_t ** p_17, union U8  p_18, int32_t  p_19)
{ 
    int32_t *l_24[4];
    int32_t l_27[3];
    int64_t l_28 = (-5);
    union U10 l_32 = {0x0C227312B8DC093CLL};
    int i;
    for (i = 0; i < 4; i++)
        l_24[i] = &g_25;
    for (i = 0; i < 3; i++)
        l_27[i] = 0x4B34BCD0;
    g_29--;
    return l_32;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_3[i], "g_3[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_26[i][j], "g_26[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_99.f0, "g_99.f0", print_hash_value);
    transparent_crc(g_116.f0, "g_116.f0", print_hash_value);
    transparent_crc(g_126.f0, "g_126.f0", print_hash_value);
    transparent_crc(g_133, "g_133", print_hash_value);
    transparent_crc(g_136.f0, "g_136.f0", print_hash_value);
    transparent_crc(g_138.f0, "g_138.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
