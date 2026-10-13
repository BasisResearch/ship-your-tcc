/* Csmith 0cdc710315cf seed 204, flattened by scripts/csmith_flatten.py */
// Options:   --seed 204 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
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
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
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
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint16_t  f0;
};

union U1 {
   const int64_t  f0;
   int32_t  f1;
};

static uint32_t g_10 = 0x9F845D81;
static uint8_t g_11 = 1U;
static int8_t g_27[2][2] = {{(-1),(-1)},{(-1),(-1)}};
static int32_t g_29 = (-1);
static const int64_t g_37 = 0xA7C769867F5D1CC0LL;
static union U1 *g_39 = (void*)0;
static union U1 g_41 = {0x657A772BE76DCB0CLL};
static union U0 g_44 = {0xBC18};
static int64_t g_59 = (-3);
static int64_t *g_58 = &g_59;
static int64_t * const *g_57 = &g_58;
static int64_t **g_61 = (void*)0;
static int32_t g_67 = (-1);

static uint8_t  func_1(void);
static int32_t  func_2(union U0  p_3);
static union U0  func_4(int16_t  p_5, uint64_t  p_6, const uint8_t  p_7);

static uint8_t  func_1(void)
{ 
    int64_t l_12 = (-8);
    int32_t *l_66 = &g_67;
    int32_t *l_68 = &g_41.f1;
    const int64_t *l_70 = (void*)0;
    const int64_t **l_69 = &l_70;
    (*l_66) &= func_2(func_4((g_11 = (safe_mul_func_uint64_t_u_u(g_10, (g_10 , 1U)))), l_12, l_12));
    (*l_68) = (g_29 , ((*l_66) = (0x8F || (*l_66))));
    (*l_68) = ((void*)0 == l_69);
    return g_11;
}

static int32_t  func_2(union U0  p_3)
{ 
    int64_t l_53 = 1;
    int64_t **l_60 = &g_58;
    int32_t *l_62 = &g_41.f1;
    int32_t *l_63 = &g_41.f1;
    int32_t *l_64[3][2] = {{&g_29,&g_29},{&g_29,&g_29},{&g_29,&g_29}};
    uint64_t l_65[4] = {0xC9AC7196A48ADB29LL,0xC9AC7196A48ADB29LL,0xC9AC7196A48ADB29LL,0xC9AC7196A48ADB29LL};
    int i, j;
    for (g_10 = 0; (g_10 <= 1); g_10 += 1)
    { 
        uint32_t l_54 = 0xFA57A880;
        for (g_41.f1 = 1; (g_41.f1 >= 0); g_41.f1 -= 1)
        { 
            int32_t *l_46 = &g_29;
            int32_t **l_45 = &l_46;
            (*l_45) = &g_29;
        }
        for (g_29 = 0; (g_29 <= 1); g_29 += 1)
        { 
            int32_t l_52[4];
            int32_t *l_55 = &l_52[1];
            int i;
            for (i = 0; i < 4; i++)
                l_52[i] = (-7);
            (*l_55) = (safe_mod_func_uint8_t_u_u(((safe_add_func_int16_t_s_s((((p_3 , g_41.f1) || (((!((p_3.f0 || l_52[1]) , 0)) != l_53) >= g_41.f1)) , p_3.f0), l_54)) || l_54), 0xD5));
        }
    }
    l_65[0] ^= (((p_3.f0 | ((((+((g_57 != (g_61 = (l_60 = l_60))) && g_27[1][1])) == g_44.f0) || g_10) || 1)) | 1U) , 0x35EE8DA5);
    return g_37;
}

static union U0  func_4(int16_t  p_5, uint64_t  p_6, const uint8_t  p_7)
{ 
    uint8_t l_13[1][1];
    union U1 *l_40[4][3];
    int32_t *l_42 = &g_29;
    int32_t **l_43 = &l_42;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
            l_13[i][j] = 0x37;
    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
            l_40[i][j] = &g_41;
    }
    for (p_6 = 0; (p_6 <= 0); p_6 += 1)
    { 
        int16_t l_25[1][2];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_25[i][j] = (-4);
        }
        if (((p_5 != ((safe_add_func_uint32_t_u_u(g_10, l_13[0][0])) ^ 6)) <= p_6))
        { 
            for (g_10 = 0; (g_10 <= 0); g_10 += 1)
            { 
                uint16_t l_16 = 65535U;
                int64_t *l_26[2];
                int32_t *l_28 = &g_29;
                int i;
                for (i = 0; i < 2; i++)
                    l_26[i] = (void*)0;
                (*l_28) = ((l_16 || (g_27[1][1] &= (safe_rshift_func_int64_t_s_s((safe_div_func_uint32_t_u_u((safe_rshift_func_int8_t_s_u((safe_sub_func_int16_t_s_s(p_7, p_7)), 1)), l_25[0][0])), 7)))) ^ g_10);
                if (g_11)
                    continue;
            }
            for (p_5 = 0; (p_5 >= 0); p_5 -= 1)
            { 
                int32_t *l_31 = &g_29;
                int32_t **l_30 = &l_31;
                (*l_30) = &g_29;
            }
        }
        else
        { 
            uint64_t l_32 = 0x0509B95520F30385LL;
            int32_t l_34 = 0xD8491F19;
            if (g_10)
                break;
            if (p_6)
            { 
                int32_t *l_33[1][2];
                const int64_t *l_36 = &g_37;
                const int64_t **l_35[4][4] = {{&l_36,&l_36,&l_36,&l_36},{&l_36,&l_36,&l_36,&l_36},{&l_36,&l_36,&l_36,&l_36},{&l_36,&l_36,&l_36,&l_36}};
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_33[i][j] = &g_29;
                }
                l_32 = (-1);
                l_34 ^= p_5;
                l_35[3][0] = l_35[3][0];
            }
            else
            { 
                int64_t l_38 = 5;
                l_38 = g_27[1][1];
            }
        }
        for (g_29 = 0; (g_29 <= 0); g_29 += 1)
        { 
            l_40[1][2] = g_39;
            for (g_11 = 0; (g_11 <= 0); g_11 += 1)
            { 
                if (p_5)
                    break;
            }
        }
    }
    (*l_43) = l_42;
    (**l_43) = g_27[0][1];
    return g_44;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_11, "g_11", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_27[i][j], "g_27[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_41.f1, "g_41.f1", print_hash_value);
    transparent_crc(g_44.f0, "g_44.f0", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_67, "g_67", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
