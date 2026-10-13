/* Csmith 0cdc710315cf seed 259, flattened by scripts/csmith_flatten.py */
// Options:   --seed 259 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
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
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
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
   int32_t  f0;
   uint8_t  f1;
   int16_t  f2;
};

union U1 {
   int32_t  f0;
   uint16_t  f1;
   const int64_t  f2;
};

union U2 {
   int8_t  f0;
   int16_t  f1;
};

union U3 {
   struct S0  f0;
};

static int32_t g_3 = (-9);
static int64_t g_5[4] = {6,6,6,6};
static int16_t g_6[1] = {0xE9C9};
static int16_t g_7 = 0x5196;
static uint64_t g_8 = 0x2C57E71F09A2B441LL;
static const union U3 g_35 = {{0x4FC4B664,0xB7,0x4763}};
static int32_t **g_36 = (void*)0;
static int32_t g_39 = 1;
static union U3 g_50 = {{0x456CFC4F,0U,0xE203}};
static int64_t g_72[4][1] = {{0xAB1DDA348D5C8807LL},{0xAB1DDA348D5C8807LL},{0xAB1DDA348D5C8807LL},{0xAB1DDA348D5C8807LL}};
static uint32_t g_74 = 0x88B1902D;
static int64_t g_77 = 0x5E415B52D1A88BB4LL;
static int64_t g_78 = 0xBD880D91824DA47CLL;
static union U2 g_93 = {-9};
static union U1 g_96 = {0x4261728A};
static struct S0 *g_134 = (void*)0;
static struct S0 **g_133 = &g_134;

static uint64_t  func_1(void);
static uint64_t  func_13(const int32_t ** p_14);
static const int32_t * func_19(int32_t  p_20, uint32_t  p_21);

static uint64_t  func_1(void)
{ 
    int32_t *l_2[1][2];
    int64_t l_4 = 2;
    int32_t **l_11 = &l_2[0][1];
    int32_t *l_12 = (void*)0;
    const int32_t *l_16[3];
    const int32_t **l_15 = &l_16[0];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_2[i][j] = &g_3;
    }
    for (i = 0; i < 3; i++)
        l_16[i] = (void*)0;
lbl_132:
    --g_8;
    l_12 = ((*l_11) = l_2[0][0]);
    if ((func_13(l_15) & (g_35.f0.f1 & (-7))))
    { 
        uint32_t l_127 = 4294967295U;
        ++l_127;
    }
    else
    { 
        for (g_77 = (-30); (g_77 != (-21)); ++g_77)
        { 
            if (g_96.f0)
                goto lbl_132;
            for (g_3 = 0; (g_3 <= 0); g_3 += 1)
            { 
                if (g_96.f0)
                    goto lbl_132;
            }
        }
        g_133 = g_133;
    }
    return g_6[0];
}

static uint64_t  func_13(const int32_t ** p_14)
{ 
    uint64_t l_106 = 1U;
    int32_t l_109 = (-1);
    uint32_t l_117[4] = {4294967295U,4294967295U,4294967295U,4294967295U};
    int i;
    for (g_8 = 14; (g_8 > 54); g_8 = safe_add_func_int8_t_s_s(g_8, 7))
    { 
        uint8_t l_22 = 0x06;
        int32_t *l_107 = (void*)0;
        int32_t *l_108 = &g_96.f0;
        int16_t l_123 = 1;
        (*p_14) = func_19(g_7, (l_22 , (~(safe_add_func_int16_t_s_s(((safe_rshift_func_uint64_t_u_s(0x547507AF9CA3EAA3LL, 40)) > 6U), 0x5616)))));
        for (g_96.f1 = 0; g_96.f1 < 4; g_96.f1 = g_96.f1 + 1)
        {
            g_5[g_96.f1] = 0xA4A3E56A0368A9B8LL;
        }
        if (((l_109 ^= ((*l_108) = l_106)) > (l_22 == ((l_106 && g_6[0]) > 0xE3A85E5D))))
        { 
            union U2 l_112 = {0xE0};
            int16_t *l_115 = &l_112.f1;
            int32_t l_116 = 0xE67F4868;
            if ((safe_mod_func_int32_t_s_s(((((l_112 , (l_106 > (safe_mod_func_int16_t_s_s(((((*l_115) = (g_50.f0.f2 >= l_112.f0)) <= 65535U) & (*l_108)), l_116)))) | (-10)) <= 0x73F0FF8D21F61EE9LL) ^ g_72[0][0]), g_50.f0.f1)))
            { 
                const int32_t *l_118 = (void*)0;
                int16_t *l_121 = (void*)0;
                uint8_t *l_122 = (void*)0;
                (*l_108) = l_117[0];
                l_118 = (*p_14);
                g_96.f0 = (l_123 |= ((l_109 = (((safe_add_func_uint8_t_u_u(l_112.f1, (l_121 == (void*)0))) == (*l_108)) > (-1))) >= g_35.f0.f2));
            }
            else
            { 
                return g_93.f0;
            }
        }
        else
        { 
            return g_5[3];
        }
    }
    for (g_50.f0.f2 = 0; (g_50.f0.f2 > 20); g_50.f0.f2 = safe_add_func_uint32_t_u_u(g_50.f0.f2, 2))
    { 
        int32_t *l_126[4][2] = {{&g_39,&g_39},{&g_39,&g_39},{&g_39,&g_39},{&g_39,&g_39}};
        int i, j;
        l_126[2][0] = (void*)0;
    }
    return g_74;
}

static const int32_t * func_19(int32_t  p_20, uint32_t  p_21)
{ 
    int32_t *l_28 = &g_3;
    uint64_t l_56 = 1U;
    struct S0 l_66 = {4,255U,-9};
    int32_t l_73 = 0x945F7E1E;
    int32_t l_79[2];
    uint16_t l_83[2];
    int i;
    for (i = 0; i < 2; i++)
        l_79[i] = 0x11ED7A4D;
    for (i = 0; i < 2; i++)
        l_83[i] = 4U;
    if (((*l_28) = p_21))
    { 
        uint8_t l_40 = 0x91;
        int32_t *l_46 = &g_39;
        for (g_3 = (-15); (g_3 <= (-16)); g_3 = safe_sub_func_uint64_t_u_u(g_3, 2))
        { 
            int64_t l_37 = 0x181448718302CCA7LL;
            int32_t *l_38[1];
            int i;
            for (i = 0; i < 1; i++)
                l_38[i] = &g_39;
            p_20 = (safe_mul_func_int32_t_s_s(((((safe_add_func_uint16_t_u_u((((g_35 , ((void*)0 == g_36)) ^ g_6[0]) && 0x07AD46D2), 65528U)) ^ p_20) && l_37) != (*l_28)), l_37));
            if (g_35.f0.f2)
                goto lbl_41;
        }
lbl_41:
        (*l_28) |= l_40;
        for (p_20 = 20; (p_20 >= 29); p_20 = safe_add_func_uint8_t_u_u(p_20, 8))
        { 
            union U3 * const l_49 = &g_50;
            union U3 *l_51 = (void*)0;
            int32_t **l_52 = &l_28;
            (*l_28) = (safe_mul_func_uint16_t_u_u((l_46 == (void*)0), (safe_mod_func_int16_t_s_s((4294967290U != g_5[0]), 65535U))));
            for (g_7 = 0; (g_7 >= 0); g_7 -= 1)
            { 
                l_51 = l_49;
            }
            (*l_52) = l_46;
        }
    }
    else
    { 
        int32_t **l_53 = &l_28;
        uint64_t *l_71[4][3] = {{&g_8,&g_8,&g_8},{&l_56,&l_56,&l_56},{&g_8,&g_8,&g_8},{&l_56,&l_56,&l_56}};
        int32_t l_80 = 0;
        int32_t l_81 = (-9);
        int32_t l_82 = 0x995E3EEF;
        struct S0 l_99 = {-1,255U,0x7AAA};
        int i, j;
        (*l_28) = (&l_28 == (l_53 = &l_28));
        if (((g_35.f0 , ((((p_21++) & (**l_53)) , (((**l_53) = g_5[0]) > l_56)) != p_20)) == p_20))
        { 
            union U2 l_59 = {0xE2};
            const int32_t *l_61[1][1];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 1; j++)
                    l_61[i][j] = &g_50.f0.f0;
            }
            p_20 &= (safe_div_func_uint32_t_u_u(((l_59 , &l_28) == &l_28), 0xA04A8F9B));
            (**l_53) ^= ((~(p_20 & (g_5[0] , 1U))) , p_20);
            return l_61[0][0];
        }
        else
        { 
            struct S0 l_64 = {0x2662B256,0x84,0};
            for (g_7 = (-24); (g_7 > (-2)); ++g_7)
            { 
                struct S0 *l_65[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_65[i] = &g_50.f0;
                l_66 = l_64;
                if (p_20)
                    continue;
            }
            for (l_66.f0 = 0; (l_66.f0 == 15); ++l_66.f0)
            { 
                struct S0 *l_69 = (void*)0;
                struct S0 **l_70 = &l_69;
                l_64.f0 = l_64.f1;
                (*l_70) = l_69;
                if (p_20)
                    continue;
            }
        }
        if ((((**l_53) = (g_74--)) > (l_83[0]++)))
        { 
            for (l_56 = (-8); (l_56 > 19); ++l_56)
            { 
                int32_t **l_88 = &l_28;
                l_53 = l_88;
            }
            (*l_28) = (safe_mul_func_uint16_t_u_u((safe_div_func_uint64_t_u_u((g_93 , ((safe_rshift_func_uint16_t_u_s((p_20 > (((g_96 , g_50.f0) , (**l_53)) || 0x3B65C4EC)), g_7)) >= (*l_28))), 18446744073709551606U)), p_21));
        }
        else
        { 
            struct S0 *l_100 = &l_99;
            union U3 l_101 = {{0xA977D591,0xF1,0x133D}};
            int32_t *l_102 = &g_39;
            for (g_7 = 0; (g_7 <= (-1)); --g_7)
            { 
                return &g_39;
            }
            if ((((*l_100) = l_99) , ((*l_102) |= (l_101 , ((**l_53) = (0xF603 | g_93.f0))))))
            { 
                (*l_102) = 0xB6AB9C1A;
                (**l_53) = 0x6FA54FBA;
            }
            else
            { 
                int32_t **l_105 = (void*)0;
                (**l_53) = ((*l_102) = (safe_mod_func_int32_t_s_s(((l_105 != &l_102) , (p_21 == g_39)), (**l_53))));
            }
        }
    }
    return &g_39;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_5[i], "g_5[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_6[i], "g_6[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_35.f0.f0, "g_35.f0.f0", print_hash_value);
    transparent_crc(g_35.f0.f1, "g_35.f0.f1", print_hash_value);
    transparent_crc(g_35.f0.f2, "g_35.f0.f2", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_50.f0.f0, "g_50.f0.f0", print_hash_value);
    transparent_crc(g_50.f0.f1, "g_50.f0.f1", print_hash_value);
    transparent_crc(g_50.f0.f2, "g_50.f0.f2", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_72[i][j], "g_72[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_78, "g_78", print_hash_value);
    transparent_crc(g_93.f0, "g_93.f0", print_hash_value);
    transparent_crc(g_96.f0, "g_96.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
