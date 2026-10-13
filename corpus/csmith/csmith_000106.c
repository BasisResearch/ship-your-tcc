/* Csmith 0cdc710315cf seed 106, flattened by scripts/csmith_flatten.py */
// Options:   --seed 106 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
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

static int32_t g_13 = 0;
static int32_t *g_25 = (void*)0;
static const uint16_t g_26 = 0x62B5;
static uint32_t g_30 = 4294967288U;
static uint64_t g_35 = 0x7999919139F4802DLL;

static uint8_t  func_1(void);
static int32_t * func_2(int32_t  p_3, int64_t  p_4, int32_t * p_5);
static int32_t * func_7(int8_t  p_8, uint8_t  p_9, int32_t * p_10, int32_t * p_11, int32_t * const  p_12);

static uint8_t  func_1(void)
{ 
    uint16_t l_14 = 1U;
    int32_t * const l_15[2] = {&g_13,&g_13};
    int32_t *l_24 = &g_13;
    int32_t **l_23 = &l_24;
    uint8_t l_27 = 250U;
    int32_t *l_28 = (void*)0;
    int32_t l_37 = 0x7DA9FC5C;
    int i;
    (*l_23) = func_2(((+(((9 ^ ((((*l_23) = func_7(g_13, l_14, &g_13, &g_13, l_15[1])) != g_25) , g_13)) >= g_26) && g_26)) > g_26), l_27, l_28);
    g_30 |= (**l_23);
    if (g_30)
    { 
        uint64_t *l_33 = (void*)0;
        uint64_t *l_34 = &g_35;
        int32_t l_36 = 1;
        int32_t **l_38 = &g_25;
        (*l_38) = ((*l_23) = func_7(((0x84A08DEB256CD219LL && (safe_mul_func_uint64_t_u_u(((((((((((*l_34) ^= (*l_24)) , g_30) == 0x478D) > l_36) < 0xD7) || g_30) > 0U) || l_37) == (*l_24)), (*l_24)))) == l_36), l_36, &l_36, &g_13, &g_13));
        l_36 |= ((safe_rshift_func_int16_t_s_u(0, (1U < g_35))) <= (**l_38));
    }
    else
    { 
        int16_t l_43 = 0xE1D6;
        for (l_27 = (-13); (l_27 == 58); l_27 = safe_add_func_uint8_t_u_u(l_27, 9))
        { 
            int32_t *l_44 = &l_37;
            if (g_13)
            { 
                return l_43;
            }
            else
            { 
                (*l_23) = func_7(l_43, g_35, &g_13, l_44, &g_13);
                (*l_23) = &g_13;
            }
        }
    }
    return g_26;
}

static int32_t * func_2(int32_t  p_3, int64_t  p_4, int32_t * p_5)
{ 
    int8_t l_29 = 0x0F;
    l_29 |= 0x5B45BEAC;
    return &g_13;
}

static int32_t * func_7(int8_t  p_8, uint8_t  p_9, int32_t * p_10, int32_t * p_11, int32_t * const  p_12)
{ 
    int32_t *l_21 = &g_13;
    int32_t **l_20 = &l_21;
    const uint16_t l_22[1] = {65535U};
    int i;
    (*p_10) = ((safe_div_func_uint8_t_u_u((safe_sub_func_int32_t_s_s(((8 > (g_13 == (((*l_20) = &g_13) == (void*)0))) | 0xB1A7A81A82E77E81LL), l_22[0])), g_13)) | 0xA60016488A973314LL);
    return &g_13;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
