/* Csmith 0cdc710315cf seed 35, flattened by scripts/csmith_flatten.py */
// Options:   --seed 35 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
   uint16_t  f1;
   int32_t  f2;
};

struct S1 {
   uint16_t  f0;
   uint32_t  f1;
   uint16_t  f2;
   struct S0  f3;
};

struct S2 {
   struct S1  f0;
   uint16_t  f1;
   uint8_t  f2;
   int16_t  f3;
};

static struct S2 g_12[2][1] = {{{{9U,1U,0U,{0x37,0xC054,-10}},0U,9U,0x08E2}},{{{9U,1U,0U,{0x37,0xC054,-10}},0U,9U,0x08E2}}};
static int32_t g_19 = (-4);
static int32_t *g_18 = &g_19;
static struct S0 *g_36 = (void*)0;
static int8_t g_55 = 0x13;
static uint16_t g_63 = 65531U;
static uint16_t *g_62 = &g_63;
static int8_t *g_86 = &g_55;
static int8_t **g_85 = &g_86;
static uint64_t g_104 = 0x2247AAA1B4DF1005LL;

static struct S1  func_1(void);
static struct S2  func_2(const int64_t  p_3, int32_t  p_4, uint64_t  p_5, uint32_t  p_6);
static int64_t  func_7(uint16_t  p_8, int64_t  p_9, int64_t  p_10, int16_t  p_11);

static struct S1  func_1(void)
{ 
    uint16_t l_17 = 0xFB7D;
    struct S2 *l_38 = (void*)0;
    struct S2 *l_39 = (void*)0;
    struct S2 *l_40[3];
    uint16_t l_41 = 1U;
    int8_t *l_54 = &g_55;
    int32_t **l_56 = &g_18;
    int32_t l_57[1];
    int32_t *l_101 = (void*)0;
    struct S1 l_108 = {0x783B,4294967295U,7U,{0x1A,65535U,0xD4B5270F}};
    int i;
    for (i = 0; i < 3; i++)
        l_40[i] = &g_12[0][0];
    for (i = 0; i < 1; i++)
        l_57[i] = 1;
    g_12[1][0] = func_2(func_7((g_12[0][0] , (safe_add_func_int16_t_s_s((safe_lshift_func_int8_t_s_s(l_17, 7)), (((((1U & l_17) | l_17) != l_17) ^ 1U) & g_12[0][0].f0.f2)))), g_12[0][0].f2, g_12[0][0].f0.f3.f0, g_12[0][0].f0.f3.f1), g_12[0][0].f0.f3.f1, l_17, g_12[0][0].f1);
    (*g_18) &= l_41;
    if ((safe_sub_func_int32_t_s_s((((safe_sub_func_uint64_t_u_u((safe_mod_func_uint64_t_u_u((((safe_sub_func_int8_t_s_s(g_12[0][0].f0.f2, (((safe_mod_func_int8_t_s_s(l_41, ((*l_54) |= (safe_div_func_int8_t_s_s(l_41, g_12[0][0].f2))))) , &g_18) == l_56))) < (**l_56)) == (*g_18)), g_12[0][0].f0.f1)), l_57[0])) <= 0xC865) < g_12[0][0].f2), (*g_18))))
    { 
        uint16_t **l_64 = (void*)0;
        uint16_t *l_66 = &g_12[0][0].f0.f0;
        uint16_t **l_65 = &l_66;
        int32_t l_67 = 0xE514D861;
        int32_t l_90[4][1];
        int64_t *l_100 = (void*)0;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_90[i][j] = 1;
        }
        l_67 ^= ((**l_56) = (safe_sub_func_int8_t_s_s((((safe_sub_func_int16_t_s_s(g_12[0][0].f0.f1, (g_62 != ((*l_65) = &g_63)))) == 4294967290U) <= g_12[0][0].f0.f2), g_63)));
        if ((safe_rshift_func_uint8_t_u_u((safe_mod_func_int16_t_s_s((((safe_rshift_func_uint64_t_u_s(((safe_rshift_func_uint64_t_u_s(((g_12[0][0].f0.f3.f0 & ((safe_add_func_int32_t_s_s((safe_lshift_func_int64_t_s_s((**l_56), g_12[0][0].f2)), 0)) >= (**l_56))) | g_12[0][0].f0.f3.f0), g_12[0][0].f0.f0)) > 0x7A), g_12[0][0].f0.f0)) && l_67) ^ g_12[0][0].f0.f3.f0), g_19)), 4)))
        { 
            int16_t l_83 = (-1);
            uint64_t l_91 = 0x42F02481C6DCE702LL;
            if ((safe_lshift_func_uint8_t_u_s((safe_unary_minus_func_int16_t_s((l_83 | (g_12[0][0].f0.f0 != g_63)))), (**l_56))))
            { 
                int8_t **l_84 = &l_54;
                g_85 = l_84;
                (*g_18) = l_67;
            }
            else
            { 
                int32_t *l_87 = &g_12[0][0].f0.f3.f2;
                int32_t *l_88 = &g_12[0][0].f0.f3.f2;
                int32_t *l_89[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_89[i] = &l_67;
                --l_91;
                (*l_87) |= ((((safe_mod_func_uint64_t_u_u((safe_sub_func_int32_t_s_s(0x0483F316, (safe_mod_func_uint8_t_u_u(((((void*)0 == l_100) || (*g_86)) , l_83), (**l_56))))), 0xF19FB563853DC631LL)) != g_12[0][0].f0.f2) <= 0xC5ED) && 0x0B84261728A6F30DLL);
            }
        }
        else
        { 
            int32_t *l_102 = &l_90[1][0];
            (*l_56) = l_101;
            (*l_102) = (&g_18 == &g_18);
        }
    }
    else
    { 
        uint64_t *l_103 = &g_104;
        int32_t *l_107[1][3];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_107[i][j] = &l_57[0];
        }
        g_12[0][0].f0.f3.f2 |= ((g_12[0][0].f0.f0 | (++(*l_103))) > (*g_18));
    }
    return l_108;
}

static struct S2  func_2(const int64_t  p_3, int32_t  p_4, uint64_t  p_5, uint32_t  p_6)
{ 
    uint16_t *l_25 = &g_12[0][0].f0.f2;
    int64_t *l_30 = (void*)0;
    int32_t l_31 = 0;
    struct S0 *l_34 = &g_12[0][0].f0.f3;
    struct S0 **l_35[3][1];
    struct S2 l_37 = {{0x9C04,4U,0x8079,{0x68,65527U,0x5965344F}},0xA2B8,251U,-5};
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_35[i][j] = &l_34;
    }
    (*g_18) ^= (safe_div_func_int64_t_s_s(((safe_mul_func_uint16_t_u_u(((*l_25)++), (safe_sub_func_int64_t_s_s((g_12[0][0].f0.f3.f0 & g_12[0][0].f0.f3.f2), (((l_31 = g_12[0][0].f0.f1) || p_6) < g_12[0][0].f0.f3.f1))))) , l_31), 0x9E5E26C5F514FC6BLL));
    (*g_18) &= (safe_add_func_uint16_t_u_u(l_31, 0U));
    g_36 = l_34;
    return l_37;
}

static int64_t  func_7(uint16_t  p_8, int64_t  p_9, int64_t  p_10, int16_t  p_11)
{ 
    int32_t **l_20 = &g_18;
    (*l_20) = g_18;
    return g_12[0][0].f1;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_12[i][j].f0.f0, "g_12[i][j].f0.f0", print_hash_value);
            transparent_crc(g_12[i][j].f0.f1, "g_12[i][j].f0.f1", print_hash_value);
            transparent_crc(g_12[i][j].f0.f2, "g_12[i][j].f0.f2", print_hash_value);
            transparent_crc(g_12[i][j].f0.f3.f0, "g_12[i][j].f0.f3.f0", print_hash_value);
            transparent_crc(g_12[i][j].f0.f3.f1, "g_12[i][j].f0.f3.f1", print_hash_value);
            transparent_crc(g_12[i][j].f0.f3.f2, "g_12[i][j].f0.f3.f2", print_hash_value);
            transparent_crc(g_12[i][j].f1, "g_12[i][j].f1", print_hash_value);
            transparent_crc(g_12[i][j].f2, "g_12[i][j].f2", print_hash_value);
            transparent_crc(g_12[i][j].f3, "g_12[i][j].f3", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_104, "g_104", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
