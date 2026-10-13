/* Csmith 0cdc710315cf seed 307, flattened by scripts/csmith_flatten.py */
// Options:   --seed 307 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
/* ---- generated program ---- */
static long __undefined;

static int16_t g_7 = 0xEB71;
static int64_t g_9 = 0x46507E673292C5EBLL;
static int32_t g_30 = (-1);
static uint16_t g_35 = 0x8E10;
static int32_t g_37 = 0x9390DFC1;
static uint8_t g_38 = 254U;
static uint16_t *g_52 = &g_35;
static uint16_t **g_51[4][4] = {{&g_52,&g_52,&g_52,&g_52},{&g_52,&g_52,&g_52,&g_52},{&g_52,&g_52,&g_52,&g_52},{&g_52,&g_52,&g_52,&g_52}};
static uint16_t g_71[2][4] = {{0x22AD,0x22AD,0x22AD,0x22AD},{0x22AD,0x22AD,0x22AD,0x22AD}};
static uint8_t g_80 = 0x56;
static uint16_t g_82 = 1U;
static uint32_t g_87[1] = {0xDAC0D4F8};
static int16_t *g_107[2] = {&g_7,&g_7};
static int32_t g_136 = 0xF4738C55;

static int32_t  func_1(void);
static int16_t  func_2(const int16_t  p_3, int32_t  p_4, int8_t  p_5);
static uint8_t  func_16(int16_t * p_17, uint16_t  p_18, int16_t * p_19);

static int32_t  func_1(void)
{ 
    int16_t *l_6 = &g_7;
    int32_t l_8[2];
    int32_t *l_135 = &g_136;
    int i;
    for (i = 0; i < 2; i++)
        l_8[i] = 0x79CF1813;
    (*l_135) &= (((func_2((((*l_6) = 0xBD94) < l_8[0]), g_9, g_9) , g_38) > g_82) && l_8[0]);
    return (*l_135);
}

static int16_t  func_2(const int16_t  p_3, int32_t  p_4, int8_t  p_5)
{ 
    int8_t l_24 = 0x37;
    uint32_t l_79 = 4294967295U;
    uint16_t *l_81[4][2] = {{&g_82,&g_82},{&g_82,&g_82},{&g_82,&g_82},{&g_82,&g_82}};
    int32_t l_83 = 0x3B9F6D4B;
    int64_t l_132 = (-1);
    int32_t *l_134[3][2];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_134[i][j] = &l_83;
    }
    if ((safe_add_func_int16_t_s_s(((l_83 &= (safe_lshift_func_int8_t_s_s((safe_mul_func_uint8_t_u_u((func_16(&g_7, g_7, ((safe_mod_func_int64_t_s_s((safe_mul_func_uint8_t_u_u((l_24 , 0x63), l_24)), l_24)) , (void*)0)) , 252U), l_79)), g_80))) != p_3), l_24)))
    { 
        const int32_t *l_84 = &g_37;
        uint16_t **l_98 = &l_81[0][1];
        int16_t l_101 = 1;
        int16_t *l_106[2][1];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 1; j++)
                l_106[i][j] = (void*)0;
        }
        if (g_7)
        { 
            const int32_t **l_85 = &l_84;
            (*l_85) = l_84;
            if (g_9)
                goto lbl_131;
        }
        else
        { 
            int32_t *l_86[2];
            int16_t *l_105 = &g_7;
            int16_t **l_104[4][3] = {{&l_105,&l_105,&l_105},{(void*)0,(void*)0,(void*)0},{&l_105,&l_105,&l_105},{(void*)0,(void*)0,(void*)0}};
            int32_t *l_119[3];
            int i, j;
            for (i = 0; i < 2; i++)
                l_86[i] = &g_30;
            for (i = 0; i < 3; i++)
                l_119[i] = (void*)0;
            ++g_87[0];
            for (g_30 = (-10); (g_30 >= 11); g_30 = safe_add_func_uint32_t_u_u(g_30, 5))
            { 
                int8_t *l_99 = &l_24;
                int32_t l_100 = 0x98501CA4;
                g_37 = (safe_rshift_func_uint8_t_u_u((safe_mul_func_uint32_t_u_u(((safe_mul_func_int8_t_s_s((g_87[0] , ((*l_99) ^= (l_98 == (g_51[0][2] = &g_52)))), l_100)) == l_83), g_7)), 1));
                if ((*l_84))
                    continue;
                if (l_79)
                    goto lbl_131;
                l_101 &= 0x873846C4;
            }
            if (((safe_add_func_int16_t_s_s(g_71[1][1], (((l_106[1][0] = &g_7) == g_107[1]) | 0x4CD5CA7D))) < p_4))
            { 
                int8_t *l_112 = (void*)0;
                int8_t *l_113 = &l_24;
                p_4 = (safe_mul_func_int16_t_s_s(p_3, (safe_mul_func_int16_t_s_s(((((*l_113) = g_87[0]) == (((p_5 , &g_107[0]) != (void*)0) >= p_4)) | 0xED3424908F88A645LL), 0x0FBF))));
            }
            else
            { 
                int16_t **l_118[3][3] = {{&l_106[1][0],&g_107[1],&l_106[1][0]},{&g_107[1],&g_107[1],&g_107[1]},{&l_106[1][0],&g_107[1],&l_106[1][0]}};
                int32_t *l_120 = (void*)0;
                int i, j;
                p_4 ^= ((0x3BFE2B3C890F2B86LL | (((safe_lshift_func_int8_t_s_u(((((g_82 , &g_7) != &g_7) != l_24) & g_30), g_7)) , (void*)0) != l_118[1][2])) ^ p_5);
                l_120 = l_119[1];
            }
        }
    }
    else
    { 
lbl_131:
        for (g_38 = 0; (g_38 > 4); g_38 = safe_add_func_int16_t_s_s(g_38, 6))
        { 
            int32_t *l_128 = &g_37;
            int32_t **l_127 = &l_128;
            for (g_7 = 0; (g_7 != (-27)); g_7--)
            { 
                g_37 &= (safe_lshift_func_int8_t_s_u(p_4, 1));
            }
            (*l_127) = &g_30;
            for (l_79 = 0; (l_79 <= 3); l_79 += 1)
            { 
                int16_t *l_130 = (void*)0;
                (*l_127) = &g_30;
                (**l_127) ^= (~((void*)0 != l_130));
            }
        }
        return l_132;
    }
    g_37 &= (p_4 = (!g_80));
    return p_5;
}

static uint8_t  func_16(int16_t * p_17, uint16_t  p_18, int16_t * p_19)
{ 
    uint8_t l_29[1];
    uint16_t ** const l_50 = (void*)0;
    int32_t l_53 = (-8);
    int8_t l_56 = (-5);
    int32_t *l_74 = &g_30;
    int32_t *l_75[2][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
    uint8_t l_76 = 1U;
    int i, j;
    for (i = 0; i < 1; i++)
        l_29[i] = 253U;
    for (g_9 = 0; (g_9 != 25); g_9++)
    { 
        uint8_t l_55[4][2] = {{251U,251U},{251U,251U},{251U,251U},{251U,251U}};
        uint8_t *l_57[1][3];
        int32_t l_58[2];
        const uint32_t l_59[2][2] = {{4294967295U,4294967295U},{4294967295U,4294967295U}};
        int32_t *l_60 = &g_37;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_57[i][j] = &g_38;
        }
        for (i = 0; i < 2; i++)
            l_58[i] = 0x8275BAED;
        for (g_7 = 5; (g_7 < 22); g_7 = safe_add_func_int8_t_s_s(g_7, 8))
        { 
            uint32_t l_33 = 4294967288U;
            uint16_t *l_34 = &g_35;
            int32_t *l_36[4] = {&g_37,&g_37,&g_37,&g_37};
            uint8_t *l_54 = &g_38;
            int i;
            if (((g_30 = l_29[0]) > (safe_add_func_int16_t_s_s(((g_37 = ((l_33 , ((*l_34) = g_7)) ^ 0U)) ^ g_7), 65534U))))
            { 
                g_30 ^= l_29[0];
            }
            else
            { 
                const uint16_t l_49[4][1] = {{8U},{0x30FA},{8U},{0x30FA}};
                int i, j;
                g_38--;
                g_30 = ((((safe_add_func_uint32_t_u_u((safe_add_func_uint16_t_u_u(g_9, (safe_add_func_uint64_t_u_u((safe_mod_func_uint16_t_u_u(g_37, ((g_38 == l_49[2][0]) ^ p_18))), p_18)))), g_9)) ^ 1U) != 0x2B5D) && g_35);
                g_51[3][1] = l_50;
            }
            l_56 = ((((p_18 = ((l_53 = (-1)) == ((*l_54) &= g_7))) , ((*l_34) ^= 0xA2E3)) != l_55[2][0]) | l_29[0]);
        }
        (*l_60) = ((l_58[0] = ((1U ^ (0U | 0x7709C18003DBE6A9LL)) , p_18)) < l_59[0][1]);
        for (g_38 = 0; (g_38 <= 1); g_38 += 1)
        { 
            if (l_53)
                break;
        }
    }
    for (p_18 = 0; (p_18 < 17); p_18++)
    { 
        uint64_t l_68 = 0x6EC4F4D126829A6ELL;
        if (g_37)
            break;
        for (g_38 = 0; (g_38 > 21); g_38 = safe_add_func_int8_t_s_s(g_38, 2))
        { 
            int64_t l_72[4][4];
            int32_t *l_73 = &l_53;
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                    l_72[i][j] = 0x50224C6ACEBDD5E1LL;
            }
            (*l_73) |= (((safe_add_func_uint32_t_u_u(0xC9D266AB, (+l_68))) | (safe_mul_func_int64_t_s_s((((((g_71[1][1] = 9U) , l_72[3][3]) , g_30) , &g_38) != (void*)0), l_68))) && l_72[0][2]);
        }
    }
    l_76--;
    return g_35;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_71[i][j], "g_71[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_80, "g_80", print_hash_value);
    transparent_crc(g_82, "g_82", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_87[i], "g_87[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_136, "g_136", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
