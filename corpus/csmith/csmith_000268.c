/* Csmith 0cdc710315cf seed 268, flattened by scripts/csmith_flatten.py */
// Options:   --seed 268 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint32_t  f0;
   int64_t  f1;
   uint64_t  f2;
};

union U1 {
   uint64_t  f0;
};

static int32_t g_2 = 0x1F94964A;
static int32_t g_5[1][1] = {{0x4822B165}};
static int32_t g_20 = (-1);
static int32_t g_24 = (-1);
static int32_t **g_34[2] = {(void*)0,(void*)0};
static int32_t *g_50 = &g_20;
static int32_t **g_49 = &g_50;
static int32_t **g_59 = &g_50;
static union U0 g_62 = {4294967290U};
static union U0 *g_61 = &g_62;
static int64_t g_123 = 0xAD213B74EAE5124ALL;
static int16_t g_125 = 3;
static uint8_t g_126 = 0xC9;

static union U0  func_1(void);
static union U0  func_8(int64_t  p_9);
static int32_t ** func_28(int32_t * p_29, int32_t ** p_30, int8_t  p_31, int32_t * const  p_32, union U1  p_33);

static union U0  func_1(void)
{ 
    union U0 l_110 = {0xCAC19DAA};
    union U1 l_114 = {1U};
    int32_t l_127 = 0xDBDB1424;
    for (g_2 = (-3); (g_2 == 28); g_2 = safe_add_func_int16_t_s_s(g_2, 8))
    { 
        int64_t l_16 = 0;
        int32_t l_109 = 0x87EA63F3;
        uint32_t l_118[3][4] = {{0x26DF2D4C,0xB2B9CE03,0x26DF2D4C,0x26DF2D4C},{0xB2B9CE03,0xB2B9CE03,4294967295U,0xB2B9CE03},{0xB2B9CE03,0x26DF2D4C,0x26DF2D4C,0xB2B9CE03}};
        int64_t *l_119 = &g_62.f1;
        int64_t *l_120 = &l_110.f1;
        int64_t *l_121 = &g_62.f1;
        int64_t *l_122[2];
        int16_t *l_124[3][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0}};
        int i, j;
        for (i = 0; i < 2; i++)
            l_122[i] = &g_62.f1;
        for (g_5[0][0] = (-30); (g_5[0][0] <= 26); ++g_5[0][0])
        { 
            uint16_t l_17 = 0x8BD8;
            union U0 **l_104 = &g_61;
            (**g_59) = ((g_62 = func_8((safe_div_func_uint16_t_u_u((0x63 > ((0x66A92721 > (((safe_mod_func_uint16_t_u_u((safe_rshift_func_uint8_t_u_s(0x92, 6)), l_16)) > 1U) > 1U)) < g_5[0][0])), l_17)))) , 0x13529C6F);
            (**g_59) |= ((((safe_unary_minus_func_uint8_t_u(250U)) | (((((*l_104) = (void*)0) != ((g_62 , l_17) , &g_62)) != 0x3769) < 7)) , 65533U) | g_5[0][0]);
            for (g_62.f1 = 0; (g_62.f1 >= 28); g_62.f1++)
            { 
                (**g_59) = (safe_sub_func_uint16_t_u_u(g_5[0][0], g_24));
                l_109 = (**g_49);
                return l_110;
            }
        }
        l_127 |= (l_109 = ((~((((safe_lshift_func_uint8_t_u_u(((l_114 , (safe_mod_func_uint64_t_u_u(((g_126 = (g_125 = ((g_123 &= (((~l_118[1][2]) | l_118[1][2]) & l_16)) == (-1)))) >= g_24), 0xB7196FBAD4E8C809LL))) > (**g_59)), 3)) , (**g_59)) | 0x0F431827) > g_24)) || 0xC2C5));
    }
    return g_62;
}

static union U0  func_8(int64_t  p_9)
{ 
    uint64_t l_64 = 0x3B61EA52FA57A880LL;
    int32_t l_68 = 0;
    union U0 *l_75 = (void*)0;
    int32_t *l_76 = (void*)0;
    int32_t *l_77 = &l_68;
    int32_t l_78 = 0;
    int32_t *l_79 = &g_20;
    int32_t *l_80 = &g_24;
    int32_t *l_81 = (void*)0;
    int32_t *l_82 = (void*)0;
    int32_t *l_83 = &l_78;
    int32_t *l_84 = &g_20;
    int32_t *l_85 = (void*)0;
    int32_t *l_86 = &l_68;
    int32_t *l_87 = (void*)0;
    int32_t *l_88 = (void*)0;
    int32_t *l_89 = (void*)0;
    int32_t *l_90 = &g_24;
    int32_t *l_91 = &g_20;
    int32_t *l_92 = &l_78;
    int32_t l_93 = 0xE74433A0;
    int32_t l_94 = 0xE0D08F1B;
    int32_t *l_95 = &g_20;
    int32_t *l_96 = &l_94;
    int32_t *l_97 = (void*)0;
    int32_t *l_98[3][2] = {{&g_20,&g_20},{&g_20,&g_20},{&g_20,&g_20}};
    uint64_t l_99[2];
    union U0 l_102 = {5U};
    int i, j;
    for (i = 0; i < 2; i++)
        l_99[i] = 0U;
    for (p_9 = (-11); (p_9 <= (-18)); p_9 = safe_sub_func_int64_t_s_s(p_9, 6))
    { 
        uint16_t l_25 = 0x3DEC;
        union U1 l_37 = {18446744073709551615U};
        int32_t * const l_57 = &g_2;
        int32_t **l_60 = &g_50;
        union U0 **l_63 = &g_61;
        const union U1 * const l_73 = &l_37;
        const union U1 *l_74 = &l_37;
        for (g_20 = 0; (g_20 == 18); ++g_20)
        { 
            int32_t *l_23[2];
            int i;
            for (i = 0; i < 2; i++)
                l_23[i] = &g_24;
            l_25++;
            for (g_24 = 1; (g_24 >= 0); g_24 -= 1)
            { 
                int32_t **l_51 = &g_50;
                int i;
                l_51 = func_28(l_23[g_24], g_34[0], (safe_mod_func_uint16_t_u_u(p_9, 0xCB64)), l_23[g_24], l_37);
            }
            for (l_37.f0 = 0; (l_37.f0 <= 20); l_37.f0 = safe_add_func_uint8_t_u_u(l_37.f0, 6))
            { 
                uint16_t l_54 = 0x80B0;
                int32_t **l_58 = &l_23[0];
                l_54++;
                (*l_58) = l_57;
                l_60 = g_59;
            }
        }
        (*l_63) = g_61;
        if (l_64)
        { 
            int8_t l_65 = (-1);
            if (l_65)
                break;
            if (p_9)
                continue;
            (*g_50) &= l_65;
        }
        else
        { 
            for (g_62.f2 = (-15); (g_62.f2 == 33); g_62.f2 = safe_add_func_uint32_t_u_u(g_62.f2, 7))
            { 
                int32_t *l_69[1];
                uint64_t l_70[2];
                int i;
                for (i = 0; i < 1; i++)
                    l_69[i] = &g_20;
                for (i = 0; i < 2; i++)
                    l_70[i] = 0x53ED870B1CB45490LL;
                l_70[0]++;
                l_74 = l_73;
            }
            (*l_63) = l_75;
        }
    }
    ++l_99[0];
    return l_102;
}

static int32_t ** func_28(int32_t * p_29, int32_t ** p_30, int8_t  p_31, int32_t * const  p_32, union U1  p_33)
{ 
    int16_t l_45 = 0xBFBC;
    for (p_33.f0 = (-6); (p_33.f0 <= 29); ++p_33.f0)
    { 
        int32_t *l_40[3][2] = {{(void*)0,&g_20},{&g_20,(void*)0},{&g_20,&g_20}};
        int32_t **l_41 = &l_40[2][0];
        int i, j;
        (*l_41) = l_40[2][1];
        for (p_31 = 0; (p_31 >= (-24)); p_31 = safe_sub_func_int16_t_s_s(p_31, 9))
        { 
            int32_t l_44[4] = {1,1,1,1};
            int i;
            if ((0x9DEB <= g_5[0][0]))
            { 
                if (l_44[1])
                    break;
            }
            else
            { 
                if (l_44[1])
                    break;
            }
            if (l_45)
            { 
                l_44[1] = (l_44[1] & ((void*)0 != g_34[1]));
            }
            else
            { 
                int16_t l_46 = (-8);
                l_46 &= (l_44[2] = 0x538E0EA6);
            }
            for (l_45 = 0; (l_45 == 26); l_45++)
            { 
                return g_49;
            }
        }
    }
    return &g_50;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_5[i][j], "g_5[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_123, "g_123", print_hash_value);
    transparent_crc(g_125, "g_125", print_hash_value);
    transparent_crc(g_126, "g_126", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
