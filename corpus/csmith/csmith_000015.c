/* Csmith 0cdc710315cf seed 15, flattened by scripts/csmith_flatten.py */
// Options:   --seed 15 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
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
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint32_t  f0;
   int32_t  f1;
};

static uint8_t g_4 = 0U;
static int32_t g_6 = (-9);
static int32_t g_9 = (-6);
static uint16_t g_15 = 65535U;
static int16_t g_40 = 0xB7D8;
static uint8_t g_113[1][1] = {{0x34}};

static const int32_t  func_1(void);
static const uint8_t  func_20(uint32_t  p_21, int32_t  p_22, int32_t * p_23);
static union U0  func_25(int32_t * p_26, int32_t  p_27);

static const int32_t  func_1(void)
{ 
    int32_t *l_5 = &g_6;
    int32_t *l_7 = (void*)0;
    int32_t *l_8 = &g_9;
    if (((*l_8) = ((safe_mul_func_uint8_t_u_u(((((*l_5) = g_4) || g_4) > g_4), 0xFE)) != 0x7865)))
    { 
        int32_t *l_10 = (void*)0;
        int32_t *l_11 = &g_9;
        int32_t *l_12 = &g_6;
        int32_t *l_13 = &g_9;
        int32_t *l_14 = &g_9;
        g_15++;
    }
    else
    { 
        int8_t l_101 = 0xD7;
        if (g_6)
        { 
            int32_t *l_24 = (void*)0;
            uint16_t l_84 = 0xAC6B;
            uint8_t l_87[1];
            int i;
            for (i = 0; i < 1; i++)
                l_87[i] = 0x32;
            (*l_8) = (g_15 , (((safe_add_func_int8_t_s_s((-1), func_20(g_4, g_6, l_24))) || g_15) ^ l_84));
            for (g_9 = (-27); (g_9 == (-19)); ++g_9)
            { 
                return g_9;
            }
            for (l_84 = 0; (l_84 <= 0); l_84 += 1)
            { 
                int16_t *l_100 = &g_40;
                uint16_t *l_102 = &g_15;
                int32_t **l_103 = &l_24;
                (*l_8) = g_6;
                (*l_103) = (((*l_102) = (safe_mod_func_int32_t_s_s(((safe_add_func_int64_t_s_s((safe_mod_func_uint8_t_u_u((safe_mod_func_int16_t_s_s(((*l_100) = (((safe_rshift_func_uint16_t_u_u(g_15, 14)) , (safe_div_func_uint64_t_u_u(0x50235B3423B0CAFCLL, g_4))) , 0xB382)), 1)), l_101)), g_4)) , 0x7ECFC28C), l_101))) , &g_6);
                return g_4;
            }
        }
        else
        { 
            int8_t l_110 = 0xEF;
            int32_t l_112 = 0xB7C037E6;
            for (l_101 = (-5); (l_101 > (-21)); l_101 = safe_sub_func_int32_t_s_s(l_101, 5))
            { 
                int64_t l_111 = 1;
                l_111 ^= (safe_add_func_int16_t_s_s((g_40 |= 0), (g_6 || (safe_add_func_uint64_t_u_u((&g_15 != &g_15), l_110)))));
                l_112 = l_110;
            }
            for (g_4 = 0; (g_4 <= 0); g_4 += 1)
            { 
                return g_4;
            }
        }
    }
    (*l_8) |= ((safe_rshift_func_uint8_t_u_s((+(safe_div_func_int64_t_s_s((safe_lshift_func_uint8_t_u_s((*l_5), g_15)), (safe_div_func_int8_t_s_s(g_6, g_113[0][0]))))), g_40)) == g_6);
    return (*l_5);
}

static const uint8_t  func_20(uint32_t  p_21, int32_t  p_22, int32_t * p_23)
{ 
    int16_t *l_34 = (void*)0;
    int16_t *l_35 = (void*)0;
    int16_t *l_36[2][1];
    int32_t l_37[2][4] = {{0xBFAEAD22,0x5E19F8C5,0xBFAEAD22,0x5E19F8C5},{0xBFAEAD22,0x5E19F8C5,0xBFAEAD22,0x5E19F8C5}};
    int64_t l_51[2];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_36[i][j] = (void*)0;
    }
    for (i = 0; i < 2; i++)
        l_51[i] = 4;
    if ((func_25(&g_9, (p_22 = (((l_37[1][0] = (safe_rshift_func_uint32_t_u_u((((g_9 ^ (safe_sub_func_int16_t_s_s(((safe_add_func_uint32_t_u_u((p_21 = ((p_21 > g_15) ^ g_15)), 0x07976B5A)) & 255U), 0xC342))) , (-1)) & 0x89), g_9))) , 255U) >= g_6))) , 1))
    { 
        int8_t l_47[4];
        int16_t * const l_55[4][4] = {{&g_40,&g_40,&g_40,(void*)0},{&g_40,&g_40,&g_40,&g_40},{&g_40,&g_40,&g_40,&g_40},{&g_40,&g_40,(void*)0,&g_40}};
        int i, j;
        for (i = 0; i < 4; i++)
            l_47[i] = 1;
        for (p_22 = 0; (p_22 <= 0); p_22 += 1)
        { 
            int64_t l_46 = 0x3231966BA0539261LL;
            int8_t *l_48 = &l_47[1];
            int32_t *l_49 = (void*)0;
            int32_t *l_50[1];
            int16_t * const l_56[4] = {&g_40,&g_40,&g_40,&g_40};
            int16_t **l_65 = &l_34;
            int i;
            for (i = 0; i < 1; i++)
                l_50[i] = &l_37[1][0];
            if ((l_51[0] = (safe_add_func_int8_t_s_s(0x0F, ((*l_48) = ((safe_div_func_int32_t_s_s(g_6, l_46)) < l_47[2]))))))
            { 
                uint64_t l_52 = 0x842353725AC95004LL;
                int64_t *l_64 = &l_51[0];
                --l_52;
                g_6 = (0xFE || (((l_55[1][2] != l_56[3]) , g_15) || 0x59));
                l_65 = (((((safe_rshift_func_uint64_t_u_u((+((*l_64) = ((g_9 > (safe_lshift_func_uint64_t_u_u((safe_add_func_uint64_t_u_u(g_15, 4U)), 1))) , p_22))), p_22)) , p_21) , g_6) , g_4) , l_65);
            }
            else
            { 
                int32_t **l_66 = &l_50[0];
                int16_t *l_67 = &g_40;
                (*l_66) = &p_22;
                g_9 ^= ((*l_65) == l_67);
            }
            for (l_46 = 0; (l_46 <= 0); l_46 += 1)
            { 
                int64_t * const l_70 = &l_51[0];
                int32_t l_71[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_71[i] = 0xDE5592ED;
                l_71[0] = (safe_lshift_func_int8_t_s_s(((l_70 == &l_51[1]) ^ (p_21 == g_15)), 2));
            }
        }
        for (p_21 = 0; (p_21 > 57); p_21 = safe_add_func_int64_t_s_s(p_21, 2))
        { 
            int32_t *l_75[1];
            int32_t **l_74 = &l_75[0];
            int i;
            for (i = 0; i < 1; i++)
                l_75[i] = &g_6;
            (*l_74) = &g_6;
            for (g_40 = 1; (g_40 >= 0); g_40 -= 1)
            { 
                uint8_t *l_79 = &g_4;
                uint16_t *l_82 = (void*)0;
                int32_t l_83 = 0xB9FEC4F7;
                int i;
                (**l_74) = ((+(safe_lshift_func_uint8_t_u_u(((*l_79) = l_51[g_40]), 3))) ^ ((safe_rshift_func_uint8_t_u_s(p_21, ((l_83 |= ((0x4A74 <= l_51[g_40]) < 1)) ^ (**l_74)))) ^ (-1)));
            }
        }
    }
    else
    { 
        return g_9;
    }
    return g_6;
}

static union U0  func_25(int32_t * p_26, int32_t  p_27)
{ 
    uint64_t l_38[1];
    int16_t *l_39 = &g_40;
    union U0 l_41 = {0x21CA7BC4};
    int i;
    for (i = 0; i < 1; i++)
        l_38[i] = 1U;
    (*p_26) |= (l_38[0] != ((*l_39) = 0));
    return l_41;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_113[i][j], "g_113[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
