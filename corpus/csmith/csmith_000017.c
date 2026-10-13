/* Csmith 0cdc710315cf seed 17, flattened by scripts/csmith_flatten.py */
// Options:   --seed 17 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 0xC204E2DB;
static int32_t g_30 = 0x9338886A;
static uint32_t g_39 = 0x72157666;
static int32_t g_49 = 5;
static int32_t *g_48 = &g_49;
static int8_t g_55 = (-5);
static uint8_t g_56 = 0x26;
static int32_t g_60 = 0xF1497EA9;
static uint16_t g_77 = 65535U;
static int64_t g_78[2] = {1,1};
static int32_t g_107 = 0xF19E1DE5;

static int16_t  func_1(void);
static uint32_t  func_5(int32_t  p_6, int8_t  p_7, uint32_t  p_8);
static int16_t  func_13(int64_t  p_14, int64_t  p_15, int16_t  p_16, uint32_t  p_17, int32_t  p_18);

static int16_t  func_1(void)
{ 
    int64_t l_9 = 0x1220985A9576C3A0LL;
    uint64_t l_111[4] = {0x4CA015B29D1CA74ALL,0x4CA015B29D1CA74ALL,0x4CA015B29D1CA74ALL,0x4CA015B29D1CA74ALL};
    int i;
    for (g_2 = 0; (g_2 > 10); g_2++)
    { 
        if ((func_5(g_2, l_9, l_9) && g_49))
        { 
            int32_t *l_109 = &g_60;
            int32_t *l_110 = &g_107;
            uint32_t *l_116[4][1];
            uint32_t l_117 = 0U;
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 1; j++)
                    l_116[i][j] = (void*)0;
            }
            l_111[1]++;
            l_117 = ((safe_mul_func_uint32_t_u_u((g_39 = ((*l_109) ^= 0xBD095E97)), (((*l_110) < g_78[0]) , 0x259FC79B))) , 0x2826EC48);
        }
        else
        { 
            uint32_t l_120 = 4294967290U;
            int32_t *l_121 = &g_49;
            (*l_121) |= (safe_rshift_func_int8_t_s_s((l_120 < ((l_111[1] ^ (l_111[1] , l_120)) , 18446744073709551615U)), 4));
            for (g_55 = 0; (g_55 <= 1); g_55 += 1)
            { 
                int32_t *l_122[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_122[i] = &g_60;
                g_60 |= ((*l_121) = (g_78[g_55] | l_111[(g_55 + 1)]));
            }
        }
    }
    return l_9;
}

static uint32_t  func_5(int32_t  p_6, int8_t  p_7, uint32_t  p_8)
{ 
    uint16_t l_10[3][2] = {{1U,0U},{0U,1U},{0U,0U}};
    int32_t l_105 = 0x0913F082;
    int32_t **l_108 = &g_48;
    int i, j;
    for (p_7 = 0; (p_7 <= 1); p_7 += 1)
    { 
        int16_t l_31 = (-1);
        int32_t l_89[4][4] = {{1,0xCDC4137E,0xCDC4137E,1},{0x2431EA72,0xCDC4137E,6,0xCDC4137E},{0xCDC4137E,0xF347F2DB,6,6},{0x2431EA72,0x2431EA72,0xCDC4137E,6}};
        uint8_t l_90 = 1U;
        int32_t *l_93 = (void*)0;
        int32_t *l_94 = (void*)0;
        int32_t *l_95 = &g_30;
        int i, j;
        for (p_8 = 0; (p_8 <= 1); p_8 += 1)
        { 
            int16_t l_32 = 0;
            int32_t *l_86 = (void*)0;
            int32_t *l_87 = (void*)0;
            int32_t *l_88[4][3] = {{&g_60,&g_60,&g_60},{&g_30,&g_30,&g_30},{&g_60,&g_60,&g_60},{&g_30,&g_30,&g_30}};
            int i, j;
            for (p_6 = 1; (p_6 >= 0); p_6 -= 1)
            { 
                int32_t *l_28 = (void*)0;
                int32_t *l_29 = &g_30;
                int i, j;
                g_49 &= (l_10[(p_7 + 1)][p_8] && (0x3411 == func_13((safe_add_func_int64_t_s_s((((((safe_rshift_func_int8_t_s_u((((safe_add_func_int64_t_s_s((safe_unary_minus_func_uint64_t_u((((*l_29) &= (safe_mul_func_int32_t_s_s(p_8, 4294967289U))) == 0))), (-1))) ^ l_31) <= (-1)), l_31)) , 0x0E) != (-4)) != l_32) || 255U), p_8)), g_2, p_6, g_2, g_2)));
            }
            p_6 = (g_49 ^= (+0));
            l_90--;
        }
        (*l_95) &= l_89[0][1];
        (*l_95) = (safe_rshift_func_uint16_t_u_u((&g_39 != (void*)0), 11));
        for (g_49 = 1; (g_49 >= 0); g_49 -= 1)
        { 
            uint64_t l_98[4];
            int i;
            for (i = 0; i < 4; i++)
                l_98[i] = 0xC1E79BFF94FF55BDLL;
            if ((l_98[2] , (p_7 , (-10))))
            { 
                const int64_t l_106[4][2] = {{0xDD39D0E3EE8C007ELL,0xDD39D0E3EE8C007ELL},{0xDD39D0E3EE8C007ELL,0xDD39D0E3EE8C007ELL},{0xDD39D0E3EE8C007ELL,0xDD39D0E3EE8C007ELL},{0xDD39D0E3EE8C007ELL,0xDD39D0E3EE8C007ELL}};
                int i, j;
                (*l_95) |= (p_6 >= 4294967289U);
                (*l_95) &= ((~0U) , (p_6 = ((safe_mod_func_uint64_t_u_u(((0xC65540566F1E831FLL < (!(safe_lshift_func_int8_t_s_u((((((l_105 = (((-10) >= 0xCF52F5978B88EAD4LL) > g_56)) >= g_78[1]) < p_7) || l_106[3][1]) <= 0xF12CA17A55E9D0F7LL), g_39)))) && g_39), l_98[3])) , g_107)));
            }
            else
            { 
                return l_105;
            }
            l_108 = &l_95;
            (*l_95) |= 0x57F9C6BD;
        }
    }
    return p_6;
}

static int16_t  func_13(int64_t  p_14, int64_t  p_15, int16_t  p_16, uint32_t  p_17, int32_t  p_18)
{ 
    uint16_t l_33 = 4U;
    uint32_t *l_38 = &g_39;
    uint64_t l_52[4];
    int32_t l_54 = 0xFEFD6E51;
    int32_t l_61 = 9;
    const uint32_t *l_65[3];
    int i;
    for (i = 0; i < 4; i++)
        l_52[i] = 18446744073709551615U;
    for (i = 0; i < 3; i++)
        l_65[i] = (void*)0;
    l_33--;
    p_18 ^= (safe_mod_func_int32_t_s_s((((*l_38) &= l_33) != (((safe_add_func_uint64_t_u_u((p_14 , 0x21494FB389FA1839LL), l_33)) , &g_30) != (void*)0)), g_2));
    for (p_15 = 0; (p_15 >= (-25)); p_15 = safe_sub_func_int8_t_s_s(p_15, 4))
    { 
        uint32_t l_82 = 1U;
        for (g_30 = 0; (g_30 <= 5); g_30++)
        { 
            int32_t **l_46 = (void*)0;
            int32_t **l_47[3];
            int i;
            for (i = 0; i < 3; i++)
                l_47[i] = (void*)0;
            g_48 = &g_30;
        }
        for (p_14 = (-19); (p_14 == (-13)); ++p_14)
        { 
            uint16_t *l_59[3][1];
            uint64_t l_62[1][4];
            uint32_t *l_66 = &g_39;
            int32_t l_68 = 0;
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_59[i][j] = &l_33;
            }
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 4; j++)
                    l_62[i][j] = 0xF5F6F594A39B7B3FLL;
            }
            for (p_16 = 0; (p_16 <= 3); p_16 += 1)
            { 
                int32_t **l_53[4][3] = {{&g_48,&g_48,&g_48},{(void*)0,&g_48,&g_48},{(void*)0,(void*)0,&g_48},{&g_48,&g_48,&g_48}};
                int i, j;
                g_48 = (void*)0;
                if (l_52[p_16])
                    break;
                g_56++;
            }
            if ((((l_62[0][2]--) , l_65[1]) != l_66))
            { 
                int32_t *l_67[4] = {&l_61,&l_61,&l_61,&l_61};
                int i;
                p_18 &= p_15;
                l_68 &= p_18;
            }
            else
            { 
                uint16_t l_69 = 0x5055;
                const int32_t l_79[3] = {0,0,0};
                uint64_t *l_80 = &l_62[0][3];
                uint64_t *l_81 = &l_52[0];
                int i;
                p_18 = ((l_69 > (safe_mod_func_uint32_t_u_u((((safe_div_func_int64_t_s_s(1, ((*l_81) = ((*l_80) = (((!(((l_68 &= (g_78[0] = (g_77 = ((safe_mod_func_uint16_t_u_u((g_30 , l_69), 0x4B50)) <= p_15)))) == 0x20B4) <= p_14)) <= l_61) && l_79[2]))))) || g_39) & 0x769E), l_79[2]))) , g_2);
                l_82--;
            }
            if (l_54)
                break;
        }
    }
    return p_18;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_78[i], "g_78[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_107, "g_107", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
