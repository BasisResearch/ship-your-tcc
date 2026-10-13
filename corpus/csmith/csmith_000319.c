/* Csmith 0cdc710315cf seed 319, flattened by scripts/csmith_flatten.py */
// Options:   --seed 319 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_unary_minus_func_int64_t_s(int64_t si )
{
  return
    (si==(-9223372036854775807L-1)) ?
    ((si)) :
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int32_t  f0;
};

static int32_t g_2 = 0xD7D490C4;
static uint8_t g_14 = 0x0F;
static int32_t g_19 = (-7);
static uint32_t g_47 = 0xC1543F36;
static int64_t g_50 = (-7);
static uint16_t g_63[3] = {65535U,65535U,65535U};
static uint8_t g_68 = 0x4A;
static int32_t g_72 = 0xF4B1A25D;
static int32_t *g_103 = &g_19;
static int8_t g_119 = 0xAF;
static int64_t g_123 = 0;
static int32_t g_126 = (-1);
static int32_t g_127 = (-1);

static uint16_t  func_1(void);
static int32_t  func_21(uint8_t  p_22, const uint8_t  p_23, int8_t  p_24, uint8_t * p_25);
static uint8_t  func_26(uint32_t  p_27, uint16_t  p_28);

static uint16_t  func_1(void)
{ 
    int16_t l_17 = 0xCAD7;
    struct S0 l_45 = {-8};
    int32_t l_48[1];
    int32_t **l_121[3][4];
    int i, j;
    for (i = 0; i < 1; i++)
        l_48[i] = 0xF4ED832C;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
            l_121[i][j] = &g_103;
    }
    for (g_2 = 14; (g_2 <= 24); g_2 = safe_add_func_int64_t_s_s(g_2, 1))
    { 
        uint16_t l_9[4];
        uint8_t *l_12 = (void*)0;
        uint8_t *l_13 = &g_14;
        int32_t *l_18[3][4] = {{(void*)0,&g_19,(void*)0,&g_19},{(void*)0,&g_19,&g_19,(void*)0},{&g_2,&g_19,&g_2,&g_19}};
        int8_t l_20 = 0xC6;
        uint32_t *l_46 = &g_47;
        int64_t *l_49 = &g_50;
        int64_t *l_122 = &g_123;
        int i, j;
        for (i = 0; i < 4; i++)
            l_9[i] = 65528U;
        l_20 &= (safe_div_func_int32_t_s_s((safe_add_func_int16_t_s_s((l_9[1] != ((safe_rshift_func_int8_t_s_u(0xDC, (((*l_13)++) != l_17))) > 1)), g_2)), l_9[1]));
        g_119 &= func_21(func_26((safe_unary_minus_func_int64_t_s((safe_add_func_int16_t_s_s((safe_lshift_func_int8_t_s_s(((l_17 <= (safe_div_func_int64_t_s_s(((*l_49) = (((safe_add_func_int8_t_s_s((safe_add_func_int32_t_s_s(g_2, (((safe_lshift_func_uint64_t_u_u(((((safe_mul_func_uint32_t_u_u(((*l_46) = (~(((g_2 >= (-1)) , l_45) , g_2))), g_2)) > 0x691704D9) ^ 0xAF) || (-4)), g_14)) , g_47) && l_45.f0))), g_14)) <= l_48[0]) , g_47)), 0x942F34C933337672LL))) != g_14), 5)), g_19)))), l_45.f0), g_2, l_17, l_12);
        if ((((*l_122) &= ((*l_49) = (!((((void*)0 != l_121[2][0]) && g_119) , 0x8ABD3BCAB3842746LL)))) != g_119))
        { 
            int32_t *l_124 = (void*)0;
            int32_t l_125 = 1;
            if ((g_119 , 0x896C2AEC))
            { 
                l_124 = &g_2;
            }
            else
            { 
                uint64_t l_128 = 0x22BBA05350A10410LL;
                --l_128;
            }
        }
        else
        { 
            return g_126;
        }
    }
    g_72 |= 0xFD13C1E3;
    return g_63[0];
}

static int32_t  func_21(uint8_t  p_22, const uint8_t  p_23, int8_t  p_24, uint8_t * p_25)
{ 
    int16_t l_110 = 0x473C;
    for (p_22 = 0; (p_22 <= 2); p_22 += 1)
    { 
        int32_t *l_108 = &g_19;
        int32_t *l_109 = (void*)0;
        int32_t *l_111 = &g_19;
        int32_t *l_112 = &g_72;
        int32_t *l_113 = &g_72;
        int32_t *l_114 = &g_19;
        int32_t *l_115 = &g_19;
        uint16_t l_116 = 65532U;
        ++l_116;
    }
    return l_110;
}

static uint8_t  func_26(uint32_t  p_27, uint16_t  p_28)
{ 
    int64_t l_54 = 1;
    int32_t *l_67 = &g_19;
    int32_t **l_66 = &l_67;
    uint32_t *l_80 = &g_47;
    int8_t l_81[3];
    struct S0 l_84 = {0x26771399};
    uint8_t *l_96 = &g_14;
    int i;
    for (i = 0; i < 3; i++)
        l_81[i] = (-1);
    for (g_47 = (-24); (g_47 == 53); g_47++)
    { 
        int32_t *l_53 = (void*)0;
        int32_t *l_55 = &g_19;
        int32_t *l_56 = &g_19;
        int32_t *l_57 = &g_19;
        int32_t *l_58 = (void*)0;
        int32_t *l_59 = &g_19;
        int32_t *l_60 = &g_19;
        int32_t l_61 = (-1);
        int32_t *l_62 = &l_61;
        g_63[0]--;
        l_66 = &l_59;
        for (g_50 = 2; (g_50 >= 0); g_50 -= 1)
        { 
            if (g_2)
                break;
            for (l_61 = 0; (l_61 <= 2); l_61 += 1)
            { 
                int32_t *l_71 = &g_72;
                ++g_68;
                (*l_71) = ((*l_57) = g_2);
                (*l_66) = &l_61;
            }
        }
    }
    if ((safe_mul_func_int16_t_s_s(((((*l_67) = (*l_67)) != ((*l_80) = (safe_mod_func_uint32_t_u_u((+g_14), (safe_add_func_uint8_t_u_u((p_27 | p_28), 0)))))) <= 1), l_81[2])))
    { 
        int32_t l_85 = (-1);
        l_85 = (((safe_add_func_uint16_t_u_u(p_28, p_27)) , l_84) , (*l_67));
    }
    else
    { 
        struct S0 l_88 = {0xF84E0F8C};
        int32_t *l_98 = &l_84.f0;
        int32_t *l_101[1];
        int32_t **l_102[3];
        struct S0 *l_106 = (void*)0;
        struct S0 *l_107 = &l_88;
        int i;
        for (i = 0; i < 1; i++)
            l_101[i] = (void*)0;
        for (i = 0; i < 3; i++)
            l_102[i] = &l_101[0];
        for (l_54 = 29; (l_54 == (-6)); --l_54)
        { 
            struct S0 *l_89 = &l_84;
            int32_t l_97 = (-3);
            (*l_89) = l_88;
            if ((safe_mod_func_int8_t_s_s(g_2, p_27)))
            { 
                l_97 |= (safe_mul_func_uint8_t_u_u(0x47, (safe_mod_func_int64_t_s_s(((void*)0 == l_96), 0xDD1A6511C0E03A84LL))));
                if (g_19)
                    continue;
            }
            else
            { 
                return l_97;
            }
            l_98 = &g_19;
        }
        (*l_107) = ((safe_div_func_uint64_t_u_u(((g_103 = l_101[0]) != &g_19), (safe_mod_func_uint32_t_u_u(g_72, p_28)))) , l_84);
        l_67 = (void*)0;
    }
    return g_14;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_50, "g_50", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_63[i], "g_63[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_68, "g_68", print_hash_value);
    transparent_crc(g_72, "g_72", print_hash_value);
    transparent_crc(g_119, "g_119", print_hash_value);
    transparent_crc(g_123, "g_123", print_hash_value);
    transparent_crc(g_126, "g_126", print_hash_value);
    transparent_crc(g_127, "g_127", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
