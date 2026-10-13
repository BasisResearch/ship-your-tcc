/* Csmith 0cdc710315cf seed 274, flattened by scripts/csmith_flatten.py */
// Options:   --seed 274 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
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
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static uint32_t
safe_unary_minus_func_uint32_t_u(uint32_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   int32_t  f0;
   uint32_t  f1;
};

static int32_t g_2 = 1;
static const union U0 g_11 = {0};
static int32_t g_13 = (-6);
static int32_t g_24 = 0xE5C8A52B;
static int32_t *g_32 = &g_24;
static int32_t **g_31 = &g_32;
static uint64_t g_37 = 0xD8607F47073E70D2LL;
static uint8_t g_43 = 0x24;
static uint32_t g_49 = 0xA0165D3E;
static const uint32_t g_63 = 1U;
static int64_t g_68 = 1;
static int8_t g_75 = 0;
static uint32_t *g_93 = &g_49;
static uint32_t **g_92[3] = {&g_93,&g_93,&g_93};
static uint32_t **g_94[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
static uint8_t g_103 = 247U;
static const uint32_t g_134 = 1U;
static uint16_t g_141 = 1U;

static int16_t  func_1(void);
static int32_t  func_5(uint64_t  p_6, uint64_t  p_7, uint8_t  p_8);
static uint64_t  func_9(const union U0  p_10);

static int16_t  func_1(void)
{ 
    int8_t l_67 = 0x91;
    int32_t l_95[4][1] = {{0},{0x70F0D5DC},{0},{0x70F0D5DC}};
    int i, j;
    for (g_2 = 0; (g_2 == (-8)); --g_2)
    { 
        int64_t l_108 = 0x31AB994F54D63FF5LL;
        union U0 l_130[4] = {{-6},{-6},{-6},{-6}};
        const int32_t *l_144 = &l_95[0][0];
        int i;
    }
    return g_11.f0;
}

static int32_t  func_5(uint64_t  p_6, uint64_t  p_7, uint8_t  p_8)
{ 
    uint64_t l_71 = 0U;
    uint8_t *l_72 = &g_43;
    int32_t **l_73 = (void*)0;
    int8_t *l_74[2][1];
    int32_t l_76 = 3;
    int32_t l_82 = 0;
    int32_t l_83 = 1;
    int32_t l_84 = 1;
    int32_t l_85 = 1;
    int32_t l_86 = 0x15B8451C;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_74[i][j] = &g_75;
    }
    (**g_31) ^= (safe_div_func_int64_t_s_s(((((*l_72) ^= l_71) != p_8) == ((l_76 |= ((void*)0 == l_73)) > p_6)), 0xB88C8ECA74B80CDELL));
    for (l_76 = 0; (l_76 < 9); l_76 = safe_add_func_int64_t_s_s(l_76, 9))
    { 
        const int32_t *l_79 = &g_24;
        const int32_t **l_80 = &l_79;
        int32_t *l_81[1][4];
        uint8_t l_87 = 0U;
        uint32_t *l_91 = &g_49;
        uint32_t **l_90 = &l_91;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 4; j++)
                l_81[i][j] = &g_24;
        }
        (*l_80) = l_79;
        l_87++;
        (**g_31) |= (&p_8 == (((g_94[2] = (g_92[0] = l_90)) != (((*l_80) != (void*)0) , &g_93)) , (void*)0));
    }
    return p_8;
}

static uint64_t  func_9(const union U0  p_10)
{ 
    uint8_t l_12[1][1];
    int32_t l_50 = (-2);
    int16_t l_64 = 1;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_12[i][j] = 255U;
    }
    for (g_13 = 0; (g_13 <= 0); g_13 += 1)
    { 
        const int32_t *l_14 = &g_2;
        const int32_t **l_15 = &l_14;
        int32_t l_20 = 0x6B9C2D62;
        int32_t *l_23 = &g_24;
        (*l_15) = l_14;
        (*l_23) ^= (safe_mul_func_uint8_t_u_u(0xD3, (safe_add_func_int8_t_s_s((l_20 = g_2), (safe_mod_func_int8_t_s_s((((g_11.f0 == g_2) < p_10.f0) == (-1)), 0x86))))));
        for (g_24 = 0; (g_24 <= 0); g_24 += 1)
        { 
            int32_t *l_29 = &l_20;
            int8_t l_30 = 0x85;
            int32_t l_51 = (-1);
            int i, j;
            (*l_29) = (~(65535U ^ (safe_div_func_uint8_t_u_u((safe_unary_minus_func_uint32_t_u(l_12[g_24][g_24])), g_2))));
            if ((l_30 & ((void*)0 != g_31)))
            { 
                int32_t *l_33 = &l_20;
                int32_t *l_34 = (void*)0;
                int32_t *l_35 = &l_20;
                int32_t *l_36 = (void*)0;
                ++g_37;
            }
            else
            { 
                uint8_t *l_42[2][3];
                uint32_t *l_48[3][3];
                const uint32_t **l_59 = (void*)0;
                const uint32_t **l_60 = (void*)0;
                const uint32_t *l_62 = &g_63;
                const uint32_t **l_61 = &l_62;
                int32_t *l_65 = (void*)0;
                int32_t *l_66 = &l_51;
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 3; j++)
                        l_42[i][j] = &g_43;
                }
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 3; j++)
                        l_48[i][j] = &g_49;
                }
                (*l_66) = ((g_43 ^= (l_12[g_13][g_13]++)) , (l_64 = ((safe_div_func_int16_t_s_s(((safe_mod_func_uint32_t_u_u((--g_49), ((*l_29) = (g_11 , (safe_add_func_int64_t_s_s((!((safe_sub_func_int16_t_s_s((((*l_61) = (void*)0) == l_29), 0xF833)) , p_10.f0)), p_10.f0)))))) < g_2), g_37)) , 1)));
                return g_63;
            }
        }
    }
    return g_43;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_11.f0, "g_11.f0", print_hash_value);
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_68, "g_68", print_hash_value);
    transparent_crc(g_75, "g_75", print_hash_value);
    transparent_crc(g_103, "g_103", print_hash_value);
    transparent_crc(g_134, "g_134", print_hash_value);
    transparent_crc(g_141, "g_141", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
