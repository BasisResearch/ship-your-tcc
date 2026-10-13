/* Csmith 0cdc710315cf seed 303, flattened by scripts/csmith_flatten.py */
// Options:   --seed 303 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint64_t  f0;
};

struct S1 {
   uint16_t  f0;
};

struct S2 {
   int64_t  f0;
   int64_t  f1;
   struct S1  f2;
   uint16_t  f3;
};

static uint16_t g_2 = 0x8A6B;
static struct S1 g_22[3] = {{4U},{4U},{4U}};
static uint16_t g_25 = 0x6689;
static uint16_t g_28 = 1U;
static uint8_t g_32 = 1U;
static int16_t g_35 = 0;
static struct S2 g_36 = {0x251F81F13D46409ELL,4,{0x248C},0U};
static int32_t g_40 = 0xB4F7BC72;
static struct S0 g_43 = {0U};
static uint32_t g_46[1] = {4294967292U};
static int64_t g_57[3][1] = {{0x8934983969C113E7LL},{0x8934983969C113E7LL},{0x8934983969C113E7LL}};
static struct S0 *g_63 = &g_43;
static struct S0 **g_62 = &g_63;
static struct S0 **g_65[3][3] = {{&g_63,&g_63,&g_63},{&g_63,&g_63,&g_63},{&g_63,&g_63,&g_63}};

static uint32_t  func_1(void);
static struct S2  func_3(int16_t  p_4);
static int8_t  func_9(uint64_t  p_10);

static uint32_t  func_1(void)
{ 
    struct S0 l_23 = {0x11B9C6FEC1B16EB2LL};
    uint64_t l_33[4] = {18446744073709551613U,18446744073709551613U,18446744073709551613U,18446744073709551613U};
    int i;
    if (g_2)
    { 
        uint64_t l_15[3];
        uint16_t *l_24 = &g_25;
        uint16_t *l_26 = (void*)0;
        uint16_t *l_27[1];
        int32_t l_29 = 0x417F95E1;
        uint16_t l_30 = 0x2D14;
        uint8_t *l_31 = &g_32;
        int16_t *l_34 = &g_35;
        int i;
        for (i = 0; i < 3; i++)
            l_15[i] = 0x488F4F368A9017CALL;
        for (i = 0; i < 1; i++)
            l_27[i] = &g_28;
        g_36 = func_3(((*l_34) = (safe_add_func_uint64_t_u_u((safe_mod_func_int8_t_s_s(func_9(((((*l_31) = (safe_mod_func_uint16_t_u_u((safe_div_func_int16_t_s_s(l_15[1], (l_30 &= (safe_mul_func_int16_t_s_s((safe_mod_func_uint32_t_u_u((g_2 , (safe_sub_func_uint8_t_u_u(((l_29 = (g_22[0].f0 = ((*l_24) |= (g_2 &= (((((((g_22[0] , l_23) , g_22[0].f0) ^ l_15[1]) || 2U) , 8U) != 0xDD9AE65C) == l_23.f0))))) > 0x9BFA), l_23.f0))), l_15[0])), g_28))))), l_23.f0))) && l_33[0]) , l_15[2])), l_15[2])), g_28))));
    }
    else
    { 
        int32_t *l_51 = &g_40;
        struct S0 **l_64[2][2] = {{&g_63,&g_63},{&g_63,&g_63}};
        int32_t **l_66 = &l_51;
        int i, j;
        for (g_35 = 0; (g_35 < (-17)); g_35 = safe_sub_func_uint32_t_u_u(g_35, 5))
        { 
            struct S0 *l_56 = &g_43;
            struct S0 **l_55 = &l_56;
            l_51 = l_51;
            if ((((+(safe_mul_func_uint8_t_u_u((g_25 ^ (((*l_55) = &g_43) == (void*)0)), g_32))) < g_35) != g_2))
            { 
                g_57[0][0] ^= (g_36.f1 || ((*l_51) ^= g_36.f2.f0));
            }
            else
            { 
                return g_32;
            }
        }
        (*l_66) = (((l_23 , (((safe_sub_func_uint64_t_u_u((safe_rshift_func_int16_t_s_u(((g_62 == (g_65[0][2] = l_64[0][1])) ^ 0xA7), 15)), g_32)) & g_36.f0) <= 0xBBAE6652)) , g_22[0].f0) , (void*)0);
    }
    return l_33[3];
}

static struct S2  func_3(int16_t  p_4)
{ 
    for (g_32 = 0; (g_32 <= 2); g_32 += 1)
    { 
        for (g_2 = 0; (g_2 <= 2); g_2 += 1)
        { 
            for (g_28 = 0; (g_28 <= 2); g_28 += 1)
            { 
                return g_36;
            }
        }
        for (g_35 = 2; (g_35 >= 0); g_35 -= 1)
        { 
            int32_t *l_37 = (void*)0;
            int32_t *l_38 = (void*)0;
            int32_t *l_39 = &g_40;
            struct S0 *l_42[3][2] = {{&g_43,&g_43},{&g_43,&g_43},{&g_43,&g_43}};
            struct S0 **l_41 = &l_42[2][0];
            int32_t *l_44 = &g_40;
            int32_t *l_45[1];
            int i, j;
            for (i = 0; i < 1; i++)
                l_45[i] = &g_40;
            (*l_39) = g_36.f2.f0;
            (*l_41) = (void*)0;
            g_46[0]++;
        }
    }
    return g_36;
}

static int8_t  func_9(uint64_t  p_10)
{ 
    return g_2;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_22[i].f0, "g_22[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_36.f0, "g_36.f0", print_hash_value);
    transparent_crc(g_36.f1, "g_36.f1", print_hash_value);
    transparent_crc(g_36.f2.f0, "g_36.f2.f0", print_hash_value);
    transparent_crc(g_36.f3, "g_36.f3", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    transparent_crc(g_43.f0, "g_43.f0", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_46[i], "g_46[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_57[i][j], "g_57[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
