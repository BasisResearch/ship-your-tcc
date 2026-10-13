/* Csmith 0cdc710315cf seed 83, flattened by scripts/csmith_flatten.py */
// Options:   --seed 83 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_13 = (-2);
static int8_t g_20 = 0;
static int32_t g_23 = 0x806AA1C3;
static int64_t g_25 = 1;
static int8_t g_27 = 0x62;
static int8_t g_28 = 0x2D;
static uint32_t g_30 = 0x41BD43D9;
static int32_t *g_62 = &g_23;
static int32_t g_73 = (-8);
static uint32_t g_108 = 2U;
static int32_t **g_122 = &g_62;
static const int32_t *g_132 = (void*)0;
static uint8_t g_151 = 1U;
static uint64_t g_161 = 0xB7EED95D173CA947LL;
static uint64_t g_164 = 0xA3D7B8FE932ECC1ELL;
static uint32_t * const *g_177 = (void*)0;
static uint32_t g_180 = 4294967289U;
static uint32_t *g_182 = (void*)0;
static uint32_t **g_181 = &g_182;
static uint16_t g_196 = 9U;
static int64_t g_206 = 0xD6ADFE8D4ABBEE46LL;

static int8_t  func_1(void);
static uint32_t  func_16(int8_t  p_17, int64_t  p_18);
static int8_t  func_37(int8_t * p_38);

static int8_t  func_1(void)
{ 
    int32_t l_4 = 0x6E6BACD7;
    uint16_t l_11 = 4U;
    int32_t *l_12[3];
    int8_t *l_19 = &g_20;
    int32_t *l_217[2][1];
    int32_t l_227 = 0xF66EBC3A;
    const uint64_t *l_228 = &g_164;
    uint32_t **l_231 = &g_182;
    int i, j;
    for (i = 0; i < 3; i++)
        l_12[i] = &g_13;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_217[i][j] = &g_23;
    }
    g_13 |= (safe_lshift_func_uint64_t_u_u((l_4 == ((safe_mod_func_uint32_t_u_u(0x85BF25CB, (safe_mod_func_uint32_t_u_u((safe_sub_func_uint32_t_u_u(l_4, 0x813F1A07)), l_11)))) , 8U)), l_4));
    if (((g_13 , 0x66C619A8) ^ ((func_16(((*l_19) = (-1)), g_13) & 0) == g_164)))
    { 
        uint16_t l_214 = 0x9046;
        --l_214;
    }
    else
    { 
        uint64_t l_222[1];
        int32_t l_224 = 0x13E252BF;
        uint32_t **l_225 = &g_182;
        int i;
        for (i = 0; i < 1; i++)
            l_222[i] = 18446744073709551609U;
        for (g_13 = 0; (g_13 <= 2); g_13 += 1)
        { 
            int64_t *l_223[1][2];
            uint32_t *l_226 = (void*)0;
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_223[i][j] = (void*)0;
            }
            l_12[g_13] = l_217[1][0];
            (*g_62) = ((safe_lshift_func_uint32_t_u_s(((safe_mod_func_uint32_t_u_u(((g_30 = ((l_224 = l_222[0]) | (l_225 == &g_182))) || g_108), 0x938C6D90)) , l_227), l_222[0])) >= (*g_62));
        }
        (*g_62) = (g_164 <= (l_228 == (((safe_div_func_int8_t_s_s((((*g_181) = (*g_181)) != (void*)0), g_23)) >= (*g_62)) , (void*)0)));
        l_231 = l_225;
    }
    return g_206;
}

static uint32_t  func_16(int8_t  p_17, int64_t  p_18)
{ 
    uint8_t l_21[1][1];
    int32_t l_29 = 0x88AD88CA;
    int8_t *l_39 = &g_28;
    uint32_t **l_183 = &g_182;
    uint64_t l_212 = 18446744073709551610U;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_21[i][j] = 0U;
    }
    for (p_17 = 0; (p_17 >= 0); p_17 -= 1)
    { 
        int64_t l_24 = 0x915BB002891BA802LL;
        int32_t l_26 = 0x18EBDB88;
        int32_t l_126[2];
        int i;
        for (i = 0; i < 2; i++)
            l_126[i] = (-1);
    }
    return l_21[0][0];
}

static int8_t  func_37(int8_t * p_38)
{ 
    int32_t l_40[2];
    uint32_t *l_69 = (void*)0;
    uint32_t *l_70 = &g_30;
    int8_t l_71 = 0xB0;
    int32_t *l_72 = &g_73;
    int i;
    for (i = 0; i < 2; i++)
        l_40[i] = (-2);
    for (g_25 = 1; (g_25 >= 0); g_25 -= 1)
    { 
        int32_t *l_61[1];
        int i;
        for (i = 0; i < 1; i++)
            l_61[i] = &l_40[1];
        if ((safe_lshift_func_int8_t_s_s((safe_lshift_func_uint8_t_u_s((safe_add_func_int32_t_s_s((safe_div_func_uint8_t_u_u(((0 >= (safe_mod_func_uint32_t_u_u((safe_add_func_uint16_t_u_u((((safe_div_func_uint64_t_u_u((+((-4) >= ((l_40[g_25] , &g_28) == &g_27))), g_23)) == 0xDFE8) <= 255U), l_40[g_25])), 5U))) & g_27), (*p_38))), g_28)), g_28)), 6)))
        { 
            int32_t *l_56 = (void*)0;
            int32_t *l_57 = &l_40[g_25];
            (*l_57) = (-1);
        }
        else
        { 
            int8_t l_58[3];
            int32_t *l_59 = &l_40[g_25];
            int32_t **l_60[1][1];
            int i, j;
            for (i = 0; i < 3; i++)
                l_58[i] = 1;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 1; j++)
                    l_60[i][j] = &l_59;
            }
            (*l_59) = l_58[0];
            g_62 = (l_61[0] = l_59);
        }
    }
    return (*l_72);
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_73, "g_73", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    transparent_crc(g_151, "g_151", print_hash_value);
    transparent_crc(g_161, "g_161", print_hash_value);
    transparent_crc(g_164, "g_164", print_hash_value);
    transparent_crc(g_180, "g_180", print_hash_value);
    transparent_crc(g_196, "g_196", print_hash_value);
    transparent_crc(g_206, "g_206", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
