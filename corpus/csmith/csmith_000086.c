/* Csmith 0cdc710315cf seed 86, flattened by scripts/csmith_flatten.py */
// Options:   --seed 86 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint32_t  f0;
   uint64_t  f1;
   const uint64_t  f2;
   uint16_t  f3;
};

union U1 {
   struct S0  f0;
   int64_t  f1;
   const uint32_t  f2;
};

union U2 {
   const int8_t  f0;
};

static int32_t g_2[2][2] = {{0xD3F46721,0xD3F46721},{0xD3F46721,0xD3F46721}};
static union U2 g_7 = {0xD8};
static int32_t g_11 = 0x4E665969;

static union U1  func_1(void);

static union U1  func_1(void)
{ 
    uint64_t l_8 = 18446744073709551613U;
    union U1 l_13[1][1] = {{{{0xCC254292,0x78D8EF068CD54673LL,0x6A0B3493876A47CDLL,0xDFC3}}}};
    int i, j;
    for (g_2[0][1] = 0; (g_2[0][1] < 4); ++g_2[0][1])
    { 
        uint64_t l_9 = 0x3773672D714F98CALL;
        int32_t *l_10 = &g_11;
        int16_t l_12[4][3] = {{(-1),0x38E4,0xE76D},{(-6),0x38E4,(-6)},{1,(-1),0xE76D},{1,1,(-1)}};
        int i, j;
        (*l_10) &= (safe_mod_func_int8_t_s_s(((g_2[0][1] & (g_7 , (g_2[0][1] ^ l_8))) || l_9), g_7.f0));
        (*l_10) &= l_8;
        for (g_11 = 0; g_11 < 4; g_11 = g_11 + 1)
        {
            for (l_8 = 0; l_8 < 3; l_8 = l_8 + 1)
            {
                l_12[g_11][l_8] = (-3);
            }
        }
    }
    return l_13[0][0];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_2[i][j], "g_2[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_7.f0, "g_7.f0", print_hash_value);
    transparent_crc(g_11, "g_11", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
