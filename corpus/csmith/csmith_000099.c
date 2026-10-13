/* Csmith 0cdc710315cf seed 99, flattened by scripts/csmith_flatten.py */
// Options:   --seed 99 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
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
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
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
   uint16_t  f0;
   const uint64_t  f1;
   uint32_t  f2;
};

struct S1 {
   struct S0  f0;
   const int8_t  f1;
};

static int32_t g_2[2] = {0xFE6E3245,0xFE6E3245};
static int32_t g_3 = 7;
static struct S1 g_13 = {{65526U,0xE2294BB489F7A9D2LL,4294967287U},0xD3};
static int64_t g_17 = 0x025004286A4ABFF1LL;
static int8_t g_21 = 1;
static int16_t g_33 = 0x4118;
static int32_t *g_35 = (void*)0;
static uint64_t g_93 = 0x208E89F23B65C4ECLL;
static uint32_t g_113 = 7U;
static uint16_t g_127 = 65527U;

static uint8_t  func_1(void);
static struct S1  func_4(int16_t  p_5);
static int16_t  func_6(uint64_t  p_7, struct S1  p_8, const uint16_t  p_9, int64_t  p_10, uint16_t  p_11);

static uint8_t  func_1(void)
{ 
    uint16_t l_12 = 65535U;
    int64_t l_91 = (-5);
    int32_t l_94 = 0x2ECD4103;
    for (g_3 = 1; (g_3 >= 0); g_3 -= 1)
    { 
        int16_t l_49 = 1;
        int32_t l_50 = 0xB52FFB46;
        int8_t *l_60 = &g_21;
        uint8_t l_61 = 0xCB;
        uint32_t l_110 = 4294967294U;
        int i;
        l_50 &= ((((func_4(func_6((g_2[g_3] < ((l_12 && g_2[g_3]) && g_2[1])), g_13, g_2[1], g_2[g_3], g_2[0])) , g_13.f0.f1) & 0x3E14DAAE8D137083LL) & l_49) < 0U);
        if ((safe_div_func_uint8_t_u_u(((safe_add_func_uint16_t_u_u((0xBAB1DDA348D5C880LL == ((((((l_12 > (safe_mod_func_int8_t_s_s(((*l_60) = ((!((safe_mod_func_int64_t_s_s(g_13.f1, 1)) , 2)) >= 18446744073709551607U)), g_13.f0.f0))) & g_33) >= g_13.f0.f0) == l_12) & g_13.f0.f0) != l_12)), 0x09C3)) != l_61), 0xB9)))
        { 
            const uint16_t l_71 = 65535U;
            int64_t *l_72 = &g_17;
            const int32_t *l_74 = &g_2[g_3];
            const int32_t **l_73 = &l_74;
            uint64_t *l_92[3][2] = {{&g_93,&g_93},{&g_93,&g_93},{&g_93,&g_93}};
            int i, j;
            (*l_73) = (((0 < (safe_mod_func_uint64_t_u_u((safe_mul_func_int64_t_s_s((safe_add_func_int32_t_s_s(((+((*l_72) = ((safe_lshift_func_uint16_t_u_u(((((l_71 , (g_33 == 1U)) == g_33) == l_71) != l_12), l_71)) < g_13.f1))) ^ l_12), (*g_35))), 0)), l_12))) ^ (-1)) , (void*)0);
            if (((safe_sub_func_int16_t_s_s(((safe_rshift_func_uint32_t_u_u(((safe_add_func_int16_t_s_s((safe_rshift_func_uint64_t_u_s((l_94 &= (((void*)0 != (*l_73)) < ((((((safe_div_func_uint8_t_u_u((safe_lshift_func_int8_t_s_s((safe_div_func_uint64_t_u_u((safe_div_func_uint64_t_u_u((g_3 <= g_33), l_91)), g_13.f1)), 6)), g_13.f0.f2)) & 0xBBCF58CC) > g_13.f0.f2) != 8) <= 0xEDFBC0B84261728ALL) && (*g_35)))), 56)), 3U)) ^ l_49), 9)) || 0xBB76), g_13.f0.f0)) < g_13.f0.f2))
            { 
                int32_t l_103 = (-1);
                int64_t *l_111 = &l_91;
                int32_t *l_112 = (void*)0;
                g_113 &= ((((safe_add_func_int8_t_s_s((((*l_111) = (safe_rshift_func_uint32_t_u_u((safe_mul_func_int64_t_s_s(((safe_lshift_func_int8_t_s_u(l_103, 3)) == ((((safe_sub_func_uint64_t_u_u(g_93, ((*l_72) = (safe_div_func_int32_t_s_s((safe_mod_func_int32_t_s_s(l_12, 0x6FEE090E)), l_61))))) >= l_110) || 0U) <= 0xCABD97907AF38478LL)), g_2[g_3])), 11))) != l_103), l_94)) <= g_93) | 1U) != (-10));
                if (l_91)
                    continue;
            }
            else
            { 
                int32_t *l_120 = &l_50;
                (*l_120) ^= (safe_mod_func_uint8_t_u_u(((safe_sub_func_uint64_t_u_u(g_2[g_3], g_13.f0.f2)) > (safe_mod_func_uint8_t_u_u(g_33, 1U))), 0x85));
                return l_12;
            }
        }
        else
        { 
            uint64_t *l_123 = &g_93;
            uint16_t *l_124 = &g_13.f0.f0;
            uint16_t *l_125 = (void*)0;
            uint16_t *l_126 = &g_127;
            if ((safe_rshift_func_uint64_t_u_u(((*l_123) = l_12), (((*l_126) = ((*l_124) = 65528U)) || (g_13.f0.f2 > 0x1D493DC333CF46C2LL)))))
            { 
                return g_17;
            }
            else
            { 
                uint32_t l_128 = 0x1FF7FB00;
                int32_t *l_129 = &g_3;
                int32_t **l_130 = &l_129;
                (*g_35) = l_128;
                (*l_130) = l_129;
            }
        }
    }
    return g_13.f0.f2;
}

static struct S1  func_4(int16_t  p_5)
{ 
    struct S1 l_48 = {{65535U,5U,5U},0x01};
    return l_48;
}

static int16_t  func_6(uint64_t  p_7, struct S1  p_8, const uint16_t  p_9, int64_t  p_10, uint16_t  p_11)
{ 
    int16_t l_23 = 0xB7D4;
    int32_t * const l_30 = &g_3;
    int16_t l_34[1][3];
    int32_t *l_36 = &g_2[0];
    int32_t *l_37 = &g_2[0];
    int32_t *l_38 = &g_2[0];
    int32_t *l_39 = &g_2[0];
    int32_t *l_40 = (void*)0;
    int32_t *l_41 = (void*)0;
    int32_t *l_42 = &g_2[0];
    int32_t *l_43 = &g_2[0];
    int32_t *l_44[4][1] = {{&g_2[0]},{&g_3},{&g_2[0]},{&g_3}};
    uint8_t l_45 = 0x5E;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_34[i][j] = (-7);
    }
    for (p_8.f0.f2 = 0; (p_8.f0.f2 <= 1); p_8.f0.f2 += 1)
    { 
        int32_t *l_14 = (void*)0;
        int32_t *l_15 = (void*)0;
        int32_t *l_16 = (void*)0;
        int8_t *l_20 = &g_21;
        int i;
        g_17 |= g_2[p_8.f0.f2];
        if ((((*l_20) ^= (safe_mod_func_uint16_t_u_u(p_11, g_13.f0.f1))) >= (+l_23)))
        { 
            for (p_7 = 0; p_7 < 2; p_7 = p_7 + 1)
            {
                g_2[p_7] = 4;
            }
        }
        else
        { 
            int32_t *l_25 = &g_2[p_8.f0.f2];
            (*l_25) ^= ((~g_13.f0.f0) >= 0x4C01E07AD46D2320LL);
        }
    }
    g_35 = (((((((p_8.f0.f2 = (safe_lshift_func_uint8_t_u_u(((safe_lshift_func_uint8_t_u_s(((((l_30 != ((0x91 | (safe_sub_func_uint8_t_u_u(0x4F, 0xAB))) , l_30)) && 0U) > (*l_30)) | (-1)), g_13.f0.f1)) , p_8.f1), g_33))) | (*l_30)) < (*l_30)) & p_8.f0.f1) & g_3) ^ l_34[0][1]) , &g_2[1]);
    --l_45;
    return g_33;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_13.f0.f0, "g_13.f0.f0", print_hash_value);
    transparent_crc(g_13.f0.f1, "g_13.f0.f1", print_hash_value);
    transparent_crc(g_13.f0.f2, "g_13.f0.f2", print_hash_value);
    transparent_crc(g_13.f1, "g_13.f1", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    transparent_crc(g_113, "g_113", print_hash_value);
    transparent_crc(g_127, "g_127", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
