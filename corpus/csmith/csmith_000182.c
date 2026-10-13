/* Csmith 0cdc710315cf seed 182, flattened by scripts/csmith_flatten.py */
// Options:   --seed 182 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

static uint32_t g_6 = 0x75C19C5A;
static int16_t g_7 = 0x4672;
static int64_t g_8 = (-5);
static int32_t g_10 = 0x449C9FB3;
static int64_t g_13 = 0xD8D383303773672DLL;
static int16_t g_14 = 0xB647;
static int32_t g_15 = (-1);
static int8_t g_16 = 0xDF;
static uint64_t g_17 = 0x6C0B333EE6768459LL;
static uint16_t g_29[3] = {65535U,65535U,65535U};
static int16_t g_37 = (-1);
static int8_t g_40 = 2;

static uint16_t  func_1(void);
static int8_t  func_2(uint64_t  p_3);
static uint64_t  func_4(int16_t  p_5);

static uint16_t  func_1(void)
{ 
    int8_t *l_39 = &g_40;
    int32_t *l_41[3][1];
    uint16_t l_42[1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_41[i][j] = &g_15;
    }
    for (i = 0; i < 1; i++)
        l_42[i] = 0x311E;
    l_42[0] = (((*l_39) &= func_2(func_4(g_6))) , g_40);
    return g_17;
}

static int8_t  func_2(uint64_t  p_3)
{ 
    const uint16_t l_36 = 3U;
    for (g_16 = 2; (g_16 >= 0); g_16 -= 1)
    { 
        int64_t l_38 = (-3);
        for (g_10 = 2; (g_10 >= 0); g_10 -= 1)
        { 
            int i;
            g_15 &= (safe_mul_func_int16_t_s_s((-4), (((safe_add_func_int64_t_s_s(((g_37 ^= ((((safe_div_func_uint8_t_u_u(g_29[g_10], (p_3 || g_17))) < 254U) >= l_36) < g_7)) == l_38), 1)) == 3) >= p_3)));
        }
    }
    return g_15;
}

static uint64_t  func_4(int16_t  p_5)
{ 
    int32_t *l_9 = &g_10;
    int32_t *l_11 = &g_10;
    int32_t *l_12[2];
    int i;
    for (i = 0; i < 2; i++)
        l_12[i] = (void*)0;
    --g_17;
    for (g_14 = 0; (g_14 != 18); g_14 = safe_add_func_uint32_t_u_u(g_14, 9))
    { 
        for (g_7 = 0; (g_7 < 0); g_7 = safe_add_func_int32_t_s_s(g_7, 6))
        { 
            uint32_t l_28 = 4294967295U;
            g_10 &= (safe_lshift_func_uint8_t_u_s((safe_sub_func_int16_t_s_s((0xC0A9C5A7 && (((l_9 == l_12[0]) == p_5) , g_8)), p_5)), 5));
            return l_28;
        }
    }
    return g_14;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_29[i], "g_29[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
