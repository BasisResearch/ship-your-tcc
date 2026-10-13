/* Csmith 0cdc710315cf seed 289, flattened by scripts/csmith_flatten.py */
// Options:   --seed 289 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int8_t  f0;
   uint32_t  f1;
};

static int32_t g_3 = 0x69F4288A;
static uint16_t g_9 = 0x3356;
static const int32_t g_24 = (-1);
static struct S0 g_25 = {0,0xEDD1EDB6};
static struct S0 g_27 = {0x60,0xC23CEA2F};
static int32_t g_53 = 0xA5481F04;

static int64_t  func_1(void);
static int32_t * func_12(int16_t  p_13, uint8_t  p_14, struct S0  p_15);
static uint16_t  func_16(const int32_t * p_17, struct S0  p_18, struct S0  p_19, uint64_t  p_20, uint32_t  p_21);

static int64_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t *l_4 = (void*)0;
    int32_t *l_5 = &g_3;
    int32_t *l_6 = &g_3;
    int32_t *l_7 = (void*)0;
    int32_t *l_8[2][4] = {{(void*)0,&g_3,(void*)0,(void*)0},{&g_3,&g_3,&g_3,&g_3}};
    const int32_t *l_23 = &g_24;
    const int32_t **l_22 = &l_23;
    struct S0 *l_26 = &g_25;
    uint16_t l_54 = 0xF7C9;
    int i, j;
    --g_9;
    (*l_22) = func_12((func_16(((*l_22) = (void*)0), ((*l_26) = g_25), g_27, (*l_5), g_9) | g_27.f1), g_27.f0, g_27);
    l_54--;
    return (*l_6);
}

static int32_t * func_12(int16_t  p_13, uint8_t  p_14, struct S0  p_15)
{ 
    return &g_53;
}

static uint16_t  func_16(const int32_t * p_17, struct S0  p_18, struct S0  p_19, uint64_t  p_20, uint32_t  p_21)
{ 
    int8_t l_42[1];
    uint8_t l_43 = 255U;
    const int32_t *l_51 = &g_3;
    int i;
    for (i = 0; i < 1; i++)
        l_42[i] = 0x96;
    for (g_9 = 17; (g_9 <= 40); g_9++)
    { 
        uint32_t l_38 = 4294967295U;
        int32_t *l_44 = &g_3;
        g_3 = ((safe_add_func_uint8_t_u_u((safe_mul_func_int16_t_s_s((-1), (p_18.f0 & (g_25.f0 <= 0x323FA9DFEEE83ADELL)))), 0x04)) , 0x77A75E3A);
        for (p_20 = 9; (p_20 >= 16); p_20 = safe_add_func_int8_t_s_s(p_20, 5))
        { 
            for (g_25.f1 = 0; (g_25.f1 <= 21); ++g_25.f1)
            { 
                l_38 &= g_27.f1;
            }
        }
        (*l_44) = ((~(((safe_lshift_func_int64_t_s_s((-10), g_25.f0)) > l_42[0]) || p_18.f1)) <= l_43);
    }
    for (g_9 = (-6); (g_9 != 20); g_9++)
    { 
        int32_t *l_47 = &g_3;
        int32_t *l_52 = &g_53;
        for (l_43 = 0; (l_43 <= 0); l_43 += 1)
        { 
            int32_t **l_48 = &l_47;
            (*l_48) = l_47;
        }
        (*l_52) = ((*l_47) = (safe_lshift_func_uint64_t_u_u((g_25 , (((void*)0 != l_51) ^ (*l_47))), 18)));
    }
    return p_18.f0;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_25.f0, "g_25.f0", print_hash_value);
    transparent_crc(g_25.f1, "g_25.f1", print_hash_value);
    transparent_crc(g_27.f0, "g_27.f0", print_hash_value);
    transparent_crc(g_27.f1, "g_27.f1", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
