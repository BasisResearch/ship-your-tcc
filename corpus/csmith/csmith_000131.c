/* Csmith 0cdc710315cf seed 131, flattened by scripts/csmith_flatten.py */
// Options:   --seed 131 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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

union U0 {
   int32_t  f0;
   int16_t  f1;
   uint32_t  f2;
};

static int32_t g_3 = 0;
static uint16_t g_12 = 0U;
static uint8_t g_15 = 255U;
static uint64_t g_17 = 0x16D21B1857497F3CLL;
static int32_t g_24 = 0;
static int32_t *g_23 = &g_24;
static union U0 g_32 = {6};
static int32_t *g_34 = &g_3;
static int32_t **g_33 = &g_34;
static uint32_t g_43 = 0xA4911CCF;
static int8_t g_50 = 0xC6;
static uint64_t g_52 = 0x8E04799C819B53B3LL;
static uint64_t g_59 = 1U;
static const uint16_t * const g_77 = &g_12;
static uint32_t g_95 = 0x3165B027;
static int64_t g_104 = 0;

static uint64_t  func_1(void);
static int32_t * func_25(int32_t * p_26, union U0  p_27, int16_t  p_28, uint16_t  p_29, int32_t ** p_30);
static int32_t * func_36(union U0  p_37, uint16_t  p_38);

static uint64_t  func_1(void)
{ 
    int16_t l_2[3][3] = {{0xF1DB,(-6),0xF1DB},{0xF1DB,(-6),0xF1DB},{0xF1DB,(-6),0xF1DB}};
    int32_t *l_31 = &g_3;
    int32_t l_51 = 4;
    uint16_t *l_75 = &g_12;
    uint64_t l_78 = 18446744073709551615U;
    int i, j;
    for (g_3 = 2; (g_3 >= 0); g_3 -= 1)
    { 
        uint16_t *l_11[4][1];
        int32_t l_13 = 0xEAB1D6A5;
        int32_t *l_14[2][1];
        int16_t l_92 = 1;
        const int64_t l_102 = 0xCA0061E2E1E1BC7BLL;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_11[i][j] = &g_12;
        }
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_14[i][j] = (void*)0;
        }
    }
    return (*l_31);
}

static int32_t * func_25(int32_t * p_26, union U0  p_27, int16_t  p_28, uint16_t  p_29, int32_t ** p_30)
{ 
    int64_t l_35 = 0x1D66BD5D51814487LL;
    union U0 l_39 = {4};
    int32_t *l_47 = &g_3;
    int32_t **l_46 = &l_47;
lbl_48:
    l_35 = (**p_30);
    (*l_46) = ((*p_30) = func_36(l_39, p_27.f0));
    if (g_43)
        goto lbl_48;
    return (*p_30);
}

static int32_t * func_36(union U0  p_37, uint16_t  p_38)
{ 
    int32_t *l_40 = &g_32.f0;
    int32_t *l_41 = &g_32.f0;
    int32_t *l_42[3][4] = {{&g_24,&g_24,&g_32.f0,(void*)0},{&g_24,&g_32.f0,&g_24,&g_32.f0},{&g_24,&g_32.f0,&g_32.f0,&g_24}};
    int i, j;
    g_43++;
    g_33 = &g_34;
    return (*g_33);
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_32.f0, "g_32.f0", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_95, "g_95", print_hash_value);
    transparent_crc(g_104, "g_104", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
