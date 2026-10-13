/* Csmith 0cdc710315cf seed 283, flattened by scripts/csmith_flatten.py */
// Options:   --seed 283 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (4294967295U >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
/* ---- generated program ---- */
static long __undefined;

struct S2 {
   uint8_t  f0;
   int16_t  f1;
};

static int32_t g_2[1] = {4};
static int32_t g_3[3][4] = {{(-1),(-1),0xA5EFA20B,(-1)},{(-1),0xD8A94B2A,0xD8A94B2A,(-1)},{0xD8A94B2A,(-1),0xD8A94B2A,0xD8A94B2A}};
static struct S2 g_45 = {2U,6};
static uint32_t g_48 = 0xEF5864DC;
static uint16_t g_49 = 65535U;

static int8_t  func_1(void);
static struct S2  func_16(uint8_t  p_17, uint64_t  p_18, int64_t  p_19, const int32_t * p_20);
static int64_t  func_34(uint8_t  p_35, int32_t * p_36, int32_t * p_37);

static int8_t  func_1(void)
{ 
    uint8_t l_11 = 0x50;
    for (g_3[0][3] = 0; (g_3[0][3] <= 0); g_3[0][3] += 1)
    { 
        int32_t *l_4 = &g_2[0];
        int32_t *l_5 = (void*)0;
        int32_t *l_6 = &g_2[0];
        int32_t l_7[1];
        int32_t *l_8 = &g_2[0];
        int32_t *l_9 = &l_7[0];
        int32_t *l_10[2];
        int i;
        for (i = 0; i < 1; i++)
            l_7[i] = (-8);
        for (i = 0; i < 2; i++)
            l_10[i] = &l_7[0];
        ++l_11;
        g_49 ^= (g_48 = (g_2[0] = (safe_lshift_func_uint16_t_u_s(((&g_2[g_3[0][3]] != (func_16(g_3[0][1], ((l_11 | 1) , g_3[1][3]), g_3[0][3], &g_3[0][0]) , &g_2[0])) == 7), l_11))));
    }
    return g_45.f1;
}

static struct S2  func_16(uint8_t  p_17, uint64_t  p_18, int64_t  p_19, const int32_t * p_20)
{ 
    int32_t l_23 = 0xC0E42606;
    int32_t l_24 = 0x62375818;
    int16_t l_25[4][3] = {{0xB721,(-1),0xB721},{3,3,3},{0xB721,(-1),0xB721},{3,3,3}};
    int32_t l_26 = 8;
    int32_t l_27 = (-1);
    int32_t l_28[2][4] = {{(-3),(-3),(-3),(-3)},{(-3),(-3),(-3),(-3)}};
    int8_t l_29 = 0x67;
    struct S2 l_33 = {6U,1};
    int32_t *l_44 = &l_23;
    struct S2 *l_46 = &l_33;
    struct S2 *l_47 = &g_45;
    int i, j;
    for (p_18 = 0; (p_18 <= 0); p_18 += 1)
    { 
        int32_t *l_21 = &g_2[p_18];
        int32_t *l_22[2][3] = {{&g_3[1][1],&g_3[1][1],&g_3[1][1]},{&g_2[p_18],&g_2[p_18],&g_2[p_18]}};
        uint32_t l_30 = 9U;
        int i, j;
        ++l_30;
        return l_33;
    }
    (*l_44) = (func_34(((safe_mod_func_uint16_t_u_u(0x388C, (safe_rshift_func_uint32_t_u_s(((g_2[0] || ((safe_lshift_func_uint32_t_u_s(((void*)0 == &g_3[0][3]), 14)) && p_18)) && l_29), 1)))) | 0xDB), &l_28[0][2], &l_26) , 8);
    (*l_47) = ((*l_46) = g_45);
    return g_45;
}

static int64_t  func_34(uint8_t  p_35, int32_t * p_36, int32_t * p_37)
{ 
    return p_35;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_3[i][j], "g_3[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_45.f0, "g_45.f0", print_hash_value);
    transparent_crc(g_45.f1, "g_45.f1", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
