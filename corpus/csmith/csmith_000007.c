/* Csmith 0cdc710315cf seed 7, flattened by scripts/csmith_flatten.py */
// Options:   --seed 7 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
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

union U0 {
   uint16_t  f0;
   const int32_t  f1;
};

static int32_t g_3 = (-1);
static int16_t g_10 = 0x5934;
static int8_t g_11 = (-1);
static uint16_t g_12 = 0U;
static int16_t g_25 = 0;
static int32_t g_38 = 0xEEA3821C;
static uint16_t g_39[3][4] = {{0U,0U,0U,0U},{0U,0U,0U,0U},{0U,0U,0U,0U}};
static int8_t g_59[3] = {0,0,0};
static uint64_t g_61[1][3] = {{0x9D242EC213DB237ELL,0x9D242EC213DB237ELL,0x9D242EC213DB237ELL}};
static int32_t **g_67[1] = {(void*)0};
static int8_t g_71 = 0x0F;
static uint32_t g_91 = 0xDA9DFCA0;
static int64_t g_108 = 0xEDCC949663287AF7LL;
static int64_t g_157[4] = {0x7C23DFB74B672E11LL,0x7C23DFB74B672E11LL,0x7C23DFB74B672E11LL,0x7C23DFB74B672E11LL};
static uint8_t g_178 = 0x85;
static int64_t g_179[1][3] = {{0xD5290F8A08BE86FCLL,0xD5290F8A08BE86FCLL,0xD5290F8A08BE86FCLL}};
static int16_t g_190 = 1;

static uint16_t  func_1(void);
static int32_t  func_15(uint64_t  p_16, int32_t  p_17, uint32_t  p_18);
static uint16_t  func_26(int32_t * p_27);

static uint16_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t *l_4 = &g_3;
    int32_t *l_5 = &g_3;
    int32_t *l_6 = (void*)0;
    int32_t *l_7 = &g_3;
    int32_t *l_8 = &g_3;
    int32_t *l_9[4];
    int i;
    for (i = 0; i < 4; i++)
        l_9[i] = &g_3;
    g_12++;
    if (g_10)
    { 
        int16_t *l_23 = &g_10;
        int16_t *l_24 = &g_25;
        int32_t *l_28 = &g_3;
        int32_t l_36 = 0x53271639;
        (*l_28) = func_15(((safe_mul_func_int8_t_s_s((safe_mul_func_int16_t_s_s(((*l_24) |= ((*l_23) = g_11)), func_26(l_28))), g_11)) <= 0xFD), l_36, g_11);
        return g_108;
    }
    else
    { 
        uint16_t l_156 = 0x3C79;
        int64_t *l_186 = &g_108;
        uint8_t l_191 = 0xDA;
        for (g_108 = 2; (g_108 >= 0); g_108 -= 1)
        { 
            int8_t l_153[2];
            union U0 l_163[3][4] = {{{0x58A9},{0x58A9},{0x58A9},{0x58A9}},{{0x58A9},{0x58A9},{0x58A9},{0x58A9}},{{0x58A9},{0x58A9},{0x58A9},{0x58A9}}};
            int32_t l_168 = (-4);
            int i, j;
            for (i = 0; i < 2; i++)
                l_153[i] = 7;
            for (g_10 = 0; (g_10 <= 2); g_10 += 1)
            { 
                int32_t l_144 = 5;
                int i, j;
                l_144 = g_39[g_108][(g_108 + 1)];
            }
            if ((safe_lshift_func_int8_t_s_u((-1), 2)))
            { 
                int32_t l_158[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_158[i] = 0xB4876285;
                l_158[0] &= (safe_add_func_int16_t_s_s((safe_sub_func_uint32_t_u_u((safe_add_func_int32_t_s_s(l_153[0], ((safe_div_func_uint32_t_u_u(g_39[2][1], l_156)) | l_153[0]))), g_157[1])), (*l_8)));
            }
            else
            { 
                int16_t l_166 = 0;
                uint64_t *l_167[3];
                uint8_t *l_177 = &g_178;
                int i;
                for (i = 0; i < 3; i++)
                    l_167[i] = &g_61[0][0];
                (*l_7) |= (safe_rshift_func_uint32_t_u_u((safe_mul_func_uint8_t_u_u((l_163[2][0] , (g_179[0][1] = ((*l_177) = ((((safe_mul_func_uint32_t_u_u(((l_168 = l_166) & ((safe_sub_func_uint32_t_u_u((((safe_sub_func_uint8_t_u_u((safe_mul_func_uint16_t_u_u(((safe_mul_func_int32_t_s_s(l_163[2][0].f0, l_163[2][0].f0)) == g_12), 0x4E2F)), g_11)) & g_38) == 0), l_156)) != l_166)), l_153[0])) >= g_39[1][3]) , 0x98B19A1D12C228CFLL) > 2)))), g_61[0][0])), 4));
                return l_163[2][0].f0;
            }
            for (g_11 = 0; (g_11 <= 2); g_11 += 1)
            { 
                int i, j;
                (*l_7) = g_39[g_108][g_11];
            }
        }
        (*l_2) &= (safe_lshift_func_int32_t_s_s(((safe_add_func_int32_t_s_s((g_71 , (((safe_lshift_func_int64_t_s_u(((*l_186) = l_156), (~((safe_lshift_func_uint8_t_u_u(g_38, 0)) || g_157[1])))) == g_190) | (-1))), l_156)) , l_191), 10));
    }
    return g_179[0][1];
}

static int32_t  func_15(uint64_t  p_16, int32_t  p_17, uint32_t  p_18)
{ 
    int32_t *l_37[1][4];
    const union U0 l_52 = {0x1915};
    int16_t l_57 = 0;
    int32_t l_68 = 0x00EEBEB9;
    uint64_t l_84[2][2] = {{0x0C89082328EFC35FLL,0x0C89082328EFC35FLL},{0x0C89082328EFC35FLL,0x0C89082328EFC35FLL}};
    int16_t l_125 = 1;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_37[i][j] = &g_3;
    }
    --g_39[1][3];
    for (p_18 = 0; (p_18 <= 2); p_18 += 1)
    { 
        int32_t **l_42 = (void*)0;
        int32_t *l_44 = &g_3;
        int32_t **l_43 = &l_44;
        union U0 l_55 = {65528U};
        int32_t l_58 = 0xD4076B84;
        int32_t l_70[1][1];
        int32_t l_76 = 0x4657C2A8;
        uint32_t l_100 = 5U;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_70[i][j] = 0x284F583E;
        }
    }
    return p_18;
}

static uint16_t  func_26(int32_t * p_27)
{ 
    uint64_t l_30 = 0x651162F63069BE54LL;
    int32_t *l_35 = (void*)0;
    (*p_27) = (safe_unary_minus_func_uint16_t_u(l_30));
    (*p_27) ^= g_12;
    for (g_12 = 0; (g_12 == 35); g_12++)
    { 
        int32_t *l_34 = &g_3;
        int32_t **l_33[2][3];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 3; j++)
                l_33[i][j] = &l_34;
        }
        l_35 = &g_3;
    }
    return g_3;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_11, "g_11", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_39[i][j], "g_39[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_59[i], "g_59[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_61[i][j], "g_61[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_71, "g_71", print_hash_value);
    transparent_crc(g_91, "g_91", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_157[i], "g_157[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_178, "g_178", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_179[i][j], "g_179[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_190, "g_190", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
