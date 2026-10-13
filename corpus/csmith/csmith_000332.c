/* Csmith 0cdc710315cf seed 332, flattened by scripts/csmith_flatten.py */
// Options:   --seed 332 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
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
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int16_t g_2 = 0x176B;
static uint16_t g_4 = 0xAA9A;
static int32_t g_48 = 0xB30732BA;
static int32_t *g_50 = (void*)0;
static int32_t **g_49 = &g_50;
static int64_t g_54 = 0;
static uint64_t g_58 = 1U;
static int32_t g_61 = 0x80411BC3;
static uint32_t g_77 = 0xFC64312F;
static int32_t g_85 = (-5);
static int32_t g_86 = 1;
static uint16_t g_87 = 65531U;

static uint8_t  func_1(void);
static int8_t  func_9(int64_t  p_10, int32_t * p_11, int8_t  p_12);
static uint32_t  func_17(int32_t * p_18, int64_t  p_19, uint32_t  p_20);

static uint8_t  func_1(void)
{ 
    uint8_t l_59[3][3];
    int32_t l_67 = 0x298B2365;
    int32_t *l_91[4] = {&g_48,&g_48,&g_48,&g_48};
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
            l_59[i][j] = 255U;
    }
    if (g_2)
    { 
        int32_t *l_3 = (void*)0;
        int32_t l_63 = 0x3153ED87;
        g_4++;
        if (g_4)
        { 
            int32_t *l_60 = &g_61;
            l_63 &= (safe_lshift_func_int8_t_s_u((g_4 , func_9((safe_sub_func_int8_t_s_s((safe_div_func_int8_t_s_s((g_2 < (func_17(((safe_mul_func_uint16_t_u_u((g_2 <= 0xEDCA8FEC), g_4)) , (void*)0), g_4, g_2) >= 1U)), l_59[2][1])), (-10))), l_60, g_61)), g_2));
        }
        else
        { 
            int32_t ** const l_64 = &g_50;
            int64_t *l_68[4];
            int32_t *l_69 = &g_61;
            int i;
            for (i = 0; i < 4; i++)
                l_68[i] = &g_54;
            (*l_69) = ((l_64 != &g_50) , (((g_54 = (safe_mul_func_int16_t_s_s(g_2, l_67))) , g_4) >= g_4));
            (*l_64) = &l_63;
        }
    }
    else
    { 
        uint8_t l_79 = 247U;
        int32_t l_82 = 0xBAF72093;
        int32_t *l_90[1];
        int i;
        for (i = 0; i < 1; i++)
            l_90[i] = &g_85;
        for (g_58 = 19; (g_58 <= 39); g_58 = safe_add_func_int8_t_s_s(g_58, 7))
        { 
            uint32_t *l_76[1];
            int32_t l_78 = 0xA031552A;
            int32_t *l_83 = &l_67;
            int32_t *l_84[3];
            int i;
            for (i = 0; i < 1; i++)
                l_76[i] = &g_77;
            for (i = 0; i < 3; i++)
                l_84[i] = &g_48;
            l_67 |= (safe_mod_func_uint8_t_u_u((((safe_lshift_func_uint32_t_u_u((--l_79), (((0xE0AFDD74 <= 0U) == (l_59[2][1] , g_54)) <= 0))) != g_48) & l_82), g_54));
            g_87++;
        }
        g_61 = ((g_85 || g_85) ^ (g_85 && 0x48F2706978B3CF74LL));
    }
    g_48 = 0x3E519B98;
    return g_87;
}

static int8_t  func_9(int64_t  p_10, int32_t * p_11, int8_t  p_12)
{ 
    int16_t l_62[4] = {0x4853,0x4853,0x4853,0x4853};
    int i;
    return l_62[1];
}

static uint32_t  func_17(int32_t * p_18, int64_t  p_19, uint32_t  p_20)
{ 
    int32_t *l_23 = (void*)0;
    int64_t l_24 = 0xF93BD5DE1FC65786LL;
    uint16_t l_36[2][2] = {{0xC0C7,0xC0C7},{0xC0C7,0xC0C7}};
    int i, j;
    l_24 ^= (-1);
    for (l_24 = 0; (l_24 >= (-10)); l_24 = safe_sub_func_int16_t_s_s(l_24, 8))
    { 
        int32_t l_33[1];
        uint16_t *l_34 = &g_4;
        int i;
        for (i = 0; i < 1; i++)
            l_33[i] = 0xB63E91CB;
        if ((0x05D7 <= ((*l_34) = (g_2 & (((safe_add_func_int32_t_s_s((((((l_33[0] = ((safe_rshift_func_uint64_t_u_u((((safe_add_func_uint16_t_u_u(3U, 0U)) == p_19) & l_24), l_33[0])) == p_20)) , 0xF303) <= p_20) ^ l_24) & g_4), 0xCAF1E2CD)) || l_33[0]) | p_20)))))
        { 
            uint16_t l_35 = 0x6F0D;
            if (g_2)
                break;
            return l_35;
        }
        else
        { 
            return g_2;
        }
    }
    for (p_20 = 0; (p_20 <= 1); p_20 += 1)
    { 
        const uint32_t l_45 = 5U;
        uint16_t *l_46 = &g_4;
        int32_t *l_47 = &g_48;
        for (l_24 = 0; (l_24 <= 1); l_24 += 1)
        { 
            for (g_4 = 0; (g_4 <= 1); g_4 += 1)
            { 
                return p_20;
            }
        }
        (*l_47) ^= (((((((safe_rshift_func_uint8_t_u_u((safe_lshift_func_int16_t_s_u(((((l_45 , ((void*)0 != l_46)) , l_46) == &l_36[1][1]) <= g_2), g_4)), g_4)) || l_45) >= 0U) ^ 0U) , g_4) , 0x8A24E228) , l_45);
        g_49 = &l_23;
        for (l_24 = 0; (l_24 <= 1); l_24 += 1)
        { 
            int64_t *l_53 = &g_54;
            uint64_t *l_57 = &g_58;
            (*l_47) = (((safe_rshift_func_int8_t_s_s((((*l_53) = p_19) != ((*l_57) |= ((*l_47) | (safe_sub_func_uint64_t_u_u((g_2 & p_19), g_4))))), 5)) <= (*l_47)) < (-1));
        }
    }
    return g_2;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_54, "g_54", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_86, "g_86", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
