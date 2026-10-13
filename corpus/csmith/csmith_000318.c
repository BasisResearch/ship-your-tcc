/* Csmith 0cdc710315cf seed 318, flattened by scripts/csmith_flatten.py */
// Options:   --seed 318 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
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
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int64_t  f0;
   int64_t  f1;
   int16_t  f2;
   uint16_t  f3;
};

struct S1 {
   uint32_t  f0;
   uint8_t  f1;
};

struct S2 {
   const struct S1  f0;
   int8_t  f1;
   struct S1  f2;
};

static int32_t g_2 = (-4);
static int32_t g_5 = 1;
static struct S2 g_17 = {{1U,0xBC},0xFA,{0xBC07C207,0xAC}};
static int32_t g_20 = 0x01213072;
static int16_t g_32 = 0xEB81;
static int32_t *g_35 = &g_20;
static struct S1 g_38 = {4294967295U,247U};
static struct S1 * const g_37[3][3] = {{&g_38,&g_38,&g_38},{&g_38,&g_38,&g_38},{&g_38,&g_38,&g_38}};
static int32_t g_46 = 1;
static uint64_t g_47 = 0x2C3978D11E8E103CLL;
static uint16_t g_59 = 65535U;
static struct S0 g_69 = {-7,-1,0x0091,0x4D94};

static struct S1  func_1(void);
static int32_t * func_8(int32_t * p_9);
static int32_t * func_10(int32_t * p_11, int32_t * p_12, int32_t * p_13, struct S2  p_14, const int32_t * p_15);

static struct S1  func_1(void)
{ 
    int32_t *l_16 = &g_2;
    int32_t l_44 = 0xC50133EC;
    int32_t l_58 = 0xE577E9FB;
    struct S1 l_75 = {1U,0x8B};
    for (g_2 = 0; (g_2 <= (-13)); g_2 = safe_sub_func_int32_t_s_s(g_2, 4))
    { 
        int32_t *l_42 = (void*)0;
        int32_t *l_43 = &g_20;
        int32_t *l_45[4];
        struct S1 l_74[2] = {{4294967295U,1U},{4294967295U,1U}};
        int i;
        for (i = 0; i < 4; i++)
            l_45[i] = (void*)0;
        for (g_5 = 9; (g_5 != 1); --g_5)
        { 
            int32_t **l_34[2];
            int i;
            for (i = 0; i < 2; i++)
                l_34[i] = &l_16;
            l_42 = func_8((g_35 = func_10(&g_2, l_16, &g_2, g_17, &g_2)));
        }
        --g_47;
        for (g_38.f0 = 0; (g_38.f0 >= 23); ++g_38.f0)
        { 
            int64_t l_54[2][4] = {{1,1,1,1},{1,1,1,1}};
            int32_t l_56 = 0x350B8378;
            int32_t l_57 = 2;
            const uint64_t l_66[3] = {1U,1U,1U};
            int i, j;
            for (g_47 = (-2); (g_47 >= 58); g_47 = safe_add_func_uint8_t_u_u(g_47, 5))
            { 
                int32_t l_55 = 0xD0D8167E;
                g_59++;
            }
            if ((*l_16))
                continue;
            if ((safe_rshift_func_uint64_t_u_s((safe_rshift_func_uint16_t_u_u(g_17.f2.f0, l_66[0])), 41)))
            { 
                struct S0 l_67 = {-1,0xDCFCB68B63C96AADLL,-6,3U};
                struct S0 *l_68[4] = {&l_67,&l_67,&l_67,&l_67};
                int16_t *l_72 = &g_69.f2;
                int32_t l_73 = 0xF8266116;
                int i;
                g_69 = l_67;
                (*g_35) |= (((safe_lshift_func_uint8_t_u_u(l_67.f0, l_67.f2)) >= (((*l_72) ^= (g_17.f1 && 0xDB0C68D18E14F04BLL)) > 0U)) == g_17.f0.f0);
                l_73 &= (*g_35);
            }
            else
            { 
                return l_74[0];
            }
        }
    }
    return l_75;
}

static int32_t * func_8(int32_t * p_9)
{ 
    uint16_t l_36 = 65535U;
    struct S1 *l_40 = &g_38;
    struct S1 **l_39[1];
    struct S1 **l_41 = &l_40;
    int i;
    for (i = 0; i < 1; i++)
        l_39[i] = &l_40;
    (*p_9) |= l_36;
    (*l_41) = g_37[2][0];
    return p_9;
}

static int32_t * func_10(int32_t * p_11, int32_t * p_12, int32_t * p_13, struct S2  p_14, const int32_t * p_15)
{ 
    int8_t l_18 = 0x4F;
    int32_t *l_19 = &g_20;
    int16_t *l_31 = &g_32;
    uint32_t *l_33 = &g_17.f2.f0;
    (*l_19) ^= l_18;
    (*l_19) &= (safe_rshift_func_int32_t_s_s((9 > (((safe_mul_func_uint32_t_u_u(0x9D73AE4C, ((*l_33) = ((safe_sub_func_int16_t_s_s(((*l_31) = (safe_sub_func_int32_t_s_s((safe_mul_func_int16_t_s_s(g_2, 0U)), (-2)))), p_14.f1)) || 0xC3D0)))) , p_12) == &g_20)), 27));
    return &g_20;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_17.f0.f0, "g_17.f0.f0", print_hash_value);
    transparent_crc(g_17.f0.f1, "g_17.f0.f1", print_hash_value);
    transparent_crc(g_17.f1, "g_17.f1", print_hash_value);
    transparent_crc(g_17.f2.f0, "g_17.f2.f0", print_hash_value);
    transparent_crc(g_17.f2.f1, "g_17.f2.f1", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_38.f0, "g_38.f0", print_hash_value);
    transparent_crc(g_38.f1, "g_38.f1", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_69.f0, "g_69.f0", print_hash_value);
    transparent_crc(g_69.f1, "g_69.f1", print_hash_value);
    transparent_crc(g_69.f2, "g_69.f2", print_hash_value);
    transparent_crc(g_69.f3, "g_69.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
