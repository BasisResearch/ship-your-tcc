/* Csmith 0cdc710315cf seed 199, flattened by scripts/csmith_flatten.py */
// Options:   --seed 199 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
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

union U0 {
   int32_t  f0;
   const uint8_t  f1;
   uint32_t  f2;
};

static uint64_t g_9 = 0x5059B8BC588E1B61LL;
static int64_t g_20 = 0x4671013923CCDE6BLL;
static int32_t g_22 = 0x351A9382;
static int32_t *g_21 = &g_22;
static int32_t g_31 = 0;
static uint32_t g_39 = 0U;
static int32_t g_47 = 0x3780D777;
static uint8_t g_79 = 255U;
static uint8_t g_81 = 255U;
static uint64_t g_85[3] = {0U,0U,0U};
static union U0 g_91[4] = {{0x136C1A4E},{0x136C1A4E},{0x136C1A4E},{0x136C1A4E}};
static int32_t g_99 = 0x3D2F63B6;
static const int32_t *g_98 = &g_99;
static uint16_t g_102[1] = {0x6458};

static union U0  func_1(void);
static int32_t * func_2(int32_t * p_3, int32_t  p_4, const int32_t * p_5, int32_t * p_6, int32_t  p_7);
static int32_t * func_12(int32_t * p_13, const uint64_t * p_14, int32_t * const  p_15, int8_t  p_16, const int16_t  p_17);

static union U0  func_1(void)
{ 
    uint64_t *l_8 = &g_9;
    int64_t *l_18 = (void*)0;
    int64_t *l_19 = &g_20;
    const int32_t l_23 = 8;
    int8_t l_24 = 0x86;
    int32_t **l_105 = &g_21;
    union U0 l_106 = {0xF67A6C79};
    (*l_105) = func_2(((--(*l_8)) , func_12(((((*l_19) = (0xE1B1B552 && 0x33880E03)) || ((((g_21 != &g_22) , l_23) , l_24) < g_22)) , &g_22), &g_9, &g_22, g_9, g_22)), g_91[2].f0, g_98, &g_99, l_24);
    return l_106;
}

static int32_t * func_2(int32_t * p_3, int32_t  p_4, const int32_t * p_5, int32_t * p_6, int32_t  p_7)
{ 
    int32_t *l_100 = (void*)0;
    int32_t *l_101[1][1];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_101[i][j] = &g_91[2].f0;
    }
    g_102[0]--;
    (*p_6) = (*g_21);
    return l_100;
}

static int32_t * func_12(int32_t * p_13, const uint64_t * p_14, int32_t * const  p_15, int8_t  p_16, const int16_t  p_17)
{ 
    int32_t **l_25 = (void*)0;
    int32_t **l_26 = (void*)0;
    int32_t **l_27 = &g_21;
    uint64_t l_77 = 0x5DE23EDE1F3C6E3ALL;
    int32_t l_84 = 0x8275CA95;
lbl_34:
    (*p_13) = (*p_15);
    (*l_27) = &g_22;
    for (g_22 = 9; (g_22 <= (-27)); g_22 = safe_sub_func_uint32_t_u_u(g_22, 7))
    { 
        int32_t *l_30 = &g_31;
        uint16_t l_71 = 65535U;
        int32_t l_83 = 1;
        union U0 l_92 = {-2};
        if (((*l_30) |= (*p_13)))
        { 
            for (g_20 = 0; (g_20 < 0); g_20 = safe_add_func_uint8_t_u_u(g_20, 6))
            { 
                if ((*g_21))
                    break;
                if (g_22)
                    goto lbl_34;
            }
            if ((*l_30))
                break;
        }
        else
        { 
            uint16_t l_37[2];
            uint32_t *l_38[4][4] = {{(void*)0,&g_39,&g_39,(void*)0},{&g_39,(void*)0,&g_39,&g_39},{(void*)0,(void*)0,&g_39,(void*)0},{(void*)0,&g_39,&g_39,(void*)0}};
            int32_t *l_46 = &g_47;
            int i, j;
            for (i = 0; i < 2; i++)
                l_37[i] = 65527U;
            (*l_46) |= (~(safe_unary_minus_func_int64_t_s(((((*l_30) = l_37[0]) >= (safe_mul_func_uint64_t_u_u((((safe_mod_func_uint8_t_u_u((safe_lshift_func_int32_t_s_s((-8), 15)), 1)) >= 0xE5685E9D) , g_9), 18446744073709551614U))) , (**l_27)))));
            for (g_20 = 0; (g_20 <= 1); g_20 += 1)
            { 
                int i, j;
                (*l_27) = l_38[(g_20 + 1)][(g_20 + 1)];
                (*l_30) = (((l_37[g_20] == (((g_9 = (safe_mul_func_uint64_t_u_u((safe_rshift_func_int8_t_s_s((*l_30), 2)), ((0x36E903AD >= (*l_30)) , (*p_14))))) & (-6)) >= p_17)) , 0x9945) <= g_47);
            }
            (*l_27) = p_13;
        }
        if ((((((p_17 | ((*p_14) ^ (g_22 || p_16))) || 4294967295U) >= p_17) == (*g_21)) , (*p_15)))
        { 
            uint32_t l_58[4][3];
            int8_t l_62 = 0xEB;
            int16_t l_63[4][2] = {{9,0x5F05},{0x5F05,9},{0x5F05,0x5F05},{9,0x5F05}};
            int32_t l_64 = 0xC8A5E720;
            int32_t l_65 = 0xF583EF4C;
            int32_t l_75 = (-9);
            int64_t * const l_76 = (void*)0;
            int32_t *l_82[4][1] = {{&l_65},{&g_47},{&l_65},{&g_47}};
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 3; j++)
                    l_58[i][j] = 5U;
            }
            for (g_31 = (-8); (g_31 >= (-5)); g_31 = safe_add_func_uint8_t_u_u(g_31, 2))
            { 
                int32_t *l_59 = &g_47;
                (*l_59) = ((((safe_add_func_int8_t_s_s(g_47, g_9)) , (safe_add_func_int32_t_s_s((((l_58[0][1] , g_20) < g_39) ^ l_58[0][1]), g_20))) , (**l_27)) == 4);
            }
            if (((safe_rshift_func_int32_t_s_s((0xFB42DE3FC80C5262LL <= (g_9 = (((l_62 , l_58[0][1]) & g_9) & 65535U))), l_63[2][0])) && 0))
            { 
                uint32_t l_66 = 3U;
                uint64_t *l_74 = &g_9;
                ++l_66;
                if ((*l_30))
                    break;
                g_47 = (((safe_sub_func_uint64_t_u_u(l_71, ((*l_74) = (safe_sub_func_int64_t_s_s((p_15 == (void*)0), (*p_14)))))) != 0x1226129DC0630FC3LL) && 0xC3AC);
            }
            else
            { 
                uint8_t *l_78 = &g_79;
                uint8_t *l_80 = &g_81;
                (*l_30) = (((*l_80) |= (((*l_78) |= (((l_75 , &g_21) != ((l_76 != (void*)0) , &g_21)) < l_77)) , 0x90)) ^ 9);
            }
            g_85[2]--;
        }
        else
        { 
            uint16_t l_90[4];
            int32_t l_93[1][1];
            int i, j;
            for (i = 0; i < 4; i++)
                l_90[i] = 0x738C;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 1; j++)
                    l_93[i][j] = 0;
            }
            for (g_31 = 27; (g_31 != (-25)); g_31 = safe_sub_func_uint64_t_u_u(g_31, 2))
            { 
                l_90[2] = (*g_21);
                l_93[0][0] ^= ((g_91[2] , g_31) >= ((l_92 , 253U) > p_17));
                return &g_47;
            }
            (*l_30) = ((l_93[0][0] && (l_93[0][0] = ((((safe_div_func_uint16_t_u_u(((g_21 = p_13) != ((safe_lshift_func_int16_t_s_s(p_17, 6)) , (void*)0)), g_81)) , p_16) , (**l_27)) , (*p_14)))) < (*p_15));
        }
    }
    return &g_47;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_79, "g_79", print_hash_value);
    transparent_crc(g_81, "g_81", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_85[i], "g_85[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_91[i].f0, "g_91[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_99, "g_99", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_102[i], "g_102[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
