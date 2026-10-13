/* Csmith 0cdc710315cf seed 256, flattened by scripts/csmith_flatten.py */
// Options:   --seed 256 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S1 {
   int16_t  f0;
   int16_t  f1;
};

struct S2 {
   int64_t  f0;
   int32_t  f1;
   const int16_t  f2;
};

union U4 {
   int64_t  f0;
   uint64_t  f1;
   int32_t  f2;
};

static int32_t g_3 = 0x50707DFE;
static struct S2 g_14 = {0xAE974CA779041289LL,0xFF5452FA,0x14B1};
static struct S1 g_15 = {-1,1};
static uint32_t g_26 = 0x699FF93F;
static uint32_t *g_31 = &g_26;
static uint32_t ** const g_30[4][1] = {{&g_31},{&g_31},{&g_31},{&g_31}};
static uint32_t g_37 = 0x7DFE98D7;
static uint64_t g_48 = 0x538E5EFB48CA1818LL;
static union U4 g_50 = {0x7FF9BDEFAB77C19CLL};
static int16_t g_62 = 0x9333;
static uint16_t g_63 = 65535U;
static struct S2 g_69 = {0x016C6A45D3122576LL,0xC99C2354,0x0A20};
static struct S2 *g_68[4] = {&g_69,&g_69,&g_69,&g_69};
static int8_t g_77 = (-1);
static int32_t g_78 = 0;
static int16_t g_79 = (-8);
static uint64_t g_81 = 18446744073709551615U;
static uint32_t g_85 = 0x4FC3DD16;

static int32_t  func_1(void);
static int8_t  func_4(int16_t  p_5, union U4  p_6);
static int32_t  func_9(struct S2  p_10, struct S2  p_11, const uint32_t  p_12, struct S1  p_13);

static int32_t  func_1(void)
{ 
    uint16_t l_2[1][1];
    uint64_t l_72[2][1];
    int32_t l_75 = 0xDD72F777;
    int32_t l_76[2][3];
    int32_t l_80[2][3] = {{(-5),0x19839B2F,0x19839B2F},{(-5),0x19839B2F,0x19839B2F}};
    union U4 *l_86 = &g_50;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_2[i][j] = 0U;
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_72[i][j] = 0xE010D33A1293870BLL;
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
            l_76[i][j] = 0xA8392DDE;
    }
    for (g_3 = 0; (g_3 <= 0); g_3 += 1)
    { 
        int8_t l_45[2];
        uint64_t *l_46 = (void*)0;
        uint64_t *l_47[2][2];
        int32_t l_49 = 0x1C2AA0BD;
        union U4 l_56 = {0x9D3F58F29ADBE56FLL};
        int i, j;
        for (i = 0; i < 2; i++)
            l_45[i] = 0x9C;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
                l_47[i][j] = &g_48;
        }
        if (((g_3 , ((func_4(((safe_rshift_func_uint64_t_u_s((l_49 = (((func_9(g_14, g_14, g_3, g_15) > l_2[0][0]) <= 0xE9347A64) < l_45[0])), 28)) ^ l_2[0][0]), g_50) != l_2[0][0]) & l_2[0][0])) <= l_45[0]))
        { 
            for (g_48 = 0; (g_48 <= 0); g_48 += 1)
            { 
                uint32_t **l_57 = &g_31;
                int32_t *l_58 = (void*)0;
                int32_t *l_59 = (void*)0;
                int32_t *l_60 = &l_49;
                int32_t *l_61[2][3] = {{(void*)0,&g_3,&g_3},{(void*)0,&g_3,&g_3}};
                int i, j;
                (*l_60) ^= (safe_div_func_int8_t_s_s(((l_56 , ((*g_31) = l_2[g_48][g_48])) == (&g_31 != l_57)), 246U));
                ++g_63;
            }
        }
        else
        { 
            struct S2 *l_67 = &g_14;
            struct S2 **l_66[4] = {&l_67,&l_67,&l_67,&l_67};
            int i;
            g_68[1] = (void*)0;
        }
        if (l_45[0])
            continue;
        g_69.f1 &= ((safe_add_func_int64_t_s_s((l_72[1][0] < 0xFFB2), l_49)) | l_45[0]);
        for (g_63 = 0; (g_63 <= 0); g_63 += 1)
        { 
            int32_t *l_73 = (void*)0;
            int32_t *l_74[3][1];
            struct S1 l_84 = {0x3DA4,-7};
            union U4 **l_87 = &l_86;
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_74[i][j] = &l_49;
            }
            ++g_81;
            l_80[1][2] &= (((g_85 = (l_84 , (-1))) ^ (g_78 = (0 || l_2[g_63][g_3]))) | 0U);
            (*l_87) = l_86;
        }
    }
    return l_72[1][0];
}

static int8_t  func_4(int16_t  p_5, union U4  p_6)
{ 
    int32_t *l_51 = &g_3;
    int32_t **l_52 = (void*)0;
    int32_t **l_53 = &l_51;
    (*l_53) = l_51;
    return g_3;
}

static int32_t  func_9(struct S2  p_10, struct S2  p_11, const uint32_t  p_12, struct S1  p_13)
{ 
    uint64_t l_16 = 0xBF3B8CFF26AF9031LL;
    uint32_t *l_25[2];
    int32_t l_35 = 5;
    int32_t l_36 = 6;
    int32_t *l_44 = &l_35;
    int i;
    for (i = 0; i < 2; i++)
        l_25[i] = &g_26;
    if ((l_16 ^ ((g_26 |= ((safe_rshift_func_int16_t_s_s((safe_div_func_int16_t_s_s((safe_rshift_func_int32_t_s_u(((g_14.f0 = p_10.f0) | ((safe_mod_func_uint64_t_u_u(p_13.f1, g_3)) && 0x46F2633933B839BFLL)), g_14.f1)), 0x90B2)), 1)) | p_10.f2)) <= 0xFBC01D4C)))
    { 
        uint8_t l_29 = 0xC8;
        uint32_t **l_32 = (void*)0;
        int32_t *l_33[1];
        int i;
        for (i = 0; i < 1; i++)
            l_33[i] = &g_14.f1;
        for (g_14.f0 = 9; (g_14.f0 < 12); g_14.f0 = safe_add_func_int32_t_s_s(g_14.f0, 7))
        { 
            if (l_29)
                break;
            l_32 = g_30[2][0];
        }
        g_14.f1 |= (&p_12 != (void*)0);
    }
    else
    { 
        int32_t *l_34[4];
        int i;
        for (i = 0; i < 4; i++)
            l_34[i] = &g_14.f1;
        g_37--;
    }
    (*l_44) &= (safe_rshift_func_int64_t_s_u((0xA458 ^ (safe_sub_func_uint16_t_u_u(((p_13 , ((g_37 | g_14.f2) < g_37)) && p_10.f0), g_14.f1))), g_37));
    return p_11.f1;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_14.f0, "g_14.f0", print_hash_value);
    transparent_crc(g_14.f1, "g_14.f1", print_hash_value);
    transparent_crc(g_14.f2, "g_14.f2", print_hash_value);
    transparent_crc(g_15.f0, "g_15.f0", print_hash_value);
    transparent_crc(g_15.f1, "g_15.f1", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_50.f0, "g_50.f0", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_69.f0, "g_69.f0", print_hash_value);
    transparent_crc(g_69.f1, "g_69.f1", print_hash_value);
    transparent_crc(g_69.f2, "g_69.f2", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_78, "g_78", print_hash_value);
    transparent_crc(g_79, "g_79", print_hash_value);
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
