/* Csmith 0cdc710315cf seed 47, flattened by scripts/csmith_flatten.py */
// Options:   --seed 47 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint16_t  f0;
};

union U1 {
   uint8_t  f0;
   const uint64_t  f1;
   uint8_t  f2;
};

static int32_t g_2[4][2] = {{(-1),(-1)},{(-1),(-1)},{(-1),(-1)},{(-1),(-1)}};
static int32_t g_6 = 0xDFCF09CC;
static struct S0 g_29 = {0x6286};
static int16_t g_33[1] = {0xAD22};
static union U1 g_36 = {0x90};
static union U1 *g_35 = &g_36;
static const int32_t *g_39 = &g_6;

static uint32_t  func_1(void);
static int32_t * func_14(uint64_t  p_15, int32_t  p_16, struct S0  p_17);
static const struct S0 * func_20(const uint16_t  p_21, uint32_t  p_22, int32_t  p_23, const union U1  p_24);

static uint32_t  func_1(void)
{ 
    uint64_t l_9 = 0xAB5457D670012AF0LL;
    const uint32_t l_18 = 0x5EEC80F2;
    int32_t l_49[3];
    int i;
    for (i = 0; i < 3; i++)
        l_49[i] = 0x0D602FF7;
    for (g_2[2][0] = 2; (g_2[2][0] > 5); g_2[2][0] = safe_add_func_uint32_t_u_u(g_2[2][0], 2))
    { 
        int32_t *l_5 = &g_6;
        int32_t *l_7 = &g_6;
        int32_t *l_8[3][4] = {{&g_2[3][0],&g_2[3][0],&g_6,&g_2[0][0]},{&g_2[2][0],&g_2[0][1],&g_2[2][0],&g_6},{&g_2[2][0],&g_6,&g_6,&g_2[2][0]}};
        struct S0 l_19 = {0x60E7};
        int i, j;
        l_9--;
        for (l_9 = 17; (l_9 == 52); l_9 = safe_add_func_uint8_t_u_u(l_9, 8))
        { 
            int32_t **l_48 = &l_8[2][3];
            l_49[2] &= (((*l_48) = func_14(g_6, ((l_18 , &g_2[2][1]) == &g_2[2][0]), l_19)) == l_5);
            if ((*l_7))
                break;
            (*l_48) = (void*)0;
        }
    }
    return l_9;
}

static int32_t * func_14(uint64_t  p_15, int32_t  p_16, struct S0  p_17)
{ 
    uint16_t l_25[1][3];
    const union U1 l_26 = {248U};
    const struct S0 *l_42 = &g_29;
    const struct S0 **l_41 = &l_42;
    union U1 **l_43[2][3];
    int32_t *l_46 = (void*)0;
    int32_t *l_47 = &g_6;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_25[i][j] = 0xEE6D;
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
            l_43[i][j] = &g_35;
    }
    (*l_41) = func_20((g_6 , p_15), l_25[0][1], l_25[0][2], l_26);
    (*l_47) |= (((g_35 = &g_36) != &g_36) != (safe_mod_func_int8_t_s_s(3, 0x5D)));
    return l_46;
}

static const struct S0 * func_20(const uint16_t  p_21, uint32_t  p_22, int32_t  p_23, const union U1  p_24)
{ 
    const int32_t *l_38 = (void*)0;
    const struct S0 *l_40 = &g_29;
    for (p_22 = (-4); (p_22 == 31); p_22 = safe_add_func_uint64_t_u_u(p_22, 1))
    { 
        struct S0 *l_30 = &g_29;
        (*l_30) = g_29;
    }
    if (((safe_div_func_int8_t_s_s(((-10) > 0x7232), g_33[0])) ^ 0xC124380D33B947E2LL))
    { 
        const struct S0 *l_34 = (void*)0;
        return l_34;
    }
    else
    { 
        union U1 **l_37 = &g_35;
        (*l_37) = g_35;
        g_39 = l_38;
    }
    return l_40;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_2[i][j], "g_2[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_29.f0, "g_29.f0", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_33[i], "g_33[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_36.f0, "g_36.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
