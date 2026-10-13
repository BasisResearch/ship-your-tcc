/* Csmith 0cdc710315cf seed 163, flattened by scripts/csmith_flatten.py */
// Options:   --seed 163 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
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
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint32_t
safe_rshift_func_uint32_t_u_s(uint32_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   uint16_t  f0;
   const int16_t  f1;
   uint16_t  f2;
};

struct S1 {
   uint8_t  f0;
   uint32_t  f1;
};

struct S2 {
   int16_t  f0;
   const int16_t  f1;
   uint8_t  f2;
   int32_t  f3;
};

union U3 {
   int8_t  f0;
   int16_t  f1;
};

static struct S0 g_13 = {1U,8,65535U};
static struct S1 g_15 = {0x9D,0U};
static int32_t g_18 = 0;
static int32_t *g_17 = &g_18;
static uint32_t g_29 = 7U;
static uint32_t g_42 = 0xCCA7A22F;
static int32_t g_51 = 0x99C819B5;
static uint8_t g_53 = 0x39;
static uint8_t g_57 = 0xC2;
static union U3 g_74 = {-7};
static union U3 *g_73 = &g_74;
static uint16_t g_84 = 0x2798;
static struct S2 g_90 = {-9,-10,255U,0x85BA07B4};
static struct S2 *g_89[4] = {&g_90,&g_90,&g_90,&g_90};

static uint64_t  func_1(void);
static struct S1  func_2(uint64_t  p_3, int16_t  p_4, int32_t  p_5, uint16_t  p_6);
static int64_t  func_7(int64_t  p_8, int32_t  p_9, struct S1  p_10, const struct S2  p_11);

static uint64_t  func_1(void)
{ 
    int8_t l_12 = 0xA4;
    uint32_t l_14 = 0x3F9BEAB1;
    const struct S2 l_16 = {-1,-7,0U,7};
    struct S1 *l_61 = (void*)0;
    struct S1 *l_62 = (void*)0;
    struct S1 *l_63 = &g_15;
    uint32_t **l_64 = (void*)0;
    int32_t l_67[1][4];
    int8_t l_77 = 0xE0;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_67[i][j] = (-4);
    }
    (*l_63) = func_2(((func_7((((l_12 , (g_13 , (l_14 , (-6)))) , g_13.f0) & 0x1CB27F08), g_13.f2, g_15, l_16) < g_57) != g_13.f0), l_16.f0, l_14, g_57);
    if (l_16.f2)
    { 
        l_64 = l_64;
    }
    else
    { 
        struct S0 *l_66 = &g_13;
        struct S0 **l_65 = &l_66;
        const struct S1 l_68 = {255U,0x1DDA348D};
        (*l_65) = &g_13;
        for (g_53 = 0; (g_53 <= 0); g_53 += 1)
        { 
            g_15 = l_68;
            if (g_42)
                break;
            for (g_18 = 0; (g_18 <= 0); g_18 += 1)
            { 
                int i, j;
                l_67[g_18][(g_53 + 2)] = (safe_rshift_func_uint32_t_u_s(((l_68 , (safe_rshift_func_int8_t_s_s((g_73 == (((*g_73) , 0x4E) , (void*)0)), l_67[g_18][(g_53 + 1)]))) == 1), l_67[g_53][(g_53 + 2)]));
            }
        }
        if (g_57)
        { 
            int32_t **l_75 = (void*)0;
            int32_t **l_76 = &g_17;
            int32_t *l_78 = (void*)0;
            int32_t *l_79 = &l_67[0][3];
            int32_t *l_80 = &l_67[0][0];
            int32_t *l_81 = &g_51;
            int32_t *l_82 = &l_67[0][2];
            int32_t *l_83[3][1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_83[i][j] = &g_18;
            }
            (*l_76) = (void*)0;
            --g_84;
        }
        else
        { 
            int32_t *l_97 = &g_51;
            for (g_13.f0 = 0; (g_13.f0 <= 15); ++g_13.f0)
            { 
                struct S2 **l_91 = &g_89[1];
                struct S2 *l_93 = &g_90;
                struct S2 **l_92 = &l_93;
                int32_t l_96[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_96[i] = 0x02481C6D;
                (*l_92) = ((*l_91) = g_89[1]);
                (*l_63) = ((((*g_73) , l_68.f0) | (safe_mul_func_int8_t_s_s(((g_90.f1 && 1) , g_84), g_51))) , l_68);
                if (l_96[1])
                    continue;
            }
            (*l_97) = (l_16.f0 == g_15.f0);
        }
    }
    return g_90.f2;
}

static struct S1  func_2(uint64_t  p_3, int16_t  p_4, int32_t  p_5, uint16_t  p_6)
{ 
    union U3 l_58 = {0xAA};
    uint32_t **l_59 = (void*)0;
    struct S1 l_60 = {6U,1U};
    l_59 = ((l_58 , g_15.f0) , (void*)0);
    return l_60;
}

static int64_t  func_7(int64_t  p_8, int32_t  p_9, struct S1  p_10, const struct S2  p_11)
{ 
    int16_t l_46 = 0xEA65;
    int32_t l_48 = (-8);
    int32_t l_49 = 1;
    int32_t l_52 = 0x6C15A2D1;
    g_17 = &p_9;
    for (g_15.f1 = (-20); (g_15.f1 < 1); g_15.f1 = safe_add_func_uint32_t_u_u(g_15.f1, 8))
    { 
        union U3 l_21[1][1] = {{{0xF3}}};
        struct S0 *l_43[4][1];
        int32_t l_47 = (-5);
        int32_t l_50 = 0x730A04A8;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_43[i][j] = &g_13;
        }
        for (p_10.f0 = 0; (p_10.f0 <= 0); p_10.f0 += 1)
        { 
            uint32_t *l_26 = (void*)0;
            uint32_t *l_27 = (void*)0;
            uint32_t *l_28 = &g_29;
            int32_t l_32[1];
            union U3 *l_38 = &l_21[p_10.f0][p_10.f0];
            uint32_t *l_41 = &g_42;
            int32_t **l_56 = &g_17;
            int i, j;
            for (i = 0; i < 1; i++)
                l_32[i] = (-5);
            if ((safe_rshift_func_int16_t_s_u((safe_rshift_func_uint32_t_u_s(((++(*l_28)) >= ((l_32[0] , ((safe_lshift_func_int64_t_s_u((((~((*l_41) = (safe_mod_func_int8_t_s_s((((*l_38) = l_21[p_10.f0][p_10.f0]) , (safe_div_func_uint8_t_u_u(0U, g_13.f2))), 0x51)))) > g_13.f1) != 4294967286U), 51)) || p_10.f1)) && g_42)), 7)), 3)))
            { 
                struct S0 **l_44 = &l_43[0][0];
                (*l_44) = l_43[1][0];
                (*l_44) = &g_13;
            }
            else
            { 
                int32_t *l_45[2][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
                int i, j;
                --g_53;
            }
            (*l_56) = &g_18;
        }
        (*g_17) ^= 0x949F72E7;
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
    transparent_crc(g_13.f0, "g_13.f0", print_hash_value);
    transparent_crc(g_13.f1, "g_13.f1", print_hash_value);
    transparent_crc(g_13.f2, "g_13.f2", print_hash_value);
    transparent_crc(g_15.f0, "g_15.f0", print_hash_value);
    transparent_crc(g_15.f1, "g_15.f1", print_hash_value);
    transparent_crc(g_18, "g_18", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_74.f0, "g_74.f0", print_hash_value);
    transparent_crc(g_84, "g_84", print_hash_value);
    transparent_crc(g_90.f0, "g_90.f0", print_hash_value);
    transparent_crc(g_90.f1, "g_90.f1", print_hash_value);
    transparent_crc(g_90.f2, "g_90.f2", print_hash_value);
    transparent_crc(g_90.f3, "g_90.f3", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
