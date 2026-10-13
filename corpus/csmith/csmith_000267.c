/* Csmith 0cdc710315cf seed 267, flattened by scripts/csmith_flatten.py */
// Options:   --seed 267 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
   uint8_t  f1;
   uint64_t  f2;
};

struct S2 {
   uint64_t  f0;
   int8_t  f1;
   const int16_t  f2;
   const uint64_t  f3;
};

struct S3 {
   int8_t  f0;
   int16_t  f1;
   const uint32_t  f2;
};

struct S4 {
   int64_t  f0;
   const int32_t  f1;
   const int32_t  f2;
   int64_t  f3;
};

static int32_t g_3 = 0xDDCD1665;
static uint64_t g_14 = 1U;
static int32_t g_28[2] = {0x051AAF19,0x051AAF19};
static int32_t g_29 = (-1);
static int32_t g_30 = (-7);
static int16_t g_44 = 0x89A0;
static uint32_t g_45 = 0xD97811D0;
static struct S2 g_48[4][4] = {{{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x0CE47CEE57CEC448LL,1,9,1U},{0x0CE47CEE57CEC448LL,1,9,1U}},{{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x0CE47CEE57CEC448LL,1,9,1U},{0x0CE47CEE57CEC448LL,1,9,1U}},{{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x0CE47CEE57CEC448LL,1,9,1U},{0x0CE47CEE57CEC448LL,1,9,1U}},{{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x6C8CEE692D353AE7LL,0x2F,1,0x72F71451CEE08268LL},{0x0CE47CEE57CEC448LL,1,9,1U},{0x0CE47CEE57CEC448LL,1,9,1U}}};
static struct S4 g_50 = {1,0xC190CC4B,0xB4C8B43F,0x205A1655ED6FA3C4LL};
static struct S4 *g_49 = &g_50;
static struct S0 g_70 = {0xB7,0U,0xCBA3753CE469C6BCLL};
static struct S0 *g_69[1][4] = {{(void*)0,(void*)0,(void*)0,(void*)0}};
static struct S0 g_77 = {0x75,254U,0xA934B7DDD9CFB06ALL};
static struct S3 g_82 = {0,9,1U};
static struct S3 *g_81 = &g_82;
static uint8_t g_100[1] = {0x96};

static struct S2  func_1(void);
static struct S0  func_19(uint8_t  p_20);
static struct S2  func_21(int32_t * p_22, const uint64_t  p_23);

static struct S2  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t *l_4 = &g_3;
    int32_t *l_5 = &g_3;
    int32_t *l_6 = &g_3;
    int32_t *l_7 = &g_3;
    int32_t l_8 = 0xF609B2B5;
    int32_t l_9[2];
    int32_t *l_10 = &l_9[0];
    int32_t *l_11 = &g_3;
    int32_t *l_12 = &l_9[0];
    int32_t *l_13[4] = {&l_9[0],&l_9[0],&l_9[0],&l_9[0]};
    int16_t l_98 = 0xFB69;
    int8_t l_99 = 0xAF;
    int i;
    for (i = 0; i < 2; i++)
        l_9[i] = 0x885EA6ED;
    g_14++;
    for (g_3 = 0; (g_3 == 8); g_3 = safe_add_func_uint64_t_u_u(g_3, 1))
    { 
        uint64_t l_24 = 0x0D790F95848308FALL;
        struct S0 *l_75 = &g_70;
        struct S0 *l_76 = &g_77;
        (*l_76) = ((*l_75) = func_19((func_21(&g_3, (((((((0xC6 > 6) | g_3) && 0xD945AD33D2AFDDF3LL) | l_24) || l_24) != 0x8BA7) , g_14)) , g_44)));
        g_28[0] = (safe_rshift_func_uint16_t_u_s((~g_28[0]), 6));
    }
    if (g_45)
    { 
        uint16_t l_85 = 0xABF7;
        if (g_44)
        { 
            struct S3 **l_83 = (void*)0;
            struct S3 **l_84[2];
            int i;
            for (i = 0; i < 2; i++)
                l_84[i] = &g_81;
            g_81 = g_81;
            l_85++;
        }
        else
        { 
            struct S2 l_90[1][1] = {{{5U,0,1,18446744073709551615U}}};
            int32_t l_91[2][3] = {{(-9),(-9),(-9)},{8,8,8}};
            int i, j;
            for (l_85 = 0; (l_85 > 54); l_85 = safe_add_func_int16_t_s_s(l_85, 2))
            { 
                return l_90[0][0];
            }
            l_91[1][0] = l_85;
        }
    }
    else
    { 
        int64_t l_92[4][1];
        int32_t l_93 = 0x26141094;
        int32_t l_94 = 0xC9291112;
        int32_t l_95 = 0xCC5544F0;
        int32_t l_96[4] = {0,0,0,0};
        int8_t l_97 = 1;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_92[i][j] = 8;
        }
        g_100[0]--;
    }
    return g_48[1][0];
}

static struct S0  func_19(uint8_t  p_20)
{ 
    struct S4 **l_51 = &g_49;
    int32_t l_63 = 0;
    int32_t l_64 = (-2);
    int32_t l_65[2][1];
    struct S0 l_74[3] = {{255U,6U,7U},{255U,6U,7U},{255U,6U,7U}};
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_65[i][j] = 0x9BCD9968;
    }
    (*l_51) = g_49;
    for (g_44 = 22; (g_44 <= 14); g_44--)
    { 
        for (g_50.f0 = 1; (g_50.f0 >= 0); g_50.f0 -= 1)
        { 
            uint8_t l_66 = 0x03;
            struct S0 **l_71 = &g_69[0][1];
            for (g_30 = 0; (g_30 <= 1); g_30 += 1)
            { 
                int32_t *l_54 = &g_28[0];
                int32_t *l_55 = &g_28[g_50.f0];
                int32_t *l_56 = &g_29;
                int32_t *l_57 = &g_28[0];
                int32_t *l_58 = &g_29;
                int32_t *l_59 = (void*)0;
                int32_t *l_60 = &g_29;
                int32_t *l_61 = &g_29;
                int32_t *l_62[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_62[i] = &g_28[g_50.f0];
                ++l_66;
            }
            (*l_71) = g_69[0][0];
        }
    }
    for (g_44 = 0; (g_44 <= 1); g_44 += 1)
    { 
        for (g_45 = 0; (g_45 <= 1); g_45 += 1)
        { 
            int i;
            g_30 |= (safe_mul_func_int16_t_s_s((g_28[g_45] == g_50.f2), g_70.f0));
        }
        return l_74[0];
    }
    return l_74[0];
}

static struct S2  func_21(int32_t * p_22, const uint64_t  p_23)
{ 
    uint16_t l_25[1][1];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_25[i][j] = 0xA9A1;
    }
    for (g_14 = 0; (g_14 <= 0); g_14 += 1)
    { 
        int32_t *l_27 = &g_3;
        int32_t **l_26 = &l_27;
        const int32_t *l_32 = &g_30;
        const int32_t **l_31 = &l_32;
        int8_t l_35 = 0xFA;
        int32_t l_43[2];
        int i;
        for (i = 0; i < 2; i++)
            l_43[i] = (-3);
        (*l_26) = &g_3;
        for (g_28[0] = 0; (g_28[0] <= 0); g_28[0] += 1)
        { 
            const int32_t **l_33[1][4] = {{&l_32,&l_32,&l_32,&l_32}};
            const int32_t **l_34 = &l_32;
            int32_t *l_36 = (void*)0;
            int32_t *l_37 = (void*)0;
            int32_t *l_38 = &g_30;
            int32_t *l_39 = &g_30;
            int32_t *l_40 = &g_29;
            int32_t *l_41 = &g_29;
            int32_t *l_42[1][4] = {{(void*)0,(void*)0,(void*)0,(void*)0}};
            int i, j;
            for (g_29 = 0; g_29 < 1; g_29 = g_29 + 1)
            {
                for (g_30 = 0; g_30 < 1; g_30 = g_30 + 1)
                {
                    l_25[g_29][g_30] = 65530U;
                }
            }
            l_34 = (l_25[g_14][g_14] , (l_33[0][1] = l_31));
            ++g_45;
        }
    }
    return g_48[1][1];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_28[i], "g_28[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_48[i][j].f0, "g_48[i][j].f0", print_hash_value);
            transparent_crc(g_48[i][j].f1, "g_48[i][j].f1", print_hash_value);
            transparent_crc(g_48[i][j].f2, "g_48[i][j].f2", print_hash_value);
            transparent_crc(g_48[i][j].f3, "g_48[i][j].f3", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_50.f0, "g_50.f0", print_hash_value);
    transparent_crc(g_50.f1, "g_50.f1", print_hash_value);
    transparent_crc(g_50.f2, "g_50.f2", print_hash_value);
    transparent_crc(g_50.f3, "g_50.f3", print_hash_value);
    transparent_crc(g_70.f0, "g_70.f0", print_hash_value);
    transparent_crc(g_70.f1, "g_70.f1", print_hash_value);
    transparent_crc(g_70.f2, "g_70.f2", print_hash_value);
    transparent_crc(g_77.f0, "g_77.f0", print_hash_value);
    transparent_crc(g_77.f1, "g_77.f1", print_hash_value);
    transparent_crc(g_77.f2, "g_77.f2", print_hash_value);
    transparent_crc(g_82.f0, "g_82.f0", print_hash_value);
    transparent_crc(g_82.f1, "g_82.f1", print_hash_value);
    transparent_crc(g_82.f2, "g_82.f2", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_100[i], "g_100[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
