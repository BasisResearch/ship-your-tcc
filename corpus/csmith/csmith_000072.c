/* Csmith 0cdc710315cf seed 72, flattened by scripts/csmith_flatten.py */
// Options:   --seed 72 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   const uint64_t  f0;
   int8_t  f1;
   uint32_t  f2;
};

union U1 {
   int32_t  f0;
   uint8_t  f1;
};

static int8_t g_8 = 0xAC;
static int32_t *g_17 = (void*)0;
static int16_t g_25 = 0xB5E4;
static int64_t g_28[2] = {0x0B0881DDA8DB8846LL,0x0B0881DDA8DB8846LL};
static uint32_t g_36 = 0x653649D9;
static uint32_t g_39 = 0x63E3202F;
static uint16_t g_44 = 0x3F8C;
static const int32_t g_52 = 0xCFD24BD9;
static uint32_t g_53 = 0x705E7732;
static union U0 g_55 = {18446744073709551615U};
static int32_t g_58 = 0x8D7774CA;

static uint8_t  func_1(void);
static uint32_t  func_4(int16_t  p_5, union U1  p_6, int64_t  p_7);
static union U1  func_9(int64_t  p_10, int32_t  p_11, int64_t  p_12, int16_t  p_13);

static uint8_t  func_1(void)
{ 
    int16_t l_14 = 0x26EC;
    int32_t *l_57 = &g_58;
    (*l_57) = (safe_mod_func_int8_t_s_s(((func_4(g_8, func_9(l_14, g_8, l_14, g_8), l_14) ^ l_14) | l_14), g_55.f0));
    return (*l_57);
}

static uint32_t  func_4(int16_t  p_5, union U1  p_6, int64_t  p_7)
{ 
    int8_t l_23 = 1;
    int16_t *l_24 = &g_25;
    uint16_t l_26 = 65535U;
    int32_t *l_27[2];
    int8_t l_31 = (-10);
    int i;
    for (i = 0; i < 2; i++)
        l_27[i] = (void*)0;
    g_28[1] = (safe_add_func_int16_t_s_s((~p_6.f0), ((((*l_24) = (l_23 = g_8)) >= (((0x2DB8 != l_26) , &g_17) == &g_17)) , 0)));
    for (l_23 = 1; (l_23 >= 0); l_23 -= 1)
    { 
        uint8_t l_37 = 246U;
        int32_t l_38[2];
        int i;
        for (i = 0; i < 2; i++)
            l_38[i] = 0xA76A447D;
        for (p_5 = 0; (p_5 <= 1); p_5 += 1)
        { 
            int i;
            p_6.f0 = (g_28[p_5] > 0x63);
            for (g_8 = 0; (g_8 <= 1); g_8 += 1)
            { 
                l_37 &= ((p_6.f0 = ((safe_div_func_uint64_t_u_u(l_31, (((safe_mul_func_uint64_t_u_u((g_36 ^= (safe_div_func_int64_t_s_s((g_28[1] < p_5), 0x711F64DB2A93F045LL))), g_28[1])) < 7U) , (-9)))) , g_8)) || 0x64E48C0C);
            }
            g_39++;
        }
        for (p_6.f1 = 0; (p_6.f1 <= 1); p_6.f1 += 1)
        { 
            int8_t l_42 = 0x9B;
            int i;
            l_38[1] = l_42;
            for (g_8 = 0; (g_8 <= 1); g_8 += 1)
            { 
                uint16_t l_43 = 0x78BE;
                int32_t l_45 = (-1);
                l_45 ^= (g_44 = ((l_43 = 0) & (-1)));
            }
        }
        for (l_37 = 0; (l_37 <= 1); l_37 += 1)
        { 
            const int32_t **l_49 = (void*)0;
            const int32_t *l_51 = &g_52;
            const int32_t **l_50 = &l_51;
            union U0 *l_54 = &g_55;
            union U0 **l_56 = &l_54;
            int i;
            g_53 = (func_9(g_8, (safe_add_func_int8_t_s_s(((!(&l_38[1] == ((*l_50) = g_17))) ^ p_6.f1), p_7)), g_36, g_36) , p_7);
            (*l_56) = l_54;
            (*l_50) = (l_27[l_23] = (p_5 , &l_38[1]));
        }
    }
    return p_7;
}

static union U1  func_9(int64_t  p_10, int32_t  p_11, int64_t  p_12, int16_t  p_13)
{ 
    union U1 l_19 = {0};
    for (p_12 = 0; (p_12 <= 13); p_12 = safe_add_func_uint8_t_u_u(p_12, 2))
    { 
        int32_t **l_18[1];
        int i;
        for (i = 0; i < 1; i++)
            l_18[i] = &g_17;
        g_17 = g_17;
    }
    return l_19;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_28[i], "g_28[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_36, "g_36", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    transparent_crc(g_55.f0, "g_55.f0", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
