/* Csmith 0cdc710315cf seed 331, flattened by scripts/csmith_flatten.py */
// Options:   --seed 331 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
   uint64_t  f1;
   const uint8_t  f2;
};

static int16_t g_15 = 0x2282;
static int16_t g_23 = 0x0CD9;
static const int32_t *g_24 = (void*)0;
static int32_t g_28 = 0x8DB370CC;
static int32_t *g_27 = &g_28;
static uint64_t g_32 = 0x52B8909FF4C90EB1LL;
static int64_t g_39 = (-3);
static uint8_t g_50 = 1U;
static int32_t g_52 = 0xBDD0C5DF;
static int64_t g_58[3][3] = {{0xC8CEE692D353AE79LL,0x4E562A08D8D3B62BLL,0xC8CEE692D353AE79LL},{0xC8CEE692D353AE79LL,0x4E562A08D8D3B62BLL,0xC8CEE692D353AE79LL},{0xC8CEE692D353AE79LL,0x4E562A08D8D3B62BLL,0xC8CEE692D353AE79LL}};
static int16_t g_59 = 0xCB3F;
static uint16_t g_60 = 0x6A47;
static struct S0 g_68 = {0xED,0x629925D9175C2255LL,0U};

static struct S0  func_1(void);
static int32_t * func_2(int32_t * p_3, int32_t * const  p_4, const int32_t * p_5, uint8_t  p_6, int32_t * p_7);
static int32_t * func_9(uint16_t  p_10, int16_t  p_11, int32_t * p_12);

static struct S0  func_1(void)
{ 
    int32_t *l_8 = (void*)0;
    int64_t l_16[3][3] = {{0xDAD94E8FE7528ACELL,3,0xDAD94E8FE7528ACELL},{1,1,1},{0xDAD94E8FE7528ACELL,3,0xDAD94E8FE7528ACELL}};
    int8_t l_44 = 0x9A;
    int8_t l_46 = (-1);
    int32_t l_47[1];
    int i, j;
    for (i = 0; i < 1; i++)
        l_47[i] = 0;
    l_8 = func_2(l_8, func_9((0xCC2443AC != (safe_lshift_func_uint16_t_u_s(((g_15 == l_16[0][0]) & l_16[0][0]), g_15))), g_15, l_8), &g_28, g_15, l_8);
    for (g_32 = 0; (g_32 < 30); g_32 = safe_add_func_uint16_t_u_u(g_32, 4))
    { 
        uint64_t l_45 = 8U;
        int32_t *l_48 = (void*)0;
        int32_t l_49 = 0x48071C33;
        int32_t *l_51 = (void*)0;
        int32_t *l_53 = &g_52;
        int32_t *l_54 = (void*)0;
        int32_t *l_55 = (void*)0;
        int32_t *l_56 = (void*)0;
        int32_t *l_57[3][3] = {{(void*)0,(void*)0,(void*)0},{&l_47[0],&g_28,&l_47[0]},{(void*)0,(void*)0,(void*)0}};
        int i, j;
        l_49 = (((*g_27) ^= (safe_mod_func_int32_t_s_s(l_44, 0x08336182))) <= (l_47[0] = ((((((((void*)0 == g_27) , g_32) & l_45) && g_23) && l_46) & l_45) >= 0x34D7F484)));
        if (g_50)
            continue;
        ++g_60;
    }
    g_52 &= ((((safe_mul_func_int8_t_s_s(((safe_lshift_func_int16_t_s_s((!((&l_47[0] != ((&g_52 == (void*)0) , (void*)0)) >= g_60)), g_32)) | g_58[1][1]), g_59)) <= (*g_27)) != 6) , (-1));
    return g_68;
}

static int32_t * func_2(int32_t * p_3, int32_t * const  p_4, const int32_t * p_5, uint8_t  p_6, int32_t * p_7)
{ 
    int32_t *l_29 = &g_28;
    int32_t *l_30[2];
    int8_t l_31 = 0x90;
    int i;
    for (i = 0; i < 2; i++)
        l_30[i] = &g_28;
    --g_32;
    g_39 ^= (p_6 || (safe_div_func_int8_t_s_s((safe_lshift_func_int32_t_s_s((p_6 >= (*l_29)), 15)), 0xA9)));
    return p_7;
}

static int32_t * func_9(uint16_t  p_10, int16_t  p_11, int32_t * p_12)
{ 
    uint64_t l_17 = 0x0B50F6B34C266D8FLL;
    l_17--;
    for (p_10 = (-19); (p_10 >= 19); ++p_10)
    { 
        int32_t l_25[4];
        int32_t *l_26 = &l_25[1];
        int i;
        for (i = 0; i < 4; i++)
            l_25[i] = 0xCBB24ADC;
        if (g_15)
            break;
        g_23 = (+p_10);
        (*l_26) = (((g_24 = (void*)0) == p_12) , l_25[1]);
    }
    return g_27;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_58[i][j], "g_58[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_68.f0, "g_68.f0", print_hash_value);
    transparent_crc(g_68.f1, "g_68.f1", print_hash_value);
    transparent_crc(g_68.f2, "g_68.f2", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
