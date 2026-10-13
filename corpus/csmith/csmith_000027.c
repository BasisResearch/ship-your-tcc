/* Csmith 0cdc710315cf seed 27, flattened by scripts/csmith_flatten.py */
// Options:   --seed 27 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
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
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
/* ---- generated program ---- */
static long __undefined;

struct S2 {
   int16_t  f0;
   uint64_t  f1;
};

struct S3 {
   uint64_t  f0;
   const struct S2  f1;
   int32_t  f2;
};

struct S0 {
   const uint64_t  f0;
   int32_t  f1;
   int32_t  f2;
};

struct S4 {
   uint32_t  f0;
   struct S0  f1;
   int16_t  f2;
   struct S0  f3;
};

union U5 {
   uint32_t  f0;
   int64_t  f1;
   int8_t  f2;
};

static const int16_t g_5 = 1;
static int32_t g_10 = 0x062D7BC3;
static uint64_t g_32 = 0x41E971521F00BB6ELL;
static uint8_t g_48 = 0U;
static int32_t g_51 = 4;
static int16_t g_62 = 0x9E3F;
static const int32_t *g_69 = (void*)0;
static int32_t *g_70[1][2] = {{(void*)0,(void*)0}};
static struct S4 g_88 = {4294967295U,{1U,0x21A34F7B,0x26758D62},0x3E90,{4U,-7,-8}};
static struct S2 g_90 = {3,0x913E687FAFF4B716LL};
static union U5 g_99 = {4294967295U};

static union U5  func_1(void);
static struct S3  func_13(int32_t  p_14, int32_t  p_15, const int32_t * p_16, int32_t * p_17, int32_t * p_18);
static struct S2  func_21(uint16_t  p_22, uint64_t  p_23, struct S4  p_24, const uint8_t  p_25);

static union U5  func_1(void)
{ 
    int16_t l_6 = (-2);
    int32_t *l_7 = (void*)0;
    int32_t *l_8 = (void*)0;
    int32_t *l_9[3][1];
    struct S4 l_26 = {4294967292U,{18446744073709551615U,0xEFEC1A2C,0xDBE4FCD5},-1,{0xD92279191D4D0043LL,0x4FCE8863,0xEEBCB174}};
    int16_t l_95[3];
    union U5 l_100 = {0U};
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_9[i][j] = &g_10;
    }
    for (i = 0; i < 3; i++)
        l_95[i] = 0;
    g_10 ^= (0xE4B7834D8A94B2ABLL ^ ((safe_mul_func_int32_t_s_s((((~g_5) ^ (18446744073709551612U || l_6)) == g_5), l_6)) & g_5));
    if (g_10)
        goto lbl_96;
lbl_96:
    l_95[2] = ((((safe_add_func_uint32_t_u_u((func_13(((safe_rshift_func_int32_t_s_s((&g_10 != (func_21(g_10, (g_10 , 18446744073709551615U), l_26, g_10) , l_8)), g_5)) , (-5)), g_5, g_69, l_9[2][0], g_70[0][1]) , g_51), 0x905F98FF)) || g_88.f1.f2) > g_88.f1.f1) && 0x17E09DBB);
    for (g_48 = (-23); (g_48 > 37); g_48 = safe_add_func_int16_t_s_s(g_48, 5))
    { 
        return g_99;
    }
    return l_100;
}

static struct S3  func_13(int32_t  p_14, int32_t  p_15, const int32_t * p_16, int32_t * p_17, int32_t * p_18)
{ 
    int64_t l_71[2][1];
    int32_t l_81 = (-9);
    int32_t l_82 = 0xB7C8A084;
    int32_t l_83[3][4] = {{0x04D8298A,0x62F9B9B5,0x62F9B9B5,0x04D8298A},{1,0x62F9B9B5,0xCEE854D9,0x62F9B9B5},{0x62F9B9B5,0x325F0A28,0xCEE854D9,0xCEE854D9}};
    int64_t l_84 = 3;
    uint16_t l_85[2];
    int16_t * const l_91 = &g_88.f2;
    struct S3 l_94 = {18446744073709551614U,{0xA199,18446744073709551613U},0x949AADFC};
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_71[i][j] = (-2);
    }
    for (i = 0; i < 2; i++)
        l_85[i] = 0x6533;
    (*p_17) = l_71[1][0];
    if ((+((safe_lshift_func_int8_t_s_s((safe_lshift_func_uint8_t_u_u(p_14, l_71[1][0])), 3)) , ((safe_mul_func_uint32_t_u_u(p_14, p_14)) != g_62))))
    { 
        int32_t *l_79 = &g_51;
        int32_t *l_80[3];
        int i;
        for (i = 0; i < 3; i++)
            l_80[i] = &g_51;
        l_85[1]++;
    }
    else
    { 
        struct S2 *l_89 = &g_90;
        int32_t l_92 = 1;
        int32_t **l_93 = &g_70[0][1];
        (*l_89) = func_21((p_15 && 0xF06D), g_62, g_88, l_84);
        l_92 = (l_91 != &g_62);
        (*l_93) = &g_51;
    }
    return l_94;
}

static struct S2  func_21(uint16_t  p_22, uint64_t  p_23, struct S4  p_24, const uint8_t  p_25)
{ 
    int16_t *l_61 = &g_62;
    int32_t *l_63 = (void*)0;
    int32_t *l_64 = &g_51;
    int16_t **l_65 = &l_61;
    int16_t *l_66 = (void*)0;
    int32_t *l_67[1];
    struct S2 l_68 = {0xF501,0x3ADA1D0942DF00ACLL};
    int i;
    for (i = 0; i < 1; i++)
        l_67[i] = &g_10;
    for (p_24.f1.f2 = 20; (p_24.f1.f2 > 9); p_24.f1.f2 = safe_sub_func_uint16_t_u_u(p_24.f1.f2, 7))
    { 
        int64_t l_31[3][3] = {{0xA1AE281B1D0B827BLL,0xA1AE281B1D0B827BLL,0xA1AE281B1D0B827BLL},{7,0x23D12EF61160DD4FLL,7},{0xA1AE281B1D0B827BLL,0xA1AE281B1D0B827BLL,0xA1AE281B1D0B827BLL}};
        int32_t *l_55[1][1];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_55[i][j] = &g_51;
        }
        for (g_10 = 0; (g_10 < (-27)); g_10 = safe_sub_func_uint64_t_u_u(g_10, 4))
        { 
            int64_t l_46 = (-8);
            for (p_24.f3.f1 = 0; (p_24.f3.f1 <= 2); p_24.f3.f1 += 1)
            { 
                struct S3 l_39 = {0xB9FE2185AEBC5F7CLL,{0xCB0A,18446744073709551615U},-9};
                uint8_t *l_47 = &g_48;
                int32_t *l_49 = (void*)0;
                int32_t *l_50 = &g_51;
                int32_t **l_52 = &l_50;
                int i, j;
                g_32++;
                (*l_50) |= ((255U && ((safe_mod_func_uint16_t_u_u((safe_add_func_int32_t_s_s((((*l_47) = (l_39 , ((safe_add_func_int8_t_s_s(((safe_add_func_uint32_t_u_u((safe_lshift_func_uint8_t_u_s(l_31[p_24.f3.f1][p_24.f3.f1], 1)), 1)) > 0xD1), 0xE4)) & l_46))) , g_32), p_24.f1.f2)), 0xAC6D)) | g_5)) <= 0U);
                (*l_52) = &g_10;
            }
            if (g_48)
                continue;
        }
        g_10 = (safe_lshift_func_int64_t_s_s(0xD946680AE02F1192LL, 44));
    }
    (*l_64) &= (safe_lshift_func_int16_t_s_s(p_24.f2, ((*l_61) &= (safe_rshift_func_int8_t_s_s((~1), 1)))));
    p_24.f1.f1 |= (((((*l_64) && g_5) || (((*l_65) = l_61) == l_66)) <= 18446744073709551607U) , 1);
    return l_68;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_88.f0, "g_88.f0", print_hash_value);
    transparent_crc(g_88.f1.f0, "g_88.f1.f0", print_hash_value);
    transparent_crc(g_88.f1.f1, "g_88.f1.f1", print_hash_value);
    transparent_crc(g_88.f1.f2, "g_88.f1.f2", print_hash_value);
    transparent_crc(g_88.f2, "g_88.f2", print_hash_value);
    transparent_crc(g_88.f3.f0, "g_88.f3.f0", print_hash_value);
    transparent_crc(g_88.f3.f1, "g_88.f3.f1", print_hash_value);
    transparent_crc(g_88.f3.f2, "g_88.f3.f2", print_hash_value);
    transparent_crc(g_90.f0, "g_90.f0", print_hash_value);
    transparent_crc(g_90.f1, "g_90.f1", print_hash_value);
    transparent_crc(g_99.f0, "g_99.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
