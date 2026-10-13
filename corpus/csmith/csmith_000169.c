/* Csmith 0cdc710315cf seed 169, flattened by scripts/csmith_flatten.py */
// Options:   --seed 169 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   int64_t  f0;
};

static int32_t g_2 = 0xAE9714A0;
static union U0 g_10 = {0x41651328F6C74AA0LL};
static uint8_t g_19 = 0x1D;
static int32_t g_29 = 0x9CA4F067;
static int16_t g_33 = 0;
static uint8_t g_34 = 0x88;
static int64_t g_37 = 7;
static int64_t g_47 = 0x0AE22781501494D8LL;
static uint64_t g_63 = 0x5B3A04BB84A8A623LL;
static int32_t *g_66 = &g_29;
static uint8_t g_74[4] = {249U,249U,249U,249U};
static uint8_t g_87 = 0x06;
static int32_t *g_90 = &g_29;
static int8_t g_99 = 2;
static uint16_t g_100 = 65531U;
static uint64_t *g_110 = &g_63;
static uint64_t **g_109[4][4] = {{&g_110,&g_110,(void*)0,&g_110},{&g_110,&g_110,&g_110,&g_110},{&g_110,&g_110,&g_110,&g_110},{&g_110,&g_110,(void*)0,&g_110}};
static uint64_t g_117 = 0x06A83B75A479C51ALL;

static uint16_t  func_1(void);
static int32_t * func_5(union U0  p_6, const uint64_t  p_7, int64_t  p_8, int16_t  p_9);
static int32_t * func_60(uint64_t  p_61);

static uint16_t  func_1(void)
{ 
    int16_t l_17 = 0x4A6F;
    int32_t *l_88 = &g_29;
    int32_t l_96 = (-1);
    int32_t l_97 = 0xA879AF5B;
    int32_t l_106[2];
    uint16_t l_131 = 7U;
    int i;
    for (i = 0; i < 2; i++)
        l_106[i] = 0x86D3611A;
    for (g_2 = 0; (g_2 == 15); g_2++)
    { 
        uint8_t *l_18[4] = {&g_19,&g_19,&g_19,&g_19};
        const int8_t l_20 = 6;
        int32_t **l_89[3][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
        int16_t l_98 = 2;
        int i, j;
        g_90 = (l_88 = func_5(g_10, ((safe_mul_func_uint8_t_u_u((safe_mul_func_int8_t_s_s(g_2, (g_19 = (safe_div_func_int64_t_s_s(0, l_17))))), g_10.f0)) , l_20), l_17, g_10.f0));
        for (g_29 = 21; (g_29 != 29); g_29++)
        { 
            uint8_t l_95 = 247U;
            for (g_47 = 0; (g_47 <= (-28)); g_47 = safe_sub_func_uint64_t_u_u(g_47, 2))
            { 
                return l_95;
            }
        }
        g_100--;
    }
    if (((*l_88) , ((~(g_2 , (safe_mul_func_int32_t_s_s(l_106[1], 0x22056F02)))) > g_74[1])))
    { 
        uint64_t *l_108 = &g_63;
        uint64_t **l_107[2];
        int32_t l_111 = 0xBCF54F23;
        int32_t *l_112 = &l_106[1];
        int32_t l_113 = 0x79F83FA7;
        int32_t *l_114 = &l_96;
        int32_t *l_115 = (void*)0;
        int32_t *l_116[4][1];
        int i, j;
        for (i = 0; i < 2; i++)
            l_107[i] = &l_108;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_116[i][j] = &l_113;
        }
        g_109[1][1] = l_107[1];
        g_117--;
        return (*l_88);
    }
    else
    { 
        int8_t l_120 = 9;
        int16_t *l_121 = &l_17;
        int16_t *l_122[1];
        int i;
        for (i = 0; i < 1; i++)
            l_122[i] = &g_33;
        if (((g_33 &= ((*l_121) ^= l_120)) ^ ((safe_sub_func_uint64_t_u_u(((((safe_mod_func_int32_t_s_s((0x074BD288 | (safe_rshift_func_int32_t_s_u((l_120 , (*l_88)), l_120))), 5)) && 247U) | 6) <= g_63), g_10.f0)) > l_120)))
        { 
            for (l_97 = 0; (l_97 < (-22)); l_97--)
            { 
                return g_87;
            }
        }
        else
        { 
            return l_131;
        }
    }
    return (*l_88);
}

static int32_t * func_5(union U0  p_6, const uint64_t  p_7, int64_t  p_8, int16_t  p_9)
{ 
    int32_t *l_23 = &g_2;
    int16_t l_24[4][4] = {{0x9A27,0x7C86,0x9A27,0x9A27},{0x7C86,0x7C86,0x2628,0x7C86},{0x7C86,0x9A27,0x2628,0x9A27},{0x2628,0x9A27,0x2628,0x2628}};
    int16_t l_41 = 9;
    int32_t l_42[1];
    union U0 *l_72 = &g_10;
    uint64_t l_85[2];
    int i, j;
    for (i = 0; i < 1; i++)
        l_42[i] = 0;
    for (i = 0; i < 2; i++)
        l_85[i] = 0xB54C1CE2A3F7B2C8LL;
    l_24[1][2] ^= ((safe_mul_func_uint16_t_u_u(((g_19 , g_19) & ((l_23 == l_23) != 0x1320F2D6273D5943LL)), 0xF7C6)) | (*l_23));
    if (p_6.f0)
    { 
        uint8_t l_38 = 0x29;
        int32_t l_43[3][2];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 2; j++)
                l_43[i][j] = 0xD39EF5B5;
        }
        for (p_9 = (-20); (p_9 > (-1)); ++p_9)
        { 
            int8_t l_27 = (-2);
            int32_t *l_28 = &g_29;
            int32_t *l_30 = &g_29;
            int32_t *l_31 = &g_29;
            int32_t *l_32[2];
            uint32_t l_52 = 4294967295U;
            int i;
            for (i = 0; i < 2; i++)
                l_32[i] = &g_29;
            g_34--;
            if (p_9)
            { 
                uint32_t l_44 = 4294967295U;
                l_38++;
                l_44--;
            }
            else
            { 
                uint64_t l_48 = 0xDD6B9EC499E98C53LL;
                int32_t l_51 = 0xA51C18CC;
                l_48--;
                l_51 |= ((*l_30) = 5);
            }
            l_52++;
        }
        for (p_6.f0 = 0; (p_6.f0 < 15); ++p_6.f0)
        { 
            return &g_2;
        }
        l_43[1][0] = (safe_add_func_int64_t_s_s((0x931B & ((void*)0 != &l_43[0][1])), p_7));
    }
    else
    { 
        int32_t *l_59[4][4] = {{&l_42[0],&l_42[0],&l_42[0],&l_42[0]},{&l_42[0],&l_42[0],&l_42[0],&l_42[0]},{&l_42[0],&l_42[0],&l_42[0],&l_42[0]},{&l_42[0],&l_42[0],&l_42[0],&l_42[0]}};
        uint64_t *l_62 = &g_63;
        int32_t **l_68 = &l_59[2][3];
        int i, j;
        g_29 = (*l_23);
        (*l_68) = func_60((++(*l_62)));
        for (g_34 = 0; (g_34 <= 3); g_34 += 1)
        { 
            uint8_t *l_69 = &g_34;
            int32_t l_80 = 0xF6366113;
            if ((l_69 == &g_34))
            { 
                union U0 *l_71 = &g_10;
                union U0 **l_70[4] = {&l_71,&l_71,&l_71,&l_71};
                int32_t l_73 = 0x39A040D4;
                int i;
                l_72 = &g_10;
                g_74[1] |= (l_73 &= (*g_66));
            }
            else
            { 
                int32_t *l_75 = &l_42[0];
                (*l_68) = l_75;
            }
            for (g_10.f0 = 3; (g_10.f0 >= 0); g_10.f0 -= 1)
            { 
                int64_t l_86 = (-1);
                int i, j;
                g_87 &= (safe_add_func_uint16_t_u_u((((safe_mul_func_uint16_t_u_u(((((l_80 <= (+(((safe_div_func_int8_t_s_s((((0xC372 >= ((+(p_6.f0 <= g_29)) ^ l_85[1])) < 0xEAC61C72) != p_9), g_19)) , (*l_23)) >= g_37))) != 0x6005) , &p_7) == (void*)0), l_80)) || g_47) , 6U), l_86));
                if ((*g_66))
                    continue;
                (*g_66) = 0x99C90458;
            }
        }
    }
    return &g_29;
}

static int32_t * func_60(uint64_t  p_61)
{ 
    int32_t *l_67 = &g_2;
    g_66 = &g_29;
    return l_67;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_10.f0, "g_10.f0", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_34, "g_34", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_74[i], "g_74[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_87, "g_87", print_hash_value);
    transparent_crc(g_99, "g_99", print_hash_value);
    transparent_crc(g_100, "g_100", print_hash_value);
    transparent_crc(g_117, "g_117", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
