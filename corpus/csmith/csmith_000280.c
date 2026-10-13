/* Csmith 0cdc710315cf seed 280, flattened by scripts/csmith_flatten.py */
// Options:   --seed 280 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
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

struct S1 {
   int64_t  f0;
   int16_t  f1;
   uint16_t  f2;
};

struct S0 {
   uint16_t  f0;
};

struct S2 {
   const struct S0  f0;
   const struct S1  f1;
   const int64_t  f2;
};

union U3 {
   int16_t  f0;
};

union U4 {
   uint32_t  f0;
   int32_t  f1;
   struct S0  f2;
};

static int32_t g_3 = 0x7F5ED9A9;
static int32_t *g_2 = &g_3;
static int32_t g_21 = (-5);
static union U4 g_31 = {4294967295U};
static int8_t g_53 = (-1);
static int64_t g_54 = 0x1AB70486D4D44114LL;
static int32_t g_55 = 8;
static int16_t g_58[2] = {0,0};
static int32_t g_60 = 1;
static int32_t g_62[2] = {0x00F61094,0x00F61094};
static struct S2 g_83 = {{65535U},{1,0x5A1C,0xA016},-1};
static union U3 g_84 = {0};
static union U4 *g_91 = &g_31;
static union U4 **g_90 = &g_91;
static struct S1 g_95 = {1,-1,0x2789};
static int16_t g_112 = (-1);
static int16_t *g_111 = &g_112;
static int16_t **g_110 = &g_111;
static int16_t **g_113 = &g_111;

static const int64_t  func_1(void);
static uint64_t  func_7(uint8_t  p_8, uint64_t  p_9, union U4  p_10, int32_t * p_11, int32_t * p_12);
static union U4  func_14(int32_t * p_15, int32_t * p_16, union U4  p_17);

static const int64_t  func_1(void)
{ 
    int32_t **l_4 = &g_2;
    union U4 l_18[3][3] = {{{0x23AAF5BF},{0x23AAF5BF},{0x23AAF5BF}},{{5U},{0xBFB2C41A},{5U}},{{0x23AAF5BF},{0x23AAF5BF},{0x23AAF5BF}}};
    int i, j;
    (*l_4) = g_2;
    if ((*g_2))
    { 
        int64_t l_13 = 0xEB4C8F0D0714AD38LL;
        int32_t *l_74 = &g_62[1];
        struct S1 l_114[1][1] = {{{-1,0,0x9261}}};
        int64_t l_118 = 0xEFA84F3720B21173LL;
        int16_t l_131 = (-1);
        struct S2 *l_134 = &g_83;
        struct S2 **l_133 = &l_134;
        int i, j;
        if ((safe_rshift_func_uint64_t_u_u(func_7(g_3, l_13, func_14((*l_4), &g_3, l_18[1][1]), l_74, &g_62[1]), 24)))
        { 
            return g_62[1];
        }
        else
        { 
            const int8_t l_109 = 0x02;
            int64_t *l_129 = &l_114[0][0].f0;
            int64_t *l_130[2][3];
            int32_t *l_132 = &g_21;
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 3; j++)
                    l_130[i][j] = &g_95.f0;
            }
            if ((*g_2))
            { 
                struct S1 *l_115 = &l_114[0][0];
                (**l_4) = l_109;
                g_113 = (g_110 = g_110);
                (*l_115) = l_114[0][0];
            }
            else
            { 
                (*l_4) = (void*)0;
                l_118 = (safe_rshift_func_int8_t_s_u(((void*)0 == &g_91), 6));
            }
            (*l_74) |= l_109;
            (*l_132) |= (safe_lshift_func_uint8_t_u_u(((safe_mod_func_int64_t_s_s(((safe_div_func_uint32_t_u_u((safe_sub_func_int64_t_s_s((*l_74), 18446744073709551615U)), ((g_95.f0 = ((*l_129) |= (safe_add_func_int8_t_s_s((l_109 , 8), 9)))) & l_131))) < (-5)), g_3)) ^ g_83.f0.f0), 4));
        }
        (*l_133) = &g_83;
    }
    else
    { 
        uint8_t l_135 = 0x40;
        l_135++;
        return (**l_4);
    }
    return l_18[1][1].f0;
}

static uint64_t  func_7(uint8_t  p_8, uint64_t  p_9, union U4  p_10, int32_t * p_11, int32_t * p_12)
{ 
    int32_t **l_79[4][2] = {{&g_2,&g_2},{&g_2,&g_2},{&g_2,&g_2},{&g_2,&g_2}};
    int32_t l_80 = (-1);
    int i, j;
    if ((safe_div_func_int32_t_s_s((safe_sub_func_uint64_t_u_u((p_8 <= ((l_79[2][1] = &g_2) == (void*)0)), (-1))), l_80)))
    { 
        return p_8;
    }
    else
    { 
        uint8_t l_85 = 2U;
        union U4 *l_89 = (void*)0;
        union U4 **l_88 = &l_89;
        int32_t l_105[1][2];
        uint16_t l_106 = 1U;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_105[i][j] = 0x0FF22B77;
        }
        l_85 &= ((*p_11) |= (safe_mul_func_int64_t_s_s(((*g_2) != ((g_83 , ((g_84 , &p_11) != (void*)0)) != g_83.f2)), 0x54D5C1110172C61BLL)));
        for (g_31.f0 = 0; (g_31.f0 > 1); g_31.f0 = safe_add_func_uint8_t_u_u(g_31.f0, 6))
        { 
            struct S1 *l_94 = &g_95;
            uint16_t *l_102[3];
            int16_t *l_103 = &g_84.f0;
            int32_t l_104 = 0x417BD2F3;
            int i;
            for (i = 0; i < 3; i++)
                l_102[i] = (void*)0;
            g_90 = l_88;
            l_104 &= ((*g_2) = (safe_rshift_func_int16_t_s_u(((*l_103) = ((((*l_94) = g_83.f1) , (safe_lshift_func_int32_t_s_s(((safe_mul_func_int16_t_s_s((safe_mul_func_uint16_t_u_u(l_85, (p_10.f2.f0 = (((*p_11) ^= ((p_9 & 0xBBE26AF1) > p_9)) | (*g_2))))), l_85)) , (*p_12)), 15))) , 0x6E85)), 1)));
        }
        l_106++;
    }
    return p_10.f0;
}

static union U4  func_14(int32_t * p_15, int32_t * p_16, union U4  p_17)
{ 
    int64_t l_32 = (-10);
    const int32_t *l_33 = &g_21;
    int32_t l_57 = 1;
    int32_t l_59 = (-1);
    int32_t l_61 = 0x260F1475;
    int32_t l_65 = 0xAF75FBD2;
    int32_t l_66 = 0x6F270ADF;
    union U4 l_73[1][1] = {{{4294967295U}}};
    int i, j;
    for (p_17.f0 = (-30); (p_17.f0 == 52); ++p_17.f0)
    { 
        int32_t *l_24 = &g_21;
        int32_t l_42 = 0;
        int32_t l_52 = 0xE8824086;
        int32_t l_56[2][4];
        uint32_t l_69 = 0xAB3286B0;
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 4; j++)
                l_56[i][j] = (-10);
        }
        for (g_21 = 22; (g_21 != 4); g_21 = safe_sub_func_uint16_t_u_u(g_21, 2))
        { 
            union U4 *l_29 = (void*)0;
            union U4 *l_30 = &g_31;
            int32_t l_41 = 0xBB5FB49A;
            g_2 = l_24;
            if (((safe_sub_func_uint8_t_u_u(g_21, (safe_mul_func_uint16_t_u_u(0x4DF7, (((*l_30) = p_17) , 0xDAA0))))) , (*p_15)))
            { 
                l_32 ^= (*g_2);
                if ((*g_2))
                    continue;
            }
            else
            { 
                const int32_t **l_34 = &l_33;
                (*l_34) = l_33;
                l_42 = (safe_add_func_int32_t_s_s((*p_16), ((safe_add_func_uint32_t_u_u((((((0x6652F093D7F285F4LL > (safe_add_func_uint64_t_u_u((((-1) >= 0x245E) | p_17.f0), 1U))) < l_41) | (-2)) | p_17.f0) > p_17.f0), g_21)) ^ (*g_2))));
            }
        }
        for (g_31.f2.f0 = 0; (g_31.f2.f0 != 58); g_31.f2.f0++)
        { 
            int64_t l_47 = 0x6315BFEB1EC6C048LL;
            int32_t l_63 = 2;
            int32_t l_64 = 0xD119E2D4;
            int32_t l_67 = 0x0C8B8AFE;
            int32_t l_68 = 0x7B9DA011;
            for (l_42 = 26; (l_42 > (-13)); l_42--)
            { 
                int32_t *l_48 = &g_21;
                int32_t *l_49 = &g_21;
                int32_t *l_50 = &g_21;
                int32_t *l_51[1][1];
                int32_t **l_72 = &g_2;
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_51[i][j] = (void*)0;
                }
                --l_69;
                (*l_72) = p_15;
            }
            if ((*p_15))
                break;
            for (l_47 = 1; (l_47 >= 0); l_47 -= 1)
            { 
                return p_17;
            }
        }
    }
    return l_73[0][0];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_31.f0, "g_31.f0", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    transparent_crc(g_54, "g_54", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_58[i], "g_58[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_60, "g_60", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_62[i], "g_62[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_83.f0.f0, "g_83.f0.f0", print_hash_value);
    transparent_crc(g_83.f1.f0, "g_83.f1.f0", print_hash_value);
    transparent_crc(g_83.f1.f1, "g_83.f1.f1", print_hash_value);
    transparent_crc(g_83.f1.f2, "g_83.f1.f2", print_hash_value);
    transparent_crc(g_83.f2, "g_83.f2", print_hash_value);
    transparent_crc(g_84.f0, "g_84.f0", print_hash_value);
    transparent_crc(g_95.f0, "g_95.f0", print_hash_value);
    transparent_crc(g_95.f1, "g_95.f1", print_hash_value);
    transparent_crc(g_95.f2, "g_95.f2", print_hash_value);
    transparent_crc(g_112, "g_112", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
