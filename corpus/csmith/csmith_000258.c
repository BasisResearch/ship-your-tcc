/* Csmith 0cdc710315cf seed 258, flattened by scripts/csmith_flatten.py */
// Options:   --seed 258 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint64_t  f0;
   int16_t  f1;
};

union U1 {
   uint32_t  f0;
   uint32_t  f1;
};

union U2 {
   int64_t  f0;
};

static uint32_t g_8 = 3U;
static uint32_t g_16 = 0x858905B8;
static int32_t g_19 = 7;
static int32_t *g_18[2][1] = {{&g_19},{&g_19}};
static uint32_t *g_24 = &g_16;
static struct S0 g_35[3][3] = {{{0x5B2801F5D8E54FB1LL,0},{0x5B2801F5D8E54FB1LL,0},{0x5B2801F5D8E54FB1LL,0}},{{18446744073709551615U,-1},{18446744073709551615U,-1},{18446744073709551615U,-1}},{{0x5B2801F5D8E54FB1LL,0},{0x5B2801F5D8E54FB1LL,0},{0x5B2801F5D8E54FB1LL,0}}};
static uint16_t g_44 = 1U;
static uint16_t g_45 = 1U;
static int32_t g_50 = 8;
static int8_t g_51[1] = {0xB2};
static union U2 g_52 = {0xEC80A0586BC1F71CLL};

static union U2  func_1(void);
static uint32_t * func_9(union U2  p_10, int64_t  p_11, const uint32_t * p_12);
static union U2  func_13(uint8_t  p_14, union U1  p_15);

static union U2  func_1(void)
{ 
    uint32_t *l_6 = (void*)0;
    uint32_t *l_7 = &g_8;
    union U1 l_17 = {0x7C7008B1};
    int32_t *l_25 = &g_19;
    int32_t *l_26[1];
    int64_t l_27 = 0x0642FC308456A9E5LL;
    int16_t l_28 = 0xAC4C;
    int16_t l_29 = 6;
    uint8_t l_30 = 252U;
    int i;
    for (i = 0; i < 1; i++)
        l_26[i] = &g_19;
    (*l_25) ^= (safe_mod_func_uint32_t_u_u((0xEE6D != (((*l_7) |= 0xF8C7B5C6) != ((g_24 = func_9(func_13(g_16, l_17), l_17.f0, &g_16)) == l_7))), g_16));
    l_30--;
    for (g_8 = (-5); (g_8 <= 57); g_8 = safe_add_func_int64_t_s_s(g_8, 4))
    { 
        uint8_t l_41[1];
        int32_t l_46 = (-1);
        uint8_t *l_49[2];
        int i;
        for (i = 0; i < 1; i++)
            l_41[i] = 0x61;
        for (i = 0; i < 2; i++)
            l_49[i] = &l_30;
        for (l_27 = 0; (l_27 >= 0); l_27 -= 1)
        { 
            int16_t *l_38 = &l_29;
            uint16_t *l_42 = (void*)0;
            uint16_t *l_43 = &g_44;
            int i;
            l_46 ^= ((g_35[0][1] , (safe_lshift_func_uint32_t_u_u((((((g_45 = ((((*l_38) &= 0) >= ((*l_43) = (((safe_sub_func_int8_t_s_s(l_41[0], g_35[0][1].f0)) ^ g_19) & g_8))) == g_35[0][1].f1)) || 0xF4A0) >= 0xBBCA155E7CE46079LL) , &l_26[l_27]) == (void*)0), (*g_24)))) , 0x013F2C8C);
        }
        l_46 &= (safe_mod_func_uint8_t_u_u(l_41[0], (g_51[0] &= (g_50 = g_45))));
    }
    return g_52;
}

static uint32_t * func_9(union U2  p_10, int64_t  p_11, const uint32_t * p_12)
{ 
    uint32_t *l_23 = &g_16;
    return l_23;
}

static union U2  func_13(uint8_t  p_14, union U1  p_15)
{ 
    int32_t **l_20 = (void*)0;
    int32_t **l_21[3][4] = {{&g_18[0][0],&g_18[0][0],&g_18[0][0],&g_18[0][0]},{&g_18[0][0],&g_18[0][0],&g_18[0][0],&g_18[0][0]},{&g_18[0][0],&g_18[0][0],&g_18[0][0],&g_18[0][0]}};
    union U2 l_22[3][2] = {{{-1},{-1}},{{-1},{-1}},{{-1},{-1}}};
    int i, j;
    g_18[0][0] = g_18[0][0];
    return l_22[1][1];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_35[i][j].f0, "g_35[i][j].f0", print_hash_value);
            transparent_crc(g_35[i][j].f1, "g_35[i][j].f1", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_51[i], "g_51[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_52.f0, "g_52.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
