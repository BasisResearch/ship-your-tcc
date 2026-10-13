/* Csmith 0cdc710315cf seed 91, flattened by scripts/csmith_flatten.py */
// Options:   --seed 91 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
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
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
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
   uint64_t  f0;
   int32_t  f1;
   uint64_t  f2;
};

union U1 {
   uint32_t  f0;
   int64_t  f1;
   uint16_t  f2;
};

static int32_t g_2 = 0x8ADAE8C9;
static int32_t g_6 = 0x735E4B78;
static int32_t g_7 = 0;
static int32_t g_12 = 1;
static struct S0 g_13 = {0U,0xCA70051E,0xDB7B2415FB57A67DLL};
static int32_t *g_16 = &g_7;
static union U1 g_26 = {4294967286U};
static union U1 *g_25 = &g_26;
static int8_t g_30 = 1;
static uint8_t g_52[1][1] = {{0x2F}};
static union U1 **g_62 = &g_25;
static uint16_t g_85 = 0x200C;
static struct S0 *g_88 = &g_13;
static int64_t g_107 = 0x2C3E9071E3CB3F2BLL;
static int32_t *g_141 = &g_12;
static int32_t *g_142[2] = {&g_12,&g_12};

static struct S0  func_1(void);
static union U1 * func_17(int32_t * p_18, int32_t  p_19, struct S0  p_20);
static int32_t * func_21(union U1 * p_22, int8_t  p_23, uint64_t  p_24);

static struct S0  func_1(void)
{ 
    union U1 *l_28 = &g_26;
    int32_t l_143 = 1;
    int16_t l_158 = 0x228A;
    uint32_t *l_159 = &g_26.f0;
    struct S0 l_160 = {0x6DA6D5B5E94B15D7LL,0x04335EDE,18446744073709551615U};
    for (g_2 = 5; (g_2 >= 8); g_2 = safe_add_func_uint64_t_u_u(g_2, 7))
    { 
        uint32_t l_5[4][2];
        int32_t *l_14 = &g_12;
        int32_t **l_15[1];
        union U1 **l_27[1];
        int8_t *l_29[4][2];
        struct S0 l_144 = {0x6A60F3683EEDB4BBLL,-1,18446744073709551615U};
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 2; j++)
                l_5[i][j] = 1U;
        }
        for (i = 0; i < 1; i++)
            l_15[i] = &l_14;
        for (i = 0; i < 1; i++)
            l_27[i] = &g_25;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 2; j++)
                l_29[i][j] = &g_30;
        }
        for (g_6 = 1; (g_6 >= 0); g_6 -= 1)
        { 
            const int32_t l_10 = 0;
            for (g_7 = 1; (g_7 >= 0); g_7 -= 1)
            { 
                if (g_6)
                    break;
            }
            for (g_7 = 0; (g_7 <= 1); g_7 += 1)
            { 
                int32_t *l_11 = &g_12;
                (*l_11) = ((g_7 , 5) != (l_10 == 0x246C));
                return g_13;
            }
        }
        g_16 = l_14;
    }
    (*g_16) = ((safe_sub_func_uint64_t_u_u((((((*l_159) = (safe_mul_func_uint8_t_u_u(((safe_rshift_func_uint8_t_u_u((!((safe_mod_func_uint32_t_u_u(g_52[0][0], (((0x02811D3E0DDAC546LL == l_143) | g_13.f2) | g_30))) > 0x7BBD)), 5)) == 0U), l_158))) , l_158) , g_13.f2) <= g_12), g_13.f2)) >= l_143);
    return l_160;
}

static union U1 * func_17(int32_t * p_18, int32_t  p_19, struct S0  p_20)
{ 
    union U1 *l_148 = &g_26;
    for (g_7 = 5; (g_7 < (-18)); g_7--)
    { 
        int32_t *l_147 = &g_12;
        (*l_147) = 0x8FFA4894;
    }
    return l_148;
}

static int32_t * func_21(union U1 * p_22, int8_t  p_23, uint64_t  p_24)
{ 
    int8_t l_35 = 0x43;
    int32_t *l_37 = &g_13.f1;
    int32_t l_51[2][3] = {{0x28B80BBE,0x28B80BBE,0x28B80BBE},{(-3),(-3),(-3)}};
    struct S0 l_57 = {0U,1,0U};
    uint64_t l_59 = 18446744073709551615U;
    uint16_t *l_93 = &g_85;
    int64_t **l_108 = (void*)0;
    int32_t *l_140 = &l_57.f1;
    int i, j;
    if ((p_23 > ((safe_mod_func_uint16_t_u_u((p_24 >= (safe_rshift_func_uint16_t_u_s((g_6 >= 0xFEC1A2C9DBE4FCD5LL), g_6))), l_35)) , p_23)))
    { 
        int64_t l_40 = 0xDDA3018177C41863LL;
        int32_t *l_44 = (void*)0;
        int32_t *l_45 = &g_13.f1;
        int32_t *l_46 = &g_7;
        int32_t *l_47 = &g_7;
        int32_t *l_48 = &g_12;
        int32_t *l_49 = &g_7;
        int32_t *l_50[3][4];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 4; j++)
                l_50[i][j] = &g_12;
        }
        if ((~p_23))
        { 
            int32_t **l_38 = &g_16;
            int32_t **l_39 = &l_37;
            l_40 ^= (((*l_39) = ((*l_38) = l_37)) != (void*)0);
        }
        else
        { 
            int32_t **l_41 = &g_16;
            (*l_41) = &g_2;
            for (g_13.f1 = (-13); (g_13.f1 < (-26)); g_13.f1--)
            { 
                return &g_6;
            }
            return (*l_41);
        }
        g_52[0][0]--;
        for (g_6 = 0; (g_6 == (-18)); g_6 = safe_sub_func_int16_t_s_s(g_6, 8))
        { 
            if ((*l_46))
            { 
                struct S0 *l_58 = &g_13;
                (*l_58) = l_57;
            }
            else
            { 
                if ((*g_16))
                    break;
                ++l_59;
            }
            g_62 = &p_22;
        }
    }
    else
    { 
        struct S0 *l_63 = (void*)0;
        int32_t l_91[4][2];
        uint64_t l_102[1][2];
        int64_t *l_106 = &g_107;
        int64_t ** const l_105 = &l_106;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 2; j++)
                l_91[i][j] = (-7);
        }
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_102[i][j] = 0x4C67DCA9E6B2CF06LL;
        }
        if (((*l_37) = (l_63 != ((**g_62) , l_63))))
        { 
            int8_t *l_67[2];
            int8_t **l_66[1][1];
            int32_t l_83 = 0;
            int32_t l_86 = 0x77F61DB4;
            int i, j;
            for (i = 0; i < 2; i++)
                l_67[i] = (void*)0;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 1; j++)
                    l_66[i][j] = &l_67[1];
            }
            for (g_30 = 12; (g_30 >= (-17)); g_30--)
            { 
                struct S0 *l_68 = (void*)0;
                struct S0 *l_69 = &g_13;
                int64_t *l_70 = &g_26.f1;
                uint16_t *l_84 = &g_85;
                l_66[0][0] = (void*)0;
                (*l_69) = l_57;
                (*l_37) = (l_86 = ((((*l_70) = (*l_37)) != (safe_mod_func_uint16_t_u_u(((safe_rshift_func_uint16_t_u_s(((*l_84) = (l_83 = (safe_rshift_func_uint8_t_u_u((safe_add_func_int32_t_s_s((((safe_add_func_int32_t_s_s((((safe_sub_func_uint8_t_u_u(0xC1, 0x51)) > g_13.f1) && 0x3CEC), p_24)) < (-6)) & 2), 0x41AE354A)), 2)))), 9)) , p_23), l_86))) ^ 1));
            }
            g_16 = &l_51[1][2];
            for (g_12 = 1; (g_12 >= 0); g_12 -= 1)
            { 
                struct S0 *l_87 = &l_57;
                g_88 = l_87;
            }
        }
        else
        { 
            uint16_t *l_94[2][3] = {{&g_85,&g_85,&g_85},{&g_85,&g_85,&g_85}};
            uint16_t **l_95 = &l_94[0][2];
            int32_t *l_98 = &l_51[1][0];
            int32_t l_99 = 0x982FD6D7;
            int32_t *l_100 = (void*)0;
            int32_t *l_101[1][2];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_101[i][j] = &g_13.f1;
            }
            if (((*g_16) = 0x2C9CB7C8))
            { 
                (*l_37) = (safe_mul_func_uint64_t_u_u((g_52[0][0] >= 0x3686DF84), l_91[2][1]));
            }
            else
            { 
                int32_t **l_92 = &l_37;
                (*l_92) = &g_7;
            }
            if ((1U && (l_93 != ((*l_95) = l_94[0][2]))))
            { 
                int32_t **l_96 = &l_37;
                (*l_96) = (void*)0;
                (*l_96) = &l_51[1][2];
            }
            else
            { 
                int32_t *l_97 = &l_91[2][1];
                return &g_7;
            }
            l_102[0][0]++;
        }
        l_108 = l_105;
    }
    for (l_59 = 0; (l_59 <= 1); l_59 += 1)
    { 
        uint8_t l_122 = 0x90;
        int32_t l_131 = 0x0ED8B559;
        int32_t l_134 = (-2);
        struct S0 l_139 = {0xE56AD494AE63F1C0LL,0xA5BE4CFF,0xC7B46AC6E2473BC5LL};
        for (g_13.f0 = 0; (g_13.f0 <= 1); g_13.f0 += 1)
        { 
            int32_t l_111[4] = {(-2),(-2),(-2),(-2)};
            int i;
            for (g_12 = 0; (g_12 <= 1); g_12 += 1)
            { 
                int32_t **l_109 = &g_16;
                struct S0 **l_110 = &g_88;
                int32_t *l_112 = &l_51[0][0];
                int32_t *l_113 = &g_13.f1;
                int32_t *l_114 = &g_7;
                int32_t *l_115 = &l_57.f1;
                int32_t l_116 = 0x896F98B6;
                int32_t *l_117 = (void*)0;
                int32_t *l_118 = &l_51[1][2];
                int32_t *l_119 = &g_6;
                int32_t *l_120 = (void*)0;
                int32_t *l_121[4][2] = {{&g_7,&g_7},{&l_51[l_59][(l_59 + 1)],&g_7},{&g_7,&l_51[l_59][(l_59 + 1)]},{&g_7,&g_7}};
                int i, j;
                (*l_109) = &l_51[l_59][(l_59 + 1)];
                l_110 = l_110;
                l_122--;
            }
            for (g_26.f2 = 0; (g_26.f2 <= 0); g_26.f2 += 1)
            { 
                struct S0 **l_125 = &g_88;
                int32_t *l_126 = &l_111[2];
                int32_t *l_127 = &g_6;
                int32_t *l_128 = &l_51[1][2];
                int32_t l_129[3][4] = {{0x5F2F1DAF,0x0A9413DE,0x5F2F1DAF,0x5F2F1DAF},{0x0A9413DE,0x0A9413DE,0xBCBB49ED,0x0A9413DE},{0x0A9413DE,0x5F2F1DAF,0x5F2F1DAF,0x0A9413DE}};
                int32_t *l_130 = &l_51[0][0];
                int32_t *l_132 = &g_13.f1;
                int32_t *l_133[4][3] = {{&g_13.f1,&g_13.f1,&g_13.f1},{&g_13.f1,&g_13.f1,&g_13.f1},{&g_13.f1,&g_13.f1,&g_2},{&g_2,&g_13.f1,&g_2}};
                uint8_t l_135 = 0x53;
                int i, j;
                (*l_125) = &g_13;
                if (g_52[g_26.f2][g_26.f2])
                    continue;
                l_135++;
            }
        }
        for (g_13.f1 = 1; (g_13.f1 >= 0); g_13.f1 -= 1)
        { 
            struct S0 l_138 = {0U,-1,1U};
            l_139 = l_138;
        }
    }
    return g_141;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_13.f0, "g_13.f0", print_hash_value);
    transparent_crc(g_13.f1, "g_13.f1", print_hash_value);
    transparent_crc(g_13.f2, "g_13.f2", print_hash_value);
    transparent_crc(g_26.f0, "g_26.f0", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_52[i][j], "g_52[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_107, "g_107", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
