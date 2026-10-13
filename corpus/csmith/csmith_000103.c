/* Csmith 0cdc710315cf seed 103, flattened by scripts/csmith_flatten.py */
// Options:   --seed 103 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
   int64_t  f0;
};

static int32_t g_3 = 8;
static int32_t *g_5 = &g_3;
static struct S0 g_15 = {0};
static int32_t g_17 = 0xDBF0CF86;
static int64_t g_19 = 6;
static int32_t g_20 = 6;
static uint8_t g_21[4][4] = {{0U,0U,0U,0U},{0U,0U,0U,246U},{0U,0U,0U,4U},{0U,246U,4U,4U}};

static uint8_t  func_1(void);
static struct S0 * func_9(int16_t  p_10, int32_t  p_11, int32_t * p_12);
static const int32_t  func_13(struct S0  p_14);

static uint8_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t **l_4[2][1];
    int32_t *l_6 = &g_3;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_4[i][j] = (void*)0;
    }
    l_6 = (g_5 = l_2);
    for (g_3 = (-23); (g_3 < (-11)); ++g_3)
    { 
        int16_t l_24 = 0x00D3;
        int32_t *l_25 = (void*)0;
        struct S0 *l_28[3][3] = {{&g_15,&g_15,(void*)0},{&g_15,&g_15,(void*)0},{&g_15,&g_15,(void*)0}};
        struct S0 **l_27 = &l_28[2][2];
        int i, j;
        (*l_27) = func_9((func_13(g_15) <= (*g_5)), l_24, l_25);
        return g_15.f0;
    }
    return (*l_6);
}

static struct S0 * func_9(int16_t  p_10, int32_t  p_11, int32_t * p_12)
{ 
    struct S0 *l_26 = &g_15;
    return l_26;
}

static const int32_t  func_13(struct S0  p_14)
{ 
    int32_t *l_16 = &g_17;
    int32_t *l_18[4][1];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
            l_18[i][j] = &g_17;
    }
    --g_21[0][1];
    return p_14.f0;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_15.f0, "g_15.f0", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_21[i][j], "g_21[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
