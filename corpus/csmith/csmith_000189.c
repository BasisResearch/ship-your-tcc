/* Csmith 0cdc710315cf seed 189, flattened by scripts/csmith_flatten.py */
// Options:   --seed 189 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint32_t  f0;
   uint64_t  f1;
   int64_t  f2;
   int8_t  f3;
};

static int32_t g_2 = (-1);
static int8_t g_12 = 0xC1;
static uint16_t g_13 = 4U;
static struct S0 g_20 = {4294967295U,18446744073709551615U,-9,8};

static uint32_t  func_1(void);
static int32_t * func_16(uint32_t  p_17);

static uint32_t  func_1(void)
{ 
    uint32_t l_5 = 3U;
    uint64_t l_6[4];
    int32_t l_9 = 5;
    int32_t l_10 = 1;
    int32_t l_11[3];
    const int64_t l_24 = 0x1C8B9BA5E0B2F67ALL;
    int64_t *l_25 = (void*)0;
    uint32_t *l_26[3];
    int32_t **l_31 = (void*)0;
    int32_t *l_32 = &l_9;
    int i;
    for (i = 0; i < 4; i++)
        l_6[i] = 0U;
    for (i = 0; i < 3; i++)
        l_11[i] = 1;
    for (i = 0; i < 3; i++)
        l_26[i] = &g_20.f0;
    for (g_2 = 24; (g_2 < 21); g_2 = safe_sub_func_uint16_t_u_u(g_2, 3))
    { 
        if (l_5)
            break;
    }
    for (l_5 = 0; (l_5 <= 3); l_5 += 1)
    { 
        int32_t *l_7[3];
        int16_t l_8[2];
        int i;
        for (i = 0; i < 3; i++)
            l_7[i] = &g_2;
        for (i = 0; i < 2; i++)
            l_8[i] = 0;
        g_13++;
    }
    l_32 = func_16((l_10 = (safe_mul_func_uint8_t_u_u((g_20 , ((safe_mod_func_int64_t_s_s((g_20.f2 = (+((-10) > l_24))), g_2)) >= (-9))), (-1)))));
    return g_20.f2;
}

static int32_t * func_16(uint32_t  p_17)
{ 
    int32_t *l_29 = (void*)0;
    uint64_t l_30[3];
    int i;
    for (i = 0; i < 3; i++)
        l_30[i] = 0x4ABF0A644184EB30LL;
    for (p_17 = 27; (p_17 <= 6); p_17 = safe_sub_func_int8_t_s_s(p_17, 1))
    { 
        return l_29;
    }
    for (g_12 = 0; (g_12 <= 2); g_12 += 1)
    { 
        return l_29;
    }
    return &g_2;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_20.f0, "g_20.f0", print_hash_value);
    transparent_crc(g_20.f1, "g_20.f1", print_hash_value);
    transparent_crc(g_20.f2, "g_20.f2", print_hash_value);
    transparent_crc(g_20.f3, "g_20.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
