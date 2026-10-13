/* Csmith 0cdc710315cf seed 244, flattened by scripts/csmith_flatten.py */
// Options:   --seed 244 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
/* ---- generated program ---- */
static long __undefined;

static uint32_t g_4 = 9U;
static int32_t g_7 = 0;
static int64_t g_10[3][4] = {{0xE197E7C14AB653B1LL,0,0xE197E7C14AB653B1LL,0xE197E7C14AB653B1LL},{0,0,0x51FA4C8210466712LL,0},{0,0xE197E7C14AB653B1LL,0xE197E7C14AB653B1LL,0}};
static uint8_t g_13 = 248U;
static int64_t g_16 = (-1);
static uint16_t g_18 = 0U;
static uint32_t g_26 = 0x8C4F412F;

static int8_t  func_1(void);

static int8_t  func_1(void)
{ 
    const int8_t l_5 = 0x8E;
    int32_t l_9 = (-1);
    int64_t l_11 = 0xC1E0F758FC5674FALL;
    int32_t l_12 = (-1);
    int32_t l_17[3][3] = {{0x45597EED,0x45597EED,0x45597EED},{1,7,1},{0x45597EED,0x45597EED,0x45597EED}};
    int i, j;
    if ((0x7C ^ (safe_div_func_uint16_t_u_u(((((18446744073709551614U & (g_4 == g_4)) & 0xFB) & 0x0E) , l_5), l_5))))
    { 
        int32_t *l_6 = &g_7;
        int32_t *l_8[1];
        int i;
        for (i = 0; i < 1; i++)
            l_8[i] = (void*)0;
        ++g_13;
        g_18++;
    }
    else
    { 
        int32_t *l_21 = &l_17[0][0];
        int32_t *l_22 = &l_9;
        int32_t *l_23 = &l_12;
        int32_t *l_24 = &g_7;
        int32_t *l_25[1][3];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_25[i][j] = &l_9;
        }
        ++g_26;
    }
    return g_4;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_10[i][j], "g_10[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
