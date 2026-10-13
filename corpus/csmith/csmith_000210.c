/* Csmith 0cdc710315cf seed 210, flattened by scripts/csmith_flatten.py */
// Options:   --seed 210 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
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
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S2 {
   int32_t  f0;
};

struct S0 {
   int16_t  f0;
   int32_t  f1;
};

struct S3 {
   struct S0  f0;
   struct S0  f1;
   const uint64_t  f2;
   int16_t  f3;
};

union U4 {
   struct S3  f0;
   const struct S3  f1;
};

static int32_t g_2 = 0x2A727599;
static struct S2 g_36 = {0xE2E31B54};
static const int32_t *g_52 = &g_2;
static const int32_t **g_51 = &g_52;
static int32_t g_57 = 0x8EE08A9C;
static uint8_t g_61[4] = {0U,0U,0U,0U};
static uint16_t g_64 = 65534U;
static union U4 g_67 = {{{0x0D70,1},{0xDC53,-9},18446744073709551615U,0x1CE1}};

static union U4  func_1(void);
static uint16_t  func_8(int64_t  p_9, int16_t  p_10, int64_t  p_11, struct S2  p_12, int32_t  p_13);
static int64_t  func_14(uint8_t  p_15, struct S3  p_16, int64_t  p_17, int64_t  p_18);

static union U4  func_1(void)
{ 
    uint8_t l_20 = 0x11;
    struct S2 l_46 = {1};
    int32_t *l_58 = &g_57;
    int32_t *l_59 = &g_2;
    int32_t *l_60[3];
    int i;
    for (i = 0; i < 3; i++)
        l_60[i] = &g_2;
    for (g_2 = 0; (g_2 < (-13)); g_2--)
    { 
        int64_t l_7[1][1];
        struct S3 l_19 = {{0,2},{5,0x2C593C86},0x92F1E5C8A52B4B5ALL,1};
        int8_t l_21 = 0x54;
        int32_t *l_56[3][4] = {{&g_57,&g_57,(void*)0,&g_57},{&g_57,&l_19.f0.f1,&l_19.f0.f1,&g_57},{&l_19.f0.f1,&g_57,&l_19.f0.f1,&l_19.f0.f1}};
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_7[i][j] = 5;
        }
    }
    ++g_61[3];
    --g_64;
    return g_67;
}

static uint16_t  func_8(int64_t  p_9, int16_t  p_10, int64_t  p_11, struct S2  p_12, int32_t  p_13)
{ 
    uint8_t l_54[4][4] = {{252U,252U,252U,252U},{252U,252U,252U,252U},{252U,252U,252U,252U},{252U,252U,252U,252U}};
    int i, j;
    for (p_12.f0 = 19; (p_12.f0 <= (-24)); p_12.f0--)
    { 
        int32_t *l_53 = &g_36.f0;
        for (p_9 = (-8); (p_9 < (-19)); --p_9)
        { 
            g_51 = (void*)0;
            return g_36.f0;
        }
        (*g_51) = l_53;
    }
    for (p_9 = 3; (p_9 >= 0); p_9 -= 1)
    { 
        uint16_t l_55 = 0x6996;
        (*g_51) = (*g_51);
        return l_55;
    }
    return l_54[2][3];
}

static int64_t  func_14(uint8_t  p_15, struct S3  p_16, int64_t  p_17, int64_t  p_18)
{ 
    int32_t *l_32 = (void*)0;
    int32_t *l_33[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
    uint8_t l_34 = 0U;
    struct S2 * const l_35 = &g_36;
    struct S2 *l_38 = &g_36;
    struct S2 **l_37 = &l_38;
    int32_t **l_45 = &l_32;
    int i;
    l_34 ^= (safe_sub_func_int8_t_s_s(p_16.f1.f1, (safe_mul_func_int16_t_s_s(0xA9A9, (((safe_rshift_func_uint32_t_u_u((safe_div_func_uint64_t_u_u((safe_div_func_uint8_t_u_u((p_16.f0.f1 > 18446744073709551614U), p_16.f3)), p_16.f3)), p_16.f1.f1)) >= (-3)) & p_16.f1.f1)))));
    (*l_37) = l_35;
    p_16.f0.f1 ^= ((safe_lshift_func_uint8_t_u_u((((safe_mod_func_uint8_t_u_u((safe_mul_func_int8_t_s_s((&g_36 != &g_36), (((*l_45) = l_33[2]) == l_33[2]))), 9)) <= g_36.f0) | g_36.f0), 5)) <= p_15);
    return g_36.f0;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_36.f0, "g_36.f0", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_61[i], "g_61[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_67.f0.f0.f0, "g_67.f0.f0.f0", print_hash_value);
    transparent_crc(g_67.f0.f0.f1, "g_67.f0.f0.f1", print_hash_value);
    transparent_crc(g_67.f0.f1.f0, "g_67.f0.f1.f0", print_hash_value);
    transparent_crc(g_67.f0.f1.f1, "g_67.f0.f1.f1", print_hash_value);
    transparent_crc(g_67.f0.f2, "g_67.f0.f2", print_hash_value);
    transparent_crc(g_67.f0.f3, "g_67.f0.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
