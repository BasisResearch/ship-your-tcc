/* Csmith 0cdc710315cf seed 262, flattened by scripts/csmith_flatten.py */
// Options:   --seed 262 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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

struct S0 {
   int32_t  f0;
   const int32_t  f1;
   const uint64_t  f2;
   int64_t  f3;
};

static int32_t g_6 = 0x4AD744D0;
static int16_t g_11[3] = {0x8BFF,0x8BFF,0x8BFF};
static uint16_t g_13 = 0xF0D3;
static struct S0 g_17 = {-8,1,0x632C5B641B44ADCALL,7};

static uint16_t  func_1(void);
static struct S0 * func_2(const uint16_t  p_3);

static uint16_t  func_1(void)
{ 
    const int64_t l_4 = (-9);
    struct S0 *l_19 = &g_17;
    struct S0 **l_18 = &l_19;
    (*l_18) = func_2(l_4);
    return l_4;
}

static struct S0 * func_2(const uint16_t  p_3)
{ 
    int32_t l_5[1][1];
    int32_t *l_7 = &l_5[0][0];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_5[i][j] = 0x173BF38C;
    }
    for (g_6 = 0; (g_6 >= 0); g_6 -= 1)
    { 
        int32_t **l_8 = (void*)0;
        int32_t **l_9 = (void*)0;
        int32_t **l_10 = &l_7;
        int32_t l_12 = 0x92C6EED1;
        if (l_5[0][0])
            break;
        (*l_10) = l_7;
        --g_13;
        for (l_12 = 0; (l_12 >= 0); l_12 -= 1)
        { 
            struct S0 *l_16 = &g_17;
            return l_16;
        }
    }
    return &g_17;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_11[i], "g_11[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_17.f0, "g_17.f0", print_hash_value);
    transparent_crc(g_17.f1, "g_17.f1", print_hash_value);
    transparent_crc(g_17.f2, "g_17.f2", print_hash_value);
    transparent_crc(g_17.f3, "g_17.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
