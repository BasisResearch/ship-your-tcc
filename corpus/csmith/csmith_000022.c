/* Csmith 0cdc710315cf seed 22, flattened by scripts/csmith_flatten.py */
// Options:   --seed 22 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
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
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static int32_t
safe_unary_minus_func_int32_t_s(int32_t si )
{
  return
    (si==(-2147483647-1)) ?
    ((si)) :
    -si;
}
static uint32_t
safe_unary_minus_func_uint32_t_u(uint32_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2[4] = {0x29A3DF60,0x29A3DF60,0x29A3DF60,0x29A3DF60};
static int32_t g_5 = 1;
static int32_t g_8 = 9;
static int32_t g_20 = (-6);
static uint8_t g_27 = 0U;
static int32_t g_29 = (-10);
static int8_t g_32 = 0;
static uint32_t g_38 = 0x92E6C0B3;
static int32_t *g_49 = &g_20;
static int8_t *g_65 = &g_32;
static int8_t **g_64 = &g_65;
static int16_t g_75 = 0xE059;
static uint8_t g_76[1] = {0U};

static const int64_t  func_1(void);
static int32_t * func_11(uint32_t  p_12, uint16_t  p_13, uint64_t  p_14);
static uint64_t  func_21(int32_t * p_22);

static const int64_t  func_1(void)
{ 
    uint16_t l_60[4][4] = {{1U,0x0888,65531U,65531U},{0xAF7C,0xAF7C,0U,0x0888},{0x0888,1U,0U,1U},{0xAF7C,0xD0E1,65531U,0U}};
    const int16_t l_79 = 0x0D8E;
    int i, j;
    for (g_2[3] = 22; (g_2[3] >= 7); g_2[3] = safe_sub_func_uint16_t_u_u(g_2[3], 5))
    { 
        uint8_t l_61[3];
        int32_t *l_62 = (void*)0;
        int32_t *l_63 = &g_29;
        int8_t **l_66 = &g_65;
        int8_t **l_67 = &g_65;
        int32_t *l_68 = &g_29;
        int i;
        for (i = 0; i < 3; i++)
            l_61[i] = 255U;
        for (g_5 = 0; (g_5 == 27); g_5++)
        { 
            int32_t l_58[1];
            uint32_t l_59[1];
            int i;
            for (i = 0; i < 1; i++)
                l_58[i] = 0x18C6A866;
            for (i = 0; i < 1; i++)
                l_59[i] = 4294967292U;
            for (g_8 = 21; (g_8 == 17); g_8--)
            { 
                int32_t *l_17 = (void*)0;
                int32_t *l_18 = (void*)0;
                int32_t *l_19 = &g_20;
                int32_t **l_47 = &l_17;
                int32_t **l_48[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_48[i] = &l_18;
                g_49 = ((*l_47) = func_11((safe_rshift_func_int32_t_s_s(((*l_19) = g_8), 22)), g_2[3], func_21(&g_8)));
                (*l_19) = (((((safe_div_func_int32_t_s_s((((safe_mod_func_uint16_t_u_u(((safe_mod_func_uint16_t_u_u((safe_mod_func_uint16_t_u_u(l_58[0], l_59[0])), l_60[3][0])) , 0xDFFA), g_29)) < 0) & l_61[2]), (*g_49))) & (**l_47)) && (*g_49)) <= (*g_49)) == l_59[0]);
            }
        }
        l_67 = (l_66 = (((*l_63) &= (*g_49)) , g_64));
        if ((*g_49))
        { 
            uint64_t l_82[4] = {0U,0U,0U,0U};
            int i;
            l_68 = &g_2[1];
            (*g_49) = (g_20 , ((0x115CD63CCF75BAE5LL && (((l_60[3][0] < 0) != 255U) | 0xB50D)) > 0x1C74F348F6F7E707LL));
            if (l_60[3][0])
            { 
                int32_t *l_71 = &g_20;
                int32_t *l_72 = &g_20;
                int32_t *l_73 = &g_8;
                int32_t *l_74[3][2];
                int i, j;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_74[i][j] = &g_29;
                }
                g_76[0]++;
                if (l_79)
                    break;
            }
            else
            { 
                (*l_63) ^= (((safe_mul_func_int16_t_s_s(l_82[0], (safe_unary_minus_func_int32_t_s((*g_49))))) || (safe_unary_minus_func_uint32_t_u(((((0 == l_60[3][0]) & (*g_49)) , g_76[0]) || 0x472F8526697289F1LL)))) , (*g_49));
                if (l_60[3][0])
                    continue;
                (*l_63) |= ((*g_49) |= l_60[1][0]);
            }
        }
        else
        { 
            int32_t *l_85 = &g_29;
            int32_t *l_86[1][3];
            uint16_t l_87[2];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 3; j++)
                    l_86[i][j] = &g_20;
            }
            for (i = 0; i < 2; i++)
                l_87[i] = 65535U;
            ++l_87[0];
        }
    }
    return l_79;
}

static int32_t * func_11(uint32_t  p_12, uint16_t  p_13, uint64_t  p_14)
{ 
    int32_t l_30 = 0xB3493876;
    int8_t *l_31 = &g_32;
    uint32_t *l_37 = &g_38;
    if ((((*l_31) = l_30) <= (safe_sub_func_uint8_t_u_u((safe_mod_func_uint32_t_u_u((((*l_37)--) ^ (p_14 , p_12)), p_14)), g_20))))
    { 
        int32_t *l_45 = &g_20;
        (*l_45) |= (safe_add_func_uint64_t_u_u((g_5 & g_38), (safe_mod_func_uint16_t_u_u(p_12, 3))));
    }
    else
    { 
        int32_t *l_46[1][2];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_46[i][j] = &g_8;
        }
        return l_46[0][0];
    }
    return &g_5;
}

static uint64_t  func_21(int32_t * p_22)
{ 
    int32_t *l_28 = &g_29;
    (*l_28) &= (g_27 = (safe_sub_func_uint16_t_u_u((((-1) <= g_8) <= (safe_add_func_int64_t_s_s(0, g_2[3]))), g_5)));
    return g_8;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_5, "g_5", print_hash_value);
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_75, "g_75", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_76[i], "g_76[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
