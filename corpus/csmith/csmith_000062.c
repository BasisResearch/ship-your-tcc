/* Csmith 0cdc710315cf seed 62, flattened by scripts/csmith_flatten.py */
// Options:   --seed 62 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static uint64_t g_5[2][1] = {{8U},{8U}};
static uint8_t g_18 = 0U;
static int64_t g_26 = 0x8AB79AE4F78D7CBALL;
static uint32_t g_32 = 0x11A53FDE;
static uint16_t g_36 = 0xEDDA;
static int16_t g_40[3][1] = {{0},{0},{0}};

static int16_t  func_1(void);
static int32_t  func_3(int32_t  p_4);
static int8_t  func_10(int16_t  p_11);

static int16_t  func_1(void)
{ 
    int32_t l_2 = 1;
    int32_t *l_39[4] = {&l_2,&l_2,&l_2,&l_2};
    int i;
    l_2 |= 0x3AE35568;
    g_40[2][0] |= func_3(g_5[1][0]);
    return g_40[2][0];
}

static int32_t  func_3(int32_t  p_4)
{ 
    uint16_t l_16[2];
    uint8_t *l_17 = &g_18;
    int16_t *l_21[4];
    int32_t l_22 = (-7);
    int32_t l_23 = 0xAE2FE817;
    int64_t *l_24 = (void*)0;
    int64_t *l_25 = &g_26;
    uint16_t *l_35[3][2] = {{&l_16[0],&l_16[0]},{&l_16[0],&l_16[0]},{&l_16[0],&l_16[0]}};
    int32_t *l_37 = &l_22;
    int32_t l_38 = (-4);
    int i, j;
    for (i = 0; i < 2; i++)
        l_16[i] = 0x1B58;
    for (i = 0; i < 4; i++)
        l_21[i] = (void*)0;
    l_38 |= ((*l_37) = (safe_sub_func_uint16_t_u_u((g_36 ^= ((safe_lshift_func_int8_t_s_u(func_10(((((((*l_25) = ((safe_div_func_int64_t_s_s((safe_add_func_uint8_t_u_u((l_23 = ((((*l_17) = l_16[0]) , (l_22 = (safe_add_func_uint16_t_u_u(l_16[0], p_4)))) <= p_4)), g_5[1][0])), l_16[0])) <= 1)) != p_4) ^ p_4) & l_16[0]) != l_16[0])), 4)) == 0x2138)), 0U)));
    return g_18;
}

static int8_t  func_10(int16_t  p_11)
{ 
    uint32_t *l_31 = &g_32;
    int32_t l_33 = 0x9EB818F8;
    int32_t *l_34 = &l_33;
    (*l_34) = ((safe_lshift_func_int8_t_s_s(((-8) && (safe_lshift_func_int16_t_s_s((((*l_31) ^= g_5[1][0]) & l_33), p_11))), 4)) || p_11);
    return p_11;
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
            transparent_crc(g_5[i][j], "g_5[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_36, "g_36", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_40[i][j], "g_40[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
