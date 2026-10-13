/* Csmith 0cdc710315cf seed 137, flattened by scripts/csmith_flatten.py */
// Options:   --seed 137 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
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
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S1 {
   const uint64_t  f0;
};

struct S0 {
   int32_t  f0;
   const uint8_t  f1;
};

union U2 {
   struct S0  f0;
   int32_t  f1;
   int8_t  f2;
};

union U3 {
   uint32_t  f0;
   int16_t  f1;
};

static int8_t g_21 = 0xCD;
static int32_t g_26 = 0xF8B243E5;
static uint16_t g_43 = 65526U;
static const struct S1 g_63 = {0x77F5592973452C79LL};
static int32_t g_69 = (-4);
static int32_t g_74 = 4;
static int32_t g_87 = (-1);
static int32_t g_88 = 1;
static int64_t g_89 = (-7);
static uint16_t g_90[1][3] = {{0x9D29,0x9D29,0x9D29}};
static int32_t *g_143[2][2] = {{&g_87,&g_87},{&g_87,&g_87}};

static uint8_t  func_1(void);
static const uint8_t  func_13(union U2  p_14, int8_t  p_15);
static union U2  func_16(int32_t  p_17, int32_t  p_18, uint16_t  p_19, union U2  p_20);

static uint8_t  func_1(void)
{ 
    union U3 l_6 = {0x276B87D0};
    union U2 l_24 = {{0x75FF4442,0x28}};
    int32_t *l_73 = &g_74;
    int32_t l_122 = 0xDEE87059;
    struct S1 l_128 = {0x6921F8CFEE4E9FC4LL};
    int32_t *l_144 = (void*)0;
    int32_t *l_145 = &g_88;
    int32_t *l_146 = &g_69;
    int32_t *l_147 = &l_24.f0.f0;
    int32_t *l_148[1][3];
    int8_t l_149 = 3;
    uint8_t l_150 = 250U;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_148[i][j] = &g_87;
    }
    if (((*l_73) ^= (safe_mul_func_int8_t_s_s(((safe_sub_func_uint8_t_u_u((l_6 , (((safe_sub_func_uint16_t_u_u((safe_rshift_func_int8_t_s_s((safe_sub_func_uint8_t_u_u(func_13(func_16(g_21, ((safe_mul_func_uint8_t_u_u(((((((l_6.f0 <= g_21) || 0xDC1B6EAB) < 0xF8) <= 0) == l_6.f0) && l_6.f0), l_6.f0)) , l_6.f0), l_6.f0, l_24), l_6.f0), g_63.f0)), l_24.f0.f1)), 9)) & l_24.f0.f1) != g_63.f0)), 0xEE)) & 8U), l_24.f0.f1))))
    { 
        int32_t **l_75 = (void*)0;
        int32_t **l_76 = &l_73;
        (*l_76) = &g_26;
    }
    else
    { 
        uint32_t *l_85[4][4] = {{&l_6.f0,&l_6.f0,&l_6.f0,&l_6.f0},{&l_6.f0,&l_6.f0,&l_6.f0,&l_6.f0},{&l_6.f0,&l_6.f0,&l_6.f0,&l_6.f0},{&l_6.f0,&l_6.f0,&l_6.f0,&l_6.f0}};
        int32_t l_86 = 1;
        int32_t *l_93[2];
        uint64_t l_94 = 0xC822056F02031241LL;
        int64_t l_95 = 5;
        int64_t l_121 = (-1);
        uint32_t l_140 = 0xB5D9AA0F;
        int i, j;
        for (i = 0; i < 2; i++)
            l_93[i] = &l_86;
        (*l_73) = (safe_sub_func_int64_t_s_s(((safe_div_func_uint8_t_u_u((((((g_63 , ((safe_sub_func_int64_t_s_s(((*l_73) & ((((((g_88 = (((((safe_div_func_uint32_t_u_u((g_90[0][1]--), l_86)) || l_86) ^ 1) == 0xFD8B) , l_86)) & g_89) | 0U) & (*l_73)) && 0) || 3U)), g_43)) , g_63.f0)) < g_74) > g_21) , &l_93[0]) != (void*)0), l_94)) , g_87), l_95));
        if ((safe_mod_func_uint64_t_u_u(((safe_mul_func_int64_t_s_s((((safe_add_func_int64_t_s_s(0xFA7B1138347E9BEELL, ((*l_73) != 0x52583266))) >= g_63.f0) & 8U), g_89)) != (-1)), g_43)))
        { 
            uint32_t l_115 = 2U;
            for (g_21 = 0; (g_21 <= 14); ++g_21)
            { 
                uint64_t *l_116 = &l_94;
                int8_t *l_117 = (void*)0;
                int32_t l_118 = 0x790E20E3;
                (*l_73) = ((safe_rshift_func_int8_t_s_s((l_118 = ((safe_sub_func_uint8_t_u_u((+g_21), (((0x665D24B411FA983ELL & ((*l_116) = (safe_mod_func_uint16_t_u_u(((safe_lshift_func_uint16_t_u_s((safe_lshift_func_int8_t_s_s(((((l_115 , l_115) , l_115) || g_87) , (*l_73)), g_21)), 13)) <= 0x6D39), g_26)))) >= g_26) && g_63.f0))) < g_89)), g_21)) ^ l_115);
            }
            for (l_24.f1 = 0; (l_24.f1 == 14); l_24.f1++)
            { 
                int32_t **l_123 = &l_93[0];
                g_69 = (l_122 = ((*l_73) = l_121));
                (*l_123) = &l_122;
                return g_69;
            }
        }
        else
        { 
            for (g_69 = 0; (g_69 > (-9)); --g_69)
            { 
                union U3 *l_127 = &l_6;
                union U3 **l_126 = &l_127;
                int32_t **l_136 = (void*)0;
                int32_t **l_137 = &l_73;
                (*l_126) = (void*)0;
                (*l_137) = ((l_128 , (safe_div_func_uint16_t_u_u(((((safe_unary_minus_func_int16_t_s(g_21)) <= (((((safe_rshift_func_uint32_t_u_s((safe_sub_func_int8_t_s_s(0xAA, (*l_73))), (*l_73))) || 8U) <= g_88) >= 0xCDB0A0E13958EBF8LL) > (*l_73))) > 0x1A12B70B) || (-1)), g_21))) , &g_87);
            }
            for (g_69 = 11; (g_69 > 7); --g_69)
            { 
                --l_140;
                g_143[0][1] = &g_88;
            }
        }
    }
    --l_150;
    return (*l_146);
}

static const uint8_t  func_13(union U2  p_14, int8_t  p_15)
{ 
    int32_t *l_71[3];
    int32_t **l_72 = &l_71[2];
    int i;
    for (i = 0; i < 3; i++)
        l_71[i] = &g_69;
    (*l_72) = l_71[0];
    (**l_72) ^= ((void*)0 != &l_71[0]);
    return (**l_72);
}

static union U2  func_16(int32_t  p_17, int32_t  p_18, uint16_t  p_19, union U2  p_20)
{ 
    int32_t *l_25 = &g_26;
    int32_t *l_27 = &g_26;
    int32_t *l_28 = &g_26;
    int32_t *l_29 = &g_26;
    int32_t *l_30 = &g_26;
    int32_t *l_31 = &g_26;
    int32_t *l_32 = &g_26;
    int32_t *l_33 = &g_26;
    int32_t *l_34[3];
    uint32_t l_35 = 4294967289U;
    int32_t **l_38 = &l_29;
    uint32_t l_47[1][2];
    union U3 l_50 = {0x74B4FBF6};
    union U2 l_70 = {{0x58B99485,0x88}};
    int i, j;
    for (i = 0; i < 3; i++)
        l_34[i] = &g_26;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_47[i][j] = 1U;
    }
    ++l_35;
    if ((((((*l_38) = &g_26) == (p_20.f0.f1 , &p_17)) == p_19) <= g_21))
    { 
        int64_t *l_42 = (void*)0;
        int32_t l_46 = 0x77ED469F;
        l_47[0][0] &= (((1U && ((safe_mod_func_int8_t_s_s(((((~(g_43 = g_21)) || (safe_mod_func_uint64_t_u_u((*l_30), l_46))) ^ p_17) >= 0x8CBF698BA832A49ELL), 0x4A)) == 0x5D2300B6)) > 4U) | (-1));
        if ((p_17 <= (safe_sub_func_uint64_t_u_u(((((void*)0 != &l_46) >= p_19) || g_43), l_46))))
        { 
            (*l_38) = &p_18;
        }
        else
        { 
            (*l_38) = &g_26;
        }
    }
    else
    { 
        int8_t l_53 = (-8);
        uint64_t l_64 = 0x0D4F6636227FA2E5LL;
        uint64_t l_65 = 8U;
        uint64_t *l_66 = (void*)0;
        uint64_t *l_67 = (void*)0;
        uint64_t *l_68 = &l_65;
        (*l_32) = (l_50 , (((safe_lshift_func_uint8_t_u_s(0x9C, (0x8A > (*l_29)))) , g_26) , l_53));
        (*l_38) = &g_26;
        g_69 = (((*l_68) = (((safe_div_func_int8_t_s_s((safe_add_func_uint16_t_u_u(((safe_mod_func_uint32_t_u_u((safe_sub_func_int64_t_s_s(0xE8B88BB1B5B3A04BLL, ((((+(g_63 , ((((((*l_27) &= ((0xD933B151854E4619LL ^ l_53) > 0x56207CCB)) ^ l_64) , (*l_25)) > 0xE904CDE8B735D465LL) != l_64))) > l_65) & g_43) < 0x5AE34030))), l_65)) >= 9), g_21)), p_20.f0.f1)) , l_65) > (-6))) == g_63.f0);
    }
    return l_70;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_63.f0, "g_63.f0", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    transparent_crc(g_88, "g_88", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_90[i][j], "g_90[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
