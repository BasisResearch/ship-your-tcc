/* Csmith 0cdc710315cf seed 325, flattened by scripts/csmith_flatten.py */
// Options:   --seed 325 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint8_t  f0;
   const uint64_t  f1;
   int32_t  f2;
   uint16_t  f3;
};

static int32_t g_6 = 4;
static int32_t g_27 = 0xF57440E7;
static int32_t *g_39 = (void*)0;
static int64_t g_42 = 0x72F236BBBD28A218LL;
static int32_t **g_45 = &g_39;
static int64_t *g_52 = &g_42;
static int64_t **g_51 = &g_52;

static uint16_t  func_1(void);
static int32_t  func_7(uint8_t  p_8);
static struct S0  func_11(int32_t * p_12);

static uint16_t  func_1(void)
{ 
    uint8_t l_2[4][4] = {{251U,1U,1U,251U},{251U,1U,1U,251U},{251U,1U,1U,251U},{251U,1U,1U,251U}};
    uint8_t l_31 = 255U;
    int32_t *l_35 = &g_6;
    int64_t **l_53[4];
    int i, j;
    for (i = 0; i < 4; i++)
        l_53[i] = &g_52;
    if (l_2[3][2])
    { 
        int16_t l_3[3][2] = {{3,3},{3,3},{3,3}};
        int32_t *l_4 = (void*)0;
        int32_t *l_5[1];
        int i, j;
        for (i = 0; i < 1; i++)
            l_5[i] = &g_6;
        if ((l_31 = ((g_6 = l_3[2][1]) , func_7(g_6))))
        { 
            uint8_t l_32 = 248U;
            uint64_t l_34 = 0xD31FC531364F2CA1LL;
            if (((l_5[0] != ((l_32 == (((safe_unary_minus_func_uint16_t_u(g_27)) , l_32) <= l_34)) , l_35)) == g_6))
            { 
                int32_t **l_38[2][1];
                int64_t *l_40 = (void*)0;
                int64_t *l_41 = &g_42;
                int i, j;
                for (i = 0; i < 2; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_38[i][j] = &l_4;
                }
                (*l_35) = (safe_mul_func_int8_t_s_s(((g_39 = l_5[0]) != (((*l_41) = l_32) , &g_27)), 249U));
                return g_27;
            }
            else
            { 
                g_39 = (func_11(&g_6) , &g_27);
                (*g_39) |= (g_6 , (g_42 > (safe_rshift_func_int16_t_s_u(1, g_6))));
                return (*l_35);
            }
        }
        else
        { 
            for (g_27 = 3; (g_27 >= 0); g_27 -= 1)
            { 
                g_39 = &g_6;
            }
            return g_42;
        }
    }
    else
    { 
        g_45 = &g_39;
        for (l_31 = 0; (l_31 <= 3); l_31 += 1)
        { 
            return g_6;
        }
        (*g_45) = (*g_45);
    }
    for (g_27 = 3; (g_27 >= 0); g_27 -= 1)
    { 
        int32_t **l_46 = &l_35;
        l_46 = &g_39;
        (*g_45) = (*l_46);
        for (g_42 = 0; (g_42 <= 3); g_42 += 1)
        { 
            int32_t **l_47 = &l_35;
            l_47 = &g_39;
        }
        for (g_42 = 3; (g_42 >= 0); g_42 -= 1)
        { 
            int i, j;
            if (l_2[g_42][g_27])
                break;
            for (l_31 = 0; (l_31 <= 3); l_31 += 1)
            { 
                int32_t l_48 = 5;
                int i, j;
                l_48 = l_2[g_27][l_31];
                g_51 = ((safe_div_func_uint64_t_u_u(g_42, g_27)) , (l_53[0] = (g_6 , g_51)));
            }
        }
    }
    for (g_42 = 24; (g_42 <= 24); g_42++)
    { 
        return (*l_35);
    }
    return g_42;
}

static int32_t  func_7(uint8_t  p_8)
{ 
    int8_t l_30 = 0x55;
    for (p_8 = 29; (p_8 <= 18); --p_8)
    { 
        int32_t *l_25 = (void*)0;
        int32_t *l_26 = &g_27;
        (*l_26) ^= (func_11(&g_6) , 0x0D42AD01);
        g_6 = p_8;
    }
    for (p_8 = 0; (p_8 < 13); p_8 = safe_add_func_uint32_t_u_u(p_8, 2))
    { 
        if (l_30)
            break;
    }
    return g_27;
}

static struct S0  func_11(int32_t * p_12)
{ 
    int16_t l_15 = 0x45BA;
    uint64_t l_23[1];
    struct S0 l_24 = {0x38,1U,-9,0x5950};
    int i;
    for (i = 0; i < 1; i++)
        l_23[i] = 18446744073709551615U;
    (*p_12) = (safe_lshift_func_int8_t_s_u(l_15, ((safe_div_func_uint16_t_u_u(((safe_rshift_func_int16_t_s_u((((safe_lshift_func_int16_t_s_s((+(-3)), 0)) != ((g_6 == 8U) > g_6)) , l_23[0]), 8)) | l_23[0]), l_23[0])) , 7U)));
    return l_24;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
