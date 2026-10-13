/* Csmith 0cdc710315cf seed 125, flattened by scripts/csmith_flatten.py */
// Options:   --seed 125 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
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
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int8_t g_3 = 0x43;
static uint16_t g_29 = 0xA0E1;
static int32_t g_35 = (-1);
static int32_t * const g_34 = &g_35;
static int64_t g_47 = (-4);
static int32_t *g_53 = &g_35;
static int64_t g_65 = (-1);
static uint64_t g_70 = 0x7EF93BB087A7CD71LL;
static uint32_t g_79 = 0x67715EE0;

static const uint32_t  func_1(void);
static int32_t  func_14(int32_t  p_15);
static int32_t * func_31(int16_t  p_32, int32_t * const  p_33);

static const uint32_t  func_1(void)
{ 
    uint16_t l_2 = 0x9B64;
    uint16_t l_9[3][3] = {{65532U,65532U,65532U},{65526U,65535U,65526U},{65532U,65532U,65532U}};
    int32_t l_69 = 0;
    int i, j;
    g_3 |= l_2;
    if ((0x55E00FE1 ^ (safe_add_func_int8_t_s_s((g_3 &= l_2), ((1U > 0x14) , l_2)))))
    { 
        const uint32_t l_8 = 0x73E7ED6F;
        int32_t *l_67[1];
        int i;
        for (i = 0; i < 1; i++)
            l_67[i] = &g_35;
        for (l_2 = 0; (l_2 == 32); l_2 = safe_add_func_int16_t_s_s(l_2, 7))
        { 
            return l_8;
        }
        for (l_2 = 0; (l_2 <= 2); l_2 += 1)
        { 
            int64_t l_28 = 8;
            uint16_t *l_30 = &g_29;
            int64_t *l_64 = &g_65;
            int32_t l_68[4] = {0x6FF1AB12,0x6FF1AB12,0x6FF1AB12,0x6FF1AB12};
            int i;
            if ((((*l_64) &= ((safe_mul_func_int8_t_s_s(l_8, ((safe_add_func_int8_t_s_s(((func_14((safe_mod_func_uint8_t_u_u((safe_mul_func_uint16_t_u_u(0xBE58, ((*l_30) = ((safe_rshift_func_int16_t_s_s((safe_mod_func_int16_t_s_s(((((((((safe_mul_func_uint64_t_u_u((safe_sub_func_uint32_t_u_u((l_2 < 0), 0xC90E69CD)), g_3)) & g_3) || 3) | l_28) > l_28) & 0) , g_29) >= 0U), 1)), l_2)) != g_3)))), l_2))) , l_9[1][2]) , 0x1A), l_9[0][0])) == l_8))) == l_9[0][2])) == 0U))
            { 
                int32_t **l_66[2][2] = {{&g_53,&g_53},{&g_53,&g_53}};
                int i, j;
                l_67[0] = &g_35;
            }
            else
            { 
                g_70++;
            }
            for (l_69 = 0; (l_69 <= 2); l_69 += 1)
            { 
                int8_t *l_76 = (void*)0;
                int8_t **l_75 = &l_76;
                int i, j;
                (*g_34) = ((safe_add_func_uint32_t_u_u((((*l_75) = &g_3) == &g_3), (5U > l_9[l_2][l_2]))) >= l_68[1]);
                (*g_34) = 0;
            }
        }
        for (g_29 = 0; (g_29 != 38); g_29 = safe_add_func_int16_t_s_s(g_29, 2))
        { 
            for (g_3 = 0; (g_3 <= 2); g_3 += 1)
            { 
                if ((*g_34))
                    break;
                g_79--;
            }
        }
    }
    else
    { 
        int32_t * const l_82 = &g_35;
        int32_t **l_83 = &g_53;
        (*l_83) = func_31(g_65, func_31(l_2, l_82));
        (**l_83) = (safe_lshift_func_int8_t_s_s((0xC2A0AA59 <= ((g_70 <= ((((*l_82) <= 0x54) < g_70) || (**l_83))) , 4294967288U)), g_3));
    }
    return g_70;
}

static int32_t  func_14(int32_t  p_15)
{ 
    int32_t **l_52[2][1];
    uint16_t *l_56 = &g_29;
    int32_t **l_59 = &g_53;
    int64_t *l_60 = &g_47;
    uint64_t l_61 = 0x06F957EEBE844599LL;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_52[i][j] = (void*)0;
    }
    g_53 = func_31(p_15, g_34);
    (*g_34) = ((safe_div_func_int64_t_s_s(((*l_60) |= ((((*l_56)++) , ((void*)0 == l_52[0][0])) == (((((l_59 = (void*)0) == l_52[1][0]) && 0) <= 1) == 0x75))), p_15)) | 0x108D8C41);
    ++l_61;
    return p_15;
}

static int32_t * func_31(int16_t  p_32, int32_t * const  p_33)
{ 
    int32_t *l_45 = (void*)0;
    int32_t l_46 = 0;
    int32_t l_51[4][3] = {{0x5ED53545,6,6},{0x5ED53545,6,6},{0x5ED53545,6,6},{0x5ED53545,6,6}};
    int i, j;
    if ((*g_34))
    { 
        int8_t *l_38[1][3];
        int32_t l_39 = 0;
        int32_t l_40 = 0x4F5774D2;
        int32_t *l_41[3][3];
        int32_t **l_42 = (void*)0;
        int32_t *l_44 = &g_35;
        int32_t **l_43[1];
        uint16_t l_48 = 0U;
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_38[i][j] = &g_3;
        }
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
                l_41[i][j] = (void*)0;
        }
        for (i = 0; i < 1; i++)
            l_43[i] = &l_44;
        l_45 = (l_41[2][0] = ((safe_div_func_uint32_t_u_u(0U, ((g_35 || ((l_39 = p_32) , g_35)) | l_40))) , (void*)0));
        --l_48;
    }
    else
    { 
        (*g_34) = ((-6) | g_3);
    }
    for (g_3 = 0; (g_3 <= 2); g_3 += 1)
    { 
        return &g_35;
    }
    return &g_35;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_79, "g_79", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
