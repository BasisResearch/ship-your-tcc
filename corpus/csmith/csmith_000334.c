/* Csmith 0cdc710315cf seed 334, flattened by scripts/csmith_flatten.py */
// Options:   --seed 334 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 1;
static int32_t g_7 = 6;
static uint16_t g_14 = 65527U;
static int32_t g_24 = 1;
static int32_t *g_31 = &g_2;
static int32_t **g_30 = &g_31;
static int32_t g_35 = (-1);
static uint16_t g_38 = 7U;
static uint32_t g_42 = 4294967295U;

static int64_t  func_1(void);
static const int32_t  func_8(int32_t * p_9);
static int32_t * func_10(uint8_t  p_11);

static int64_t  func_1(void)
{ 
    uint32_t l_5[3][2];
    const int32_t l_40 = (-6);
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_5[i][j] = 0xB88146CD;
    }
    for (g_2 = 20; (g_2 != (-28)); g_2 = safe_sub_func_int16_t_s_s(g_2, 4))
    { 
        int32_t *l_6 = &g_7;
        (*l_6) ^= l_5[2][0];
        (*l_6) = func_8(func_10((safe_div_func_uint64_t_u_u(((g_14 = (g_7 , ((void*)0 != &g_2))) || (*l_6)), g_2))));
    }
    for (g_14 = 0; (g_14 <= 1); g_14 += 1)
    { 
        uint64_t l_39[4][2] = {{8U,8U},{8U,8U},{8U,8U},{8U,8U}};
        int i, j;
        for (g_2 = 1; (g_2 >= 0); g_2 -= 1)
        { 
            uint32_t l_22[3][4] = {{0x7CF2842C,0x177355A2,0x7CF2842C,0x08F497C3},{0x7CF2842C,0x08F497C3,0x08F497C3,0x7CF2842C},{0x03FDDA78,0x08F497C3,0xACC8E11F,0x08F497C3}};
            int32_t *l_23 = &g_24;
            int i, j;
            (*l_23) &= (g_14 || (g_7 || (((void*)0 == &g_2) && l_22[2][0])));
            for (g_24 = 1; (g_24 >= 0); g_24 -= 1)
            { 
                int32_t **l_29 = (void*)0;
                int32_t **l_34[2];
                uint16_t *l_36 = (void*)0;
                uint16_t *l_37 = &g_38;
                uint8_t l_41 = 0xEB;
                int i, j;
                for (i = 0; i < 2; i++)
                    l_34[i] = (void*)0;
                g_42 &= (((safe_div_func_int32_t_s_s(((((*l_37) = (((safe_add_func_uint64_t_u_u(((l_29 = (void*)0) != g_30), ((l_5[(g_14 + 1)][g_2] = ((g_7 = (safe_add_func_int64_t_s_s((l_34[1] == (void*)0), g_35))) & (*g_31))) ^ (**g_30)))) || g_35) || 0x2949029496125B66LL)) && l_39[2][1]) ^ l_39[2][1]), l_40)) > l_41) <= g_14);
            }
        }
        for (g_24 = 0; (g_24 <= 2); ++g_24)
        { 
            return g_14;
        }
    }
    return l_40;
}

static const int32_t  func_8(int32_t * p_9)
{ 
    int32_t *l_16[3][3] = {{(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0}};
    int32_t **l_21 = &l_16[0][0];
    int i, j;
    g_7 = 0xFE24307A;
    (*l_21) = func_10(((safe_mul_func_uint8_t_u_u(((safe_div_func_uint64_t_u_u((((((g_2 & (0x6FD8C65816309070LL | g_7)) , 0x46552DED) ^ g_14) >= g_2) != 6U), 0xF8DC70E74BB9CEC0LL)) ^ 4294967291U), g_14)) != g_2));
    return g_2;
}

static int32_t * func_10(uint8_t  p_11)
{ 
    int32_t *l_15 = (void*)0;
    return l_15;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_38, "g_38", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
