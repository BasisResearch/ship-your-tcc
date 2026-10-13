/* Csmith 0cdc710315cf seed 220, flattened by scripts/csmith_flatten.py */
// Options:   --seed 220 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint32_t  f0;
   uint64_t  f1;
   uint16_t  f2;
};

union U1 {
   const uint16_t  f0;
   int16_t  f1;
};

union U2 {
   uint16_t  f0;
};

union U3 {
   const int16_t  f0;
};

static int32_t g_3 = 1;
static uint64_t g_19 = 0xF23E52509E71B35DLL;
static int32_t g_32 = 9;
static int32_t * const g_31 = &g_32;
static int32_t * const *g_30 = &g_31;
static union U1 g_36 = {0x64E7};
static uint32_t g_48 = 4294967287U;
static int32_t *g_54 = &g_3;
static int16_t g_77 = (-4);
static int64_t g_78 = 0x4EC100FABB65F0DELL;
static int32_t g_87 = 0xBE7CA300;
static union U3 g_88[2][2] = {{{1},{1}},{{1},{1}}};

static union U3  func_1(void);
static union U2  func_6(int32_t * p_7, int32_t * p_8);
static int32_t * func_9(int32_t * p_10, int32_t * p_11, int32_t * const  p_12, const int32_t  p_13);

static union U3  func_1(void)
{ 
    int32_t *l_2 = &g_3;
    int32_t * const l_14 = &g_3;
    int32_t *l_86 = &g_87;
    (*l_2) = 0x768C0AA3;
    (*l_86) &= (safe_div_func_int64_t_s_s((-2), (func_6(func_9(l_2, l_2, l_14, (*l_14)), &g_3) , 0xEE889C39DB43F150LL)));
    (*l_2) &= (*l_86);
    return g_88[1][0];
}

static union U2  func_6(int32_t * p_7, int32_t * p_8)
{ 
    struct S0 l_24 = {8U,0U,7U};
    int32_t * const l_29[1][1] = {{&g_3}};
    int32_t * const *l_28[2][1];
    union U2 l_45 = {6U};
    int16_t l_73 = (-3);
    uint8_t l_79 = 0xF8;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_28[i][j] = &l_29[0][0];
    }
    for (g_19 = 10; (g_19 <= 44); ++g_19)
    { 
        struct S0 *l_25 = &l_24;
        int32_t l_33 = 0x8B0D58E7;
        int32_t l_76 = 0;
        (*l_25) = l_24;
        if ((safe_rshift_func_uint16_t_u_u((((((((l_28[0][0] = (g_30 = l_28[0][0])) != (void*)0) == (g_32 && g_3)) <= l_33) , g_32) == g_19) >= l_33), l_33)))
        { 
            union U1 *l_35[2][1];
            union U1 **l_34 = &l_35[1][0];
            int32_t l_44 = 9;
            struct S0 l_46 = {0xA1BBDB81,9U,0U};
            int32_t l_47 = (-5);
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_35[i][j] = &g_36;
            }
            (*l_34) = (void*)0;
            (*p_8) = (((l_33 , (safe_mod_func_uint32_t_u_u((!(((g_19 , (((safe_mul_func_int8_t_s_s((safe_lshift_func_uint64_t_u_s((((*p_8) || l_33) != (*p_7)), 4)), g_36.f0)) != 0xE2646899DB2213AALL) , (void*)0)) != &g_36) && l_33)), l_44))) , l_33) | l_33);
            if (((void*)0 == &l_44))
            { 
                return l_45;
            }
            else
            { 
                (*l_25) = l_46;
                g_48++;
            }
        }
        else
        { 
            uint16_t l_62 = 65528U;
            int16_t l_70 = 3;
            int8_t l_74 = (-10);
            if (l_33)
            { 
                struct S0 **l_51 = &l_25;
                int32_t *l_53 = (void*)0;
                int32_t **l_52 = &l_53;
                (*l_51) = &l_24;
                (*l_52) = (*g_30);
                g_54 = p_8;
            }
            else
            { 
                int16_t l_63 = 0x915C;
                l_63 |= (g_36 , ((~(safe_div_func_uint32_t_u_u(((safe_div_func_uint16_t_u_u((0xE7A53CC61BBCB567LL != (((safe_mul_func_uint32_t_u_u(l_62, l_33)) != g_3) | 8U)), g_3)) >= l_62), 8U))) , 0x52A103AB));
                (*p_7) ^= l_62;
                (*p_7) = ((*g_31) |= (safe_rshift_func_uint8_t_u_s(0xC7, 2)));
            }
            for (l_62 = (-20); (l_62 <= 59); l_62++)
            { 
                int16_t l_68 = 0x19EE;
                int32_t l_69 = 0x6FCCBB28;
                int32_t l_71 = 0x7C870D18;
                int32_t l_72 = (-3);
                int32_t l_75 = (-1);
                l_79++;
                (*g_54) &= ((safe_add_func_uint64_t_u_u((l_68 > ((((9 | 0xFC81) < (-8)) != 0x59D8E95A) > l_70)), 0)) < 0x90F62EB7B5DC4542LL);
            }
        }
        return l_45;
    }
    return l_45;
}

static int32_t * func_9(int32_t * p_10, int32_t * p_11, int32_t * const  p_12, const int32_t  p_13)
{ 
    const int32_t *l_16 = &g_3;
    const int32_t **l_15 = &l_16;
    int32_t l_17 = 0x7F1A969A;
    int32_t *l_18[2];
    int i;
    for (i = 0; i < 2; i++)
        l_18[i] = &l_17;
    (*l_15) = &p_13;
    g_19--;
    return p_11;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_36.f0, "g_36.f0", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_77, "g_77", print_hash_value);
    transparent_crc(g_78, "g_78", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_88[i][j].f0, "g_88[i][j].f0", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
