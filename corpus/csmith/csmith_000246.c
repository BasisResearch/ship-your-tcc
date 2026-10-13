/* Csmith 0cdc710315cf seed 246, flattened by scripts/csmith_flatten.py */
// Options:   --seed 246 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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

union U0 {
   uint32_t  f0;
   int32_t  f1;
};

static uint16_t g_19[1][3] = {{0x14F9,0x14F9,0x14F9}};
static uint8_t g_21 = 247U;
static uint8_t *g_23 = (void*)0;
static int64_t g_25 = 6;
static int32_t g_28 = (-3);
static union U0 g_31 = {0xB333EE67};
static int32_t *g_37 = &g_28;
static int8_t g_43 = 4;
static int64_t g_44 = 0x9E5F9C6A002B50D2LL;
static uint32_t g_45[1] = {0U};
static uint32_t g_62 = 4294967295U;

static uint16_t  func_1(void);
static const uint32_t  func_2(uint64_t  p_3, union U0  p_4, uint32_t  p_5, const int64_t  p_6);
static union U0  func_8(int64_t  p_9, uint32_t  p_10);

static uint16_t  func_1(void)
{ 
    uint16_t l_7 = 65535U;
    uint8_t *l_20 = &g_21;
    int64_t *l_24 = &g_25;
    int32_t l_32 = (-6);
    union U0 l_33 = {4294967292U};
    int32_t l_61 = 0;
    if ((func_2((l_32 = (l_7 != (((g_31 = func_8(((*l_24) |= (safe_mod_func_uint16_t_u_u((((((safe_sub_func_int32_t_s_s((safe_rshift_func_int16_t_s_u((safe_mul_func_uint8_t_u_u(((*l_20) = g_19[0][1]), (!((void*)0 == g_23)))), l_7)), l_7)) ^ g_19[0][1]) != (-1)) < l_7) & l_7), g_19[0][1]))), g_19[0][1])) , g_25) | g_19[0][1]))), l_33, g_28, g_19[0][1]) != l_33.f0))
    { 
        int8_t l_60 = 0x6F;
        for (l_33.f0 = (-20); (l_33.f0 != 36); l_33.f0++)
        { 
            int64_t l_59 = 0x9D328171C63CC5DBLL;
            for (g_44 = 0; (g_44 <= 0); g_44 += 1)
            { 
                int32_t *l_53 = &g_31.f1;
                int32_t *l_54 = &g_28;
                int32_t *l_55 = &g_28;
                int32_t *l_56 = &g_28;
                int32_t *l_57 = &l_32;
                int32_t *l_58[2][3] = {{(void*)0,(void*)0,(void*)0},{&l_32,&l_32,&l_32}};
                int8_t l_65[1];
                int i, j;
                for (i = 0; i < 1; i++)
                    l_65[i] = (-5);
                ++g_62;
                (*l_57) ^= ((*l_56) = g_45[g_44]);
                l_65[0] ^= g_19[g_44][(g_44 + 2)];
            }
        }
    }
    else
    { 
        (*g_37) = l_61;
    }
    return l_61;
}

static const uint32_t  func_2(uint64_t  p_3, union U0  p_4, uint32_t  p_5, const int64_t  p_6)
{ 
    int32_t *l_34 = &g_28;
    int32_t **l_35 = (void*)0;
    int32_t **l_36[3];
    int i;
    for (i = 0; i < 3; i++)
        l_36[i] = (void*)0;
    (*l_34) ^= ((l_34 == (g_37 = l_34)) | p_6);
    for (g_28 = 0; (g_28 >= 3); ++g_28)
    { 
        int32_t l_40 = 0x87C7A501;
        int32_t l_41 = 0xA5EB5A40;
        int32_t *l_48 = &g_31.f1;
        l_41 = ((l_40 = ((((((func_8(p_4.f0, g_19[0][2]) , &g_37) != &l_34) == 8) >= p_3) >= 0x1EAE) > g_25)) < (*g_37));
        for (p_5 = 0; (p_5 <= 0); p_5 += 1)
        { 
            int32_t l_42 = 0x09E00C25;
            int i;
            g_45[0]--;
            l_48 = &g_28;
        }
        l_41 |= (((safe_mul_func_uint16_t_u_u(((*l_34) || p_4.f0), 0xA273)) <= (-2)) , (*g_37));
    }
    (*l_34) = 0;
    return p_6;
}

static union U0  func_8(int64_t  p_9, uint32_t  p_10)
{ 
    uint64_t l_26[1];
    int32_t *l_27[1];
    int16_t l_29 = (-1);
    union U0 l_30 = {8U};
    int i;
    for (i = 0; i < 1; i++)
        l_26[i] = 8U;
    for (i = 0; i < 1; i++)
        l_27[i] = &g_28;
    l_29 &= l_26[0];
    return l_30;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_19[i][j], "g_19[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_31.f0, "g_31.f0", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_45[i], "g_45[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_62, "g_62", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
