/* Csmith 0cdc710315cf seed 223, flattened by scripts/csmith_flatten.py */
// Options:   --seed 223 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int16_t  f0;
};

static uint64_t g_14 = 0xE1D11023DCDF5EF8LL;
static const int32_t g_20 = 0x54E3F157;
static const int32_t *g_19[1][1] = {{&g_20}};
static int32_t g_25 = 0;
static uint64_t g_27[2] = {1U,1U};
static int32_t g_33 = 0x372625AA;
static struct S0 g_34 = {0xCB08};

static uint64_t  func_1(void);
static uint16_t  func_7(uint64_t  p_8, int16_t  p_9, int8_t  p_10, const int32_t  p_11, int8_t  p_12);
static int32_t * func_15(const int32_t * p_16, struct S0  p_17, int32_t  p_18);

static uint64_t  func_1(void)
{ 
    int32_t l_6 = 6;
    int32_t *l_32 = &g_33;
    struct S0 *l_35 = &g_34;
    uint64_t l_38 = 0xC933337672A6DFE1LL;
    (*l_32) ^= (safe_rshift_func_uint64_t_u_u((((((safe_mul_func_int16_t_s_s((l_6 = ((((l_6 != func_7((~l_6), l_6, g_14, g_14, l_6)) , l_6) <= l_6) != 0xA925F321EE01E6E8LL)), g_14)) && l_6) >= g_14) <= (-2)) == g_14), 10));
    (*l_35) = g_34;
    for (l_6 = 0; (l_6 > (-5)); l_6 = safe_sub_func_int32_t_s_s(l_6, 9))
    { 
        int32_t **l_39 = &l_32;
        (*l_39) = func_15(&g_33, g_34, l_38);
    }
    return (*l_32);
}

static uint16_t  func_7(uint64_t  p_8, int16_t  p_9, int8_t  p_10, const int32_t  p_11, int8_t  p_12)
{ 
    struct S0 l_21 = {-5};
    int32_t **l_30 = (void*)0;
    int32_t *l_31 = &g_25;
    l_31 = func_15(g_19[0][0], l_21, (l_21.f0 > l_21.f0));
    return g_14;
}

static int32_t * func_15(const int32_t * p_16, struct S0  p_17, int32_t  p_18)
{ 
    uint16_t l_22[4] = {0x196E,0x196E,0x196E,0x196E};
    int32_t *l_23 = (void*)0;
    int32_t *l_24 = &g_25;
    int32_t *l_26[1][3];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_26[i][j] = &g_25;
    }
    (*l_24) = l_22[0];
    --g_27[0];
    return &g_25;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_27[i], "g_27[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_34.f0, "g_34.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
