/* Csmith 0cdc710315cf seed 234, flattened by scripts/csmith_flatten.py */
// Options:   --seed 234 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
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
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
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
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static int8_t
safe_unary_minus_func_int8_t_s(int8_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

union U1 {
   uint32_t  f0;
   int32_t  f1;
};

union U2 {
   uint16_t  f0;
};

static uint64_t g_8 = 1U;
static union U1 g_20 = {4294967294U};
static int32_t g_29 = (-9);
static uint32_t g_46 = 0x4A0435E4;
static uint32_t g_55[1] = {4294967295U};
static int32_t g_57 = 0;
static int32_t *g_68[2] = {&g_29,&g_29};
static uint16_t g_71 = 0U;
static uint64_t *g_76 = &g_8;
static uint64_t **g_75[1] = {&g_76};
static union U2 g_81 = {65526U};
static int32_t g_83 = 0xC22C9C81;

static uint64_t  func_1(void);
static int32_t  func_5(int64_t  p_6);
static int32_t  func_16(union U2  p_17, union U1  p_18);

static uint64_t  func_1(void)
{ 
    int8_t l_4 = 4;
    uint64_t *l_7[3];
    int32_t l_9 = 9;
    int16_t l_10 = 1;
    int32_t *l_56[1][3];
    uint64_t **l_62 = (void*)0;
    uint64_t **l_63 = &l_7[2];
    const uint64_t *l_64 = &g_8;
    uint64_t *l_65[2][2] = {{&g_8,&g_8},{&g_8,&g_8}};
    int32_t **l_66 = (void*)0;
    int32_t **l_67[2][2] = {{&l_56[0][0],&l_56[0][0]},{&l_56[0][0],&l_56[0][0]}};
    uint16_t *l_69 = (void*)0;
    uint16_t *l_70 = &g_71;
    uint64_t l_72 = 0x012CB5DCFD1F9135LL;
    uint8_t l_99 = 0x41;
    uint64_t l_124 = 0U;
    uint64_t l_146 = 1U;
    int32_t l_147 = (-9);
    int i, j;
    for (i = 0; i < 3; i++)
        l_7[i] = &g_8;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_56[i][j] = &g_57;
    }
    g_57 |= ((safe_mod_func_int32_t_s_s(l_4, func_5(((l_9 ^= l_4) < l_10)))) ^ 0x8477);
    if (((l_72 = ((safe_lshift_func_uint16_t_u_s(((*l_70) = ((g_68[1] = (g_55[0] , ((safe_lshift_func_uint64_t_u_u(((g_8 = (((*l_63) = &g_8) != l_64)) != (-7)), 27)) , (void*)0))) != &g_29)), g_29)) < g_55[0])) >= g_29))
    { 
        uint8_t l_73 = 5U;
        int32_t l_74 = 0;
        uint32_t *l_80 = &g_55[0];
        int32_t l_87 = 7;
        int32_t l_92[4][3] = {{0,0,0},{1,1,1},{0,0,0},{1,1,1}};
        uint64_t l_115[3];
        int64_t l_122[4] = {(-3),(-3),(-3),(-3)};
        int64_t *l_123 = &l_122[2];
        int8_t *l_125 = &l_4;
        int i, j;
        for (i = 0; i < 3; i++)
            l_115[i] = 0U;
        if ((l_74 = l_73))
        { 
            for (l_10 = 0; (l_10 <= 1); l_10 += 1)
            { 
                g_75[0] = ((((g_55[0] , 1U) != (((0xB2D2 >= 0x149A) | 0x0839) >= 0x2677E161)) & g_29) , (void*)0);
            }
        }
        else
        { 
            int32_t l_77 = 3;
            return l_77;
        }
        if (((safe_mul_func_int8_t_s_s((l_80 != (void*)0), g_20.f1)) , 0xC4895D34))
        { 
            int8_t l_82[4][4] = {{(-1),0xC5,0xC5,(-1)},{(-1),0xC5,0xC5,(-1)},{(-1),0xC5,0xC5,(-1)},{(-1),0xC5,0xC5,(-1)}};
            int i, j;
            g_83 = (l_82[1][3] = (0x3CBC9198 && (g_81 , 0xF148A9B2)));
        }
        else
        { 
            int8_t l_86 = 0x23;
            int32_t l_88 = 0xC39F6C45;
            int32_t l_91 = 0xDEC5ABB5;
            int32_t l_94 = (-8);
            int32_t l_95 = 0xE352C4D3;
            int32_t l_96 = 0x85FCF45A;
            int32_t l_97 = 0x61FA26FB;
            int32_t l_98[3][1];
            int8_t l_104 = 0xA4;
            int32_t *l_105[2][1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_98[i][j] = 0xC3ECA65C;
            }
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_105[i][j] = &l_92[0][1];
            }
            for (l_9 = 0; (l_9 >= (-27)); l_9--)
            { 
                int8_t l_89 = 0;
                int32_t l_90 = 0x6011814A;
                int32_t l_93[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_93[i] = (-8);
                l_99--;
                g_68[1] = &g_29;
                g_68[0] = &g_29;
            }
            for (g_81.f0 = 29; (g_81.f0 < 26); g_81.f0--)
            { 
                return l_94;
            }
            for (g_8 = 0; (g_8 <= 0); g_8 += 1)
            { 
                const int8_t l_114 = 0x2D;
                int32_t l_116 = 0x67BC28BB;
                int32_t l_117[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_117[i] = 0xA51DA565;
                if (l_104)
                    break;
                l_92[2][0] = ((l_105[1][0] = &g_57) == (void*)0);
                l_117[1] &= (safe_div_func_uint16_t_u_u((l_73 != ((safe_add_func_int16_t_s_s((safe_div_func_uint64_t_u_u(((l_116 = ((((safe_mod_func_uint16_t_u_u(((g_20 , g_20) , l_114), g_57)) >= g_20.f1) && l_115[2]) == g_29)) & 0xE7E56D92668A2AEELL), l_87)), l_114)) >= g_55[0])), g_46));
            }
        }
        l_87 = (((*l_125) = (safe_mod_func_int64_t_s_s(((*l_123) = (l_115[0] ^ ((safe_add_func_uint16_t_u_u((l_115[1] , (g_20.f1 == l_122[2])), l_115[2])) && g_20.f1))), l_124))) ^ g_20.f1);
    }
    else
    { 
        int32_t l_133 = 0xA301C7F3;
        int32_t l_134 = 0x642EFCB1;
        uint64_t l_139 = 0x92CBA2C95388961ELL;
        if ((+(safe_lshift_func_uint64_t_u_u(18446744073709551607U, (safe_lshift_func_uint8_t_u_s((&g_57 == ((safe_mod_func_int32_t_s_s(l_133, 0xF3CD2035)) , (void*)0)), g_20.f1))))))
        { 
            uint8_t l_135 = 0x20;
            l_135++;
            for (g_20.f1 = 0; g_20.f1 < 3; g_20.f1 = g_20.f1 + 1)
            {
                l_7[g_20.f1] = &l_124;
            }
        }
        else
        { 
            union U1 l_138 = {1U};
            int32_t l_144 = 0x1EEF570C;
            int32_t l_145 = 1;
            l_139 &= (l_138 , (l_134 = l_134));
            for (g_71 = 0; (g_71 >= 33); g_71 = safe_add_func_int8_t_s_s(g_71, 1))
            { 
                l_145 &= ((safe_sub_func_uint64_t_u_u((*g_76), (&l_56[0][1] == (void*)0))) ^ l_144);
            }
        }
    }
    l_147 &= l_146;
    return (*g_76);
}

static int32_t  func_5(int64_t  p_6)
{ 
    uint64_t *l_13 = (void*)0;
    union U2 l_19 = {5U};
    int32_t *l_34 = &g_20.f1;
    uint32_t *l_35 = &g_20.f0;
    uint32_t *l_36 = &g_20.f0;
    uint32_t *l_37 = &g_20.f0;
    uint32_t *l_38 = &g_20.f0;
    uint32_t *l_39 = (void*)0;
    uint32_t *l_40 = (void*)0;
    uint32_t *l_41 = &g_20.f0;
    int32_t l_42 = 0x34071E99;
    uint32_t *l_43 = &g_20.f0;
    uint32_t *l_44 = &g_20.f0;
    uint32_t *l_45[3][4] = {{&g_20.f0,&g_46,&g_46,&g_46},{&g_46,&g_46,&g_46,&g_46},{&g_46,&g_46,&g_20.f0,&g_46}};
    uint64_t *l_54 = &g_8;
    int i, j;
    (*l_34) = ((safe_lshift_func_int16_t_s_u((((void*)0 == l_13) , ((safe_rshift_func_int32_t_s_u(func_16(l_19, g_20), 16)) ^ (-1))), 14)) || p_6);
    (*l_34) = ((++g_46) | ((safe_add_func_uint8_t_u_u((*l_34), (p_6 & (safe_unary_minus_func_int8_t_s((g_55[0] = ((((*l_54) = (safe_add_func_uint16_t_u_u(((*l_34) <= 65535U), p_6))) , &l_42) == &l_42))))))) & (-1)));
    return p_6;
}

static int32_t  func_16(union U2  p_17, union U1  p_18)
{ 
    const int8_t l_32 = 0x9E;
    uint32_t l_33 = 4294967286U;
    for (g_20.f1 = 0; (g_20.f1 > (-1)); g_20.f1--)
    { 
        int32_t *l_28 = &g_29;
        for (g_8 = (-30); (g_8 < 14); g_8++)
        { 
            int32_t *l_27 = &g_20.f1;
            for (p_18.f1 = 0; (p_18.f1 != 17); ++p_18.f1)
            { 
                l_28 = l_27;
                g_29 ^= (((void*)0 == &g_8) >= (((safe_rshift_func_int16_t_s_s(g_8, 9)) <= 0xE7) >= l_32));
                return p_18.f1;
            }
        }
    }
    return l_33;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_20.f1, "g_20.f1", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_55[i], "g_55[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_71, "g_71", print_hash_value);
    transparent_crc(g_81.f0, "g_81.f0", print_hash_value);
    transparent_crc(g_83, "g_83", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
