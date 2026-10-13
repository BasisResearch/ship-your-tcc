/* Csmith 0cdc710315cf seed 4, flattened by scripts/csmith_flatten.py */
// Options:   --seed 4 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
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
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   int64_t  f1;
   uint16_t  f2;
};

union U2 {
   uint16_t  f0;
};

union U3 {
   const uint64_t  f0;
   struct S0  f1;
};

union U6 {
   const struct S0  f0;
};

static int32_t g_5 = 8;
static uint8_t g_12 = 0xAF;
static uint16_t g_15 = 0x7941;
static int32_t g_19 = 0x2C40A98C;
static union U3 g_33 = {1U};
static int32_t g_40 = 0x4C38D679;
static int32_t * const g_39 = &g_40;
static int32_t * const *g_38 = &g_39;
static struct S0 g_55 = {0x489718B2F1036C14LL,0x8E8DCE9DB3FA3CE7LL,1U};
static const int32_t *g_63 = &g_5;
static const int32_t **g_62[1] = {&g_63};
static int8_t g_67[2] = {0xC0,0xC0};
static uint64_t g_68 = 0xE690D922041F5FBDLL;
static int32_t *g_72 = &g_19;
static int32_t **g_71 = &g_72;
static struct S0 *g_82 = (void*)0;
static struct S0 **g_81 = &g_82;
static struct S0 **g_83 = &g_82;

static union U6  func_1(void);
static int32_t * func_20(const int32_t ** p_21, union U3  p_22, uint16_t * p_23, const uint8_t * p_24);
static const int32_t ** func_25(int16_t  p_26, uint16_t * p_27, union U2  p_28, union U3  p_29);

static union U6  func_1(void)
{ 
    int32_t **l_2 = (void*)0;
    int32_t *l_4[1][1];
    int32_t **l_3 = &l_4[0][0];
    int32_t *l_18 = &g_19;
    union U2 l_32 = {65526U};
    uint16_t *l_65 = &g_55.f2;
    uint8_t *l_78 = &g_12;
    uint8_t **l_79 = &l_78;
    struct S0 **l_80 = (void*)0;
    union U6 l_84 = {{1,5,0xE7FB}};
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_4[i][j] = (void*)0;
    }
    (*l_3) = (void*)0;
    for (g_5 = 0; (g_5 < 27); g_5 = safe_add_func_int8_t_s_s(g_5, 4))
    { 
        int16_t l_10 = 0xA8B3;
        uint8_t *l_11[3];
        int32_t l_13[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
        uint16_t *l_14 = &g_15;
        int i, j;
        for (i = 0; i < 3; i++)
            l_11[i] = &g_12;
        if ((g_5 | (safe_mod_func_int16_t_s_s(((l_10 < (((*l_14) = ((l_13[1][3] = 5U) != g_5)) | l_10)) | 7), 0x8B16))))
        { 
            int32_t **l_16 = (void*)0;
            int32_t **l_17 = (void*)0;
            l_18 = ((*l_3) = &g_5);
        }
        else
        { 
            int8_t l_30[3];
            uint16_t *l_31 = (void*)0;
            int i;
            for (i = 0; i < 3; i++)
                l_30[i] = 0xB0;
            (*l_3) = func_20(func_25(l_30[0], l_31, l_32, g_33), g_33, l_65, &g_12);
            l_2 = (g_71 = g_71);
        }
        for (l_32.f0 = 0; (l_32.f0 > 34); l_32.f0 = safe_add_func_uint16_t_u_u(l_32.f0, 4))
        { 
            const int8_t l_77 = 0x1B;
            l_13[3][1] = (g_55.f2 | (((safe_mul_func_int32_t_s_s((0U <= g_55.f2), (**g_71))) > l_77) & 0x3AA3630DD35930CBLL));
            (*g_72) |= (-9);
        }
    }
    (*g_39) &= ((((((*l_79) = l_78) != (((((l_80 = (g_81 = l_80)) == (g_83 = &g_82)) , g_19) >= 0xCB41) , &g_12)) > g_68) ^ (*l_18)) | g_67[0]);
    return l_84;
}

static int32_t * func_20(const int32_t ** p_21, union U3  p_22, uint16_t * p_23, const uint8_t * p_24)
{ 
    int32_t *l_66[2];
    int i;
    for (i = 0; i < 2; i++)
        l_66[i] = &g_40;
    g_68++;
    return l_66[1];
}

static const int32_t ** func_25(int16_t  p_26, uint16_t * p_27, union U2  p_28, union U3  p_29)
{ 
    int32_t *l_37 = (void*)0;
    int32_t * const *l_36 = &l_37;
    uint32_t l_41 = 0xBC906383;
    uint8_t l_49[1][3];
    struct S0 *l_50 = (void*)0;
    const int32_t **l_64[2];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_49[i][j] = 0x26;
    }
    for (i = 0; i < 2; i++)
        l_64[i] = (void*)0;
    if ((safe_mul_func_uint16_t_u_u(((g_38 = l_36) != &l_37), (p_28.f0 ^ l_41))))
    { 
        int32_t ** const l_48[1] = {&l_37};
        struct S0 **l_51 = (void*)0;
        struct S0 **l_52 = &l_50;
        int i;
        l_49[0][2] = (safe_rshift_func_uint64_t_u_u(((safe_mul_func_int32_t_s_s((g_40 ^ ((void*)0 == l_48[0])), 0)) | g_5), p_28.f0));
        (*l_52) = l_50;
    }
    else
    { 
        uint64_t l_60 = 0xE7924A04C7C9A7B2LL;
        for (p_29.f1.f0 = 0; (p_29.f1.f0 == 7); p_29.f1.f0++)
        { 
            int32_t *l_61[2];
            int i;
            for (i = 0; i < 2; i++)
                l_61[i] = (void*)0;
            g_19 |= (g_55 , ((safe_lshift_func_int32_t_s_s((safe_sub_func_int64_t_s_s(l_60, 0xAD50A0C201006516LL)), 19)) , (*g_39)));
        }
        return g_62[0];
    }
    g_38 = &l_37;
    return l_64[0];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_33.f0, "g_33.f0", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    transparent_crc(g_55.f0, "g_55.f0", print_hash_value);
    transparent_crc(g_55.f1, "g_55.f1", print_hash_value);
    transparent_crc(g_55.f2, "g_55.f2", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_67[i], "g_67[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_68, "g_68", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
