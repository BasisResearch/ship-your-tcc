/* Csmith 0cdc710315cf seed 141, flattened by scripts/csmith_flatten.py */
// Options:   --seed 141 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
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
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint64_t  f0;
   int64_t  f1;
};

union U1 {
   const int64_t  f0;
};

static uint64_t g_12 = 0x64896E6A3735B263LL;
static int32_t g_14 = 1;
static int32_t *g_13[3][4] = {{&g_14,&g_14,&g_14,&g_14},{&g_14,&g_14,&g_14,&g_14},{&g_14,&g_14,&g_14,&g_14}};
static const union U1 g_23 = {6};
static int16_t g_29 = (-3);
static uint16_t g_31 = 3U;
static int32_t g_47 = (-1);
static int64_t g_55 = (-10);
static int32_t *g_79 = &g_47;
static int32_t **g_78 = &g_79;
static union U0 g_84 = {5U};

static uint16_t  func_1(void);
static int32_t * func_2(uint8_t  p_3, uint8_t  p_4, int32_t * p_5, int32_t * p_6, int32_t * p_7);
static uint16_t  func_21(const union U1  p_22);

static uint16_t  func_1(void)
{ 
    int32_t *l_15 = &g_14;
    int32_t *l_16 = &g_14;
    int32_t **l_90 = &g_13[1][1];
    int32_t *l_91 = &g_14;
    int32_t *l_92[2];
    int32_t l_93 = 0x6CB076EB;
    uint8_t l_94 = 254U;
    int i;
    for (i = 0; i < 2; i++)
        l_92[i] = (void*)0;
    (*l_90) = func_2((safe_sub_func_uint64_t_u_u((safe_mod_func_int64_t_s_s(0x4210E164CAF14C90LL, ((0x71DFD947 || 0x7A61EEFB) & g_12))), g_12)), g_12, g_13[0][1], l_15, l_16);
    ++l_94;
    return g_31;
}

static int32_t * func_2(uint8_t  p_3, uint8_t  p_4, int32_t * p_5, int32_t * p_6, int32_t * p_7)
{ 
    uint64_t l_68 = 0x985D36CA66A39569LL;
    int32_t *l_74 = &g_47;
    const int8_t l_85 = 0x39;
    union U0 *l_89 = &g_84;
    for (p_4 = 0; (p_4 != 5); ++p_4)
    { 
        uint32_t l_56 = 0x103D800C;
        int32_t l_58 = (-1);
        uint8_t l_75 = 0xB4;
        if ((safe_lshift_func_uint16_t_u_s(func_21(g_23), (+p_4))))
        { 
            for (g_12 = 0; (g_12 <= 8); g_12 = safe_add_func_int32_t_s_s(g_12, 7))
            { 
                (*p_6) ^= g_55;
            }
        }
        else
        { 
            int32_t *l_57[4][1];
            uint16_t *l_65 = &g_31;
            int64_t l_66[1];
            int32_t **l_73[2][3];
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 1; j++)
                    l_57[i][j] = (void*)0;
            }
            for (i = 0; i < 1; i++)
                l_66[i] = 0xC3AD06F0504CB07ELL;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 3; j++)
                    l_73[i][j] = &l_57[0][0];
            }
            l_58 |= ((*p_6) = (l_56 >= g_14));
            if ((p_4 <= (p_4 ^ ((*l_65) = (safe_div_func_uint32_t_u_u((safe_sub_func_int16_t_s_s((safe_mul_func_uint32_t_u_u((p_7 == &l_58), 0x7BCF5413)), 0x64E3)), 1U))))))
            { 
                if (l_66[0])
                    break;
                return p_7;
            }
            else
            { 
                int16_t l_67 = 0xFD25;
                --l_68;
                if ((*p_7))
                    break;
            }
            if ((g_55 & ((g_13[0][1] = (((((safe_rshift_func_uint8_t_u_u(((l_74 = l_57[1][0]) != p_7), p_4)) || g_47) || 0) != 0xF9829FEAD6BFE3AELL) , &g_14)) != &l_58)))
            { 
                return l_74;
            }
            else
            { 
                p_6 = &g_14;
                --l_75;
                g_78 = &l_74;
            }
        }
        l_58 = (safe_lshift_func_int16_t_s_u((((*g_78) == (*g_78)) , (((safe_add_func_int8_t_s_s((g_84 , g_12), l_85)) && g_29) >= 0x14C84822)), 12));
    }
    (*p_7) = (*p_5);
    for (g_47 = 0; (g_47 <= 2); g_47 += 1)
    { 
        int8_t l_86 = 7;
        (*g_78) = (*g_78);
        l_86 = ((*p_7) = (*p_6));
        for (p_3 = 0; (p_3 <= 2); p_3 += 1)
        { 
            union U0 *l_87 = &g_84;
            union U0 **l_88[4][3] = {{&l_87,&l_87,&l_87},{&l_87,&l_87,&l_87},{&l_87,&l_87,&l_87},{&l_87,&l_87,&l_87}};
            int i, j;
            l_89 = l_87;
        }
    }
    return l_74;
}

static uint16_t  func_21(const union U1  p_22)
{ 
    int32_t l_32[4][2];
    int32_t *l_35 = &g_14;
    int8_t l_40 = 1;
    int32_t **l_51 = &l_35;
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
            l_32[i][j] = 0x488D514F;
    }
    for (g_12 = 0; (g_12 <= 2); g_12 += 1)
    { 
        uint32_t l_26 = 0xFBC43CC9;
        int32_t l_36 = 0x681C37F5;
        int32_t l_37 = 0x3E27C213;
        int32_t l_38 = 1;
        int32_t l_39[3][1];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_39[i][j] = 5;
        }
        if ((safe_sub_func_uint64_t_u_u(0x0C11E043EEF7D266LL, (l_26 | (safe_rshift_func_uint8_t_u_u(p_22.f0, 4))))))
        { 
            uint16_t *l_30[3];
            int32_t *l_33 = &g_14;
            int i;
            for (i = 0; i < 3; i++)
                l_30[i] = &g_31;
            (*l_33) |= ((g_29 = g_23.f0) | (l_32[1][0] = p_22.f0));
        }
        else
        { 
            int32_t **l_34[4][1];
            uint16_t l_41[3];
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 1; j++)
                    l_34[i][j] = &g_13[0][1];
            }
            for (i = 0; i < 3; i++)
                l_41[i] = 0xA4A7;
            l_35 = &g_14;
            l_41[2]++;
        }
        for (l_36 = 2; (l_36 >= 0); l_36 -= 1)
        { 
            int32_t l_44[1][2];
            int32_t *l_45 = &l_32[1][0];
            int32_t *l_46[4][2];
            uint64_t l_48[4][2] = {{0xC2F892C2D3C6BE40LL,0x5F8A0FD8B3473E55LL},{0x5F8A0FD8B3473E55LL,0xC2F892C2D3C6BE40LL},{0x5F8A0FD8B3473E55LL,0x5F8A0FD8B3473E55LL},{0xC2F892C2D3C6BE40LL,0x5F8A0FD8B3473E55LL}};
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_44[i][j] = 0x23EAFAF4;
            }
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 2; j++)
                    l_46[i][j] = &l_38;
            }
            (*l_35) = l_44[0][0];
            l_48[1][0]++;
            l_51 = &l_46[0][1];
        }
    }
    return g_12;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_12, "g_12", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_23.f0, "g_23.f0", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_84.f0, "g_84.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
