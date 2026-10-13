/* Csmith 0cdc710315cf seed 132, flattened by scripts/csmith_flatten.py */
// Options:   --seed 132 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (4294967295U >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   uint64_t  f1;
   uint8_t  f2;
};

struct S1 {
   int64_t  f0;
   uint64_t  f1;
   int32_t  f2;
};

struct S2 {
   const struct S0  f0;
};

static int64_t g_11 = 1;
static int32_t g_27 = 1;
static struct S0 g_29 = {9,0x55ADB51FD9AB8A87LL,6U};
static int8_t g_55 = 0;
static int32_t g_63 = 0xB5C50EC3;
static uint16_t g_64 = 65527U;
static uint16_t g_66 = 65535U;
static uint8_t g_69 = 255U;
static struct S2 g_101[4] = {{{3,0xF6575EB915481186LL,248U}},{{3,0xF6575EB915481186LL,248U}},{{3,0xF6575EB915481186LL,248U}},{{3,0xF6575EB915481186LL,248U}}};
static int16_t g_106 = (-2);
static int32_t g_110 = 0x660493B4;
static int8_t g_111 = 0;
static uint16_t g_112 = 0xB806;

static uint8_t  func_1(void);
static struct S2  func_4(int64_t  p_5, uint64_t  p_6, struct S1  p_7, int64_t  p_8);
static uint16_t  func_12(uint8_t  p_13, int32_t  p_14, int16_t  p_15);

static uint8_t  func_1(void)
{ 
    int16_t l_22[2][2];
    uint16_t *l_65 = &g_66;
    int64_t *l_67 = &g_11;
    int64_t *l_68[1][1];
    struct S1 l_70 = {2,0x52F506A54EE8E20ELL,0xE86CD98C};
    uint16_t l_103 = 1U;
    int64_t l_104 = 0xBE15C8DD589E83D4LL;
    int32_t *l_105[2][3] = {{(void*)0,(void*)0,(void*)0},{&g_63,&g_63,&g_63}};
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
            l_22[i][j] = 9;
    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_68[i][j] = (void*)0;
    }
    g_106 = ((((safe_mul_func_uint8_t_u_u((((g_29.f0 = ((func_4((g_69 |= ((*l_67) = (safe_mod_func_int32_t_s_s(g_11, (((*l_65) &= func_12((safe_div_func_uint64_t_u_u((safe_add_func_int8_t_s_s(((((safe_add_func_uint16_t_u_u(l_22[1][1], g_11)) >= 4294967293U) , l_22[1][1]) > 0x595AD3380EE91B76LL), 0x38)), l_22[1][1])), g_11, l_22[1][1])) , 0x44CAD706))))), g_29.f2, l_70, g_29.f0) , 1U) >= l_103)) < l_22[1][0]) > l_104), g_29.f2)) && g_29.f2) <= l_22[1][1]) <= l_22[1][1]);
    for (g_69 = 0; (g_69 != 3); g_69++)
    { 
        int32_t l_109[1];
        int i;
        for (i = 0; i < 1; i++)
            l_109[i] = (-1);
        --g_112;
        l_109[0] = g_101[2].f0.f0;
    }
    return g_101[2].f0.f0;
}

static struct S2  func_4(int64_t  p_5, uint64_t  p_6, struct S1  p_7, int64_t  p_8)
{ 
    int16_t l_73 = 0x0A5F;
    struct S0 *l_78 = &g_29;
    struct S2 l_102 = {{1,1U,247U}};
    for (g_55 = 18; (g_55 >= (-14)); g_55 = safe_sub_func_uint32_t_u_u(g_55, 1))
    { 
        struct S0 *l_82 = (void*)0;
        int32_t l_98 = 0x0A06BCA3;
        if (l_73)
        { 
            int32_t *l_81 = &g_63;
            if (p_6)
            { 
                uint16_t l_74 = 1U;
                int32_t *l_77 = &g_63;
                l_74++;
                (*l_77) &= p_7.f1;
                l_78 = ((*l_77) , &g_29);
            }
            else
            { 
                struct S0 *l_79 = &g_29;
                int32_t *l_80 = &g_63;
                (*l_80) &= ((p_8 > 254U) || (((((void*)0 != l_79) == g_55) >= g_27) != p_6));
            }
            (*l_81) ^= l_73;
        }
        else
        { 
            struct S0 **l_83 = &l_82;
            (*l_83) = l_82;
            for (p_7.f2 = 0; (p_7.f2 > (-19)); p_7.f2 = safe_sub_func_int8_t_s_s(p_7.f2, 1))
            { 
                int32_t *l_87 = (void*)0;
                int32_t **l_86 = &l_87;
                int64_t *l_97 = &g_11;
                int64_t **l_96 = &l_97;
                (*l_86) = &g_63;
                (**l_86) = (**l_86);
                (*l_87) = (safe_div_func_int16_t_s_s(p_6, ((safe_mul_func_int16_t_s_s(((safe_sub_func_uint8_t_u_u(((safe_lshift_func_uint64_t_u_s(((((*l_96) = &p_5) == &p_5) | 0x3EC5), g_11)) , (**l_86)), p_7.f2)) != l_98), p_7.f0)) ^ 1)));
            }
        }
        for (g_63 = 13; (g_63 < (-5)); --g_63)
        { 
            return g_101[2];
        }
    }
    if (l_73)
    { 
        return g_101[2];
    }
    else
    { 
        return l_102;
    }
}

static uint16_t  func_12(uint8_t  p_13, int32_t  p_14, int16_t  p_15)
{ 
    uint64_t l_25 = 0x2FA5D8720725FC1CLL;
    int32_t *l_26 = &g_27;
    (*l_26) |= (((p_13 < (safe_mod_func_uint64_t_u_u(l_25, l_25))) , p_13) & 0);
    if (p_15)
    { 
        struct S0 * const l_28 = &g_29;
        struct S0 *l_31 = &g_29;
        struct S0 **l_30 = &l_31;
        (*l_30) = l_28;
        (*l_26) = ((safe_sub_func_int32_t_s_s((*l_26), 1U)) > (safe_lshift_func_uint32_t_u_s((safe_mul_func_uint16_t_u_u(((safe_add_func_int64_t_s_s(((safe_lshift_func_int8_t_s_u(p_15, 0)) , (-7)), p_15)) , 0x837E), g_29.f0)), 4)));
    }
    else
    { 
        int32_t *l_46 = (void*)0;
        for (p_13 = (-26); (p_13 >= 38); p_13 = safe_add_func_uint32_t_u_u(p_13, 8))
        { 
            int16_t l_49 = 1;
            for (g_29.f1 = 7; (g_29.f1 != 7); g_29.f1 = safe_add_func_int16_t_s_s(g_29.f1, 8))
            { 
                l_46 = &g_27;
            }
            for (l_25 = 0; (l_25 >= 21); l_25++)
            { 
                return l_49;
            }
            for (p_14 = 0; (p_14 < 7); p_14 = safe_add_func_uint16_t_u_u(p_14, 2))
            { 
                int8_t *l_54 = &g_55;
                int32_t *l_62 = &g_63;
                g_64 &= ((*l_62) ^= (safe_div_func_int8_t_s_s(((((((*l_54) = 0xEE) , (safe_mul_func_uint64_t_u_u((safe_mod_func_int32_t_s_s((safe_mul_func_int32_t_s_s(((*l_26) = (g_55 , p_14)), p_13)), (-9))), (-5)))) && p_14) & p_13) <= l_49), p_13)));
            }
        }
    }
    l_26 = &p_14;
    return g_55;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_11, "g_11", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_29.f0, "g_29.f0", print_hash_value);
    transparent_crc(g_29.f1, "g_29.f1", print_hash_value);
    transparent_crc(g_29.f2, "g_29.f2", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_66, "g_66", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_101[i].f0.f0, "g_101[i].f0.f0", print_hash_value);
        transparent_crc(g_101[i].f0.f1, "g_101[i].f0.f1", print_hash_value);
        transparent_crc(g_101[i].f0.f2, "g_101[i].f0.f2", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_106, "g_106", print_hash_value);
    transparent_crc(g_110, "g_110", print_hash_value);
    transparent_crc(g_111, "g_111", print_hash_value);
    transparent_crc(g_112, "g_112", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
