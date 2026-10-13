/* Csmith 0cdc710315cf seed 208, flattened by scripts/csmith_flatten.py */
// Options:   --seed 208 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
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
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 1;
static int32_t g_21 = (-1);
static uint16_t g_31 = 65530U;
static int32_t g_47 = 0x1C19CD89;
static int8_t g_48[3] = {1,1,1};
static int64_t g_50 = 0xC9AA6F576105F61CLL;
static int16_t g_51[1] = {1};
static int64_t g_52[3][4] = {{0xA9D45C545F0A8622LL,0xA9D45C545F0A8622LL,6,0xA9D45C545F0A8622LL},{0xA9D45C545F0A8622LL,0xE0BBE0E206F6EC37LL,0xE0BBE0E206F6EC37LL,0xA9D45C545F0A8622LL},{0xE0BBE0E206F6EC37LL,0xA9D45C545F0A8622LL,0xE0BBE0E206F6EC37LL,0xE0BBE0E206F6EC37LL}};
static int16_t g_53[4][2] = {{0,0},{0,0},{0,0},{0,0}};
static uint64_t g_55 = 18446744073709551608U;
static uint64_t g_64 = 0xAF535935178915D4LL;
static uint16_t g_70 = 0U;
static int32_t g_74 = 0x90BC2142;
static int16_t g_76 = 0;
static uint64_t g_77 = 4U;
static uint64_t *g_91 = &g_64;

static uint32_t  func_1(void);
static uint16_t  func_13(uint32_t  p_14, const uint16_t  p_15, const int64_t  p_16);
static const int32_t  func_23(uint64_t  p_24, int32_t * p_25, uint16_t  p_26, int32_t * p_27, int32_t * p_28);

static uint32_t  func_1(void)
{ 
    uint32_t l_18[4][2];
    int32_t l_22 = 0x66B17B72;
    int16_t l_29 = 0x8F92;
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
            l_18[i][j] = 0xA7084BC9;
    }
    for (g_2 = 0; (g_2 < 6); ++g_2)
    { 
        const uint8_t l_17 = 0x16;
        if (g_2)
        { 
            if ((safe_sub_func_int16_t_s_s((safe_sub_func_uint16_t_u_u((g_2 < (safe_lshift_func_int32_t_s_s(g_2, (safe_div_func_uint16_t_u_u(func_13(g_2, g_2, l_17), g_2))))), 0)), l_18[2][1])))
            { 
                int32_t *l_19 = (void*)0;
                int32_t *l_20[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_20[i] = &g_21;
                l_22 = g_2;
            }
            else
            { 
                uint16_t *l_30[1];
                int32_t *l_32 = &g_21;
                int i;
                for (i = 0; i < 1; i++)
                    l_30[i] = &g_31;
                (*l_32) = func_23(l_29, &g_21, (g_31 = ((l_17 && g_2) , 0xC77D)), l_32, &g_2);
            }
        }
        else
        { 
            int32_t *l_99 = &g_21;
            (*l_99) = g_52[2][2];
            return g_52[2][2];
        }
    }
    return g_76;
}

static uint16_t  func_13(uint32_t  p_14, const uint16_t  p_15, const int64_t  p_16)
{ 
    return g_2;
}

static const int32_t  func_23(uint64_t  p_24, int32_t * p_25, uint16_t  p_26, int32_t * p_27, int32_t * p_28)
{ 
    int32_t l_35 = 0xB30477AE;
    int32_t l_54 = 0x94A22835;
    int32_t *l_73 = &g_21;
    uint64_t *l_92 = &g_55;
    if ((*p_28))
    { 
        int32_t **l_33 = (void*)0;
        int32_t *l_34 = (void*)0;
        l_34 = p_28;
        (*p_25) = (-1);
    }
    else
    { 
        int16_t l_42[1];
        int32_t l_49[1][1];
        uint64_t *l_63 = &g_64;
        int32_t **l_69 = (void*)0;
        int i, j;
        for (i = 0; i < 1; i++)
            l_42[i] = 0x7720;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_49[i][j] = 0;
        }
        (*p_25) ^= ((l_35 != ((safe_mul_func_int8_t_s_s((safe_rshift_func_uint16_t_u_s((safe_mod_func_int16_t_s_s((&g_21 == (void*)0), 1U)), l_42[0])), 0x1E)) != g_31)) == l_42[0]);
        if ((*p_27))
        { 
            int32_t *l_43 = &g_21;
            int32_t *l_44 = &g_21;
            int32_t *l_45 = &g_21;
            int32_t *l_46[3][3] = {{&g_21,&l_35,&g_21},{&g_21,&l_35,&g_21},{&g_21,&l_35,&g_21}};
            uint64_t *l_58[1][2];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_58[i][j] = &g_55;
            }
            ++g_55;
            if (((g_55--) >= ((((safe_rshift_func_uint32_t_u_u((g_31 != (g_31 , ((void*)0 == l_63))), g_48[2])) , g_21) & p_26) && (-6))))
            { 
                uint16_t l_68 = 7U;
                l_49[0][0] = (((safe_div_func_uint64_t_u_u((!(18446744073709551615U || (((p_26 > ((*p_25) &= l_68)) , 0xC677CCB2DC50754BLL) , l_54))), p_24)) , l_69) != &l_46[1][1]);
                ++g_70;
            }
            else
            { 
                int64_t l_75 = 0;
                l_73 = (void*)0;
                g_77++;
                g_47 |= (!(safe_lshift_func_int32_t_s_u(0xC059577E, 7)));
            }
        }
        else
        { 
            return g_52[1][3];
        }
    }
    for (g_77 = 0; (g_77 <= 0); g_77 += 1)
    { 
        int16_t *l_83 = &g_53[3][0];
        int32_t l_84 = (-7);
        int i;
        l_84 = ((g_51[g_77] , g_51[g_77]) <= ((*l_83) = (((g_21 && 2U) != g_51[g_77]) == 0xDA)));
        for (g_70 = 0; (g_70 <= 2); g_70 += 1)
        { 
            uint64_t *l_89[3];
            uint16_t *l_95 = &g_31;
            int i;
            for (i = 0; i < 3; i++)
                l_89[i] = &g_64;
            for (l_35 = 0; (l_35 <= 2); l_35 += 1)
            { 
                int32_t *l_87 = (void*)0;
                int32_t *l_88[4][3] = {{&l_54,&g_47,&l_54},{&l_54,&g_47,&l_54},{&l_54,&g_47,&l_54},{&l_54,&g_47,&l_54}};
                uint64_t **l_90[3][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
                int i, j;
                l_54 = ((safe_div_func_int32_t_s_s((g_74 = ((*p_25) = g_53[g_77][(g_77 + 1)])), 4294967290U)) || ((g_53[(g_77 + 3)][g_77] = g_52[(g_77 + 1)][(l_35 + 1)]) != g_51[g_77]));
                (*p_27) &= ((g_91 = l_89[1]) != l_92);
            }
            (*p_27) = (((((*l_95) = 1U) ^ (((*p_27) > (!(g_76 , g_50))) == (*g_91))) ^ 1) >= 7);
        }
    }
    return (*p_27);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_48[i], "g_48[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_50, "g_50", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_51[i], "g_51[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_52[i][j], "g_52[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_53[i][j], "g_53[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_76, "g_76", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
