/* Csmith 0cdc710315cf seed 326, flattened by scripts/csmith_flatten.py */
// Options:   --seed 326 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
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
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   int32_t  f0;
   const int64_t  f1;
   uint8_t  f2;
   int32_t  f3;
};

struct S1 {
   int32_t  f0;
};

static int32_t g_5[3][1] = {{0x7CEFA9D1},{0x7CEFA9D1},{0x7CEFA9D1}};
static struct S0 g_11 = {0x8AE505B0,0xB8BFFBEFA5F9C710LL,0x27,0xBA75A592};
static int8_t g_20 = 0x46;
static uint64_t g_30 = 0x32C2A4E630971CF7LL;
static struct S1 g_33[3] = {{-10},{-10},{-10}};
static int32_t g_35 = 1;
static int32_t *g_34 = &g_35;
static uint16_t g_58 = 3U;

static uint32_t  func_1(void);
static int32_t  func_2(const uint32_t  p_3);
static int32_t * func_8(struct S0  p_9, int32_t  p_10);

static uint32_t  func_1(void)
{ 
    const int64_t l_4 = 1;
    int32_t l_32 = 0x904E20EE;
    uint64_t *l_40 = &g_30;
    int32_t l_42[1][2];
    uint64_t l_68[2][1];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_42[i][j] = 0xFB996340;
    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_68[i][j] = 1U;
    }
    l_32 = func_2(l_4);
    g_34 = (g_33[1] , func_8(g_11, l_32));
    if (((*g_34) && (((safe_mod_func_uint64_t_u_u(0x2D89188B9ABC19ACLL, ((*l_40) = (safe_sub_func_uint64_t_u_u(9U, l_4))))) | l_32) , 0x63D2C82F)))
    { 
        int32_t *l_41[2][4] = {{&g_11.f3,&g_11.f3,&g_11.f3,&g_11.f3},{&g_11.f3,&g_11.f3,&g_11.f3,&g_11.f3}};
        int64_t l_43[2][4] = {{0x361030FACFA70DAELL,0xE4C0C5DC74738709LL,0xE4C0C5DC74738709LL,0x361030FACFA70DAELL},{0xE4C0C5DC74738709LL,0x361030FACFA70DAELL,0xE4C0C5DC74738709LL,0xE4C0C5DC74738709LL}};
        uint32_t l_44 = 0x1586CE7E;
        int32_t **l_47 = &l_41[0][0];
        int i, j;
        l_44++;
        (*g_34) = 0x1292777E;
        (*l_47) = &g_5[0][0];
    }
    else
    { 
        uint16_t l_56 = 0x1208;
        int16_t l_59 = 0x0893;
        int64_t l_66[3][2];
        int i, j;
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 2; j++)
                l_66[i][j] = 0;
        }
        for (g_11.f3 = 0; (g_11.f3 >= (-10)); --g_11.f3)
        { 
            uint16_t *l_57 = &g_58;
            const struct S0 l_63 = {0xDE950740,2,0xEB,0x95BC5B36};
            int32_t l_67[4][4] = {{(-2),(-2),0,0},{(-2),(-2),0,0},{(-2),(-2),0,0},{(-2),(-2),0,0}};
            int i, j;
            for (g_35 = 0; (g_35 != (-11)); --g_35)
            { 
                (*g_34) = (*g_34);
            }
            if (((*g_34) = (safe_sub_func_int64_t_s_s((safe_sub_func_uint16_t_u_u((l_59 ^= ((*l_57) &= l_56)), ((+(safe_rshift_func_uint32_t_u_u(l_56, ((((l_63 , g_5[2][0]) || l_56) == 0) || 0x76A8768DD0F05760LL)))) || g_11.f0))), g_11.f3))))
            { 
                uint16_t l_64 = 65532U;
                return l_64;
            }
            else
            { 
                int32_t *l_65[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_65[i] = &l_32;
                ++l_68[1][0];
                if (l_4)
                    break;
            }
        }
    }
    return l_42[0][1];
}

static int32_t  func_2(const uint32_t  p_3)
{ 
    struct S0 l_26 = {-1,-8,0x57,1};
    uint64_t *l_29 = &g_30;
    int32_t *l_31[3][2];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_31[i][j] = &g_5[1][0];
    }
    for (g_5[2][0] = 15; (g_5[2][0] < (-28)); --g_5[2][0])
    { 
        int64_t l_12 = 0x06C0E2632C5B641BLL;
        int32_t *l_25[2];
        int32_t **l_24 = &l_25[1];
        int i;
        for (i = 0; i < 2; i++)
            l_25[i] = &g_11.f0;
        (*l_24) = func_8(g_11, l_12);
        return g_11.f1;
    }
    l_31[0][0] = func_8(l_26, (safe_rshift_func_uint64_t_u_u(((*l_29) |= g_5[2][0]), 39)));
    return p_3;
}

static int32_t * func_8(struct S0  p_9, int32_t  p_10)
{ 
    const uint64_t l_15 = 0x46E74A6E8F59172DLL;
    int32_t *l_18 = &g_11.f3;
    int8_t *l_19 = &g_20;
    uint16_t l_21 = 1U;
    g_11.f0 &= (p_9.f1 == ((((safe_mod_func_int8_t_s_s(l_15, (l_21 = ((*l_19) = (safe_mul_func_int64_t_s_s(((void*)0 != l_18), (*l_18))))))) == g_11.f1) ^ (*l_18)) & (*l_18)));
    (*l_18) = (safe_lshift_func_int16_t_s_u(0x9482, 4));
    return &g_5[2][0];
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
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_5[i][j], "g_5[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_11.f0, "g_11.f0", print_hash_value);
    transparent_crc(g_11.f1, "g_11.f1", print_hash_value);
    transparent_crc(g_11.f2, "g_11.f2", print_hash_value);
    transparent_crc(g_11.f3, "g_11.f3", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_33[i].f0, "g_33[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
