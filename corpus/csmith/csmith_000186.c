/* Csmith 0cdc710315cf seed 186, flattened by scripts/csmith_flatten.py */
// Options:   --seed 186 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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

union U0 {
   const uint16_t  f0;
   int8_t  f1;
   uint32_t  f2;
};

union U1 {
   uint16_t  f0;
   const uint8_t  f1;
};

static int8_t g_6 = (-1);
static uint16_t g_25 = 0x4675;
static union U1 g_26 = {0U};
static int32_t g_28 = 0x20A6FF60;
static int16_t g_32[4][1] = {{1},{(-1)},{1},{(-1)}};
static uint8_t g_33 = 0x49;
static int32_t g_42 = 0xEA4087B3;
static union U0 g_46 = {0xE69D};
static union U0 *g_45 = &g_46;
static union U1 g_54 = {65535U};
static union U1 g_84 = {0x66DE};
static union U1 *g_83 = &g_84;

static int8_t  func_1(void);
static int32_t  func_2(uint64_t  p_3, uint8_t  p_4, const union U1  p_5);
static int32_t  func_11(union U1  p_12);

static int8_t  func_1(void)
{ 
    int16_t l_23 = 1;
    uint16_t *l_24 = &g_25;
    int32_t l_36 = 2;
    int32_t *l_44 = &g_42;
    union U0 **l_47 = &g_45;
    uint8_t l_75 = 0x40;
    (*l_44) = func_2(g_6, (safe_sub_func_int64_t_s_s((g_6 != ((safe_rshift_func_int32_t_s_u(func_11((((*l_24) = ((safe_mod_func_uint16_t_u_u((safe_sub_func_uint64_t_u_u(((safe_div_func_uint16_t_u_u(((safe_add_func_uint64_t_u_u((safe_rshift_func_uint8_t_u_u(((l_23 && 0x5C) >= g_6), g_6)), g_6)) , 65532U), l_23)) , g_6), g_6)), g_6)) < 4U)) , g_26)), l_23)) , l_36)), g_28)), g_26);
    (*l_47) = g_45;
    if ((*l_44))
    { 
        uint16_t *l_50 = &g_26.f0;
        union U1 **l_51 = (void*)0;
        union U1 **l_52 = (void*)0;
        union U1 *l_53 = &g_54;
        for (g_6 = 16; (g_6 > 17); g_6++)
        { 
            (*l_44) |= (l_50 == &g_25);
        }
        l_53 = &g_26;
    }
    else
    { 
        uint16_t l_74[2][4] = {{0xA830,0xA830,0xCA2C,0xA830},{0xA830,8U,8U,0xA830}};
        int32_t *l_80 = &g_42;
        union U1 *l_82 = &g_26;
        int i, j;
        (*l_44) = g_26.f0;
        if (((((safe_sub_func_uint32_t_u_u((safe_mod_func_int16_t_s_s((+((safe_add_func_int8_t_s_s((safe_sub_func_uint8_t_u_u(((safe_mod_func_int32_t_s_s((g_25 > (safe_mod_func_int32_t_s_s((((safe_mod_func_int16_t_s_s((0xA615B38D6AEAB669LL && (((safe_mod_func_int64_t_s_s((safe_div_func_int64_t_s_s(1, 1U)), g_6)) >= g_32[3][0]) ^ l_74[1][0])), 0x1695)) , 0x89) , l_74[1][2]), g_42))), l_75)) , 0xA1), l_74[0][3])), 0x95)) == g_46.f0)), g_32[0][0])), l_74[1][0])) ^ l_74[0][2]) && 0x0ECE) > (*l_44)))
        { 
            for (g_33 = (-30); (g_33 < 47); g_33++)
            { 
                return g_33;
            }
        }
        else
        { 
            int32_t *l_78 = &g_28;
            int32_t **l_79[4];
            int16_t *l_81 = &g_32[0][0];
            int i;
            for (i = 0; i < 4; i++)
                l_79[i] = &l_44;
            l_80 = (g_46.f0 , l_78);
            l_36 ^= (((*l_80) ^ ((g_33 <= (((*l_81) ^= (g_46.f0 & g_33)) < 0x2348)) < (*l_44))) < g_25);
        }
        g_83 = l_82;
    }
    return g_46.f0;
}

static int32_t  func_2(uint64_t  p_3, uint8_t  p_4, const union U1  p_5)
{ 
    int32_t *l_37 = &g_28;
    int32_t *l_41 = &g_42;
    int32_t **l_43 = &l_37;
    l_37 = l_37;
    for (g_28 = 0; (g_28 == (-28)); g_28 = safe_sub_func_int32_t_s_s(g_28, 7))
    { 
        int32_t **l_40[2];
        int i;
        for (i = 0; i < 2; i++)
            l_40[i] = &l_37;
        l_41 = (void*)0;
    }
    (*l_43) = &g_42;
    return g_33;
}

static int32_t  func_11(union U1  p_12)
{ 
    int32_t *l_27 = &g_28;
    int32_t *l_29 = (void*)0;
    int32_t *l_30 = &g_28;
    int32_t *l_31[4][2] = {{(void*)0,&g_28},{&g_28,(void*)0},{&g_28,&g_28},{(void*)0,&g_28}};
    int i, j;
    g_33--;
    return p_12.f0;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_26.f0, "g_26.f0", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_32[i][j], "g_32[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_46.f0, "g_46.f0", print_hash_value);
    transparent_crc(g_54.f0, "g_54.f0", print_hash_value);
    transparent_crc(g_84.f0, "g_84.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
