/* Csmith 0cdc710315cf seed 144, flattened by scripts/csmith_flatten.py */
// Options:   --seed 144 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint8_t  f0;
   int16_t  f1;
};

static uint8_t g_5 = 255U;
static int32_t g_8 = 0x52F76EC7;
static int32_t g_20 = 0xDD7770B1;
static int32_t *g_21 = &g_8;
static union U0 g_28 = {255U};
static uint8_t g_37 = 1U;

static const uint8_t  func_1(void);
static uint64_t  func_15(uint16_t  p_16);

static const uint8_t  func_1(void)
{ 
    const int32_t l_4 = 0x53072226;
    int16_t l_31[4] = {(-9),(-9),(-9),(-9)};
    int32_t l_32 = 0x58D0336D;
    int i;
    if ((0U > (safe_add_func_uint64_t_u_u(((l_4 >= 65532U) , l_4), g_5))))
    { 
        uint32_t l_33 = 0xC295A527;
        for (g_5 = 3; (g_5 <= 50); g_5++)
        { 
            int16_t l_11 = (-2);
            int32_t *l_12 = &g_8;
            for (g_8 = (-13); (g_8 < 8); ++g_8)
            { 
                if (l_11)
                    break;
            }
            (*l_12) &= l_4;
            if (g_8)
            { 
                int32_t **l_22 = &l_12;
                (*l_12) = (l_4 & (g_8 | (safe_mul_func_uint64_t_u_u(func_15(g_8), g_8))));
                (*l_22) = (g_21 = (void*)0);
            }
            else
            { 
                union U0 **l_23 = (void*)0;
                union U0 *l_25 = (void*)0;
                union U0 **l_24 = &l_25;
                union U0 *l_27 = &g_28;
                union U0 **l_26 = &l_27;
                (*l_26) = ((*l_24) = (void*)0);
                l_32 |= ((func_15(g_28.f0) & (((l_4 & g_5) , g_28.f0) != l_31[3])) != g_5);
                if (l_33)
                    break;
            }
        }
    }
    else
    { 
        int32_t **l_34 = &g_21;
        for (g_20 = 0; (g_20 <= 3); g_20 += 1)
        { 
            if ((*g_21))
                break;
        }
        (*l_34) = (void*)0;
    }
    for (g_28.f1 = 9; (g_28.f1 < (-27)); g_28.f1 = safe_sub_func_int16_t_s_s(g_28.f1, 5))
    { 
        const uint64_t l_38 = 18446744073709551612U;
        l_32 &= g_37;
        return l_38;
    }
    return l_32;
}

static uint64_t  func_15(uint16_t  p_16)
{ 
    for (p_16 = 23; (p_16 >= 21); p_16 = safe_sub_func_uint16_t_u_u(p_16, 4))
    { 
        int32_t *l_19 = &g_20;
        (*l_19) ^= g_8;
    }
    return p_16;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_28.f1, "g_28.f1", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
