/* Csmith 0cdc710315cf seed 304, flattened by scripts/csmith_flatten.py */
// Options:   --seed 304 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
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
/* ---- generated program ---- */
static long __undefined;

union U0 {
   int8_t  f0;
   uint16_t  f1;
   uint32_t  f2;
};

union U1 {
   const uint8_t  f0;
   const int32_t  f1;
};

static int32_t g_12 = 0xA205B290;
static int16_t g_28[3] = {0x1FCC,0x1FCC,0x1FCC};
static union U1 g_32 = {0x22};
static int64_t g_46[2] = {1,1};
static uint64_t g_63[3][4] = {{18446744073709551615U,18446744073709551607U,18446744073709551607U,18446744073709551615U},{18446744073709551607U,18446744073709551615U,18446744073709551607U,18446744073709551607U},{18446744073709551615U,18446744073709551615U,18446744073709551615U,18446744073709551615U}};
static int32_t g_65 = 0x12536DE7;
static int32_t g_68 = 0x9E2962DE;
static uint64_t g_78 = 1U;
static int32_t *g_99 = &g_12;
static int32_t *g_100 = &g_12;
static int32_t g_104 = 0x81310918;
static union U0 **g_124 = (void*)0;

static int32_t  func_1(void);
static uint16_t  func_4(const uint32_t  p_5, uint64_t  p_6, uint32_t  p_7, uint32_t  p_8, const uint32_t  p_9);
static uint64_t  func_24(uint8_t  p_25, int32_t * p_26, int8_t  p_27);

static int32_t  func_1(void)
{ 
    int8_t l_3 = 0x22;
    int32_t *l_103 = &g_104;
    int32_t *l_130 = &g_68;
    (*l_103) ^= ((+((l_3 , (func_4(l_3, ((safe_div_func_int32_t_s_s(g_12, 1U)) , l_3), l_3, g_12, g_12) , g_46[0])) == 18446744073709551615U)) , l_3);
    for (l_3 = 0; (l_3 != 18); ++l_3)
    { 
        int16_t *l_115 = (void*)0;
        int16_t *l_116 = &g_28[0];
        int32_t l_119 = 0xE3454E7B;
        (*l_103) = ((*g_99) = (((*l_116) ^= (+(safe_div_func_uint8_t_u_u((safe_rshift_func_int32_t_s_s((safe_lshift_func_uint8_t_u_s(g_46[1], (~g_32.f0))), (*l_103))), (*l_103))))) < 0x01CF));
        (*l_103) = (safe_mul_func_uint64_t_u_u(l_119, (safe_div_func_uint16_t_u_u((safe_mul_func_uint32_t_u_u(((void*)0 == &g_28[2]), 9U)), l_119))));
        for (g_12 = 2; (g_12 >= 0); g_12 -= 1)
        { 
            union U0 **l_125 = (void*)0;
            g_124 = (l_125 = g_124);
            for (g_104 = 0; (g_104 <= 2); g_104 += 1)
            { 
                int i;
                if (g_28[g_104])
                    break;
                if ((*g_100))
                    continue;
            }
        }
    }
    (*l_130) = ((*g_99) = (((safe_div_func_int64_t_s_s((((((0xDE520ED3B4235E97LL == ((safe_sub_func_uint64_t_u_u(0x93C35E3CEA876B16LL, ((((((*l_103) = 0x53535970) || (*l_103)) && 1U) > 1U) == 2))) & 0x434A)) && g_104) >= (-1)) == 0xF1) != (*g_100)), l_3)) <= (-1)) >= g_63[2][2]));
    return (*l_103);
}

static uint16_t  func_4(const uint32_t  p_5, uint64_t  p_6, uint32_t  p_7, uint32_t  p_8, const uint32_t  p_9)
{ 
    uint32_t l_13 = 4294967294U;
    int32_t *l_14 = &g_12;
    union U0 l_15 = {0x8E};
    uint32_t l_29 = 0x77AE2718;
    int8_t l_69 = 0xE3;
    (*l_14) &= l_13;
    if ((l_15 , (safe_lshift_func_uint8_t_u_u(((safe_mod_func_uint64_t_u_u((safe_mul_func_int16_t_s_s((safe_lshift_func_uint8_t_u_s((((func_24(g_12, l_14, g_12) | g_28[0]) , p_9) > l_29), 1)), p_8)), (-3))) && (*l_14)), 0))))
    { 
        uint16_t l_30 = 0xBF7C;
        int32_t l_70 = (-8);
        int32_t l_71 = 0x9B16E5D1;
        union U1 *l_87 = &g_32;
        union U1 **l_86 = &l_87;
        if ((*l_14))
        { 
            const union U1 **l_62 = (void*)0;
            int32_t l_66 = (-1);
            int32_t l_77[3][4] = {{0x262A889E,0x262A889E,0x262A889E,0x262A889E},{0x262A889E,0x262A889E,0x262A889E,0x262A889E},{0x262A889E,0x262A889E,0x262A889E,0x262A889E}};
            int i, j;
            if (l_30)
            { 
                union U1 *l_31 = &g_32;
                union U1 **l_33 = &l_31;
                (*l_33) = l_31;
            }
            else
            { 
                return g_12;
            }
            for (l_13 = 0; l_13 < 3; l_13 = l_13 + 1)
            {
                g_28[l_13] = (-7);
            }
            if (((g_28[0] , ((p_5 <= ((((safe_div_func_int8_t_s_s((safe_div_func_uint64_t_u_u(((safe_mod_func_int8_t_s_s(((safe_sub_func_uint16_t_u_u(p_7, g_32.f0)) | 0x4071C19CD890F6DALL), 0x0D)) ^ l_30), 0x40C8E82B3F54C3DCLL)), (*l_14))) == p_6) <= g_28[0]) | p_5)) & (*l_14))) != p_8))
            { 
                (*l_14) |= 0xCA727646;
            }
            else
            { 
                union U1 **l_61 = (void*)0;
                uint32_t *l_64[1];
                int32_t *l_67 = &g_68;
                int32_t *l_72 = &g_68;
                int32_t *l_73 = &l_71;
                int32_t *l_74 = &g_12;
                int32_t *l_75 = &g_65;
                int32_t *l_76[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_64[i] = (void*)0;
                for (i = 0; i < 1; i++)
                    l_76[i] = (void*)0;
                (*l_67) ^= (safe_lshift_func_int32_t_s_u(((((((safe_lshift_func_uint8_t_u_u(g_46[1], (safe_add_func_int32_t_s_s(((safe_div_func_int8_t_s_s((safe_sub_func_uint32_t_u_u(9U, (safe_sub_func_uint32_t_u_u((g_65 = (g_63[2][1] = (safe_add_func_uint8_t_u_u(((safe_lshift_func_uint8_t_u_u((((((safe_lshift_func_int64_t_s_u((((void*)0 != l_61) != p_6), 1)) || 5U) , l_62) != (void*)0) == p_9), p_9)) || (*l_14)), l_30)))), g_46[1])))), 0x75)) , l_66), (-1))))) != l_66) > p_6) , (*l_14)) <= l_30) & g_12), g_32.f0));
                g_78--;
                return (*l_67);
            }
        }
        else
        { 
            uint8_t l_92 = 0x20;
            for (p_8 = (-9); (p_8 >= 26); p_8++)
            { 
                int32_t l_85[1];
                union U1 ** const l_88[2][2] = {{&l_87,&l_87},{&l_87,&l_87}};
                int32_t *l_89 = &g_12;
                int32_t *l_90 = &g_65;
                int32_t *l_91[4] = {&g_12,&g_12,&g_12,&g_12};
                union U0 *l_95 = &l_15;
                union U0 **l_96 = &l_95;
                int i, j;
                for (i = 0; i < 1; i++)
                    l_85[i] = 0;
                (*l_14) = ((safe_rshift_func_int16_t_s_u(g_46[1], l_85[0])) <= ((l_86 != l_88[0][0]) , p_7));
                l_92++;
                (*l_96) = l_95;
            }
        }
        for (g_68 = 0; (g_68 <= 29); ++g_68)
        { 
            (*l_14) = (&l_15 == &l_15);
            g_100 = (g_99 = &g_65);
        }
        return g_46[1];
    }
    else
    { 
        (*g_99) = (g_63[2][1] ^ (p_7 | p_5));
    }
    for (g_65 = 1; (g_65 >= 0); g_65 -= 1)
    { 
        int32_t **l_101 = &g_99;
        int32_t **l_102 = &g_99;
        int i;
        l_102 = (l_101 = &g_100);
        return g_46[g_65];
    }
    return p_5;
}

static uint64_t  func_24(uint8_t  p_25, int32_t * p_26, int8_t  p_27)
{ 
    (*p_26) = (((g_12 , (p_27 ^= g_12)) , ((9U | 4294967294U) && 0x11)) , 0xB7B83196);
    return p_25;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_12, "g_12", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_28[i], "g_28[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_32.f0, "g_32.f0", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_46[i], "g_46[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_63[i][j], "g_63[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_68, "g_68", print_hash_value);
    transparent_crc(g_78, "g_78", print_hash_value);
    transparent_crc(g_104, "g_104", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
