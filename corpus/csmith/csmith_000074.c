/* Csmith 0cdc710315cf seed 74, flattened by scripts/csmith_flatten.py */
// Options:   --seed 74 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   uint64_t  f1;
};

struct S1 {
   struct S0  f0;
   int64_t  f1;
};

struct S2 {
   struct S0  f0;
   const uint32_t  f1;
   int16_t  f2;
   struct S0  f3;
};

union U3 {
   struct S1  f0;
   const uint32_t  f1;
   struct S2  f2;
};

union U4 {
   int32_t  f0;
};

static int32_t g_2 = 0xCE3E63E3;
static struct S0 g_8 = {0xFE012D2C87A7525DLL,0xD91509CF13E13A6CLL};
static int8_t g_13 = 0x71;
static int64_t g_26 = 0x947A332A5B4D633CLL;
static int32_t g_27 = 0x99991913;
static uint64_t g_28 = 18446744073709551608U;
static int8_t g_39 = 0xB0;
static uint16_t g_43 = 0U;
static struct S1 g_54 = {{-1,18446744073709551615U},1};
static int8_t g_55 = 1;
static uint8_t g_57 = 0x6C;
static union U3 g_62 = {{{9,0x8811FBA6500C2479LL},0}};
static struct S2 g_66 = {{0xB0D2C86D4C770407LL,0x5320B35339FE21ACLL},1U,-1,{0xA8502006303C50B1LL,0xB9BD427A37A7FFAELL}};
static union U4 g_72 = {0xC4196DCF};
static int32_t *g_79 = (void*)0;
static uint8_t g_85 = 252U;
static uint64_t g_94[4] = {0x6D21B0195B865CE0LL,0x6D21B0195B865CE0LL,0x6D21B0195B865CE0LL,0x6D21B0195B865CE0LL};

static uint32_t  func_1(void);
static int8_t  func_5(struct S0  p_6, uint32_t  p_7);
static const struct S1  func_17(union U3  p_18, uint8_t  p_19, int32_t * const  p_20, int32_t * p_21, const int64_t  p_22);

static uint32_t  func_1(void)
{ 
    int8_t l_15[1][3];
    struct S1 *l_73[1];
    struct S0 l_82 = {5,0U};
    int32_t l_83 = 0xCAF053B5;
    int32_t l_84[2][4] = {{0x5BB6F065,0x5BB6F065,0x5BB6F065,0x5BB6F065},{0x5BB6F065,0x5BB6F065,0x5BB6F065,0x5BB6F065}};
    uint16_t l_88 = 65527U;
    int32_t *l_97 = &g_72.f0;
    struct S1 l_98[1][3] = {{{{1,18446744073709551615U},0xCA65C328A76B0EF2LL},{{1,18446744073709551615U},0xCA65C328A76B0EF2LL},{{1,18446744073709551615U},0xCA65C328A76B0EF2LL}}};
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_15[i][j] = 0x00;
    }
    for (i = 0; i < 1; i++)
        l_73[i] = &g_54;
    for (g_2 = (-9); (g_2 >= (-21)); g_2 = safe_sub_func_int16_t_s_s(g_2, 1))
    { 
        int16_t l_11 = 0x1846;
        int8_t *l_12[2];
        int32_t l_14 = 0x147F59EC;
        int32_t *l_69 = &l_14;
        union U4 l_76 = {0xA2A43FB2};
        struct S0 l_80 = {-4,2U};
        int32_t *l_91 = (void*)0;
        int32_t *l_92 = (void*)0;
        int32_t *l_93[3][4] = {{(void*)0,(void*)0,&g_27,&g_27},{&g_2,&g_2,(void*)0,&g_27},{&l_84[0][0],(void*)0,&l_84[0][0],(void*)0}};
        int i, j;
        for (i = 0; i < 2; i++)
            l_12[i] = &g_13;
        (*l_69) = (func_5(g_8, (safe_mul_func_int8_t_s_s(0x2E, (((l_14 = (((g_2 == g_8.f0) >= g_8.f0) || l_11)) == l_15[0][2]) && (-2))))) <= 0xCA);
        if (l_15[0][2])
        { 
            struct S1 **l_74 = &l_73[0];
            int32_t l_75 = (-1);
            for (g_62.f0.f0.f0 = 0; (g_62.f0.f0.f0 >= 0); g_62.f0.f0.f0 -= 1)
            { 
                union U4 *l_71[2];
                union U4 **l_70 = &l_71[1];
                int i;
                for (i = 0; i < 2; i++)
                    l_71[i] = &g_72;
                (*l_70) = (void*)0;
            }
            (*l_74) = l_73[0];
            if (l_75)
                break;
        }
        else
        { 
            int32_t *l_77[2][3];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 3; j++)
                    l_77[i][j] = (void*)0;
            }
            if ((l_76 , 0x51B451AF))
            { 
                int32_t **l_78 = &l_69;
                struct S0 *l_81[4][2] = {{&g_66.f3,&g_66.f0},{&g_66.f0,&g_66.f3},{&g_66.f0,&g_66.f0},{&g_66.f3,&g_66.f0}};
                int i, j;
                g_79 = ((*l_78) = l_77[1][2]);
                if (g_62.f0.f1)
                    break;
                l_82 = l_80;
            }
            else
            { 
                g_85--;
                if (l_82.f0)
                    break;
                --l_88;
            }
            g_72.f0 ^= ((void*)0 != &g_8);
        }
        ++g_94[3];
    }
    (*l_97) = l_15[0][2];
    g_62.f0 = (l_98[0][0] , func_17(g_62, (*l_97), &l_84[0][0], &g_2, g_2));
    return g_66.f0.f1;
}

static int8_t  func_5(struct S0  p_6, uint32_t  p_7)
{ 
    int64_t *l_16 = &g_8.f0;
    union U3 l_23[3] = {{{{8,0x8C5CECCE005AB136LL},0}},{{{8,0x8C5CECCE005AB136LL},0}},{{{8,0x8C5CECCE005AB136LL},0}}};
    int32_t *l_24[1][3];
    int32_t **l_68 = &l_24[0][0];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_24[i][j] = (void*)0;
    }
    (*l_68) = ((((*l_16) ^= (&g_13 == &g_13)) | ((func_17(l_23[0], g_2, &g_2, l_24[0][0], g_13) , 0xD6BEAEC5) != 4294967295U)) , (void*)0);
    return g_62.f0.f0.f1;
}

static const struct S1  func_17(union U3  p_18, uint8_t  p_19, int32_t * const  p_20, int32_t * p_21, const int64_t  p_22)
{ 
    int32_t *l_25[3][2];
    uint32_t l_38 = 0x171B3FE7;
    uint16_t *l_42 = &g_43;
    uint8_t l_44 = 0x76;
    uint64_t l_45 = 18446744073709551606U;
    union U3 *l_61 = &g_62;
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_25[i][j] = (void*)0;
    }
lbl_48:
    g_28--;
    l_45 = ((safe_div_func_int64_t_s_s(((((*l_42) &= (((g_39 = ((~(safe_lshift_func_uint64_t_u_u((safe_rshift_func_uint64_t_u_s(l_38, 13)), 22))) > 0x75C8)) || (safe_add_func_uint64_t_u_u(g_39, (-2)))) != p_18.f0.f1)) , l_25[0][1]) == &g_27), g_2)) && l_44);
    for (l_38 = 0; (l_38 <= 1); l_38 += 1)
    { 
        uint64_t l_47 = 0x4210AB9CA2468F5BLL;
        int32_t l_49 = 5;
        int32_t l_56 = 6;
        uint64_t l_64 = 0x3E802F2280F24A82LL;
        for (g_39 = 1; (g_39 >= 0); g_39 -= 1)
        { 
            uint8_t l_50 = 0x89;
            int32_t l_53 = 0x4D1B33B7;
            for (g_13 = 1; (g_13 >= 0); g_13 -= 1)
            { 
                int32_t l_46 = 0xF470B9C4;
                int i, j;
                l_47 |= l_46;
            }
            for (g_8.f1 = 0; (g_8.f1 <= 1); g_8.f1 += 1)
            { 
                int i, j;
                if (p_18.f0.f1)
                    goto lbl_48;
                --l_50;
                return p_18.f0;
            }
            for (l_50 = 0; (l_50 <= 1); l_50 += 1)
            { 
                l_53 &= g_28;
            }
        }
        g_54 = g_54;
        --g_57;
        for (l_44 = 0; (l_44 <= 1); l_44 += 1)
        { 
            int64_t *l_60 = &g_26;
            if (((void*)0 == l_60))
            { 
                union U3 **l_63[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_63[i] = &l_61;
                l_61 = l_61;
                if (g_54.f1)
                    goto lbl_48;
            }
            else
            { 
                struct S2 *l_65 = &g_66;
                struct S2 **l_67 = &l_65;
                if (l_64)
                    break;
                (*l_67) = l_65;
            }
        }
    }
    return g_54;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_8.f0, "g_8.f0", print_hash_value);
    transparent_crc(g_8.f1, "g_8.f1", print_hash_value);
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_54.f0.f0, "g_54.f0.f0", print_hash_value);
    transparent_crc(g_54.f0.f1, "g_54.f0.f1", print_hash_value);
    transparent_crc(g_54.f1, "g_54.f1", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_62.f0.f0.f0, "g_62.f0.f0.f0", print_hash_value);
    transparent_crc(g_62.f0.f0.f1, "g_62.f0.f0.f1", print_hash_value);
    transparent_crc(g_62.f0.f1, "g_62.f0.f1", print_hash_value);
    transparent_crc(g_66.f0.f0, "g_66.f0.f0", print_hash_value);
    transparent_crc(g_66.f0.f1, "g_66.f0.f1", print_hash_value);
    transparent_crc(g_66.f1, "g_66.f1", print_hash_value);
    transparent_crc(g_66.f2, "g_66.f2", print_hash_value);
    transparent_crc(g_66.f3.f0, "g_66.f3.f0", print_hash_value);
    transparent_crc(g_66.f3.f1, "g_66.f3.f1", print_hash_value);
    transparent_crc(g_72.f0, "g_72.f0", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_94[i], "g_94[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
