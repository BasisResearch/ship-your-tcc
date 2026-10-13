/* Csmith 0cdc710315cf seed 114, flattened by scripts/csmith_flatten.py */
// Options:   --seed 114 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint64_t  f0;
};

union U1 {
   int32_t  f0;
   int16_t  f1;
   uint64_t  f2;
};

static uint64_t g_3 = 18446744073709551612U;
static int32_t g_7 = 0;
static int32_t g_20 = 4;
static union U0 g_37[2] = {{0x0FC24489BE67E784LL},{0x0FC24489BE67E784LL}};
static uint16_t g_69 = 1U;
static int64_t g_73 = (-4);
static uint8_t g_75 = 0xDB;
static int32_t *g_81 = &g_20;

static int32_t  func_1(void);
static uint16_t  func_25(uint64_t  p_26, int64_t  p_27, int32_t ** p_28, union U1  p_29);
static uint64_t  func_30(union U0  p_31, uint64_t * p_32, int32_t  p_33);

static int32_t  func_1(void)
{ 
    uint64_t *l_2 = &g_3;
    int32_t *l_6 = &g_7;
    const uint32_t l_14 = 0xEB969161;
    int32_t l_74 = (-9);
    int32_t **l_80[2];
    int8_t l_82 = 0xD1;
    int i;
    for (i = 0; i < 2; i++)
        l_80[i] = &l_6;
    if ((18446744073709551614U > (--(*l_2))))
    { 
        int32_t **l_8 = (void*)0;
        int32_t **l_9 = &l_6;
        int32_t *l_19 = &g_20;
        (*l_9) = l_6;
        g_7 = 0xD2AB60E7;
        (*l_19) |= (safe_lshift_func_int16_t_s_s((safe_div_func_int32_t_s_s(((0xC213 && l_14) != (safe_sub_func_int16_t_s_s((safe_add_func_int16_t_s_s((**l_9), (*l_6))), (*l_6)))), (-8))), 9));
    }
    else
    { 
        uint32_t l_23 = 0xC5EC62BB;
        uint64_t *l_36 = (void*)0;
        int32_t **l_41 = &l_6;
        for (g_3 = 0; (g_3 < 30); ++g_3)
        { 
            uint64_t *l_38 = &g_37[1].f0;
            int32_t l_40 = (-3);
            union U1 l_42 = {-1};
            int32_t *l_67 = (void*)0;
            int32_t *l_68 = &l_42.f0;
            int32_t *l_70 = &g_7;
            int32_t *l_71 = &g_20;
            int32_t *l_72[3];
            int i;
            for (i = 0; i < 3; i++)
                l_72[i] = &l_42.f0;
            g_69 |= ((*l_68) = (l_23 & ((+func_25(func_30((((65531U ^ (((g_3 , l_36) != (void*)0) == g_7)) != g_3) , g_37[1]), l_38, g_37[1].f0), l_40, l_41, l_42)) , 4294967295U)));
            --g_75;
            for (g_75 = 2; (g_75 == 48); g_75++)
            { 
                (*l_70) = 0x7FB90EFD;
            }
        }
        (*l_6) ^= 0x33967BE2;
    }
    g_81 = &l_74;
    return l_82;
}

static uint16_t  func_25(uint64_t  p_26, int64_t  p_27, int32_t ** p_28, union U1  p_29)
{ 
    int8_t l_43 = 1;
    if (l_43)
    { 
        int16_t l_44 = (-3);
        int32_t l_47 = (-2);
        l_47 ^= (l_44 > (((((l_44 <= (((safe_mod_func_int16_t_s_s(g_3, (-1))) >= g_7) < l_43)) < g_37[1].f0) && p_27) & 0x8C46) , 0xD28B));
        (**p_28) = ((p_29.f0 | ((safe_lshift_func_int64_t_s_u(((safe_div_func_uint64_t_u_u((((~((((safe_lshift_func_int16_t_s_u(0x15F7, (safe_unary_minus_func_uint64_t_u((safe_mul_func_uint32_t_u_u(p_26, 0x5A5285B5)))))) , &g_7) == &l_47) , 1U)) & 4294967295U) ^ l_44), l_47)) || 1), 32)) ^ 0U)) > g_3);
        for (g_7 = 0; (g_7 <= 1); g_7 += 1)
        { 
            for (p_29.f1 = 0; (p_29.f1 <= 1); p_29.f1 += 1)
            { 
                (*p_28) = (*p_28);
            }
            for (p_26 = 0; (p_26 <= 1); p_26 += 1)
            { 
                if (l_43)
                    break;
            }
            for (p_26 = 0; (p_26 <= 1); p_26 += 1)
            { 
                uint32_t l_58 = 0x64560F91;
                if ((**p_28))
                    break;
                return l_58;
            }
        }
    }
    else
    { 
        int32_t *l_59 = (void*)0;
        int32_t *l_60 = &g_7;
        (*p_28) = &g_20;
        (*l_60) ^= (**p_28);
    }
    for (p_29.f1 = 1; (p_29.f1 > 25); p_29.f1 = safe_add_func_uint16_t_u_u(p_29.f1, 1))
    { 
        g_20 |= (g_7 = g_37[1].f0);
    }
    for (p_26 = 0; (p_26 <= 41); ++p_26)
    { 
        for (p_29.f0 = 1; (p_29.f0 >= 0); p_29.f0 -= 1)
        { 
            int32_t *l_66 = &g_7;
            int32_t **l_65 = &l_66;
            (*l_65) = ((*p_28) = &g_20);
            (**p_28) = g_3;
            (*l_65) = (void*)0;
        }
    }
    return l_43;
}

static uint64_t  func_30(union U0  p_31, uint64_t * p_32, int32_t  p_33)
{ 
    int32_t *l_39 = &g_20;
    (*l_39) |= 2;
    l_39 = l_39;
    return (*l_39);
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_37[i].f0, "g_37[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_73, "g_73", print_hash_value);
    transparent_crc(g_75, "g_75", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
