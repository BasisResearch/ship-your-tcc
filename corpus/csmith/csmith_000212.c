/* Csmith 0cdc710315cf seed 212, flattened by scripts/csmith_flatten.py */
// Options:   --seed 212 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
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
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint16_t  f0;
   int64_t  f1;
   int16_t  f2;
};

struct S1 {
   int8_t  f0;
   const uint32_t  f1;
};

struct S2 {
   uint16_t  f0;
   uint16_t  f1;
};

static uint32_t g_5 = 0x88E17B84;
static int32_t g_7 = 0x20B963AB;
static int16_t g_9 = 0x23A2;
static struct S1 g_23 = {0x51,1U};
static int32_t g_27[1] = {0x2D38CA8C};
static struct S0 g_56 = {1U,-3,0xCF83};
static uint8_t g_72 = 5U;
static struct S1 g_77 = {6,0x2180DBEA};
static int32_t *g_78 = &g_7;

static struct S0  func_1(void);
static int32_t * func_10(int32_t  p_11, int16_t * const  p_12, int16_t * p_13);
static int32_t * func_16(int32_t * p_17, int32_t * p_18, struct S1  p_19);

static struct S0  func_1(void)
{ 
    uint16_t l_2[4];
    struct S2 l_33[1][3] = {{{0x3418,0xB0BA},{0x3418,0xB0BA},{0x3418,0xB0BA}}};
    int32_t *l_45[1][4];
    int32_t **l_44[2];
    int32_t **l_48[2];
    int32_t l_57 = 0x930A8756;
    uint8_t *l_71 = &g_72;
    int16_t *l_75[3][2] = {{&g_56.f2,&g_56.f2},{&g_56.f2,&g_56.f2},{&g_56.f2,&g_56.f2}};
    int32_t *l_76[2][3] = {{&l_57,&l_57,&l_57},{&g_27[0],&g_27[0],&g_27[0]}};
    struct S0 l_79 = {65535U,-8,0xBF39};
    int i, j;
    for (i = 0; i < 4; i++)
        l_2[i] = 0x91C5;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_45[i][j] = (void*)0;
    }
    for (i = 0; i < 2; i++)
        l_44[i] = &l_45[0][1];
    for (i = 0; i < 2; i++)
        l_48[i] = &l_45[0][3];
    if (l_2[3])
    { 
        uint16_t l_6 = 0xAAED;
        int16_t *l_8 = &g_9;
        struct S1 l_50 = {0x26,9U};
        if ((safe_mul_func_int64_t_s_s(((g_5 , ((*l_8) = ((g_7 |= l_6) < l_6))) < g_5), (-1))))
        { 
            int32_t *l_32 = &g_7;
            int32_t **l_31 = &l_32;
            (*l_31) = func_10((safe_div_func_int64_t_s_s(0xAD5BEFA1E1C5A3D7LL, (-1))), &g_9, &g_9);
            l_33[0][1] = ((1 == (l_6 == ((*l_8) |= g_27[0]))) , l_33[0][0]);
        }
        else
        { 
            int32_t **l_46 = &l_45[0][1];
            int32_t **l_47 = &l_45[0][2];
            const int32_t **l_49 = (void*)0;
            uint8_t l_51 = 0xAB;
            g_27[0] = (safe_sub_func_uint16_t_u_u(l_6, (((l_6 , (((safe_sub_func_uint8_t_u_u(((safe_lshift_func_int16_t_s_s(((((safe_add_func_int16_t_s_s((safe_mod_func_int64_t_s_s(((((((l_48[0] = (l_47 = (l_46 = l_44[0]))) != l_49) | g_27[0]) >= l_6) > g_27[0]) | 0x28C4F412), g_27[0])), 0xE8BD)) , l_50) , g_9) ^ l_51), g_5)) < g_9), l_50.f0)) && g_27[0]) > (-1))) && 0x9633) , 65535U)));
            g_7 = ((g_9 != (safe_div_func_int16_t_s_s((safe_div_func_uint8_t_u_u((g_23.f0 == (((l_50.f0 | (-1)) , l_50.f0) >= g_23.f1)), l_6)), g_23.f1))) <= g_5);
            return g_56;
        }
    }
    else
    { 
        int64_t l_58 = 0x7386F73136105796LL;
        int32_t l_59 = 0xF00AFC62;
        int32_t l_60 = 1;
        uint64_t l_61 = 18446744073709551615U;
        l_61--;
    }
    g_78 = func_16(((g_56.f2 = (safe_mul_func_int64_t_s_s((safe_rshift_func_uint16_t_u_u(0x1802, 11)), (safe_sub_func_uint64_t_u_u(g_56.f1, (((safe_unary_minus_func_uint8_t_u(((*l_71)++))) != g_27[0]) & g_9)))))) , l_76[1][0]), &g_27[0], g_77);
    return l_79;
}

static int32_t * func_10(int32_t  p_11, int16_t * const  p_12, int16_t * p_13)
{ 
    int32_t *l_21 = &g_7;
    int32_t **l_20 = &l_21;
    int32_t *l_22 = &g_7;
    (*l_20) = func_16(&g_7, (l_22 = ((*l_20) = &g_7)), g_23);
    return &g_27[0];
}

static int32_t * func_16(int32_t * p_17, int32_t * p_18, struct S1  p_19)
{ 
    uint64_t l_28 = 1U;
    for (g_9 = (-11); (g_9 > 8); g_9 = safe_add_func_int64_t_s_s(g_9, 2))
    { 
        int32_t *l_26[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
        int i;
        l_28--;
    }
    return &g_7;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_23.f0, "g_23.f0", print_hash_value);
    transparent_crc(g_23.f1, "g_23.f1", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_27[i], "g_27[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_56.f0, "g_56.f0", print_hash_value);
    transparent_crc(g_56.f1, "g_56.f1", print_hash_value);
    transparent_crc(g_56.f2, "g_56.f2", print_hash_value);
    transparent_crc(g_72, "g_72", print_hash_value);
    transparent_crc(g_77.f0, "g_77.f0", print_hash_value);
    transparent_crc(g_77.f1, "g_77.f1", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
