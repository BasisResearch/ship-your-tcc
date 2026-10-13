/* Csmith 0cdc710315cf seed 329, flattened by scripts/csmith_flatten.py */
// Options:   --seed 329 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int16_t g_12[4][4] = {{0x1454,0,0x1454,0x1454},{0,0,2,0},{0,0x1454,0x1454,0},{0x1454,0,0x1454,0x1454}};
static uint8_t g_27 = 0U;
static int16_t g_29 = 0x2E63;
static int32_t g_31 = 0xC2AFE249;
static int32_t *g_30 = &g_31;
static int16_t *g_36 = &g_29;
static uint64_t g_42 = 0x300B6311140AE776LL;
static uint32_t g_61 = 0x7A5E2B19;
static int8_t g_81 = 0xD8;
static uint16_t g_84 = 0x0F42;
static uint64_t g_89 = 9U;

static uint64_t  func_1(void);
static uint8_t  func_4(uint16_t  p_5, uint64_t  p_6, const uint16_t  p_7);
static uint64_t  func_9(uint16_t  p_10, uint16_t  p_11);

static uint64_t  func_1(void)
{ 
    int64_t l_8 = (-6);
    uint16_t l_20[2];
    uint64_t *l_88 = &g_89;
    int32_t *l_90 = &g_31;
    int i;
    for (i = 0; i < 2; i++)
        l_20[i] = 65535U;
    (*l_90) = ((safe_rshift_func_uint32_t_u_s((((func_4(l_8, ((*l_88) &= func_9(((g_12[0][0] && (((safe_mul_func_int8_t_s_s((safe_lshift_func_int64_t_s_u((safe_lshift_func_int8_t_s_s((+g_12[1][3]), g_12[0][0])), 9)), 1U)) < l_20[1]) && l_20[1])) || (-2)), g_12[0][0])), g_12[0][0]) && l_8) ^ 0x17) >= l_20[1]), 8)) || 65533U);
    return g_84;
}

static uint8_t  func_4(uint16_t  p_5, uint64_t  p_6, const uint16_t  p_7)
{ 
    return g_61;
}

static uint64_t  func_9(uint16_t  p_10, uint16_t  p_11)
{ 
    uint16_t l_39 = 65535U;
    uint64_t *l_41[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
    uint64_t *l_48 = (void*)0;
    int32_t l_60[3];
    const int32_t **l_71 = (void*)0;
    int32_t ** const l_72 = (void*)0;
    int32_t *l_83[1][4] = {{&l_60[2],&l_60[2],&l_60[2],&l_60[2]}};
    uint16_t l_87 = 0xD34D;
    int i, j;
    for (i = 0; i < 3; i++)
        l_60[i] = 0x834D56B8;
    for (p_11 = (-29); (p_11 >= 6); p_11++)
    { 
        int16_t *l_28 = &g_29;
        int32_t l_33 = 0xED050B7F;
        uint64_t **l_43 = &l_41[2];
        uint64_t *l_45[4];
        uint64_t **l_44 = &l_45[1];
        uint64_t *l_47 = &g_42;
        uint64_t **l_46[2][2] = {{&l_47,&l_47},{&l_47,&l_47}};
        int i, j;
        for (i = 0; i < 4; i++)
            l_45[i] = &g_42;
        for (p_10 = 15; (p_10 == 56); p_10++)
        { 
            uint64_t l_25 = 8U;
            int32_t *l_26[1];
            int i;
            for (i = 0; i < 1; i++)
                l_26[i] = (void*)0;
            g_27 = l_25;
        }
        if ((0x3E5EC35211521FAFLL | ((((*l_28) ^= g_12[0][3]) || 0x2585) || (-1))))
        { 
            for (g_29 = 3; (g_29 >= 0); g_29 -= 1)
            { 
                int32_t **l_32 = &g_30;
                int i, j;
                (*l_32) = g_30;
                (*l_32) = (*l_32);
                if (g_12[g_29][g_29])
                    continue;
            }
        }
        else
        { 
            uint8_t *l_37 = &g_27;
            uint64_t *l_38[2];
            int i;
            for (i = 0; i < 2; i++)
                l_38[i] = (void*)0;
            (*g_30) = (((l_33 != (l_39 = (((safe_sub_func_uint8_t_u_u((((*l_37) = (&g_12[3][0] == (g_36 = &g_29))) || p_11), 0xD3)) , (*g_36)) ^ g_29))) != p_11) & 1);
        }
        (*g_30) ^= ((!18446744073709551613U) != (((*l_44) = ((*l_43) = l_41[2])) != (l_48 = &g_42)));
    }
    for (g_27 = (-28); (g_27 >= 16); g_27++)
    { 
        int64_t l_51 = (-4);
        int32_t *l_52 = &g_31;
        int32_t l_53[3][2] = {{0xA80146BF,0xA80146BF},{0xA80146BF,0xA80146BF},{0xA80146BF,0xA80146BF}};
        int32_t *l_54 = &l_53[0][0];
        int32_t *l_55 = &g_31;
        int32_t *l_56 = (void*)0;
        int32_t *l_57 = &g_31;
        int32_t *l_58 = &g_31;
        int32_t *l_59[3];
        int i, j;
        for (i = 0; i < 3; i++)
            l_59[i] = &l_53[0][0];
        g_61--;
        for (g_42 = 0; (g_42 == 60); g_42 = safe_add_func_int8_t_s_s(g_42, 5))
        { 
            int64_t *l_68 = &l_51;
            int32_t l_73[2];
            int32_t * const *l_79[3];
            int i;
            for (i = 0; i < 2; i++)
                l_73[i] = 0xDAD933B1;
            for (i = 0; i < 3; i++)
                l_79[i] = &l_56;
            if ((safe_sub_func_int64_t_s_s(((*l_68) = (*l_55)), ((*g_30) != (safe_rshift_func_int8_t_s_s((l_71 == l_72), g_12[0][0]))))))
            { 
                l_73[1] |= (*g_30);
            }
            else
            { 
                uint16_t *l_74 = &l_39;
                int8_t *l_80[3][2];
                int32_t l_82 = 0xFF636611;
                int i, j;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_80[i][j] = &g_81;
                }
                (*l_52) ^= (((*l_74)--) || (0xD39A040D4F663622LL == (65529U < (safe_add_func_int64_t_s_s(((((l_60[2] ^= (l_79[1] != &g_30)) ^ g_42) | 4U) ^ p_10), l_82)))));
            }
        }
    }
    --g_84;
    return l_87;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_12[i][j], "g_12[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_31, "g_31", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_84, "g_84", print_hash_value);
    transparent_crc(g_89, "g_89", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
