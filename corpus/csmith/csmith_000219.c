/* Csmith 0cdc710315cf seed 219, flattened by scripts/csmith_flatten.py */
// Options:   --seed 219 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static int64_t
safe_unary_minus_func_int64_t_s(int64_t si )
{
  return
    (si==(-9223372036854775807L-1)) ?
    ((si)) :
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint64_t  f0;
   uint16_t  f1;
   uint8_t  f2;
};

struct S1 {
   uint16_t  f0;
   const int8_t  f1;
   int64_t  f2;
   int32_t  f3;
};

static int32_t g_4 = (-9);
static int32_t *g_7 = &g_4;
static uint32_t g_16[3] = {0xB57A67D9,0xB57A67D9,0xB57A67D9};
static int8_t g_24 = 2;
static int8_t g_42 = 1;
static struct S1 g_45 = {0x5BBB,0x85,-9,-8};
static struct S0 g_52 = {0x16CBB4D05C4471F2LL,0xED5A,6U};
static int8_t *g_60 = &g_42;
static uint16_t g_87 = 65526U;

static const struct S1  func_1(void);
static uint8_t  func_8(int32_t * p_9);
static int32_t * func_10(uint16_t  p_11, int32_t * const  p_12);

static const struct S1  func_1(void)
{ 
    int32_t *l_3 = &g_4;
    int32_t **l_2 = &l_3;
    int32_t *l_6 = (void*)0;
    int32_t **l_5[4];
    const struct S1 l_88 = {0xF61D,0x45,0x224176C37093F07CLL,0x432BFDB4};
    int i;
    for (i = 0; i < 4; i++)
        l_5[i] = &l_6;
    g_7 = ((*l_2) = (void*)0);
    g_87 = (func_8(func_10(g_4, &g_4)) || 0xAC);
    return l_88;
}

static uint8_t  func_8(int32_t * p_9)
{ 
    int64_t l_86 = 5;
    (*p_9) = (*p_9);
    (*g_7) = (-7);
    return l_86;
}

static int32_t * func_10(uint16_t  p_11, int32_t * const  p_12)
{ 
    int32_t **l_13[3];
    int8_t *l_28 = &g_24;
    struct S1 * const l_44 = &g_45;
    int8_t **l_59[1];
    const int64_t l_61 = 0xCE4AC035584D571FLL;
    int i;
    for (i = 0; i < 3; i++)
        l_13[i] = &g_7;
    for (i = 0; i < 1; i++)
        l_59[i] = &l_28;
    g_7 = &g_4;
    if ((g_16[0] ^= (safe_mod_func_uint32_t_u_u(0x825062D7, (*g_7)))))
    { 
        int32_t l_22 = 5;
        for (p_11 = 14; (p_11 <= 57); p_11 = safe_add_func_uint32_t_u_u(p_11, 9))
        { 
            int8_t *l_23 = &g_24;
            int32_t l_25 = (-2);
            (*p_12) = (safe_unary_minus_func_int64_t_s((((((safe_rshift_func_uint8_t_u_s(p_11, (l_22 , ((*l_23) = 0)))) < p_11) == l_25) >= 4) , 0x13053BF844BA928FLL)));
        }
    }
    else
    { 
        int32_t *l_49 = &g_45.f3;
        if ((&g_4 != &g_4))
        { 
            int8_t *l_29 = &g_24;
            int32_t l_30 = 0xF82966B9;
            if ((safe_rshift_func_int64_t_s_u((l_28 != l_29), (l_30 = (g_4 | g_24)))))
            { 
                uint8_t l_39 = 0x28;
                g_42 ^= (safe_mul_func_int32_t_s_s((safe_sub_func_uint32_t_u_u((safe_mod_func_uint8_t_u_u((safe_add_func_uint32_t_u_u(g_4, (l_39 < (safe_mul_func_int16_t_s_s(g_16[1], l_39))))), 0xFC)), 0x36EEBCB1)), (*g_7)));
            }
            else
            { 
                int32_t **l_43 = &g_7;
                (*l_43) = p_12;
            }
            if (((((l_29 == l_28) == (p_11 != 0xDD88)) != 0x54F90BF567D7132CLL) , (*g_7)))
            { 
                struct S1 *l_47 = &g_45;
                struct S1 **l_46 = &l_47;
                (*l_46) = l_44;
            }
            else
            { 
                int32_t *l_48 = &l_30;
                return l_49;
            }
        }
        else
        { 
            int32_t *l_50 = &g_45.f3;
            struct S0 *l_51 = &g_52;
            struct S0 **l_53 = (void*)0;
            struct S0 **l_54 = &l_51;
            l_50 = (g_7 = l_49);
            (*l_54) = l_51;
        }
    }
    if ((safe_add_func_uint16_t_u_u((safe_rshift_func_int8_t_s_s(((-4) && ((g_60 = &g_24) == (void*)0)), p_11)), l_61)))
    { 
        uint32_t l_76 = 4294967289U;
        uint16_t *l_80 = (void*)0;
        for (g_52.f1 = (-24); (g_52.f1 >= 45); g_52.f1++)
        { 
            const int32_t l_77 = 0x6A49DF3C;
            (*p_12) = ((safe_mul_func_int64_t_s_s(g_4, (safe_sub_func_int32_t_s_s((safe_mul_func_int32_t_s_s(((g_45 , ((safe_mul_func_int32_t_s_s(((safe_div_func_int32_t_s_s((safe_mod_func_uint32_t_u_u((&g_45 != (void*)0), p_11)), 1U)) >= l_76), 1U)) ^ (*g_7))) , 0xF014A1F6), p_11)), l_77)))) || 1U);
        }
        (*g_7) = (safe_lshift_func_int16_t_s_s(((p_11 >= ((g_16[0] || (g_45.f0--)) != (safe_add_func_int8_t_s_s(p_11, p_11)))) | 0xA8), 2));
    }
    else
    { 
        int32_t * const l_85 = (void*)0;
        (*g_7) = 0x654EA11A;
        g_7 = l_85;
        g_7 = &g_4;
    }
    return &g_4;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_16[i], "g_16[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_45.f0, "g_45.f0", print_hash_value);
    transparent_crc(g_45.f1, "g_45.f1", print_hash_value);
    transparent_crc(g_45.f2, "g_45.f2", print_hash_value);
    transparent_crc(g_45.f3, "g_45.f3", print_hash_value);
    transparent_crc(g_52.f0, "g_52.f0", print_hash_value);
    transparent_crc(g_52.f1, "g_52.f1", print_hash_value);
    transparent_crc(g_52.f2, "g_52.f2", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
