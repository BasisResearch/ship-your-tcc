/* Csmith 0cdc710315cf seed 311, flattened by scripts/csmith_flatten.py */
// Options:   --seed 311 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
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
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
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
static int8_t
safe_unary_minus_func_int8_t_s(int8_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint32_t  f0;
};

struct S1 {
   int64_t  f0;
   int32_t  f1;
   struct S0  f2;
   uint64_t  f3;
};

union U3 {
   const struct S1  f0;
   struct S0  f1;
   int16_t  f2;
};

union U4 {
   uint8_t  f0;
   uint64_t  f1;
};

static int32_t g_3 = 4;
static int32_t g_34 = 0;
static struct S0 g_35[4] = {{4294967291U},{4294967291U},{4294967291U},{4294967291U}};
static int32_t *g_55 = &g_34;
static int32_t **g_54[1] = {&g_55};
static struct S1 g_68 = {0xF8DAE6E92ECD4426LL,-2,{0xF031B10C},0x78DC7DAD05572BF2LL};
static uint64_t g_72 = 0x7CD62FFE2AA969AALL;
static union U3 g_91 = {{0x639A721153E23B9CLL,0x33C15289,{4294967295U},0xFA15260EE2FB047BLL}};
static union U3 *g_90 = &g_91;

static uint16_t  func_1(void);
static uint64_t  func_12(uint64_t  p_13, int32_t * const  p_14, int32_t * p_15, int32_t  p_16, struct S1  p_17);
static uint64_t  func_18(struct S0  p_19, int64_t  p_20);

static uint16_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    union U4 l_25 = {0x0D};
    int8_t l_28 = (-7);
    (*l_2) |= 5;
lbl_95:
    for (g_3 = (-12); (g_3 > 15); g_3 = safe_add_func_int8_t_s_s(g_3, 7))
    { 
        return (*l_2);
    }
    for (g_3 = (-5); (g_3 < (-10)); g_3 = safe_sub_func_uint8_t_u_u(g_3, 8))
    { 
        int64_t l_32 = 1;
        int32_t *l_33 = &g_34;
        struct S1 l_73[1] = {{0,0x74B34BCD,{0x122BF4B2},1U}};
        uint8_t l_107 = 255U;
        int64_t *l_121 = (void*)0;
        int64_t **l_120 = &l_121;
        int i;
        if ((((safe_mul_func_int8_t_s_s(((safe_sub_func_uint64_t_u_u(func_12(func_18(((safe_sub_func_int32_t_s_s(((*l_33) = ((safe_div_func_int8_t_s_s((l_25 , (((safe_mul_func_uint8_t_u_u(l_28, (safe_unary_minus_func_int8_t_s((((safe_rshift_func_uint64_t_u_s(18446744073709551613U, l_32)) != l_32) != g_3))))) > 0) && (*l_2))), g_3)) & g_3)), 0xB8319B88)) , g_35[2]), g_3), l_33, l_33, g_72, l_73[0]), (*l_2))) != 0xBF1C), 1U)) , g_91.f0.f2.f0) < g_91.f0.f3))
        { 
            const int32_t l_101 = 0x304E8079;
            uint64_t l_113 = 1U;
            for (l_32 = (-25); (l_32 == 8); l_32++)
            { 
                int8_t l_106 = (-3);
                int32_t l_108 = (-4);
                int32_t *l_109 = &g_68.f1;
                int32_t *l_110 = &g_68.f1;
                int32_t *l_111 = &g_68.f1;
                int32_t *l_112[2][1];
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_112[i][j] = &l_73[0].f1;
                }
                if (g_72)
                    goto lbl_95;
                l_108 = ((*g_55) = (safe_add_func_int64_t_s_s((~(safe_lshift_func_int8_t_s_u(l_101, (safe_sub_func_int16_t_s_s((*l_2), (safe_lshift_func_uint64_t_u_s(((((0xA2 <= l_106) | l_106) | l_107) > (*l_33)), 48))))))), 0x44EF0715EAD26C8BLL)));
                l_113--;
            }
        }
        else
        { 
            int8_t l_116[2];
            int32_t *l_117 = &g_68.f1;
            int64_t *l_119[2][2] = {{&l_32,&l_32},{&l_32,&l_32}};
            int64_t **l_118 = &l_119[0][1];
            int i, j;
            for (i = 0; i < 2; i++)
                l_116[i] = 0x33;
            (*l_117) = ((*l_33) = l_116[1]);
            l_120 = l_118;
        }
    }
    return g_91.f0.f2.f0;
}

static uint64_t  func_12(uint64_t  p_13, int32_t * const  p_14, int32_t * p_15, int32_t  p_16, struct S1  p_17)
{ 
    int32_t *l_74 = (void*)0;
    int32_t l_75 = (-6);
    int32_t *l_76 = &l_75;
    int32_t *l_77 = (void*)0;
    int32_t *l_78 = &l_75;
    int32_t *l_79 = &l_75;
    int32_t *l_80 = (void*)0;
    int32_t *l_81 = &g_34;
    int32_t l_82 = 0x3324EF46;
    int32_t *l_83 = &g_34;
    int32_t *l_84 = &l_75;
    int32_t *l_85 = (void*)0;
    int32_t *l_86[1][4];
    uint16_t l_87[3][1];
    union U3 **l_92 = &g_90;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_86[i][j] = (void*)0;
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_87[i][j] = 0xD39A;
    }
    l_87[0][0]--;
    (*l_92) = g_90;
    return g_68.f2.f0;
}

static uint64_t  func_18(struct S0  p_19, int64_t  p_20)
{ 
    uint64_t l_38 = 1U;
    int32_t *l_51 = &g_34;
    union U3 l_53 = {{0xD3EAFCA19AB6322ALL,-1,{0x84BB8686},0x464BBCDFA8187147LL}};
    int32_t **l_56 = &g_55;
    if (((((l_38 || ((~(-10)) , ((safe_div_func_uint64_t_u_u((l_38 <= 4), l_38)) != 0xCC492975A1448AFELL))) , (void*)0) != (void*)0) == (-6)))
    { 
        uint64_t l_42 = 8U;
        int32_t l_43 = (-1);
        l_43 = (l_42 <= ((g_34 >= l_38) && g_34));
    }
    else
    { 
        int32_t *l_50 = &g_34;
        for (l_38 = 8; (l_38 >= 32); ++l_38)
        { 
            int32_t *l_47 = &g_34;
            int32_t **l_46 = &l_47;
            int64_t *l_52[2];
            int i;
            for (i = 0; i < 2; i++)
                l_52[i] = (void*)0;
            (*l_46) = (void*)0;
            l_56 = (((safe_rshift_func_int64_t_s_s((((*l_50) &= (l_50 == (p_20 , ((*l_46) = l_51)))) | 0x0C2DF100F83BF0B6LL), 62)) , l_53) , g_54[0]);
            for (p_20 = 0; (p_20 > (-24)); --p_20)
            { 
                if ((*l_47))
                    break;
                (*l_56) = l_50;
            }
        }
        if (((*l_50) != (safe_lshift_func_uint16_t_u_u((safe_mul_func_int8_t_s_s(p_20, ((((safe_rshift_func_int32_t_s_s((-1), (**l_56))) ^ p_19.f0) > p_20) , g_3))), (**l_56)))))
        { 
            for (l_53.f1.f0 = (-26); (l_53.f1.f0 < 5); l_53.f1.f0 = safe_add_func_int8_t_s_s(l_53.f1.f0, 8))
            { 
                return p_20;
            }
        }
        else
        { 
            int32_t * const l_69 = &g_34;
            int32_t **l_70 = &g_55;
            for (g_34 = 0; (g_34 >= 0); g_34 -= 1)
            { 
                struct S1 *l_67 = &g_68;
                (*l_67) = l_53.f0;
            }
            (*l_70) = l_69;
        }
        for (g_68.f1 = 0; (g_68.f1 >= 0); g_68.f1 -= 1)
        { 
            int32_t l_71 = 0xC19DE87A;
            int i;
            l_71 ^= (-5);
            if ((*g_55))
                continue;
        }
    }
    return p_20;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_34, "g_34", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_35[i].f0, "g_35[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_68.f0, "g_68.f0", print_hash_value);
    transparent_crc(g_68.f1, "g_68.f1", print_hash_value);
    transparent_crc(g_68.f2.f0, "g_68.f2.f0", print_hash_value);
    transparent_crc(g_68.f3, "g_68.f3", print_hash_value);
    transparent_crc(g_72, "g_72", print_hash_value);
    transparent_crc(g_91.f0.f0, "g_91.f0.f0", print_hash_value);
    transparent_crc(g_91.f0.f1, "g_91.f0.f1", print_hash_value);
    transparent_crc(g_91.f0.f2.f0, "g_91.f0.f2.f0", print_hash_value);
    transparent_crc(g_91.f0.f3, "g_91.f0.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
