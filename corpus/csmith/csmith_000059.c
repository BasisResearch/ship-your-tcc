/* Csmith 0cdc710315cf seed 59, flattened by scripts/csmith_flatten.py */
// Options:   --seed 59 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int16_t g_2[4] = {0x4A9B,0x4A9B,0x4A9B,0x4A9B};
static int32_t g_18 = 0x3A3DBC56;
static int16_t g_39 = 0xB739;
static int32_t g_42 = 0;
static int32_t g_44 = (-1);
static uint16_t g_46 = 0xFE53;
static int32_t g_48 = 0;
static int8_t g_72 = 1;
static int32_t *g_77 = (void*)0;
static int32_t g_82 = 4;
static int8_t g_85 = (-1);
static int16_t g_93 = (-1);
static uint8_t g_94 = 0x03;
static const uint16_t g_121 = 0x3F33;
static const uint16_t g_123 = 65535U;
static const uint16_t *g_122 = &g_123;

static const int64_t  func_1(void);
static const int8_t  func_9(int64_t  p_10, int32_t  p_11, uint32_t  p_12, int64_t  p_13);
static int16_t  func_21(int32_t * p_22, uint16_t  p_23);

static const int64_t  func_1(void)
{ 
    uint32_t l_14 = 0x92F644AC;
    uint16_t *l_45 = &g_46;
    int32_t *l_47 = &g_48;
    int32_t l_91 = 0x5CE9A204;
    int16_t l_126 = (-6);
    int32_t **l_128 = &l_47;
    const int32_t *l_130 = (void*)0;
    const int32_t **l_129 = &l_130;
    int32_t *l_131 = (void*)0;
    int32_t *l_132 = &g_18;
    return g_2[1];
}

static const int8_t  func_9(int64_t  p_10, int32_t  p_11, uint32_t  p_12, int64_t  p_13)
{ 
    int32_t *l_24 = &g_18;
    int32_t *l_43 = &g_44;
    for (p_11 = 0; (p_11 == 27); p_11 = safe_add_func_int16_t_s_s(p_11, 1))
    { 
        int32_t *l_17 = &g_18;
        (*l_17) ^= 0xA0A5DEC4;
        for (g_18 = 0; (g_18 < (-28)); g_18 = safe_sub_func_int8_t_s_s(g_18, 6))
        { 
            return (*l_17);
        }
    }
    (*l_43) &= (0x2D7B == ((func_21(l_24, ((1 ^ g_2[3]) != g_18)) > 0xDBE4) <= 4294967295U));
    return (*l_43);
}

static int16_t  func_21(int32_t * p_22, uint16_t  p_23)
{ 
    uint32_t l_28 = 0xC1031C36;
    for (g_18 = (-14); (g_18 <= 3); ++g_18)
    { 
        uint16_t l_27 = 65535U;
        if (l_27)
            break;
        if (l_27)
            continue;
    }
    l_28--;
    for (p_23 = 4; (p_23 == 55); p_23++)
    { 
        uint8_t l_37 = 1U;
        if ((*p_22))
            break;
        for (g_18 = 0; (g_18 == (-11)); g_18--)
        { 
            if (g_2[1])
                break;
            for (l_28 = 0; (l_28 <= 3); l_28 += 1)
            { 
                int16_t *l_38 = &g_39;
                int32_t *l_40 = (void*)0;
                int32_t *l_41 = &g_42;
                int i;
                (*l_41) = (((safe_unary_minus_func_uint64_t_u((safe_unary_minus_func_uint16_t_u(g_2[l_28])))) & (((g_2[3] != ((*l_38) = l_37)) > g_2[l_28]) >= 2)) == 0);
                if (l_28)
                    continue;
            }
        }
    }
    return g_42;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_2[i], "g_2[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_72, "g_72", print_hash_value);
    transparent_crc(g_82, "g_82", print_hash_value);
    transparent_crc(g_85, "g_85", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    transparent_crc(g_94, "g_94", print_hash_value);
    transparent_crc(g_121, "g_121", print_hash_value);
    transparent_crc(g_123, "g_123", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
