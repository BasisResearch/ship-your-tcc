/* Csmith 0cdc710315cf seed 155, flattened by scripts/csmith_flatten.py */
// Options:   --seed 155 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
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
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static int64_t
safe_unary_minus_func_int64_t_s(int64_t si )
{
  return
    (si==(-9223372036854775807L-1)) ?
    ((si)) :
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint64_t  f0;
   int64_t  f1;
   uint16_t  f2;
};

static uint16_t g_7 = 0xA5EF;
static uint8_t g_21 = 0x36;
static int32_t g_24 = 0x5C2F1FCC;
static uint32_t g_28 = 0xD907C4E7;
static uint16_t *g_42 = &g_7;
static uint16_t **g_41 = &g_42;
static int64_t g_61 = 0xA29ADD9258C0F86FLL;
static int16_t g_63 = 0x3EC6;
static uint8_t g_64 = 255U;
static int32_t *g_69[3] = {&g_24,&g_24,&g_24};

static int8_t  func_1(void);
static int32_t  func_2(const int16_t  p_3, uint64_t  p_4);
static struct S0  func_12(uint64_t  p_13, uint16_t * p_14, uint16_t * p_15, int32_t  p_16);

static int8_t  func_1(void)
{ 
    uint16_t *l_6[3][1];
    int32_t l_10 = (-4);
    int32_t l_11 = 1;
    int32_t *l_74 = (void*)0;
    int16_t l_81[2];
    uint16_t l_99 = 0x51AF;
    int32_t l_101 = 7;
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_6[i][j] = &g_7;
    }
    for (i = 0; i < 2; i++)
        l_81[i] = 0x94BA;
    l_11 = func_2(((+18446744073709551606U) , ((l_10 = (g_7++)) , ((g_7 <= (((&g_7 != (void*)0) < g_7) & 0x7E81)) != g_7))), l_11);
    for (g_28 = (-14); (g_28 > 24); g_28 = safe_add_func_int16_t_s_s(g_28, 4))
    { 
        int32_t l_77 = 1;
        int32_t *l_84[1];
        int32_t **l_98[2];
        uint64_t l_100 = 1U;
        int i;
        for (i = 0; i < 1; i++)
            l_84[i] = &g_24;
        for (i = 0; i < 2; i++)
            l_98[i] = &g_69[2];
        l_10 |= (l_77 || ((l_11 = (safe_unary_minus_func_int64_t_s((safe_mul_func_int8_t_s_s((l_81[0] && (safe_add_func_int8_t_s_s(g_64, g_61))), 0x96))))) , 0x982AEBAC));
        l_101 &= (+(((safe_mul_func_int8_t_s_s(((((((((((safe_rshift_func_uint8_t_u_u(((safe_add_func_uint32_t_u_u((250U == ((safe_mul_func_uint64_t_u_u((safe_div_func_int32_t_s_s((safe_rshift_func_int32_t_s_s(((l_74 = &g_24) != (void*)0), l_99)), l_100)), g_24)) , 0x9B)), l_99)) || g_63), 0)) ^ 0xEC415B45A7EABDE2LL) >= 0xFC04FBFD2E0221BCLL) || (*l_74)) & g_28) != g_63) , &l_11) != &g_24) <= g_64) | g_61), g_7)) | 0xA2F2E681A9B78C74LL) && (*l_74)));
    }
    return g_64;
}

static int32_t  func_2(const int16_t  p_3, uint64_t  p_4)
{ 
    uint16_t *l_17 = (void*)0;
    uint16_t **l_18 = (void*)0;
    uint16_t *l_19 = &g_7;
    int32_t l_20 = 0x59EA4D59;
    int32_t *l_71 = (void*)0;
    int32_t l_72[2][3];
    int32_t **l_73 = &g_69[2];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
            l_72[i][j] = 0x1A5BDB26;
    }
    l_72[0][1] |= ((func_12(g_7, (l_17 = l_17), l_19, l_20) , p_4) < 0xE22E7E12F4000348LL);
    (*l_73) = &l_72[0][1];
    return p_4;
}

static struct S0  func_12(uint64_t  p_13, uint16_t * p_14, uint16_t * p_15, int32_t  p_16)
{ 
    uint16_t l_25[4];
    uint16_t *l_40 = &l_25[1];
    uint16_t **l_39 = &l_40;
    int32_t *l_46[4];
    int32_t *l_68[1];
    int32_t **l_67[4] = {&l_68[0],&l_68[0],&l_68[0],&l_68[0]};
    struct S0 l_70 = {18446744073709551612U,1,65535U};
    int i;
    for (i = 0; i < 4; i++)
        l_25[i] = 1U;
    for (i = 0; i < 4; i++)
        l_46[i] = &g_24;
    for (i = 0; i < 1; i++)
        l_68[i] = (void*)0;
    if ((g_7 , (g_21 = p_13)))
    { 
        int32_t *l_22 = (void*)0;
        int32_t *l_23 = &g_24;
        (*l_23) ^= g_7;
        for (g_24 = 0; (g_24 <= 3); g_24 += 1)
        { 
            int16_t l_26[3];
            uint32_t *l_27 = &g_28;
            uint8_t *l_37 = &g_21;
            int32_t l_38 = 0xEFEC1A2C;
            int i;
            for (i = 0; i < 3; i++)
                l_26[i] = 0xF844;
            l_38 &= ((++(*l_27)) && (safe_add_func_uint32_t_u_u(((*l_27) = (((safe_lshift_func_int32_t_s_s((safe_sub_func_uint8_t_u_u(l_25[g_24], p_13)), 23)) ^ ((*l_37) = ((l_25[g_24] || 0x82) == 0xB911))) == p_16)), g_24)));
        }
        g_41 = l_39;
    }
    else
    { 
        int32_t *l_44 = &g_24;
        int32_t **l_43 = &l_44;
        int32_t **l_45 = (void*)0;
        l_46[2] = ((*l_43) = &p_16);
        (*l_43) = (void*)0;
    }
    for (g_7 = (-22); (g_7 == 19); ++g_7)
    { 
        const int8_t l_59[3][2] = {{0x85,0x85},{(-4),0x85},{0x85,(-4)}};
        int64_t *l_60 = &g_61;
        int32_t l_62 = (-1);
        int i, j;
        p_16 = (l_62 = (safe_sub_func_int64_t_s_s(((*l_60) = (safe_sub_func_uint8_t_u_u(p_16, (safe_div_func_int8_t_s_s((((safe_add_func_int64_t_s_s((safe_add_func_int64_t_s_s((((void*)0 == &g_24) != l_59[1][0]), 0xB1D0B827BE627823LL)), l_59[2][1])) < 1U) >= g_21), p_13))))), p_13)));
        ++g_64;
        g_24 = p_13;
    }
    g_69[2] = (l_46[0] = &g_24);
    return l_70;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
