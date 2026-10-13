/* Csmith 0cdc710315cf seed 253, flattened by scripts/csmith_flatten.py */
// Options:   --seed 253 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
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
   uint32_t  f0;
   int8_t  f1;
   int16_t  f2;
   const int16_t  f3;
};

struct S1 {
   const int64_t  f0;
};

union U3 {
   uint32_t  f0;
   uint8_t  f1;
};

static int32_t g_2 = 0;
static int32_t g_5 = 1;
static union U3 g_13[4] = {{4294967295U},{4294967295U},{4294967295U},{4294967295U}};
static struct S0 g_19 = {0x478C9DFF,-9,0x4996,0xCB66};
static struct S0 *g_18 = &g_19;
static struct S0 g_22 = {7U,0x6E,0x0424,0x969B};
static int32_t g_27 = 0xD17CD953;
static int32_t g_40 = (-2);
static struct S1 g_49[1][4] = {{{1},{1},{1},{1}}};
static int32_t **g_50 = (void*)0;
static uint64_t g_63 = 0xBB0E3CFE890704A9LL;
static struct S0 **g_64 = &g_18;
static int32_t g_68 = 2;

static uint8_t  func_1(void);
static uint8_t  func_8(union U3  p_9, uint32_t  p_10, const union U3  p_11, uint16_t  p_12);
static uint8_t  func_16(int8_t  p_17);

static uint8_t  func_1(void)
{ 
    const union U3 l_14 = {0x59008CF5};
    for (g_2 = 0; (g_2 <= 18); g_2 = safe_add_func_int8_t_s_s(g_2, 5))
    { 
        uint8_t l_15[1];
        int i;
        for (i = 0; i < 1; i++)
            l_15[i] = 0U;
        for (g_5 = 2; (g_5 <= (-19)); g_5 = safe_sub_func_int8_t_s_s(g_5, 8))
        { 
            uint8_t l_66 = 0x0F;
            int32_t *l_67 = &g_68;
        }
    }
    return g_49[0][3].f0;
}

static uint8_t  func_8(union U3  p_9, uint32_t  p_10, const union U3  p_11, uint16_t  p_12)
{ 
    int16_t l_43 = 0x198F;
    for (p_9.f1 = 0; (p_9.f1 <= 3); p_9.f1 += 1)
    { 
        for (p_12 = 0; (p_12 <= 3); p_12 += 1)
        { 
            int32_t *l_39 = &g_40;
            (*l_39) ^= (func_16(g_2) | (g_19.f0 && (-6)));
            g_27 = ((*l_39) = (safe_sub_func_int8_t_s_s(l_43, (((((safe_div_func_int64_t_s_s(((0 <= ((safe_add_func_int8_t_s_s((!((g_49[0][3] , g_50) == (void*)0)), 0x69)) , 5U)) || (*l_39)), (-1))) <= p_11.f0) != g_19.f0) || p_12) | (*l_39)))));
            (*l_39) = (((*l_39) , g_19.f1) >= ((3 >= p_10) < p_9.f1));
        }
        return p_12;
    }
    for (g_22.f0 = 0; (g_22.f0 <= 3); g_22.f0 += 1)
    { 
        int32_t *l_51 = &g_27;
        uint8_t *l_56 = (void*)0;
        uint8_t *l_57 = &g_13[2].f1;
        const struct S1 l_60 = {0x3BF156DE04B380B7LL};
        uint64_t *l_61 = (void*)0;
        uint64_t *l_62 = &g_63;
        int32_t **l_65 = &l_51;
        (*l_51) = ((-3) < p_12);
        g_64 = ((((safe_add_func_int16_t_s_s((safe_mod_func_uint8_t_u_u(((*l_57) = (*l_51)), (safe_rshift_func_uint8_t_u_u(5U, 3)))), ((((*l_62) = ((l_60 , p_10) , g_27)) ^ (-3)) >= p_11.f0))) && (-2)) <= g_40) , g_64);
        (*l_65) = &g_2;
    }
    return p_12;
}

static uint8_t  func_16(int8_t  p_17)
{ 
    struct S0 *l_21 = &g_22;
    int32_t *l_26 = &g_27;
    uint8_t l_34[1][2];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_34[i][j] = 7U;
    }
    if (p_17)
    { 
        struct S0 **l_20 = (void*)0;
        struct S0 **l_23 = &g_18;
        int32_t *l_25 = (void*)0;
        int32_t **l_24[3];
        int i;
        for (i = 0; i < 3; i++)
            l_24[i] = &l_25;
        (*l_23) = (l_21 = g_18);
        l_26 = (void*)0;
    }
    else
    { 
        uint8_t l_32 = 254U;
        int32_t l_35 = 1;
        if ((safe_div_func_int16_t_s_s((safe_add_func_int8_t_s_s(((*l_26) < l_32), ((((+(l_35 = (l_34[0][1] && (-5)))) == g_22.f0) != l_32) | g_22.f2))), g_27)))
        { 
            uint64_t l_36 = 7U;
            return l_36;
        }
        else
        { 
            for (l_32 = 25; (l_32 == 44); l_32++)
            { 
                return p_17;
            }
        }
    }
    return g_22.f3;
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
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_13[i].f0, "g_13[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_19.f0, "g_19.f0", print_hash_value);
    transparent_crc(g_19.f1, "g_19.f1", print_hash_value);
    transparent_crc(g_19.f2, "g_19.f2", print_hash_value);
    transparent_crc(g_19.f3, "g_19.f3", print_hash_value);
    transparent_crc(g_22.f0, "g_22.f0", print_hash_value);
    transparent_crc(g_22.f1, "g_22.f1", print_hash_value);
    transparent_crc(g_22.f2, "g_22.f2", print_hash_value);
    transparent_crc(g_22.f3, "g_22.f3", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_49[i][j].f0, "g_49[i][j].f0", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_68, "g_68", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
