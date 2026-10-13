/* Csmith 0cdc710315cf seed 65, flattened by scripts/csmith_flatten.py */
// Options:   --seed 65 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
/* ---- generated program ---- */
static long __undefined;

static uint16_t g_7 = 2U;
static int32_t g_15 = 7;
static int32_t *g_14 = &g_15;
static uint32_t g_23 = 0xD34CAA8E;
static int32_t g_26 = 0x25739271;

static int8_t  func_1(void);
static int32_t * func_2(int8_t  p_3, int32_t * p_4, int64_t  p_5, int32_t * p_6);
static int64_t  func_9(int32_t * p_10, int32_t * p_11, uint32_t  p_12, uint64_t  p_13);

static int8_t  func_1(void)
{ 
    int32_t *l_8[2][1];
    int32_t **l_27[1][3];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_8[i][j] = (void*)0;
    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_27[i][j] = &l_8[1][0];
    }
    g_14 = func_2(g_7, l_8[0][0], func_9(g_14, &g_15, g_15, g_7), l_8[0][0]);
    return g_26;
}

static int32_t * func_2(int8_t  p_3, int32_t * p_4, int64_t  p_5, int32_t * p_6)
{ 
    int32_t *l_21 = &g_15;
    uint32_t *l_22 = &g_23;
    uint32_t l_24[2];
    int32_t *l_25 = &g_26;
    int i;
    for (i = 0; i < 2; i++)
        l_24[i] = 0x0FAEB382;
    (*l_25) |= (safe_rshift_func_uint8_t_u_u(((((void*)0 == l_21) > ((*l_22) = (*l_21))) & (-1)), l_24[0]));
    return l_21;
}

static int64_t  func_9(int32_t * p_10, int32_t * p_11, uint32_t  p_12, uint64_t  p_13)
{ 
    uint64_t l_18 = 0x0CD9D75405732AA6LL;
    for (p_13 = 0; (p_13 != 57); p_13++)
    { 
        return l_18;
    }
    return l_18;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
