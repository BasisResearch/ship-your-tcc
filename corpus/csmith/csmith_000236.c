/* Csmith 0cdc710315cf seed 236, flattened by scripts/csmith_flatten.py */
// Options:   --seed 236 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
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
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
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
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const int16_t  f0;
   const int16_t  f1;
   uint8_t  f2;
};

static uint64_t g_2 = 0x4A53AA9AC70C3099LL;
static int32_t g_4 = 0x6580849F;
static uint64_t g_5 = 0xBEDEDB5CABFC3938LL;
static uint32_t g_30 = 0x68DACB31;
static int32_t g_32 = (-3);
static uint32_t g_58 = 3U;
static int32_t g_63 = (-1);
static uint64_t g_93 = 0xF42B3FDF2DC13529LL;
static int32_t *g_97 = &g_63;

static uint16_t  func_1(void);
static int32_t * func_10(int32_t * p_11, uint8_t  p_12, int32_t  p_13, uint8_t  p_14, int16_t  p_15);
static int32_t * func_16(struct S0  p_17, int32_t * p_18);

static uint16_t  func_1(void)
{ 
    uint16_t l_77[1];
    int32_t *l_107[3][1];
    uint32_t l_109 = 4294967293U;
    int i, j;
    for (i = 0; i < 1; i++)
        l_77[i] = 0x552A;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_107[i][j] = &g_32;
    }
    if (g_2)
    { 
        int32_t *l_3[1];
        int i;
        for (i = 0; i < 1; i++)
            l_3[i] = &g_4;
        g_5++;
    }
    else
    { 
        struct S0 l_19 = {0x0515,0x66BC,0x57};
        int32_t *l_76 = (void*)0;
        uint16_t l_110 = 1U;
        int64_t l_118[3];
        int i;
        for (i = 0; i < 3; i++)
            l_118[i] = 0xB1924C0CEF9D813ALL;
        for (g_4 = (-27); (g_4 < 9); g_4 = safe_add_func_uint8_t_u_u(g_4, 1))
        { 
            int8_t l_36 = (-1);
            int32_t **l_73 = (void*)0;
            int32_t *l_75 = (void*)0;
            int32_t **l_74 = &l_75;
            (*l_74) = func_10(func_16(l_19, &g_4), g_2, g_5, g_4, l_36);
            (*l_74) = l_76;
        }
        for (l_19.f2 = 0; (l_19.f2 <= 0); l_19.f2 += 1)
        { 
            uint64_t *l_84 = &g_5;
            int32_t *l_94 = &g_32;
            int32_t **l_95 = (void*)0;
            int32_t **l_96[2][4] = {{&l_76,&l_76,&l_76,&l_76},{&l_76,&l_76,&l_76,&l_76}};
            uint8_t l_106 = 0x4E;
            int i, j;
            (*l_94) = ((safe_sub_func_int8_t_s_s((safe_rshift_func_int32_t_s_u(((((((safe_mul_func_int32_t_s_s(((g_2 = ((*l_84)--)) == (-10)), ((safe_lshift_func_int16_t_s_u((safe_rshift_func_int64_t_s_s((safe_lshift_func_int8_t_s_u((g_93 &= 2), 6)), 54)), 9)) <= (l_77[l_19.f2] != g_63)))) , l_94) == (void*)0) != (*l_94)) , 0xBC) && 246U), l_77[0])), g_32)) && 4294967292U);
            g_97 = &g_63;
            for (g_63 = 0; (g_63 >= 0); g_63 -= 1)
            { 
                int32_t l_100 = 0x53D9B4DC;
                int i;
                l_100 = (0xAAC0 != ((((g_58 > (l_77[g_63] = (safe_mul_func_int8_t_s_s(g_93, g_93)))) , l_77[0]) | 0x71) != (*l_94)));
                (*l_94) &= ((0x8FF697105BA91867LL ^ (safe_sub_func_int64_t_s_s(0x2B9CE03596026DF2LL, ((safe_lshift_func_int8_t_s_s((((!0xD2) > 0xB6A4) , 0xAC), 4)) <= l_77[0])))) & l_106);
                l_107[1][0] = &g_32;
            }
        }
        for (g_63 = 0; (g_63 >= 0); g_63 -= 1)
        { 
            int64_t l_117 = 0xD249FF5904A8A8F1LL;
            for (g_5 = 0; (g_5 <= 0); g_5 += 1)
            { 
                int32_t l_108 = 0xD9254FF3;
                int i;
                if (l_77[g_5])
                    break;
                g_4 &= ((l_108 = (g_32 = (*g_97))) | g_93);
            }
            if (l_109)
            { 
                if ((*g_97))
                    break;
            }
            else
            { 
                ++l_110;
                return g_5;
            }
            l_118[0] |= ((((safe_mul_func_uint16_t_u_u((((void*)0 != &g_97) | (safe_mul_func_uint64_t_u_u(((l_117 || g_4) | 0), 0x286D7ADF186003B3LL))), g_32)) > g_32) == 0x8A) != g_4);
        }
    }
    return g_63;
}

static int32_t * func_10(int32_t * p_11, uint8_t  p_12, int32_t  p_13, uint8_t  p_14, int16_t  p_15)
{ 
    int32_t *l_38 = &g_4;
    int32_t **l_37 = &l_38;
    int64_t l_39[3];
    int32_t l_72 = 0x8ADB298F;
    int i;
    for (i = 0; i < 3; i++)
        l_39[i] = 0xB249E3C8D3FD9053LL;
    (*l_37) = (g_32 , &p_13);
    for (g_30 = 0; (g_30 <= 2); g_30 += 1)
    { 
        int64_t l_61 = 0xC59A6287676E3B61LL;
        int32_t *l_71[2][2] = {{&g_63,&g_63},{&g_63,&g_63}};
        int i, j;
        for (g_5 = 0; (g_5 <= 2); g_5 += 1)
        { 
            uint64_t l_62 = 18446744073709551608U;
            int32_t l_65 = 0x2A2CA01D;
            for (g_32 = 2; (g_32 >= 0); g_32 -= 1)
            { 
                int32_t l_56 = 5;
                uint32_t *l_57 = &g_58;
                int32_t *l_64[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_64[i] = &l_56;
                l_65 |= (((safe_mod_func_uint64_t_u_u(((safe_add_func_uint64_t_u_u((safe_mod_func_int8_t_s_s((-5), (safe_rshift_func_int8_t_s_s(((l_39[g_5] == (((safe_mod_func_uint32_t_u_u(((safe_add_func_int64_t_s_s((safe_add_func_uint32_t_u_u((safe_rshift_func_uint32_t_u_s(((*l_57)--), 4)), (**l_37))), 1U)) > g_2), l_39[g_5])) | 0xFA2F) >= (*l_38))) >= l_61), 2)))), l_62)) , g_5), l_62)) < g_63) , (**l_37));
                if ((*p_11))
                    break;
                (*l_37) = &p_13;
            }
            for (g_32 = 0; g_32 < 3; g_32 = g_32 + 1)
            {
                l_39[g_32] = 0x53A2AD41D6648539LL;
            }
        }
        if ((*p_11))
            continue;
        l_72 = ((*l_38) = ((safe_add_func_uint8_t_u_u((p_14 |= (((void*)0 != &p_13) > ((safe_mod_func_uint16_t_u_u(((~((g_5 == 0x42121A65) , g_58)) <= p_15), 0x2AAC)) && 2U))), g_32)) , (-6)));
    }
    return &g_4;
}

static int32_t * func_16(struct S0  p_17, int32_t * p_18)
{ 
    uint8_t l_26 = 0x23;
    int32_t *l_28[3][2] = {{&g_4,&g_4},{&g_4,&g_4},{&g_4,&g_4}};
    int32_t **l_27 = &l_28[1][1];
    uint32_t *l_29 = &g_30;
    int32_t *l_31 = &g_32;
    int i, j;
    if (((*l_31) = ((((safe_sub_func_int16_t_s_s((((safe_mul_func_uint32_t_u_u(((*l_29) ^= (((g_2 <= (l_26 >= ((((*l_27) = &g_4) == &g_4) && 0xF39281C1D99EA5D8LL))) > p_17.f0) < (*p_18))), g_2)) ^ p_17.f0) , 1), 0xF303)) , 0xD0) & 0xC0) | g_4)))
    { 
        const int32_t *l_35 = &g_4;
        for (p_17.f2 = (-30); (p_17.f2 != 30); p_17.f2 = safe_add_func_int16_t_s_s(p_17.f2, 5))
        { 
            (*l_31) &= (g_30 || (l_35 != (void*)0));
        }
    }
    else
    { 
        return p_18;
    }
    g_32 = 8;
    return p_18;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
