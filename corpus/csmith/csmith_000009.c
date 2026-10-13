/* Csmith 0cdc710315cf seed 9, flattened by scripts/csmith_flatten.py */
// Options:   --seed 9 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
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
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
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
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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

union U0 {
   const int16_t  f0;
   uint8_t  f1;
   uint32_t  f2;
};

static int32_t g_13 = 0x2276B87D;
static int32_t *g_12[4][3] = {{&g_13,&g_13,&g_13},{&g_13,&g_13,&g_13},{&g_13,&g_13,&g_13},{&g_13,&g_13,&g_13}};
static int32_t g_17 = 1;
static int32_t g_23 = 1;
static uint32_t g_27 = 0x2D6273D5;
static union U0 g_35 = {-7};
static union U0 *g_34 = &g_35;
static union U0 **g_33 = &g_34;
static uint8_t g_65 = 7U;
static uint64_t g_101 = 0x1928B54DF11966D8LL;
static uint32_t g_103 = 0x53696E02;
static int16_t g_108 = 0x3742;
static uint16_t g_110 = 65535U;

static int64_t  func_1(void);
static int32_t * func_2(int32_t  p_3, uint64_t  p_4, int64_t  p_5, uint8_t  p_6);
static int64_t  func_8(int32_t * p_9, int32_t * p_10, int32_t * p_11);

static int64_t  func_1(void)
{ 
    uint32_t l_7 = 0xBC6E5C7A;
    int32_t *l_14 = (void*)0;
    int32_t **l_112[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
    int i;
    g_12[1][0] = func_2(l_7, l_7, func_8(g_12[1][0], l_14, l_14), g_103);
    return g_101;
}

static int32_t * func_2(int32_t  p_3, uint64_t  p_4, int64_t  p_5, uint8_t  p_6)
{ 
    uint32_t l_106 = 1U;
    int16_t *l_107 = &g_108;
    uint16_t *l_109 = &g_110;
    int32_t *l_111 = &g_13;
    (*l_111) &= ((((*l_109) |= (p_4 & (((((*l_107) = (safe_div_func_int64_t_s_s(1, l_106))) < l_106) , l_106) | p_5))) && g_108) , p_4);
    (*l_111) = (*l_111);
    return l_111;
}

static int64_t  func_8(int32_t * p_9, int32_t * p_10, int32_t * p_11)
{ 
    union U0 **l_36 = (void*)0;
    const int32_t l_37[1][2] = {{0x02E63372,0x02E63372}};
    int32_t l_39 = 0x786C7A88;
    int32_t l_66 = 0xC907F7A7;
    const int64_t l_95[2] = {0x76005BF03BE979E0LL,0x76005BF03BE979E0LL};
    int i, j;
    for (g_13 = 0; (g_13 > (-18)); --g_13)
    { 
        uint16_t l_20 = 0U;
        uint32_t *l_26 = &g_27;
        union U0 *l_77[2][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0}};
        int i, j;
        for (g_17 = 2; (g_17 >= 0); g_17 -= 1)
        { 
            int i, j;
            if ((*p_9))
            { 
                union U0 *l_18[1];
                union U0 **l_19 = &l_18[0];
                int i;
                for (i = 0; i < 1; i++)
                    l_18[i] = (void*)0;
                (*l_19) = l_18[0];
            }
            else
            { 
                int32_t *l_21 = (void*)0;
                int32_t *l_22 = &g_23;
                (*l_22) = l_20;
            }
        }
        if ((safe_sub_func_int8_t_s_s((((*l_26) = 1U) == (*p_9)), (safe_lshift_func_uint32_t_u_u((+0x4C6A), l_20)))))
        { 
            for (g_27 = 0; g_27 < 4; g_27 = g_27 + 1)
            {
                for (g_23 = 0; g_23 < 3; g_23 = g_23 + 1)
                {
                    g_12[g_27][g_23] = (void*)0;
                }
            }
        }
        else
        { 
            int64_t l_46 = 0x6D4F509E13C9534CLL;
            int32_t *l_51 = (void*)0;
            int32_t *l_52 = (void*)0;
            int32_t *l_53 = &g_23;
            int32_t **l_54 = (void*)0;
            int32_t **l_55 = (void*)0;
            int32_t **l_56 = &g_12[1][0];
            for (g_23 = 0; (g_23 <= (-20)); g_23 = safe_sub_func_uint32_t_u_u(g_23, 9))
            { 
                int32_t *l_38[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
                int i;
                l_39 = (((g_33 = (void*)0) == l_36) < l_37[0][1]);
            }
            (*l_53) &= (safe_sub_func_int16_t_s_s((safe_mod_func_int32_t_s_s((g_17 == ((--(*l_26)) < l_46)), (safe_mul_func_int8_t_s_s(((safe_mod_func_int32_t_s_s(0x400B8F3B, l_39)) , l_46), l_46)))), 7U));
            (*l_56) = p_9;
        }
        for (g_35.f2 = (-19); (g_35.f2 >= 3); g_35.f2 = safe_add_func_uint16_t_u_u(g_35.f2, 1))
        { 
            const union U0 *l_61 = &g_35;
            uint8_t *l_64[3][4] = {{&g_65,&g_65,&g_65,&g_65},{&g_65,&g_65,&g_65,&g_65},{&g_65,&g_65,&g_65,&g_65}};
            int32_t l_67 = 0xC717898A;
            int i, j;
            for (g_23 = 17; (g_23 == (-11)); g_23 = safe_sub_func_uint8_t_u_u(g_23, 6))
            { 
                return g_17;
            }
            if (((l_61 != (((safe_lshift_func_uint16_t_u_u((((g_65++) ^ ((-1) & (safe_mul_func_int32_t_s_s((safe_sub_func_int64_t_s_s(l_20, 0x40931BC1F9C85207LL)), (*p_9))))) >= l_20), 1)) , l_67) , &g_35)) , 2))
            { 
                union U0 **l_74 = &g_34;
                union U0 *l_76 = &g_35;
                union U0 **l_75 = &l_76;
                int32_t l_78 = 0;
                int32_t *l_79 = &g_17;
                (*l_75) = ((*l_74) = &g_35);
                l_78 &= (((*l_75) = (l_67 , l_77[0][1])) != (*l_74));
                (*l_79) ^= (*p_9);
            }
            else
            { 
                uint64_t l_90[4][1] = {{0xE904CDE8B735D465LL},{0xDB4C2A7322E546ECLL},{0xE904CDE8B735D465LL},{0xDB4C2A7322E546ECLL}};
                int32_t l_91 = (-1);
                int32_t **l_92[2];
                int i, j;
                for (i = 0; i < 2; i++)
                    l_92[i] = &g_12[1][0];
                l_91 ^= ((safe_lshift_func_int64_t_s_s(((safe_sub_func_uint64_t_u_u((((((safe_sub_func_int16_t_s_s((&l_77[0][0] != &g_34), (safe_sub_func_uint64_t_u_u((safe_rshift_func_int8_t_s_s(0x63, l_67)), g_17)))) != l_90[3][0]) <= l_20) ^ 9) < l_37[0][1]), 0x687F7F2BFCFF1470LL)) <= g_13), l_67)) , l_20);
                g_12[1][0] = (void*)0;
            }
        }
    }
    if ((g_27 != ((g_13 && (((safe_mul_func_uint8_t_u_u((l_66 ^ g_13), g_13)) & (*p_9)) > 0xE0)) > l_95[0])))
    { 
        int32_t l_96 = 0xFBCC8317;
        return l_96;
    }
    else
    { 
        const uint8_t *l_99 = &g_65;
        uint64_t *l_100 = &g_101;
        uint8_t *l_102 = &g_65;
        (*p_9) |= (safe_lshift_func_int64_t_s_u(((l_99 != (void*)0) || (((*l_102) = (((*l_100) = (1 <= 0x27CECC5629281337LL)) > g_27)) & (-7))), 30));
    }
    return l_37[0][1];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_101, "g_101", print_hash_value);
    transparent_crc(g_103, "g_103", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    transparent_crc(g_110, "g_110", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
