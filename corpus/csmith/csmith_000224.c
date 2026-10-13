/* Csmith 0cdc710315cf seed 224, flattened by scripts/csmith_flatten.py */
// Options:   --seed 224 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 + si2);
}
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
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
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = (-3);
static uint8_t g_23 = 0xA7;
static int16_t g_25 = 0x1289;
static int8_t g_30 = 1;
static uint64_t g_82 = 18446744073709551607U;
static uint32_t g_88 = 0xCDB7CFF1;

static int8_t  func_1(void);
static int32_t * func_12(uint64_t  p_13, uint16_t  p_14, int32_t * p_15);
static uint32_t  func_21(uint64_t  p_22);

static int8_t  func_1(void)
{ 
    int32_t l_11 = 0x5950707D;
    uint8_t l_56[1];
    int i;
    for (i = 0; i < 1; i++)
        l_56[i] = 0xDA;
    for (g_2 = 0; (g_2 == 2); g_2 = safe_add_func_int16_t_s_s(g_2, 1))
    { 
        int64_t l_10 = (-4);
        int32_t l_28 = 0xA1D2DAFF;
        int32_t l_54 = 0x42FAFECE;
        uint8_t *l_67 = &g_23;
        int32_t *l_87 = (void*)0;
        if ((safe_div_func_uint8_t_u_u(((!(((safe_mul_func_int32_t_s_s(l_10, l_11)) , g_2) & g_2)) <= g_2), 254U)))
        { 
            uint8_t l_18 = 255U;
            int16_t *l_24[4][4] = {{&g_25,&g_25,&g_25,&g_25},{&g_25,&g_25,&g_25,&g_25},{&g_25,&g_25,&g_25,&g_25},{&g_25,&g_25,&g_25,&g_25}};
            int32_t l_26[4][4] = {{0xCFF15CC7,(-1),(-1),0xCFF15CC7},{0xCFF15CC7,(-1),(-1),0xCFF15CC7},{0xCFF15CC7,(-1),(-1),0xCFF15CC7},{0xCFF15CC7,(-1),(-1),0xCFF15CC7}};
            int32_t *l_32 = (void*)0;
            int32_t **l_31 = &l_32;
            uint8_t *l_51 = (void*)0;
            uint8_t *l_52 = (void*)0;
            uint8_t *l_53 = &g_23;
            int32_t *l_55[3][3] = {{(void*)0,(void*)0,(void*)0},{&l_26[2][3],&l_28,&l_26[2][3]},{(void*)0,(void*)0,(void*)0}};
            int i, j;
            (*l_31) = func_12((l_28 = ((safe_mul_func_int64_t_s_s((l_18 || (safe_sub_func_uint32_t_u_u(func_21(((g_2 , (l_26[2][3] |= (((&g_2 != &g_2) | 1) | g_23))) > 0xB8CF)), 1))), 0xC6DD3ED23C5AD058LL)) > l_11)), g_25, &g_2);
            l_56[0] = (safe_lshift_func_uint8_t_u_u((safe_mod_func_uint16_t_u_u(0xF205, (safe_div_func_uint64_t_u_u(((safe_lshift_func_uint32_t_u_u((safe_div_func_uint16_t_u_u(0x0187, (safe_add_func_uint8_t_u_u((((safe_div_func_int8_t_s_s((safe_rshift_func_uint8_t_u_u(((*l_53) = (safe_sub_func_int8_t_s_s((-1), 0x88))), 0)), 7)) & 7U) , l_54), 1)))), g_2)) == g_25), l_10)))), g_25));
            for (g_23 = 12; (g_23 != 28); g_23 = safe_add_func_int32_t_s_s(g_23, 2))
            { 
                (*l_31) = (void*)0;
            }
        }
        else
        { 
            int8_t l_85 = 0x5C;
            for (l_10 = 0; (l_10 == (-8)); --l_10)
            { 
                uint8_t *l_66 = &l_56[0];
                uint8_t **l_65 = &l_66;
                int32_t *l_68 = &l_54;
                (*l_68) ^= (0U || (safe_div_func_uint8_t_u_u(g_2, (safe_sub_func_uint8_t_u_u((((*l_65) = &g_23) != l_67), g_30)))));
                (*l_68) ^= g_25;
                if (l_28)
                    continue;
            }
            if (l_28)
            { 
                if (l_11)
                    break;
            }
            else
            { 
                uint64_t *l_81 = &g_82;
                int32_t l_84 = 0;
                int32_t *l_86 = &l_28;
                (*l_86) = (safe_mul_func_uint16_t_u_u((safe_mul_func_int16_t_s_s((safe_rshift_func_uint64_t_u_u((safe_lshift_func_uint16_t_u_u((((safe_lshift_func_int8_t_s_u(((safe_mod_func_uint64_t_u_u(((*l_81) = l_56[0]), 0x29ADBE56F6324B7CLL)) > (~(l_84 , g_30))), 6)) != 1U) && 1U), l_85)), l_84)), 4U)), 6));
            }
        }
        g_88 |= 0x8108949E;
    }
    return l_11;
}

static int32_t * func_12(uint64_t  p_13, uint16_t  p_14, int32_t * p_15)
{ 
    int32_t *l_29[1];
    int i;
    for (i = 0; i < 1; i++)
        l_29[i] = (void*)0;
    g_30 = ((g_25 > 5) | (p_14 , (-1)));
    return &g_2;
}

static uint32_t  func_21(uint64_t  p_22)
{ 
    int64_t l_27 = (-8);
    return l_27;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_82, "g_82", print_hash_value);
    transparent_crc(g_88, "g_88", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
