/* Csmith 0cdc710315cf seed 90, flattened by scripts/csmith_flatten.py */
// Options:   --seed 90 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint32_t  f0;
   int32_t  f1;
};

struct S1 {
   uint32_t  f0;
   int32_t  f1;
};

struct S2 {
   int32_t  f0;
   struct S1  f1;
};

static int16_t g_7 = 0xB49D;
static int16_t g_10[2][2] = {{1,1},{1,1}};
static uint64_t g_18 = 0xA6FF609D70CADD63LL;
static struct S1 g_22 = {0xADD5D088,0};
static int8_t g_27 = 0x87;
static int32_t g_55 = 0xE0CA2CDF;
static int32_t *g_54 = &g_55;
static struct S2 g_60[1] = {{6,{4294967295U,6}}};
static uint16_t g_81 = 5U;
static uint32_t g_83 = 0x99732ED8;
static int32_t g_85 = (-6);
static int8_t *g_89 = &g_27;
static int8_t **g_88 = &g_89;
static struct S0 g_92 = {0x4A10B7D5,0};
static uint64_t g_110 = 0x7E885CFE2AFDD6AALL;

static struct S1  func_1(void);
static const uint16_t  func_8(uint32_t  p_9);
static struct S1  func_11(int32_t  p_12, const int32_t  p_13, int16_t * p_14);

static struct S1  func_1(void)
{ 
    uint32_t l_4 = 0xA42B4837;
    int16_t *l_5 = (void*)0;
    int16_t *l_6 = &g_7;
    int8_t *l_32[2][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0}};
    uint8_t l_37 = 0xE1;
    uint16_t l_64[1];
    struct S1 l_120 = {6U,0};
    int i, j;
    for (i = 0; i < 1; i++)
        l_64[i] = 0xBC3F;
    if ((safe_mul_func_uint8_t_u_u((((l_4 & (((*l_6) = 0x7925) >= func_8((g_10[0][1] , 7U)))) ^ g_10[1][1]) , 0x2B), l_4)))
    { 
        int32_t *l_33 = &g_22.f1;
        int32_t l_35 = 0x0863E871;
        int32_t l_36[3];
        uint8_t l_80 = 5U;
        int i;
        for (i = 0; i < 3; i++)
            l_36[i] = 0x283A38C8;
        if (((safe_lshift_func_int32_t_s_s(((*l_33) = (l_4 > ((void*)0 == l_32[0][1]))), 27)) | 0x87B332E0C280563ALL))
        { 
            int32_t *l_34[3][1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_34[i][j] = &g_22.f1;
            }
            --l_37;
            (*l_33) |= 1;
        }
        else
        { 
            int64_t l_52 = 0x4F7D9FD593B88A61LL;
            uint16_t *l_82[1][3];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 3; j++)
                    l_82[i][j] = &l_64[0];
            }
            for (g_22.f1 = 0; (g_22.f1 != 11); g_22.f1++)
            { 
                uint64_t *l_51 = &g_18;
                int32_t *l_53 = &l_36[1];
                int32_t **l_56[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_56[i] = &l_33;
                (*l_53) = (safe_mod_func_int16_t_s_s((g_10[0][1] <= (l_4 ^ (((~(safe_add_func_uint8_t_u_u(((safe_mod_func_int8_t_s_s((((*l_51) = g_22.f0) & l_52), l_37)) && 0xE9), 0U))) || (*l_33)) < 1U))), 6));
                l_33 = g_54;
            }
            if (((((0 > (((((((--g_18) ^ ((~(*l_33)) <= (((g_60[0] , (*l_33)) , l_37) ^ g_27))) ^ 0xE07D3F149C99C4DBLL) , 1U) < g_22.f0) & g_22.f0) & l_37)) <= g_10[0][1]) < g_60[0].f1.f0) < g_27))
            { 
                uint64_t *l_63 = &g_18;
                struct S1 *l_65 = &g_60[0].f1;
                (*g_54) = (&g_27 != (void*)0);
                (*l_33) ^= (((*l_63) &= (safe_rshift_func_uint16_t_u_u((0x8A || (g_27 = g_22.f0)), 11))) == ((65535U != l_64[0]) > g_10[0][1]));
                (*l_65) = g_60[0].f1;
            }
            else
            { 
                struct S1 l_66[1][3] = {{{4294967295U,-5},{4294967295U,-5},{4294967295U,-5}}};
                int i, j;
                return l_66[0][0];
            }
            if ((safe_sub_func_int16_t_s_s((!(g_83 &= ((((safe_rshift_func_uint8_t_u_u((safe_sub_func_int16_t_s_s((safe_rshift_func_int64_t_s_u(((safe_mul_func_uint8_t_u_u(((((g_60[0].f1.f0 , (safe_mul_func_uint64_t_u_u(18446744073709551613U, (*l_33)))) && l_52) , g_27) > l_52), l_80)) & 0x635B), 1)), g_81)), 6)) == (-1)) ^ l_52) || l_52))), (*l_33))))
            { 
                (*l_33) |= l_64[0];
                (*l_33) ^= ((void*)0 != &l_64[0]);
                (*l_33) = (~(g_85 |= 0));
            }
            else
            { 
                int32_t **l_86 = (void*)0;
                int32_t **l_87 = &g_54;
                (*l_87) = &g_85;
                g_88 = (void*)0;
            }
        }
    }
    else
    { 
        int8_t l_90 = 4;
        struct S1 *l_98 = (void*)0;
        int32_t l_105 = (-2);
        for (g_18 = 0; (g_18 <= 0); g_18 += 1)
        { 
            struct S0 *l_91 = &g_92;
            struct S0 **l_93 = &l_91;
            int32_t l_103 = (-1);
            uint16_t *l_104 = (void*)0;
            struct S1 *l_106 = &g_60[0].f1;
            struct S1 **l_107 = &l_106;
            const uint8_t l_108 = 0U;
            uint8_t l_119 = 4U;
            (*g_54) = l_90;
            (*l_93) = l_91;
            (*g_54) = (((safe_lshift_func_int16_t_s_s((safe_add_func_uint16_t_u_u((l_98 == ((*l_107) = ((safe_mod_func_uint16_t_u_u(g_7, (l_105 |= (((safe_sub_func_uint64_t_u_u((((4294967294U < l_90) , l_90) ^ l_103), l_103)) != g_85) != 0xD2)))) , l_106))), l_108)), 10)) , 0xC59DB3CA632B0D1CLL) & g_60[0].f1.f1);
            for (g_22.f1 = 0; (g_22.f1 >= 0); g_22.f1 -= 1)
            { 
                uint64_t *l_109 = &g_110;
                (*g_54) ^= (1 <= (((*l_109)++) != ((((safe_mod_func_int8_t_s_s((safe_mul_func_uint32_t_u_u((safe_rshift_func_uint64_t_u_u((((*l_6) = l_119) || ((((void*)0 == &g_54) , l_90) ^ 0x82E3)), 30)), g_83)), g_85)) & g_83) ^ l_119) ^ 18446744073709551608U)));
            }
        }
    }
    return l_120;
}

static const uint16_t  func_8(uint32_t  p_9)
{ 
    int16_t *l_15[1][2];
    struct S1 *l_20 = (void*)0;
    struct S1 *l_21 = &g_22;
    int32_t l_25 = 1;
    int8_t *l_26 = &g_27;
    int32_t *l_28 = &l_25;
    int32_t *l_29 = &g_22.f1;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_15[i][j] = (void*)0;
    }
    (*l_21) = func_11(g_10[1][0], p_9, l_15[0][1]);
    (*l_29) ^= (safe_mul_func_int32_t_s_s(p_9, ((*l_28) = (g_22.f0 & ((*l_26) = ((l_25 ^ l_25) ^ 0x9EFE))))));
    return g_18;
}

static struct S1  func_11(int32_t  p_12, const int32_t  p_13, int16_t * p_14)
{ 
    struct S1 l_19 = {4294967292U,0x6F8F9ED0};
    g_18 &= (safe_rshift_func_uint32_t_u_u(g_10[1][1], 31));
    return l_19;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_7, "g_7", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_10[i][j], "g_10[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_22.f0, "g_22.f0", print_hash_value);
    transparent_crc(g_22.f1, "g_22.f1", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_60[i].f0, "g_60[i].f0", print_hash_value);
        transparent_crc(g_60[i].f1.f0, "g_60[i].f1.f0", print_hash_value);
        transparent_crc(g_60[i].f1.f1, "g_60[i].f1.f1", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_83, "g_83", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_92.f0, "g_92.f0", print_hash_value);
    transparent_crc(g_92.f1, "g_92.f1", print_hash_value);
    transparent_crc(g_110, "g_110", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
