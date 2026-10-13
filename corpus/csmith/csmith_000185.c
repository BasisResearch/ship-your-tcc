/* Csmith 0cdc710315cf seed 185, flattened by scripts/csmith_flatten.py */
// Options:   --seed 185 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int32_t  f0;
   uint16_t  f1;
};

union U1 {
   struct S0  f0;
   struct S0  f1;
};

static const struct S0 g_5 = {0x4FC2285F,0xAFE3};
static union U1 g_8 = {{1,0xB8E9}};
static union U1 g_15 = {{-1,0x2466}};
static union U1 *g_14 = &g_15;
static uint32_t g_28[4][4] = {{0x69B41DBC,0x1AF6658D,0x1AF6658D,0x69B41DBC},{0x69B41DBC,0x1AF6658D,0x1AF6658D,0x69B41DBC},{0x69B41DBC,0x1AF6658D,0x1AF6658D,0x69B41DBC},{0x69B41DBC,0x1AF6658D,0x1AF6658D,0x69B41DBC}};
static int32_t g_30 = 0;
static int16_t g_42 = (-1);

static int8_t  func_1(void);
static uint16_t  func_2(const struct S0  p_3, int32_t  p_4);
static uint16_t  func_6(int16_t  p_7);

static int8_t  func_1(void)
{ 
    int16_t l_9 = 0;
    uint16_t *l_56 = &g_8.f0.f1;
    uint64_t l_57 = 0xF1DFBA308D5B9499LL;
    int32_t *l_74 = &g_15.f1.f0;
    int32_t *l_75 = &g_30;
    (*l_75) = ((*l_74) = ((func_2(g_5, (g_5.f1 | (((((*l_56) = func_6((g_8 , l_9))) , g_28[1][2]) != l_9) <= l_57))) | 0xB6FE) != g_5.f0));
    return g_28[1][2];
}

static uint16_t  func_2(const struct S0  p_3, int32_t  p_4)
{ 
    uint64_t l_62[2][3];
    int32_t *l_69 = &g_8.f1.f0;
    int32_t *l_70 = (void*)0;
    int32_t *l_71[3];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
            l_62[i][j] = 18446744073709551609U;
    }
    for (i = 0; i < 3; i++)
        l_71[i] = &g_15.f0.f0;
    for (g_15.f1.f0 = 0; (g_15.f1.f0 == 13); g_15.f1.f0++)
    { 
        int32_t *l_60 = &g_30;
        int32_t *l_61[4][4] = {{&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0},{&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0},{&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0},{&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0,&g_8.f1.f0}};
        int i, j;
        l_62[0][0]--;
    }
    g_15.f0.f0 ^= ((*l_69) = (safe_div_func_uint8_t_u_u(p_4, (safe_div_func_uint32_t_u_u((g_5.f0 != 0x97E3), l_62[0][0])))));
    for (g_8.f0.f0 = 0; (g_8.f0.f0 <= 2); g_8.f0.f0 += 1)
    { 
        const struct S0 *l_73 = (void*)0;
        const struct S0 **l_72 = &l_73;
        (*l_72) = &g_5;
        for (g_15.f0.f0 = 1; (g_15.f0.f0 >= 0); g_15.f0.f0 -= 1)
        { 
            int i, j;
            return l_62[g_15.f0.f0][g_8.f0.f0];
        }
    }
    return g_8.f0.f0;
}

static uint16_t  func_6(int16_t  p_7)
{ 
    uint8_t l_17 = 0xF5;
    int32_t l_40 = 3;
    int32_t *l_44 = (void*)0;
    int32_t *l_45 = &g_8.f1.f0;
    int32_t l_46 = 0x3668F781;
    int32_t l_47 = 0xC737ECE5;
    int32_t *l_48 = &l_40;
    int32_t *l_49 = &g_30;
    int32_t l_50 = 0x1C210293;
    int32_t *l_51 = (void*)0;
    int32_t *l_52[4];
    uint64_t l_53 = 0x28C330C202EAB385LL;
    int i;
    for (i = 0; i < 4; i++)
        l_52[i] = &l_47;
    for (g_8.f1.f0 = 0; (g_8.f1.f0 > (-29)); g_8.f1.f0 = safe_sub_func_int32_t_s_s(g_8.f1.f0, 6))
    { 
        union U1 *l_12 = &g_8;
        union U1 **l_13 = (void*)0;
        int32_t *l_16[2][2];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
                l_16[i][j] = &g_15.f0.f0;
        }
        g_14 = l_12;
        l_17 ^= (-1);
    }
    for (g_15.f1.f0 = 5; (g_15.f1.f0 >= (-11)); g_15.f1.f0 = safe_sub_func_uint32_t_u_u(g_15.f1.f0, 5))
    { 
        uint16_t l_25 = 0x6CC7;
        int32_t l_31 = (-8);
        for (g_8.f0.f1 = 22; (g_8.f0.f1 > 33); ++g_8.f0.f1)
        { 
            uint16_t *l_26 = (void*)0;
            uint16_t *l_27[1];
            int32_t *l_29[4] = {&g_30,&g_30,&g_30,&g_30};
            int i;
            for (i = 0; i < 1; i++)
                l_27[i] = &l_25;
            l_31 = (g_15.f0.f1 < (g_28[1][2] = (((safe_mod_func_uint32_t_u_u((safe_unary_minus_func_uint16_t_u(((l_25 <= (0x0F3BD791F9E6A012LL >= g_5.f0)) < 0xF4))), p_7)) , (void*)0) == (void*)0)));
            if ((((&g_14 != (void*)0) != 0U) < p_7))
            { 
                return g_28[1][2];
            }
            else
            { 
                int16_t *l_41 = &g_42;
                int32_t **l_43 = &l_29[3];
                (*l_43) = (((safe_mod_func_int64_t_s_s((safe_lshift_func_int16_t_s_s(((*l_41) |= (safe_mod_func_uint32_t_u_u(l_17, (safe_sub_func_int64_t_s_s((l_40 = (0xFF ^ (((&g_15 == (void*)0) , g_5.f1) & 0))), l_17))))), g_5.f1)), l_31)) ^ p_7) , (void*)0);
            }
            if (g_8.f0.f0)
                continue;
        }
    }
    l_53++;
    return g_5.f0;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5.f0, "g_5.f0", print_hash_value);
    transparent_crc(g_5.f1, "g_5.f1", print_hash_value);
    transparent_crc(g_8.f0.f0, "g_8.f0.f0", print_hash_value);
    transparent_crc(g_8.f0.f1, "g_8.f0.f1", print_hash_value);
    transparent_crc(g_15.f0.f0, "g_15.f0.f0", print_hash_value);
    transparent_crc(g_15.f0.f1, "g_15.f0.f1", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_28[i][j], "g_28[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
