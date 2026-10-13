/* Csmith 0cdc710315cf seed 146, flattened by scripts/csmith_flatten.py */
// Options:   --seed 146 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
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
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int32_t  f0;
   const int16_t  f1;
};

struct S1 {
   uint8_t  f0;
   struct S0  f1;
};

union U2 {
   struct S1  f0;
   int64_t  f1;
   uint8_t  f2;
};

union U3 {
   const int16_t  f0;
   uint8_t  f1;
};

static int32_t g_2[1] = {0x5B3B9DA4};
static int32_t g_19 = 1;
static struct S0 g_21 = {0xE5C8A52B,0xB5A9};
static int32_t g_27 = 0x5A60C437;
static uint8_t g_28 = 1U;

static int16_t  func_1(void);
static int32_t  func_5(uint64_t  p_6, int16_t  p_7, struct S0  p_8);
static const union U3  func_12(int8_t  p_13, uint8_t  p_14, union U2  p_15);

static int16_t  func_1(void)
{ 
    int64_t l_11 = 1;
    int64_t l_45 = 0xD7279364560F9103LL;
    for (g_2[0] = 0; (g_2[0] <= (-15)); g_2[0] = safe_sub_func_int8_t_s_s(g_2[0], 8))
    { 
        uint64_t l_16 = 0U;
        union U2 l_17 = {{0U,{7,0}}};
        int32_t *l_40 = &g_27;
        (*l_40) = (((func_5((safe_mul_func_int16_t_s_s(g_2[0], ((l_11 , func_12((l_16 != 0xF8), g_2[0], l_17)) , l_11))), g_2[0], g_21) <= 4294967295U) > 1) ^ 4294967295U);
    }
    for (g_28 = 3; (g_28 > 49); ++g_28)
    { 
        for (l_11 = 0; (l_11 <= 22); l_11 = safe_add_func_int8_t_s_s(l_11, 2))
        { 
            if (l_45)
                break;
        }
    }
    return l_45;
}

static int32_t  func_5(uint64_t  p_6, int16_t  p_7, struct S0  p_8)
{ 
    int32_t *l_22 = &g_21.f0;
    int32_t *l_23 = (void*)0;
    int32_t *l_24 = &g_19;
    int32_t *l_25[1];
    int32_t l_26 = 7;
    int i;
    for (i = 0; i < 1; i++)
        l_25[i] = &g_19;
    g_28--;
    for (p_8.f0 = 0; (p_8.f0 <= 0); p_8.f0 += 1)
    { 
        int i;
        if (g_2[0])
        { 
            int i;
            for (l_26 = 0; (l_26 <= 0); l_26 += 1)
            { 
                return p_8.f1;
            }
            (*l_24) = (safe_sub_func_uint32_t_u_u((((0xC702B5BD == ((safe_rshift_func_int16_t_s_u(((((l_25[p_8.f0] = &g_27) == &g_19) && 0xE31B5465) ^ g_2[0]), 11)) | 0xA83A56CF6D70783DLL)) || 0x1EB1ED309061E942LL) , g_27), p_7));
        }
        else
        { 
            (*l_22) &= (safe_lshift_func_int64_t_s_s(((safe_unary_minus_func_int16_t_s((p_8.f1 != 0x2A55))) < (safe_mul_func_int64_t_s_s(g_19, 0x987F9BD85DD727ABLL))), p_6));
            if (p_6)
                break;
        }
    }
    return g_2[0];
}

static const union U3  func_12(int8_t  p_13, uint8_t  p_14, union U2  p_15)
{ 
    int32_t *l_18[1][3];
    const union U3 l_20 = {0xEC62};
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_18[i][j] = &g_19;
    }
    g_19 |= 0;
    return l_20;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_21.f0, "g_21.f0", print_hash_value);
    transparent_crc(g_21.f1, "g_21.f1", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
