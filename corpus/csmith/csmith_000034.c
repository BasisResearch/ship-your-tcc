/* Csmith 0cdc710315cf seed 34, flattened by scripts/csmith_flatten.py */
// Options:   --seed 34 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint8_t  f0;
   int8_t  f1;
};

static const int32_t g_2 = 0xAD3B3152;
static int32_t g_6 = 0xC6EFF3DE;
static int32_t g_15[2][3] = {{0x53FFD03B,0x53FFD03B,0x53FFD03B},{0xF0442E11,0xF0442E11,0xF0442E11}};
static uint16_t g_20 = 65535U;
static union U0 g_39 = {0x18};
static int32_t *g_50 = &g_6;
static int32_t **g_49[4][1] = {{&g_50},{&g_50},{&g_50},{&g_50}};
static int8_t g_78 = (-3);
static int32_t **g_109[2] = {&g_50,&g_50};
static uint64_t g_115 = 0x7D68548FAEAFAE61LL;

static int64_t  func_1(void);
static const int32_t  func_21(int32_t * p_22, int32_t * p_23, union U0  p_24, uint16_t  p_25, int32_t * p_26);
static int32_t * func_27(int32_t  p_28);

static int64_t  func_1(void)
{ 
    int16_t l_3 = 1;
    int32_t l_8 = 6;
    int32_t l_14[3];
    int32_t l_16 = 1;
    uint64_t l_17 = 18446744073709551612U;
    int32_t *l_38 = &l_14[2];
    int32_t *l_41[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
    int8_t *l_53 = (void*)0;
    int8_t *l_54[3];
    const int32_t l_64 = 0x056B0C4F;
    union U0 l_113 = {8U};
    uint32_t l_117 = 0U;
    uint64_t *l_121 = &l_17;
    int i;
    for (i = 0; i < 3; i++)
        l_14[i] = 2;
    for (i = 0; i < 3; i++)
        l_54[i] = &g_39.f1;
lbl_97:
    if (((((g_2 , g_2) != g_2) & l_3) ^ 0x1D962EE6))
    { 
        int32_t l_4 = 1;
        int32_t *l_5 = &g_6;
        int32_t *l_7 = &g_6;
        int32_t *l_9 = (void*)0;
        int32_t *l_10 = &l_4;
        int32_t *l_11 = (void*)0;
        int32_t *l_12 = &g_6;
        int32_t *l_13[4][3] = {{&l_4,&l_4,&l_4},{(void*)0,(void*)0,(void*)0},{&l_4,&l_4,&l_4},{(void*)0,(void*)0,(void*)0}};
        int i, j;
        (*l_5) &= l_4;
        l_17--;
        (*l_12) ^= g_20;
    }
    else
    { 
        int32_t *l_33 = &g_15[1][0];
        int32_t **l_37 = &l_33;
        int32_t l_40 = 0xCF3D0097;
        g_6 = (8U && func_21(((*l_37) = func_27(((*l_33) = (((safe_lshift_func_uint8_t_u_s((safe_add_func_uint64_t_u_u(((g_20 ^ g_15[1][0]) != g_15[0][2]), g_6)), 1)) == g_15[1][0]) && g_15[1][0])))), l_38, g_39, l_40, l_41[1]));
        (*l_38) = ((safe_div_func_uint16_t_u_u(((safe_lshift_func_uint16_t_u_s((g_49[0][0] == &l_41[3]), (((**l_37) > 65527U) || (**l_37)))) & 0U), g_20)) >= g_15[1][0]);
    }
    if ((safe_add_func_int8_t_s_s(((g_39.f1 = 1) , (safe_mul_func_int8_t_s_s((safe_lshift_func_uint32_t_u_u(g_15[1][2], (safe_unary_minus_func_uint8_t_u(((safe_mod_func_uint32_t_u_u((safe_lshift_func_uint16_t_u_s(65535U, (*l_38))), g_2)) <= (*l_38)))))), 0xD9))), l_64)))
    { 
        int32_t l_80 = 1;
        uint64_t *l_86 = &l_17;
        int32_t *l_94 = (void*)0;
        for (g_39.f0 = 3; (g_39.f0 > 4); g_39.f0++)
        { 
            uint32_t l_76 = 0x8A85F878;
            uint64_t *l_77[1][3];
            uint16_t *l_79 = &g_20;
            const uint16_t l_87[1][3] = {{65535U,65535U,65535U}};
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 3; j++)
                    l_77[i][j] = &l_17;
            }
            (*l_38) = (*g_50);
            (*g_50) = (safe_div_func_uint32_t_u_u(((0xB18A < ((*l_79) = (!((safe_sub_func_uint64_t_u_u((g_78 = ((safe_div_func_uint64_t_u_u((safe_sub_func_uint8_t_u_u(1U, g_20)), l_76)) | g_6)), l_76)) >= g_20)))) == l_76), l_80));
            if ((safe_lshift_func_int8_t_s_s((safe_rshift_func_uint32_t_u_u(4294967294U, (!(((l_86 == ((g_6 != 0xB92311C7) , (void*)0)) < g_15[0][2]) <= l_87[0][1])))), l_80)))
            { 
                int32_t **l_88[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_88[i] = &l_41[1];
                l_38 = &l_80;
                if (l_80)
                    break;
            }
            else
            { 
                int64_t l_89[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_89[i] = 1;
                if (l_89[1])
                    break;
                (*l_38) ^= l_89[3];
                if ((*g_50))
                    break;
            }
        }
        for (g_39.f1 = 9; (g_39.f1 <= (-11)); --g_39.f1)
        { 
            for (l_80 = 0; (l_80 == 28); l_80++)
            { 
                return g_15[1][2];
            }
        }
        l_41[1] = (l_94 = func_27(((void*)0 == &l_80)));
    }
    else
    { 
        int32_t * const l_101 = (void*)0;
        int32_t l_118 = (-10);
        for (l_16 = 0; (l_16 < 28); l_16 = safe_add_func_uint64_t_u_u(l_16, 2))
        { 
            if (l_17)
                goto lbl_97;
            for (l_17 = 0; (l_17 <= 3); l_17 += 1)
            { 
                (*g_50) = (!g_2);
            }
        }
        for (l_16 = (-25); (l_16 > 2); l_16 = safe_add_func_int32_t_s_s(l_16, 6))
        { 
            int32_t **l_102[2];
            int32_t **l_103 = (void*)0;
            int32_t **l_104 = &l_41[2];
            int i;
            for (i = 0; i < 2; i++)
                l_102[i] = &l_41[1];
            (*l_104) = l_101;
            if ((safe_lshift_func_uint8_t_u_s((safe_sub_func_uint8_t_u_u(7U, 0x06)), 2)))
            { 
                int32_t **l_110 = &l_41[1];
                l_110 = (g_109[1] = (g_49[0][0] = &g_50));
            }
            else
            { 
                uint8_t *l_114[1][4];
                int32_t l_116 = 0x23FC0092;
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 4; j++)
                        l_114[i][j] = &g_39.f0;
                }
                l_116 = ((-1) || ((g_115 ^= ((l_113 , 18446744073709551614U) ^ g_15[1][0])) < 2U));
                if (l_117)
                    break;
                if (l_118)
                    break;
            }
        }
    }
    (*g_50) |= (safe_lshift_func_uint64_t_u_u(((*l_121) = g_15[1][0]), 34));
    return g_20;
}

static const int32_t  func_21(int32_t * p_22, int32_t * p_23, union U0  p_24, uint16_t  p_25, int32_t * p_26)
{ 
    uint8_t l_42 = 0U;
    --l_42;
    return g_20;
}

static int32_t * func_27(int32_t  p_28)
{ 
    int32_t *l_34 = &g_15[0][0];
    int32_t **l_35 = &l_34;
    int32_t *l_36[4];
    int i;
    for (i = 0; i < 4; i++)
        l_36[i] = &g_15[1][0];
    (*l_35) = l_34;
    return l_36[2];
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
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_15[i][j], "g_15[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_39.f1, "g_39.f1", print_hash_value);
    transparent_crc(g_78, "g_78", print_hash_value);
    transparent_crc(g_115, "g_115", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
