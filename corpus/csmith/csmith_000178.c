/* Csmith 0cdc710315cf seed 178, flattened by scripts/csmith_flatten.py */
// Options:   --seed 178 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_unary_minus_func_int32_t_s(int32_t si )
{
  return
    (si==(-2147483647-1)) ?
    ((si)) :
    -si;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static const int64_t g_10 = 0xD073FF82FF40FC02LL;
static int8_t g_14 = 0x56;
static int32_t g_19 = 0;
static uint16_t g_37 = 8U;
static uint64_t g_42 = 18446744073709551612U;
static int32_t *g_44 = (void*)0;
static int32_t *g_46 = &g_19;

static int16_t  func_1(void);
static int32_t * func_2(int32_t * p_3);
static int32_t * func_4(const uint32_t  p_5, int16_t  p_6, int8_t  p_7, uint8_t  p_8, uint32_t  p_9);

static int16_t  func_1(void)
{ 
    int16_t l_12 = 0xE1F6;
    int32_t **l_45 = &g_44;
    int32_t *l_48 = &g_19;
    int32_t **l_47 = &l_48;
    (*l_47) = (g_46 = ((*l_45) = func_2(func_4(g_10, g_10, (((safe_unary_minus_func_int32_t_s(g_10)) != (-3)) >= g_10), l_12, g_10))));
    return (**l_47);
}

static int32_t * func_2(int32_t * p_3)
{ 
    const int64_t l_31 = 0x59F89F2E072305ABLL;
    uint16_t *l_36 = &g_37;
    int32_t *l_38 = &g_19;
    uint64_t *l_41[1][3];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_41[i][j] = &g_42;
    }
    l_38 = func_4(l_31, ((safe_rshift_func_int32_t_s_s((safe_lshift_func_int8_t_s_u(l_31, 1)), (((*l_36) = g_10) , (*p_3)))) , 0x073E), g_19, l_31, l_31);
    g_44 = func_4(g_14, (((safe_add_func_int64_t_s_s((((*l_38) = (*l_38)) != (safe_unary_minus_func_uint64_t_u((g_42 != g_10)))), g_14)) | 4U) <= g_37), l_31, g_10, g_37);
    return &g_19;
}

static int32_t * func_4(const uint32_t  p_5, int16_t  p_6, int8_t  p_7, uint8_t  p_8, uint32_t  p_9)
{ 
    int32_t *l_13 = (void*)0;
    int32_t *l_15 = (void*)0;
    int32_t *l_16 = (void*)0;
    int32_t *l_17 = (void*)0;
    int32_t *l_18 = &g_19;
    int32_t *l_20 = (void*)0;
    int32_t *l_21 = &g_19;
    int32_t l_22 = (-1);
    int32_t *l_23 = &l_22;
    int32_t *l_24 = &l_22;
    int32_t *l_25 = &g_19;
    int32_t *l_26 = (void*)0;
    int32_t *l_27[4][2];
    uint32_t l_28[1];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
            l_27[i][j] = &g_19;
    }
    for (i = 0; i < 1; i++)
        l_28[i] = 1U;
    l_28[0]++;
    return &g_19;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
