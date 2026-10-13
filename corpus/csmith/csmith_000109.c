/* Csmith 0cdc710315cf seed 109, flattened by scripts/csmith_flatten.py */
// Options:   --seed 109 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 6;
static uint64_t g_27 = 1U;
static uint16_t g_39 = 0x043E;
static int8_t g_43 = 0x92;
static int64_t g_45[3] = {0x8842B4CF22A1EFE0LL,0x8842B4CF22A1EFE0LL,0x8842B4CF22A1EFE0LL};
static uint64_t g_47 = 8U;
static uint64_t *g_46[1][3] = {{&g_47,&g_47,&g_47}};
static uint32_t g_52 = 0x94681C37;
static int16_t g_56 = 0;
static uint64_t g_57 = 18446744073709551615U;
static int32_t g_67 = (-1);
static uint32_t g_76 = 4294967295U;
static uint32_t g_97 = 0x72D3BEAF;

static uint32_t  func_1(void);
static int32_t  func_8(uint32_t  p_9, uint16_t  p_10, int16_t  p_11);
static const uint8_t  func_14(uint32_t  p_15);

static uint32_t  func_1(void)
{ 
    uint32_t l_5 = 1U;
    int32_t l_44 = 0x4186E145;
    uint64_t *l_49 = (void*)0;
    uint64_t *l_50 = (void*)0;
    for (g_2 = 0; (g_2 > (-30)); g_2--)
    { 
        return l_5;
    }
    for (l_5 = (-28); (l_5 == 5); l_5++)
    { 
        uint64_t *l_26 = &g_27;
        int32_t l_28 = 1;
        uint16_t *l_37 = (void*)0;
        uint16_t *l_38 = &g_39;
        int8_t *l_42[2];
        uint64_t **l_48 = &g_46[0][0];
        uint32_t *l_51 = &g_52;
        int16_t l_120 = 0;
        int i;
        for (i = 0; i < 2; i++)
            l_42[i] = &g_43;
        if (func_8((safe_div_func_uint8_t_u_u(func_14(((*l_51) = ((safe_div_func_int16_t_s_s((safe_add_func_uint32_t_u_u((((((safe_mod_func_int8_t_s_s((((safe_rshift_func_uint64_t_u_u(((*l_26) ^= (!(~g_2))), (l_28 | (((l_49 = ((*l_48) = ((g_45[2] ^= (safe_lshift_func_uint8_t_u_s(((safe_mul_func_int8_t_s_s((l_44 = (safe_mod_func_int8_t_s_s((safe_lshift_func_uint16_t_u_s(((*l_38)++), ((((0x6E6C8CAC31E13CA4LL <= l_5) & l_28) & g_2) != l_5))), g_2))), l_5)) > l_28), g_43))) , g_46[0][0]))) == l_50) >= 255U)))) ^ l_28) & l_28), l_5)) >= 0x9096A9CB) != l_28) ^ (-1)) && l_44), g_43)), l_28)) , g_45[2]))), l_5)), l_28, l_5))
        { 
            uint64_t l_100 = 0x8ED9CDEFDC0328A1LL;
            if (g_57)
                break;
            l_100--;
        }
        else
        { 
            int32_t *l_107 = (void*)0;
            uint16_t l_112 = 1U;
            uint64_t **l_119 = &l_50;
            int32_t **l_121 = &l_107;
            g_67 = (((safe_sub_func_int32_t_s_s((-2), 0xD11DCE25)) >= (safe_sub_func_uint8_t_u_u(l_5, g_43))) && g_27);
            l_120 = (safe_lshift_func_int16_t_s_u((safe_rshift_func_int16_t_s_s(l_112, ((!(((safe_add_func_int16_t_s_s((safe_unary_minus_func_uint64_t_u((((((9 >= (safe_add_func_int8_t_s_s(l_5, l_28))) | l_5) , (void*)0) == l_119) ^ g_67))), l_44)) ^ 0x1004047E) ^ g_39)) & 0xC79C))), 1));
            (*l_121) = ((0x21B5A5E52939A49ALL > l_120) , &g_2);
        }
    }
    return g_45[2];
}

static int32_t  func_8(uint32_t  p_9, uint16_t  p_10, int16_t  p_11)
{ 
    int64_t l_96 = 0x4F8299994516193ELL;
    for (g_2 = (-9); (g_2 != 27); g_2 = safe_add_func_uint16_t_u_u(g_2, 1))
    { 
        int64_t l_65 = 2;
        for (g_52 = 0; (g_52 > 7); ++g_52)
        { 
            int32_t *l_66 = &g_67;
            uint32_t *l_75 = &g_76;
            int32_t l_83[4][4] = {{0x9510DDF4,0x9510DDF4,0,0},{0x9510DDF4,0x9510DDF4,0,0},{0x9510DDF4,0x9510DDF4,0,0},{0x9510DDF4,0x9510DDF4,0,0}};
            int i, j;
            (*l_66) |= l_65;
            for (l_65 = (-7); (l_65 <= (-3)); l_65 = safe_add_func_int16_t_s_s(l_65, 2))
            { 
                int32_t **l_70 = &l_66;
            }
            if (((safe_sub_func_uint32_t_u_u((safe_lshift_func_int16_t_s_s(((--(*l_75)) & (&g_43 == &g_43)), (safe_add_func_uint64_t_u_u((safe_lshift_func_int8_t_s_u((g_56 & p_9), p_10)), l_65)))), g_45[2])) <= 0xA48C5527))
            { 
                (*l_66) = (1 && (g_76 > p_11));
                l_83[2][0] = ((*l_66) = l_65);
            }
            else
            { 
                int32_t *l_84 = &l_83[1][3];
                int32_t *l_85 = &l_83[2][0];
                int32_t *l_86 = &l_83[0][0];
                int32_t *l_87 = (void*)0;
                int32_t *l_88 = &l_83[2][0];
                int32_t *l_89 = (void*)0;
                int32_t *l_90 = &g_67;
                int32_t *l_91 = (void*)0;
                int32_t *l_92 = &l_83[2][0];
                int32_t *l_93 = &l_83[0][0];
                int32_t *l_94 = &g_67;
                int32_t *l_95[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_95[i] = (void*)0;
                g_97--;
            }
        }
        return p_9;
    }
    return g_57;
}

static const uint8_t  func_14(uint32_t  p_15)
{ 
    int32_t *l_53 = (void*)0;
    int32_t *l_54 = &g_2;
    int32_t *l_55[1];
    int32_t l_60 = (-1);
    int i;
    for (i = 0; i < 1; i++)
        l_55[i] = &g_2;
    ++g_57;
    g_2 |= l_60;
    return g_52;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_45[i], "g_45[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_67, "g_67", print_hash_value);
    transparent_crc(g_76, "g_76", print_hash_value);
    transparent_crc(g_97, "g_97", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
