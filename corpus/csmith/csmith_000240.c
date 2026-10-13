/* Csmith 0cdc710315cf seed 240, flattened by scripts/csmith_flatten.py */
// Options:   --seed 240 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint16_t  f0;
};

struct S1 {
   int16_t  f0;
   uint8_t  f1;
};

union U2 {
   uint32_t  f0;
   struct S1  f1;
};

static uint8_t g_5 = 9U;
static union U2 g_16 = {4294967286U};
static union U2 *g_25 = &g_16;
static union U2 **g_24 = &g_25;
static struct S0 g_36 = {0xA7AB};
static uint32_t g_47 = 0xD5EBAE88;
static uint16_t g_57 = 1U;
static int32_t g_61 = 0x05104E0B;
static int32_t *g_60 = &g_61;
static uint64_t g_75 = 0x12536DE7EBAC3F40LL;
static int8_t g_86 = 0;

static int16_t  func_1(void);
static int32_t  func_2(uint64_t  p_3, uint32_t  p_4);
static int16_t  func_9(const uint16_t  p_10, const uint64_t  p_11, int16_t  p_12);

static int16_t  func_1(void)
{ 
    uint32_t l_6[3][4] = {{4294967295U,4294967295U,4294967295U,4294967295U},{4294967295U,4294967295U,4294967295U,4294967295U},{4294967295U,4294967295U,4294967295U,4294967295U}};
    int32_t *l_93 = (void*)0;
    int32_t *l_94 = &g_61;
    int i, j;
    (*l_94) = func_2(g_5, l_6[2][2]);
    return (*l_94);
}

static int32_t  func_2(uint64_t  p_3, uint32_t  p_4)
{ 
    const int16_t l_13[1] = {0x7084};
    int32_t l_32 = 0x18B9628E;
    uint32_t l_52 = 0x7276463A;
    struct S1 l_65[2][2] = {{{-2,0U},{-2,0U}},{{-2,0U},{-2,0U}}};
    int8_t *l_85[4][2] = {{&g_86,&g_86},{&g_86,&g_86},{&g_86,&g_86},{&g_86,&g_86}};
    uint8_t *l_89 = &l_65[0][0].f1;
    int32_t *l_92 = &g_61;
    int i, j;
    if ((l_32 = (safe_sub_func_int8_t_s_s(p_4, (func_9((l_13[0] , (p_3 & g_5)), p_3, g_5) ^ p_4)))))
    { 
        struct S0 *l_35 = &g_36;
        int32_t l_40 = (-1);
        int32_t l_42 = 1;
        int32_t l_43 = 0xC4B79A97;
        uint8_t l_44 = 0x82;
        int32_t *l_55 = &l_32;
        int32_t *l_56[1][3];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_56[i][j] = &l_40;
        }
        for (p_3 = 0; (p_3 > 5); p_3++)
        { 
            struct S0 **l_37 = (void*)0;
            struct S0 **l_38 = &l_35;
            int32_t *l_39 = &l_32;
            int32_t *l_41[4] = {&l_40,&l_40,&l_40,&l_40};
            int i;
            (*l_38) = l_35;
            l_44++;
            g_47--;
        }
        (*l_55) = (safe_sub_func_int32_t_s_s((l_52 || (safe_sub_func_int8_t_s_s(p_3, p_3))), g_47));
        g_57--;
    }
    else
    { 
        int32_t **l_62 = &g_60;
        (*l_62) = g_60;
    }
    if ((p_3 & (0x78 == 0x65)))
    { 
        struct S1 *l_66 = &l_65[1][0];
        (*l_66) = l_65[0][0];
        for (g_61 = 16; (g_61 <= 26); g_61 = safe_add_func_int32_t_s_s(g_61, 1))
        { 
            int32_t *l_69 = &l_32;
            (*l_69) ^= 0x1356723F;
        }
    }
    else
    { 
        int32_t **l_70 = &g_60;
        uint64_t *l_74 = &g_75;
        (*l_70) = &l_32;
        if ((((((*l_74) = (safe_lshift_func_uint32_t_u_u((g_61 , 4294967294U), ((~((g_47 && 65535U) ^ g_47)) > 0xB7)))) , p_4) == 0xBD03) != g_61))
        { 
            int64_t l_80[2];
            uint8_t *l_81 = &g_16.f1.f1;
            int32_t *l_82 = (void*)0;
            int i;
            for (i = 0; i < 2; i++)
                l_80[i] = 1;
            (*g_60) |= (((safe_mul_func_uint16_t_u_u((((*l_81) = ((((((!((safe_unary_minus_func_uint8_t_u((&g_60 == &g_60))) >= (p_4 | (-6)))) | 0xC6) , g_36.f0) , g_36.f0) , l_80[0]) ^ p_3)) > 1U), 0)) == g_47) != (-1));
            g_60 = l_82;
        }
        else
        { 
            (*l_70) = (*l_70);
            for (p_3 = 15; (p_3 >= 24); p_3 = safe_add_func_int16_t_s_s(p_3, 1))
            { 
                return p_4;
            }
        }
    }
    (*l_92) = ((g_86 = (((l_32 = 0) >= ((safe_rshift_func_uint8_t_u_s(((*l_89)--), 1)) >= g_75)) ^ g_61)) || g_47);
    return p_3;
}

static int16_t  func_9(const uint16_t  p_10, const uint64_t  p_11, int16_t  p_12)
{ 
    union U2 *l_15 = &g_16;
    union U2 **l_14 = &l_15;
    const int32_t l_21 = 0xC7AB89BD;
    (*l_14) = (void*)0;
    if ((safe_sub_func_uint64_t_u_u(p_10, ((safe_mul_func_int8_t_s_s((l_21 | ((safe_sub_func_uint16_t_u_u(g_5, p_11)) > 0x5261)), 9)) > l_21))))
    { 
        union U2 **l_26 = (void*)0;
        l_26 = g_24;
    }
    else
    { 
        int32_t *l_27 = (void*)0;
        int32_t **l_28 = &l_27;
        (*l_28) = l_27;
    }
    for (g_16.f1.f1 = 0; (g_16.f1.f1 > 1); g_16.f1.f1 = safe_add_func_uint16_t_u_u(g_16.f1.f1, 8))
    { 
        uint64_t l_31[4] = {0U,0U,0U,0U};
        int i;
        return l_31[2];
    }
    return l_21;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_16.f0, "g_16.f0", print_hash_value);
    transparent_crc(g_36.f0, "g_36.f0", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_75, "g_75", print_hash_value);
    transparent_crc(g_86, "g_86", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
