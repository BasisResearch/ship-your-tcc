/* Csmith 0cdc710315cf seed 179, flattened by scripts/csmith_flatten.py */
// Options:   --seed 179 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
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
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static int8_t
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const int32_t  f0;
   int8_t  f1;
   const int64_t  f2;
};

struct S1 {
   int64_t  f0;
   int32_t  f1;
   int8_t  f2;
   uint16_t  f3;
};

struct S2 {
   uint16_t  f0;
   struct S1  f1;
   const uint8_t  f2;
};

struct S4 {
   const int16_t  f0;
   int32_t  f1;
   uint32_t  f2;
   struct S1  f3;
};

union U5 {
   struct S0  f0;
   uint8_t  f1;
   const struct S2  f2;
};

static int32_t g_2[2][4] = {{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)}};
static int32_t g_6 = (-6);
static struct S0 g_17 = {0x74C3EEC7,0xF5,0xFEB4E52275DD3610LL};
static int32_t g_19 = 0;
static int64_t g_20[3] = {0x909CC0F90B8A442FLL,0x909CC0F90B8A442FLL,0x909CC0F90B8A442FLL};
static int8_t g_21 = (-4);
static int64_t g_22[1][3] = {{0x11B1996FCD4D3380LL,0x11B1996FCD4D3380LL,0x11B1996FCD4D3380LL}};
static int16_t g_23[1][3] = {{0xE01A,0xE01A,0xE01A}};
static int64_t g_24 = 1;
static uint8_t g_25 = 0xEF;
static const struct S2 g_30 = {0xA8B5,{0x6E56995E148CB42BLL,0x8F18E0DA,0x3D,0xEDBC},0x77};
static int32_t g_45 = 6;
static uint32_t g_55 = 2U;
static uint32_t g_59 = 0x1BC6E4F4;
static uint32_t g_62 = 4294967295U;
static struct S4 g_63 = {0x3F4A,9,4294967287U,{0x459313D93154CD5CLL,0x7D7ACB2E,0,1U}};
static int32_t *g_79 = &g_45;
static struct S0 g_83 = {0x4738C559,0xA3,0xB787705A97E96616LL};
static uint8_t g_88 = 255U;
static union U5 g_114 = {{0,-6,-1}};

static struct S2  func_1(void);
static struct S0  func_9(struct S2  p_10, int16_t  p_11);
static struct S2  func_12(uint64_t  p_13, int16_t  p_14);

static struct S2  func_1(void)
{ 
    uint8_t *l_97 = &g_25;
    const int32_t l_98 = 0x9039BF07;
    struct S0 *l_112 = &g_83;
    uint32_t *l_115 = &g_55;
    struct S2 l_118 = {0x9A07,{0,0x52BFD302,1,0x8773},0x05};
    for (g_2[1][3] = 0; (g_2[1][3] == (-27)); --g_2[1][3])
    { 
        uint16_t l_5[4][3] = {{8U,0x6CE0,8U},{0x1D82,0x1D82,0x1D82},{8U,0x6CE0,8U},{0x1D82,0x1D82,0x1D82}};
        int32_t l_99 = (-9);
        int i, j;
    }
    (*g_79) = l_98;
    (*g_79) = ((g_114 , ((*l_115)--)) == ((&l_98 != &g_2[0][3]) || g_63.f3.f2));
    return l_118;
}

static struct S0  func_9(struct S2  p_10, int16_t  p_11)
{ 
    int32_t *l_35 = (void*)0;
    uint64_t l_42 = 0U;
    union U5 l_48 = {{-3,1,0xB0ECC8E5C8A68674LL}};
    uint8_t *l_64 = &g_25;
    int32_t l_85 = 0x11538544;
    int32_t l_86[1][2];
    int32_t l_87 = 6;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_86[i][j] = 0;
    }
    if (((((&g_6 == l_35) != (0 <= 4294967295U)) , p_10.f1.f3) != g_6))
    { 
        int32_t *l_38 = &g_19;
        const struct S0 l_39 = {0x086ADB69,8,0x6059BF80B7F2DBF9LL};
        uint8_t *l_43 = (void*)0;
        uint8_t *l_44[2][3] = {{(void*)0,(void*)0,(void*)0},{&g_25,&g_25,&g_25}};
        uint32_t *l_54 = &g_55;
        uint32_t *l_58 = &g_59;
        uint32_t *l_60 = (void*)0;
        uint32_t *l_61 = &g_62;
        int i, j;
        g_19 ^= (safe_rshift_func_uint8_t_u_u(0xAC, ((l_38 = l_35) != (void*)0)));
        if ((l_39 , (255U == (g_45 = ((safe_mod_func_uint16_t_u_u(((func_12(p_10.f1.f1, p_10.f0) , (-1)) , l_42), p_11)) != p_10.f0)))))
        { 
            int32_t *l_49 = &g_19;
            int32_t **l_50 = &l_38;
            (*l_49) |= (((void*)0 == l_44[1][0]) ^ ((((*l_50) = (l_48 , l_49)) == l_35) >= p_10.f1.f2));
            (*l_38) ^= g_20[2];
        }
        else
        { 
            uint8_t l_53[2];
            int i;
            for (i = 0; i < 2; i++)
                l_53[i] = 0x96;
            g_19 |= (safe_mod_func_uint16_t_u_u(g_30.f1.f3, ((p_10 , ((0x58 != l_53[0]) < p_10.f1.f3)) || p_10.f1.f3)));
            p_10.f1.f1 = l_53[1];
        }
        if ((((*l_61) = ((*l_58) = ((*l_54)--))) <= (l_39.f2 <= ((g_63 , (l_64 != l_43)) & l_39.f0))))
        { 
            uint64_t l_67 = 0x7919BB5C08239865LL;
            int32_t *l_68 = &g_63.f1;
            (*l_68) = ((safe_add_func_uint64_t_u_u((((l_39 , (((0x26 >= (p_10.f2 != p_10.f1.f1)) <= l_67) ^ g_17.f1)) <= l_67) , g_63.f3.f2), g_55)) > g_2[1][3]);
            (*l_68) &= (safe_add_func_uint16_t_u_u((255U != 248U), (((*l_64) = (((l_39.f1 , g_63.f3.f0) | p_10.f2) && p_11)) , g_23[0][1])));
            if ((safe_sub_func_int8_t_s_s((+(safe_unary_minus_func_int16_t_s(p_10.f1.f3))), (safe_rshift_func_uint8_t_u_u((g_2[1][3] || ((void*)0 == &g_62)), p_10.f1.f1)))))
            { 
                int32_t **l_77 = &l_38;
                (*l_77) = &g_2[1][3];
            }
            else
            { 
                int32_t **l_78[1][2];
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_78[i][j] = &l_35;
                }
                l_68 = (g_79 = (void*)0);
                g_19 ^= ((!((p_10.f1.f2 = 0x5B) <= (safe_sub_func_int16_t_s_s(p_10.f1.f3, g_63.f3.f2)))) != 0x170B2B3379FE0895LL);
                g_79 = &g_2[1][3];
            }
        }
        else
        { 
            return g_83;
        }
    }
    else
    { 
        int32_t *l_84[4][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0}};
        int i, j;
        g_88--;
    }
    return l_48.f0;
}

static struct S2  func_12(uint64_t  p_13, int16_t  p_14)
{ 
    int32_t *l_18[1];
    struct S2 l_33 = {65530U,{-6,0x0713981D,0x8A,0x68CF},255U};
    int i;
    for (i = 0; i < 1; i++)
        l_18[i] = &g_19;
    ++g_25;
    g_19 = (safe_div_func_int64_t_s_s(((g_30 , (p_13 , ((safe_sub_func_int32_t_s_s(3, p_13)) < g_25))) < p_13), g_21));
    return l_33;
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
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_2[i][j], "g_2[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_17.f0, "g_17.f0", print_hash_value);
    transparent_crc(g_17.f1, "g_17.f1", print_hash_value);
    transparent_crc(g_17.f2, "g_17.f2", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_20[i], "g_20[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_21, "g_21", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_22[i][j], "g_22[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_23[i][j], "g_23[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_30.f0, "g_30.f0", print_hash_value);
    transparent_crc(g_30.f1.f0, "g_30.f1.f0", print_hash_value);
    transparent_crc(g_30.f1.f1, "g_30.f1.f1", print_hash_value);
    transparent_crc(g_30.f1.f2, "g_30.f1.f2", print_hash_value);
    transparent_crc(g_30.f1.f3, "g_30.f1.f3", print_hash_value);
    transparent_crc(g_30.f2, "g_30.f2", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_59, "g_59", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_63.f0, "g_63.f0", print_hash_value);
    transparent_crc(g_63.f1, "g_63.f1", print_hash_value);
    transparent_crc(g_63.f2, "g_63.f2", print_hash_value);
    transparent_crc(g_63.f3.f0, "g_63.f3.f0", print_hash_value);
    transparent_crc(g_63.f3.f1, "g_63.f3.f1", print_hash_value);
    transparent_crc(g_63.f3.f2, "g_63.f3.f2", print_hash_value);
    transparent_crc(g_63.f3.f3, "g_63.f3.f3", print_hash_value);
    transparent_crc(g_83.f0, "g_83.f0", print_hash_value);
    transparent_crc(g_83.f1, "g_83.f1", print_hash_value);
    transparent_crc(g_83.f2, "g_83.f2", print_hash_value);
    transparent_crc(g_88, "g_88", print_hash_value);
    transparent_crc(g_114.f0.f0, "g_114.f0.f0", print_hash_value);
    transparent_crc(g_114.f0.f1, "g_114.f0.f1", print_hash_value);
    transparent_crc(g_114.f0.f2, "g_114.f0.f2", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
