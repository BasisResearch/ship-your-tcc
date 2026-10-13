/* Csmith 0cdc710315cf seed 237, flattened by scripts/csmith_flatten.py */
// Options:   --seed 237 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
   int32_t  f0;
   uint32_t  f1;
   int16_t  f2;
   const uint16_t  f3;
};

struct S4 {
   int64_t  f0;
   int16_t  f1;
};

static int32_t g_2 = 0xDFD94759;
static int32_t g_5 = 0x4896E6A3;
static int32_t g_10 = (-9);
static struct S4 g_18 = {0x4D9A54E5878ED7C3LL,-1};
static int32_t g_30 = 0;
static uint8_t g_37 = 0x8E;

static struct S0  func_1(void);
static const int32_t  func_11(const int32_t * p_12);
static struct S4 * const  func_13(int32_t * p_14, struct S4 * p_15);

static struct S0  func_1(void)
{ 
    int32_t l_36 = 0x969E0CD1;
    struct S0 l_40[2][3] = {{{0,0x03A67D62,-1,1U},{-4,3U,2,1U},{0,0x03A67D62,-1,1U}},{{0,0x03A67D62,-1,1U},{-4,3U,2,1U},{0,0x03A67D62,-1,1U}}};
    int i, j;
    for (g_2 = 0; (g_2 > 8); g_2 = safe_add_func_int16_t_s_s(g_2, 8))
    { 
        for (g_5 = (-27); (g_5 >= 3); g_5 = safe_add_func_int8_t_s_s(g_5, 4))
        { 
            uint16_t l_8 = 0x8003;
            int32_t *l_9 = &g_10;
            (*l_9) = ((g_5 == ((0x9FE57F6CC9CA3191LL > l_8) , 0x08ECB302AFF63588LL)) && 65526U);
            if (g_5)
                continue;
            (*l_9) = func_11(&g_5);
        }
    }
    for (g_2 = 22; (g_2 < 3); g_2 = safe_sub_func_int64_t_s_s(g_2, 7))
    { 
        int32_t *l_35[2];
        int i;
        for (i = 0; i < 2; i++)
            l_35[i] = &g_30;
        g_37--;
        if (g_18.f0)
            break;
    }
    return l_40[0][0];
}

static const int32_t  func_11(const int32_t * p_12)
{ 
    int32_t *l_16 = &g_2;
    struct S4 *l_17[2];
    struct S4 **l_19 = &l_17[0];
    int32_t l_32[2][3] = {{6,(-8),6},{6,(-8),6}};
    int i, j;
    for (i = 0; i < 2; i++)
        l_17[i] = &g_18;
    (*l_19) = func_13(l_16, l_17[0]);
    for (g_10 = 0; (g_10 >= 24); g_10 = safe_add_func_int16_t_s_s(g_10, 6))
    { 
        int8_t l_28 = 0x0F;
        int32_t *l_29 = &g_30;
        int32_t *l_31[2][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_31[i][j] = (void*)0;
        }
        l_32[1][0] = ((safe_lshift_func_int32_t_s_u(((*l_29) = (safe_lshift_func_int8_t_s_s((safe_sub_func_int64_t_s_s((0x23782ECC && (*p_12)), ((((*l_16) && 0x4C) || 0x3C17) > 0x0AEACB2D))), l_28))), 22)) | g_18.f1);
    }
    return (*p_12);
}

static struct S4 * const  func_13(int32_t * p_14, struct S4 * p_15)
{ 
    return &g_18;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_18.f0, "g_18.f0", print_hash_value);
    transparent_crc(g_18.f1, "g_18.f1", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
