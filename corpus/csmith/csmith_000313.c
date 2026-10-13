/* Csmith 0cdc710315cf seed 313, flattened by scripts/csmith_flatten.py */
// Options:   --seed 313 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const int64_t  f0;
   int64_t  f1;
};

union U1 {
   int16_t  f0;
   int32_t  f1;
};

union U2 {
   uint32_t  f0;
   int32_t  f1;
};

static int64_t g_2[4] = {0x72D9EE1C97412DF3LL,0x72D9EE1C97412DF3LL,0x72D9EE1C97412DF3LL,0x72D9EE1C97412DF3LL};
static int32_t g_5 = 0x310C13FE;
static union U2 g_21 = {0x78EF8A6E};
static int8_t g_23 = 0x11;
static uint32_t g_29 = 4294967295U;
static int64_t g_35 = 0x97140DA8F9E1C210LL;
static int32_t *g_36 = &g_5;
static int32_t *g_56 = (void*)0;
static uint64_t g_65 = 0x18944B8798E375BCLL;
static union U1 g_73 = {0};
static int32_t g_85 = (-6);
static int8_t g_86 = (-1);
static int8_t g_87 = 1;
static int8_t g_89 = 1;
static int16_t g_90 = 1;
static int32_t g_91 = 0x2B5D7664;
static uint64_t g_92[2][3] = {{0U,0U,0U},{0U,0U,0U}};
static struct S0 g_97[3] = {{-1,0x3DB0A16214FB9B3DLL},{-1,0x3DB0A16214FB9B3DLL},{-1,0x3DB0A16214FB9B3DLL}};
static uint16_t g_101 = 65527U;
static uint32_t g_107 = 4294967295U;

static struct S0  func_1(void);
static int32_t * func_6(uint8_t  p_7);
static struct S0  func_17(union U2  p_18, int32_t * p_19, int32_t * p_20);

static struct S0  func_1(void)
{ 
    uint8_t l_3 = 255U;
    int32_t *l_4 = &g_5;
    const struct S0 l_51 = {0,0xBC4D5916FF7DD1A1LL};
    int8_t *l_53 = &g_23;
    int32_t l_88[1][4] = {{(-9),(-9),(-9),(-9)}};
    int i, j;
    if ((((*l_4) = (g_2[1] , l_3)) < 0x2DB8E964))
    { 
        int32_t **l_37 = (void*)0;
        int32_t **l_38 = (void*)0;
        int32_t *l_39 = (void*)0;
        l_39 = (g_36 = func_6(g_2[0]));
    }
    else
    { 
        uint64_t l_44[3];
        int64_t *l_52 = &g_2[1];
        int32_t *l_54 = &g_5;
        uint8_t l_96[1];
        int32_t l_104 = 0xAB0DC940;
        int32_t l_105 = (-5);
        int32_t l_106 = 0;
        int i;
        for (i = 0; i < 3; i++)
            l_44[i] = 0x3ECB6AEF2D6A391DLL;
        for (i = 0; i < 1; i++)
            l_96[i] = 0xA2;
        if (((*l_4) < (((safe_rshift_func_uint16_t_u_s(l_44[2], (safe_mul_func_uint64_t_u_u(((((*l_52) = (safe_div_func_uint8_t_u_u(((safe_sub_func_uint64_t_u_u((l_51 , 0U), 18446744073709551615U)) || l_44[2]), g_35))) , l_53) != (void*)0), 0x8F4439D3FD5B128CLL)))) <= g_5) <= 18446744073709551615U)))
        { 
            int32_t **l_55[2];
            int i;
            for (i = 0; i < 2; i++)
                l_55[i] = &l_4;
            (*g_36) = (*l_4);
            g_56 = l_54;
        }
        else
        { 
            int64_t l_60 = (-1);
            for (g_29 = 0; (g_29 < 48); g_29 = safe_add_func_int8_t_s_s(g_29, 2))
            { 
                int64_t l_59 = 0x18A8DC8D62BAD2CCLL;
                int32_t *l_61 = &g_5;
                int32_t *l_62 = &g_5;
                int32_t *l_63 = &g_5;
                int32_t *l_64[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_64[i] = &g_21.f1;
                (*l_4) ^= (l_59 == (g_29 == l_60));
                g_65++;
            }
            return l_51;
        }
lbl_95:
        for (g_35 = 18; (g_35 <= (-10)); --g_35)
        { 
            int32_t l_70 = (-3);
            (*g_36) = 0x3B6624E9;
            if ((*g_36))
            { 
                if (l_70)
                    break;
            }
            else
            { 
                union U1 *l_72 = &g_73;
                union U1 **l_71 = &l_72;
                (*l_4) = (l_71 != &l_72);
                (*l_4) = (*g_36);
            }
        }
        for (g_73.f0 = 0; (g_73.f0 <= 3); g_73.f0 += 1)
        { 
            int64_t *l_74 = &g_2[3];
            uint64_t l_75[2][1];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_75[i][j] = 0U;
            }
            if ((*g_36))
            { 
                if ((*l_54))
                    break;
                l_75[0][0] = ((*l_54) <= (g_5 == (((g_21.f0 > g_65) , l_74) != &g_2[3])));
            }
            else
            { 
                int32_t *l_76 = &g_5;
                int32_t *l_77 = &g_5;
                int32_t *l_78 = &g_5;
                int32_t *l_79 = &g_21.f1;
                int32_t *l_80 = (void*)0;
                int64_t l_81 = 6;
                int32_t *l_82 = (void*)0;
                int32_t *l_83 = &g_5;
                int32_t *l_84[2][4];
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 4; j++)
                        l_84[i][j] = &g_5;
                }
                --g_92[1][2];
            }
            for (g_87 = 0; (g_87 <= 3); g_87 += 1)
            { 
                if (g_29)
                    goto lbl_95;
                if (l_96[0])
                    break;
                return g_97[0];
            }
            for (g_21.f0 = 0; (g_21.f0 <= 3); g_21.f0 += 1)
            { 
                return g_97[2];
            }
            for (g_85 = 0; (g_85 <= 3); g_85 += 1)
            { 
                int32_t *l_98 = (void*)0;
                int32_t *l_99 = (void*)0;
                int32_t *l_100[3][4] = {{&g_5,&l_88[0][1],&l_88[0][1],&g_5},{&l_88[0][1],&g_5,&l_88[0][1],&l_88[0][1]},{&g_5,&g_5,&g_91,&g_5}};
                int i, j;
                g_101--;
                if ((*g_36))
                    break;
                g_107--;
            }
        }
    }
    (*g_36) ^= 0;
    return g_97[0];
}

static int32_t * func_6(uint8_t  p_7)
{ 
    int32_t *l_8[1][3];
    uint8_t l_9 = 0x08;
    uint32_t l_33 = 1U;
    int64_t *l_34 = &g_2[0];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_8[i][j] = &g_5;
    }
    l_9 = g_5;
    for (p_7 = 0; (p_7 == 34); p_7++)
    { 
        uint64_t l_12 = 5U;
        g_5 = l_12;
    }
    g_35 &= (((*l_34) = (safe_mul_func_uint64_t_u_u(((safe_lshift_func_int8_t_s_s((((func_17(g_21, l_8[0][2], &g_5) , l_33) , l_8[0][2]) == (void*)0), 4)) , g_2[3]), p_7))) , 1);
    return l_8[0][2];
}

static struct S0  func_17(union U2  p_18, int32_t * p_19, int32_t * p_20)
{ 
    union U1 *l_24 = (void*)0;
    int32_t l_26 = 0;
    int32_t *l_28[2][3];
    struct S0 l_32[4] = {{6,0xA2C160EC253D5AB8LL},{6,0xA2C160EC253D5AB8LL},{6,0xA2C160EC253D5AB8LL},{6,0xA2C160EC253D5AB8LL}};
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
            l_28[i][j] = &g_21.f1;
    }
    for (p_18.f1 = 3; (p_18.f1 >= 0); p_18.f1 -= 1)
    { 
        int8_t *l_22[3];
        struct S0 l_27 = {-5,0xDBC75B167D3D8E1CLL};
        int i;
        for (i = 0; i < 3; i++)
            l_22[i] = &g_23;
        if (g_2[p_18.f1])
            break;
        if (((1 == (g_23 = g_5)) && 9))
        { 
            union U1 **l_25 = &l_24;
            (*l_25) = l_24;
            (*p_19) = ((g_2[0] && (p_18.f1 & l_26)) > (-7));
        }
        else
        { 
            if (g_23)
                break;
            return l_27;
        }
    }
    ++g_29;
    return l_32[1];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_21.f0, "g_21.f0", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_73.f0, "g_73.f0", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_86, "g_86", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    transparent_crc(g_90, "g_90", print_hash_value);
    transparent_crc(g_91, "g_91", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_92[i][j], "g_92[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_97[i].f0, "g_97[i].f0", print_hash_value);
        transparent_crc(g_97[i].f1, "g_97[i].f1", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_101, "g_101", print_hash_value);
    transparent_crc(g_107, "g_107", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
