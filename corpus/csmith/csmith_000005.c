/* Csmith 0cdc710315cf seed 5, flattened by scripts/csmith_flatten.py */
// Options:   --seed 5 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
/* ---- generated program ---- */
static long __undefined;

union U1 {
   uint16_t  f0;
   int16_t  f1;
};

static int32_t g_2 = 1;
static uint32_t g_31 = 0x0A1434A9;
static int32_t g_34 = 0;
static uint32_t g_35 = 4294967290U;
static int32_t *g_42 = (void*)0;

static uint16_t  func_1(void);
static int32_t * func_5(union U1  p_6, uint8_t  p_7, int32_t * p_8, const int32_t * p_9);
static union U1  func_10(int32_t * const  p_11, int32_t * p_12, int32_t * p_13);

static uint16_t  func_1(void)
{ 
    int32_t l_15 = 0;
    for (g_2 = 0; (g_2 > 28); ++g_2)
    { 
        int32_t * const l_14[2] = {(void*)0,(void*)0};
        int32_t *l_16 = &g_2;
        int32_t **l_41[1];
        int i;
        for (i = 0; i < 1; i++)
            l_41[i] = (void*)0;
        g_42 = func_5(func_10(l_14[1], &g_2, (l_15 , l_16)), l_15, &g_2, &g_2);
    }
    return g_2;
}

static int32_t * func_5(union U1  p_6, uint8_t  p_7, int32_t * p_8, const int32_t * p_9)
{ 
    return &g_2;
}

static union U1  func_10(int32_t * const  p_11, int32_t * p_12, int32_t * p_13)
{ 
    uint16_t l_17[4] = {0xC1D6,0xC1D6,0xC1D6,0xC1D6};
    int8_t *l_20[2];
    int32_t l_21 = 0x512A30A4;
    int32_t l_22 = 0x6C4A87FC;
    int32_t l_23 = 0x8112DEF9;
    int32_t l_24 = 1;
    int32_t *l_39 = &g_34;
    union U1 l_40 = {4U};
    int i;
    for (i = 0; i < 2; i++)
        l_20[i] = (void*)0;
lbl_38:
    l_24 &= ((l_17[2] = g_2) , (safe_div_func_int8_t_s_s(g_2, (l_23 = (l_22 = (l_21 = l_17[2]))))));
    if (g_2)
    { 
        union U1 l_25 = {0xDDAC};
        return l_25;
    }
    else
    { 
        int32_t *l_26 = &l_23;
        int32_t *l_27 = &l_22;
        int32_t *l_28 = &l_21;
        int32_t *l_29 = &l_23;
        int32_t *l_30 = &l_21;
        g_31--;
        ++g_35;
        if (g_35)
            goto lbl_38;
    }
    (*l_39) = (l_24 ^ 0x6D99);
    return l_40;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_34, "g_34", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
