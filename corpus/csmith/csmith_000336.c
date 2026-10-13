/* Csmith 0cdc710315cf seed 336, flattened by scripts/csmith_flatten.py */
// Options:   --seed 336 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint32_t  f0;
};

static int16_t g_4 = 0xB136;
static int32_t g_19 = 0x5261FC42;
static uint8_t g_22 = 0U;
static struct S0 g_34 = {0x1B705618};
static struct S0 g_36 = {4294967295U};
static struct S0 g_37 = {0U};
static int32_t *g_42 = (void*)0;
static int8_t g_43 = 0;
static int32_t **g_47 = &g_42;

static int64_t  func_1(void);
static int32_t  func_2(uint32_t  p_3);
static int32_t * func_6(const uint16_t  p_7, struct S0  p_8, int32_t * const  p_9);

static int64_t  func_1(void)
{ 
    uint8_t l_5 = 0x30;
    struct S0 l_17 = {1U};
    int32_t *l_41 = &g_19;
    if (func_2(g_4))
    { 
        const uint32_t l_10[4] = {4294967289U,4294967289U,4294967289U,4294967289U};
        int16_t *l_15[4][1];
        int16_t l_16 = 0x587C;
        int32_t * const l_18[4] = {&g_19,&g_19,&g_19,&g_19};
        int32_t **l_38 = (void*)0;
        int32_t *l_40 = (void*)0;
        int32_t **l_39[2][2] = {{&l_40,&l_40},{&l_40,&l_40}};
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_15[i][j] = (void*)0;
        }
        g_42 = (l_41 = (l_5 , func_6((l_10[0] , (safe_sub_func_int32_t_s_s((safe_mul_func_int16_t_s_s((l_16 = (l_10[2] == 0xFB)), 0x89BD)), 0x168F928D))), l_17, l_18[0])));
        g_43 ^= (*g_42);
    }
    else
    { 
        int32_t **l_48 = &g_42;
        for (l_5 = 0; (l_5 <= 19); ++l_5)
        { 
            int32_t **l_46 = &g_42;
            (*l_41) ^= ((l_48 = (g_47 = l_46)) == (void*)0);
        }
    }
    return (*l_41);
}

static int32_t  func_2(uint32_t  p_3)
{ 
    return g_4;
}

static int32_t * func_6(const uint16_t  p_7, struct S0  p_8, int32_t * const  p_9)
{ 
    uint8_t *l_21 = &g_22;
    const int32_t l_31 = (-5);
    int32_t l_33 = (-6);
    if (((~((*l_21) = ((-10) ^ (((*p_9) = (g_4 ^ g_19)) & g_4)))) > p_7))
    { 
        int32_t *l_25 = &g_19;
        int32_t **l_24 = &l_25;
        (*l_24) = ((+0x3423637B30477AE2LL) , p_9);
    }
    else
    { 
        int32_t *l_32 = (void*)0;
        struct S0 *l_35 = &g_36;
        for (p_8.f0 = 0; (p_8.f0 == 1); p_8.f0++)
        { 
            uint32_t l_28 = 7U;
            g_19 = l_28;
        }
        l_33 ^= (((g_19 , (safe_lshift_func_uint8_t_u_s(((*l_21) = (4294967295U != g_4)), l_31))) , g_19) , 0x3709E8E5);
        g_37 = ((*l_35) = (g_34 = g_34));
    }
    return &g_19;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_34.f0, "g_34.f0", print_hash_value);
    transparent_crc(g_36.f0, "g_36.f0", print_hash_value);
    transparent_crc(g_37.f0, "g_37.f0", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
