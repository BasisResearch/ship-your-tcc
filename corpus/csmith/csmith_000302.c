/* Csmith 0cdc710315cf seed 302, flattened by scripts/csmith_flatten.py */
// Options:   --seed 302 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
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
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
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

static int32_t g_3 = (-6);
static uint32_t g_15 = 1U;
static int32_t g_18 = (-8);
static uint16_t g_24 = 0x94AB;
static uint8_t g_40 = 0U;
static int32_t *g_44[4][1] = {{&g_3},{&g_3},{&g_3},{&g_3}};

static uint32_t  func_1(void);
static uint8_t  func_8(uint64_t  p_9, uint32_t  p_10, uint16_t  p_11, int32_t * p_12, const int32_t  p_13);
static const int64_t  func_29(int8_t  p_30, int32_t * p_31, int16_t  p_32);

static uint32_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    uint32_t *l_14[3];
    int32_t *l_16 = (void*)0;
    uint64_t l_47 = 0xDE197B2C3CC8C9F1LL;
    uint16_t l_52 = 0xCC8E;
    int i;
    for (i = 0; i < 3; i++)
        l_14[i] = &g_15;
    (*l_2) |= 0;
    if ((safe_sub_func_int32_t_s_s(((safe_mod_func_uint8_t_u_u(g_3, func_8(g_3, (g_15 = (*l_2)), g_3, l_16, (*l_2)))) > 1), g_18)))
    { 
        l_47 = ((((safe_lshift_func_int64_t_s_u(((func_29(g_24, &g_18, (((g_3 <= g_15) ^ 0xC6581630) <= g_18)) , (*l_2)) <= 18446744073709551613U), (*l_2))) > 0xE89B93B3BBBB531ELL) < (-8)) > g_15);
    }
    else
    { 
        int32_t **l_51 = &l_16;
        for (g_40 = 0; (g_40 <= 10); g_40++)
        { 
            int32_t **l_50 = &l_2;
            (*l_50) = l_14[1];
        }
        (*l_51) = &g_18;
        return l_52;
    }
    return g_40;
}

static uint8_t  func_8(uint64_t  p_9, uint32_t  p_10, uint16_t  p_11, int32_t * p_12, const int32_t  p_13)
{ 
    int32_t *l_17 = &g_18;
    int32_t l_19 = 0xFFCF33F2;
    int32_t *l_20 = &l_19;
    int32_t *l_21 = &l_19;
    int32_t l_22 = 0xA2215256;
    int32_t *l_23[3][1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_23[i][j] = &l_22;
    }
    ++g_24;
    return p_9;
}

static const int64_t  func_29(int8_t  p_30, int32_t * p_31, int16_t  p_32)
{ 
    uint64_t l_38 = 0xE77DA19AE86A6760LL;
    uint8_t *l_39 = &g_40;
    int32_t *l_41 = &g_3;
    int32_t **l_42 = (void*)0;
    int32_t **l_43 = &l_41;
    int32_t *l_46 = &g_3;
    int32_t **l_45 = &l_46;
    (*p_31) ^= (safe_sub_func_int16_t_s_s((safe_add_func_uint32_t_u_u((+((&g_18 != &g_18) & ((*l_39) ^= l_38))), l_38)), g_3));
    (*p_31) = (g_24 > (((*l_43) = l_41) != ((*l_45) = (g_44[0][0] = &g_3))));
    return g_40;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
