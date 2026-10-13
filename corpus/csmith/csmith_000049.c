/* Csmith 0cdc710315cf seed 49, flattened by scripts/csmith_flatten.py */
// Options:   --seed 49 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
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
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const int64_t  f0;
   int8_t  f1;
};

struct S1 {
   uint64_t  f0;
   int8_t  f1;
   struct S0  f2;
};

static int32_t g_2 = (-1);
static uint8_t g_18 = 0x5B;
static int32_t g_31 = (-1);
static int32_t *g_38 = &g_31;
static int32_t **g_37 = &g_38;
static uint8_t g_57 = 1U;
static uint8_t g_60 = 0U;
static uint8_t g_62 = 0x8F;
static int8_t g_64 = (-5);
static struct S1 g_69 = {0xCA6F77A312446B8CLL,7,{0x9878FF5F6F594A39LL,0x7B}};
static uint32_t g_97 = 0xBD2431EA;
static int32_t g_114 = 1;
static uint32_t g_116 = 4294967295U;
static uint16_t g_143 = 0xE7A6;
static int16_t g_145 = 0xCB90;
static int16_t g_159 = 1;
static uint32_t g_188[3] = {4294967287U,4294967287U,4294967287U};
static int16_t g_195 = 0;
static uint32_t g_204 = 0x7620F8F3;
static const int32_t g_214 = 0xC23372F7;

static struct S1  func_1(void);
static uint64_t  func_8(struct S0  p_9, uint8_t  p_10, int64_t  p_11, struct S0  p_12, uint32_t  p_13);
static struct S1  func_14(const uint32_t  p_15, int64_t  p_16);

static struct S1  func_1(void)
{ 
    struct S0 l_148 = {0xA522227AEB4D02DCLL,0x10};
    int8_t l_222 = 0;
    int32_t l_238[3][2];
    struct S1 l_240 = {18446744073709551615U,0,{1,3}};
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_238[i][j] = 0xCAC54704;
    }
    for (g_2 = 0; (g_2 <= 4); g_2 = safe_add_func_uint32_t_u_u(g_2, 1))
    { 
        int16_t l_17[2][4] = {{1,1,1,1},{1,1,1,1}};
        struct S1 l_230[3][4] = {{{18446744073709551612U,0,{0x29F626814382FC2CLL,2}},{18446744073709551612U,0x9A,{0x7665D8ACBC52F4FCLL,0x7E}},{18446744073709551612U,0x9A,{0x7665D8ACBC52F4FCLL,0x7E}},{18446744073709551612U,0,{0x29F626814382FC2CLL,2}}},{{18446744073709551612U,0x9A,{0x7665D8ACBC52F4FCLL,0x7E}},{18446744073709551612U,0,{0x29F626814382FC2CLL,2}},{18446744073709551612U,0x9A,{0x7665D8ACBC52F4FCLL,0x7E}},{18446744073709551612U,0x9A,{0x7665D8ACBC52F4FCLL,0x7E}}},{{18446744073709551612U,0,{0x29F626814382FC2CLL,2}},{18446744073709551612U,0,{0x29F626814382FC2CLL,2}},{0xBE21FB04089B8EC4LL,-6,{-1,0xA6}},{18446744073709551612U,0,{0x29F626814382FC2CLL,2}}}};
        const struct S1 *l_243 = &l_230[2][0];
        const struct S1 **l_242 = &l_243;
        int32_t *l_246 = &l_238[1][0];
        int i, j;
    }
    (**g_37) &= ((g_188[1] != (g_69.f0 | (safe_div_func_uint16_t_u_u((safe_mul_func_int16_t_s_s((safe_mod_func_uint32_t_u_u((safe_add_func_uint64_t_u_u((safe_mod_func_int8_t_s_s(g_60, 0x83)), g_145)), l_240.f0)), g_18)), 0xBEDF)))) , 0x423CF226);
    return g_69;
}

static uint64_t  func_8(struct S0  p_9, uint8_t  p_10, int64_t  p_11, struct S0  p_12, uint32_t  p_13)
{ 
    uint32_t l_151 = 4294967288U;
    int32_t l_193 = 0x4EC9872C;
    int32_t l_194 = (-4);
    uint8_t l_196 = 2U;
lbl_219:
    if (((void*)0 == (*g_37)))
    { 
        uint16_t l_149 = 65535U;
        uint32_t *l_150[3][1];
        int16_t *l_158[1];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_150[i][j] = &g_116;
        }
        for (i = 0; i < 1; i++)
            l_158[i] = (void*)0;
        (*g_38) &= (((l_149 ^ p_13) <= (l_151--)) , ((safe_mod_func_uint8_t_u_u(((((safe_sub_func_int8_t_s_s(((g_159 = ((((g_69.f2 , &p_13) == &p_13) >= 65535U) < l_151)) > (-1)), 3)) && l_151) == 0x2A7D) , g_57), l_149)) ^ 65535U));
    }
    else
    { 
        uint16_t *l_164 = &g_143;
        int32_t l_171 = 0xF2A17659;
        uint64_t *l_172 = &g_69.f0;
        l_171 = ((safe_sub_func_int16_t_s_s(((--(*l_164)) > ((((g_114 < ((*l_172) = (safe_add_func_int64_t_s_s(p_11, ((safe_rshift_func_uint8_t_u_s((((l_151 , l_171) < l_171) != g_116), 7)) || (**g_37)))))) > g_2) , (void*)0) == (void*)0)), p_12.f0)) == 1U);
    }
    for (g_69.f2.f1 = 0; (g_69.f2.f1 == 5); g_69.f2.f1 = safe_add_func_uint16_t_u_u(g_69.f2.f1, 2))
    { 
        int32_t l_191 = 6;
        int32_t *l_208 = (void*)0;
        int32_t **l_207 = &l_208;
        for (g_116 = 17; (g_116 >= 35); g_116 = safe_add_func_int32_t_s_s(g_116, 2))
        { 
            uint8_t l_181 = 248U;
            int8_t l_182 = 7;
            for (g_97 = (-25); (g_97 != 49); g_97++)
            { 
                int32_t *l_180 = &g_114;
                uint64_t *l_185[2][3] = {{&g_69.f0,&g_69.f0,&g_69.f0},{&g_69.f0,&g_69.f0,&g_69.f0}};
                int i, j;
                (*l_180) &= ((**g_37) |= (safe_unary_minus_func_int16_t_s(g_69.f2.f0)));
                (*l_180) = ((l_181 >= l_182) & (safe_rshift_func_int32_t_s_u(((*g_38) = (((g_69.f0 = g_18) & g_31) & g_97)), 8)));
            }
        }
        for (g_60 = 0; (g_60 <= 53); g_60++)
        { 
            int32_t *l_199 = &l_194;
            int32_t *l_200 = &g_114;
            int32_t *l_201 = &l_193;
            int32_t *l_202 = &g_31;
            int32_t *l_203[2];
            int i;
            for (i = 0; i < 2; i++)
                l_203[i] = &g_114;
            for (p_11 = 0; p_11 < 3; p_11 = p_11 + 1)
            {
                g_188[p_11] = 0U;
            }
            for (p_12.f1 = (-5); (p_12.f1 == 3); p_12.f1++)
            { 
                int32_t *l_192[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_192[i] = &g_114;
                (*g_37) = (*g_37);
                if (l_191)
                    break;
                l_196++;
            }
            --g_204;
        }
        (*l_207) = ((*g_37) = &l_194);
    }
    if (l_194)
    { 
        int64_t l_211[1];
        int i;
        for (i = 0; i < 1; i++)
            l_211[i] = 1;
        for (g_145 = 0; (g_145 > (-1)); g_145 = safe_sub_func_int32_t_s_s(g_145, 3))
        { 
            const int32_t *l_213 = &g_214;
            const int32_t **l_212 = &l_213;
            const int32_t *l_216 = &g_31;
            const int32_t **l_215 = &l_216;
            if (l_211[0])
                break;
            (*l_215) = ((*l_212) = &l_194);
            (*g_38) &= (safe_mod_func_int32_t_s_s((l_211[0] < (&l_213 == &g_38)), 0x3CE1FE00));
        }
    }
    else
    { 
        if (l_196)
            goto lbl_219;
    }
    return p_10;
}

static struct S1  func_14(const uint32_t  p_15, int64_t  p_16)
{ 
    uint8_t l_29 = 2U;
    int32_t *l_30 = &g_31;
    int32_t l_32 = 2;
    uint64_t l_81 = 0xF4B502D48012CFB7LL;
    int32_t l_115 = 1;
    int16_t l_141[4][3] = {{(-10),(-10),(-10)},{(-6),(-6),(-6)},{(-10),(-10),(-10)},{(-6),(-6),(-6)}};
    struct S1 l_147[2] = {{1U,0x1E,{0xE6C69FB00B4203E2LL,0x3A}},{1U,0x1E,{0xE6C69FB00B4203E2LL,0x3A}}};
    int i, j;
    --g_18;
    return l_147[0];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_69.f0, "g_69.f0", print_hash_value);
    transparent_crc(g_69.f1, "g_69.f1", print_hash_value);
    transparent_crc(g_69.f2.f0, "g_69.f2.f0", print_hash_value);
    transparent_crc(g_69.f2.f1, "g_69.f2.f1", print_hash_value);
    transparent_crc(g_97, "g_97", print_hash_value);
    transparent_crc(g_114, "g_114", print_hash_value);
    transparent_crc(g_116, "g_116", print_hash_value);
    transparent_crc(g_143, "g_143", print_hash_value);
    transparent_crc(g_145, "g_145", print_hash_value);
    transparent_crc(g_159, "g_159", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_188[i], "g_188[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_195, "g_195", print_hash_value);
    transparent_crc(g_204, "g_204", print_hash_value);
    transparent_crc(g_214, "g_214", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
