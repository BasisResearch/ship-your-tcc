/* Csmith 0cdc710315cf seed 216, flattened by scripts/csmith_flatten.py */
// Options:   --seed 216 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
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

static int64_t g_12 = 0x0D461E3FF14EB4C8LL;
static int32_t g_16 = (-1);
static int32_t *g_15 = &g_16;
static uint64_t g_22 = 1U;
static int64_t g_24 = 0x093D7F285F402BE2LL;
static uint16_t g_39 = 65535U;
static int8_t g_42 = (-1);
static uint8_t g_44 = 255U;
static int32_t g_60 = 0;
static uint64_t g_62 = 0xFE07384C7B934CEBLL;
static int32_t g_79[1][2] = {{0xD91D694B,0xD91D694B}};
static uint8_t g_90 = 0U;
static uint32_t *g_102 = (void*)0;

static uint64_t  func_1(void);
static int32_t  func_3(uint8_t  p_4);
static uint64_t  func_5(uint64_t  p_6, int64_t  p_7, int32_t  p_8, uint32_t  p_9);

static uint64_t  func_1(void)
{ 
    uint32_t l_2 = 0x60F57A7C;
    int32_t l_14 = 0x38DC5336;
    int16_t l_17 = 0;
    int8_t l_40 = 0;
    uint32_t l_50 = 1U;
    int32_t l_81 = 0x99A55AEF;
    int8_t l_82 = 0x77;
    int32_t l_83 = 0x50B76A7F;
    int32_t l_84 = 0;
    int32_t l_85 = 0xF99D2484;
    int16_t l_86 = (-1);
    int32_t l_87 = 0xA7054D2A;
    int32_t l_89[2];
    uint16_t *l_108 = &g_39;
    int64_t l_109[4][1] = {{0x41019322E619775ELL},{0},{0x41019322E619775ELL},{0}};
    int64_t l_110 = (-3);
    const int64_t l_111[2][1] = {{0x16D180367A8BF02ELL},{0x16D180367A8BF02ELL}};
    int i, j;
    for (i = 0; i < 2; i++)
        l_89[i] = 0;
    if ((l_2 == 0x398070AD))
    { 
        int32_t *l_13 = (void*)0;
        uint64_t *l_21[4][4] = {{&g_22,&g_22,&g_22,&g_22},{&g_22,&g_22,&g_22,&g_22},{&g_22,&g_22,&g_22,&g_22},{&g_22,&g_22,&g_22,&g_22}};
        int i, j;
        g_24 |= func_3(((g_22 ^= func_5((((safe_sub_func_uint64_t_u_u(((l_14 = g_12) && 0x0184DCC8), ((((l_2 == 0xB2C41AC3DC99CDF9LL) , g_15) != &g_16) >= (*g_15)))) || l_17) , 0U), g_12, (*g_15), g_16)) || g_16));
    }
    else
    { 
        int16_t l_30 = (-1);
        uint16_t *l_38[4][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0}};
        int8_t *l_41[1];
        uint8_t *l_43 = &g_44;
        int32_t l_45[1];
        uint16_t l_53 = 0U;
        int32_t l_88 = 4;
        uint16_t l_97 = 65527U;
        int i, j;
        for (i = 0; i < 1; i++)
            l_41[i] = &g_42;
        for (i = 0; i < 1; i++)
            l_45[i] = 0x14EFC710;
lbl_68:
        l_45[0] = (((safe_mod_func_int8_t_s_s(((((*l_43) = (safe_mul_func_uint16_t_u_u((g_16 && ((g_42 |= ((safe_unary_minus_func_int64_t_s(l_30)) < (safe_rshift_func_uint16_t_u_u((l_14 |= (!((safe_rshift_func_int64_t_s_u((safe_lshift_func_int8_t_s_s(1, 6)), 37)) >= 5U))), l_40)))) & 0x1A)), g_12))) != 0x54) != 1), g_24)) , l_30) > g_39);
        if ((g_39 & (((((safe_mul_func_uint16_t_u_u(0U, (!((~((*l_43) = g_12)) ^ l_50)))) , l_40) < g_16) ^ g_24) < g_16)))
        { 
            uint16_t l_54 = 0xB579;
            int64_t *l_55 = &g_24;
            int32_t l_61 = 0x929B4DB5;
            (*g_15) = (safe_rshift_func_uint8_t_u_u((((*l_55) |= (l_53 > l_54)) | l_54), g_22));
            for (l_2 = 0; (l_2 < 13); ++l_2)
            { 
                int32_t *l_58 = (void*)0;
                int32_t *l_59[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_59[i] = &l_14;
                --g_62;
                return l_54;
            }
        }
        else
        { 
            int32_t **l_65[3];
            int i;
            for (i = 0; i < 3; i++)
                l_65[i] = &g_15;
            g_15 = &l_14;
            for (g_12 = 23; (g_12 < 3); g_12 = safe_sub_func_int64_t_s_s(g_12, 9))
            { 
                uint32_t *l_75 = &l_2;
                int64_t *l_76 = (void*)0;
                int64_t *l_77[4] = {&g_24,&g_24,&g_24,&g_24};
                int32_t l_78 = 1;
                int32_t l_80[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_80[i] = 2;
                if (l_14)
                    goto lbl_68;
                g_16 ^= ((*g_15) |= ((l_45[0] = (safe_div_func_int32_t_s_s(0, (safe_unary_minus_func_int16_t_s((((+((((*l_75) = (safe_sub_func_int32_t_s_s((0U == (18446744073709551610U > g_62)), l_45[0]))) , &l_40) == &g_42)) != g_42) ^ g_39)))))) == 0xBD28F7D061DAE56BLL));
                --g_90;
            }
            for (g_24 = (-1); (g_24 >= 3); ++g_24)
            { 
                int32_t *l_95 = &l_89[1];
                int32_t l_96[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_96[i] = 0x177FA41F;
                g_15 = l_95;
                g_15 = &g_16;
                l_97++;
            }
        }
    }
    l_84 &= (((safe_lshift_func_uint16_t_u_u((g_102 != ((((((((~((g_39 ^ (((l_109[2][0] = (safe_rshift_func_uint32_t_u_s((l_108 != l_108), l_87))) && 5U) != 1U)) == l_86)) | 1) != l_110) < g_39) > l_110) | l_81) , l_111[0][0]) , &l_50)), l_89[1])) && (*g_15)) >= g_39);
    (*g_15) &= ((safe_sub_func_uint32_t_u_u(0x63DA6633, (0xDC2A9FDAC8820D8BLL | (-4)))) >= 0x2AA995AD63A6EC57LL);
    return l_89[1];
}

static int32_t  func_3(uint8_t  p_4)
{ 
    uint32_t l_23 = 4294967292U;
    (*g_15) = (*g_15);
    return l_23;
}

static uint64_t  func_5(uint64_t  p_6, int64_t  p_7, int32_t  p_8, uint32_t  p_9)
{ 
    const uint8_t l_18 = 0xBD;
    (*g_15) = ((g_12 || (l_18 == (-1))) || l_18);
    (*g_15) = ((g_12 , 1U) != (safe_mul_func_int32_t_s_s((p_6 >= 0), 0x638299FA)));
    return g_16;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_16, "g_16", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_79[i][j], "g_79[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_90, "g_90", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
