/* Csmith 0cdc710315cf seed 46, flattened by scripts/csmith_flatten.py */
// Options:   --seed 46 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
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
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint64_t  f0;
   uint32_t  f1;
   const uint16_t  f2;
   const int64_t  f3;
};

struct S1 {
   int8_t  f0;
   struct S0  f1;
   uint16_t  f2;
};

struct S2 {
   int32_t  f0;
   uint64_t  f1;
   uint8_t  f2;
};

static int32_t g_2 = 2;
static int32_t *g_15 = (void*)0;
static int32_t g_17 = (-2);
static int32_t *g_16 = &g_17;
static const struct S2 g_26 = {0xEF05DE08,0xADE89B93B3BBBB53LL,0xEA};
static struct S2 g_28 = {6,1U,0x14};
static struct S1 g_35 = {0x35,{0xA2C03FDDA78404D0LL,0x3830386B,0xBDD1,0x92F88B3176A17913LL},0xA513};
static struct S0 g_37 = {0x3C3A871D1BA05761LL,0x9EFA4ED9,65528U,0x4A05A9EE2B407435LL};
static struct S0 *g_36[3] = {&g_37,&g_37,&g_37};
static uint32_t g_41[3] = {0U,0U,0U};
static int16_t g_45 = 0;
static struct S0 g_59 = {0xC5FD5384E059069CLL,1U,0xD28B,1};

static int32_t  func_1(void);
static struct S1  func_5(struct S0 * p_6, struct S0 * p_7, struct S0 * p_8);
static struct S0 * func_9(struct S0 * p_10);

static int32_t  func_1(void)
{ 
    struct S0 *l_11 = (void*)0;
    int32_t l_75 = (-1);
    int32_t l_81 = 0x986B1ECD;
    int32_t l_82 = 5;
    int8_t l_83 = 1;
    int32_t l_84[3][4] = {{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)}};
    int i, j;
    for (g_2 = 0; (g_2 != 11); g_2++)
    { 
        const struct S0 *l_72 = &g_59;
        const struct S0 **l_73 = &l_72;
        (*l_73) = (func_5(func_9(l_11), (g_35 , (void*)0), g_36[0]) , l_72);
    }
    for (g_37.f0 = 0; (g_37.f0 <= 2); g_37.f0 += 1)
    { 
        int32_t *l_74 = &g_2;
        int32_t *l_76 = &g_17;
        int32_t *l_77 = (void*)0;
        int32_t *l_78 = &l_75;
        int32_t *l_79 = &g_2;
        int32_t *l_80[2];
        uint64_t l_85 = 0x65289450AD34F8EFLL;
        int i;
        for (i = 0; i < 2; i++)
            l_80[i] = &g_2;
        l_85++;
        g_36[0] = &g_37;
    }
    return l_84[0][3];
}

static struct S1  func_5(struct S0 * p_6, struct S0 * p_7, struct S0 * p_8)
{ 
    int32_t l_38 = (-10);
    int32_t *l_39 = &l_38;
    int32_t **l_40 = &g_16;
    int16_t *l_44 = &g_45;
    struct S2 *l_47 = &g_28;
    struct S2 **l_46 = &l_47;
    struct S1 *l_50 = (void*)0;
    struct S1 l_63 = {0,{18446744073709551609U,8U,0xCFA8,-6},0xEE99};
    uint32_t l_70[2][1];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_70[i][j] = 0U;
    }
    (*l_39) = (l_38 < 0xBAD6);
    (*l_40) = (void*)0;
    if (((((((((((g_41[1] = (*l_39)) >= ((*l_44) = (safe_lshift_func_int32_t_s_s((*l_39), 12)))) , 0x316A37A54743E03ELL) , (*l_39)) , l_46) == &l_47) & (*l_39)) < g_35.f1.f2) && 0x90) >= (*l_39)))
    { 
        return g_35;
    }
    else
    { 
        struct S1 *l_48[1];
        struct S1 **l_49 = (void*)0;
        const int32_t l_53 = 0xDC662BFF;
        struct S0 *l_58 = &g_59;
        struct S2 **l_71 = &l_47;
        int i;
        for (i = 0; i < 1; i++)
            l_48[i] = &g_35;
        l_50 = l_48[0];
        if (((safe_mul_func_int8_t_s_s(g_28.f2, l_53)) && (g_28.f2 |= (!(safe_mod_func_int32_t_s_s(((g_35.f1.f1 && 0) > l_53), (*l_39)))))))
        { 
            struct S0 **l_57 = (void*)0;
            l_58 = p_6;
        }
        else
        { 
            uint8_t l_60 = 2U;
            (*l_39) &= (l_60 == (safe_lshift_func_uint32_t_u_u((l_63 , l_53), 12)));
            (*l_39) |= (((g_28.f1 != (((safe_div_func_int8_t_s_s(g_35.f2, (safe_mul_func_uint16_t_u_u((safe_div_func_uint16_t_u_u(65535U, l_53)), l_53)))) || l_53) , g_26.f0)) || l_70[0][0]) < (*g_15));
            l_46 = l_71;
        }
    }
    return g_35;
}

static struct S0 * func_9(struct S0 * p_10)
{ 
    struct S1 l_12 = {0,{0x8F4A8FE24307A329LL,0xF4A0426A,0U,0xFA85F88E10E5E534LL},0x4ABC};
    int32_t *l_13 = &g_2;
    int32_t **l_14[1][2];
    struct S2 *l_29 = &g_28;
    struct S0 *l_34 = &l_12.f1;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_14[i][j] = &l_13;
    }
    g_16 = (l_12 , (g_15 = l_13));
    if ((safe_add_func_int32_t_s_s((safe_mod_func_int8_t_s_s((((safe_mul_func_uint64_t_u_u(0xB8CE22955D482642LL, ((void*)0 != &g_15))) && g_2) < g_2), (*l_13))), 4)))
    { 
        struct S2 *l_30 = (void*)0;
        for (g_17 = (-18); (g_17 > 7); g_17 = safe_add_func_int64_t_s_s(g_17, 5))
        { 
            struct S2 *l_27 = &g_28;
            (*l_27) = g_26;
            l_30 = l_29;
            for (g_28.f1 = 0; (g_28.f1 < 29); g_28.f1 = safe_add_func_uint64_t_u_u(g_28.f1, 6))
            { 
                return p_10;
            }
        }
    }
    else
    { 
        struct S2 **l_33[1][4];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 4; j++)
                l_33[i][j] = &l_29;
        }
        l_29 = l_29;
    }
    return p_10;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_26.f0, "g_26.f0", print_hash_value);
    transparent_crc(g_26.f1, "g_26.f1", print_hash_value);
    transparent_crc(g_26.f2, "g_26.f2", print_hash_value);
    transparent_crc(g_28.f0, "g_28.f0", print_hash_value);
    transparent_crc(g_28.f1, "g_28.f1", print_hash_value);
    transparent_crc(g_28.f2, "g_28.f2", print_hash_value);
    transparent_crc(g_35.f0, "g_35.f0", print_hash_value);
    transparent_crc(g_35.f1.f0, "g_35.f1.f0", print_hash_value);
    transparent_crc(g_35.f1.f1, "g_35.f1.f1", print_hash_value);
    transparent_crc(g_35.f1.f2, "g_35.f1.f2", print_hash_value);
    transparent_crc(g_35.f1.f3, "g_35.f1.f3", print_hash_value);
    transparent_crc(g_35.f2, "g_35.f2", print_hash_value);
    transparent_crc(g_37.f0, "g_37.f0", print_hash_value);
    transparent_crc(g_37.f1, "g_37.f1", print_hash_value);
    transparent_crc(g_37.f2, "g_37.f2", print_hash_value);
    transparent_crc(g_37.f3, "g_37.f3", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_41[i], "g_41[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_59.f0, "g_59.f0", print_hash_value);
    transparent_crc(g_59.f1, "g_59.f1", print_hash_value);
    transparent_crc(g_59.f2, "g_59.f2", print_hash_value);
    transparent_crc(g_59.f3, "g_59.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
