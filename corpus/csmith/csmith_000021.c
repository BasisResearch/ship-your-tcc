/* Csmith 0cdc710315cf seed 21, flattened by scripts/csmith_flatten.py */
// Options:   --seed 21 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int16_t  f0;
   uint32_t  f1;
   int64_t  f2;
   int8_t  f3;
};

union U1 {
   int32_t  f0;
   const int8_t  f1;
};

static int32_t g_6 = 0x52D8CB4C;
static int32_t *g_5 = &g_6;
static struct S0 g_10 = {1,0xF5AF8B51,1,0};
static int64_t g_59 = 1;
static uint64_t g_61 = 18446744073709551612U;
static const uint64_t *g_60[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
static union U1 g_64 = {0xADC49480};
static int8_t g_76 = 0;
static uint32_t g_77 = 4294967295U;

static uint64_t  func_1(void);
static int32_t * func_2(int32_t * p_3, union U1  p_4);
static union U1  func_7(struct S0  p_8, int32_t  p_9);

static uint64_t  func_1(void)
{ 
    int32_t *l_13 = &g_6;
    int32_t *l_67 = &g_64.f0;
    int32_t *l_68 = &g_6;
    int32_t *l_69 = &g_64.f0;
    int32_t *l_70 = &g_6;
    int32_t *l_71 = &g_64.f0;
    int32_t *l_72 = &g_6;
    int32_t *l_73 = &g_6;
    int32_t l_74 = 0x959CD49D;
    int32_t *l_75 = &l_74;
    l_13 = func_2(g_5, func_7(g_10, ((safe_add_func_uint32_t_u_u((l_13 == (void*)0), 9)) || g_10.f2)));
    --g_77;
    return (*l_72);
}

static int32_t * func_2(int32_t * p_3, union U1  p_4)
{ 
    int32_t **l_65[4] = {&g_5,&g_5,&g_5,&g_5};
    int32_t ** const l_66 = (void*)0;
    int i;
    (*g_5) = ((l_65[0] = &p_3) == l_66);
    return &g_6;
}

static union U1  func_7(struct S0  p_8, int32_t  p_9)
{ 
    struct S0 *l_20 = &g_10;
    int32_t l_24 = 0x2324067B;
    uint32_t *l_25 = &g_10.f1;
    for (g_6 = 5; (g_6 > 9); g_6 = safe_add_func_int16_t_s_s(g_6, 2))
    { 
        for (p_8.f2 = 0; (p_8.f2 != 12); p_8.f2++)
        { 
            int32_t *l_19 = &g_6;
            int32_t **l_18 = &l_19;
            if ((0 == ((((*l_18) = &g_6) == (void*)0) < 0x23B16AAC)))
            { 
                p_9 = (*g_5);
            }
            else
            { 
                (*l_18) = &g_6;
            }
            (*l_18) = &p_9;
        }
    }
    (*l_20) = g_10;
    if ((((g_10.f2 , (((*l_25) ^= ((p_9 & ((safe_add_func_uint8_t_u_u((~0xFE51), p_8.f3)) ^ g_10.f2)) != l_24)) == p_8.f3)) , g_10.f3) | 0))
    { 
        for (g_6 = 0; (g_6 <= (-1)); --g_6)
        { 
            int32_t *l_35 = &g_6;
            int32_t **l_36 = &l_35;
            for (p_8.f0 = 0; (p_8.f0 != (-10)); p_8.f0--)
            { 
                int32_t *l_34[3][1];
                int i, j;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_34[i][j] = &l_24;
                }
                p_9 = (1 < ((safe_lshift_func_int8_t_s_s(((safe_sub_func_int32_t_s_s((*g_5), (g_10 , g_10.f2))) | 0x6E16511C), 5)) || p_8.f2));
            }
            (*l_36) = l_35;
        }
    }
    else
    { 
        uint64_t *l_39[3][4];
        int32_t l_40[2];
        int32_t l_49 = (-1);
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 4; j++)
                l_39[i][j] = (void*)0;
        }
        for (i = 0; i < 2; i++)
            l_40[i] = 1;
        (*g_5) ^= (safe_rshift_func_uint64_t_u_u((l_40[0] = 0U), 6));
        for (g_10.f2 = 0; (g_10.f2 <= 2); g_10.f2 += 1)
        { 
            uint8_t l_50 = 0x5C;
            int32_t l_52[3];
            const union U1 l_57[2][3] = {{{-1},{-1},{-1}},{{-1},{-1},{-1}}};
            int i, j;
            for (i = 0; i < 3; i++)
                l_52[i] = 4;
            for (p_8.f3 = 0; (p_8.f3 <= 2); p_8.f3 += 1)
            { 
                int32_t l_51 = 0x726B0657;
                int32_t **l_53 = (void*)0;
                int32_t **l_54 = (void*)0;
                int32_t *l_56 = &l_51;
                int32_t **l_55 = &l_56;
                int64_t *l_58 = &g_59;
                int i, j;
                l_52[1] ^= (safe_mul_func_int8_t_s_s((safe_add_func_uint32_t_u_u(((safe_mul_func_uint32_t_u_u(0x9F00F3BC, (((((safe_lshift_func_int16_t_s_u(((l_49 ^= l_40[0]) >= p_8.f0), 5)) != l_50) != 0x1C4E) < (*g_5)) || g_10.f3))) != l_51), l_51)), 0x4B));
                (*l_55) = ((*g_5) , &l_52[0]);
                (*l_56) = (((((((*l_58) = ((((0xE8C0996B || l_24) , (l_57[1][2] , g_10.f1)) && p_8.f2) , l_40[0])) <= g_10.f0) , g_60[3]) == l_39[1][2]) != g_10.f1) ^ 0xE3);
            }
            for (l_24 = 2; (l_24 >= 0); l_24 -= 1)
            { 
                int32_t *l_63 = (void*)0;
                int32_t **l_62 = &l_63;
                (*l_62) = &g_6;
            }
        }
    }
    return g_64;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_10.f0, "g_10.f0", print_hash_value);
    transparent_crc(g_10.f1, "g_10.f1", print_hash_value);
    transparent_crc(g_10.f2, "g_10.f2", print_hash_value);
    transparent_crc(g_10.f3, "g_10.f3", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    transparent_crc(g_64.f0, "g_64.f0", print_hash_value);
    transparent_crc(g_76, "g_76", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
