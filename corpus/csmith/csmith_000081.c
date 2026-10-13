/* Csmith 0cdc710315cf seed 81, flattened by scripts/csmith_flatten.py */
// Options:   --seed 81 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
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
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
static int64_t
safe_unary_minus_func_int64_t_s(int64_t si )
{
  return
    (si==(-9223372036854775807L-1)) ?
    ((si)) :
    -si;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   int32_t  f0;
   uint16_t  f1;
};

static int32_t g_2 = 0x8E469AD5;
static int32_t g_12 = 0x74A52027;
static int32_t **g_25 = (void*)0;
static uint64_t g_27 = 1U;
static int32_t g_34 = 0xBECC33BA;
static int32_t *g_35 = &g_34;
static union U0 g_48 = {0x9A46BB21};
static int64_t g_68 = (-7);
static int16_t g_85[1][3] = {{(-3),(-3),(-3)}};
static uint64_t *g_107 = (void*)0;
static uint64_t **g_106 = &g_107;
static int8_t g_118 = 2;
static uint16_t g_119[1][1] = {{65526U}};
static uint16_t g_120 = 65535U;
static int32_t g_127[4] = {(-1),(-1),(-1),(-1)};

static uint64_t  func_1(void);
static uint8_t  func_7(int32_t * p_8);
static int32_t ** func_20(int16_t  p_21, int32_t ** p_22, uint64_t  p_23);

static uint64_t  func_1(void)
{ 
    int32_t *l_9 = &g_2;
    int32_t l_70 = 0x41EDA738;
    uint64_t *l_81 = &g_27;
    union U0 l_82 = {0x88EAD427};
    int32_t *l_141[3];
    int8_t l_152 = 0;
    int64_t l_155[2];
    int i;
    for (i = 0; i < 3; i++)
        l_141[i] = &g_34;
    for (i = 0; i < 2; i++)
        l_155[i] = 0xF76BB7620F8F3257LL;
    for (g_2 = 0; (g_2 != 0); g_2 = safe_add_func_uint8_t_u_u(g_2, 2))
    { 
        int32_t **l_59[3][4] = {{&g_35,&g_35,&g_35,&g_35},{&g_35,&g_35,&g_35,&g_35},{&g_35,&g_35,&g_35,&g_35}};
        int32_t *l_60 = &g_34;
        int i, j;
        l_60 = (((safe_rshift_func_uint64_t_u_s((func_7(l_9) ^ (((safe_sub_func_uint8_t_u_u(((safe_add_func_int32_t_s_s(((((safe_rshift_func_int16_t_s_u((18446744073709551613U || (*l_9)), 12)) , (void*)0) != (void*)0) , g_2), g_2)) & (*l_9)), (*l_9))) <= g_2) && 0xFB6675F713EC2CFCLL)), 63)) > g_2) , l_9);
        for (g_48.f0 = 0; (g_48.f0 >= 29); g_48.f0 = safe_add_func_int32_t_s_s(g_48.f0, 1))
        { 
            uint16_t l_65 = 0x18A9;
            uint64_t l_80[3];
            int i;
            for (i = 0; i < 3; i++)
                l_80[i] = 0U;
        }
        g_34 = (*g_35);
    }
    return l_155[1];
}

static uint8_t  func_7(int32_t * p_8)
{ 
    int8_t l_10 = 0x3E;
    int32_t l_17 = 8;
    uint64_t l_24 = 0x0AE7B1B0CADFB1AFLL;
    if (l_10)
    { 
        uint16_t l_11 = 0x1BA0;
        int32_t *l_15 = &g_12;
        g_12 = ((((l_11 , l_10) >= g_2) , g_2) > (-1));
        (*l_15) ^= (safe_add_func_uint16_t_u_u(l_11, l_11));
    }
    else
    { 
        int32_t *l_16[2][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_16[i][j] = &g_12;
        }
        l_17 = (*p_8);
        for (g_12 = (-12); (g_12 == 2); g_12 = safe_add_func_uint16_t_u_u(g_12, 7))
        { 
            uint64_t *l_26 = &g_27;
            int32_t l_51[2][1];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_51[i][j] = 0x8319B20E;
            }
            g_25 = func_20(l_24, g_25, ((*l_26) = (g_2 >= l_24)));
            l_17 |= ((safe_rshift_func_uint16_t_u_s(((safe_rshift_func_int8_t_s_u((safe_unary_minus_func_int64_t_s((g_48 , ((void*)0 != l_26)))), 0)) , g_2), 15)) == g_12);
            for (g_48.f0 = 29; (g_48.f0 <= 8); g_48.f0 = safe_sub_func_uint64_t_u_u(g_48.f0, 9))
            { 
                uint8_t l_52 = 0x34;
                l_51[0][0] ^= l_10;
                if ((**g_25))
                    break;
                l_52 = 0xFA0B5BCD;
            }
        }
    }
    return l_17;
}

static int32_t ** func_20(int16_t  p_21, int32_t ** p_22, uint64_t  p_23)
{ 
    int16_t l_30[4][1];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
            l_30[i][j] = 0x4D0E;
    }
    if (((((l_30[3][0] = (safe_mod_func_int64_t_s_s(p_23, g_12))) , l_30[3][0]) > 0x9C93E32883E731E7LL) , l_30[2][0]))
    { 
        int32_t *l_32 = &g_2;
        int32_t **l_31[2];
        int32_t *l_33 = &g_34;
        int i;
        for (i = 0; i < 2; i++)
            l_31[i] = &l_32;
        g_35 = (l_33 = &g_2);
        g_34 = (*l_32);
    }
    else
    { 
        int32_t * const l_41 = &g_12;
    }
    return &g_35;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_34, "g_34", print_hash_value);
    transparent_crc(g_48.f0, "g_48.f0", print_hash_value);
    transparent_crc(g_68, "g_68", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_85[i][j], "g_85[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_118, "g_118", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_119[i][j], "g_119[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_120, "g_120", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_127[i], "g_127[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
