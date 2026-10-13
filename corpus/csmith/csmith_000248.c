/* Csmith 0cdc710315cf seed 248, flattened by scripts/csmith_flatten.py */
// Options:   --seed 248 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
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
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
};

static int32_t g_2 = (-1);
static int32_t g_5[4][1] = {{2},{2},{2},{2}};
static uint32_t g_21 = 0x065C2DD7;
static uint32_t g_29 = 0xA5308633;
static int8_t g_33[3] = {1,1,1};
static int32_t g_35 = 0xF9F3B06D;
static int64_t g_48 = 0xB1D4416DA84B505DLL;
static uint32_t g_60[2][1] = {{0x9B4DB59F},{0x9B4DB59F}};
static const struct S0 g_64 = {-3};
static int32_t *g_66 = &g_2;
static int32_t **g_65 = &g_66;
static int16_t g_85 = 0xDA01;
static uint64_t g_92 = 0xF27357609E5B7364LL;
static struct S0 g_97 = {5};
static int16_t g_102 = 1;
static int16_t g_105 = 1;
static struct S0 *g_114 = &g_97;
static int16_t g_149 = 0x96E4;
static uint8_t g_150 = 0xA6;
static int64_t g_155 = 0x90C0348559906447LL;
static uint16_t g_156 = 65535U;
static const uint32_t *g_166 = &g_21;
static const uint32_t **g_165[3][2] = {{&g_166,&g_166},{&g_166,&g_166},{&g_166,&g_166}};
static uint64_t g_184 = 0x735A46787CD55F75LL;
static uint32_t *g_207 = &g_21;
static int32_t *g_219[4][2] = {{&g_35,&g_35},{&g_35,&g_35},{&g_35,&g_35},{&g_35,&g_35}};

static int16_t  func_1(void);
static int32_t * func_8(int64_t  p_9, uint64_t  p_10, int32_t * p_11, uint64_t  p_12, int32_t * p_13);
static int64_t  func_22(int32_t  p_23, uint8_t  p_24, uint32_t * p_25, uint16_t  p_26, uint16_t  p_27);

static int16_t  func_1(void)
{ 
    int8_t l_19 = 0x80;
    uint32_t *l_32 = &g_29;
    struct S0 l_252 = {1};
    int32_t l_254 = 0xED05F352;
    for (g_2 = 20; (g_2 >= (-9)); g_2 = safe_sub_func_int32_t_s_s(g_2, 8))
    { 
        uint32_t *l_28 = &g_29;
        int32_t l_106 = 0xF57EB907;
        int32_t l_107 = 0x346F960B;
        struct S0 *l_271 = &g_97;
        for (g_5[1][0] = 0; (g_5[1][0] < 7); g_5[1][0] = safe_add_func_uint8_t_u_u(g_5[1][0], 5))
        { 
            uint32_t *l_20 = &g_21;
            uint32_t **l_30 = &l_28;
            int8_t l_31 = 0x64;
            int32_t l_100 = 0x16E9852B;
            int16_t *l_101 = &g_102;
            int16_t *l_103 = (void*)0;
            int16_t *l_104[2];
            int32_t *l_253[1][2];
            int i, j;
            for (i = 0; i < 2; i++)
                l_104[i] = &g_105;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_253[i][j] = (void*)0;
            }
            (*g_65) = func_8((((~(safe_div_func_int16_t_s_s((l_106 |= ((((safe_lshift_func_uint32_t_u_u(((*l_20) ^= l_19), 31)) > ((*l_101) ^= (func_22(((((l_31 = (((*l_30) = l_28) != &g_29)) >= g_5[1][0]) >= 0x55C0C8FBFC75DAA0LL) != 0x3800955205300CE3LL), g_2, l_32, g_5[1][0], l_19) ^ l_100))) || 0xFFA30257628AE825LL) || 0x07418113439BF9ABLL)), l_100))) && l_100) , g_92), g_5[1][0], l_32, l_107, &l_100);
            (*g_114) = l_252;
            l_254 &= l_19;
        }
        for (g_92 = (-10); (g_92 != 46); g_92 = safe_add_func_uint32_t_u_u(g_92, 1))
        { 
            int64_t l_266 = (-1);
            uint8_t *l_269 = &g_150;
            uint64_t *l_270 = &g_184;
            struct S0 **l_272 = &g_114;
            int64_t *l_273 = &g_48;
            uint16_t l_274 = 0xA182;
            int32_t l_275[1];
            int8_t *l_276 = (void*)0;
            int8_t *l_277 = &l_19;
            int32_t *l_278 = (void*)0;
            int i;
            for (i = 0; i < 1; i++)
                l_275[i] = 0x8B874153;
            (*g_65) = func_8((((*l_277) ^= (safe_mul_func_uint32_t_u_u((l_275[0] |= ((safe_mul_func_int16_t_s_s((((*l_273) &= (((*l_272) = (((!(((safe_lshift_func_int16_t_s_s(((safe_sub_func_int32_t_s_s(l_106, ((*l_32) &= ((*g_207) |= ((l_266 ^ (((((*l_270) = ((safe_rshift_func_uint64_t_u_u((l_269 != &g_150), 21)) == 0U)) == l_107) > g_149) && l_266)) <= 65529U))))) || l_107), l_252.f0)) | 0U) | l_266)) >= 0xA0) , l_271)) == &g_64)) , 0x7CC2), 1U)) , l_274)), 0x6E46704D))) > 0xC0), g_35, l_278, l_274, (*g_65));
            l_252 = (*g_114);
        }
    }
    return g_60[1][0];
}

static int32_t * func_8(int64_t  p_9, uint64_t  p_10, int32_t * p_11, uint64_t  p_12, int32_t * p_13)
{ 
    int32_t l_108[1];
    uint32_t *l_119 = &g_60[1][0];
    struct S0 **l_129 = &g_114;
    uint32_t * const *l_168 = (void*)0;
    int32_t l_214 = 7;
    int i;
    for (i = 0; i < 1; i++)
        l_108[i] = 0x2E7910AB;
    return &g_2;
}

static int64_t  func_22(int32_t  p_23, uint8_t  p_24, uint32_t * p_25, uint16_t  p_26, uint16_t  p_27)
{ 
    int8_t l_46 = (-1);
    int32_t *l_51 = (void*)0;
    int32_t l_54 = 0xF24264A3;
    int32_t l_55 = (-1);
    int32_t l_57 = 0xD8E66C79;
    int32_t l_58 = 0x9286DF49;
    int32_t l_59 = 1;
    struct S0 *l_99 = &g_97;
    struct S0 **l_98 = &l_99;
    for (g_29 = 0; (g_29 <= 2); g_29 += 1)
    { 
        int32_t *l_36 = (void*)0;
        int32_t l_56 = 1;
        int32_t *l_63 = &l_54;
        int i;
        if (g_33[g_29])
        { 
            int32_t *l_34 = &g_35;
            int i;
            (*l_34) |= g_33[g_29];
        }
        else
        { 
            int64_t *l_47 = &g_48;
            int32_t l_49 = (-4);
            int32_t l_53[2][2];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 2; j++)
                    l_53[i][j] = 0xDC4EA3C9;
            }
            for (g_35 = 0; (g_35 <= 2); g_35 += 1)
            { 
                int32_t **l_37 = &l_36;
                int i;
                (*l_37) = l_36;
                p_23 = g_33[g_35];
            }
            if ((safe_sub_func_uint8_t_u_u((safe_add_func_int8_t_s_s((safe_mul_func_int8_t_s_s((g_33[g_29] = (safe_mod_func_int64_t_s_s(((((*l_47) = (p_24 , ((0x78CD908F == l_46) > p_26))) <= p_23) , l_49), g_5[3][0]))), p_24)), g_35)), 0xB4)))
            { 
                int32_t **l_50[2][1];
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_50[i][j] = &l_36;
                }
                l_51 = &g_5[2][0];
            }
            else
            { 
                int32_t *l_52[2][2];
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_52[i][j] = &l_49;
                }
                g_60[0][0]++;
            }
            p_23 &= g_35;
        }
        p_23 = ((*l_63) = p_27);
        if (((*l_63) = (g_64 , (g_65 == ((safe_mod_func_uint64_t_u_u(((safe_mod_func_uint64_t_u_u(p_27, 0x119E2D4DC4505AF7LL)) , 0xBD26DA6125F89738LL), g_29)) , &l_36)))))
        { 
            struct S0 l_71 = {0x87EDF5A291505FFFLL};
            struct S0 *l_72 = &l_71;
            (*l_72) = l_71;
        }
        else
        { 
            for (l_55 = 0; (l_55 <= 0); l_55 += 1)
            { 
                int16_t *l_84 = &g_85;
                int32_t *l_86 = &g_35;
                int32_t *l_87 = &l_54;
                int32_t *l_88 = &l_57;
                int32_t *l_89 = &l_56;
                int32_t *l_90 = &l_56;
                int32_t *l_91 = &l_56;
                struct S0 *l_96 = &g_97;
                struct S0 **l_95 = &l_96;
                int i, j;
                (*l_63) &= (((((+(((safe_mul_func_int64_t_s_s((safe_add_func_int16_t_s_s(((*l_84) = (safe_sub_func_int64_t_s_s(((safe_mul_func_uint8_t_u_u((((safe_sub_func_int64_t_s_s(g_33[g_29], (&g_64 == &g_64))) , (void*)0) != (void*)0), g_33[(l_55 + 2)])) > 0x0632420D), g_60[l_55][l_55]))), 1)), 18446744073709551614U)) , 6U) & 0x5B)) || g_35) > 0x120EAC2F6E5819C3LL) ^ p_26) , p_24);
                ++g_92;
                l_98 = l_95;
            }
            (*g_65) = &p_23;
            (*g_66) = (-1);
        }
    }
    return g_60[0][0];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_5[i][j], "g_5[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_33[i], "g_33[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_60[i][j], "g_60[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_64.f0, "g_64.f0", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_92, "g_92", print_hash_value);
    transparent_crc(g_97.f0, "g_97.f0", print_hash_value);
    transparent_crc(g_102, "g_102", print_hash_value);
    transparent_crc(g_105, "g_105", print_hash_value);
    transparent_crc(g_149, "g_149", print_hash_value);
    transparent_crc(g_150, "g_150", print_hash_value);
    transparent_crc(g_155, "g_155", print_hash_value);
    transparent_crc(g_156, "g_156", print_hash_value);
    transparent_crc(g_184, "g_184", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
