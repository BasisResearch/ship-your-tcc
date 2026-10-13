/* Csmith 0cdc710315cf seed 8, flattened by scripts/csmith_flatten.py */
// Options:   --seed 8 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
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
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
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
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
   uint8_t  f0;
};

struct S1 {
   uint64_t  f0;
   int8_t  f1;
   uint16_t  f2;
};

struct S2 {
   const int16_t  f0;
   const uint16_t  f1;
};

union U3 {
   uint64_t  f0;
   int8_t  f1;
};

static int16_t g_15[1] = {9};
static int32_t g_29 = 0xFA6F63EE;
static uint8_t g_32 = 246U;
static struct S1 g_40 = {0x9E520C3D8D58945CLL,0,65527U};
static int32_t *g_46 = &g_29;
static struct S0 g_47 = {253U};
static uint16_t g_50 = 0xEE1E;
static struct S1 g_57 = {1U,-10,0x2523};
static int32_t g_70 = 0;
static int64_t g_79 = 0xE27FCE2E6445A0AELL;
static int8_t g_80[3] = {(-8),(-8),(-8)};
static int16_t g_81 = 0x1534;
static int8_t g_82 = 0xCF;
static int16_t g_84 = (-6);
static uint16_t g_87[1] = {1U};
static int16_t g_106 = (-1);
static uint8_t g_108 = 253U;
static uint64_t g_112 = 18446744073709551610U;
static int32_t g_122 = 0xBDFDC5FA;
static uint16_t g_146 = 0U;

static uint16_t  func_1(void);
static int32_t  func_2(union U3  p_3);
static int32_t  func_7(struct S0  p_8, const struct S1  p_9, int64_t  p_10, int32_t  p_11, const int64_t  p_12);

static uint16_t  func_1(void)
{ 
    union U3 l_4 = {18446744073709551613U};
    int32_t *l_147 = &g_70;
    (*l_147) = func_2(l_4);
    return g_15[0];
}

static int32_t  func_2(union U3  p_3)
{ 
    struct S0 l_13 = {1U};
    const struct S1 l_14 = {18446744073709551615U,1,0x373A};
    g_146 &= ((((((safe_add_func_int32_t_s_s(func_7(l_13, l_14, g_15[0], l_14.f2, g_15[0]), 0U)) == 1U) ^ g_122) ^ 0xA506) == g_106) != l_14.f1);
    return p_3.f0;
}

static int32_t  func_7(struct S0  p_8, const struct S1  p_9, int64_t  p_10, int32_t  p_11, const int64_t  p_12)
{ 
    int64_t l_27 = 0xEF2F37FC0336AE3DLL;
    struct S2 l_68 = {-1,9U};
    uint16_t l_73 = 5U;
    int32_t l_98 = 0;
    int32_t l_111 = 2;
    int16_t l_139 = 0xA5B3;
    if (((safe_lshift_func_int16_t_s_s((!(p_9.f1 , (((((g_15[0] <= ((safe_mod_func_uint64_t_u_u((safe_mul_func_int8_t_s_s((safe_div_func_int8_t_s_s((safe_lshift_func_int8_t_s_u(0x52, 0)), g_15[0])), l_27)), g_15[0])) && l_27)) == 0x4E) & l_27) == l_27) >= 0xC214))), l_27)) && l_27))
    { 
        int32_t *l_28 = &g_29;
        int32_t *l_30 = &g_29;
        int32_t *l_31[1][1];
        struct S0 l_41[1][2] = {{{0xF9},{0xF9}}};
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_31[i][j] = &g_29;
        }
        g_32--;
        for (l_27 = 16; (l_27 <= (-30)); l_27--)
        { 
            return p_8.f0;
        }
        if (p_9.f0)
        { 
            struct S0 *l_42[3][4];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 4; j++)
                    l_42[i][j] = &l_41[0][1];
            }
            for (l_27 = 0; (l_27 > (-23)); l_27 = safe_sub_func_int64_t_s_s(l_27, 8))
            { 
                struct S1 *l_39 = &g_40;
                (*l_30) ^= ((p_11 || l_27) >= g_15[0]);
                (*l_39) = p_9;
            }
            p_8 = l_41[0][1];
            for (g_40.f0 = 0; (g_40.f0 > 47); ++g_40.f0)
            { 
                int32_t **l_45 = (void*)0;
                g_46 = l_28;
            }
        }
        else
        { 
            struct S0 *l_48 = &l_41[0][1];
            (*l_48) = g_47;
            return (*g_46);
        }
    }
    else
    { 
        uint64_t l_49 = 0x00AC80EABF554C62LL;
        return l_49;
    }
    if ((l_27 && (g_40.f2 = g_50)))
    { 
        int16_t l_64[2];
        int32_t *l_69[3][4] = {{&g_70,&g_70,&g_70,&g_70},{&g_70,&g_70,&g_70,&g_70},{&g_70,&g_70,&g_70,&g_70}};
        int i, j;
        for (i = 0; i < 2; i++)
            l_64[i] = 0;
        for (g_40.f2 = 0; (g_40.f2 != 24); g_40.f2 = safe_add_func_int32_t_s_s(g_40.f2, 8))
        { 
            int32_t **l_53 = &g_46;
            if ((l_53 != l_53))
            { 
                p_11 |= 0xD97C2923;
            }
            else
            { 
                struct S1 *l_54 = (void*)0;
                struct S1 *l_55 = (void*)0;
                struct S1 *l_56 = &g_57;
                (*l_56) = g_40;
                if ((**l_53))
                    continue;
            }
            (*g_46) &= p_10;
            g_47 = g_47;
        }
        if ((safe_mul_func_int8_t_s_s(((safe_mod_func_uint16_t_u_u(((((safe_lshift_func_uint32_t_u_u(l_64[1], (((*g_46) = (~9U)) && ((l_68 , g_40.f1) != g_47.f0)))) , (void*)0) == l_69[0][0]) != 0x3887E981C310705ELL), g_57.f1)) <= p_10), 0x73)))
        { 
            struct S2 *l_72 = &l_68;
            struct S2 **l_71 = &l_72;
            (*l_71) = &l_68;
        }
        else
        { 
            int32_t l_95 = 0x265DC7DF;
            for (l_27 = 0; (l_27 <= 1); l_27 += 1)
            { 
                l_73++;
            }
            for (p_8.f0 = 0; (p_8.f0 != 18); p_8.f0 = safe_add_func_uint8_t_u_u(p_8.f0, 6))
            { 
                int64_t l_78 = 5;
                int32_t l_83 = 1;
                int32_t l_85 = (-1);
                int32_t l_86 = 1;
                int64_t *l_92[1];
                uint16_t l_93 = 0xDFDF;
                int32_t **l_94[4][3];
                int i, j;
                for (i = 0; i < 1; i++)
                    l_92[i] = &g_79;
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 3; j++)
                        l_94[i][j] = &g_46;
                }
                g_87[0]--;
                l_95 = ((safe_mod_func_int64_t_s_s((((*g_46) = (((l_93 = (g_79 = l_68.f1)) || (&l_69[2][2] == (g_47 , l_94[0][0]))) ^ p_9.f0)) <= p_9.f1), l_68.f1)) & (-1));
            }
            return l_95;
        }
    }
    else
    { 
        int32_t l_104 = (-10);
        int32_t l_105 = 0x3F0CD8B2;
        int32_t l_107 = 0x77CD1832;
        struct S0 l_117 = {251U};
        const int32_t *l_120 = &l_111;
        const int32_t **l_119 = &l_120;
        int16_t *l_141 = &g_15[0];
        int16_t l_145 = 0x6962;
        for (g_70 = 0; (g_70 != 24); g_70 = safe_add_func_int16_t_s_s(g_70, 3))
        { 
            int32_t *l_99 = &g_29;
            int32_t *l_100 = (void*)0;
            int32_t *l_101 = &l_98;
            int32_t *l_102[3][1];
            int32_t l_103 = 1;
            union U3 l_118 = {0x9F70EF1A53783AD5LL};
            const int32_t **l_121[1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_102[i][j] = &g_29;
            }
            for (i = 0; i < 1; i++)
                l_121[i] = &l_120;
            g_108--;
            ++g_112;
            if ((g_57.f2 < (safe_sub_func_int64_t_s_s(g_79, (l_117 , 0x3E3F5A7A75AA8E54LL)))))
            { 
                uint8_t l_123 = 251U;
                (*l_101) ^= (l_107 , (((l_118 , 0xC0D59BB7) && 9) != g_57.f1));
                l_121[0] = l_119;
                --l_123;
            }
            else
            { 
                int8_t *l_136[1];
                int32_t l_140[3][4] = {{(-5),0x39F22760,(-5),0x39F22760},{(-5),0x39F22760,(-5),0x39F22760},{(-5),0x39F22760,(-5),0x39F22760}};
                int i, j;
                for (i = 0; i < 1; i++)
                    l_136[i] = &g_57.f1;
                l_140[2][2] = (safe_sub_func_uint32_t_u_u((safe_mul_func_uint32_t_u_u(((l_139 = (safe_rshift_func_int64_t_s_s((safe_sub_func_int8_t_s_s((g_40.f1 = (safe_sub_func_uint8_t_u_u((**l_119), 0xA4))), (safe_add_func_int16_t_s_s((((*g_46) = 0x476FD32F) && (*g_46)), 0)))), 30))) || p_11), 0)), g_57.f0));
                if (l_68.f1)
                    break;
            }
        }
        l_104 = (((*l_141) = 0xC111) == ((((p_11 ^ ((+(((safe_div_func_int32_t_s_s((4U >= g_32), l_145)) , (*l_120)) >= g_122)) >= 0xEA4A799C)) || 0) == l_27) , g_47.f0));
        (*l_119) = &p_11;
    }
    (*g_46) = (*g_46);
    return p_9.f0;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_15[i], "g_15[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_40.f0, "g_40.f0", print_hash_value);
    transparent_crc(g_40.f1, "g_40.f1", print_hash_value);
    transparent_crc(g_40.f2, "g_40.f2", print_hash_value);
    transparent_crc(g_47.f0, "g_47.f0", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    transparent_crc(g_57.f0, "g_57.f0", print_hash_value);
    transparent_crc(g_57.f1, "g_57.f1", print_hash_value);
    transparent_crc(g_57.f2, "g_57.f2", print_hash_value);
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_79, "g_79", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_80[i], "g_80[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_82, "g_82", print_hash_value);
    transparent_crc(g_84, "g_84", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_87[i], "g_87[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_106, "g_106", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    transparent_crc(g_112, "g_112", print_hash_value);
    transparent_crc(g_122, "g_122", print_hash_value);
    transparent_crc(g_146, "g_146", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
