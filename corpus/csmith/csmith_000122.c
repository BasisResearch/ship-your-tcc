/* Csmith 0cdc710315cf seed 122, flattened by scripts/csmith_flatten.py */
// Options:   --seed 122 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 0x41B3C692;
static int32_t g_26 = 0;
static int32_t *g_29 = &g_26;
static int32_t *g_32[2][2] = {{&g_2,&g_2},{&g_2,&g_2}};

static int32_t  func_1(void);
static int32_t * func_5(int32_t  p_6, uint8_t  p_7, int32_t * p_8, int32_t * const  p_9);
static int32_t * func_19(int64_t  p_20, const uint32_t  p_21);

static int32_t  func_1(void)
{ 
    uint32_t l_22 = 6U;
    int32_t *l_31 = &g_26;
    int32_t l_33 = (-1);
    int32_t l_34 = 0x80563ADE;
    int32_t *l_35[3][1];
    int32_t *l_36 = &l_33;
    int16_t l_37 = 0xBF9B;
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_35[i][j] = (void*)0;
    }
    for (g_2 = 8; (g_2 == (-28)); g_2 = safe_sub_func_uint8_t_u_u(g_2, 4))
    { 
        int32_t **l_30[1];
        int i;
        for (i = 0; i < 1; i++)
            l_30[i] = &g_29;
        l_36 = func_5((g_2 , (safe_lshift_func_uint64_t_u_u((safe_div_func_int32_t_s_s(g_2, (l_34 = (safe_div_func_int32_t_s_s((((~(((safe_add_func_int16_t_s_s(((((l_33 = ((((((l_31 = func_19((l_22 <= g_2), g_2)) == (void*)0) | g_2) , g_32[1][0]) != &g_2) , (*l_31))) != 1U) | 0xCE) <= l_22), 1)) , (void*)0) == &g_2)) > l_22) , (*l_31)), 5U))))), 37))), l_22, l_35[1][0], &g_2);
    }
    l_37 |= (*g_29);
    (*l_36) = (safe_mod_func_int8_t_s_s((g_26 >= (safe_div_func_int16_t_s_s(((void*)0 != &l_34), (*l_36)))), (*l_36)));
    return (*g_29);
}

static int32_t * func_5(int32_t  p_6, uint8_t  p_7, int32_t * p_8, int32_t * const  p_9)
{ 
    return &g_26;
}

static int32_t * func_19(int64_t  p_20, const uint32_t  p_21)
{ 
    int32_t *l_27[4] = {&g_26,&g_26,&g_26,&g_26};
    int32_t **l_28[1];
    int i;
    for (i = 0; i < 1; i++)
        l_28[i] = &l_27[3];
    for (p_20 = 12; (p_20 >= 16); p_20 = safe_add_func_int32_t_s_s(p_20, 1))
    { 
        int32_t *l_25 = &g_26;
        (*l_25) &= p_20;
    }
    g_29 = l_27[3];
    return &g_26;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
