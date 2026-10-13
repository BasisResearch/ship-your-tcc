/* Csmith 0cdc710315cf seed 187, flattened by scripts/csmith_flatten.py */
// Options:   --seed 187 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
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
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
   int8_t  f0;
   int16_t  f1;
   int32_t  f2;
};

struct S2 {
   const uint32_t  f0;
   const uint32_t  f1;
   struct S0  f2;
};

union U3 {
   uint64_t  f0;
   const int32_t  f1;
   int64_t  f2;
};

union U5 {
   uint8_t  f0;
   const int64_t  f1;
};

static int64_t g_7 = 0x4ACCB24CE55459EELL;
static int32_t g_26 = 0x844BA928;
static int64_t g_28 = (-1);
static int16_t g_41 = (-6);
static int8_t g_43 = 0x00;
static uint8_t g_44 = 254U;
static int8_t g_56 = 1;
static int64_t g_57 = 0x966A49DF3C4FB361LL;
static int64_t g_58 = 0x67D996F0491844C3LL;
static int32_t g_59 = 1;
static int64_t g_60 = (-4);
static int8_t g_62 = 1;
static uint8_t g_63 = 0xF9;
static int32_t **g_67 = (void*)0;

static uint64_t  func_1(void);
static int32_t  func_2(int8_t  p_3, uint64_t  p_4, const int8_t  p_5, uint64_t  p_6);
static const int8_t  func_8(struct S2  p_9, uint16_t  p_10, uint8_t  p_11, uint8_t  p_12);

static uint64_t  func_1(void)
{ 
    struct S2 l_13 = {0U,0xD7BC3524,{0xCA,0x0051,-9}};
    int32_t *l_66[4][1];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 1; j++)
            l_66[i][j] = &g_59;
    }
    g_59 = func_2(g_7, g_7, func_8(l_13, l_13.f1, g_7, g_7), l_13.f2.f2);
    g_67 = g_67;
    return g_62;
}

static int32_t  func_2(int8_t  p_3, uint64_t  p_4, const int8_t  p_5, uint64_t  p_6)
{ 
    int32_t *l_54 = &g_26;
    int32_t *l_55[3];
    int8_t l_61 = (-5);
    int i;
    for (i = 0; i < 3; i++)
        l_55[i] = &g_26;
    g_63--;
    return g_26;
}

static const int8_t  func_8(struct S2  p_9, uint16_t  p_10, uint8_t  p_11, uint8_t  p_12)
{ 
    union U3 l_24[2][1] = {{{0U}},{{0U}}};
    int32_t *l_25[1][4] = {{&g_26,&g_26,&g_26,&g_26}};
    int64_t *l_27 = &g_28;
    const int16_t *l_46 = &g_41;
    const int16_t **l_45 = &l_46;
    const int16_t **l_47[1][3];
    union U5 l_49[1] = {{1U}};
    int32_t *l_53 = &g_26;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_47[i][j] = &l_46;
    }
    if (((safe_mod_func_uint16_t_u_u((safe_rshift_func_uint16_t_u_u((((*l_27) |= (safe_lshift_func_int32_t_s_u((g_26 = ((safe_add_func_int16_t_s_s((-9), 0x1C36)) | (safe_mul_func_uint32_t_u_u((l_24[1][0] , 0x29ED7D41), g_7)))), p_9.f1))) || g_28), 13)), g_7)) && g_26))
    { 
        int32_t l_35 = 4;
        int16_t *l_40 = &g_41;
        int8_t *l_42 = &g_43;
        g_44 ^= (safe_lshift_func_uint16_t_u_s(((((*l_42) = (safe_rshift_func_int16_t_s_s((((((*l_40) = (((safe_lshift_func_int16_t_s_s((l_35 & (((g_7 , (((((safe_mod_func_uint64_t_u_u(((safe_sub_func_int8_t_s_s(g_7, p_10)) ^ p_10), g_28)) != 0x74D9070A6219FF40LL) == 0x1EDE) != p_12) , (-1))) , 0xB827BE627823D12ELL) < 1U)), 0)) || g_26) , 0x80BB)) , p_9.f0) & p_9.f1) || p_11), 1))) , &g_26) == l_25[0][2]), 11));
    }
    else
    { 
        int8_t l_50 = 0xD5;
        int32_t **l_51 = (void*)0;
        int32_t **l_52[2][2];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
                l_52[i][j] = &l_25[0][3];
        }
        for (p_9.f2.f1 = 0; (p_9.f2.f1 <= 0); p_9.f2.f1 += 1)
        { 
            const uint64_t l_48 = 0U;
            l_47[0][1] = l_45;
            return l_48;
        }
        l_50 &= ((l_49[0] , &l_46) == (void*)0);
        l_53 = l_25[0][3];
    }
    return g_28;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_43, "g_43", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_56, "g_56", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
