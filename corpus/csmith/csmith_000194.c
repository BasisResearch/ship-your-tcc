/* Csmith 0cdc710315cf seed 194, flattened by scripts/csmith_flatten.py */
// Options:   --seed 194 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
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
static uint32_t
safe_unary_minus_func_uint32_t_u(uint32_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 0x85F70AD6;
static int8_t g_21 = 0x64;
static int32_t g_34 = 0xA06C0F1A;
static uint32_t g_50 = 0xB2DE47E5;
static uint32_t g_54[4] = {0x8431BB57,0x8431BB57,0x8431BB57,0x8431BB57};
static uint32_t g_56 = 0x25AD92ED;
static uint8_t g_72 = 0U;

static int8_t  func_1(void);
static uint64_t  func_10(int32_t * p_11, int8_t  p_12, uint64_t  p_13, int32_t * const  p_14, int32_t * p_15);
static int32_t * func_23(int32_t  p_24, uint8_t  p_25);

static int8_t  func_1(void)
{ 
    uint32_t l_5 = 0x8C3AD3B3;
    int64_t l_6[3];
    int32_t * const l_22[1][4] = {{&g_2,&g_2,&g_2,&g_2}};
    int i, j;
    for (i = 0; i < 3; i++)
        l_6[i] = 0xB770A77C8748AFA2LL;
    for (g_2 = (-24); (g_2 > 0); g_2++)
    { 
        l_5 &= g_2;
    }
    for (l_5 = 0; (l_5 <= 2); l_5 += 1)
    { 
        int32_t *l_7 = &g_2;
        int32_t l_73 = 0x3ECEF36A;
        (*l_7) = 0;
        for (g_2 = 0; (g_2 <= 2); g_2 += 1)
        { 
            int32_t *l_16 = (void*)0;
            int8_t *l_19 = (void*)0;
            int8_t *l_20 = &g_21;
            int i;
            l_73 = (safe_add_func_uint64_t_u_u(func_10(l_16, l_6[l_5], ((safe_mul_func_int8_t_s_s(((*l_20) = (g_2 ^ 3)), g_2)) >= 18446744073709551610U), l_22[0][1], &g_2), g_2));
            return g_2;
        }
    }
    return g_50;
}

static uint64_t  func_10(int32_t * p_11, int8_t  p_12, uint64_t  p_13, int32_t * const  p_14, int32_t * p_15)
{ 
    int64_t l_30[2][3] = {{0xF1540EE1D4595A2ELL,0xF1540EE1D4595A2ELL,0xF1540EE1D4595A2ELL},{0x1978E60D743CCC09LL,0x1978E60D743CCC09LL,0x1978E60D743CCC09LL}};
    int32_t **l_35 = (void*)0;
    int32_t *l_37[2][1];
    int32_t **l_36 = &l_37[0][0];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_37[i][j] = &g_2;
    }
    if (((((*l_36) = func_23(((p_13 , (safe_mul_func_uint64_t_u_u((safe_add_func_int16_t_s_s(l_30[1][1], (safe_sub_func_int16_t_s_s((-1), g_21)))), p_12))) & g_2), l_30[0][0])) != p_14) <= p_12))
    { 
        (*l_36) = func_23(((safe_add_func_uint32_t_u_u((safe_add_func_int32_t_s_s(g_2, ((&p_14 == &l_37[1][0]) | p_13))), g_34)) | g_21), p_12);
    }
    else
    { 
        uint16_t l_51 = 0x74E6;
        for (g_34 = 0; (g_34 <= 1); g_34 += 1)
        { 
            int64_t l_42[2];
            int i;
            for (i = 0; i < 2; i++)
                l_42[i] = 0x3D35555D5DF5B04CLL;
            for (p_12 = 0; p_12 < 2; p_12 = p_12 + 1)
            {
                for (p_13 = 0; p_13 < 1; p_13 = p_13 + 1)
                {
                    l_37[p_12][p_13] = (void*)0;
                }
            }
            return l_42[0];
        }
        for (g_34 = (-28); (g_34 >= 26); g_34 = safe_add_func_int16_t_s_s(g_34, 9))
        { 
            int64_t *l_47 = &l_30[0][2];
            uint32_t *l_49 = &g_50;
            int32_t **l_55 = &l_37[0][0];
            if ((safe_sub_func_int64_t_s_s(((*l_47) = (-8)), (safe_unary_minus_func_uint32_t_u(((*l_49) &= g_2))))))
            { 
                ++l_51;
                g_54[1] = 0x9D7FCCEA;
            }
            else
            { 
                g_56 &= (g_54[1] | (l_55 == &p_14));
            }
        }
        for (g_50 = 10; (g_50 > 30); g_50 = safe_add_func_int8_t_s_s(g_50, 2))
        { 
            uint32_t l_64 = 0U;
            int32_t l_67 = 1;
            int32_t l_68 = 1;
            uint16_t l_69 = 4U;
            for (p_12 = (-11); (p_12 <= 28); p_12 = safe_add_func_int8_t_s_s(p_12, 1))
            { 
                int16_t l_63 = 0;
                l_64 = (safe_sub_func_int16_t_s_s(l_63, 6));
            }
            for (p_13 = 0; (p_13 >= 35); ++p_13)
            { 
                --l_69;
            }
        }
    }
    g_72 = (g_34 = (p_12 <= 0x53CD));
    return p_12;
}

static int32_t * func_23(int32_t  p_24, uint8_t  p_25)
{ 
    int32_t *l_33 = &g_34;
    (*l_33) = (g_2 , ((void*)0 != &p_24));
    return &g_34;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_34, "g_34", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_54[i], "g_54[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_72, "g_72", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
