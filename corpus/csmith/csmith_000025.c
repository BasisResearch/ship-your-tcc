/* Csmith 0cdc710315cf seed 25, flattened by scripts/csmith_flatten.py */
// Options:   --seed 25 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_14 = 0x1C97412D;
static int32_t g_18 = 7;
static const int32_t *g_22 = &g_14;
static const int32_t **g_21 = &g_22;
static uint8_t g_47 = 248U;
static uint8_t g_49 = 6U;
static int32_t g_51 = 6;
static uint32_t g_59 = 0x0D5D742C;
static int32_t g_68 = 0xFDCBC339;
static int64_t g_69 = 0xF73ECB6AEF2D6A39LL;
static uint64_t g_70[1] = {0x720603003393D3B2LL};

static const int8_t  func_1(void);
static int32_t * func_2(int32_t * p_3, uint16_t  p_4, int32_t * p_5, int32_t * p_6, uint64_t  p_7);
static int16_t  func_8(int32_t * p_9, uint64_t  p_10, uint32_t  p_11, int32_t * p_12);

static const int8_t  func_1(void)
{ 
    int32_t *l_13 = &g_14;
    int32_t **l_25 = &l_13;
    uint16_t l_26[1][4];
    int32_t *l_78 = &g_68;
    int32_t l_79 = 0xE64F81AA;
    int32_t l_80 = (-2);
    int32_t *l_81[4][2];
    uint16_t l_82 = 0xC01E;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_26[i][j] = 0xAE9F;
    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
            l_81[i][j] = &g_51;
    }
    (*g_21) = ((*l_25) = func_2(((((func_8(l_13, (l_13 == l_13), (*l_13), &g_14) || 65535U) <= g_14) | 0x43DDCA27) , (void*)0), (*l_13), &g_14, l_13, g_14));
    for (g_18 = 0; (g_18 <= 0); g_18 += 1)
    { 
        int16_t l_27 = 0x7276;
    }
    --l_82;
    return g_70[0];
}

static int32_t * func_2(int32_t * p_3, uint16_t  p_4, int32_t * p_5, int32_t * p_6, uint64_t  p_7)
{ 
    int32_t *l_24[2][3];
    int32_t * const *l_23 = &l_24[1][2];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
            l_24[i][j] = &g_18;
    }
    (*p_6) = (safe_mul_func_uint16_t_u_u((((l_23 = (((void*)0 == g_21) , l_23)) != &g_22) ^ g_14), p_4));
    return p_5;
}

static int16_t  func_8(int32_t * p_9, uint64_t  p_10, uint32_t  p_11, int32_t * p_12)
{ 
    int32_t *l_16 = &g_14;
    int32_t **l_15 = &l_16;
    int32_t *l_17 = &g_18;
    (*l_15) = (void*)0;
    (*l_17) |= g_14;
    return g_14;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_68, "g_68", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_70[i], "g_70[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
