/* Csmith 0cdc710315cf seed 51, flattened by scripts/csmith_flatten.py */
// Options:   --seed 51 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
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
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
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
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int16_t  f0;
   uint16_t  f1;
   int64_t  f2;
};

struct S1 {
   uint8_t  f0;
   int64_t  f1;
};

struct S2 {
   int64_t  f0;
   int32_t  f1;
   const uint32_t  f2;
   int32_t  f3;
};

static int16_t g_6 = 0;
static int32_t g_24 = (-3);
static uint64_t g_37 = 0U;
static const int32_t g_45 = 0;
static const int32_t *g_46 = &g_24;
static uint16_t g_59 = 1U;
static struct S0 g_63 = {-1,0x1268,0x9A6E4C1C17FA6FBALL};
static struct S0 *g_65 = &g_63;
static uint8_t g_66[1] = {0x22};
static struct S1 g_67 = {247U,0xAE8BCD06DB255963LL};
static int8_t g_79[4][1] = {{0xCF},{(-2)},{0xCF},{(-2)}};
static uint8_t g_80 = 0x9C;
static struct S2 g_88 = {0x0085190AC52CF448LL,0x36471EDB,0xDF84107A,1};
static int8_t g_107 = (-1);
static int16_t g_108 = 0x111D;
static uint64_t g_109 = 5U;

static struct S0  func_1(void);
static uint8_t  func_2(int64_t  p_3, uint16_t  p_4, int16_t  p_5);
static uint16_t  func_8(struct S1  p_9, int16_t  p_10);

static struct S0  func_1(void)
{ 
    uint8_t l_7 = 1U;
    int32_t l_76 = 0x4008F4F8;
    int32_t l_78 = 0;
    struct S0 l_87 = {3,1U,0};
    int32_t *l_98 = &g_24;
    int32_t *l_99 = &l_78;
    int32_t *l_100 = &g_88.f3;
    int32_t *l_101 = &l_76;
    int32_t *l_102 = &g_88.f3;
    int32_t *l_103 = (void*)0;
    int32_t *l_104 = &g_88.f1;
    int32_t *l_105 = &l_78;
    int32_t *l_106[3][2];
    int32_t *l_112 = &l_78;
    int32_t **l_113 = &l_105;
    struct S0 l_114 = {-1,0x9173,1};
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_106[i][j] = (void*)0;
    }
    if (((247U <= func_2((0x7E08A110 & 0xAA96141E), g_6, l_7)) == g_6))
    { 
        int32_t *l_71 = &g_24;
        int32_t *l_72 = &g_24;
        int32_t *l_73 = &g_24;
        int32_t *l_74 = &g_24;
        int32_t *l_75 = &g_24;
        int32_t *l_77[1][2];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_77[i][j] = &g_24;
        }
lbl_97:
        for (l_7 = 27; (l_7 > 19); l_7--)
        { 
            struct S0 *l_64 = &g_63;
            (*l_64) = g_63;
            g_65 = l_64;
            for (g_63.f1 = 0; (g_63.f1 <= 0); g_63.f1 += 1)
            { 
                struct S1 *l_68[3][2] = {{&g_67,&g_67},{&g_67,&g_67},{&g_67,&g_67}};
                int32_t *l_69 = (void*)0;
                int32_t *l_70 = &g_24;
                int i, j;
                g_67 = g_67;
                (*l_70) = l_7;
            }
        }
        g_80++;
        for (g_67.f1 = 0; (g_67.f1 < (-10)); --g_67.f1)
        { 
            const struct S1 l_93 = {0x47,0x33B2D9701DEE0D32LL};
            if (((+(!((l_87 , (void*)0) == (g_88 , (void*)0)))) == l_78))
            { 
                uint32_t l_89[1][1];
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_89[i][j] = 0xD2F7AF16;
                }
                l_89[0][0] |= (*g_46);
            }
            else
            { 
                const int64_t l_96 = 0xAD0AF0966AB14209LL;
                (*l_74) = (safe_mul_func_int32_t_s_s((~g_67.f0), (l_93 , (safe_rshift_func_int64_t_s_u(l_96, g_24)))));
                if (g_67.f0)
                    goto lbl_97;
            }
        }
    }
    else
    { 
        (*g_65) = l_87;
    }
    ++g_109;
    (*l_113) = l_112;
    return l_114;
}

static uint8_t  func_2(int64_t  p_3, uint16_t  p_4, int16_t  p_5)
{ 
    struct S1 l_11 = {0U,1};
    int32_t l_14 = (-2);
    const int32_t *l_44 = &g_45;
    uint64_t * const l_52 = (void*)0;
    int32_t *l_56 = &g_24;
    if (((func_8(l_11, (safe_mod_func_int32_t_s_s(l_11.f1, (l_14 &= ((((g_6 == p_5) < g_6) ^ p_4) | 0x6091))))) | p_5) | 0xBD))
    { 
        struct S1 l_40 = {0x35,-1};
        const int32_t *l_42 = (void*)0;
        uint64_t *l_51 = (void*)0;
        int32_t *l_53 = &l_14;
        for (p_5 = 0; (p_5 <= (-2)); p_5 = safe_sub_func_uint64_t_u_u(p_5, 1))
        { 
            uint64_t *l_35 = (void*)0;
            uint64_t *l_36 = &g_37;
            int32_t * const l_41 = &g_24;
            const int32_t **l_43[2][1];
            struct S1 *l_47[3][2];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_43[i][j] = &l_42;
            }
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 2; j++)
                    l_47[i][j] = &l_40;
            }
            g_46 = ((safe_lshift_func_uint64_t_u_s(((safe_rshift_func_int8_t_s_s((safe_lshift_func_uint8_t_u_u((safe_mod_func_int32_t_s_s(((((*l_36)++) , l_40) , ((l_41 != (l_44 = l_42)) , 0x1B1996FC)), p_3)), 2)), g_24)) , (*l_41)), p_3)) , &g_45);
            l_40 = l_40;
        }
        (*l_53) = (((-1) > (safe_add_func_uint16_t_u_u(l_14, (!(((l_51 != l_52) <= 0x2A30FB9C0D287709LL) & 0xBE6A916C6C9A4A91LL))))) , 1);
    }
    else
    { 
        int32_t *l_57 = &g_24;
        uint16_t *l_58[1];
        int32_t *l_60 = &l_14;
        int i;
        for (i = 0; i < 1; i++)
            l_58[i] = &g_59;
        (*l_60) ^= ((safe_sub_func_uint16_t_u_u((g_59 = (l_56 == l_57)), ((*l_44) && (*g_46)))) >= g_45);
    }
    return g_59;
}

static uint16_t  func_8(struct S1  p_9, int16_t  p_10)
{ 
    uint16_t l_15 = 1U;
    l_15--;
    g_24 = ((safe_add_func_int32_t_s_s(l_15, (safe_mul_func_uint8_t_u_u((safe_lshift_func_uint64_t_u_u((l_15 && (((g_6 >= 0U) | 0) < l_15)), p_9.f1)), g_6)))) && p_9.f1);
    return g_6;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_63.f0, "g_63.f0", print_hash_value);
    transparent_crc(g_63.f1, "g_63.f1", print_hash_value);
    transparent_crc(g_63.f2, "g_63.f2", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_66[i], "g_66[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_67.f0, "g_67.f0", print_hash_value);
    transparent_crc(g_67.f1, "g_67.f1", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_79[i][j], "g_79[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_80, "g_80", print_hash_value);
    transparent_crc(g_88.f0, "g_88.f0", print_hash_value);
    transparent_crc(g_88.f1, "g_88.f1", print_hash_value);
    transparent_crc(g_88.f2, "g_88.f2", print_hash_value);
    transparent_crc(g_88.f3, "g_88.f3", print_hash_value);
    transparent_crc(g_107, "g_107", print_hash_value);
    transparent_crc(g_108, "g_108", print_hash_value);
    transparent_crc(g_109, "g_109", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
