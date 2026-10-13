/* Csmith 0cdc710315cf seed 196, flattened by scripts/csmith_flatten.py */
// Options:   --seed 196 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint16_t  f0;
};

static uint32_t g_5 = 0x5683C8FC;
static const union U0 g_18 = {65532U};
static int32_t g_20 = 0xEEF9A855;
static int32_t g_25 = 0x41022496;
static int32_t *g_30 = &g_25;
static int32_t * const *g_29 = &g_30;
static int32_t * const g_32 = &g_20;
static union U0 g_35 = {65527U};
static uint8_t g_42 = 255U;
static uint32_t g_47 = 0xD709C591;
static uint16_t g_51 = 3U;

static int64_t  func_1(void);
static int32_t  func_2(int64_t  p_3, uint8_t  p_4);
static int64_t  func_8(union U0  p_9, uint8_t  p_10, int32_t  p_11, union U0 * const  p_12, int64_t  p_13);

static int64_t  func_1(void)
{ 
    uint64_t l_6 = 18446744073709551614U;
    const int32_t *l_27[1];
    int32_t * const *l_31[2];
    int i;
    for (i = 0; i < 1; i++)
        l_27[i] = &g_25;
    for (i = 0; i < 2; i++)
        l_31[i] = (void*)0;
    if (func_2(g_5, l_6))
    { 
        int32_t *l_23 = (void*)0;
        int32_t *l_24 = &g_25;
        (*l_24) ^= g_20;
    }
    else
    { 
        int8_t l_26[4];
        union U0 *l_34[1];
        const int32_t **l_36 = &l_27[0];
        int i;
        for (i = 0; i < 4; i++)
            l_26[i] = 0;
        for (i = 0; i < 1; i++)
            l_34[i] = &g_35;
        for (g_20 = 3; (g_20 >= 0); g_20 -= 1)
        { 
            const int32_t **l_28 = &l_27[0];
            int32_t **l_33 = &g_30;
            (*l_28) = l_27[0];
            l_31[1] = g_29;
            (*l_33) = g_32;
        }
        (*l_36) = ((((((&g_18 == l_34[0]) > g_35.f0) , l_26[1]) >= 0x13633E7A) != g_20) , (void*)0);
    }
    for (g_35.f0 = 0; (g_35.f0 <= 1); g_35.f0 += 1)
    { 
        const int8_t l_40[1] = {0x2F};
        uint16_t *l_50 = &g_51;
        int32_t **l_52 = &g_30;
        int i;
        for (l_6 = 0; (l_6 <= 0); l_6 += 1)
        { 
            uint8_t *l_41 = &g_42;
            int32_t l_43 = (-1);
            int32_t l_44 = (-6);
            int32_t l_45 = 0xE948ECA1;
            int32_t l_46 = 0x44CAD706;
            if ((~((**g_29) = ((((safe_div_func_uint8_t_u_u(l_40[0], ((*l_41) = g_18.f0))) == l_43) & 0xE7E5) <= l_43))))
            { 
                int i;
                (*g_32) = l_43;
                return l_43;
            }
            else
            { 
                int i;
                g_47++;
            }
        }
        (*l_52) = ((((0U < ((((*l_50) = (((((*g_29) == (*g_29)) && l_40[0]) == (**g_29)) | 0x01)) != g_42) , g_25)) >= (*g_32)) | 0x0A93) , (*g_29));
        if ((*g_30))
            continue;
    }
    return g_18.f0;
}

static int32_t  func_2(int64_t  p_3, uint8_t  p_4)
{ 
    int64_t l_7[4];
    int i;
    for (i = 0; i < 4; i++)
        l_7[i] = (-6);
    for (p_3 = 3; (p_3 >= 0); p_3 -= 1)
    { 
        union U0 l_14 = {0x76AB};
        union U0 *l_22[4][2] = {{&l_14,&l_14},{&l_14,&l_14},{&l_14,&l_14},{&l_14,&l_14}};
        union U0 **l_21 = &l_22[0][1];
        int i, j;
        for (p_4 = 0; (p_4 <= 3); p_4 += 1)
        { 
            int i;
            if (l_7[p_4])
                break;
        }
        if (l_7[p_3])
            break;
        (*l_21) = ((func_8(l_14, ((+((((safe_lshift_func_uint32_t_u_u((g_5 ^ (g_18 , (-1))), p_4)) | g_5) | l_14.f0) ^ l_7[3])) || p_4), g_5, &l_14, g_18.f0) > g_5) , (void*)0);
    }
    return g_18.f0;
}

static int64_t  func_8(union U0  p_9, uint8_t  p_10, int32_t  p_11, union U0 * const  p_12, int64_t  p_13)
{ 
    int32_t *l_19[4][4] = {{&g_20,&g_20,&g_20,&g_20},{&g_20,&g_20,&g_20,&g_20},{&g_20,&g_20,&g_20,&g_20},{&g_20,&g_20,&g_20,&g_20}};
    int i, j;
    g_20 &= p_10;
    return p_13;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_18.f0, "g_18.f0", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_35.f0, "g_35.f0", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
