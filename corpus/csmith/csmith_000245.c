/* Csmith 0cdc710315cf seed 245, flattened by scripts/csmith_flatten.py */
// Options:   --seed 245 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_u(int16_t left, unsigned int right )
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
static uint16_t
safe_rshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static uint16_t
safe_unary_minus_func_uint16_t_u(uint16_t ui )
{
  return -ui;
}
static uint32_t
safe_unary_minus_func_uint32_t_u(uint32_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const int64_t  f0;
   uint64_t  f1;
   int16_t  f2;
   int32_t  f3;
};

union U1 {
   int32_t  f0;
};

static int32_t g_2 = (-2);
static struct S0 g_14[2] = {{0xEBF495E3B372BA29LL,0U,-8,0xDB979CD3},{0xEBF495E3B372BA29LL,0U,-8,0xDB979CD3}};
static uint32_t g_27 = 4294967287U;
static int16_t g_44 = 0xDB9B;
static uint8_t g_47 = 0x6F;
static int32_t g_49 = (-6);
static const uint8_t g_62 = 0xD3;
static uint32_t *g_66 = &g_27;
static uint32_t **g_65 = &g_66;
static union U1 g_70 = {0x37EEBDC1};
static union U1 *g_72 = &g_70;

static union U1  func_1(void);
static const int32_t  func_5(uint64_t  p_6, uint64_t  p_7);
static uint64_t  func_8(uint64_t  p_9, uint32_t  p_10, int16_t  p_11, struct S0  p_12, struct S0  p_13);

static union U1  func_1(void)
{ 
    int8_t l_58 = 0x38;
    for (g_2 = (-27); (g_2 > 5); g_2++)
    { 
        int8_t l_57 = 0xCD;
        uint32_t *l_64 = &g_27;
        uint32_t **l_63[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
        union U1 *l_69 = &g_70;
        union U1 l_74 = {1};
        int i;
        if ((((0x4C | 255U) & func_5(func_8(g_2, g_2, g_2, g_14[0], g_14[0]), g_14[0].f1)) , g_14[0].f2))
        { 
            union U1 l_51 = {0x845D487C};
            return l_51;
        }
        else
        { 
            if ((safe_mod_func_int8_t_s_s(g_14[0].f0, g_27)))
            { 
                union U1 l_56[4] = {{0xC590CB25},{0xC590CB25},{0xC590CB25},{0xC590CB25}};
                int32_t *l_59 = &g_49;
                const uint8_t *l_61[2][3] = {{&g_62,&g_62,&g_62},{&g_62,&g_62,&g_62}};
                const uint8_t **l_60 = &l_61[1][2];
                int i, j;
                (*l_59) |= (safe_rshift_func_uint16_t_u_s(((((l_56[3] , (void*)0) != ((l_57 , g_14[0]) , (void*)0)) > l_58) & 0xEDD679405A9AD1DCLL), 12));
                g_49 = ((0x7E8793EB104B5BC6LL > g_49) > (((*l_60) = &g_47) == (void*)0));
                g_65 = l_63[1];
            }
            else
            { 
                int32_t *l_68[3];
                int32_t **l_67 = &l_68[2];
                union U1 **l_71[4];
                int i;
                for (i = 0; i < 3; i++)
                    l_68[i] = &g_2;
                for (i = 0; i < 4; i++)
                    l_71[i] = &l_69;
                (*l_67) = &g_49;
                g_72 = l_69;
                g_70.f0 &= (safe_unary_minus_func_uint32_t_u(0xBD129AC0));
            }
            return l_74;
        }
    }
    return (*g_72);
}

static const int32_t  func_5(uint64_t  p_6, uint64_t  p_7)
{ 
    return g_47;
}

static uint64_t  func_8(uint64_t  p_9, uint32_t  p_10, int16_t  p_11, struct S0  p_12, struct S0  p_13)
{ 
    uint64_t l_18 = 0xB07FB07D0D1D8475LL;
    uint16_t l_23[2];
    int i;
    for (i = 0; i < 2; i++)
        l_23[i] = 0x9D38;
    if (g_14[0].f0)
    { 
        int32_t *l_15 = (void*)0;
        int32_t *l_16 = &g_14[0].f3;
        int32_t *l_17[4][1];
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 1; j++)
                l_17[i][j] = &g_14[0].f3;
        }
        --l_18;
    }
    else
    { 
        int64_t l_45 = (-2);
        for (p_9 = (-20); (p_9 > 44); p_9++)
        { 
            return g_14[0].f2;
        }
lbl_50:
        g_14[0].f3 |= 1;
        for (p_11 = 0; (p_11 <= 1); p_11 += 1)
        { 
            uint32_t *l_26 = &g_27;
            uint16_t l_43 = 0x507B;
            uint8_t *l_46 = &g_47;
            int32_t *l_48 = &g_49;
            (*l_48) |= (g_14[0].f2 == ((p_12.f2 , ((*l_46) = (safe_mul_func_uint32_t_u_u((--(*l_26)), (safe_rshift_func_int16_t_s_u((safe_rshift_func_uint64_t_u_s((safe_rshift_func_uint8_t_u_u((safe_rshift_func_uint16_t_u_u((safe_sub_func_uint8_t_u_u((g_44 = (safe_unary_minus_func_uint16_t_u((safe_add_func_int16_t_s_s(((((g_14[0].f1 ^ 0xE165) , p_12.f2) < g_14[0].f2) || l_43), l_43))))), p_12.f2)), l_45)), g_14[0].f3)), 21)), 2)))))) || l_43));
            for (p_12.f3 = 1; (p_12.f3 >= 0); p_12.f3 -= 1)
            { 
                if (g_27)
                    goto lbl_50;
                return g_27;
            }
        }
    }
    return g_14[0].f3;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_14[i].f0, "g_14[i].f0", print_hash_value);
        transparent_crc(g_14[i].f1, "g_14[i].f1", print_hash_value);
        transparent_crc(g_14[i].f2, "g_14[i].f2", print_hash_value);
        transparent_crc(g_14[i].f3, "g_14[i].f3", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_70.f0, "g_70.f0", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
