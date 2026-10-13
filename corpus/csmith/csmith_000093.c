/* Csmith 0cdc710315cf seed 93, flattened by scripts/csmith_flatten.py */
// Options:   --seed 93 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
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
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
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
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
   const int32_t  f1;
   uint64_t  f2;
   uint16_t  f3;
};

struct S1 {
   const int8_t  f0;
   const uint8_t  f1;
   int32_t  f2;
};

struct S2 {
   const uint8_t  f0;
   struct S0  f1;
};

union U3 {
   uint64_t  f0;
   struct S0  f1;
};

static uint64_t g_12 = 0U;
static struct S0 g_17 = {1U,-6,18446744073709551609U,1U};
static int32_t g_19 = 0x58FB5C4C;
static union U3 g_22 = {1U};
static int64_t g_30 = 1;
static uint32_t g_41 = 0x35667B22;
static struct S1 g_51 = {0x17,0x9B,0x10DA9BE8};
static int64_t g_56 = 0x8C68BE14B93870AFLL;
static uint32_t g_57 = 1U;
static uint16_t g_71 = 0x31BE;
static const uint16_t *g_70 = &g_71;
static int16_t g_87 = 1;
static int64_t g_108 = 4;
static uint16_t g_109 = 0U;
static uint8_t *g_130 = (void*)0;
static uint64_t g_136 = 0xDEF6EF3277B429FFLL;

static uint32_t  func_1(void);
static int32_t  func_2(uint8_t  p_3, uint16_t  p_4, uint16_t  p_5);
static struct S1  func_6(struct S1  p_7, struct S2  p_8, int8_t  p_9, int32_t  p_10, struct S0  p_11);

static uint32_t  func_1(void)
{ 
    uint32_t l_13 = 0xE6FC6088;
    struct S1 l_14 = {0x64,0xB8,0x277EBD00};
    struct S2 l_15[2] = {{0xE5,{0x49,0x68CB667F,0U,0x0CD9}},{0xE5,{0x49,0x68CB667F,0U,0x0CD9}}};
    int16_t l_16[3][4];
    int32_t l_117 = 0x76F4719C;
    int32_t l_119 = 0x2A4309C4;
    int32_t *l_131 = (void*)0;
    int32_t *l_132 = &l_119;
    int32_t *l_133 = &g_51.f2;
    int32_t *l_134 = &l_119;
    int32_t *l_135 = &g_19;
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
            l_16[i][j] = 0x1300;
    }
    if (func_2((func_6(((g_12 && l_13) , l_14), l_15[0], l_15[0].f1.f1, l_16[1][3], g_17) , 0U), (*g_70), (*g_70)))
    { 
        uint8_t l_120 = 0xBE;
        for (g_51.f2 = (-10); (g_51.f2 != (-13)); g_51.f2 = safe_sub_func_uint8_t_u_u(g_51.f2, 3))
        { 
            const uint8_t l_101 = 0x80;
            int32_t *l_112 = &g_19;
            int32_t *l_113 = &l_14.f2;
            int32_t *l_114 = &l_14.f2;
            int32_t *l_115 = &g_19;
            int32_t *l_116[1];
            int64_t l_118 = (-2);
            int i;
            for (i = 0; i < 1; i++)
                l_116[i] = &g_19;
            if (g_57)
            { 
                union U3 *l_100 = (void*)0;
                int32_t *l_102 = &l_14.f2;
                (*l_102) |= (g_19 = (safe_lshift_func_uint64_t_u_u(((0x9086 > ((void*)0 != l_100)) >= 1), l_101)));
                if (l_101)
                    break;
            }
            else
            { 
                int32_t *l_103 = &g_19;
                int32_t *l_104 = &g_19;
                int32_t *l_105 = &g_19;
                int32_t *l_106 = &g_19;
                int32_t *l_107[3][3] = {{&l_14.f2,&g_19,&g_19},{&l_14.f2,&g_19,&g_19},{&l_14.f2,&g_19,&g_19}};
                int i, j;
                --g_109;
            }
            l_120--;
        }
    }
    else
    { 
        int8_t l_127 = 0x2F;
        for (g_22.f0 = 0; (g_22.f0 != 60); ++g_22.f0)
        { 
            int32_t *l_125 = &g_51.f2;
            int32_t **l_126 = &l_125;
            (*l_126) = l_125;
        }
        return l_127;
    }
    g_19 = ((safe_div_func_int16_t_s_s(((((void*)0 != g_130) , g_51) , l_15[0].f1.f1), (*g_70))) && l_15[0].f1.f3);
    ++g_136;
    return g_51.f0;
}

static int32_t  func_2(uint8_t  p_3, uint16_t  p_4, uint16_t  p_5)
{ 
    struct S2 l_93[3] = {{1U,{0x42,0,18446744073709551607U,65535U}},{1U,{0x42,0,18446744073709551607U,65535U}},{1U,{0x42,0,18446744073709551607U,65535U}}};
    uint8_t *l_94 = (void*)0;
    int32_t l_95 = 0xF8029A73;
    int i;
    if ((+((safe_lshift_func_uint64_t_u_s(((l_95 ^= (l_93[2] , g_87)) & (g_57 , 0x37)), l_93[2].f1.f0)) != l_93[2].f1.f1)))
    { 
        return p_4;
    }
    else
    { 
        return p_4;
    }
}

static struct S1  func_6(struct S1  p_7, struct S2  p_8, int8_t  p_9, int32_t  p_10, struct S0  p_11)
{ 
    int32_t *l_18 = &g_19;
    int32_t **l_20 = &l_18;
    union U3 *l_21 = &g_22;
    int32_t l_31 = 0x8FE831E3;
    int64_t *l_52 = &g_30;
    int16_t *l_86[1];
    int32_t *l_88 = &g_51.f2;
    struct S1 l_89 = {-10,0x07,0x4EDE3B14};
    int i;
    for (i = 0; i < 1; i++)
        l_86[i] = &g_87;
    (*l_18) ^= p_11.f0;
    if ((*l_18))
    { 
        l_20 = &l_18;
    }
    else
    { 
        union U3 **l_23 = &l_21;
        uint32_t l_26 = 0x097D93CB;
        int64_t *l_29 = &g_30;
        uint64_t *l_34 = &g_22.f0;
        int32_t l_40 = (-1);
        (*l_23) = l_21;
        l_31 &= (((safe_sub_func_int64_t_s_s((p_11 , l_26), ((*l_29) = (safe_mul_func_uint64_t_u_u((0x1FDF != p_7.f1), p_11.f0))))) || (**l_20)) && g_17.f1);
        if ((((p_7 , ((((safe_sub_func_int64_t_s_s((((((((*l_34) = 0U) && (((safe_div_func_uint16_t_u_u(p_8.f1.f0, p_7.f1)) | 0x47BEBA6E) == g_19)) , 0x9B92B2C2) > g_30) > p_8.f1.f1) ^ l_26), 1)) > 0x9EDE1510) >= p_11.f1) != 0xE235)) < p_11.f3) || (**l_20)))
        { 
            int32_t *l_37 = &g_19;
            int32_t *l_38 = &l_31;
            int32_t *l_39[3][1];
            uint16_t *l_48 = &g_17.f3;
            int16_t l_54 = 0x9E91;
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_39[i][j] = &g_19;
            }
            g_41--;
            if ((safe_lshift_func_uint64_t_u_u(((safe_mul_func_uint8_t_u_u(p_7.f1, (((((*l_48) = p_7.f0) && (safe_sub_func_uint64_t_u_u(((&g_19 != &g_19) >= l_40), g_12))) , l_26) <= 0xD17753CA))) | p_8.f1.f1), 44)))
            { 
                return g_51;
            }
            else
            { 
                int64_t **l_53 = &l_29;
                int32_t l_55 = (-1);
                g_19 = (((*l_53) = l_52) == (void*)0);
                ++g_57;
            }
            for (g_57 = 0; (g_57 <= 0); g_57 += 1)
            { 
                if (g_57)
                    break;
            }
        }
        else
        { 
            struct S0 l_64 = {255U,0xFF1AB120,0xA2E755E18DB56EC4LL,0xC960};
            uint16_t *l_83 = &g_22.f1.f3;
            uint16_t *l_84[2];
            int i;
            for (i = 0; i < 2; i++)
                l_84[i] = (void*)0;
            for (p_7.f2 = (-1); (p_7.f2 != (-10)); p_7.f2--)
            { 
                (**l_20) = ((safe_add_func_uint32_t_u_u((l_64 , ((safe_mul_func_uint64_t_u_u((((safe_unary_minus_func_uint64_t_u((safe_div_func_uint8_t_u_u(((p_8 , ((p_7.f2 < g_22.f0) || p_8.f1.f0)) != 0x2D6D), 0xB1)))) && p_7.f1) || l_64.f1), l_64.f0)) <= g_17.f0)), (*l_18))) ^ p_10);
                if (g_51.f1)
                    continue;
            }
            g_51.f2 &= (g_70 == (void*)0);
            if (((*l_18) &= (safe_add_func_int16_t_s_s(0x1FDD, (safe_mul_func_int8_t_s_s(((+((p_8.f1.f3 |= (((safe_lshift_func_int32_t_s_s(((safe_rshift_func_int64_t_s_s((safe_mul_func_uint8_t_u_u((g_17.f2 < ((g_30 = (l_64 , g_17.f1)) , g_17.f3)), (-2))), g_71)) < (*g_70)), 22)) != (*g_70)) , p_7.f2)) > l_40)) && p_11.f2), 7))))))
            { 
                (*l_18) &= p_9;
            }
            else
            { 
                return p_7;
            }
        }
    }
    (*l_88) = ((~((((0x0FB7A05A861DE5BBLL <= (((*l_18) = p_8.f0) && p_11.f2)) ^ 0x3E) | p_9) || p_8.f1.f2)) && p_8.f1.f3);
    return l_89;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_17.f0, "g_17.f0", print_hash_value);
    transparent_crc(g_17.f1, "g_17.f1", print_hash_value);
    transparent_crc(g_17.f2, "g_17.f2", print_hash_value);
    transparent_crc(g_17.f3, "g_17.f3", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_22.f0, "g_22.f0", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_51.f0, "g_51.f0", print_hash_value);
    transparent_crc(g_51.f1, "g_51.f1", print_hash_value);
    transparent_crc(g_51.f2, "g_51.f2", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_71, "g_71", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    transparent_crc(g_109, "g_109", print_hash_value);
    transparent_crc(g_136, "g_136", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
