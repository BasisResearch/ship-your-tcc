/* Csmith 0cdc710315cf seed 206, flattened by scripts/csmith_flatten.py */
// Options:   --seed 206 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (4294967295U >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int32_t  f0;
   int16_t  f1;
   int8_t  f2;
   uint32_t  f3;
};

union U1 {
   uint32_t  f0;
   struct S0  f1;
   uint32_t  f2;
};

static uint64_t g_8 = 0x146CDEAD9BACC5D9LL;
static const int64_t g_16 = 0xF4FFCF33F23E93D0LL;
static int32_t g_18 = 7;
static int32_t *g_17 = &g_18;
static uint8_t g_23 = 248U;
static uint64_t g_41 = 0xB88CDF8ABFF5ECCELL;
static struct S0 g_47 = {0xB8CE2295,0,-8,0x22F8DC70};
static union U1 g_48[1] = {{0U}};
static int32_t g_52 = 1;
static int32_t g_83 = (-1);

static union U1  func_1(void);
static const int32_t * func_2(uint8_t  p_3, int32_t * p_4);
static int64_t  func_11(const int8_t  p_12, uint64_t * p_13, int32_t * p_14, int8_t  p_15);

static union U1  func_1(void)
{ 
    uint64_t *l_7 = &g_8;
    int32_t l_69 = 0x7D796E99;
    const int32_t *l_93 = &g_52;
    const int32_t **l_92 = &l_93;
    (*l_92) = func_2((((safe_sub_func_int64_t_s_s(((++(*l_7)) & (func_11(g_16, l_7, g_17, g_16) > g_47.f2)), g_47.f2)) & l_69) || (*g_17)), g_17);
    return g_48[0];
}

static const int32_t * func_2(uint8_t  p_3, int32_t * p_4)
{ 
    int32_t **l_70 = &g_17;
    int32_t l_84[4] = {1,1,1,1};
    const int64_t l_85 = 0xD55137C5FD5384E0LL;
    int i;
    (*l_70) = p_4;
    for (g_8 = (-2); (g_8 < 9); g_8 = safe_add_func_uint64_t_u_u(g_8, 6))
    { 
        for (p_3 = 5; (p_3 < 23); p_3 = safe_add_func_int8_t_s_s(p_3, 9))
        { 
            struct S0 l_80 = {0x4F37316A,4,0x47,0x3E03EB41};
            int8_t *l_81 = &g_48[0].f1.f2;
            int32_t *l_82[3];
            int i;
            for (i = 0; i < 3; i++)
                l_82[i] = &g_83;
            l_84[1] &= ((**l_70) = (((((-5) | (g_47.f3 = (~(safe_mul_func_uint8_t_u_u((safe_div_func_int32_t_s_s(((l_80 , (((*l_81) = ((g_52 || 4294967295U) == p_3)) , p_3)) == p_3), (*g_17))), (-1)))))) , 0x88197C51) | g_47.f0) || g_47.f2));
            if (l_85)
                break;
        }
    }
    for (g_83 = 0; (g_83 <= (-4)); g_83 = safe_sub_func_int64_t_s_s(g_83, 5))
    { 
        for (g_47.f1 = (-11); (g_47.f1 <= (-17)); g_47.f1 = safe_sub_func_int32_t_s_s(g_47.f1, 2))
        { 
            for (g_47.f2 = 0; (g_47.f2 <= (-13)); g_47.f2 = safe_sub_func_uint16_t_u_u(g_47.f2, 6))
            { 
                if ((*g_17))
                    break;
            }
            return (*l_70);
        }
        (*l_70) = &g_52;
        if ((**l_70))
            break;
    }
    return &g_52;
}

static int64_t  func_11(const int8_t  p_12, uint64_t * p_13, int32_t * p_14, int8_t  p_15)
{ 
    int16_t l_20 = 0;
    uint8_t *l_21 = (void*)0;
    uint8_t *l_22 = &g_23;
    uint64_t *l_40 = &g_41;
    int16_t l_42[2];
    int32_t l_62 = 0xDF936BB3;
    int32_t l_63 = 1;
    int32_t l_64 = 0x71C9EEB3;
    int32_t l_65 = (-1);
    int i;
    for (i = 0; i < 2; i++)
        l_42[i] = (-6);
    if (((+((*l_22)++)) ^ (((safe_add_func_int32_t_s_s((safe_div_func_int64_t_s_s((safe_mod_func_uint8_t_u_u((safe_lshift_func_int8_t_s_u((safe_lshift_func_uint64_t_u_s(((*l_40) = ((safe_mod_func_int64_t_s_s(l_20, (-1))) != (safe_mul_func_int64_t_s_s((0xC7FFFAA2 | p_12), l_20)))), 53)), l_20)), p_12)), 0x455F992DB756FD8CLL)), p_15)) , l_20) > g_16)))
    { 
        const int32_t l_57 = 1;
        for (g_23 = 0; (g_23 <= 1); g_23 += 1)
        { 
            int64_t l_55 = (-3);
            for (g_18 = 1; (g_18 >= 0); g_18 -= 1)
            { 
                int32_t *l_51 = &g_52;
                int i;
                (*l_51) |= ((safe_add_func_int32_t_s_s(((safe_div_func_int8_t_s_s((g_47 , (g_48[0] , (safe_lshift_func_uint32_t_u_s(4294967294U, 19)))), 1)) >= l_42[g_23]), l_42[0])) <= l_42[1]);
            }
            (*p_14) = (safe_div_func_int64_t_s_s((((l_55 ^= 7U) != (safe_unary_minus_func_uint8_t_u((0x3CC8C9F13A4B1F05LL > l_57)))) == p_15), 1U));
            (*g_17) = (*g_17);
        }
    }
    else
    { 
        union U1 *l_58 = &g_48[0];
        union U1 **l_59 = &l_58;
        int32_t l_60 = 1;
        int32_t *l_61[1];
        uint64_t l_66 = 18446744073709551615U;
        int i;
        for (i = 0; i < 1; i++)
            l_61[i] = &g_47.f0;
        (*l_59) = l_58;
        l_66--;
        l_65 &= (*p_14);
    }
    return l_64;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_47.f0, "g_47.f0", print_hash_value);
    transparent_crc(g_47.f1, "g_47.f1", print_hash_value);
    transparent_crc(g_47.f2, "g_47.f2", print_hash_value);
    transparent_crc(g_47.f3, "g_47.f3", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_48[i].f0, "g_48[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_83, "g_83", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
