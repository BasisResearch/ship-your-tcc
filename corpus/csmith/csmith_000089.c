/* Csmith 0cdc710315cf seed 89, flattened by scripts/csmith_flatten.py */
// Options:   --seed 89 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_5 = 0xBE1D925B;
static int32_t *g_4 = &g_5;
static int8_t g_12 = (-1);
static int8_t g_14 = (-9);
static uint64_t g_30 = 0x142C78EF8A6EBBFALL;
static uint16_t g_38 = 0x67C8;
static uint64_t * const g_44 = (void*)0;
static uint64_t * const *g_43[2] = {&g_44,&g_44};
static int16_t g_45 = 0x7D3D;
static uint32_t g_53 = 0x04418F7F;
static int16_t g_69 = 0x53C4;
static uint8_t g_73 = 255U;

static int8_t  func_1(void);
static int32_t * func_2(int32_t * p_3);
static int32_t  func_18(int64_t  p_19, const uint64_t  p_20, int8_t  p_21);

static int8_t  func_1(void)
{ 
    int32_t l_78 = 0xCC01E3B4;
    g_4 = func_2(g_4);
    return l_78;
}

static int32_t * func_2(int32_t * p_3)
{ 
    int8_t l_16 = 0xFD;
    for (g_5 = 0; (g_5 > (-14)); g_5--)
    { 
        uint8_t l_8 = 0xC2;
        int8_t *l_11 = &g_12;
        int8_t *l_13 = &g_14;
        uint8_t *l_15 = &l_8;
        int32_t l_17 = 0;
        uint8_t **l_28 = &l_15;
        uint64_t *l_29 = &g_30;
        int64_t l_31 = (-3);
        int32_t *l_76 = (void*)0;
        int32_t *l_77 = &l_17;
        l_17 = ((l_8 || (((safe_mul_func_uint32_t_u_u((((((*l_13) = ((*l_11) ^= 1)) & ((*l_15) |= (g_5 && (*p_3)))) == 65527U) ^ 0x145CCB97), l_16)) && g_12) , 1)) >= g_5);
        (*l_77) = (func_18(((((safe_rshift_func_int16_t_s_u((((safe_mul_func_int16_t_s_s((((*l_29) ^= (l_16 > (safe_sub_func_int16_t_s_s((((*l_28) = &l_8) == (void*)0), l_8)))) , l_8), l_31)) & l_17) <= 0xD800F4D4E2095E04LL), 2)) , (*g_4)) < (*p_3)) <= g_5), g_5, g_5) != 1U);
    }
    return &g_5;
}

static int32_t  func_18(int64_t  p_19, const uint64_t  p_20, int8_t  p_21)
{ 
    uint64_t l_50 = 0x9C1C50D5D742CC32LL;
    int64_t l_57 = 3;
    uint64_t *l_67 = &l_50;
    uint64_t **l_66[2][2] = {{&l_67,&l_67},{&l_67,&l_67}};
    int i, j;
    for (p_19 = 0; (p_19 == 19); ++p_19)
    { 
        uint64_t * const l_42 = (void*)0;
        uint64_t * const *l_41 = &l_42;
        uint32_t *l_51 = (void*)0;
        uint32_t *l_52 = &g_53;
        int32_t l_54 = 0xECE519C8;
        const int32_t l_65 = 0xD1A1FBCD;
        for (g_12 = 25; (g_12 <= (-12)); g_12 = safe_sub_func_uint8_t_u_u(g_12, 2))
        { 
            int32_t *l_36 = (void*)0;
            int32_t *l_37 = (void*)0;
            g_38++;
            g_43[1] = l_41;
            g_45 = p_19;
        }
        l_54 ^= (((safe_mod_func_uint32_t_u_u(((*l_52) = ((p_21 <= (((((0xC253D5AB & (((safe_mod_func_int32_t_s_s(p_21, l_50)) , l_50) <= p_20)) , 6U) , p_19) <= p_19) >= l_50)) & p_21)), 0x3E4F9F01)) >= 0x89) != 0x9523668F781BE2FCLL);
        if ((safe_sub_func_int8_t_s_s(g_38, l_57)))
        { 
            int32_t *l_68 = &l_54;
            for (g_53 = 0; (g_53 <= 1); g_53 += 1)
            { 
                int32_t *l_62 = &l_54;
                int i;
                (*l_62) = (safe_lshift_func_int16_t_s_u(((safe_rshift_func_int64_t_s_s(((l_52 != &g_53) < p_20), 42)) , 4), 9));
            }
            (*l_68) = (((((safe_rshift_func_int64_t_s_u((((l_65 , l_66[1][0]) != &l_42) > p_20), p_21)) || p_19) > (-1)) < p_19) ^ (-6));
            if (p_20)
                break;
        }
        else
        { 
            int32_t *l_70 = &l_54;
            int32_t *l_71 = &l_54;
            int32_t *l_72[2];
            int i;
            for (i = 0; i < 2; i++)
                l_72[i] = &l_54;
            for (g_38 = 0; (g_38 <= 1); g_38 += 1)
            { 
                return p_21;
            }
            g_73--;
        }
    }
    return p_19;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_73, "g_73", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
