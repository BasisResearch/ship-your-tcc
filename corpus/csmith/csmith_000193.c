/* Csmith 0cdc710315cf seed 193, flattened by scripts/csmith_flatten.py */
// Options:   --seed 193 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint64_t  f0;
   int8_t  f1;
};

union U1 {
   const int8_t  f0;
   uint16_t  f1;
};

static uint64_t g_12[2][2] = {{18446744073709551615U,18446744073709551615U},{18446744073709551615U,18446744073709551615U}};
static struct S0 g_17 = {18446744073709551615U,0xA3};
static int32_t g_21 = 0x3700168A;
static const int32_t *g_27 = &g_21;
static int32_t *g_33 = &g_21;
static int64_t g_38 = 0x5B0D6B1F8487B254LL;
static uint32_t g_40 = 0xCC72FDDD;

static uint32_t  func_1(void);
static const int32_t * func_2(int64_t  p_3);
static int32_t * func_5(int8_t  p_6, uint32_t  p_7, int64_t  p_8);

static uint32_t  func_1(void)
{ 
    uint16_t l_4 = 0x836C;
    int32_t *l_30 = &g_21;
    int32_t *l_32 = &g_21;
    int32_t **l_31[4] = {&l_32,&l_32,&l_32,&l_32};
    union U1 l_34 = {-3};
    const int32_t *l_43 = &g_21;
    struct S0 l_65 = {7U,8};
    int i;
    g_27 = func_2(l_4);
    for (g_17.f0 = 0; (g_17.f0 != 50); g_17.f0 = safe_add_func_uint8_t_u_u(g_17.f0, 1))
    { 
        g_21 = l_4;
    }
    if ((l_30 != (g_33 = &g_21)))
    { 
        int64_t *l_37 = &g_38;
        uint32_t *l_39 = &g_40;
        struct S0 l_41[3] = {{18446744073709551615U,0},{18446744073709551615U,0},{18446744073709551615U,0}};
        struct S0 *l_42[1][1];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_42[i][j] = &l_41[1];
        }
        g_17 = ((0xE5156BE8236511F0LL <= ((((*l_39) |= (((l_34 , (safe_lshift_func_int64_t_s_s(((*l_37) = g_17.f1), 6))) >= g_21) > g_21)) < g_12[1][0]) > (-1))) , l_41[1]);
        if ((*g_33))
        { 
            return g_12[1][0];
        }
        else
        { 
            l_43 = func_2(g_12[1][0]);
        }
    }
    else
    { 
        uint16_t *l_45 = &l_4;
        int32_t l_56 = 0xB720FAE2;
        int32_t *l_57 = &g_21;
        (*l_30) = (safe_unary_minus_func_uint16_t_u((++(*l_45))));
        if (g_40)
            goto lbl_58;
lbl_58:
        g_27 = (l_57 = func_5((safe_div_func_int16_t_s_s((((*l_32) = (safe_add_func_int32_t_s_s((safe_mul_func_uint64_t_u_u(0U, (g_21 , (safe_div_func_int8_t_s_s(g_40, l_56))))), (*g_33)))) >= l_56), l_56)), g_40, g_17.f1));
        if ((((-2) <= 0xD5A33B115E53F01ELL) == g_17.f1))
        { 
            int64_t *l_62 = &g_38;
            int32_t l_63[4][3] = {{0x01266394,0xD19708E6,1},{0x01266394,0x01266394,0xD19708E6},{0x102A83C5,0xD19708E6,0xD19708E6},{0xD19708E6,0x16AF0CFC,1}};
            int i, j;
            if ((safe_sub_func_uint64_t_u_u((((g_33 = func_5((l_63[3][0] = (safe_unary_minus_func_int64_t_s(((((&g_38 == l_62) < ((*g_33) |= 0xEE3B6EA1)) > g_12[1][0]) >= 0)))), g_38, g_17.f1)) == g_27) ^ l_56), 3U)))
            { 
                struct S0 *l_64 = &g_17;
                (*l_64) = g_17;
                g_17 = l_65;
            }
            else
            { 
                g_27 = &g_21;
            }
            (*l_32) |= 7;
        }
        else
        { 
            for (l_56 = 0; (l_56 > 16); ++l_56)
            { 
                int64_t *l_69[3][2];
                int64_t **l_68 = &l_69[1][0];
                int32_t l_70 = (-1);
                int i, j;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_69[i][j] = &g_38;
                }
                l_70 &= (((((((*l_68) = (void*)0) == (void*)0) <= 1) , (*l_30)) != g_17.f1) , (-3));
            }
        }
    }
    return (*l_32);
}

static const int32_t * func_2(int64_t  p_3)
{ 
    uint16_t l_13 = 65526U;
    int32_t *l_20[3][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0}};
    int32_t **l_19 = &l_20[0][0];
    struct S0 *l_22 = &g_17;
    uint32_t l_25 = 4294967295U;
    struct S0 *l_26 = &g_17;
    int i, j;
    (*l_19) = func_5((safe_lshift_func_uint64_t_u_u(p_3, 2)), p_3, (~(g_12[1][0] && l_13)));
    g_21 = ((*l_19) != ((((l_22 = &g_17) != (((safe_mul_func_uint64_t_u_u(l_25, 0x000E2E4ED383DE46LL)) || g_12[0][1]) , l_26)) & p_3) , (*l_19)));
    (*l_19) = (*l_19);
    return &g_21;
}

static int32_t * func_5(int8_t  p_6, uint32_t  p_7, int64_t  p_8)
{ 
    struct S0 l_14 = {0U,0xFC};
    struct S0 *l_15 = &l_14;
    struct S0 *l_16 = &g_17;
    int32_t *l_18 = (void*)0;
    (*l_16) = ((*l_15) = l_14);
    return l_18;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_12[i][j], "g_12[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_17.f0, "g_17.f0", print_hash_value);
    transparent_crc(g_17.f1, "g_17.f1", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
