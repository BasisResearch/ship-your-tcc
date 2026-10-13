/* Csmith 0cdc710315cf seed 145, flattened by scripts/csmith_flatten.py */
// Options:   --seed 145 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
   const uint64_t  f1;
};

struct S1 {
   int8_t  f0;
};

static int32_t g_12 = 0xEA388845;
static struct S0 g_24[1] = {{0x42,0x06DD2CC9105FA2EFLL}};
static int32_t g_26 = 1;
static uint8_t g_28 = 0U;
static int32_t g_31 = 0x0B56CEF3;
static int32_t *g_30 = &g_31;
static struct S1 g_32 = {0xF3};
static int16_t g_39 = 0xF839;
static int32_t g_40 = 0;
static int64_t g_41 = 0xFE298319B20E9505LL;
static int8_t g_42 = 0xBD;
static uint8_t g_43 = 255U;
static const uint8_t g_60 = 0x75;
static const uint8_t *g_59 = &g_60;
static int64_t g_76 = 0x9F207B6F817EA5CALL;
static uint16_t g_96 = 2U;

static uint16_t  func_1(void);
static int32_t  func_13(uint8_t  p_14, struct S1  p_15, int32_t * p_16, int32_t * p_17, int32_t * p_18);
static uint8_t  func_19(int32_t * p_20, int16_t  p_21, struct S0  p_22, int32_t * p_23);

static uint16_t  func_1(void)
{ 
    uint32_t l_2 = 1U;
    int32_t *l_11 = &g_12;
    uint8_t *l_27 = &g_28;
    struct S1 l_29 = {0x6D};
    int32_t l_35 = 0x83E731E7;
    int32_t l_38[3];
    int8_t l_63 = (-7);
    int8_t l_75 = 1;
    int16_t l_77 = 5;
    int8_t *l_102 = &g_42;
    int8_t **l_101[2][1];
    int i, j;
    for (i = 0; i < 3; i++)
        l_38[i] = 1;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_101[i][j] = &l_102;
    }
    (*l_11) = (l_2 || (+(safe_add_func_uint8_t_u_u(1U, ((safe_lshift_func_uint16_t_u_u((safe_sub_func_uint16_t_u_u((~(-3)), 0)), l_2)) || 1)))));
    if (func_13(((*l_27) ^= func_19(&g_12, g_12, g_24[0], l_11)), l_29, l_11, g_30, &g_31))
    { 
        int32_t *l_34 = &g_12;
        int32_t *l_36 = &l_35;
        int32_t *l_37[3][1];
        const uint8_t *l_58 = &g_43;
        int8_t *l_61 = (void*)0;
        int8_t *l_62 = &g_42;
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_37[i][j] = &g_31;
        }
        g_43++;
        l_38[2] |= (safe_sub_func_int32_t_s_s((((safe_sub_func_uint64_t_u_u(0xD8EDCE5F1497EA9BLL, (((*l_62) = (safe_rshift_func_int64_t_s_u((safe_sub_func_uint8_t_u_u((safe_mod_func_uint64_t_u_u((((safe_sub_func_int64_t_s_s(((g_59 = l_58) == &g_43), (*l_11))) || (*l_36)) < g_39), g_31)), (*l_11))), g_60))) || (*l_11)))) ^ (*l_34)) ^ (*l_36)), l_63));
        for (g_31 = 5; (g_31 != (-5)); g_31 = safe_sub_func_int64_t_s_s(g_31, 5))
        { 
            int32_t **l_78 = &g_30;
            for (g_43 = 0; (g_43 > 43); g_43++)
            { 
                int32_t **l_70[2][3] = {{&l_37[0][0],&l_37[0][0],&l_37[0][0]},{&l_11,&l_11,&l_11}};
                uint8_t **l_73 = (void*)0;
                uint8_t **l_74 = &l_27;
                int i, j;
                (*l_11) &= (safe_mul_func_uint8_t_u_u((*g_59), (&g_42 != &g_42)));
                l_34 = (g_30 = &g_31);
                l_77 = ((safe_add_func_int16_t_s_s((((*g_30) , &g_28) != ((*l_74) = &g_28)), l_75)) | g_76);
            }
            (*l_78) = &g_26;
        }
    }
    else
    { 
        int32_t l_82[4][4] = {{1,1,0xBD2431EA,1},{1,0x25BA0F3F,0x25BA0F3F,1},{0x25BA0F3F,1,0x25BA0F3F,0x25BA0F3F},{1,1,0xBD2431EA,1}};
        int i, j;
        for (g_31 = 16; (g_31 >= 25); g_31 = safe_add_func_uint16_t_u_u(g_31, 5))
        { 
            int16_t *l_81[2];
            int32_t l_93 = (-1);
            int32_t *l_94 = &g_26;
            uint16_t *l_95 = &g_96;
            int8_t **l_103 = &l_102;
            int i;
            for (i = 0; i < 2; i++)
                l_81[i] = &l_77;
            (*l_11) = ((l_82[0][0] = (g_39 = g_39)) != g_42);
            (*l_94) |= (safe_div_func_int64_t_s_s((safe_rshift_func_uint8_t_u_s(0x78, (((((safe_lshift_func_int64_t_s_s((safe_sub_func_int8_t_s_s((safe_lshift_func_int8_t_s_u((*l_11), l_93)), (*l_11))), 5)) || g_31) >= g_76) && g_24[0].f1) || (*l_11)))), g_32.f0));
            if ((((*l_95)++) & (0x4639 > (safe_lshift_func_int64_t_s_s(0x5D2C3574787CBDB4LL, 51)))))
            { 
                (*l_11) = 0x8C655405;
                l_103 = l_101[1][0];
            }
            else
            { 
                struct S1 l_106 = {-9};
                l_93 |= (safe_rshift_func_uint8_t_u_s((l_106 , (safe_mod_func_uint32_t_u_u((((*l_11) || (g_96 &= (safe_sub_func_int64_t_s_s(0x4BD0913F082F4DE9LL, 1U)))) >= (*l_94)), g_41))), 6));
            }
        }
    }
    return (*l_11);
}

static int32_t  func_13(uint8_t  p_14, struct S1  p_15, int32_t * p_16, int32_t * p_17, int32_t * p_18)
{ 
    struct S1 *l_33[3];
    int i;
    for (i = 0; i < 3; i++)
        l_33[i] = (void*)0;
    g_32 = g_32;
    return (*p_18);
}

static uint8_t  func_19(int32_t * p_20, int16_t  p_21, struct S0  p_22, int32_t * p_23)
{ 
    int32_t *l_25 = &g_26;
    (*l_25) ^= ((*p_20) = g_24[0].f0);
    return g_24[0].f1;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_12, "g_12", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_24[i].f0, "g_24[i].f0", print_hash_value);
        transparent_crc(g_24[i].f1, "g_24[i].f1", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_32.f0, "g_32.f0", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_76, "g_76", print_hash_value);
    transparent_crc(g_96, "g_96", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
