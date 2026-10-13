/* Csmith 0cdc710315cf seed 264, flattened by scripts/csmith_flatten.py */
// Options:   --seed 264 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   const uint8_t  f0;
};

static uint16_t g_3 = 3U;
static int32_t g_5 = 0x3281D21F;
static uint8_t g_13[4][4] = {{253U,253U,253U,253U},{253U,253U,253U,253U},{253U,253U,253U,253U},{253U,253U,253U,253U}};
static union U0 g_22 = {0U};
static int32_t g_26 = 0x717B38E5;
static union U0 *g_30 = &g_22;
static union U0 **g_29[4] = {&g_30,&g_30,&g_30,&g_30};
static int32_t g_33 = 9;
static uint64_t g_34 = 0xD73C12D4938140A9LL;
static union U0 g_54 = {254U};

static int16_t  func_1(void);
static int32_t * func_7(int32_t * p_8, int32_t * p_9, uint32_t  p_10, int32_t  p_11);
static union U0 * func_17(union U0 * p_18, int32_t * p_19, int16_t  p_20);

static int16_t  func_1(void)
{ 
    int64_t l_2[2];
    int32_t *l_4[3][4] = {{&g_5,&g_5,&g_5,&g_5},{&g_5,&g_5,&g_5,&g_5},{&g_5,&g_5,&g_5,&g_5}};
    uint8_t l_6 = 255U;
    int32_t **l_66 = &l_4[2][0];
    uint8_t l_67 = 0U;
    int i, j;
    for (i = 0; i < 2; i++)
        l_2[i] = 6;
    l_6 = (l_2[0] != ((0 | 0x801FC3EEE2FDE2BBLL) , g_3));
    (*l_66) = func_7(&g_5, l_4[1][1], (+1U), g_5);
    return l_67;
}

static int32_t * func_7(int32_t * p_8, int32_t * p_9, uint32_t  p_10, int32_t  p_11)
{ 
    int32_t * const l_16[4] = {&g_5,&g_5,&g_5,&g_5};
    union U0 *l_53 = &g_54;
    int i;
    for (g_3 = 0; (g_3 <= 3); g_3 += 1)
    { 
        union U0 *l_21 = &g_22;
        int32_t l_58[2][2] = {{0x2C352423,0x2C352423},{0x2C352423,0x2C352423}};
        int i, j;
        for (p_11 = 3; (p_11 >= 0); p_11 -= 1)
        { 
            int32_t *l_15 = (void*)0;
            int32_t **l_14 = &l_15;
            (*l_14) = &p_11;
        }
        for (p_11 = 3; (p_11 >= 0); p_11 -= 1)
        { 
            p_9 = l_16[0];
        }
        for (g_5 = 0; (g_5 <= 3); g_5 += 1)
        { 
            int32_t *l_23 = (void*)0;
            union U0 **l_52 = (void*)0;
            int i, j;
            p_11 = g_13[g_5][g_5];
            l_53 = func_17(l_21, l_23, p_11);
            for (g_26 = 0; (g_26 <= 3); g_26 += 1)
            { 
                uint8_t l_65 = 3U;
                int i;
                g_29[g_26] = (((((((safe_div_func_uint8_t_u_u((~l_58[0][1]), (safe_mod_func_int64_t_s_s((-7), (safe_sub_func_int16_t_s_s((safe_mod_func_int64_t_s_s((((*p_8) != 4294967294U) , g_3), l_58[0][1])), 0xD8D7)))))) || 0U) ^ g_13[1][1]) != l_65) , l_53) != l_53) , (void*)0);
            }
        }
    }
    return &g_33;
}

static union U0 * func_17(union U0 * p_18, int32_t * p_19, int16_t  p_20)
{ 
    int32_t *l_25 = &g_5;
    int32_t **l_24 = &l_25;
    int32_t l_38 = 8;
    union U0 *l_51 = &g_22;
    (*l_24) = &g_5;
    for (p_20 = 0; (p_20 <= 3); p_20 += 1)
    { 
        int32_t *l_40[3][1];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 1; j++)
                l_40[i][j] = (void*)0;
        }
        p_18 = p_18;
        for (g_26 = 3; (g_26 >= 0); g_26 -= 1)
        { 
            union U0 **l_31 = (void*)0;
            int32_t *l_37[2];
            int i, j;
            for (i = 0; i < 2; i++)
                l_37[i] = &g_33;
            if (g_13[g_26][p_20])
            { 
                union U0 *l_28 = &g_22;
                union U0 **l_27 = &l_28;
                (*l_27) = p_18;
                l_31 = g_29[1];
            }
            else
            { 
                int32_t *l_32 = &g_33;
                ++g_34;
            }
            l_38 ^= 0xDB2A93F0;
        }
        for (l_38 = 0; (l_38 <= 3); l_38 += 1)
        { 
            int32_t *l_39 = &l_38;
            int32_t l_42 = 0;
            int32_t *l_50 = &g_26;
            for (g_33 = 3; (g_33 >= 0); g_33 -= 1)
            { 
                if ((**l_24))
                    break;
                l_42 = (((((l_39 != (l_40[1][0] = p_19)) , (safe_unary_minus_func_int16_t_s((((((((void*)0 != &g_5) , 0x9C67) , 0x9898791B) >= g_13[1][3]) || (*l_25)) , 0x554C)))) , p_20) & 0x91) < (*l_25));
            }
            for (g_26 = 3; (g_26 >= 0); g_26 -= 1)
            { 
                int32_t l_49 = 2;
                int i, j;
            }
        }
    }
    return l_51;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_5, "g_5", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_13[i][j], "g_13[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_22.f0, "g_22.f0", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_33, "g_33", print_hash_value);
    transparent_crc(g_34, "g_34", print_hash_value);
    transparent_crc(g_54.f0, "g_54.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
