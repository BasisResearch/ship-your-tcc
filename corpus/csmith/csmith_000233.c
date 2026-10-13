/* Csmith 0cdc710315cf seed 233, flattened by scripts/csmith_flatten.py */
// Options:   --seed 233 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int8_t
safe_unary_minus_func_int8_t_s(int8_t si )
{
  return
    -si;
}
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static uint64_t g_7 = 0x6C74AA0D72276B87LL;
static int32_t g_9 = 1;
static int32_t g_11 = 0xCE4A6F44;
static int64_t g_26 = (-1);
static int32_t *g_36 = &g_9;
static uint8_t g_48 = 0xFB;
static uint16_t g_57 = 0x1B3D;

static int8_t  func_1(void);
static uint64_t  func_18(int32_t * p_19, int64_t  p_20);
static int32_t * func_21(int32_t  p_22, int64_t  p_23, int32_t  p_24);

static int8_t  func_1(void)
{ 
    uint16_t l_6[2];
    int32_t *l_8 = &g_9;
    int32_t *l_10[1];
    int64_t *l_25 = &g_26;
    int i;
    for (i = 0; i < 2; i++)
        l_6[i] = 0x6DEE;
    for (i = 0; i < 1; i++)
        l_10[i] = &g_11;
    g_11 ^= ((*l_8) = ((-1) ^ ((safe_sub_func_int16_t_s_s((safe_mul_func_uint8_t_u_u((((l_6[1] , g_7) , g_7) >= g_7), 0xFC)), 0x5BF2)) , l_6[1])));
    (*l_8) = (safe_mod_func_uint16_t_u_u(((safe_div_func_int16_t_s_s(((safe_unary_minus_func_uint8_t_u((~func_18(func_21(g_7, ((*l_25) = 0x780EEC299E885DC1LL), ((safe_mod_func_int64_t_s_s((safe_rshift_func_int32_t_s_u(((safe_mod_func_int64_t_s_s(((safe_rshift_func_int64_t_s_u((g_11 > (*l_8)), 40)) != 0x7C7128ABF8B243E5LL), g_9)) , g_9), 31)), 18446744073709551615U)) ^ g_7)), (*l_8))))) , 0x7269), g_7)) , g_9), g_7));
    return g_11;
}

static uint64_t  func_18(int32_t * p_19, int64_t  p_20)
{ 
    int8_t l_38[1];
    int32_t l_42 = (-7);
    const int8_t l_46 = 0x6A;
    int16_t l_52 = 4;
    int32_t l_61 = 0x84A8A623;
    int i;
    for (i = 0; i < 1; i++)
        l_38[i] = 1;
    for (g_9 = 0; (g_9 <= 0); g_9 += 1)
    { 
        int32_t l_49 = 0x40931BC1;
        int i;
        if ((safe_lshift_func_uint8_t_u_s(l_38[g_9], (l_42 = ((((safe_unary_minus_func_int8_t_s((5U | l_38[g_9]))) != l_38[g_9]) | l_38[0]) || l_38[g_9])))))
        { 
            uint32_t l_45 = 1U;
            int64_t *l_53 = (void*)0;
            for (p_20 = 0; (p_20 <= 0); p_20 += 1)
            { 
                uint8_t *l_47 = &g_48;
                l_49 = ((l_42 <= 4U) & (safe_lshift_func_uint8_t_u_s(((*l_47) = (((l_38[g_9] , (*p_19)) , l_45) > l_46)), 6)));
                (*p_19) = (safe_sub_func_int16_t_s_s(l_52, (g_11 ^ (((void*)0 != l_53) && p_20))));
            }
        }
        else
        { 
            for (g_11 = (-30); (g_11 <= 8); g_11 = safe_add_func_uint64_t_u_u(g_11, 5))
            { 
                uint16_t *l_56 = &g_57;
                int32_t *l_60 = &l_42;
                (*l_60) = (g_48 && ((*l_56)--));
            }
        }
    }
    return l_61;
}

static int32_t * func_21(int32_t  p_22, int64_t  p_23, int32_t  p_24)
{ 
    int32_t **l_35[1];
    int32_t *l_37 = &g_11;
    int i;
    for (i = 0; i < 1; i++)
        l_35[i] = (void*)0;
    g_36 = &p_22;
    g_36 = (void*)0;
    g_36 = &p_22;
    return l_37;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_11, "g_11", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
