/* Csmith 0cdc710315cf seed 298, flattened by scripts/csmith_flatten.py */
// Options:   --seed 298 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = 1;
static int32_t g_4 = 5;
static int32_t g_6 = 0x1DA622A1;
static int16_t g_9 = (-2);
static uint64_t g_10 = 0U;
static const int32_t **g_13 = (void*)0;
static int8_t g_15 = (-8);
static int8_t g_22 = 0x4F;
static int32_t g_23 = (-1);
static uint16_t g_24 = 9U;
static uint8_t g_30 = 1U;

static int16_t  func_1(void);

static int16_t  func_1(void)
{ 
    uint64_t l_2[4][2];
    int32_t *l_17 = &g_4;
    int32_t *l_18 = &g_4;
    int32_t *l_19 = &g_4;
    int32_t *l_20 = &g_6;
    int32_t *l_21[2];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
            l_2[i][j] = 18446744073709551615U;
    }
    for (i = 0; i < 2; i++)
        l_21[i] = (void*)0;
    for (g_3 = 1; (g_3 >= 0); g_3 -= 1)
    { 
        int32_t *l_7 = (void*)0;
        int32_t *l_8[2][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_8[i][j] = &g_6;
        }
        for (g_4 = 0; (g_4 <= 1); g_4 += 1)
        { 
            int32_t *l_5 = &g_6;
            (*l_5) ^= 0x0CB8CE3E;
        }
        ++g_10;
        for (g_6 = 0; (g_6 <= 1); g_6 += 1)
        { 
            for (g_4 = 0; (g_4 <= 1); g_4 += 1)
            { 
                uint8_t l_14 = 0U;
                int8_t l_16 = 0xC1;
                g_13 = (void*)0;
                g_15 = l_14;
                l_16 = 0x18461E70;
            }
        }
    }
    ++g_24;
    for (g_3 = 0; (g_3 <= (-20)); g_3 = safe_sub_func_uint64_t_u_u(g_3, 6))
    { 
        uint8_t *l_29 = &g_30;
        (*l_19) = ((g_9 , g_15) != ((*l_29)++));
        (*l_19) = 0xA59E478D;
    }
    return g_6;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
