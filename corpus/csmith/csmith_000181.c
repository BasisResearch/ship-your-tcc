/* Csmith 0cdc710315cf seed 181, flattened by scripts/csmith_flatten.py */
// Options:   --seed 181 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_rshift_func_int16_t_s_s(int16_t left, int right )
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
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint16_t  f0;
   uint16_t  f1;
   uint64_t  f2;
   int8_t  f3;
};

struct S1 {
   uint16_t  f0;
};

static int16_t g_12[3][4] = {{0xBEA5,0xBEA5,0xBEA5,0xBEA5},{0xBEA5,0xBEA5,0xBEA5,0xBEA5},{0xBEA5,0xBEA5,0xBEA5,0xBEA5}};
static struct S1 g_21 = {0U};
static int32_t g_24 = (-2);
static struct S0 g_28 = {65535U,0x067B,0x6200D40A7FCC65ADLL,0};
static struct S0 *g_27 = &g_28;

static int8_t  func_1(void);
static struct S1  func_2(uint64_t  p_3, int8_t  p_4, uint32_t  p_5, int64_t  p_6);
static int16_t  func_9(int32_t  p_10);

static int8_t  func_1(void)
{ 
    int16_t l_11 = 1;
    struct S1 *l_30 = &g_21;
    uint64_t l_31 = 0xDDA3143253674F9ALL;
    int32_t *l_32 = &g_24;
    (*l_30) = func_2(((safe_rshift_func_int16_t_s_s(func_9(l_11), 2)) == g_12[1][1]), g_12[1][1], l_11, l_11);
    (*l_32) &= (7 < l_31);
    return (*l_32);
}

static struct S1  func_2(uint64_t  p_3, int8_t  p_4, uint32_t  p_5, int64_t  p_6)
{ 
    struct S1 l_15 = {0xDB97};
    for (p_6 = (-21); (p_6 > (-25)); p_6 = safe_sub_func_uint16_t_u_u(p_6, 3))
    { 
        return l_15;
    }
    for (p_4 = 0; (p_4 != 21); p_4++)
    { 
        struct S1 l_20[4] = {{65535U},{65535U},{65535U},{65535U}};
        int i;
        for (p_6 = 0; (p_6 <= 2); p_6 += 1)
        { 
            struct S1 l_18 = {0x9CC0};
            struct S1 *l_19[2][1];
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_19[i][j] = &l_18;
            }
            l_20[2] = l_18;
        }
        if ((((g_21 , 0x70) == p_4) <= g_12[0][3]))
        { 
            int8_t l_22 = 0x23;
            l_22 |= ((void*)0 == &l_20[3]);
        }
        else
        { 
            int32_t *l_23[3][4] = {{&g_24,&g_24,&g_24,&g_24},{&g_24,&g_24,&g_24,&g_24},{&g_24,&g_24,&g_24,&g_24}};
            int i, j;
            g_24 = l_15.f0;
            for (p_3 = (-18); (p_3 >= 54); p_3 = safe_add_func_uint64_t_u_u(p_3, 1))
            { 
                struct S0 **l_29 = &g_27;
                (*l_29) = g_27;
            }
        }
    }
    return g_21;
}

static int16_t  func_9(int32_t  p_10)
{ 
    return g_12[0][1];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_12[i][j], "g_12[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_21.f0, "g_21.f0", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_28.f0, "g_28.f0", print_hash_value);
    transparent_crc(g_28.f1, "g_28.f1", print_hash_value);
    transparent_crc(g_28.f2, "g_28.f2", print_hash_value);
    transparent_crc(g_28.f3, "g_28.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
