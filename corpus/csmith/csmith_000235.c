/* Csmith 0cdc710315cf seed 235, flattened by scripts/csmith_flatten.py */
// Options:   --seed 235 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int64_t g_4 = 0xA27689351F200025LL;
static int32_t g_9 = (-5);
static int32_t g_25 = 0x2C34CCBB;
static uint64_t g_55 = 0U;
static int32_t *g_59[3] = {(void*)0,(void*)0,(void*)0};
static uint8_t g_60 = 0x8B;
static int32_t g_66 = (-4);

static int64_t  func_1(void);
static int32_t * func_10(int32_t * p_11, int32_t * const  p_12, int32_t * p_13, uint32_t  p_14);
static int32_t * func_15(int8_t  p_16, int32_t * p_17, uint8_t  p_18, uint32_t  p_19, int32_t * p_20);

static int64_t  func_1(void)
{ 
    uint32_t l_5 = 0x95010F7A;
    int32_t *l_8[4][1] = {{&g_9},{&g_9},{&g_9},{&g_9}};
    int8_t l_21 = 0x26;
    const uint16_t l_67 = 0xA478;
    uint32_t l_68[2][2];
    int32_t l_69[3];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
            l_68[i][j] = 1U;
    }
    for (i = 0; i < 3; i++)
        l_69[i] = (-1);
    g_9 ^= ((safe_sub_func_int32_t_s_s((g_4 | l_5), ((((safe_div_func_uint64_t_u_u(1U, l_5)) == g_4) , l_5) , 0xAC46DDCD))) , l_5);
    g_59[1] = func_10(func_15(g_9, l_8[1][0], g_9, l_21, &g_9), l_8[1][0], g_59[0], g_60);
    l_68[0][1] = l_67;
    return l_69[2];
}

static int32_t * func_10(int32_t * p_11, int32_t * const  p_12, int32_t * p_13, uint32_t  p_14)
{ 
    uint16_t l_63 = 4U;
    const int64_t l_64 = (-8);
    int32_t *l_65 = &g_66;
    (*l_65) ^= (((safe_mul_func_int32_t_s_s(l_63, ((*p_11) || 0x41E7D42A))) ^ l_63) >= l_64);
    return l_65;
}

static int32_t * func_15(int8_t  p_16, int32_t * p_17, uint8_t  p_18, uint32_t  p_19, int32_t * p_20)
{ 
    uint64_t l_35 = 0xF4C90EB1F0CFADBALL;
    int32_t l_54[2][3] = {{8,8,0x9DA97883},{8,8,0x9DA97883}};
    int i, j;
    if (g_9)
    { 
        const uint16_t l_22 = 0x95FF;
        int32_t *l_24 = &g_25;
        g_25 = (((((*p_20) = g_4) , l_22) < ((+((void*)0 == l_24)) , 0xADC9)) == (*l_24));
    }
    else
    { 
        int8_t l_34 = 0xC3;
        for (p_19 = 0; (p_19 <= 58); ++p_19)
        { 
            int32_t l_32 = 7;
            int32_t l_33 = 0x63439FCB;
            for (g_4 = 0; (g_4 > (-5)); g_4 = safe_sub_func_uint32_t_u_u(g_4, 1))
            { 
                int32_t *l_30 = &g_9;
                int32_t *l_31[3][2] = {{&g_9,&g_9},{&g_9,&g_9},{&g_9,&g_9}};
                int i, j;
                ++l_35;
                return &g_25;
            }
        }
    }
    for (p_16 = 0; (p_16 != 4); ++p_16)
    { 
        int32_t *l_40 = &g_9;
        int32_t *l_41 = &g_9;
        int32_t *l_42 = &g_9;
        int32_t *l_43 = (void*)0;
        int32_t *l_44 = &g_9;
        int32_t *l_45 = &g_9;
        int32_t *l_46 = &g_9;
        int32_t *l_47 = &g_25;
        int32_t *l_48 = &g_9;
        int32_t l_49 = 0x55C71DA8;
        int32_t *l_50 = &g_9;
        int32_t *l_51 = &l_49;
        int32_t *l_52 = &g_25;
        int32_t *l_53[1];
        int32_t **l_58 = &l_53[0];
        int i;
        for (i = 0; i < 1; i++)
            l_53[i] = (void*)0;
        --g_55;
        (*l_52) = (&g_9 == ((*l_58) = &g_25));
    }
    return &g_9;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_66, "g_66", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
