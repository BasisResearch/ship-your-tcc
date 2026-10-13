/* Csmith 0cdc710315cf seed 3, flattened by scripts/csmith_flatten.py */
// Options:   --seed 3 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint16_t  f0;
   uint64_t  f1;
};

union U1 {
   int8_t  f0;
};

union U2 {
   uint8_t  f0;
   uint64_t  f1;
   int64_t  f2;
};

union U3 {
   uint16_t  f0;
};

static int32_t g_2 = 1;
static int32_t g_6 = (-1);
static union U3 g_11 = {1U};
static union U1 g_18 = {-1};
static union U0 g_21[3][3] = {{{7U},{7U},{7U}},{{0xF079},{0xF079},{0xF079}},{{7U},{7U},{7U}}};
static uint16_t g_37 = 0xFB7D;
static int32_t g_39 = 0x8718302C;
static int8_t g_47 = 2;
static uint64_t g_122[3][2] = {{0xBFB6AB9C1A6072D4LL,0xD480AB10ED2AF603LL},{0xD480AB10ED2AF603LL,0xBFB6AB9C1A6072D4LL},{0xD480AB10ED2AF603LL,0xD480AB10ED2AF603LL}};
static int16_t g_147 = 0x2EA8;
static uint32_t g_157 = 0x51F23073;
static int32_t *g_159 = (void*)0;
static int64_t g_170 = 1;
static uint8_t g_171 = 0x9D;
static int32_t g_173 = (-9);
static int32_t g_207[4] = {0x5C898DBE,0x5C898DBE,0x5C898DBE,0x5C898DBE};

static uint16_t  func_1(void);
static int64_t  func_7(union U3  p_8, union U3  p_9, int64_t  p_10);
static union U3  func_12(union U1  p_13, int32_t  p_14, union U0  p_15, union U0  p_16, int16_t  p_17);

static uint16_t  func_1(void)
{ 
    uint16_t l_174 = 0U;
    int32_t l_208 = 0xFD2DC112;
    for (g_2 = (-5); (g_2 <= (-11)); g_2 = safe_sub_func_uint8_t_u_u(g_2, 1))
    { 
        union U2 l_5[1][4] = {{{7U},{7U},{7U},{7U}}};
        int32_t l_180 = 0;
        int32_t l_202 = (-1);
        int32_t l_206 = 3;
        int32_t l_209[3][4] = {{2,2,2,2},{2,2,2,2},{2,2,2,2}};
        int i, j;
        for (g_6 = 0; (g_6 <= 0); g_6 += 1)
        { 
            int32_t l_19 = 0x96950A4B;
            union U0 l_20 = {65534U};
            int32_t *l_203 = (void*)0;
            int32_t *l_204 = (void*)0;
            int32_t *l_205[1][4] = {{&l_202,&l_202,&l_202,&l_202}};
            uint32_t l_210 = 0xB78321BB;
            int i, j;
            if ((func_7(g_11, func_12(g_18, l_19, l_20, g_21[1][1], g_18.f0), g_11.f0) <= l_174))
            { 
                int32_t **l_175 = (void*)0;
                int32_t **l_176 = &g_159;
                int32_t *l_177 = (void*)0;
                int32_t *l_178 = (void*)0;
                int32_t *l_179 = &l_19;
                (*l_176) = &g_39;
                (*l_179) |= ((*g_159) = (1 >= l_174));
                l_180 |= (*g_159);
            }
            else
            { 
                int32_t *l_181[1];
                int32_t **l_182 = (void*)0;
                int8_t *l_200 = &g_47;
                uint16_t *l_201[4][2] = {{&l_20.f0,&l_174},{&l_20.f0,&l_174},{&l_20.f0,&l_174},{&l_20.f0,&l_174}};
                int i, j;
                for (i = 0; i < 1; i++)
                    l_181[i] = &g_39;
                g_159 = l_181[0];
                l_202 &= ((l_180 = (safe_mod_func_int16_t_s_s((safe_sub_func_int64_t_s_s((safe_sub_func_uint64_t_u_u((safe_add_func_int8_t_s_s(((void*)0 == &g_11), (safe_lshift_func_uint64_t_u_u((safe_mul_func_int8_t_s_s(((*l_200) = ((!(((safe_div_func_int16_t_s_s(((safe_rshift_func_uint8_t_u_s(l_174, 3)) > 0xAE), (-9))) == 0xA7151E09FDA15D0CLL) == 0x3F)) , l_180)), g_18.f0)), 32)))), 8U)), (-10))), l_180))) | l_174);
            }
            l_210--;
            (*g_159) |= 0xEE946CD2;
        }
    }
    return l_174;
}

static int64_t  func_7(union U3  p_8, union U3  p_9, int64_t  p_10)
{ 
    int32_t *l_133[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
    int i;
    g_39 ^= 0x8A9B892D;
    for (p_10 = 0; (p_10 <= (-3)); p_10--)
    { 
        union U0 l_145 = {65528U};
        int16_t *l_146 = &g_147;
        int32_t l_150 = 0x01793202;
        int32_t *l_151[2][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_151[i][j] = &l_150;
        }
        if ((g_6 , (~(safe_add_func_uint8_t_u_u(((p_10 | (safe_div_func_uint8_t_u_u((safe_rshift_func_int64_t_s_s((((*l_146) = (+(safe_unary_minus_func_int16_t_s((l_145 , 1))))) <= 0xC936), 37)), g_11.f0))) <= 0x3F), l_145.f0)))))
        { 
            uint32_t *l_156[4] = {&g_157,&g_157,&g_157,&g_157};
            int i;
            for (g_147 = 0; (g_147 < (-11)); g_147 = safe_sub_func_uint8_t_u_u(g_147, 1))
            { 
                l_150 = (g_18 , l_150);
                if (g_39)
                    continue;
                if (g_147)
                    break;
            }
            l_151[0][0] = &g_2;
            g_39 &= (g_37 , (safe_add_func_uint16_t_u_u((0xD5DD7BA2 || (g_157 = ((safe_mul_func_int64_t_s_s(g_2, 0)) <= g_18.f0))), p_8.f0)));
        }
        else
        { 
            int32_t **l_158[1][4] = {{&l_151[1][0],&l_151[1][0],&l_151[1][0],&l_151[1][0]}};
            uint32_t *l_168 = &g_157;
            int i, j;
            g_159 = l_133[3];
            for (l_145.f1 = 0; (l_145.f1 <= 0); l_145.f1 += 1)
            { 
                int i, j;
                l_151[l_145.f1][l_145.f1] = l_151[l_145.f1][l_145.f1];
            }
            if ((safe_sub_func_uint32_t_u_u((((safe_div_func_int64_t_s_s(((safe_mod_func_uint32_t_u_u(1U, ((7U < ((*l_146) = ((safe_mod_func_int16_t_s_s((((((*l_168) = (g_21[1][1] , 0x4F6BAD65)) && 0x35E04E33) > p_8.f0) && g_21[1][1].f0), (-1))) | p_10))) || g_11.f0))) || g_2), p_10)) <= 0xE0B3DFB7) < p_8.f0), (-10))))
            { 
                g_159 = &l_150;
                g_159 = (g_6 , &l_150);
                (*g_159) |= 0xEA57A25A;
            }
            else
            { 
                int64_t *l_169 = &g_170;
                int32_t *l_172 = (void*)0;
                g_39 |= ((g_2 , (g_171 = ((*l_169) = p_8.f0))) || g_11.f0);
                l_172 = &l_150;
            }
        }
        g_173 = g_173;
    }
    return p_8.f0;
}

static union U3  func_12(union U1  p_13, int32_t  p_14, union U0  p_15, union U0  p_16, int16_t  p_17)
{ 
    int8_t l_27 = 0x50;
    int32_t l_48 = 0x11CCFE98;
    uint16_t l_49 = 65530U;
    union U3 l_58 = {0xF093};
    uint8_t l_70 = 252U;
    union U2 l_96[1] = {{1U}};
    union U1 l_101[1] = {{-6}};
    int32_t l_120 = 1;
    uint16_t l_127[4][2] = {{0U,0U},{0U,0U},{0U,0U},{0U,0U}};
    int32_t *l_129 = &l_48;
    int i, j;
    return g_11;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_11.f0, "g_11.f0", print_hash_value);
    transparent_crc(g_18.f0, "g_18.f0", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_21[i][j].f0, "g_21[i][j].f0", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_122[i][j], "g_122[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_147, "g_147", print_hash_value);
    transparent_crc(g_157, "g_157", print_hash_value);
    transparent_crc(g_170, "g_170", print_hash_value);
    transparent_crc(g_171, "g_171", print_hash_value);
    transparent_crc(g_173, "g_173", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_207[i], "g_207[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
