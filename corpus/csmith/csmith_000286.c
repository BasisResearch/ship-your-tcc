/* Csmith 0cdc710315cf seed 286, flattened by scripts/csmith_flatten.py */
// Options:   --seed 286 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
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
static uint32_t
safe_lshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (4294967295U >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

static const uint64_t g_6 = 0x3009CE4562513244LL;
static uint16_t g_10 = 0x182B;
static int8_t g_20 = 0;
static uint16_t *g_22 = (void*)0;
static uint16_t **g_21 = &g_22;
static int32_t g_25 = 1;
static int32_t *g_24[3] = {&g_25,&g_25,&g_25};
static uint8_t g_45[1][2] = {{0xBE,0xBE}};
static int64_t g_48 = 0x80A2560131B3B333LL;
static uint64_t g_51 = 18446744073709551610U;
static int16_t g_60 = 0x350B;
static uint64_t g_95 = 18446744073709551615U;
static uint64_t g_97 = 0xBC135235ED9E4BB1LL;
static uint32_t g_110[3][3] = {{0U,0xEC30626F,0xEC30626F},{0U,0xEC30626F,0xEC30626F},{0U,0xEC30626F,0xEC30626F}};

static uint8_t  func_1(void);
static const int32_t  func_2(uint32_t  p_3, uint8_t  p_4);
static int32_t * func_7(uint64_t  p_8);

static uint8_t  func_1(void)
{ 
    int32_t l_5[4][4] = {{4,0x9D568D52,0x9D568D52,4},{0x9D568D52,4,(-1),0},{0x9D568D52,(-1),0x9D568D52,0x83111738},{4,0,0x83111738,0x83111738}};
    int32_t *l_76 = &g_25;
    int32_t l_129 = 0x89DC94FE;
    int i, j;
lbl_124:
    (*l_76) = func_2(l_5[0][0], (18446744073709551615U > g_6));
    for (g_51 = 0; (g_51 >= 39); g_51 = safe_add_func_int8_t_s_s(g_51, 3))
    { 
        int8_t l_79 = (-1);
        int32_t **l_80 = &l_76;
        int32_t l_102 = 0xFB3452CA;
        int32_t l_109[1][1];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 1; j++)
                l_109[i][j] = 0xB361CED5;
        }
        (*l_80) = (l_79 , &l_5[2][0]);
        if ((*l_76))
        { 
            int64_t l_98 = 0x4809F77A2FDF8326LL;
            int32_t *l_103 = &l_5[0][0];
            int32_t *l_104 = &g_25;
            int32_t *l_105 = (void*)0;
            int32_t l_106 = 6;
            int32_t *l_107 = &l_106;
            int32_t *l_108[1];
            int8_t l_123 = 0;
            int i;
            for (i = 0; i < 1; i++)
                l_108[i] = (void*)0;
            for (g_48 = (-17); (g_48 == 25); ++g_48)
            { 
                uint64_t *l_94 = &g_95;
                uint64_t *l_96 = &g_97;
                int32_t *l_99 = (void*)0;
                int32_t *l_100 = (void*)0;
                int32_t *l_101[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_101[i] = &g_25;
                l_102 = ((safe_mod_func_int16_t_s_s((safe_lshift_func_uint8_t_u_s(((((safe_sub_func_int64_t_s_s((safe_div_func_uint8_t_u_u((((((safe_mod_func_int8_t_s_s((~(((*l_76) = ((*l_94) = 0U)) <= ((*l_96) = g_60))), (g_20 = g_10))) || g_95) < l_98) , 0x66BB) > l_79), g_10)), 0x20E0461AF4752A80LL)) , (*l_80)) != (*l_80)) > g_45[0][0]), 6)), g_25)) & g_48);
            }
            g_110[1][0]--;
            (**l_80) = ((safe_mod_func_int64_t_s_s((safe_mul_func_int8_t_s_s((g_20 = (safe_lshift_func_int64_t_s_u(((*l_76) , g_97), ((safe_rshift_func_int32_t_s_u((safe_add_func_uint16_t_u_u(5U, 0x4A9F)), (*l_76))) <= g_45[0][1])))), g_51)), 0x98C00C08B1440151LL)) < l_123);
        }
        else
        { 
            int8_t l_127 = (-3);
            if (g_51)
                goto lbl_124;
            for (g_10 = 0; (g_10 <= 2); g_10 += 1)
            { 
                int32_t *l_128[4];
                int i;
                for (i = 0; i < 4; i++)
                    l_128[i] = &l_109[0][0];
                l_129 &= ((safe_lshift_func_uint32_t_u_s((1 ^ g_20), 31)) & (l_127 | (*l_76)));
                if ((*l_76))
                    continue;
            }
            return g_20;
        }
    }
    g_24[1] = &g_25;
    return g_95;
}

static const int32_t  func_2(uint32_t  p_3, uint8_t  p_4)
{ 
    uint16_t *l_9 = &g_10;
    int32_t l_18 = 7;
    int32_t *l_19[1][3];
    int32_t **l_26 = &l_19[0][1];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_19[i][j] = &l_18;
    }
    (*l_26) = func_7((((*l_9) |= g_6) , (+(g_20 = (safe_sub_func_int64_t_s_s((safe_lshift_func_uint64_t_u_s(((safe_rshift_func_int32_t_s_s(l_18, 29)) && p_3), 31)), g_6))))));
    for (g_25 = 0; (g_25 >= 0); g_25 -= 1)
    { 
        int32_t l_29 = 9;
        int32_t l_36 = (-1);
        int32_t **l_61 = &l_19[0][1];
        uint16_t *l_74 = (void*)0;
        int i;
        l_29 = (safe_rshift_func_uint32_t_u_u(1U, 3));
        l_36 |= (safe_rshift_func_uint16_t_u_s((safe_mul_func_int64_t_s_s(l_29, ((safe_lshift_func_int32_t_s_u((g_10 >= p_3), (**l_26))) , p_4))), 7));
        (*l_26) = g_24[(g_25 + 1)];
    }
    return p_3;
}

static int32_t * func_7(uint64_t  p_8)
{ 
    uint16_t **l_23 = &g_22;
    l_23 = g_21;
    return g_24[1];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_45[i][j], "g_45[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_95, "g_95", print_hash_value);
    transparent_crc(g_97, "g_97", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_110[i][j], "g_110[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
