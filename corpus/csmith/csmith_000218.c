/* Csmith 0cdc710315cf seed 218, flattened by scripts/csmith_flatten.py */
// Options:   --seed 218 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
/* ---- generated program ---- */
static long __undefined;

struct S1 {
   const uint64_t  f0;
   int32_t  f1;
};

struct S4 {
   int16_t  f0;
};

static struct S1 g_14 = {0xD46758E356EC7E43LL,0x392F2092};
static struct S1 *g_13 = &g_14;
static struct S1 g_18 = {0xC5540FFF06982B35LL,-10};
static struct S1 *g_17 = &g_18;
static uint16_t g_20 = 65535U;
static struct S1 g_23 = {0U,0x172C2E8B};
static struct S1 *g_22[1] = {&g_23};
static struct S1 g_27 = {0x5F98A97D63F7CEC7LL,1};
static struct S1 *g_26 = &g_27;

static int64_t  func_1(void);
static struct S1 * func_2(struct S1  p_3, struct S1 * p_4, int64_t  p_5, int32_t  p_6);
static struct S1 * func_8(struct S1 * p_9, struct S4  p_10, struct S1 * p_11, int64_t  p_12);

static int64_t  func_1(void)
{ 
    struct S1 l_7 = {0x837182ABBF9ACF25LL,0x9253A34C};
    struct S4 l_15 = {0xCADD};
    struct S1 **l_16[1][1];
    uint16_t *l_19 = &g_20;
    const int16_t l_21 = (-10);
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_16[i][j] = (void*)0;
    }
    g_26 = func_2(l_7, (g_22[0] = (((((*l_19) ^= (((g_17 = (g_13 = func_8(g_13, l_15, &g_14, g_14.f0))) != &g_14) > l_15.f0)) >= l_21) == 0x4468) , (void*)0)), g_14.f1, g_18.f0);
    return l_7.f0;
}

static struct S1 * func_2(struct S1  p_3, struct S1 * p_4, int64_t  p_5, int32_t  p_6)
{ 
    struct S1 **l_24[3];
    int32_t *l_25 = &g_18.f1;
    int i;
    for (i = 0; i < 3; i++)
        l_24[i] = &g_13;
    g_22[0] = (void*)0;
    (*l_25) |= 0x1CF2BBA2;
    return &g_18;
}

static struct S1 * func_8(struct S1 * p_9, struct S4  p_10, struct S1 * p_11, int64_t  p_12)
{ 
    return &g_14;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_14.f0, "g_14.f0", print_hash_value);
    transparent_crc(g_14.f1, "g_14.f1", print_hash_value);
    transparent_crc(g_18.f0, "g_18.f0", print_hash_value);
    transparent_crc(g_18.f1, "g_18.f1", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_23.f0, "g_23.f0", print_hash_value);
    transparent_crc(g_23.f1, "g_23.f1", print_hash_value);
    transparent_crc(g_27.f0, "g_27.f0", print_hash_value);
    transparent_crc(g_27.f1, "g_27.f1", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
