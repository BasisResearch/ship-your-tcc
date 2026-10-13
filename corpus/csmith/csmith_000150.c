/* Csmith 0cdc710315cf seed 150, flattened by scripts/csmith_flatten.py */
// Options:   --seed 150 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
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
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
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
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint32_t  f0;
   int8_t  f1;
   uint16_t  f2;
   int32_t  f3;
};

static uint8_t g_2 = 0x15;
static uint16_t g_4 = 0x91E2;
static struct S0 g_30[3][2] = {{{4294967289U,0x91,0x4C6E,0x2CDFD7F7},{4294967289U,1,0U,-1}},{{4294967289U,1,0U,-1},{4294967289U,0x91,0x4C6E,0x2CDFD7F7}},{{4294967289U,1,0U,-1},{4294967289U,1,0U,-1}}};
static uint32_t g_39 = 4294967295U;
static int16_t g_43 = 0x609E;
static int16_t *g_47 = &g_43;
static int16_t **g_46 = &g_47;
static int64_t g_80 = 0x328171C63CC5DB09LL;
static uint16_t g_81 = 65535U;
static int32_t g_86 = 0x31782E4D;
static int32_t *g_85 = &g_86;
static int32_t *g_87 = (void*)0;
static uint16_t g_103 = 0xD562;
static uint16_t *g_108 = (void*)0;

static int16_t  func_1(void);
static uint16_t  func_17(int16_t  p_18, uint16_t * p_19);
static uint16_t * func_20(int16_t  p_21, int16_t  p_22, struct S0  p_23, const uint64_t  p_24, uint8_t  p_25);

static int16_t  func_1(void)
{ 
    uint16_t *l_3 = &g_4;
    int32_t l_5[3];
    uint8_t l_6 = 0x70;
    int16_t l_110[4] = {0xAE78,0xAE78,0xAE78,0xAE78};
    int i;
    for (i = 0; i < 3; i++)
        l_5[i] = (-9);
    if ((((*l_3) = g_2) && (1 > (((l_5[0] && 1) , l_5[2]) < l_6))))
    { 
        int8_t l_7[4][3] = {{0xAF,0xAF,0xAF},{0x2D,0x2D,0x2D},{0xAF,0xAF,0xAF},{0x2D,0x2D,0x2D}};
        int i, j;
        l_7[1][1] |= (g_4 ^ l_6);
    }
    else
    { 
        int16_t l_8 = 0xB647;
        int32_t *l_9 = &l_5[2];
        uint8_t l_29[4];
        uint8_t l_88 = 7U;
        int32_t l_102[1][2];
        int i, j;
        for (i = 0; i < 4; i++)
            l_29[i] = 0x46;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_102[i][j] = (-4);
        }
        (*l_9) ^= l_8;
        for (l_8 = 0; (l_8 <= 18); ++l_8)
        { 
            int64_t l_31[2];
            int32_t *l_82 = &g_30[2][1].f3;
            int i;
            for (i = 0; i < 2; i++)
                l_31[i] = 1;
            if ((safe_lshift_func_uint16_t_u_u(g_2, ((*l_9) , (((l_6 && l_5[0]) || g_2) ^ g_2)))))
            { 
                if (g_4)
                    break;
                (*l_9) ^= g_4;
            }
            else
            { 
                (*l_9) |= 0;
                (*l_9) |= ((void*)0 != &g_4);
            }
            for (l_6 = (-8); (l_6 > 5); l_6 = safe_add_func_uint8_t_u_u(l_6, 3))
            { 
                uint64_t l_16[2][3] = {{0U,0U,0U},{18446744073709551615U,18446744073709551615U,18446744073709551615U}};
                int32_t **l_83 = (void*)0;
                int32_t **l_84 = (void*)0;
                int i, j;
                if (l_16[1][0])
                    break;
                g_81 |= ((func_17(l_16[1][1], func_20((((safe_add_func_uint64_t_u_u((0xAB && ((safe_unary_minus_func_int16_t_s(0)) || (-1))), 0U)) & 0x918C6A86) , 0x10B4), l_29[2], g_30[2][1], g_30[2][1].f1, l_31[1])) == g_80) , 0xCF10AA6F);
                g_87 = (g_85 = l_82);
            }
            for (g_39 = 0; (g_39 <= 3); g_39 += 1)
            { 
                int32_t **l_91 = &g_87;
                l_88--;
                (*l_91) = &g_86;
                (*l_91) = &g_86;
            }
        }
        for (g_4 = (-19); (g_4 >= 13); g_4 = safe_add_func_uint64_t_u_u(g_4, 4))
        { 
            uint16_t *l_107 = &g_4;
            int32_t l_109 = (-2);
            for (g_43 = 3; (g_43 >= 0); g_43 -= 1)
            { 
                int i;
                return l_29[g_43];
            }
            for (g_81 = 2; (g_81 < 16); ++g_81)
            { 
                int32_t **l_96 = (void*)0;
                int32_t **l_97 = &l_9;
                (*l_97) = l_9;
            }
            for (g_2 = 5; (g_2 >= 9); g_2 = safe_add_func_int64_t_s_s(g_2, 3))
            { 
                int32_t *l_100 = &g_30[2][1].f3;
                int32_t *l_101[1][4];
                uint16_t **l_106 = &l_3;
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 4; j++)
                        l_101[i][j] = (void*)0;
                }
                ++g_103;
                (*l_100) = ((g_30[2][1].f2 == (((*l_106) = &g_81) == (g_108 = l_107))) , l_109);
                (*g_85) &= ((l_5[0] || (**g_46)) > 1U);
            }
        }
    }
    return l_110[1];
}

static uint16_t  func_17(int16_t  p_18, uint16_t * p_19)
{ 
    uint8_t *l_55 = &g_2;
    int32_t l_66 = 0;
    int8_t *l_67 = &g_30[2][1].f1;
    int32_t *l_68 = &g_30[2][1].f3;
    (*l_68) |= (safe_div_func_uint32_t_u_u(((safe_div_func_uint16_t_u_u((safe_add_func_uint8_t_u_u((--(*l_55)), (((*g_47) = 0x2CC3) == (safe_lshift_func_int8_t_s_s(((*l_67) = (safe_div_func_uint32_t_u_u(((safe_mul_func_uint32_t_u_u(p_18, (((safe_mod_func_uint64_t_u_u(g_4, g_30[2][1].f0)) >= l_66) == 18446744073709551606U))) && g_4), p_18))), 0))))), 1)) , 0xB7DFF937), p_18));
    if (((safe_div_func_uint8_t_u_u(((((p_18 , ((safe_mul_func_int64_t_s_s((+(safe_rshift_func_int16_t_s_s((*g_47), 13))), (((((safe_mod_func_int64_t_s_s((((g_2 || 0U) == (*l_68)) , (*l_68)), 1U)) ^ 0x526697289F16C484LL) && 2U) == g_2) & (*l_68)))) < 0U)) || (*l_68)) || (*p_19)) , (*l_68)), 0xF2)) , p_18))
    { 
        return (*p_19);
    }
    else
    { 
        return g_43;
    }
}

static uint16_t * func_20(int16_t  p_21, int16_t  p_22, struct S0  p_23, const uint64_t  p_24, uint8_t  p_25)
{ 
    const int32_t *l_44[4][2] = {{&g_30[2][1].f3,&g_30[2][1].f3},{&g_30[2][1].f3,&g_30[2][1].f3},{&g_30[2][1].f3,&g_30[2][1].f3},{&g_30[2][1].f3,&g_30[2][1].f3}};
    int16_t **l_48 = (void*)0;
    int i, j;
    for (g_2 = (-22); (g_2 > 37); g_2++)
    { 
        uint32_t *l_38 = &g_39;
        int16_t *l_42 = &g_43;
        const int32_t **l_45 = &l_44[2][0];
        (*l_45) = ((safe_sub_func_uint16_t_u_u((safe_sub_func_uint32_t_u_u((++(*l_38)), p_23.f1)), ((*l_42) = g_30[2][1].f2))) , l_44[2][0]);
    }
    l_48 = g_46;
    return &g_4;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_30[i][j].f0, "g_30[i][j].f0", print_hash_value);
            transparent_crc(g_30[i][j].f1, "g_30[i][j].f1", print_hash_value);
            transparent_crc(g_30[i][j].f2, "g_30[i][j].f2", print_hash_value);
            transparent_crc(g_30[i][j].f3, "g_30[i][j].f3", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_80, "g_80", print_hash_value);
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_86, "g_86", print_hash_value);
    transparent_crc(g_103, "g_103", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
