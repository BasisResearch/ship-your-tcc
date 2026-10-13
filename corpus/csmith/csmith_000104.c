/* Csmith 0cdc710315cf seed 104, flattened by scripts/csmith_flatten.py */
// Options:   --seed 104 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int16_t  f0;
};

static int64_t g_26 = 0x33C7D24B5E495761LL;
static int32_t g_28 = (-1);
static int32_t *g_27 = &g_28;
static int32_t g_35 = 0x0335900B;
static uint32_t g_37 = 0xB38E5161;
static struct S0 g_39 = {-4};
static int32_t g_52 = 0x2039CE85;
static uint8_t g_53 = 255U;

static uint64_t  func_1(void);
static int32_t  func_4(uint32_t  p_5, int64_t  p_6, struct S0  p_7, struct S0  p_8, int16_t  p_9);
static uint64_t  func_20(int8_t  p_21, int16_t  p_22, const int8_t  p_23, uint64_t  p_24);

static uint64_t  func_1(void)
{ 
    int64_t l_25[1];
    uint32_t *l_36[4][4] = {{&g_37,&g_37,&g_37,&g_37},{&g_37,&g_37,&g_37,&g_37},{&g_37,&g_37,&g_37,&g_37},{&g_37,&g_37,&g_37,&g_37}};
    struct S0 l_38[3] = {{0},{0},{0}};
    uint16_t l_50 = 0xD051;
    int32_t *l_51[3][3] = {{(void*)0,(void*)0,(void*)0},{&g_52,&g_52,&g_52},{(void*)0,(void*)0,(void*)0}};
    int i, j;
    for (i = 0; i < 1; i++)
        l_25[i] = (-5);
    if ((g_53 = (((safe_mod_func_int32_t_s_s((g_52 ^= ((0x801FC3EE <= func_4((g_37 |= (safe_sub_func_uint32_t_u_u((safe_mod_func_int8_t_s_s((safe_add_func_int16_t_s_s((safe_lshift_func_uint16_t_u_u((safe_rshift_func_uint8_t_u_u((((func_20(((l_25[0] >= (l_25[0] ^ g_26)) && l_25[0]), l_25[0], l_25[0], g_26) , g_26) && l_25[0]) <= g_26), l_25[0])), l_25[0])), l_25[0])), l_25[0])), l_25[0]))), l_25[0], l_38[2], g_39, g_39.f0)) != l_50)), 4294967289U)) , (*g_27)) | 1)))
    { 
        struct S0 *l_54 = &g_39;
        (*l_54) = l_38[0];
        return g_52;
    }
    else
    { 
        struct S0 l_55[1] = {{0x0CF6}};
        struct S0 *l_56 = &g_39;
        int i;
        (*l_56) = l_55[0];
        return g_28;
    }
}

static int32_t  func_4(uint32_t  p_5, int64_t  p_6, struct S0  p_7, struct S0  p_8, int16_t  p_9)
{ 
    int32_t **l_47 = &g_27;
    int32_t **l_48 = (void*)0;
    int32_t **l_49 = &g_27;
    for (p_5 = 0; (p_5 < 42); p_5 = safe_add_func_uint64_t_u_u(p_5, 6))
    { 
        uint16_t l_42[2][3] = {{0x9381,0x9381,0x9381},{0x3D73,0x3D73,0x3D73}};
        int32_t *l_45 = (void*)0;
        int32_t **l_46 = &l_45;
        int i, j;
        for (g_28 = 0; (g_28 <= 1); g_28 += 1)
        { 
            for (p_8.f0 = 0; (p_8.f0 <= 1); p_8.f0 += 1)
            { 
                int32_t *l_43[3];
                int32_t **l_44 = &l_43[2];
                int i, j;
                for (i = 0; i < 3; i++)
                    l_43[i] = &g_28;
                if (l_42[p_8.f0][(p_8.f0 + 1)])
                    break;
                (*l_44) = l_43[2];
            }
        }
        (*l_46) = l_45;
    }
    l_47 = (void*)0;
    (*l_49) = &g_35;
    return p_9;
}

static uint64_t  func_20(int8_t  p_21, int16_t  p_22, const int8_t  p_23, uint64_t  p_24)
{ 
    int32_t **l_29 = &g_27;
    (*l_29) = g_27;
    for (g_28 = 10; (g_28 <= (-25)); g_28--)
    { 
        int32_t *l_34 = &g_35;
        (*l_34) = (safe_lshift_func_int16_t_s_u((-2), 3));
    }
    return g_26;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_39.f0, "g_39.f0", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
