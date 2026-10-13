/* Csmith 0cdc710315cf seed 79, flattened by scripts/csmith_flatten.py */
// Options:   --seed 79 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
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
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint16_t  f0;
};

struct S1 {
   uint32_t  f0;
   uint8_t  f1;
   int16_t  f2;
};

static int32_t g_2[4] = {0xD444182B,0xD444182B,0xD444182B,0xD444182B};
static struct S0 *g_7 = (void*)0;
static int8_t g_16 = 0xD1;
static int64_t g_25 = 0x1B9C6FEC1B16EB2DLL;
static uint64_t g_32 = 18446744073709551615U;
static uint8_t g_40[3] = {248U,248U,248U};
static struct S1 g_45 = {0x4965DA1F,0x97,2};
static struct S1 * const g_44 = &g_45;
static struct S1 * const *g_43 = &g_44;
static uint32_t g_83 = 0x66BFFE0A;
static struct S0 g_92[2][4] = {{{0x728E},{0x0D67},{0x728E},{0x0D67}},{{0x728E},{0x0D67},{0x728E},{0x0D67}}};
static uint16_t g_102 = 9U;
static uint16_t *g_101 = &g_102;
static uint64_t g_105 = 1U;

static int8_t  func_1(void);
static struct S0 * func_5(struct S0 * p_6);
static const uint32_t  func_8(struct S0  p_9, uint16_t  p_10, struct S0 * p_11, uint8_t  p_12, int64_t  p_13);

static int8_t  func_1(void)
{ 
    struct S0 *l_125[4] = {&g_92[0][2],&g_92[0][2],&g_92[0][2],&g_92[0][2]};
    int32_t l_126[3][2] = {{(-6),(-6)},{7,(-6)},{(-6),7}};
    int i, j;
    for (g_2[2] = 0; (g_2[2] >= (-2)); --g_2[2])
    { 
        struct S0 **l_124[2][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_124[i][j] = &g_7;
        }
        l_125[0] = func_5(g_7);
    }
    return l_126[2][0];
}

static struct S0 * func_5(struct S0 * p_6)
{ 
    struct S0 l_14[4][2] = {{{0x0669},{65535U}},{{0x0669},{65535U}},{{0x0669},{65535U}},{{0x0669},{65535U}}};
    int8_t *l_15 = &g_16;
    struct S1 *l_123 = &g_45;
    struct S1 **l_122 = &l_123;
    int i, j;
    (*l_122) = ((func_8(l_14[1][1], ((((*l_15) = ((l_14[1][1].f0 , (g_2[3] != 1)) >= g_2[3])) > g_2[2]) , l_14[1][1].f0), g_7, l_14[1][1].f0, g_2[2]) , g_40[0]) , (*g_43));
    return p_6;
}

static const uint32_t  func_8(struct S0  p_9, uint16_t  p_10, struct S0 * p_11, uint8_t  p_12, int64_t  p_13)
{ 
    int32_t l_22 = (-1);
    int64_t *l_23 = (void*)0;
    int64_t *l_24[4] = {&g_25,&g_25,&g_25,&g_25};
    int8_t *l_28[3];
    int8_t **l_29 = &l_28[1];
    int32_t l_30 = 0xA81D5F49;
    uint64_t *l_31 = &g_32;
    int8_t *l_33 = &g_16;
    struct S0 *l_64 = (void*)0;
    struct S1 l_90 = {0x2E340D7E,250U,0x8C89};
    int i;
    for (i = 0; i < 3; i++)
        l_28[i] = &g_16;
    if (((safe_mod_func_int8_t_s_s((g_2[2] , ((*l_33) = (+((*l_31) ^= (safe_add_func_int64_t_s_s((l_22 = l_22), (((safe_mul_func_uint64_t_u_u((((*l_29) = l_28[2]) == &g_16), 2U)) , l_30) , g_2[1]))))))), g_25)) && p_9.f0))
    { 
        int32_t *l_34 = &l_22;
        int32_t *l_35 = &l_30;
        int32_t *l_36 = &l_22;
        int32_t *l_37 = &l_30;
        int32_t *l_38 = &l_22;
        int32_t *l_39[3][4];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 4; j++)
                l_39[i][j] = &l_22;
        }
        g_40[0]--;
        g_43 = g_43;
    }
    else
    { 
        uint8_t l_56 = 0x89;
        int32_t * const *l_62 = (void*)0;
        int32_t l_63[1][3];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_63[i][j] = 0;
        }
        for (g_45.f0 = 29; (g_45.f0 <= 9); g_45.f0 = safe_sub_func_int16_t_s_s(g_45.f0, 4))
        { 
            int32_t *l_52[4][1];
            int32_t **l_53 = &l_52[3][0];
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 1; j++)
                    l_52[i][j] = (void*)0;
            }
            l_30 = (g_45.f1 < (safe_sub_func_uint64_t_u_u(((safe_add_func_int16_t_s_s(0x0AAF, 0U)) < p_9.f0), 0x842353725AC95004LL)));
            (*l_53) = l_52[1][0];
            if ((safe_div_func_int8_t_s_s(5, l_56)))
            { 
                uint16_t *l_59 = (void*)0;
                uint32_t l_60 = 0xB64D43D0;
                struct S0 **l_65 = &g_7;
                l_30 = (((((safe_mul_func_uint16_t_u_u((l_60 = p_10), (((~g_45.f0) , l_62) != &l_52[2][0]))) >= 6) >= g_45.f1) | p_13) == g_40[0]);
                l_63[0][0] = l_60;
                (*l_65) = l_64;
            }
            else
            { 
                int32_t *l_66 = &g_2[3];
                int64_t **l_69 = &l_24[3];
                int64_t **l_70 = &l_23;
                int32_t *l_73 = &l_63[0][1];
                (*l_53) = l_66;
                l_22 = ((safe_add_func_int32_t_s_s((&p_13 == ((*l_70) = ((*l_69) = &g_25))), (safe_add_func_int64_t_s_s((g_45.f1 , g_32), p_12)))) < l_30);
                (*l_53) = l_73;
            }
        }
    }
    if ((g_40[2] == ((safe_div_func_int8_t_s_s(g_32, (safe_sub_func_uint64_t_u_u((g_40[0] && p_13), 0x26C3A74F57033C99LL)))) >= l_30)))
    { 
        uint64_t l_82 = 3U;
        struct S0 l_93 = {5U};
lbl_94:
        g_83 = (((0xF0789CDEE7B07E04LL <= ((safe_rshift_func_uint64_t_u_s((((0x97A3 & (((safe_sub_func_int8_t_s_s((((*l_29) = (void*)0) == (void*)0), p_13)) ^ g_45.f1) , 0xED11)) && l_22) != g_16), l_22)) && 18446744073709551615U)) < l_30) & l_82);
        for (l_22 = 0; (l_22 > 29); l_22 = safe_add_func_uint32_t_u_u(l_22, 1))
        { 
            if ((safe_add_func_int8_t_s_s((-3), (g_40[1] <= (safe_add_func_int8_t_s_s(g_45.f0, p_9.f0))))))
            { 
                struct S1 l_91 = {4294967286U,0x4D,1};
                l_91 = l_90;
                l_93 = g_92[1][2];
                l_30 |= p_12;
            }
            else
            { 
                int32_t *l_103 = (void*)0;
                int32_t *l_104[1][3];
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 3; j++)
                        l_104[i][j] = &l_30;
                }
                if (p_9.f0)
                    goto lbl_94;
                g_105 = (safe_div_func_int32_t_s_s(p_12, (safe_div_func_uint8_t_u_u(((safe_sub_func_int32_t_s_s(((void*)0 != g_101), p_10)) == p_9.f0), 8))));
                return g_45.f2;
            }
        }
        return p_12;
    }
    else
    { 
        struct S1 l_108 = {4U,0x33,0x8DBD};
        int32_t l_115 = 0xA8EECE75;
        int32_t *l_116 = &l_30;
        uint8_t *l_119 = &g_40[2];
        uint8_t *l_120 = &l_90.f1;
        int32_t *l_121[2][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_121[i][j] = &l_115;
        }
        for (g_32 = 0; (g_32 > 19); g_32 = safe_add_func_uint64_t_u_u(g_32, 5))
        { 
            return g_25;
        }
        (*l_116) = (((l_108 , ((l_22 |= (l_115 ^= ((safe_add_func_uint8_t_u_u((safe_rshift_func_int32_t_s_s((safe_div_func_uint32_t_u_u(g_2[2], l_108.f1)), l_90.f0)), 0x84)) , g_32))) == g_45.f2)) , p_9.f0) , p_10);
        l_22 |= (safe_div_func_uint16_t_u_u(((g_45.f1 = ((*l_120) = (((*l_119) = (((*l_31) = ((*g_44) , 0x252A4F93C24CE628LL)) | (((void*)0 == &g_2[2]) | (*l_116)))) == p_9.f0))) , 6U), p_12));
    }
    return g_25;
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
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_40[i], "g_40[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_45.f0, "g_45.f0", print_hash_value);
    transparent_crc(g_45.f1, "g_45.f1", print_hash_value);
    transparent_crc(g_45.f2, "g_45.f2", print_hash_value);
    transparent_crc(g_83, "g_83", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_92[i][j].f0, "g_92[i][j].f0", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_102, "g_102", print_hash_value);
    transparent_crc(g_105, "g_105", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
