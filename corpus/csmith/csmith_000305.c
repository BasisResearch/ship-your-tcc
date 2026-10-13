/* Csmith 0cdc710315cf seed 305, flattened by scripts/csmith_flatten.py */
// Options:   --seed 305 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint32_t  f0;
   uint64_t  f1;
};

union U1 {
   int32_t  f0;
   uint32_t  f1;
};

union U2 {
   int32_t  f0;
   uint8_t  f1;
};

union U4 {
   struct S0  f0;
};

static int32_t g_2[3][3] = {{0xE4438BEA,0xE4438BEA,0xE4438BEA},{0xE4438BEA,0xE4438BEA,0xE4438BEA},{0xE4438BEA,0xE4438BEA,0xE4438BEA}};
static union U4 g_20[1][2] = {{{{0x3E45BBBD,0U}},{{0x3E45BBBD,0U}}}};
static uint32_t g_23 = 0U;
static uint16_t g_26 = 0x6B71;
static int64_t g_30 = 0;
static int32_t g_44 = 1;
static int32_t * const g_49 = &g_44;
static int32_t * const * const g_48 = &g_49;
static int16_t g_52 = 2;
static int32_t *g_57[2][1] = {{&g_44},{&g_44}};
static union U1 g_66 = {1};

static int32_t  func_1(void);
static uint32_t  func_9(uint8_t  p_10, int16_t  p_11, struct S0  p_12, int8_t  p_13, union U2  p_14);
static uint8_t  func_15(int8_t  p_16, int8_t  p_17, uint8_t  p_18, uint16_t  p_19);

static int32_t  func_1(void)
{ 
    union U2 l_27 = {5};
    uint64_t l_82 = 0xA73889CFDD8C90CCLL;
    int32_t l_92 = 0xCA1FCAC0;
    for (g_2[0][2] = 28; (g_2[0][2] >= (-9)); g_2[0][2] = safe_sub_func_uint32_t_u_u(g_2[0][2], 4))
    { 
        uint32_t *l_21 = (void*)0;
        uint32_t *l_22 = &g_23;
        int32_t l_53 = 0xEC458AF3;
        int32_t l_77 = (-7);
        uint32_t **l_78 = &l_22;
        if ((safe_lshift_func_int8_t_s_u((safe_rshift_func_uint32_t_u_u(func_9(func_15((((g_20[0][0] , (g_2[0][2] ^ (g_26 = (--(*l_22))))) >= (l_21 != (void*)0)) >= 0xCADFB1AF), g_20[0][0].f0.f0, g_20[0][0].f0.f1, g_2[2][1]), g_20[0][0].f0.f1, g_20[0][0].f0, g_20[0][0].f0.f0, l_27), l_27.f0)), g_2[0][2])))
        { 
            uint16_t l_50 = 65535U;
            int16_t *l_51 = &g_52;
            int32_t **l_67[4];
            int32_t **l_68[4][4];
            int32_t **l_69 = &g_57[0][0];
            int16_t *l_74 = &g_52;
            int16_t **l_73 = &l_74;
            int i, j;
            for (i = 0; i < 4; i++)
                l_67[i] = &g_57[1][0];
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                    l_68[i][j] = &g_57[1][0];
            }
            if ((!(safe_mul_func_uint64_t_u_u((g_48 != &g_49), ((((*l_51) = (0x978C == l_50)) < g_30) , 0)))))
            { 
                if ((**g_48))
                    break;
            }
            else
            { 
                int32_t *l_55[4] = {&l_27.f0,&l_27.f0,&l_27.f0,&l_27.f0};
                int32_t **l_54 = &l_55[0];
                int32_t **l_56[2][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
                const int32_t *l_61 = (void*)0;
                const int32_t **l_60 = &l_61;
                int i, j;
                if (l_53)
                    break;
                g_57[1][0] = ((*l_54) = (void*)0);
                l_53 = (safe_sub_func_int32_t_s_s(((*g_48) != ((*l_60) = (*l_54))), (safe_div_func_int16_t_s_s(((((safe_rshift_func_uint16_t_u_u((g_20[0][0].f0 , g_23), g_44)) , g_66) , (void*)0) == &g_52), 0xD2BB))));
            }
            (*l_69) = (*g_48);
            l_77 |= (l_27.f0 = ((~((safe_lshift_func_int16_t_s_s((((l_51 == ((*l_73) = &g_52)) >= (safe_rshift_func_uint32_t_u_u(l_53, l_27.f0))) != 65535U), g_52)) ^ 65533U)) , (**g_48)));
        }
        else
        { 
            int32_t l_79 = 0xF70C77B7;
            l_78 = &l_21;
            if (l_79)
                break;
            for (l_77 = 0; (l_77 >= 5); ++l_77)
            { 
                if (l_82)
                    break;
            }
        }
    }
    if ((safe_div_func_int8_t_s_s(g_23, ((l_27.f0 ^ g_26) , l_82))))
    { 
        for (l_27.f1 = 0; (l_27.f1 >= 53); l_27.f1++)
        { 
            int32_t **l_87 = &g_57[0][0];
            (*l_87) = (*g_48);
            if ((*g_49))
                continue;
            (**g_48) = l_82;
        }
    }
    else
    { 
        uint32_t l_90 = 0xBA0F3F00;
        uint64_t *l_91[2];
        int32_t l_93[4];
        uint32_t l_100 = 0xBBB00C11;
        int i;
        for (i = 0; i < 2; i++)
            l_91[i] = &g_20[0][0].f0.f1;
        for (i = 0; i < 4; i++)
            l_93[i] = 0x3C27CC01;
        if ((l_27.f0 & ((safe_add_func_uint64_t_u_u((l_93[3] &= (l_92 ^= l_90)), 0xDF88EE36B02009A6LL)) , l_82)))
        { 
            int16_t l_96 = (-3);
            int32_t l_97 = 0;
            int32_t *l_103 = (void*)0;
            int32_t *l_104 = (void*)0;
            int32_t *l_105[4];
            uint64_t l_106[1];
            int i;
            for (i = 0; i < 4; i++)
                l_105[i] = &l_97;
            for (i = 0; i < 1; i++)
                l_106[i] = 0xC007EE7193220CCBLL;
            for (g_44 = (-3); (g_44 == 5); g_44 = safe_add_func_int64_t_s_s(g_44, 3))
            { 
                int32_t *l_98 = &l_93[3];
                int32_t *l_99[3][4] = {{&l_27.f0,&l_27.f0,(void*)0,&l_27.f0},{&l_27.f0,&l_93[3],&l_93[3],&l_27.f0},{&l_93[3],&l_27.f0,&l_93[3],&l_93[3]}};
                int i, j;
                if (l_96)
                    break;
                l_100++;
            }
            ++l_106[0];
            return l_27.f0;
        }
        else
        { 
            for (l_92 = 0; l_92 < 3; l_92 = l_92 + 1)
            {
                for (g_66.f1 = 0; g_66.f1 < 3; g_66.f1 = g_66.f1 + 1)
                {
                    g_2[l_92][g_66.f1] = 0xA17A55E9;
                }
            }
        }
    }
    return l_92;
}

static uint32_t  func_9(uint8_t  p_10, int16_t  p_11, struct S0  p_12, int8_t  p_13, union U2  p_14)
{ 
    int32_t *l_28 = &g_2[0][2];
    int32_t **l_29 = &l_28;
    union U4 l_41 = {{4294967287U,0x131EC8E12B6A3604LL}};
    (*l_29) = l_28;
    g_30 = (-1);
    if (g_2[1][2])
    { 
        return p_13;
    }
    else
    { 
        int32_t l_37[3][3];
        uint16_t l_38[4] = {7U,7U,7U,7U};
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
                l_37[i][j] = 0xC51AC06E;
        }
        p_14.f0 ^= (safe_mul_func_int64_t_s_s((safe_lshift_func_uint64_t_u_s((((safe_add_func_uint8_t_u_u(l_37[1][2], 5)) | (*l_28)) && l_37[1][2]), l_38[2])), g_2[0][2]));
        for (p_11 = 3; (p_11 >= 0); p_11 -= 1)
        { 
            const int32_t *l_39 = (void*)0;
            const int32_t **l_40 = &l_39;
            (*l_40) = l_39;
            for (p_12.f1 = 0; (p_12.f1 <= 3); p_12.f1 += 1)
            { 
                int i;
                return l_38[p_12.f1];
            }
            for (p_10 = 0; (p_10 <= 3); p_10 += 1)
            { 
                int32_t *l_43 = &g_44;
                int i;
                (*l_43) |= (((l_41 , 0x8CDA) | (l_38[p_11] |= (!(l_37[1][2] = 5)))) >= g_2[0][0]);
            }
        }
    }
    return p_13;
}

static uint8_t  func_15(int8_t  p_16, int8_t  p_17, uint8_t  p_18, uint16_t  p_19)
{ 
    return p_16;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_2[i][j], "g_2[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_20[i][j].f0.f0, "g_20[i][j].f0.f0", print_hash_value);
            transparent_crc(g_20[i][j].f0.f1, "g_20[i][j].f0.f1", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_66.f0, "g_66.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
