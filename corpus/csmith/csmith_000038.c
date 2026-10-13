/* Csmith 0cdc710315cf seed 38, flattened by scripts/csmith_flatten.py */
// Options:   --seed 38 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
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
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
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
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int8_t  f0;
   uint64_t  f1;
   int64_t  f2;
   uint8_t  f3;
};

struct S1 {
   int8_t  f0;
   const uint64_t  f1;
};

struct S2 {
   int64_t  f0;
};

union U3 {
   struct S1  f0;
   const int16_t  f1;
};

static uint32_t g_20 = 0x0FC36FE5;
static int32_t g_22 = 0xC0E2632C;
static uint32_t g_23 = 0x4ADCA1A1;
static int16_t g_25 = 0x483B;
static struct S2 g_44 = {-10};
static uint8_t g_49[2][1] = {{0xE5},{0xE5}};
static int32_t g_64 = 0;
static int32_t *g_68 = (void*)0;
static int32_t **g_67 = &g_68;
static struct S0 g_82 = {1,0x7A87E153341C0C23LL,1,0x7D};
static struct S0 g_97 = {0xC5,0U,3,0x94};
static union U3 g_100 = {{-8,0x02195449CDFC879DLL}};
static uint8_t g_108 = 0x1F;
static uint8_t * const g_107 = &g_108;
static uint64_t g_132 = 0xB778E667A4CD60ACLL;

static uint32_t  func_1(void);
static int16_t  func_2(int16_t  p_3, int8_t  p_4, struct S0  p_5, int16_t  p_6, uint32_t  p_7);
static int8_t  func_14(int16_t  p_15, int16_t  p_16, int8_t  p_17);

static uint32_t  func_1(void)
{ 
    int32_t *l_21 = &g_22;
    int16_t *l_24 = &g_25;
    int32_t l_26 = 0;
    struct S1 l_90[1] = {{0,18446744073709551611U}};
    struct S2 *l_93 = &g_44;
    struct S0 l_98 = {-6,0xEBD28546E32D400DLL,-1,0x06};
    union U3 *l_99 = &g_100;
    int32_t l_129 = 0;
    int32_t l_130 = 0x78AFB31A;
    int32_t l_131 = 0x7205C45C;
    int i;
    if ((func_2((safe_sub_func_uint16_t_u_u(0xE4B9, (safe_mul_func_int64_t_s_s((safe_rshift_func_int8_t_s_u(func_14(((*l_24) = (safe_lshift_func_int32_t_s_s((((((g_23 = ((*l_21) = g_20)) || (g_22 <= 0x28)) >= g_20) || (*l_21)) < g_20), g_20))), l_26, g_20), 3)), 2U)))), g_20, g_82, g_82.f2, g_82.f2) <= g_82.f0))
    { 
        struct S2 *l_91 = &g_44;
        struct S0 l_102 = {2,1U,-5,0x71};
        for (g_82.f3 = 0; (g_82.f3 >= 9); ++g_82.f3)
        { 
            struct S2 **l_92 = (void*)0;
            l_93 = (l_90[0] , l_91);
            for (g_64 = (-22); (g_64 != 23); g_64 = safe_add_func_int16_t_s_s(g_64, 5))
            { 
                struct S0 *l_96 = &g_97;
                union U3 **l_101 = &l_99;
                (*l_96) = g_82;
                (*l_101) = ((l_98 = g_82) , l_99);
            }
        }
        l_102 = g_82;
    }
    else
    { 
        int32_t *l_111 = (void*)0;
        for (g_23 = 0; g_23 < 2; g_23 = g_23 + 1)
        {
            for (g_82.f1 = 0; g_82.f1 < 1; g_82.f1 = g_82.f1 + 1)
            {
                g_49[g_23][g_82.f1] = 0xED;
            }
        }
        if ((safe_div_func_uint8_t_u_u((safe_mul_func_uint64_t_u_u((((void*)0 == g_107) ^ (*g_107)), (*l_21))), 0x3B)))
        { 
            for (g_64 = 23; (g_64 > (-4)); g_64 = safe_sub_func_uint32_t_u_u(g_64, 2))
            { 
                union U3 *l_112 = &g_100;
                union U3 **l_113 = &l_112;
                l_111 = &l_26;
                (*l_113) = l_112;
            }
        }
        else
        { 
            uint16_t l_116 = 65535U;
            (*l_21) = (safe_rshift_func_uint32_t_u_u(l_116, 20));
            (*l_21) = (l_116 >= 0x203B);
        }
    }
    for (l_98.f2 = 15; (l_98.f2 > (-5)); l_98.f2 = safe_sub_func_int32_t_s_s(l_98.f2, 2))
    { 
        int32_t *l_119 = &l_26;
        int32_t *l_120 = (void*)0;
        int32_t *l_121 = &g_22;
        int32_t *l_122 = &g_64;
        int32_t *l_123 = &g_64;
        int32_t *l_124 = (void*)0;
        int32_t *l_125 = (void*)0;
        int32_t *l_126 = &l_26;
        int32_t *l_127 = &g_22;
        int32_t *l_128[2];
        int i;
        for (i = 0; i < 2; i++)
            l_128[i] = &l_26;
        --g_132;
        if (g_49[1][0])
            continue;
    }
    return (*l_21);
}

static int16_t  func_2(int16_t  p_3, int8_t  p_4, struct S0  p_5, int16_t  p_6, uint32_t  p_7)
{ 
    int32_t l_86 = 0x8B96EC4A;
    int32_t *l_87 = &g_22;
    (*l_87) |= (l_86 = ((((~(safe_add_func_uint8_t_u_u(g_64, g_49[1][0]))) & g_82.f3) < p_5.f0) <= l_86));
    return g_49[1][0];
}

static int8_t  func_14(int16_t  p_15, int16_t  p_16, int8_t  p_17)
{ 
    struct S1 l_27[1][4] = {{{-7,0x09A164E62B500989LL},{-7,0x09A164E62B500989LL},{-7,0x09A164E62B500989LL},{-7,0x09A164E62B500989LL}}};
    const struct S0 l_28 = {0x17,18446744073709551615U,6,1U};
    union U3 l_29 = {{0,0x457720F84B0F4F2DLL}};
    int32_t l_55 = 1;
    int i, j;
    if (((((((l_27[0][2] , 0xEA1BCA3396D56A42LL) < ((l_28 , p_17) < l_28.f0)) || p_16) , l_29) , (void*)0) != (void*)0))
    { 
        uint8_t l_41 = 253U;
        struct S0 l_50 = {0xD1,0x7A07E09FA00A4EDBLL,1,255U};
        int32_t *l_56 = &l_55;
        if ((safe_lshift_func_int8_t_s_s((safe_unary_minus_func_uint8_t_u(((safe_div_func_uint32_t_u_u(((((safe_rshift_func_uint16_t_u_s(((((0x2547 || (safe_mul_func_uint32_t_u_u((safe_sub_func_int16_t_s_s((((p_16 && g_23) & 0xBFD33F11) & p_17), p_15)), g_20))) >= l_28.f0) >= p_17) <= g_20), 7)) || l_41) || 0x299B5F277F141C5ALL) >= g_20), l_41)) > p_16))), 2)))
        { 
            int16_t l_48[2];
            int i;
            for (i = 0; i < 2; i++)
                l_48[i] = (-2);
            for (g_23 = (-5); (g_23 <= 53); g_23 = safe_add_func_uint8_t_u_u(g_23, 6))
            { 
                g_44 = g_44;
            }
            for (l_29.f0.f0 = (-8); (l_29.f0.f0 < 4); l_29.f0.f0++)
            { 
                int32_t *l_47[2][2] = {{&g_22,&g_22},{&g_22,&g_22}};
                int i, j;
                l_48[0] &= ((g_22 > (p_16 >= 7)) | 4294967295U);
                if (g_49[1][0])
                    continue;
                l_55 = ((l_50 , ((((safe_sub_func_uint16_t_u_u(((g_49[1][0] > (((safe_mod_func_uint32_t_u_u(p_15, l_48[1])) & g_44.f0) & p_15)) & 0x81), 0U)) , (-9)) == p_15) , l_48[0])) ^ 0xD5381D84);
            }
        }
        else
        { 
            int32_t **l_57 = &l_56;
            (*l_57) = l_56;
            for (l_41 = 17; (l_41 < 36); l_41++)
            { 
                int32_t **l_60 = (void*)0;
                int32_t *l_62 = &g_22;
                int32_t **l_61 = &l_62;
                if (g_22)
                    break;
                (*l_61) = ((*l_57) = &g_22);
            }
        }
        for (l_50.f0 = 0; (l_50.f0 >= 0); l_50.f0 -= 1)
        { 
            int32_t *l_63 = &g_64;
            (*l_63) |= (*l_56);
            for (g_22 = 0; (g_22 <= 0); g_22 += 1)
            { 
                g_64 = g_44.f0;
            }
        }
    }
    else
    { 
        int32_t * const l_66 = &g_64;
        int32_t * const *l_65 = &l_66;
        uint8_t *l_78 = &g_49[1][0];
        int32_t l_79[2][3] = {{0xE3606EC1,(-7),(-7)},{0xE3606EC1,(-7),(-7)}};
        union U3 *l_80 = &l_29;
        union U3 **l_81 = &l_80;
        int i, j;
        (**l_65) ^= ((l_65 == ((((void*)0 == &l_55) || g_49[1][0]) , g_67)) , (-7));
        l_79[1][2] = (~(g_25 != (safe_add_func_int64_t_s_s((g_25 | (((**l_65) = ((((((safe_mul_func_uint64_t_u_u((safe_mul_func_int16_t_s_s(((safe_mul_func_uint8_t_u_u(((*l_78) = p_17), p_17)) | g_22), 65535U)), g_20)) <= p_16) < (-1)) == 0xCB01B09547896B8ALL) ^ 8) < 0xDA)) ^ (-1))), p_17))));
        (*l_81) = l_80;
    }
    return l_29.f0.f0;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_44.f0, "g_44.f0", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_49[i][j], "g_49[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_82.f0, "g_82.f0", print_hash_value);
    transparent_crc(g_82.f1, "g_82.f1", print_hash_value);
    transparent_crc(g_82.f2, "g_82.f2", print_hash_value);
    transparent_crc(g_82.f3, "g_82.f3", print_hash_value);
    transparent_crc(g_97.f0, "g_97.f0", print_hash_value);
    transparent_crc(g_97.f1, "g_97.f1", print_hash_value);
    transparent_crc(g_97.f2, "g_97.f2", print_hash_value);
    transparent_crc(g_97.f3, "g_97.f3", print_hash_value);
    transparent_crc(g_100.f0.f0, "g_100.f0.f0", print_hash_value);
    transparent_crc(g_100.f0.f1, "g_100.f0.f1", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    transparent_crc(g_132, "g_132", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
