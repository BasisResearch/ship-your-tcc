/* Csmith 0cdc710315cf seed 54, flattened by scripts/csmith_flatten.py */
// Options:   --seed 54 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
/* ---- generated program ---- */
static long __undefined;

union U1 {
   uint64_t  f0;
};

static uint32_t g_4[2][1] = {{0x603FA75C},{0x603FA75C}};
static int16_t g_16 = 0x9A22;
static int16_t g_20 = 0;
static int32_t g_26 = (-1);
static int32_t g_28 = 0x9200279E;
static int16_t *g_30 = &g_20;
static int16_t **g_29 = &g_30;
static union U1 g_32 = {0xF9C328129EC6EA3DLL};
static int32_t g_37 = 0xBA4A2C6E;

static int64_t  func_1(void);
static union U1  func_2(int8_t  p_3);
static int64_t  func_5(int32_t  p_6, int16_t  p_7, int8_t  p_8, uint16_t  p_9);

static int64_t  func_1(void)
{ 
    int16_t *l_35 = (void*)0;
    int32_t *l_36 = &g_37;
    (*l_36) ^= (func_2(g_4[0][0]) , (safe_mod_func_int32_t_s_s((l_35 != (*g_29)), 0x2BB1E5DE)));
    return g_4[0][0];
}

static union U1  func_2(int8_t  p_3)
{ 
    uint32_t l_12 = 4294967294U;
    int32_t *l_27 = &g_28;
    int16_t **l_31 = &g_30;
    (*l_27) &= (g_4[0][0] && func_5(((g_4[0][0] & (safe_mod_func_int16_t_s_s(3, 65531U))) , 3), l_12, l_12, p_3));
    (*l_27) ^= (g_29 == (l_31 = &g_30));
    return g_32;
}

static int64_t  func_5(int32_t  p_6, int16_t  p_7, int8_t  p_8, uint16_t  p_9)
{ 
    int16_t *l_19 = &g_20;
    const int32_t l_24 = 4;
    for (p_9 = 0; (p_9 < 4); ++p_9)
    { 
        int16_t *l_15[4][4];
        int16_t **l_21 = (void*)0;
        int16_t **l_22 = &l_15[3][0];
        union U1 l_23 = {0U};
        int32_t *l_25 = &g_26;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
                l_15[i][j] = &g_16;
        }
        (*l_25) = ((((g_16 ^= (-4)) | (safe_add_func_uint8_t_u_u((((*l_22) = l_19) != (l_23 , &g_20)), p_8))) == l_24) , 0x684593F6);
        return p_6;
    }
    return p_9;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_4[i][j], "g_4[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_32.f0, "g_32.f0", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
