/* Csmith 0cdc710315cf seed 158, flattened by scripts/csmith_flatten.py */
// Options:   --seed 158 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint32_t  f0;
   uint64_t  f1;
   uint16_t  f2;
};

static int32_t g_15 = 0xA1C17C77;
static int32_t *g_14[4] = {&g_15,&g_15,&g_15,&g_15};
static int8_t g_29 = 0xB3;
static uint16_t g_45 = 0xE446;
static int8_t g_47 = 8;
static int64_t g_69 = 0;
static uint32_t g_71[1] = {1U};
static union U0 g_82 = {0x42294EC2};
static uint32_t g_100 = 4294967291U;
static int32_t *g_109[2][3] = {{&g_15,&g_15,&g_15},{&g_15,&g_15,&g_15}};
static const uint32_t g_119 = 4294967291U;
static const uint32_t *g_118 = &g_119;
static int32_t *g_147 = (void*)0;
static int32_t **g_146 = &g_147;
static int64_t * const g_169 = &g_69;
static int64_t * const *g_168 = &g_169;
static union U0 *g_194 = &g_82;
static union U0 **g_193[4] = {&g_194,&g_194,&g_194,&g_194};
static int16_t g_197 = 0;
static uint8_t g_199[3][4] = {{0x79,251U,0x51,0x51},{0x53,0x53,0x79,0x51},{254U,251U,254U,0x79}};

static union U0  func_1(void);
static int32_t * func_2(union U0  p_3);
static int32_t  func_5(int32_t  p_6, union U0  p_7, uint8_t  p_8, int16_t  p_9);

static union U0  func_1(void)
{ 
    union U0 l_4 = {0x5D09EA98};
    int32_t **l_80 = &g_14[2];
    int64_t l_84[2][3];
    int64_t l_87 = 0xE31553368A3BAB64LL;
    const uint32_t l_90[3] = {4294967290U,4294967290U,4294967290U};
    int32_t l_131 = 5;
    uint8_t l_145 = 0x35;
    int32_t l_180 = (-3);
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
            l_84[i][j] = (-4);
    }
    return (*g_194);
}

static int32_t * func_2(union U0  p_3)
{ 
    int32_t *l_16 = &g_15;
    union U0 l_17 = {0x2BA167C2};
    int32_t l_72 = 0;
    if ((func_5(((safe_add_func_int8_t_s_s(0, (((safe_rshift_func_uint16_t_u_u(((-7) & (((g_14[3] != l_16) , 0x3E) != g_15)), (*l_16))) ^ g_15) && 65535U))) , (*l_16)), l_17, g_15, p_3.f0) , p_3.f0))
    { 
        uint32_t l_62 = 0xEF59DFF0;
        int32_t l_63 = 0xC7BFB9E2;
        for (p_3.f1 = 6; (p_3.f1 == 3); --p_3.f1)
        { 
            int32_t **l_53[1];
            int64_t *l_68 = &g_69;
            int16_t *l_70 = (void*)0;
            uint64_t l_73 = 0x9A51C30C3E41A785LL;
            uint32_t *l_75 = &g_71[0];
            int i;
            for (i = 0; i < 1; i++)
                l_53[i] = &g_14[3];
            g_14[1] = (void*)0;
            l_72 ^= ((safe_mul_func_uint8_t_u_u((safe_add_func_uint32_t_u_u(((safe_div_func_uint32_t_u_u((((safe_sub_func_uint64_t_u_u((((l_63 = l_62) , (((((safe_lshift_func_int8_t_s_s((((g_71[0] = (((safe_rshift_func_int64_t_s_u(((*l_68) = g_47), p_3.f1)) & p_3.f1) < l_63)) != (*l_16)) && p_3.f1), 2)) && g_47) & 0x6BDE5E33) == 0xB1) ^ g_15)) >= (-8)), p_3.f1)) , (void*)0) == (void*)0), p_3.f1)) != 0U), 0x18E14F04)), l_62)) == p_3.f1);
            (*l_16) |= (((*l_75) ^= (l_73 < ((!(p_3.f1 > (-1))) && p_3.f1))) , p_3.f1);
        }
    }
    else
    { 
        int32_t *l_76[3][1];
        uint8_t l_77 = 1U;
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_76[i][j] = &g_15;
        }
        --l_77;
    }
    return &g_15;
}

static int32_t  func_5(int32_t  p_6, union U0  p_7, uint8_t  p_8, int16_t  p_9)
{ 
    int32_t l_24[2][4] = {{0x722ABAC8,0x722ABAC8,0x722ABAC8,0x722ABAC8},{0x722ABAC8,0x722ABAC8,0x722ABAC8,0x722ABAC8}};
    int32_t **l_50 = &g_14[1];
    int i, j;
    for (g_15 = 3; (g_15 >= 0); g_15 -= 1)
    { 
        int32_t *l_25 = (void*)0;
        int32_t l_26 = (-8);
        int8_t *l_28 = &g_29;
        int i;
    }
    (*l_50) = &l_24[1][0];
    return p_6;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_71[i], "g_71[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_82.f0, "g_82.f0", print_hash_value);
    transparent_crc(g_100, "g_100", print_hash_value);
    transparent_crc(g_119, "g_119", print_hash_value);
    transparent_crc(g_197, "g_197", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_199[i][j], "g_199[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
