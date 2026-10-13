/* Csmith 0cdc710315cf seed 31, flattened by scripts/csmith_flatten.py */
// Options:   --seed 31 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
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
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint64_t  f0;
};

static int32_t g_2 = (-2);
static int32_t g_30 = 0xC3CBAA6F;
static uint16_t g_32 = 0xE3F5;
static uint16_t g_33[1] = {0x89BB};
static int32_t *g_34[3][3] = {{&g_2,&g_2,&g_2},{&g_30,&g_30,&g_30},{&g_2,&g_2,&g_2}};
static uint16_t * const *g_37 = (void*)0;
static union U0 g_67 = {0x6F28A7DEB6559726LL};
static int64_t g_89 = 0;

static int64_t  func_1(void);
static int32_t * func_5(uint64_t  p_6, int8_t  p_7, int32_t * p_8, int32_t * p_9);
static uint16_t  func_13(int32_t * p_14, uint32_t  p_15, const uint64_t  p_16);

static int64_t  func_1(void)
{ 
    uint32_t l_92 = 4294967290U;
    int32_t *l_93[4][2] = {{&g_2,&g_2},{&g_2,&g_2},{&g_2,&g_2},{&g_2,&g_2}};
    int i, j;
    for (g_2 = 18; (g_2 >= 13); g_2--)
    { 
        int64_t l_10 = (-1);
        g_34[2][2] = func_5((g_2 ^ l_10), (l_92 = (safe_rshift_func_uint64_t_u_s((func_13((l_10 , &g_2), l_10, g_2) >= g_2), 49))), l_93[2][0], &g_2);
    }
    return g_33[0];
}

static int32_t * func_5(uint64_t  p_6, int8_t  p_7, int32_t * p_8, int32_t * p_9)
{ 
    int32_t *l_94 = &g_2;
    int32_t **l_95[4];
    uint16_t *l_99 = (void*)0;
    uint16_t **l_98 = &l_99;
    int8_t l_100 = 0x6B;
    int32_t *l_101 = &g_30;
    uint64_t l_102 = 0U;
    int i;
    for (i = 0; i < 4; i++)
        l_95[i] = (void*)0;
    g_34[0][0] = l_94;
    (*l_101) = (((((((safe_rshift_func_uint64_t_u_s(((g_30 >= ((&g_34[0][0] == (l_95[1] = &l_94)) ^ p_6)) , 0x2362835DF1A5E6C3LL), 49)) > (*l_94)) , l_98) == &l_99) == g_67.f0) | p_7) && l_100);
    l_102 |= (*l_101);
    return p_8;
}

static uint16_t  func_13(int32_t * p_14, uint32_t  p_15, const uint64_t  p_16)
{ 
    int8_t l_19 = 0xEF;
    int32_t **l_24 = (void*)0;
    int32_t *l_26 = &g_2;
    int32_t **l_25[3][2];
    uint16_t l_42[4][1] = {{2U},{0xEC5C},{2U},{0xEC5C}};
    int16_t l_46 = 0x5401;
    uint64_t l_71 = 0xE8027EF3DD4F1ACCLL;
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_25[i][j] = &l_26;
    }
    if ((safe_rshift_func_int64_t_s_u((l_19 < ((((((safe_mul_func_uint32_t_u_u((g_2 >= (safe_mod_func_uint32_t_u_u(((p_14 = &g_2) != (void*)0), p_15))), p_16)) & g_2) , g_2) , 0xD66E5DC64F6B903CLL) >= 1U) < g_2)), p_16)))
    { 
        int32_t *l_28 = (void*)0;
        int32_t *l_29 = &g_30;
        uint16_t *l_36[1];
        uint16_t **l_35 = &l_36[0];
        const uint8_t l_44 = 0x2C;
        int32_t l_48 = 0x819277EF;
        int i;
        for (i = 0; i < 1; i++)
            l_36[i] = &g_32;
        for (l_19 = 0; (l_19 <= 1); l_19 += 1)
        { 
            int32_t *l_27[2][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0}};
            uint16_t *l_31 = &g_32;
            uint16_t l_45 = 8U;
            uint8_t l_62 = 0U;
            int i, j;
            l_27[0][1] = (p_14 = (void*)0);
            if ((g_33[0] &= (g_30 ^= (((*l_31) ^= ((l_28 != (l_29 = &g_2)) , g_2)) | 0x6FFA))))
            { 
                const uint16_t **l_38 = (void*)0;
                int32_t *l_39 = &g_30;
                g_34[0][2] = l_28;
                l_38 = ((((l_35 != g_37) != g_32) || 0xF874340CABDBFAD2LL) , (void*)0);
                l_39 = &g_30;
            }
            else
            { 
                g_34[0][2] = &g_30;
                return p_16;
            }
            for (g_32 = 0; (g_32 <= 1); g_32 += 1)
            { 
                int64_t l_43[1][4];
                uint8_t l_47 = 0x0A;
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 4; j++)
                        l_43[i][j] = 0xEAE5249F3923BFA4LL;
                }
                l_45 |= (p_15 || (safe_mul_func_int32_t_s_s(l_42[3][0], ((l_43[0][2] ^ g_2) < l_44))));
                l_46 |= ((*l_29) != g_33[0]);
                if (l_47)
                    continue;
            }
            for (p_15 = 0; (p_15 <= 1); p_15 += 1)
            { 
                int32_t l_49[3];
                uint16_t *l_58 = &l_45;
                int16_t *l_59 = (void*)0;
                int16_t *l_60 = (void*)0;
                int16_t *l_61 = &l_46;
                int i;
                for (i = 0; i < 3; i++)
                    l_49[i] = 9;
                l_49[2] = (l_48 = 0xA8461A3E);
                l_48 = ((safe_sub_func_int64_t_s_s(((((safe_rshift_func_uint16_t_u_s((safe_rshift_func_int64_t_s_s(((g_2 & g_32) || (safe_div_func_uint64_t_u_u((((*l_61) = (&g_32 != l_58)) == 0x6473), 3))), 50)), 6)) , p_15) <= l_49[2]) , 0x38B78E619AEFA9ADLL), l_62)) && (*l_29));
                g_34[2][2] = (l_28 = &g_30);
            }
        }
        for (g_32 = (-6); (g_32 < 4); g_32++)
        { 
            uint32_t l_87[1];
            int32_t l_90 = 0xE0C2A5AD;
            int i;
            for (i = 0; i < 1; i++)
                l_87[i] = 4294967294U;
            for (g_30 = 26; (g_30 == (-17)); g_30 = safe_sub_func_uint16_t_u_u(g_30, 7))
            { 
                const uint8_t l_70 = 1U;
                int64_t *l_88 = &g_89;
                l_71 = (((*l_29) && (((g_67 , (safe_mul_func_int64_t_s_s(g_30, 0xCE3E596A42263F48LL))) == (-1)) < l_70)) , 0x0A0F9AE2);
                l_90 |= ((p_15 ^ (((safe_add_func_int64_t_s_s(((safe_div_func_int8_t_s_s(g_67.f0, (~((*l_88) = (((safe_add_func_uint8_t_u_u(((safe_add_func_int16_t_s_s((safe_lshift_func_int16_t_s_u((safe_div_func_uint16_t_u_u((safe_div_func_uint16_t_u_u(l_87[0], g_32)), 5)), p_15)), p_16)) , g_2), l_87[0])) <= 65526U) | p_15))))) | g_33[0]), g_67.f0)) < l_87[0]) <= p_16)) <= p_16);
            }
        }
    }
    else
    { 
        int8_t l_91 = 0xAC;
        return l_91;
    }
    return p_16;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_33[i], "g_33[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_67.f0, "g_67.f0", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
