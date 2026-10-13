/* Csmith 0cdc710315cf seed 270, flattened by scripts/csmith_flatten.py */
// Options:   --seed 270 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
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
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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

struct S0 {
   int8_t  f0;
   uint8_t  f1;
   const uint16_t  f2;
   int64_t  f3;
};

struct S1 {
   int16_t  f0;
   const struct S0  f1;
   const uint8_t  f2;
};

struct S2 {
   struct S0  f0;
   int8_t  f1;
};

static uint64_t g_2 = 0x99FB023ED15DE05DLL;
static int32_t g_6 = 1;
static struct S2 g_18 = {{0x29,0xF4,0x0426,0},0x9F};
static struct S1 g_20 = {0xB8FA,{8,0xB8,65535U,-5},0x5B};
static struct S1 *g_19 = &g_20;
static int8_t g_39[1][2] = {{(-1),(-1)}};
static uint16_t g_41 = 0x3CC8;
static int16_t g_49 = 0x8509;
static uint8_t g_53 = 0xA0;
static int32_t g_63 = 0x00AF4084;
static uint32_t g_69 = 4294967295U;
static const uint8_t g_91 = 0x7A;
static const uint8_t *g_90 = &g_91;
static int16_t g_93[2] = {1,1};
static int64_t g_94 = 0xDB2349D53036CC05LL;
static uint64_t g_95 = 0xAECBAC114349BA41LL;
static int32_t *g_106 = &g_6;
static struct S1 g_123 = {0x13D8,{0xF8,0x03,0x6701,1},0x5A};
static struct S1 *g_122 = &g_123;

static int32_t  func_1(void);
static struct S1 * func_7(int32_t * p_8, uint8_t  p_9, int32_t * p_10, int64_t  p_11, const struct S1 * const  p_12);
static uint32_t  func_14(struct S2  p_15, struct S1 * p_16, int32_t * p_17);

static int32_t  func_1(void)
{ 
    int32_t *l_5 = &g_6;
    uint32_t l_109[1][1];
    const struct S1 * const l_110 = (void*)0;
    struct S1 **l_121[1][2];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_109[i][j] = 0xA7570BAE;
    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_121[i][j] = &g_19;
    }
    g_2++;
    (*l_5) = (-1);
    g_122 = func_7(&g_6, (((~func_14(g_18, g_19, l_5)) , (void*)0) == (void*)0), l_5, l_109[0][0], l_110);
    return (*l_5);
}

static struct S1 * func_7(int32_t * p_8, uint8_t  p_9, int32_t * p_10, int64_t  p_11, const struct S1 * const  p_12)
{ 
    int8_t l_111 = 0;
    struct S1 *l_120 = &g_20;
    l_111 = 0x703095B6;
    for (g_20.f0 = 0; (g_20.f0 == 23); g_20.f0++)
    { 
        uint32_t *l_119 = &g_69;
        for (g_49 = (-17); (g_49 <= 21); g_49++)
        { 
            int32_t *l_116 = &g_6;
            l_116 = p_8;
            g_63 &= (*l_116);
        }
        (*p_8) |= (safe_sub_func_uint16_t_u_u((((*l_119) = g_41) <= 1U), p_9));
    }
    (*g_106) = l_111;
    return l_120;
}

static uint32_t  func_14(struct S2  p_15, struct S1 * p_16, int32_t * p_17)
{ 
    uint64_t l_21[3];
    int32_t **l_23 = (void*)0;
    int32_t *l_24 = &g_6;
    int32_t l_57 = 4;
    int32_t l_68[1];
    int16_t l_100 = 0xFDE2;
    int32_t *l_108 = (void*)0;
    int i;
    for (i = 0; i < 3; i++)
        l_21[i] = 5U;
    for (i = 0; i < 1; i++)
        l_68[i] = (-7);
    for (g_18.f0.f1 = 0; (g_18.f0.f1 <= 2); g_18.f0.f1 += 1)
    { 
        int64_t l_22 = 0xE4DE5A82A6F944DCLL;
        int i;
        (*p_17) = ((l_21[g_18.f0.f1] , p_15.f0.f3) <= g_20.f1.f1);
        return l_22;
    }
    l_24 = p_17;
    if (g_20.f1.f3)
    { 
        uint16_t *l_40[2][4] = {{&g_41,&g_41,&g_41,&g_41},{&g_41,&g_41,&g_41,&g_41}};
        int32_t l_42 = 0x9960698B;
        int i, j;
        (*l_24) = (safe_sub_func_uint8_t_u_u((safe_lshift_func_int32_t_s_s((!(p_15.f0.f3 ^= (-7))), 13)), (safe_rshift_func_int64_t_s_u(((safe_add_func_uint16_t_u_u((l_42 &= (g_39[0][1] ^= ((((g_6 & ((safe_mul_func_uint64_t_u_u((((~(safe_div_func_uint8_t_u_u(p_15.f1, g_18.f0.f1))) ^ g_18.f1) <= g_18.f0.f1), 0x082ADE89B93B3BBBLL)) < (-1))) <= g_20.f1.f3) || 0xA7) <= p_15.f0.f2))), g_18.f0.f2)) , 0), g_18.f0.f2))));
    }
    else
    { 
        int32_t **l_43 = &l_24;
        int32_t l_66[3][2];
        uint64_t l_101[4] = {0xDAFCE28766912E21LL,0xDAFCE28766912E21LL,0xDAFCE28766912E21LL,0xDAFCE28766912E21LL};
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 2; j++)
                l_66[i][j] = 0x66072949;
        }
        (*l_43) = &g_6;
        if ((safe_rshift_func_uint64_t_u_s(9U, 52)))
        { 
            int32_t *l_54 = &g_6;
            int32_t *l_55 = &g_6;
            int32_t *l_56 = (void*)0;
            int32_t *l_58 = &l_57;
            int32_t *l_59 = &l_57;
            int32_t *l_60 = &g_6;
            int32_t *l_61 = &l_57;
            int32_t *l_62 = &l_57;
            int32_t *l_64 = &l_57;
            int32_t *l_65 = &g_6;
            int32_t *l_67[4];
            uint8_t *l_87 = &g_53;
            int i;
            for (i = 0; i < 4; i++)
                l_67[i] = &l_57;
            for (g_6 = 0; (g_6 <= 22); g_6++)
            { 
                struct S1 **l_48 = &g_19;
                int32_t **l_52 = (void*)0;
                g_49 &= (p_15.f0.f2 == (((*l_48) = (void*)0) != (void*)0));
                g_53 &= ((*p_16) , (safe_mod_func_int16_t_s_s(((&p_17 == (l_52 = (void*)0)) || (-8)), p_15.f0.f3)));
                if ((*p_17))
                    continue;
            }
            ++g_69;
            for (p_15.f0.f0 = 1; (p_15.f0.f0 >= 0); p_15.f0.f0 -= 1)
            { 
                uint8_t *l_75 = &g_18.f0.f1;
                uint8_t *l_78[4][1] = {{&g_53},{&g_53},{&g_53},{&g_53}};
                const uint8_t *l_89 = &g_20.f2;
                const uint8_t **l_88[2];
                int i, j;
                for (i = 0; i < 2; i++)
                    l_88[i] = &l_89;
                (*l_64) = (safe_rshift_func_int8_t_s_s(((~(p_15.f0.f1 = (++(*l_75)))) & ((safe_lshift_func_int32_t_s_u(((**l_43) = (safe_add_func_uint32_t_u_u((safe_sub_func_int32_t_s_s(((((safe_div_func_uint16_t_u_u((((*l_24) > ((l_87 != (g_90 = l_87)) >= (**l_43))) || 0U), p_15.f0.f2)) != g_6) || g_20.f1.f3) && p_15.f0.f0), p_15.f1)), g_18.f0.f2))), 19)) <= g_20.f2)), p_15.f0.f0));
            }
        }
        else
        { 
            int32_t *l_92[3];
            int i;
            for (i = 0; i < 3; i++)
                l_92[i] = &g_6;
            g_95++;
            for (g_2 = 0; (g_2 <= 15); g_2 = safe_add_func_int8_t_s_s(g_2, 4))
            { 
                int32_t **l_107[3][1];
                int i, j;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_107[i][j] = &l_92[2];
                }
                l_101[1]++;
                g_63 &= (safe_rshift_func_uint64_t_u_u(((g_106 = &g_63) == (l_108 = ((*l_43) = p_17))), 54));
            }
            g_63 |= (**l_43);
        }
    }
    return g_18.f0.f1;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_18.f0.f0, "g_18.f0.f0", print_hash_value);
    transparent_crc(g_18.f0.f1, "g_18.f0.f1", print_hash_value);
    transparent_crc(g_18.f0.f2, "g_18.f0.f2", print_hash_value);
    transparent_crc(g_18.f0.f3, "g_18.f0.f3", print_hash_value);
    transparent_crc(g_18.f1, "g_18.f1", print_hash_value);
    transparent_crc(g_20.f0, "g_20.f0", print_hash_value);
    transparent_crc(g_20.f1.f0, "g_20.f1.f0", print_hash_value);
    transparent_crc(g_20.f1.f1, "g_20.f1.f1", print_hash_value);
    transparent_crc(g_20.f1.f2, "g_20.f1.f2", print_hash_value);
    transparent_crc(g_20.f1.f3, "g_20.f1.f3", print_hash_value);
    transparent_crc(g_20.f2, "g_20.f2", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_39[i][j], "g_39[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_91, "g_91", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_93[i], "g_93[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_94, "g_94", print_hash_value);
    transparent_crc(g_95, "g_95", print_hash_value);
    transparent_crc(g_123.f0, "g_123.f0", print_hash_value);
    transparent_crc(g_123.f1.f0, "g_123.f1.f0", print_hash_value);
    transparent_crc(g_123.f1.f1, "g_123.f1.f1", print_hash_value);
    transparent_crc(g_123.f1.f2, "g_123.f1.f2", print_hash_value);
    transparent_crc(g_123.f1.f3, "g_123.f1.f3", print_hash_value);
    transparent_crc(g_123.f2, "g_123.f2", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
