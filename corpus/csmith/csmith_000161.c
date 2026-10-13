/* Csmith 0cdc710315cf seed 161, flattened by scripts/csmith_flatten.py */
// Options:   --seed 161 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
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

union U1 {
   int32_t  f0;
   const int32_t  f1;
};

union U2 {
   int32_t  f0;
   uint32_t  f1;
   int32_t  f2;
};

union U3 {
   int8_t  f0;
};

static union U1 g_15 = {0xA062B000};
static uint64_t g_26 = 2U;
static union U3 g_27[3] = {{0},{0},{0}};
static uint64_t *g_31 = &g_26;
static uint64_t **g_30[4][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
static int32_t g_36 = 0xE83ADEB9;
static int32_t *g_35 = &g_36;
static uint64_t g_51 = 0xAC3BC5F263070B7BLL;

static int64_t  func_1(void);
static int32_t  func_11(union U1  p_12, uint8_t  p_13);
static int32_t * func_20(int64_t  p_21, union U3  p_22, union U2  p_23);

static int64_t  func_1(void)
{ 
    int32_t l_19 = 1;
    int32_t *l_37 = &g_36;
    uint32_t *l_40 = (void*)0;
    uint64_t l_41[4];
    int32_t *l_48 = &g_15.f0;
    union U1 *l_55 = &g_15;
    int i;
    for (i = 0; i < 4; i++)
        l_41[i] = 0x832397BDE5156BE8LL;
    (*l_37) = (safe_sub_func_uint8_t_u_u((safe_add_func_uint16_t_u_u((0x9AA810D21DFE4B28LL < (!(safe_mod_func_int64_t_s_s((safe_div_func_int32_t_s_s(func_11(((+(g_15 , ((safe_mul_func_int16_t_s_s((safe_unary_minus_func_uint8_t_u(0U)), g_15.f0)) == g_15.f0))) , g_15), l_19), 4294967295U)), g_36)))), 0)), (-1)));
    if (((*l_37) = (safe_add_func_int16_t_s_s(((l_41[1] |= (*l_37)) <= ((*g_35) = ((safe_add_func_uint8_t_u_u((safe_add_func_uint64_t_u_u(((*g_35) != (safe_lshift_func_int8_t_s_s((g_15.f0 , g_26), (*l_37)))), (*l_37))), g_26)) ^ 0x54764FE605A42069LL))), (*l_37)))))
    { 
        for (l_19 = 2; (l_19 >= 0); l_19 -= 1)
        { 
            int32_t **l_49 = &l_48;
            int32_t *l_50[1];
            int i;
            for (i = 0; i < 1; i++)
                l_50[i] = &g_15.f0;
            (*l_49) = l_48;
            ++g_51;
        }
    }
    else
    { 
        int16_t l_54[2];
        union U1 **l_56 = &l_55;
        int i;
        for (i = 0; i < 2; i++)
            l_54[i] = 0x546D;
        (*l_48) = (l_54[1] = (*g_35));
        (*l_56) = l_55;
    }
    return (*l_37);
}

static int32_t  func_11(union U1  p_12, uint8_t  p_13)
{ 
    int32_t *l_24 = &g_15.f0;
    uint64_t *l_25[1][3];
    union U2 l_28 = {0x67B47E0F};
    int32_t **l_34[1];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_25[i][j] = &g_26;
    }
    for (i = 0; i < 1; i++)
        l_34[i] = &l_24;
    g_35 = func_20((((((*l_24) = (l_24 == (void*)0)) | p_12.f0) ^ 9U) , g_15.f0), g_27[0], l_28);
    return p_12.f0;
}

static int32_t * func_20(int64_t  p_21, union U3  p_22, union U2  p_23)
{ 
    uint16_t l_29 = 0x4981;
    uint64_t **l_32[4] = {&g_31,&g_31,&g_31,&g_31};
    int32_t *l_33 = &g_15.f0;
    int i;
    l_32[0] = (((l_29 , g_30[1][3]) != l_32[0]) , l_32[0]);
    return l_33;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_15.f0, "g_15.f0", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_27[i].f0, "g_27[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_36, "g_36", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
