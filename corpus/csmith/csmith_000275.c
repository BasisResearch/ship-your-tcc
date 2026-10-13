/* Csmith 0cdc710315cf seed 275, flattened by scripts/csmith_flatten.py */
// Options:   --seed 275 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
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
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
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
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
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
   int16_t  f0;
   int32_t  f1;
   int8_t  f2;
};

union U1 {
   uint16_t  f0;
   uint8_t  f1;
   const int8_t  f2;
};

union U2 {
   const uint32_t  f0;
};

static int32_t g_2 = 0x95364446;
static int32_t g_5 = 0x3BEB7161;
static int32_t g_9[1] = {1};
static int16_t g_18 = 0x88DC;
static uint8_t g_24 = 0x11;
static int32_t g_26 = (-7);
static int32_t **g_39 = (void*)0;
static union U2 g_41 = {0x5C7B1239};
static uint64_t g_63 = 18446744073709551608U;
static union U2 *g_73 = &g_41;
static union U2 **g_72[2][3] = {{&g_73,&g_73,&g_73},{&g_73,&g_73,&g_73}};
static union U2 **g_74 = &g_73;
static struct S0 g_77 = {2,0x5C8A6867,0};

static const uint32_t  func_1(void);
static uint16_t  func_10(int8_t  p_11, uint16_t  p_12, int16_t  p_13, union U1  p_14);
static int32_t ** func_32(int32_t ** const  p_33);

static const uint32_t  func_1(void)
{ 
    uint32_t l_29 = 0U;
    int32_t l_51 = (-1);
    int32_t *l_52[3][1];
    uint16_t l_53[2];
    uint8_t l_54 = 0U;
    int64_t l_67 = 0xA56EC073B9F6D4B6LL;
    uint16_t l_76 = 0x1061;
    int32_t l_83 = (-6);
    union U2 l_91[1][2] = {{{0x6D799D52},{0x6D799D52}}};
    int16_t l_93[2];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_52[i][j] = &g_9[0];
    }
    for (i = 0; i < 2; i++)
        l_53[i] = 0xFA82;
    for (i = 0; i < 2; i++)
        l_93[i] = 5;
    for (g_2 = 0; (g_2 == (-4)); g_2--)
    { 
        int32_t l_28[4];
        int i;
        for (i = 0; i < 4; i++)
            l_28[i] = 0;
        for (g_5 = 0; (g_5 < 25); g_5 = safe_add_func_uint16_t_u_u(g_5, 8))
        { 
            int64_t l_8[4][3] = {{0x619A80BFB3A4DF6FLL,0x619A80BFB3A4DF6FLL,0x0946507E673292C5LL},{0x6887E08A11015AA9LL,0x0946507E673292C5LL,0x0946507E673292C5LL},{0x0946507E673292C5LL,(-4),0x141E4F1F13B01D91LL},{0x6887E08A11015AA9LL,(-4),0x6887E08A11015AA9LL}};
            int64_t l_19 = 0xDE9E5A9390DFC1F7LL;
            int i, j;
            for (g_9[0] = 2; (g_9[0] >= 0); g_9[0] -= 1)
            { 
                int16_t *l_17[2][1];
                union U1 l_20 = {0xE4BC};
                int32_t *l_27[3];
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_17[i][j] = &g_18;
                }
                for (i = 0; i < 3; i++)
                    l_27[i] = &g_26;
                l_28[1] = (((func_10(g_9[0], g_2, (l_19 = ((safe_rshift_func_uint32_t_u_u((0x15A60915BB002891LL != 0), 8)) ^ 0x4E401D933BD3E018LL)), l_20) < 1U) | 0) | g_2);
                return l_28[3];
            }
        }
        if ((l_28[1] ^ l_29))
        { 
            int32_t *l_35 = &g_9[0];
            int32_t ** const l_34 = &l_35;
            for (g_5 = 0; (g_5 < (-9)); g_5 = safe_sub_func_int8_t_s_s(g_5, 1))
            { 
                g_39 = func_32(l_34);
                if (l_29)
                    continue;
            }
            return g_5;
        }
        else
        { 
            union U2 *l_40[2];
            union U2 **l_42 = &l_40[1];
            int i;
            for (i = 0; i < 2; i++)
                l_40[i] = &g_41;
            (*l_42) = l_40[0];
            return g_9[0];
        }
    }
    g_9[0] = (((l_29 ^ (safe_rshift_func_uint32_t_u_s(((safe_lshift_func_uint8_t_u_s(((safe_lshift_func_uint64_t_u_u((safe_rshift_func_uint16_t_u_s(65531U, ((g_18 & g_41.f0) > g_5))), 37)) , l_51), 0)) , l_29), l_51))) < 0xC180) || l_29);
    if (l_53[0])
    { 
        int32_t *l_61[3];
        int i;
        for (i = 0; i < 3; i++)
            l_61[i] = &g_9[0];
        for (g_18 = 0; (g_18 >= 0); g_18 -= 1)
        { 
            int32_t l_64 = 7;
            int32_t l_66 = 1;
            int i;
            g_9[g_18] |= l_54;
            for (l_51 = 0; (l_51 < (-28)); l_51 = safe_sub_func_uint8_t_u_u(l_51, 7))
            { 
                union U2 l_59[3][1] = {{{4294967295U}},{{4294967295U}},{{4294967295U}}};
                int32_t **l_60 = &l_52[1][0];
                uint64_t *l_62 = &g_63;
                int32_t l_65 = 0x23341981;
                int32_t l_68 = 0x4B4F5124;
                uint32_t l_69 = 0xFDF84107;
                int i, j;
                g_9[g_18] = (safe_add_func_uint64_t_u_u((((*l_62) = ((((*l_60) = (l_59[1][0] , &g_2)) == l_61[0]) , g_2)) && g_18), 0xEB0CCCF04A32A0C0LL));
                l_69--;
            }
            g_74 = g_72[1][2];
        }
    }
    else
    { 
        int64_t *l_75 = &l_67;
        struct S0 *l_78 = (void*)0;
        int32_t l_84 = (-1);
        g_77 = (((((*l_75) &= g_26) || (l_76 , g_63)) , g_18) , g_77);
        if (((safe_add_func_uint16_t_u_u(0xAB14, (&g_9[0] != (void*)0))) >= 1))
        { 
            uint64_t l_85[1][2];
            union U1 l_92 = {0x01A5};
            int64_t l_94 = 0x91F76D5A1B73F452LL;
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_85[i][j] = 0x7329A4BB29D6AE76LL;
            }
            for (l_29 = 0; (l_29 <= 0); l_29 += 1)
            { 
                int32_t l_81 = 1;
                int32_t l_82[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_82[i] = 5;
                l_85[0][0]++;
                l_84 &= (~(safe_mod_func_int32_t_s_s((((l_91[0][1] , l_53[l_29]) | ((l_92 , l_93[0]) <= g_24)) <= l_94), g_26)));
                if (l_85[0][1])
                    continue;
            }
        }
        else
        { 
            return g_77.f1;
        }
    }
    return g_77.f0;
}

static uint16_t  func_10(int8_t  p_11, uint16_t  p_12, int16_t  p_13, union U1  p_14)
{ 
    int16_t *l_21 = &g_18;
    uint8_t *l_22 = (void*)0;
    uint8_t *l_23 = &g_24;
    int32_t *l_25 = &g_26;
    (*l_25) &= (((*l_23) = (l_21 == l_21)) & p_11);
    return p_11;
}

static int32_t ** func_32(int32_t ** const  p_33)
{ 
    int32_t *l_37 = &g_5;
    int32_t **l_36 = &l_37;
    int32_t **l_38[2];
    int i;
    for (i = 0; i < 2; i++)
        l_38[i] = &l_37;
    return g_39;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_9[i], "g_9[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_41.f0, "g_41.f0", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_77.f0, "g_77.f0", print_hash_value);
    transparent_crc(g_77.f1, "g_77.f1", print_hash_value);
    transparent_crc(g_77.f2, "g_77.f2", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
