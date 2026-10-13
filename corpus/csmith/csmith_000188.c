/* Csmith 0cdc710315cf seed 188, flattened by scripts/csmith_flatten.py */
// Options:   --seed 188 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 + si2);
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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

static int32_t g_2 = (-1);
static int32_t g_7 = 0x114768C0;
static int32_t *g_17 = &g_2;
static int32_t **g_22[3][4] = {{&g_17,&g_17,&g_17,&g_17},{&g_17,&g_17,&g_17,&g_17},{&g_17,&g_17,&g_17,&g_17}};
static uint32_t g_25 = 0x3847D228;

static uint16_t  func_1(void);
static int32_t  func_8(int32_t * p_9, int32_t * p_10, uint16_t  p_11, int32_t * p_12);
static uint64_t  func_13(int8_t  p_14);

static uint16_t  func_1(void)
{ 
    int16_t l_5 = (-1);
    int32_t *l_16 = &g_2;
    int32_t **l_15 = &l_16;
    int32_t *l_28 = (void*)0;
    int32_t *l_37 = &g_7;
    for (g_2 = 26; (g_2 != 7); g_2--)
    { 
        int32_t *l_6 = &g_7;
        (*l_6) = l_5;
    }
    (*l_37) = func_8(((func_13((((*l_15) = &g_2) == &g_2)) , (**l_15)) , (*l_15)), &g_2, g_2, l_28);
    return g_7;
}

static int32_t  func_8(int32_t * p_9, int32_t * p_10, uint16_t  p_11, int32_t * p_12)
{ 
    const uint8_t l_33 = 1U;
    int32_t l_35[4][3] = {{0x8D8131DD,(-5),0x8D8131DD},{0x8D8131DD,(-5),0x8D8131DD},{0x8D8131DD,(-5),0x8D8131DD},{0x8D8131DD,(-5),0x8D8131DD}};
    uint32_t l_36[1];
    int i, j;
    for (i = 0; i < 1; i++)
        l_36[i] = 0x58E7EA1B;
    for (g_25 = (-1); (g_25 > 27); g_25 = safe_add_func_int16_t_s_s(g_25, 6))
    { 
        if ((*p_10))
            break;
    }
    l_35[2][2] = (safe_div_func_uint32_t_u_u(l_33, (+l_33)));
    return l_36[0];
}

static uint64_t  func_13(int8_t  p_14)
{ 
    int32_t **l_23 = &g_17;
    uint32_t *l_24 = &g_25;
    int32_t l_26 = 0xDB2B058E;
    int32_t *l_27 = &g_7;
    g_17 = (void*)0;
    (*l_27) = (((*l_24) |= ((safe_mod_func_uint64_t_u_u((247U & (safe_lshift_func_int16_t_s_s(((l_23 = (g_22[1][2] = &g_17)) != &g_17), 4))), p_14)) ^ 0x71)) <= l_26);
    return p_14;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
