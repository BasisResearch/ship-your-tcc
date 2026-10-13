/* Csmith 0cdc710315cf seed 276, flattened by scripts/csmith_flatten.py */
// Options:   --seed 276 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S1 {
   uint8_t  f0;
};

struct S2 {
   uint32_t  f0;
};

static const int64_t g_6 = 0;
static struct S2 g_13 = {0x844A7543};
static int32_t g_18 = (-8);
static uint16_t g_29 = 0x97E7;
static struct S1 g_40[1] = {{2U}};
static int8_t g_42 = 0x3D;
static int32_t g_45 = 9;
static struct S1 g_46 = {0x90};

static struct S1  func_1(void);
static struct S1  func_9(struct S2  p_10, uint16_t  p_11, int32_t  p_12);
static int32_t  func_19(const int32_t * p_20, int32_t * p_21);

static struct S1  func_1(void)
{ 
    uint32_t l_14 = 4294967292U;
    int8_t *l_41 = &g_42;
    int32_t *l_43 = (void*)0;
    int32_t *l_44[2];
    int i;
    for (i = 0; i < 2; i++)
        l_44[i] = &g_18;
    g_45 = (safe_lshift_func_int64_t_s_s(((safe_sub_func_uint8_t_u_u(g_6, ((*l_41) = (safe_sub_func_uint32_t_u_u((func_9(g_13, l_14, l_14) , 0x52601C8D), l_14))))) > g_6), l_14));
    return g_46;
}

static struct S1  func_9(struct S2  p_10, uint16_t  p_11, int32_t  p_12)
{ 
    int16_t l_33[2];
    int i;
    for (i = 0; i < 2; i++)
        l_33[i] = (-1);
    for (g_13.f0 = 0; (g_13.f0 == 56); g_13.f0 = safe_add_func_uint16_t_u_u(g_13.f0, 3))
    { 
        int32_t *l_17 = &g_18;
        int32_t **l_22 = (void*)0;
        int32_t *l_24 = &g_18;
        int32_t **l_23 = &l_24;
        (*l_17) = (1U || 1);
        l_33[1] &= func_19(l_17, ((*l_23) = (p_12 , (void*)0)));
        (*l_17) |= ((safe_mul_func_int16_t_s_s(0xFE69, p_10.f0)) > ((safe_rshift_func_uint8_t_u_u((((((safe_mod_func_int16_t_s_s(0xAC1E, g_13.f0)) > g_13.f0) <= l_33[1]) , p_12) == g_13.f0), 4)) , 0x6D7D));
    }
    return g_40[0];
}

static int32_t  func_19(const int32_t * p_20, int32_t * p_21)
{ 
    uint64_t l_25 = 1U;
    int32_t *l_26 = &g_18;
    int32_t *l_27 = &g_18;
    int32_t l_28 = 0xEE491BA6;
    int32_t l_32 = 0xB1DAB551;
    (*l_26) ^= l_25;
    g_29--;
    return l_32;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_13.f0, "g_13.f0", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_40[i].f0, "g_40[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_46.f0, "g_46.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
