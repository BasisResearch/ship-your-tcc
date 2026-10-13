/* Csmith 0cdc710315cf seed 48, flattened by scripts/csmith_flatten.py */
// Options:   --seed 48 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
};

struct S3 {
   int32_t  f0;
};

static uint64_t g_15[4][2] = {{0x9B58331C587C7AB8LL,0x9B58331C587C7AB8LL},{0x9B58331C587C7AB8LL,0x9B58331C587C7AB8LL},{0x9B58331C587C7AB8LL,0x9B58331C587C7AB8LL},{0x9B58331C587C7AB8LL,0x9B58331C587C7AB8LL}};
static int32_t g_20 = 0x7D02268F;

static int64_t  func_1(void);
static int32_t * func_3(struct S3  p_4, struct S0  p_5, uint16_t  p_6, uint64_t  p_7, const int16_t  p_8);
static struct S0  func_10(uint8_t  p_11);

static int64_t  func_1(void)
{ 
    int32_t l_2 = 0x23B1A956;
    struct S3 l_9 = {0xA7084BC9};
    int32_t *l_22 = &l_2;
    int32_t **l_21[1];
    int32_t *l_23 = &g_20;
    int i;
    for (i = 0; i < 1; i++)
        l_21[i] = &l_22;
    l_2 = 0x45307222;
    l_23 = func_3(l_9, func_10(l_2), l_9.f0, g_15[0][1], l_9.f0);
    return (*l_23);
}

static int32_t * func_3(struct S3  p_4, struct S0  p_5, uint16_t  p_6, uint64_t  p_7, const int16_t  p_8)
{ 
    uint64_t l_16 = 0x61FC42FF18C9A01CLL;
    int32_t *l_19[3][1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_19[i][j] = &g_20;
    }
    g_20 ^= (l_16 <= (safe_rshift_func_uint64_t_u_u(0U, p_4.f0)));
    return l_19[2][0];
}

static struct S0  func_10(uint8_t  p_11)
{ 
    int16_t l_12[1];
    int32_t l_13 = (-1);
    struct S0 l_14 = {0x2B};
    int i;
    for (i = 0; i < 1; i++)
        l_12[i] = 6;
    l_13 &= l_12[0];
    return l_14;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_15[i][j], "g_15[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_20, "g_20", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
