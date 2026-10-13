/* Csmith 0cdc710315cf seed 52, flattened by scripts/csmith_flatten.py */
// Options:   --seed 52 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
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
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
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
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint64_t  f0;
   int64_t  f1;
   int8_t  f2;
};

struct S2 {
   uint16_t  f0;
   uint32_t  f1;
   int32_t  f2;
};

union U3 {
   uint64_t  f0;
   uint32_t  f1;
   const int64_t  f2;
};

union U4 {
   struct S0  f0;
};

static int32_t g_3 = 0x0EF307D7;
static int32_t g_4 = 0xE32CF415;
static uint64_t g_28 = 0xDD2E91C84132D38CLL;
static union U4 g_30 = {{0x49C4B05EF13550E3LL,-1,-2}};
static struct S2 g_50 = {0x825D,0x284A8988,-1};
static struct S2 *g_49[4] = {&g_50,&g_50,&g_50,&g_50};
static uint32_t g_55 = 0xD7787123;
static int64_t g_60 = 0xC3DB88773635D5AALL;
static int32_t g_61 = 0x1B86553E;
static int16_t g_62 = 0xC9D4;
static int8_t g_63 = (-1);
static int16_t g_67[3] = {0x21BC,0x21BC,0x21BC};
static uint64_t g_70 = 0xBCF703B84D8593D4LL;
static int32_t *g_86 = &g_3;
static uint32_t g_95 = 4U;
static int16_t g_116 = 0x812E;
static int8_t g_117 = 0x73;
static int32_t g_120[4] = {1,1,1,1};

static int16_t  func_1(void);
static uint8_t  func_9(struct S0  p_10, uint32_t  p_11, struct S0  p_12);
static uint64_t  func_17(uint32_t  p_18, int8_t  p_19, union U4  p_20, int64_t  p_21, const uint64_t  p_22);

static int16_t  func_1(void)
{ 
    uint64_t l_2[4];
    struct S0 l_130 = {18446744073709551610U,0xF5253A83723A6807LL,8};
    int32_t l_133 = 0x04EBC73C;
    int i;
    for (i = 0; i < 4; i++)
        l_2[i] = 1U;
    for (g_3 = 3; (g_3 >= 0); g_3 -= 1)
    { 
        int8_t l_29 = 0xA3;
        struct S0 *l_134 = &g_30.f0;
        struct S0 *l_135 = (void*)0;
        struct S0 *l_136 = &g_30.f0;
        struct S0 *l_137 = &l_130;
        for (g_4 = 0; (g_4 <= 3); g_4 += 1)
        { 
            uint64_t *l_27 = &g_28;
            int32_t *l_132[4];
            int i;
            for (i = 0; i < 4; i++)
                l_132[i] = &g_120[2];
            l_133 ^= (safe_sub_func_int32_t_s_s((safe_div_func_int64_t_s_s(l_2[g_3], (func_9((((safe_add_func_uint32_t_u_u(g_4, (safe_div_func_int8_t_s_s(((func_17(((safe_mod_func_uint16_t_u_u((safe_rshift_func_int16_t_s_s(((((*l_27) = (l_2[2] ^ g_3)) == 0xC1E0F758FC5674FALL) && g_28), 7)), 0xDFD9)) < l_29), g_4, g_30, l_29, l_2[g_3]) , l_2[g_3]) <= l_29), l_2[1])))) < 1) , g_30.f0), g_50.f1, l_130) ^ l_2[g_3]))), 0x21386D8F));
            return g_4;
        }
        (*l_137) = g_30.f0;
        return g_120[2];
    }
    return g_50.f1;
}

static uint8_t  func_9(struct S0  p_10, uint32_t  p_11, struct S0  p_12)
{ 
    int32_t *l_131 = &g_50.f2;
    (*l_131) &= (*g_86);
    return p_10.f1;
}

static uint64_t  func_17(uint32_t  p_18, int8_t  p_19, union U4  p_20, int64_t  p_21, const uint64_t  p_22)
{ 
    uint32_t l_31 = 0x0040B319;
    uint64_t *l_39[2];
    int32_t l_40 = 1;
    uint8_t l_41 = 0x10;
    union U4 l_44[2][3] = {{{{4U,0x6AE20245597EEDF6LL,0x79}},{{4U,0x6AE20245597EEDF6LL,0x79}},{{4U,0x6AE20245597EEDF6LL,0x79}}},{{{4U,0x6AE20245597EEDF6LL,0x79}},{{4U,0x6AE20245597EEDF6LL,0x79}},{{4U,0x6AE20245597EEDF6LL,0x79}}}};
    uint32_t *l_45 = &l_31;
    int32_t *l_47 = &l_40;
    struct S2 *l_48 = (void*)0;
    int32_t l_54 = 0xA8C97D73;
    int32_t l_64 = (-1);
    int32_t l_65 = 0x39219EC1;
    int32_t l_66 = 6;
    int32_t l_69[1];
    uint64_t l_122 = 0x7EB6903EB460E39ELL;
    int i, j;
    for (i = 0; i < 2; i++)
        l_39[i] = (void*)0;
    for (i = 0; i < 1; i++)
        l_69[i] = 1;
    if ((0U || ((*l_45) = (l_31 <= (safe_rshift_func_uint64_t_u_s((safe_mod_func_int32_t_s_s((((safe_lshift_func_int32_t_s_s((~((p_20.f0.f0 &= (++l_41)) && (p_20.f0.f0 = (l_44[1][1] , 0U)))), l_44[1][1].f0.f2)) < g_28) && g_30.f0.f0), g_3)), 52))))))
    { 
        int8_t l_51 = 0;
        int32_t l_52 = (-9);
        int32_t l_53 = 9;
        int32_t l_68[4] = {0x4242223A,0x4242223A,0x4242223A,0x4242223A};
        int32_t *l_87 = &l_64;
        int i;
        for (p_20.f0.f0 = 0; (p_20.f0.f0 <= 1); p_20.f0.f0 += 1)
        { 
            int32_t **l_46[3][3];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                    l_46[i][j] = (void*)0;
            }
            l_47 = &g_3;
            g_49[3] = l_48;
            for (g_50.f2 = 0; (g_50.f2 <= 1); g_50.f2 += 1)
            { 
                int32_t l_58 = (-1);
                int32_t l_59[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_59[i] = 0x55BD1B84;
                ++g_55;
                l_53 &= p_21;
                g_70--;
            }
            for (g_30.f0.f0 = 0; (g_30.f0.f0 <= 1); g_30.f0.f0 += 1)
            { 
                struct S2 l_81[2] = {{0xED5C,0xB5B2332F,4},{0xED5C,0xB5B2332F,4}};
                int i;
                l_52 ^= (((((((safe_mod_func_int8_t_s_s(((g_61 == 0x59) , (safe_lshift_func_uint8_t_u_u((safe_rshift_func_int64_t_s_s(((((((((((safe_mul_func_uint8_t_u_u((l_81[0] , p_20.f0.f0), p_19)) || 0xE29F52E1657FB82ELL) || p_20.f0.f1) ^ p_22) || l_81[0].f1) | 1U) <= l_68[3]) != g_67[1]) != l_81[0].f2) ^ (*l_47)), l_81[0].f1)), (*l_47)))), p_20.f0.f0)) || g_63) , p_22) & g_61) != g_67[1]) > p_20.f0.f2) > 0);
                l_68[1] = 0;
                l_81[0].f2 |= (l_68[2] = (safe_sub_func_uint8_t_u_u((safe_sub_func_int8_t_s_s((p_21 ^ ((&g_3 != g_86) ^ p_20.f0.f1)), p_20.f0.f1)), p_18)));
            }
        }
        (*l_87) = 0x9C88BF8F;
    }
    else
    { 
        uint32_t l_92 = 0U;
        int32_t l_94 = 0x307078AD;
        int32_t l_115[4][1];
        struct S0 *l_127 = (void*)0;
        struct S0 *l_128 = &g_30.f0;
        struct S0 *l_129 = &l_44[1][1].f0;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_115[i][j] = (-1);
        }
        if (((safe_rshift_func_int8_t_s_u((*l_47), 2)) | (p_20.f0.f0 = ((safe_mul_func_uint16_t_u_u(((((void*)0 != &g_50) | p_19) < l_92), g_30.f0.f0)) != 0xB2))))
        { 
            int32_t *l_93[2][3] = {{&l_69[0],(void*)0,(void*)0},{&l_69[0],(void*)0,(void*)0}};
            int i, j;
            g_95++;
            for (g_63 = 1; (g_63 >= 0); g_63 -= 1)
            { 
                uint8_t l_102[2][2];
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_102[i][j] = 0xCE;
                }
                (*l_47) = ((safe_rshift_func_uint32_t_u_s((((safe_rshift_func_int64_t_s_u((*l_47), ((l_102[1][1] | ((safe_unary_minus_func_uint16_t_u(0xF5EE)) > g_4)) == p_18))) & p_22) == l_94), 27)) & (*l_47));
            }
        }
        else
        { 
            struct S0 *l_109 = &g_30.f0;
            int32_t l_118 = (-1);
            int32_t l_119 = 0xAA58BC82;
            for (l_41 = (-11); (l_41 == 59); l_41 = safe_add_func_int64_t_s_s(l_41, 5))
            { 
                struct S2 *l_106 = &g_50;
                struct S2 **l_107 = &g_49[3];
                int32_t l_108 = 0x354A0364;
                (*l_107) = l_106;
                (*l_47) &= 0x76CAEA25;
                if (l_108)
                    break;
            }
            if (p_22)
            { 
                struct S0 **l_110 = &l_109;
                int32_t *l_111 = &l_64;
                int32_t *l_112 = &l_69[0];
                int32_t *l_113 = &l_64;
                int32_t *l_114[1][2];
                int32_t l_121 = 0;
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_114[i][j] = &l_64;
                }
                (*l_110) = l_109;
                ++l_122;
                (*l_111) ^= (((p_20 , (g_67[1] = l_118)) || (safe_rshift_func_uint64_t_u_u(p_21, (*l_47)))) || 3U);
            }
            else
            { 
                return p_18;
            }
        }
        (*l_129) = g_30.f0;
    }
    l_65 = (((0x6CCB9D5B < ((&p_18 != (void*)0) > p_19)) & p_20.f0.f1) >= (*l_47));
    return g_55;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_30.f0.f0, "g_30.f0.f0", print_hash_value);
    transparent_crc(g_30.f0.f1, "g_30.f0.f1", print_hash_value);
    transparent_crc(g_30.f0.f2, "g_30.f0.f2", print_hash_value);
    transparent_crc(g_50.f0, "g_50.f0", print_hash_value);
    transparent_crc(g_50.f1, "g_50.f1", print_hash_value);
    transparent_crc(g_50.f2, "g_50.f2", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_67[i], "g_67[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_95, "g_95", print_hash_value);
    transparent_crc(g_116, "g_116", print_hash_value);
    transparent_crc(g_117, "g_117", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_120[i], "g_120[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
