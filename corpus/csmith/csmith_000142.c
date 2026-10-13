/* Csmith 0cdc710315cf seed 142, flattened by scripts/csmith_flatten.py */
// Options:   --seed 142 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
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
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static int8_t
safe_unary_minus_func_int8_t_s(int8_t si )
{
  return
    -si;
}
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const int16_t  f0;
   int32_t  f1;
   uint32_t  f2;
   int8_t  f3;
};

struct S1 {
   uint64_t  f0;
   const int32_t  f1;
   int8_t  f2;
};

union U2 {
   int64_t  f0;
   uint16_t  f1;
   uint64_t  f2;
};

union U3 {
   uint16_t  f0;
};

union U4 {
   int64_t  f0;
   struct S1  f1;
   const struct S1  f2;
};

static int32_t g_11 = 0x77278FA8;
static int8_t g_20 = 0xA4;
static uint32_t g_21 = 1U;
static const struct S1 g_32 = {0x5D4826422F8DC70ELL,0x4BB9CEC0,0xDF};
static uint8_t g_38 = 1U;
static union U2 g_39 = {-8};
static int16_t g_41[2] = {0xDCAF,0xDCAF};
static uint64_t g_43 = 0xB63E3B667D15778CLL;
static uint16_t g_60 = 0x8B7F;
static int8_t g_63 = 0x93;
static uint8_t g_64 = 0x9F;
static struct S1 g_95 = {0xE02B323EC0817FF2LL,-9,0x0B};
static int8_t g_113 = 0;
static int16_t g_114 = 0x9A12;
static int32_t g_116 = 1;
static int64_t g_123[4][1] = {{6},{6},{6},{6}};
static int8_t g_125 = 0x89;
static union U4 g_129 = {0x8C81A46AFD11974ALL};
static union U3 g_130 = {0U};
static int16_t *g_136 = &g_114;
static int16_t **g_135 = &g_136;
static int32_t *g_139[2] = {&g_116,&g_116};

static int64_t  func_1(void);
static int32_t * func_2(int32_t * p_3);
static union U4  func_4(int32_t * p_5, const union U3  p_6, int32_t * const  p_7, int32_t * p_8, union U4  p_9);

static int64_t  func_1(void)
{ 
    int32_t *l_10 = &g_11;
    const union U3 l_12 = {0xFCE2};
    int32_t *l_13 = &g_11;
    union U4 l_14[2] = {{0x486B6FEB9E9D60A2LL},{0x486B6FEB9E9D60A2LL}};
    union U2 l_131 = {0x8761BCA9B0E0A5FELL};
    int32_t **l_138 = &l_10;
    int i;
    g_139[1] = ((*l_138) = func_2(((((func_4(l_10, l_12, &g_11, l_13, l_14[0]) , g_130) , l_131) , (*l_13)) , (void*)0)));
    return g_43;
}

static int32_t * func_2(int32_t * p_3)
{ 
    int32_t l_134 = (-5);
    int16_t **l_137 = &g_136;
    for (g_43 = 0; g_43 < 4; g_43 = g_43 + 1)
    {
        for (g_11 = 0; g_11 < 1; g_11 = g_11 + 1)
        {
            g_123[g_43][g_11] = (-8);
        }
    }
    l_137 = (((safe_div_func_uint8_t_u_u((0xB48C356ED06C0765LL != l_134), ((-6) || g_60))) && g_32.f2) , g_135);
    return &g_116;
}

static union U4  func_4(int32_t * p_5, const union U3  p_6, int32_t * const  p_7, int32_t * p_8, union U4  p_9)
{ 
    int32_t l_15[1];
    int32_t *l_16 = &l_15[0];
    int32_t *l_17 = &l_15[0];
    int32_t *l_18 = &l_15[0];
    int32_t *l_19[3][1];
    struct S0 *l_72 = (void*)0;
    int i, j;
    for (i = 0; i < 1; i++)
        l_15[i] = (-1);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_19[i][j] = &l_15[0];
    }
    g_21++;
    if ((safe_lshift_func_int32_t_s_u((*p_7), 3)))
    { 
        uint8_t l_29 = 0xB5;
        struct S1 l_34 = {0x643C8F438361CD5CLL,-5,0xF0};
        int32_t l_42[1][2];
        struct S0 **l_73 = &l_72;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_42[i][j] = 0x64C66072;
        }
        if ((((*p_8) | g_20) , ((((4294967295U ^ (*l_17)) < (*l_16)) >= g_21) > 0U)))
        { 
            int32_t **l_26[3];
            int i;
            for (i = 0; i < 3; i++)
                l_26[i] = &l_18;
            p_8 = &g_11;
        }
        else
        { 
            const int32_t *l_28 = (void*)0;
            const int32_t **l_27 = &l_28;
            (*l_27) = (void*)0;
            (*l_16) = l_29;
        }
        if ((safe_add_func_uint64_t_u_u((g_32 , (((safe_unary_minus_func_int32_t_s((l_34 , (((((g_38 = (~(safe_div_func_uint8_t_u_u(3U, p_6.f0)))) >= p_6.f0) , l_29) , g_39) , (*p_5))))) || 4294967290U) < (-5))), p_9.f0)))
        { 
            int32_t l_40[2];
            int16_t *l_54 = &g_41[1];
            int i;
            for (i = 0; i < 2; i++)
                l_40[i] = 1;
            (*p_8) = g_21;
            --g_43;
            (*l_16) ^= ((*p_7) |= ((safe_lshift_func_uint32_t_u_u(0xEB41E225, (((((*l_54) = (p_9.f0 , (safe_mul_func_int32_t_s_s((safe_mul_func_uint32_t_u_u((safe_add_func_int32_t_s_s(1, g_32.f2)), p_6.f0)), 0x0F86AEFB)))) <= 0x4D3D) && l_40[1]) == 250U))) < 0U));
        }
        else
        { 
            uint8_t l_71 = 0x9C;
            (*p_7) ^= 1;
            for (l_34.f0 = 18; (l_34.f0 >= 27); ++l_34.f0)
            { 
                int32_t **l_57 = &l_16;
                (*l_57) = &g_11;
            }
            if ((safe_sub_func_uint8_t_u_u((g_32.f2 != 0x51), p_6.f0)))
            { 
                return p_9;
            }
            else
            { 
                const int32_t l_70 = (-5);
                ++g_60;
                ++g_64;
                (*l_18) |= (&l_19[1][0] != ((g_41[1] <= (safe_unary_minus_func_uint16_t_u((((((safe_sub_func_uint8_t_u_u(l_70, 0)) , g_43) , 0x73) & 5U) , l_71)))) , (void*)0));
            }
        }
        (*l_73) = l_72;
    }
    else
    { 
        int16_t l_83 = 0xB90C;
        int32_t l_115 = 0x2A6D4945;
        int32_t l_118[1][3];
        uint64_t l_126 = 0x53257E1E0ADED112LL;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_118[i][j] = 1;
        }
        for (g_63 = 0; (g_63 > 22); ++g_63)
        { 
            int32_t *l_81[4];
            int8_t *l_82 = &g_20;
            int i;
            for (i = 0; i < 4; i++)
                l_81[i] = &l_15[0];
            (*l_16) ^= (((*l_82) = (((safe_unary_minus_func_int8_t_s((safe_mod_func_int16_t_s_s((safe_div_func_uint8_t_u_u((0x22B755D73B7D9658LL | 0x494CAA1C64AC225CLL), g_39.f0)), p_9.f0)))) , l_81[2]) == &g_11)) , l_83);
        }
        if ((safe_lshift_func_uint8_t_u_u(l_83, (safe_add_func_uint16_t_u_u(1U, ((p_6.f0 , &g_11) != &g_11))))))
        { 
            int8_t l_92 = 5;
            struct S1 *l_94 = &g_95;
            struct S1 **l_93 = &l_94;
            struct S1 **l_96 = (void*)0;
            struct S1 **l_97 = (void*)0;
            struct S1 *l_99 = &g_95;
            struct S1 **l_98 = &l_99;
            (*l_98) = ((*l_93) = (((*l_17) <= (safe_div_func_uint64_t_u_u((p_9 , (safe_rshift_func_int32_t_s_s(((l_92 && l_83) , g_38), 17))), p_6.f0))) , (void*)0));
        }
        else
        { 
            int32_t l_117 = 1;
            int32_t l_119 = 0xD1ED58CD;
            int32_t l_120 = 0;
            int32_t l_121 = (-3);
            int32_t l_122 = 0x56846699;
            int32_t l_124 = 0x425F5104;
            for (p_9.f1.f2 = 14; (p_9.f1.f2 < 20); p_9.f1.f2 = safe_add_func_uint64_t_u_u(p_9.f1.f2, 4))
            { 
                int8_t l_102 = 0xD1;
                uint16_t *l_105 = &g_60;
                int16_t *l_110 = &g_41[1];
                (*p_7) = ((*l_16) = (l_102 > (g_113 &= (safe_div_func_int64_t_s_s(((--(*l_105)) <= (safe_add_func_int16_t_s_s(((*l_110) = (-8)), (safe_mod_func_uint64_t_u_u(p_9.f0, p_6.f0))))), 0x63F1C15673F556C0LL)))));
            }
            --l_126;
        }
    }
    return g_129;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_11, "g_11", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_32.f0, "g_32.f0", print_hash_value);
    transparent_crc(g_32.f1, "g_32.f1", print_hash_value);
    transparent_crc(g_32.f2, "g_32.f2", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_39.f0, "g_39.f0", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_41[i], "g_41[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_95.f0, "g_95.f0", print_hash_value);
    transparent_crc(g_95.f1, "g_95.f1", print_hash_value);
    transparent_crc(g_95.f2, "g_95.f2", print_hash_value);
    transparent_crc(g_113, "g_113", print_hash_value);
    transparent_crc(g_114, "g_114", print_hash_value);
    transparent_crc(g_116, "g_116", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_123[i][j], "g_123[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_125, "g_125", print_hash_value);
    transparent_crc(g_129.f0, "g_129.f0", print_hash_value);
    transparent_crc(g_130.f0, "g_130.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
