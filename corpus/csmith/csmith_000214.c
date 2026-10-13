/* Csmith 0cdc710315cf seed 214, flattened by scripts/csmith_flatten.py */
// Options:   --seed 214 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   uint8_t  f1;
   const uint8_t  f2;
   int16_t  f3;
};

struct S1 {
   uint32_t  f0;
};

struct S2 {
   uint8_t  f0;
   uint8_t  f1;
   uint32_t  f2;
   int8_t  f3;
};

struct S3 {
   int32_t  f0;
   const uint8_t  f1;
};

static uint64_t g_4 = 0x449C9FB37B04800FLL;
static int32_t g_20 = 0;
static uint64_t g_28 = 1U;
static const uint64_t **g_43 = (void*)0;
static uint64_t g_48 = 1U;
static uint64_t g_49 = 0xF9C6A002B50D22F7LL;
static uint32_t g_65 = 0xC08F5283;
static uint32_t *g_64[4] = {&g_65,&g_65,&g_65,&g_65};
static struct S1 g_97[1] = {{0xE0669094}};
static struct S2 g_100[3] = {{0x82,4U,0x4FC06BE8,1},{0x82,4U,0x4FC06BE8,1},{0x82,4U,0x4FC06BE8,1}};
static uint8_t g_107 = 6U;

static uint16_t  func_1(void);
static uint8_t  func_5(int16_t  p_6, struct S2  p_7, uint32_t  p_8, uint64_t  p_9, struct S3  p_10);
static struct S2  func_11(int16_t  p_12);

static uint16_t  func_1(void)
{ 
    uint16_t l_14 = 65526U;
    int32_t *l_15[4][1];
    uint16_t l_16[1];
    int8_t l_101[1];
    struct S3 l_102[4][3] = {{{1,0U},{0xD6B031E6,8U},{1,0U}},{{0xF4C769F5,1U},{0x886BE4B2,253U},{-2,0U}},{{0xF4C769F5,1U},{0xF4C769F5,1U},{0x886BE4B2,253U}},{{1,0U},{0x886BE4B2,253U},{0x886BE4B2,253U}}};
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
            l_15[i][j] = (void*)0;
    }
    for (i = 0; i < 1; i++)
        l_16[i] = 1U;
    for (i = 0; i < 1; i++)
        l_101[i] = 0x72;
    g_107 = (safe_div_func_uint16_t_u_u(g_4, (func_5(g_4, func_11((~((l_16[0] = l_14) | 8))), l_101[0], g_100[1].f1, l_102[2][0]) || g_100[1].f1)));
    return g_107;
}

static uint8_t  func_5(int16_t  p_6, struct S2  p_7, uint32_t  p_8, uint64_t  p_9, struct S3  p_10)
{ 
    struct S1 l_105 = {0U};
    for (p_10.f0 = (-25); (p_10.f0 >= 16); p_10.f0++)
    { 
        struct S1 *l_106 = &g_97[0];
        (*l_106) = l_105;
    }
    return p_10.f1;
}

static struct S2  func_11(int16_t  p_12)
{ 
    struct S1 l_19[1] = {{0U}};
    int32_t l_26 = 0xFBFCC013;
    int32_t l_51 = 0x15FE49BA;
    int32_t *l_59 = &g_20;
    int32_t *l_99 = &l_26;
    int i;
    for (p_12 = 24; (p_12 >= (-6)); p_12--)
    { 
        uint32_t l_21 = 4294967287U;
        uint64_t *l_27 = &g_28;
        const struct S3 l_63 = {8,4U};
        uint64_t l_82 = 0x67D69BDFBFBE8317LL;
        for (g_20 = 0; g_20 < 1; g_20 = g_20 + 1)
        {
            struct S1 tmp = {1U};
            l_19[g_20] = tmp;
        }
        if (l_21)
            continue;
        if ((((((safe_mod_func_int64_t_s_s(((safe_sub_func_uint64_t_u_u(l_26, (++(*l_27)))) != ((safe_mod_func_uint16_t_u_u((0 & l_21), l_21)) || g_20)), l_26)) ^ g_4) | l_26) < p_12) ^ p_12))
        { 
            struct S0 l_38[1] = {{-1,0xEE,0x9C,0x5708}};
            int i;
            for (g_28 = 0; (g_28 != 13); ++g_28)
            { 
                int32_t *l_35 = &g_20;
                if (g_4)
                    break;
                (*l_35) |= p_12;
                g_20 &= (255U >= (((&g_4 != ((((safe_rshift_func_uint32_t_u_u(((void*)0 != l_35), 13)) <= g_28) , l_38[0]) , l_27)) > g_4) & g_4));
            }
        }
        else
        { 
            const uint64_t **l_46[1][4];
            int32_t *l_57 = (void*)0;
            struct S1 l_62 = {4294967287U};
            int32_t *l_66[1];
            uint32_t *l_83 = (void*)0;
            uint32_t *l_84 = &g_65;
            int8_t l_85 = 0x6C;
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 4; j++)
                    l_46[i][j] = (void*)0;
            }
            for (i = 0; i < 1; i++)
                l_66[i] = &l_26;
            for (l_26 = (-12); (l_26 >= (-26)); --l_26)
            { 
                const uint64_t *l_42 = &g_28;
                const uint64_t **l_41[3];
                uint32_t *l_47 = &l_21;
                int32_t *l_50 = (void*)0;
                struct S1 l_56 = {0x37B1BDA2};
                int32_t **l_58[3][1];
                int i, j;
                for (i = 0; i < 3; i++)
                    l_41[i] = &l_42;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_58[i][j] = &l_50;
                }
                g_43 = l_41[2];
                l_51 |= (g_28 & (((g_49 = (safe_div_func_int64_t_s_s((((((*l_47) |= (g_43 != l_46[0][0])) != 0xCF75BAE5) <= g_4) , 0x10E3A113D7AFF79CLL), g_48))) || 6U) != g_28));
                l_59 = ((p_12 <= (safe_div_func_uint32_t_u_u((safe_lshift_func_uint16_t_u_s((&g_20 != (l_56 , (void*)0)), g_28)), g_4))) , l_57);
            }
            g_20 ^= (safe_mod_func_int64_t_s_s((l_62 , ((l_63 , l_57) != g_64[2])), 0xBE98B872472F8526LL));
            if ((safe_rshift_func_int8_t_s_u((safe_div_func_int64_t_s_s((safe_lshift_func_int8_t_s_s((((safe_lshift_func_int64_t_s_s(((+p_12) || (safe_div_func_uint16_t_u_u((((p_12 == (safe_sub_func_uint32_t_u_u(((*l_84) = ((safe_mod_func_uint32_t_u_u(l_82, (-10))) >= 8)), g_20))) > 1U) , 0x9319), g_49))), 20)) & l_85) | 0xF952), g_28)), p_12)), 2)))
            { 
                uint16_t l_94[4] = {1U,1U,1U,1U};
                uint32_t l_95[1];
                int32_t l_96 = 0x088E2D1E;
                int i;
                for (i = 0; i < 1; i++)
                    l_95[i] = 0x04212E26;
                l_96 &= (g_20 = (((((safe_mod_func_int8_t_s_s(9, (safe_add_func_uint32_t_u_u(((safe_rshift_func_uint16_t_u_s((safe_div_func_int64_t_s_s(((l_46[0][2] != &l_27) && g_4), p_12)), 1)) , l_94[0]), p_12)))) >= l_95[0]) != p_12) < g_65) | 6U));
                g_20 = g_48;
            }
            else
            { 
                struct S1 *l_98 = &g_97[0];
                (*l_98) = g_97[0];
            }
        }
    }
    (*l_99) &= g_65;
    return g_100[1];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_97[i].f0, "g_97[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_100[i].f0, "g_100[i].f0", print_hash_value);
        transparent_crc(g_100[i].f1, "g_100[i].f1", print_hash_value);
        transparent_crc(g_100[i].f2, "g_100[i].f2", print_hash_value);
        transparent_crc(g_100[i].f3, "g_100[i].f3", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_107, "g_107", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
