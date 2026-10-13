/* Csmith 0cdc710315cf seed 87, flattened by scripts/csmith_flatten.py */
// Options:   --seed 87 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
};

struct S1 {
   uint64_t  f0;
   uint16_t  f1;
   const struct S0  f2;
   uint16_t  f3;
};

union U2 {
   uint16_t  f0;
   uint16_t  f1;
};

union U3 {
   uint16_t  f0;
   const int8_t  f1;
   int32_t  f2;
};

union U4 {
   struct S1  f0;
   int64_t  f1;
   const int16_t  f2;
};

union U5 {
   struct S1  f0;
};

static int16_t g_17 = 5;
static int8_t g_18 = 0xC5;
static int32_t g_33 = 1;
static union U4 g_47[4][4] = {{{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}}},{{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}}},{{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}}},{{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}},{{0U,0x6322,{0x57},0U}}}};
static int16_t g_57 = 0xF555;
static uint32_t g_63 = 4294967295U;
static int32_t *g_78 = (void*)0;
static int16_t g_85 = 5;
static int64_t g_86 = 1;
static int64_t g_87 = 0x492C623687E89A4CLL;
static int16_t g_88[1] = {1};
static uint16_t g_89[1][1] = {{0xD304}};
static int16_t *g_102 = (void*)0;
static union U3 g_105 = {0xCA82};
static int16_t g_110[4][3] = {{0x77CE,0x77CE,0x77CE},{0x77CE,0x77CE,0x77CE},{0x77CE,0x77CE,0x77CE},{0x77CE,0x77CE,0x77CE}};
static uint8_t g_111[4] = {255U,255U,255U,255U};
static int32_t g_115 = (-1);
static struct S0 g_117 = {6U};
static struct S0 *g_116 = &g_117;
static struct S0 *g_119 = &g_117;

static struct S1  func_1(void);
static int32_t  func_2(union U3  p_3, uint32_t  p_4);
static union U3  func_5(int16_t  p_6);

static struct S1  func_1(void)
{ 
    int16_t l_11 = 7;
    int16_t *l_16 = &g_17;
    int32_t *l_114 = &g_115;
    struct S0 **l_118[1];
    struct S1 l_120 = {5U,0U,{0xFF},65532U};
    int i;
    for (i = 0; i < 1; i++)
        l_118[i] = &g_116;
    (*l_114) &= func_2(func_5(((safe_mod_func_int8_t_s_s((safe_div_func_uint8_t_u_u(((l_11 ^ ((safe_sub_func_int16_t_s_s(((*l_16) = (safe_add_func_int64_t_s_s(l_11, (0U | l_11)))), g_18)) == g_18)) >= g_18), l_11)), l_11)) && l_11)), l_11);
    g_119 = (g_116 = g_116);
    return l_120;
}

static int32_t  func_2(union U3  p_3, uint32_t  p_4)
{ 
    int32_t *l_106 = &g_105.f2;
    int32_t *l_107 = &g_105.f2;
    int32_t *l_108 = &g_105.f2;
    int32_t *l_109[3];
    int i;
    for (i = 0; i < 3; i++)
        l_109[i] = &g_33;
    --g_111[2];
    return p_4;
}

static union U3  func_5(int16_t  p_6)
{ 
    int32_t l_19[4];
    uint32_t l_25 = 6U;
    int32_t *l_39 = &g_33;
    union U2 l_41 = {0U};
    int64_t l_48[1][4] = {{(-7),(-7),(-7),(-7)}};
    int32_t l_60 = 0x88663631;
    union U5 l_71 = {{6U,0x13E6,{1U},65535U}};
    int i, j;
    for (i = 0; i < 4; i++)
        l_19[i] = 0;
    for (p_6 = 3; (p_6 >= 0); p_6 -= 1)
    { 
        int32_t l_62 = 0x7D9BA741;
        int32_t *l_81 = &l_19[p_6];
        int32_t *l_82 = &g_33;
        int32_t *l_83 = &l_19[1];
        int32_t *l_84[2];
        struct S0 l_95 = {251U};
        int i;
        for (i = 0; i < 2; i++)
            l_84[i] = &l_62;
        if (l_19[p_6])
        { 
            struct S1 l_24 = {0x5F5A2F617A9413E4LL,0x8C45,{0xA9},0x2A19};
            union U2 l_30[2][4] = {{{0x8069},{65535U},{65535U},{0x8069}},{{65535U},{0x8069},{65535U},{65535U}}};
            union U3 l_38 = {65527U};
            int16_t *l_46 = &g_17;
            int32_t l_61 = 0x7D0B67C4;
            int i, j;
            if ((safe_add_func_uint32_t_u_u((safe_sub_func_uint16_t_u_u(((l_24 , p_6) || (65531U | 0x2A6C)), l_25)), (-1))))
            { 
                int16_t *l_31[2][3] = {{(void*)0,(void*)0,&g_17},{(void*)0,(void*)0,&g_17}};
                int32_t *l_32 = &g_33;
                int i, j;
                (*l_32) |= (safe_lshift_func_uint32_t_u_s((safe_rshift_func_int16_t_s_s(8, (g_17 &= (l_30[1][3] , l_19[p_6])))), 6));
            }
            else
            { 
                uint32_t l_37 = 4294967295U;
                int32_t *l_40 = &l_19[3];
                if (l_25)
                    break;
                (*l_40) ^= ((((+((((((((safe_sub_func_uint32_t_u_u(l_37, ((l_38 , (l_39 = &g_33)) != (void*)0))) || 0x618B2576953C5357LL) > g_17) || l_37) >= l_24.f3) < l_37) > l_37) && l_30[1][3].f0)) == p_6) != p_6) , p_6);
            }
            if ((l_41 , ((safe_rshift_func_uint32_t_u_s(l_19[p_6], (safe_mul_func_int32_t_s_s(g_17, 0xF100F83B)))) <= p_6)))
            { 
                (*l_39) ^= (((void*)0 == l_46) ^ (g_47[2][0] , g_18));
                if (l_48[0][3])
                    break;
            }
            else
            { 
                int8_t l_52[1];
                int32_t *l_58 = &l_38.f2;
                int32_t *l_59[1][3];
                int i, j;
                for (i = 0; i < 1; i++)
                    l_52[i] = 0x52;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 3; j++)
                        l_59[i][j] = &l_38.f2;
                }
                l_60 |= (safe_div_func_uint32_t_u_u((+(l_52[0] ^ (safe_add_func_uint32_t_u_u((g_57 &= ((safe_mod_func_int16_t_s_s((l_24.f2 , (g_33 < 0U)), g_33)) >= l_52[0])), p_6)))), l_19[p_6]));
                --g_63;
            }
            (*l_39) = p_6;
        }
        else
        { 
            int32_t *l_74 = &l_19[p_6];
            (*l_74) |= ((*l_39) = ((safe_add_func_int64_t_s_s((safe_add_func_int64_t_s_s((!(p_6 & ((((((l_71 , (safe_div_func_uint64_t_u_u(p_6, g_17))) > 9) > g_33) == 1U) == l_62) > 0xA4))), 0x62155B9876582894LL)), g_33)) > (-1)));
            for (g_17 = 0; (g_17 != 10); g_17 = safe_add_func_uint32_t_u_u(g_17, 5))
            { 
                int32_t **l_77[3][3] = {{&l_39,&l_39,&l_39},{&l_39,&l_39,&l_39},{&l_39,&l_39,&l_39}};
                int i, j;
                g_78 = &l_19[p_6];
                (*l_39) = (safe_add_func_int16_t_s_s(p_6, 65528U));
                if ((*g_78))
                    break;
            }
        }
        ++g_89[0][0];
        for (g_57 = 0; (g_57 <= (-3)); g_57 = safe_sub_func_int16_t_s_s(g_57, 3))
        { 
            int8_t l_94 = 0x4B;
            union U2 l_104 = {0x98A5};
            (*l_82) &= g_57;
            (*l_81) = (l_94 <= (((l_95 , ((safe_sub_func_uint16_t_u_u((*l_39), g_87)) | g_47[2][0].f0.f1)) ^ p_6) < 1U));
            if ((*l_39))
            { 
                int16_t *l_101 = &g_17;
                int16_t **l_100[2][1];
                int32_t l_103 = 0x2A6C391F;
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_100[i][j] = &l_101;
                }
                (*l_81) |= (((safe_lshift_func_int8_t_s_u((((*l_82) = ((g_102 = &g_57) == (void*)0)) & p_6), 2)) , 0x4E94C435) | p_6);
                if (l_103)
                    continue;
            }
            else
            { 
                (*l_81) = ((l_104 , g_105) , (g_47[2][0].f0 , 0x551584D3));
            }
        }
    }
    return g_105;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_33, "g_33", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_47[i][j].f0.f0, "g_47[i][j].f0.f0", print_hash_value);
            transparent_crc(g_47[i][j].f0.f1, "g_47[i][j].f0.f1", print_hash_value);
            transparent_crc(g_47[i][j].f0.f2.f0, "g_47[i][j].f0.f2.f0", print_hash_value);
            transparent_crc(g_47[i][j].f0.f3, "g_47[i][j].f0.f3", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_86, "g_86", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_88[i], "g_88[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_89[i][j], "g_89[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_105.f0, "g_105.f0", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_110[i][j], "g_110[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_111[i], "g_111[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_115, "g_115", print_hash_value);
    transparent_crc(g_117.f0, "g_117.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
