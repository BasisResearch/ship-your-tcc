/* Csmith 0cdc710315cf seed 239, flattened by scripts/csmith_flatten.py */
// Options:   --seed 239 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
/* ---- generated program ---- */
static long __undefined;

union U1 {
   int16_t  f0;
   uint16_t  f1;
};

static int8_t g_9 = 0xF4;
static int32_t g_13 = 3;
static int32_t g_15 = 0xEFFB91E6;
static uint32_t g_29[4][1] = {{0x29AC92D1},{0x2E7C650B},{0x29AC92D1},{0x2E7C650B}};
static int64_t g_32 = 0xFE868E0133D4965DLL;
static int8_t g_33 = 5;
static uint64_t g_34 = 18446744073709551609U;

static int64_t  func_1(void);

static int64_t  func_1(void)
{ 
    union U1 l_2 = {0xD7B1};
    int8_t l_10 = (-2);
    int8_t *l_11 = &g_9;
    int32_t *l_12 = &g_13;
    int32_t *l_14 = &g_15;
    int32_t l_16 = 0x4C9D8D92;
    int32_t *l_17 = &g_15;
    int32_t *l_18 = &g_15;
    int32_t *l_19 = &l_16;
    int32_t *l_20 = &l_16;
    int32_t *l_21 = &g_15;
    int32_t *l_22 = (void*)0;
    int32_t *l_23 = &g_15;
    int32_t *l_24 = &g_15;
    int32_t *l_25 = (void*)0;
    int32_t *l_26 = (void*)0;
    int32_t *l_27[2][3] = {{&l_16,&l_16,&l_16},{&l_16,&l_16,&l_16}};
    int64_t l_28 = 1;
    int i, j;
    (*l_14) ^= ((*l_12) ^= (l_2 , ((safe_div_func_uint32_t_u_u((0xDFCF ^ ((((safe_add_func_uint16_t_u_u((((*l_11) = (safe_mod_func_uint64_t_u_u((((1U && 0x695998AB5457D670LL) || g_9) || l_10), g_9))) ^ l_2.f0), l_10)) && g_9) >= 0x55) <= l_10)), 3U)) || l_10)));
    ++g_29[2][0];
    g_34++;
    return g_34;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_29[i][j], "g_29[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_34, "g_34", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
