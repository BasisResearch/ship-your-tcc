/* Csmith 0cdc710315cf seed 113, flattened by scripts/csmith_flatten.py */
// Options:   --seed 113 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
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
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static int32_t
safe_unary_minus_func_int32_t_s(int32_t si )
{
  return
    (si==(-2147483647-1)) ?
    ((si)) :
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   int16_t  f1;
};

struct S1 {
   int64_t  f0;
};

struct S3 {
   struct S1  f0;
   uint64_t  f1;
};

struct S4 {
   uint64_t  f0;
   const struct S3  f1;
   int16_t  f2;
};

union U5 {
   int16_t  f0;
   const int8_t  f1;
};

union U6 {
   int64_t  f0;
};

static int32_t g_4 = (-10);
static int32_t g_6 = 0x22E974A5;
static uint8_t g_22[3] = {255U,255U,255U};
static int32_t g_24 = 9;
static int32_t *g_30 = &g_4;
static int32_t **g_29[1] = {&g_30};
static const struct S3 g_31[4][3] = {{{{0xEC76E8D2CA8E1F91LL},18446744073709551606U},{{0xEC76E8D2CA8E1F91LL},18446744073709551606U},{{0xEC76E8D2CA8E1F91LL},18446744073709551606U}},{{{1},5U},{{0},0xFEFD6E511AFD208DLL},{{1},5U}},{{{0xEC76E8D2CA8E1F91LL},18446744073709551606U},{{0xEC76E8D2CA8E1F91LL},18446744073709551606U},{{0xEC76E8D2CA8E1F91LL},18446744073709551606U}},{{{1},5U},{{0},0xFEFD6E511AFD208DLL},{{1},5U}}};
static int64_t g_41 = (-8);
static int32_t g_44 = 0x399D6DB7;
static struct S4 g_49 = {1U,{{8},0x993103D53AF4B502LL},0x4801};
static struct S1 g_52 = {7};
static const int64_t *g_60 = &g_41;
static const int64_t **g_59 = &g_60;
static union U5 g_61 = {0x3C27};
static union U6 g_63 = {0};

static union U5  func_1(void);
static int32_t  func_8(const int32_t * p_9);
static int32_t * func_10(int32_t  p_11, int32_t * p_12, int32_t * p_13, uint32_t  p_14);

static union U5  func_1(void)
{ 
    int8_t l_2 = 0;
    const uint16_t l_3 = 0x28EA;
    int32_t *l_5 = &g_6;
    int8_t l_42 = 1;
    struct S1 l_48 = {0x71E1D77F595C9828LL};
    union U5 l_58 = {-1};
    union U6 *l_66 = &g_63;
    (*l_5) ^= (0x95 <= ((0xCB143E95AAD8530DLL ^ (l_2 , l_3)) <= g_4));
    if ((l_5 != &g_6))
    { 
        struct S0 l_7[2] = {{1,0xB90F},{1,0xB90F}};
        int32_t l_17 = 0;
        int i;
        for (g_4 = 0; (g_4 <= 1); g_4 += 1)
        { 
            int32_t *l_15[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
            int i;
            for (g_6 = 1; (g_6 >= 0); g_6 -= 1)
            { 
                int32_t **l_16[2];
                uint32_t l_28 = 0xAD8A0F83;
                int i;
                for (i = 0; i < 2; i++)
                    l_16[i] = (void*)0;
                l_28 = func_8(func_10(((void*)0 != l_15[2]), (l_5 = &g_6), l_15[2], l_17));
            }
        }
        g_29[0] = g_29[0];
    }
    else
    { 
        uint64_t l_39 = 0x9B7B3FA8726ABE92LL;
        int64_t *l_40 = &g_41;
        int32_t *l_43 = &g_44;
        int32_t **l_45 = &l_5;
        (*g_30) = ((&g_30 != (g_31[1][2] , (void*)0)) & 0x4B3BEBDA3FA0B5BCLL);
        (*l_43) |= (~((safe_add_func_uint64_t_u_u(((safe_lshift_func_uint32_t_u_u(((safe_add_func_int64_t_s_s(((*l_40) = (0x5AD8EDCE & ((*l_5) <= (((g_4 || g_22[2]) | l_39) == g_31[1][2].f1)))), g_24)) , g_31[1][2].f0.f0), g_4)) > (*l_5)), l_39)) , l_42));
        (*l_45) = l_43;
    }
    if ((safe_sub_func_int8_t_s_s(((*l_5) == (l_48 , 3U)), g_41)))
    { 
        int32_t *l_50 = &g_24;
        (*l_50) ^= ((*g_30) = (g_49 , (*l_5)));
        for (g_6 = 0; (g_6 <= 2); g_6 += 1)
        { 
            int64_t * const *l_51 = (void*)0;
            int i;
            l_51 = (void*)0;
            g_52 = g_49.f1.f0;
            if ((((((safe_lshift_func_uint16_t_u_s(((safe_unary_minus_func_int32_t_s(((safe_div_func_int8_t_s_s(((9U || g_22[g_6]) == (l_58 , (*l_50))), g_22[1])) , (*l_5)))) && 0x2431), 10)) <= g_22[g_6]) & 0x0C75) ^ g_22[2]) < 0x8A24))
            { 
                g_59 = g_59;
            }
            else
            { 
                if ((*l_50))
                    break;
                return g_61;
            }
        }
    }
    else
    { 
        union U6 *l_62[2][3];
        union U6 *l_65 = &g_63;
        union U6 **l_64[3][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 3; j++)
                l_62[i][j] = &g_63;
        }
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_64[i][j] = &l_65;
        }
        l_66 = (l_62[1][2] = l_62[1][2]);
    }
    return g_61;
}

static int32_t  func_8(const int32_t * p_9)
{ 
    int32_t *l_23 = &g_24;
    int32_t *l_26[2][4];
    int32_t **l_25 = &l_26[0][1];
    int32_t l_27 = 0x7F20BAB1;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
            l_26[i][j] = (void*)0;
    }
    (*l_25) = func_10(((*l_23) ^= g_22[2]), l_23, ((*l_25) = &g_6), l_27);
    return (*p_9);
}

static int32_t * func_10(int32_t  p_11, int32_t * p_12, int32_t * p_13, uint32_t  p_14)
{ 
    g_22[2] &= (safe_mul_func_int8_t_s_s((0x90 > (((safe_add_func_uint8_t_u_u(0xA0, 0xCC)) ^ g_6) > g_4)), p_11));
    return &g_6;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_22[i], "g_22[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_24, "g_24", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_31[i][j].f0.f0, "g_31[i][j].f0.f0", print_hash_value);
            transparent_crc(g_31[i][j].f1, "g_31[i][j].f1", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_49.f0, "g_49.f0", print_hash_value);
    transparent_crc(g_49.f1.f0.f0, "g_49.f1.f0.f0", print_hash_value);
    transparent_crc(g_49.f1.f1, "g_49.f1.f1", print_hash_value);
    transparent_crc(g_49.f2, "g_49.f2", print_hash_value);
    transparent_crc(g_52.f0, "g_52.f0", print_hash_value);
    transparent_crc(g_61.f0, "g_61.f0", print_hash_value);
    transparent_crc(g_63.f0, "g_63.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
