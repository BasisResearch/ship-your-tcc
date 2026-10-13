/* Csmith 0cdc710315cf seed 320, flattened by scripts/csmith_flatten.py */
// Options:   --seed 320 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
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
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (18446744073709551615UL >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = 0x99DADA43;
static uint16_t g_27 = 0x99FF;
static uint64_t g_32 = 2U;
static uint64_t *g_31 = &g_32;
static int32_t g_38 = (-1);
static int32_t g_41 = 1;
static int16_t g_43[3] = {0x138D,0x138D,0x138D};
static int32_t g_44 = 0x46CA37EA;
static int32_t g_45 = 0x13122E3B;
static int8_t g_46 = (-10);
static uint32_t g_47[4] = {0xB172C66C,0xB172C66C,0xB172C66C,0xB172C66C};
static int32_t *g_54 = &g_45;
static uint16_t *g_86 = &g_27;
static uint16_t **g_85 = &g_86;

static int32_t  func_1(void);
static int32_t  func_4(int32_t  p_5, int32_t * p_6, const uint16_t  p_7, uint8_t  p_8, int16_t  p_9);
static int32_t  func_11(uint64_t  p_12);

static int32_t  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t *l_10[4];
    int64_t l_113[1][3];
    int16_t l_135[2][2] = {{0x0F15,0x0F15},{0x0F15,0x0F15}};
    const uint64_t *l_149 = &g_32;
    const uint64_t **l_148 = &l_149;
    int i, j;
    for (i = 0; i < 4; i++)
        l_10[i] = (void*)0;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_113[i][j] = (-2);
    }
    (*l_2) = 0;
    if (((*l_2) < ((((func_4((l_10[1] != (void*)0), l_2, g_3, (*l_2), g_3) ^ 0x4620FE39) && 9) <= g_46) , g_43[0])))
    { 
        uint64_t **l_100 = (void*)0;
        int32_t l_101[2];
        int i;
        for (i = 0; i < 2; i++)
            l_101[i] = 0xB607E664;
        if ((safe_sub_func_int16_t_s_s((safe_sub_func_uint32_t_u_u((g_43[0] | (!g_38)), (safe_rshift_func_int64_t_s_u((safe_rshift_func_int32_t_s_u(((l_100 == &g_31) , l_101[1]), 13)), 28)))), g_47[3])))
        { 
            for (g_46 = 0; (g_46 > 20); ++g_46)
            { 
                uint64_t l_104 = 0xD7867E7C57241C05LL;
                return l_104;
            }
        }
        else
        { 
            l_101[0] ^= ((*l_2) = (safe_lshift_func_uint64_t_u_u((*g_31), 32)));
        }
        (*l_2) = ((safe_sub_func_uint32_t_u_u(0x33A36175, (g_44 | ((safe_lshift_func_int64_t_s_u((*l_2), 35)) , g_47[3])))) <= g_3);
    }
    else
    { 
        uint16_t l_120[1][3];
        uint16_t *l_134 = &l_120[0][0];
        int8_t l_136 = 0x85;
        uint8_t l_142 = 0x3F;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_120[i][j] = 0x5312;
        }
        for (g_44 = (-7); (g_44 < 28); g_44 = safe_add_func_int32_t_s_s(g_44, 7))
        { 
            return l_113[0][1];
        }
        for (g_3 = 0; (g_3 >= 0); g_3 -= 1)
        { 
            int32_t l_118 = 0x5914D98E;
            const int64_t l_122 = 0;
            int8_t *l_126 = &g_46;
            int64_t l_137[1];
            int i;
            for (i = 0; i < 1; i++)
                l_137[i] = 0x7ABECF34627D408DLL;
            if (((g_43[(g_3 + 2)] = ((safe_sub_func_uint64_t_u_u((((safe_add_func_uint64_t_u_u(((*g_31) = (*g_31)), (g_41 , g_47[3]))) | 0x8534) | 0xD8), 0xDDAFFDA28E2A20ABLL)) , g_3)) == l_118))
            { 
                int32_t **l_119 = &l_10[1];
                int16_t *l_121 = &g_43[(g_3 + 2)];
                int i;
                (*l_119) = l_10[g_3];
                l_118 = (l_120[0][0] == ((*l_121) ^= ((*g_31) , ((g_45 , 0) || 0xB59B))));
            }
            else
            { 
                g_44 &= l_122;
            }
            if (l_120[0][0])
                continue;
            g_38 = (~(safe_sub_func_int8_t_s_s((((*l_126) = (-6)) & (!(l_118 &= (safe_sub_func_uint64_t_u_u(0xD9A5CC0B3D1FD5F4LL, ((((safe_add_func_uint64_t_u_u((safe_sub_func_int64_t_s_s(((*g_85) == l_134), l_120[0][0])), g_47[0])) > l_135[1][1]) & 0xA677) ^ l_136)))))), 0xBB)));
            for (g_32 = 0; (g_32 <= 0); g_32 += 1)
            { 
                int64_t l_138 = 0xCAE1E1C95C5D00D0LL;
                uint8_t l_139 = 0x6E;
                --l_139;
                l_142++;
            }
        }
    }
    for (g_44 = (-27); (g_44 >= 28); ++g_44)
    { 
        for (g_32 = 0; (g_32 <= 3); g_32 += 1)
        { 
            uint64_t **l_147 = &g_31;
            int i;
            l_10[g_32] = l_10[g_32];
            (*l_2) = (l_147 != l_148);
            g_41 |= 0x9184E9DB;
        }
    }
    return (*l_2);
}

static int32_t  func_4(int32_t  p_5, int32_t * p_6, const uint16_t  p_7, uint8_t  p_8, int16_t  p_9)
{ 
    int8_t l_15 = 0x85;
    int32_t *l_22[4][4];
    int32_t **l_23[1][4];
    uint64_t l_24 = 0x8CBDB62B2FAA9634LL;
    uint64_t *l_25 = (void*)0;
    uint64_t *l_26[4] = {&l_24,&l_24,&l_24,&l_24};
    const int32_t l_72 = 5;
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            l_22[i][j] = &g_3;
    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_23[i][j] = &l_22[2][2];
    }
    p_5 = func_11((g_27 = ((safe_add_func_uint16_t_u_u(l_15, (p_9 > (((safe_sub_func_uint64_t_u_u((safe_add_func_int16_t_s_s(((((safe_rshift_func_int16_t_s_u(((((p_6 = l_22[1][3]) != (void*)0) ^ g_3) == 0x44), p_9)) == 0x6AF9) , g_3) , p_9), 3U)), p_7)) >= p_8) == p_9)))) != l_24)));
    for (p_8 = (-16); (p_8 > 18); p_8 = safe_add_func_int32_t_s_s(p_8, 1))
    { 
        int8_t l_53 = 0xA1;
        l_53 ^= (~g_44);
        g_54 = &p_5;
        (*g_54) |= (0x1338462C7DCB2139LL || p_7);
    }
    for (g_41 = (-2); (g_41 < (-18)); g_41 = safe_sub_func_uint8_t_u_u(g_41, 1))
    { 
        const uint8_t l_59[2][3] = {{1U,1U,1U},{0x4A,0x4A,0x4A}};
        int32_t l_61 = (-4);
        int32_t **l_74[1][2];
        uint64_t **l_81 = &l_25;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_74[i][j] = &g_54;
        }
    }
    return (*p_6);
}

static int32_t  func_11(uint64_t  p_12)
{ 
    int32_t *l_28 = &g_3;
    uint64_t **l_33 = &g_31;
    int32_t *l_36 = (void*)0;
    int32_t *l_37 = &g_38;
    int32_t *l_39 = &g_38;
    int32_t *l_40[1][3];
    int16_t l_42 = 0xCC13;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_40[i][j] = &g_38;
    }
    l_28 = l_28;
    (*l_37) |= (safe_rshift_func_int64_t_s_u(((((((*l_33) = g_31) == &g_32) | (safe_lshift_func_uint64_t_u_u((5U & 1), (*l_28)))) > 0x19) | 1U), 22));
    --g_47[3];
    return p_12;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_43[i], "g_43[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_47[i], "g_47[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
