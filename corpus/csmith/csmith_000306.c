/* Csmith 0cdc710315cf seed 306, flattened by scripts/csmith_flatten.py */
// Options:   --seed 306 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   const int32_t  f1;
};

union U1 {
   int64_t  f0;
};

static uint16_t g_4 = 65533U;
static int32_t g_11 = (-9);
static uint8_t g_13 = 0xB9;
static int64_t g_22 = (-10);
static int32_t *g_26 = (void*)0;
static uint32_t g_54 = 0x3DB47DC4;
static int32_t g_56 = 0xD17F4926;
static union U1 g_61 = {0x4FCBDFAC86E89BE3LL};

static const union U1  func_1(void);
static union U1  func_2(uint64_t  p_3);
static int32_t * func_5(int32_t * const  p_6, const int32_t * p_7);

static const union U1  func_1(void)
{ 
    uint8_t l_28[3];
    struct S0 l_29 = {1,-8};
    int32_t *l_35 = &g_11;
    int16_t l_49 = 0xC8BB;
    int i;
    for (i = 0; i < 3; i++)
        l_28[i] = 0U;
    if (((func_2(g_4) , l_28[1]) & (-8)))
    { 
        int32_t *l_30 = &g_11;
        int16_t *l_52 = &l_49;
        uint32_t *l_53[3][3] = {{&g_54,&g_54,&g_54},{&g_54,&g_54,&g_54},{&g_54,&g_54,&g_54}};
        int32_t *l_55 = &g_56;
        int i, j;
        (*l_30) = (g_13 & (1 & (l_29 , l_28[0])));
        for (g_13 = 0; (g_13 > 24); g_13++)
        { 
            uint32_t l_40 = 0x6DAD8BE7;
            for (g_4 = 0; (g_4 != 12); ++g_4)
            { 
                int32_t *l_36 = &g_11;
                int32_t *l_37 = (void*)0;
                int32_t *l_38 = &g_11;
                int32_t *l_39[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_39[i] = (void*)0;
                l_36 = l_35;
                --l_40;
            }
        }
        (*l_55) &= (safe_add_func_uint8_t_u_u((safe_mul_func_uint32_t_u_u((g_54 ^= (safe_div_func_int16_t_s_s(((*l_52) = ((l_49 < (*l_30)) < (safe_lshift_func_int32_t_s_s((&g_26 == &g_26), 3)))), g_11))), (*l_35))), (*l_30)));
    }
    else
    { 
        union U1 *l_60 = &g_61;
        union U1 **l_62 = &l_60;
        for (g_13 = 3; (g_13 != 38); g_13 = safe_add_func_int64_t_s_s(g_13, 7))
        { 
            int32_t *l_59 = &g_56;
            (*l_59) |= (*l_35);
            (*l_35) = g_22;
        }
        (*l_62) = l_60;
    }
    return g_61;
}

static union U1  func_2(uint64_t  p_3)
{ 
    int32_t *l_10[2][4] = {{&g_11,&g_11,&g_11,&g_11},{&g_11,&g_11,(void*)0,&g_11}};
    uint8_t *l_12 = &g_13;
    int32_t * const l_14 = &g_11;
    int32_t **l_25 = (void*)0;
    union U1 l_27[2] = {{0xCBBBC254E7F13BCFLL},{0xCBBBC254E7F13BCFLL}};
    int i, j;
    g_26 = func_5((((safe_div_func_uint8_t_u_u(((*l_12) &= ((l_10[0][0] != &g_11) | (l_10[0][0] == &g_11))), g_11)) , g_13) , l_14), &g_11);
    return l_27[0];
}

static int32_t * func_5(int32_t * const  p_6, const int32_t * p_7)
{ 
    int32_t *l_24[3][4] = {{&g_11,&g_11,&g_11,&g_11},{&g_11,&g_11,&g_11,&g_11},{&g_11,&g_11,&g_11,&g_11}};
    int i, j;
    for (g_4 = 0; (g_4 > 37); g_4 = safe_add_func_uint16_t_u_u(g_4, 1))
    { 
        const uint16_t l_19[4] = {65532U,65532U,65532U,65532U};
        int i;
        for (g_11 = 0; (g_11 > (-20)); --g_11)
        { 
            int32_t *l_23 = (void*)0;
            if (l_19[0])
                break;
            g_22 = ((safe_div_func_uint8_t_u_u((((-8) < (0xCCED & 7)) != 0x0578CCA6), g_13)) , (*p_6));
            return l_23;
        }
    }
    return l_24[2][0];
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_11, "g_11", print_hash_value);
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_54, "g_54", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_61.f0, "g_61.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
