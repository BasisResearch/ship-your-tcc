/* Csmith 0cdc710315cf seed 148, flattened by scripts/csmith_flatten.py */
// Options:   --seed 148 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static uint64_t g_9 = 0xA6E6F7FBF2121F90LL;
static uint64_t g_13 = 18446744073709551608U;
static int16_t g_15[3] = {0x3C6F,0x3C6F,0x3C6F};
static int32_t g_18 = 0;
static int32_t g_20 = 0x9B27FA3D;
static uint64_t *g_34 = &g_9;
static uint64_t **g_33[1][4] = {{&g_34,&g_34,&g_34,&g_34}};

static uint64_t  func_1(void);
static uint64_t  func_2(uint8_t  p_3, const uint64_t  p_4, uint16_t  p_5, uint32_t  p_6, int32_t  p_7);
static uint64_t ** func_21(int32_t * p_22, uint8_t  p_23);

static uint64_t  func_1(void)
{ 
    uint64_t *l_8 = &g_9;
    int32_t l_14 = 0x32CF415D;
    int32_t *l_35 = &l_14;
    int32_t *l_36 = &g_18;
    int32_t **l_37 = &l_36;
    (*l_35) = (((func_2(((((++(*l_8)) , (safe_unary_minus_func_uint8_t_u(g_9))) == g_13) <= l_14), g_13, g_13, g_13, l_14) || 0xF57F54AB10079D62LL) , 0xB1C2FBF3) & l_14);
    (*l_37) = l_36;
    return (*l_36);
}

static uint64_t  func_2(uint8_t  p_3, const uint64_t  p_4, uint16_t  p_5, uint32_t  p_6, int32_t  p_7)
{ 
    int32_t *l_24 = &g_20;
    for (g_9 = 0; (g_9 <= 2); g_9 += 1)
    { 
        int32_t *l_17 = &g_18;
        int32_t *l_19[3][2];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 2; j++)
                l_19[i][j] = &g_20;
        }
        g_20 = ((*l_17) = (+(-10)));
        for (p_5 = 0; (p_5 <= 2); p_5 += 1)
        { 
            int i;
            for (g_18 = 1; (g_18 >= 0); g_18 -= 1)
            { 
                int i, j;
                p_7 = g_15[g_18];
                g_20 = 0x53B1DAB5;
                return g_20;
            }
            g_33[0][0] = func_21(l_24, (safe_mod_func_int16_t_s_s((g_15[p_5] == (safe_rshift_func_int8_t_s_s((0xF758 || (*l_24)), g_13))), g_13)));
        }
    }
    return p_7;
}

static uint64_t ** func_21(int32_t * p_22, uint8_t  p_23)
{ 
    uint64_t *l_30[1];
    uint64_t *l_32[2][4] = {{&g_9,&g_9,&g_9,&g_9},{&g_9,&g_9,&g_9,&g_9}};
    uint64_t **l_31 = &l_32[1][0];
    int i, j;
    for (i = 0; i < 1; i++)
        l_30[i] = &g_13;
    (*p_22) = (~(l_30[0] != ((*l_31) = l_30[0])));
    return g_33[0][3];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_13, "g_13", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_15[i], "g_15[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
