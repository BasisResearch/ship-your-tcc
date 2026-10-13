/* Csmith 0cdc710315cf seed 241, flattened by scripts/csmith_flatten.py */
// Options:   --seed 241 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint8_t  f0;
};

union U1 {
   uint8_t  f0;
   uint32_t  f1;
};

static uint64_t g_3 = 0x76C3A0B8EE41B66BLL;
static int32_t g_15 = (-1);
static int64_t g_19 = 0;
static uint8_t g_21 = 0xEE;
static int16_t g_28 = 0x56D9;
static int16_t g_29 = 0xBED9;
static uint16_t g_36[2] = {7U,7U};
static uint64_t g_50 = 0x5C12BBF3AF71E1D7LL;
static uint64_t g_55 = 1U;
static union U0 g_59 = {0xE8};
static uint32_t g_64 = 4294967290U;
static int8_t g_65 = (-8);

static uint8_t  func_1(void);
static int64_t  func_6(uint64_t  p_7, const int64_t  p_8, int64_t  p_9);
static int32_t  func_11(const int32_t * p_12, int32_t * p_13);

static uint8_t  func_1(void)
{ 
    int32_t *l_2[4][3];
    int16_t l_66[1];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
            l_2[i][j] = (void*)0;
    }
    for (i = 0; i < 1; i++)
        l_66[i] = 7;
    g_3 = 0x197F0A29;
    g_65 ^= (g_64 &= ((safe_lshift_func_int64_t_s_s(func_6(g_3, (g_3 , g_3), g_3), g_19)) == g_28));
    l_2[0][1] = &g_15;
    return l_66[0];
}

static int64_t  func_6(uint64_t  p_7, const int64_t  p_8, int64_t  p_9)
{ 
    int16_t l_10[1][3];
    int32_t l_27[4];
    union U1 l_42 = {0xDC};
    uint8_t l_45 = 0xF4;
    int32_t **l_63 = (void*)0;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_10[i][j] = 0;
    }
    for (i = 0; i < 4; i++)
        l_27[i] = 0xB221494F;
    for (p_7 = 0; (p_7 <= 0); p_7 += 1)
    { 
        const int32_t *l_35[1];
        int32_t l_53[2];
        union U0 * const l_58 = &g_59;
        int i;
        for (i = 0; i < 1; i++)
            l_35[i] = &l_27[3];
        for (i = 0; i < 2; i++)
            l_53[i] = (-6);
        for (p_9 = 0; (p_9 >= 0); p_9 -= 1)
        { 
            const int32_t *l_14 = &g_15;
            int32_t *l_16 = &g_15;
            int32_t *l_26[3][3];
            uint32_t l_30 = 0U;
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                    l_26[i][j] = &g_15;
            }
            for (g_3 = 0; (g_3 <= 0); g_3 += 1)
            { 
                int i, j;
                (*l_16) = (func_11(l_14, l_16) != l_10[p_7][(g_3 + 2)]);
            }
            --l_30;
            if (((safe_sub_func_uint8_t_u_u((l_10[p_7][(p_7 + 2)] && ((l_10[p_9][(p_7 + 1)] , (&g_15 != l_35[0])) <= l_10[p_9][(p_7 + 2)])), (-1))) < l_10[0][1]))
            { 
                g_36[0] ^= (*l_14);
                l_27[1] = (safe_unary_minus_func_uint8_t_u(((safe_div_func_int32_t_s_s((p_9 | (safe_sub_func_uint16_t_u_u((l_42 , p_9), g_19))), g_36[0])) ^ g_19)));
            }
            else
            { 
                int8_t l_43[4];
                int32_t l_44 = 0x9F7E48FF;
                int i;
                for (i = 0; i < 4; i++)
                    l_43[i] = 0x26;
                l_45--;
            }
        }
        if (p_9)
            continue;
        for (g_29 = 0; (g_29 >= 0); g_29 -= 1)
        { 
            int32_t *l_48 = &g_15;
            int32_t *l_49[4] = {&l_27[1],&l_27[1],&l_27[1],&l_27[1]};
            int i;
            --g_50;
            for (p_9 = 0; (p_9 <= 1); p_9 += 1)
            { 
                int32_t l_54 = 0xEEB1CB69;
                union U0 *l_61[1];
                union U0 **l_60[4][4] = {{&l_61[0],&l_61[0],&l_61[0],&l_61[0]},{&l_61[0],&l_61[0],&l_61[0],&l_61[0]},{&l_61[0],&l_61[0],&l_61[0],&l_61[0]},{&l_61[0],&l_61[0],&l_61[0],&l_61[0]}};
                union U0 **l_62 = &l_61[0];
                int i, j;
                for (i = 0; i < 1; i++)
                    l_61[i] = &g_59;
                --g_55;
                (*l_62) = l_58;
                return g_36[(p_7 + 1)];
            }
            for (g_59.f0 = 0; (g_59.f0 <= 0); g_59.f0 += 1)
            { 
                (*l_48) = g_3;
                (*l_48) ^= (0x7B6F817EA5CA4386LL >= 0x450074354AF31781LL);
                l_63 = l_63;
            }
        }
    }
    for (l_45 = 0; (l_45 <= 0); l_45 += 1)
    { 
        return g_19;
    }
    return p_7;
}

static int32_t  func_11(const int32_t * p_12, int32_t * p_13)
{ 
    int32_t *l_17 = (void*)0;
    int32_t *l_18 = &g_15;
    int32_t *l_20[3];
    int32_t **l_24 = (void*)0;
    int32_t **l_25 = &l_20[1];
    int i;
    for (i = 0; i < 3; i++)
        l_20[i] = (void*)0;
    g_21++;
    (*l_25) = &g_15;
    return g_15;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_36[i], "g_36[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_50, "g_50", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_59.f0, "g_59.f0", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
