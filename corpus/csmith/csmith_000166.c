/* Csmith 0cdc710315cf seed 166, flattened by scripts/csmith_flatten.py */
// Options:   --seed 166 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint8_t  f0;
   uint16_t  f1;
   int8_t  f2;
};

static int32_t g_2 = 0;
static uint16_t g_14 = 2U;
static int32_t g_16 = 0x3E34B64B;
static int64_t g_18 = (-1);
static uint64_t g_28[2] = {1U,1U};
static int32_t *g_53 = &g_16;
static int8_t g_72 = 1;
static int8_t *g_71 = &g_72;
static uint64_t g_89 = 0xF18CA9A6AE6C26CFLL;
static uint64_t *g_88 = &g_89;
static uint64_t **g_87 = &g_88;
static union U0 *g_92 = (void*)0;

static uint64_t  func_1(void);
static int32_t * func_5(int32_t  p_6);
static const union U0  func_19(uint8_t  p_20, int64_t * p_21, int64_t  p_22);

static uint64_t  func_1(void)
{ 
    int32_t *l_10 = &g_2;
    uint8_t l_73 = 0xFA;
    int16_t l_74[2];
    uint64_t **l_91 = (void*)0;
    int i;
    for (i = 0; i < 2; i++)
        l_74[i] = 0x15BA;
lbl_79:
    for (g_2 = 0; (g_2 > 1); g_2++)
    { 
        uint16_t *l_13 = &g_14;
        int32_t *l_15[1][3];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_15[i][j] = &g_16;
        }
        g_53 = func_5((g_16 = (safe_unary_minus_func_uint16_t_u(((*l_13) |= ((safe_lshift_func_uint64_t_u_s((l_10 != &g_2), 53)) & (safe_div_func_uint64_t_u_u(18446744073709551615U, 7U))))))));
        (*g_53) = (*g_53);
    }
    if (((*l_10) || (safe_mod_func_int8_t_s_s((((safe_mod_func_uint64_t_u_u((((safe_div_func_int8_t_s_s((((safe_add_func_uint8_t_u_u((safe_mul_func_int32_t_s_s(((((((((safe_sub_func_int8_t_s_s((safe_div_func_uint32_t_u_u(0x22BB76E0, ((safe_div_func_uint8_t_u_u((+(g_71 == &g_72)), l_73)) | 0xEE1FA017))), (*l_10))) , (void*)0) != &g_16) > (*l_10)) && (*l_10)) & (-6)) <= g_2) , 0xAE77DDBA), 4294967295U)), 0xD8)) >= l_74[1]) , (*g_71)), (*l_10))) ^ (*l_10)) >= g_28[0]), g_18)) , (*l_10)) && 0x2A96), (*l_10)))))
    { 
        int64_t *l_77 = (void*)0;
        int32_t l_78 = 0x15C25834;
        (*g_53) = (g_28[0] >= (safe_mul_func_int16_t_s_s(((((void*)0 == l_77) | l_78) , g_28[1]), 0xF409)));
        if (l_73)
            goto lbl_79;
    }
    else
    { 
        int8_t *l_85 = &g_72;
        if ((safe_lshift_func_uint16_t_u_u((*l_10), 0)))
        { 
            uint64_t *l_84 = &g_28[0];
            int16_t *l_86 = &l_74[1];
            uint64_t **l_90 = (void*)0;
            union U0 **l_93 = &g_92;
            (*g_53) = ((safe_add_func_uint16_t_u_u((((*l_86) = ((((*l_84) = ((void*)0 != &g_14)) , (l_85 != (void*)0)) || g_28[0])) < g_16), 0x8C1B)) , (*g_53));
            l_91 = (l_90 = g_87);
            (*l_93) = g_92;
        }
        else
        { 
            int64_t l_111[1][4] = {{(-9),(-9),(-9),(-9)}};
            int i, j;
            (*g_53) = (safe_rshift_func_int64_t_s_u((safe_lshift_func_uint8_t_u_s((!0x5F0D2369), (safe_div_func_int8_t_s_s(((safe_mod_func_int16_t_s_s((((((safe_mul_func_uint8_t_u_u((safe_rshift_func_uint8_t_u_u(g_28[1], 1)), (safe_add_func_int64_t_s_s((((((*g_53) < g_2) >= g_72) > (*l_10)) > (*l_10)), (-6))))) , (*g_87)) != (*g_87)) > 1) > (*l_10)), g_16)) , (*g_71)), (*g_71))))), 48));
            for (g_72 = 10; (g_72 <= (-8)); --g_72)
            { 
                return l_111[0][3];
            }
        }
        for (g_18 = 9; (g_18 < 5); g_18 = safe_sub_func_int64_t_s_s(g_18, 1))
        { 
            return (*l_10);
        }
    }
    return (*l_10);
}

static int32_t * func_5(int32_t  p_6)
{ 
    int64_t *l_17 = &g_18;
    int32_t l_23[2];
    union U0 l_50 = {0xC0};
    int32_t l_51[3];
    int32_t l_52 = 0x14E288A3;
    int i;
    for (i = 0; i < 2; i++)
        l_23[i] = 0x6E74A6E8;
    for (i = 0; i < 3; i++)
        l_51[i] = 0xF141C5AB;
    l_52 ^= ((g_2 > (l_51[1] ^= ((((((*l_17) = g_2) , (func_19(l_23[1], l_17, p_6) , l_50)) , g_28[0]) | p_6) | l_23[1]))) & g_2);
    return &g_16;
}

static const union U0  func_19(uint8_t  p_20, int64_t * p_21, int64_t  p_22)
{ 
    int32_t *l_24 = &g_16;
    int32_t *l_25 = &g_16;
    int32_t *l_26 = (void*)0;
    int32_t *l_27[4][4] = {{&g_2,(void*)0,&g_16,(void*)0},{&g_16,&g_16,&g_2,&g_16},{(void*)0,&g_16,&g_16,(void*)0},{&g_16,(void*)0,&g_16,&g_2}};
    union U0 l_45 = {0xA8};
    union U0 l_48[4][3] = {{{249U},{249U},{0x19}},{{0xAB},{0xAB},{3U}},{{249U},{249U},{0x19}},{{0xAB},{0xAB},{3U}}};
    int i, j;
lbl_49:
    --g_28[0];
    for (p_22 = 0; (p_22 > 11); p_22 = safe_add_func_int8_t_s_s(p_22, 6))
    { 
        int32_t l_46 = 0xF8000858;
        int8_t *l_47 = &l_45.f2;
        (*l_25) = (safe_mod_func_uint32_t_u_u((safe_rshift_func_uint8_t_u_u((safe_mul_func_int16_t_s_s(((safe_mod_func_uint64_t_u_u((safe_mul_func_uint32_t_u_u(((safe_mul_func_uint16_t_u_u((l_45 , ((((*l_47) = l_46) & ((l_48[2][2] , 0x575F7F77771FDA66LL) || l_46)) ^ g_2)), 1U)) , 4294967295U), 4294967293U)), l_46)) | g_28[1]), p_22)), g_2)), 0xFD232038));
        for (l_46 = 0; l_46 < 4; l_46 = l_46 + 1)
        {
            for (l_45.f1 = 0; l_45.f1 < 4; l_45.f1 = l_45.f1 + 1)
            {
                l_27[l_46][l_45.f1] = &g_16;
            }
        }
        if (l_46)
            goto lbl_49;
    }
    return l_45;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_28[i], "g_28[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_72, "g_72", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
