/* Csmith 0cdc710315cf seed 159, flattened by scripts/csmith_flatten.py */
// Options:   --seed 159 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static uint32_t g_7[1][4] = {{4294967295U,4294967295U,4294967295U,4294967295U}};
static int32_t g_10 = 0xDC660F7E;
static int32_t * const g_9 = &g_10;
static int32_t g_27 = 0;
static uint8_t g_29 = 0x26;
static int32_t *g_38 = &g_27;

static uint64_t  func_1(void);
static int32_t * func_2(uint64_t  p_3, int16_t  p_4, int8_t  p_5, int32_t * const  p_6);
static int32_t * func_15(const int64_t  p_16, int64_t  p_17, uint32_t  p_18);

static uint64_t  func_1(void)
{ 
    int32_t l_8[1][3];
    int32_t **l_41 = &g_38;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_8[i][j] = 0xC1574781;
    }
    (*l_41) = func_2((g_7[0][3] , l_8[0][2]), (g_7[0][2] > l_8[0][2]), g_7[0][3], g_9);
    return g_10;
}

static int32_t * func_2(uint64_t  p_3, int16_t  p_4, int8_t  p_5, int32_t * const  p_6)
{ 
    int32_t *l_40 = &g_27;
    for (g_10 = 29; (g_10 >= 10); g_10 = safe_sub_func_int16_t_s_s(g_10, 5))
    { 
        int32_t l_37 = 0x9E588CE3;
        for (p_4 = 0; (p_4 >= 29); p_4++)
        { 
            uint32_t l_25 = 0xACAE7BF1;
            int32_t **l_39[1][2];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 2; j++)
                    l_39[i][j] = &g_38;
            }
            for (p_3 = 0; (p_3 <= 0); p_3 += 1)
            { 
                int32_t **l_34 = (void*)0;
                int32_t *l_36 = &g_27;
                int32_t **l_35 = &l_36;
                int i, j;
                (*l_35) = func_15(((safe_add_func_int16_t_s_s((safe_sub_func_uint64_t_u_u((g_7[p_3][(p_3 + 1)] != (1 > (safe_rshift_func_uint32_t_u_s((((void*)0 != p_6) | p_4), 22)))), p_3)), g_7[0][1])) | l_25), p_4, p_4);
                g_38 = func_15(l_25, (((void*)0 != &g_27) != l_25), l_37);
                if ((*p_6))
                    continue;
            }
            if ((*g_38))
                break;
            g_38 = p_6;
        }
    }
    return l_40;
}

static int32_t * func_15(const int64_t  p_16, int64_t  p_17, uint32_t  p_18)
{ 
    int32_t l_28 = 1;
    int32_t *l_32 = (void*)0;
    for (p_17 = 0; (p_17 >= 0); p_17 -= 1)
    { 
        int32_t *l_26[2];
        int i;
        for (i = 0; i < 2; i++)
            l_26[i] = &g_27;
        ++g_29;
        for (p_18 = 0; (p_18 <= 0); p_18 += 1)
        { 
            int32_t **l_33 = &l_32;
            l_32 = l_26[0];
            (*l_33) = &l_28;
            return l_26[0];
        }
    }
    return &g_27;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_7[i][j], "g_7[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
