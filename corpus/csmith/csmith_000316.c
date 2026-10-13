/* Csmith 0cdc710315cf seed 316, flattened by scripts/csmith_flatten.py */
// Options:   --seed 316 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
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
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static const int64_t g_5 = 0x080C170CD509978FLL;
static int32_t g_6[3][2] = {{0x2A3B3F36,0x2A3B3F36},{0x2A3B3F36,0x2A3B3F36},{0x2A3B3F36,0x2A3B3F36}};
static int32_t g_29 = 7;
static int16_t g_37 = 0;
static int32_t g_39 = 0xEBDAB6AC;
static int32_t *g_45 = &g_39;
static int32_t **g_44 = &g_45;
static uint16_t g_57 = 7U;
static int16_t g_71 = (-3);
static uint64_t g_72 = 18446744073709551614U;

static int32_t  func_1(void);
static int32_t * func_2(const uint64_t  p_3);
static int32_t ** func_9(int64_t  p_10, int32_t ** const  p_11, int8_t  p_12);

static int32_t  func_1(void)
{ 
    uint32_t l_4[1];
    int32_t **l_84 = &g_45;
    int i;
    for (i = 0; i < 1; i++)
        l_4[i] = 0x24F84B81;
    (*l_84) = func_2((l_4[0] , g_5));
    return (**l_84);
}

static int32_t * func_2(const uint64_t  p_3)
{ 
    int32_t *l_15[2];
    int32_t ** const l_14[3][1] = {{&l_15[1]},{&l_15[1]},{&l_15[1]}};
    int16_t *l_83 = &g_71;
    int i, j;
    for (i = 0; i < 2; i++)
        l_15[i] = (void*)0;
    for (g_6[2][0] = 0; (g_6[2][0] != (-21)); g_6[2][0] = safe_sub_func_int8_t_s_s(g_6[2][0], 3))
    { 
        int8_t l_13[2];
        int32_t **l_16 = &l_15[1];
        int32_t l_69 = 0;
        int32_t l_70 = 0xA39DA5D7;
        int i;
        for (i = 0; i < 2; i++)
            l_13[i] = 0x7E;
        g_44 = func_9(l_13[0], (l_16 = l_14[2][0]), g_5);
        g_72++;
    }
    (*g_45) ^= ((safe_rshift_func_uint16_t_u_s(((safe_sub_func_uint8_t_u_u((safe_div_func_uint64_t_u_u((safe_div_func_int16_t_s_s(((*l_83) = g_72), p_3)), 1)), p_3)) , p_3), g_72)) != 0U);
    return (*g_44);
}

static int32_t ** func_9(int64_t  p_10, int32_t ** const  p_11, int8_t  p_12)
{ 
    uint64_t l_20 = 18446744073709551611U;
    int32_t *l_43 = (void*)0;
    int32_t **l_42 = &l_43;
    int32_t l_59 = 0;
    for (p_12 = 0; (p_12 <= (-9)); p_12 = safe_sub_func_uint8_t_u_u(p_12, 6))
    { 
        uint32_t l_27 = 0x6548AAB5;
        int32_t *l_28 = &g_29;
        int16_t *l_36 = &g_37;
        int32_t *l_38 = &g_39;
        uint8_t l_40[1];
        int i;
        for (i = 0; i < 1; i++)
            l_40[i] = 3U;
        (*l_28) ^= (((~l_20) == (safe_sub_func_int32_t_s_s(((safe_mul_func_int32_t_s_s((p_10 , (safe_lshift_func_int64_t_s_u(g_6[2][0], p_12))), g_6[2][0])) != 0x058E), l_27))) && g_6[0][1]);
        (*l_38) &= (safe_mul_func_uint32_t_u_u((l_20 >= p_12), (safe_div_func_int32_t_s_s((safe_sub_func_int8_t_s_s(((((*l_36) = ((((((*l_28) = (((((void*)0 == l_28) , p_12) >= (*l_28)) & p_10)) ^ p_12) || 0xA450) < p_12) < l_27)) != l_20) | g_6[0][1]), g_5)), 0x94484F75))));
        for (g_29 = 0; (g_29 >= 0); g_29 -= 1)
        { 
            (*p_11) = &g_6[2][0];
        }
    }
    if ((~0xF4AEDF2F))
    { 
        (*p_11) = (*p_11);
        return g_44;
    }
    else
    { 
        int32_t **l_60 = &g_45;
        if ((safe_mul_func_int32_t_s_s(0x2646899D, (((((((void*)0 == (*p_11)) != 0x7B) , g_6[1][0]) , 0x47EB) > p_10) == p_10))))
        { 
            int64_t l_48[2][4] = {{(-8),(-6),(-8),(-6)},{(-8),(-6),(-8),(-6)}};
            int32_t *l_58[4][4] = {{&g_29,(void*)0,(void*)0,&g_29},{(void*)0,&g_29,(void*)0,(void*)0},{&g_29,&g_29,&g_29,&g_29},{&g_29,(void*)0,(void*)0,&g_29}};
            int i, j;
            l_59 ^= (p_10 , (l_48[0][0] > ((safe_add_func_int8_t_s_s((safe_mod_func_uint8_t_u_u((g_57 = ((((safe_lshift_func_int32_t_s_s((-10), (**g_44))) >= 0U) || g_37) > 65535U)), p_12)), p_10)) | 18446744073709551615U)));
            return l_60;
        }
        else
        { 
            int64_t *l_67 = (void*)0;
            int64_t *l_68[3][2];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 2; j++)
                    l_68[i][j] = (void*)0;
            }
            if ((safe_div_func_uint8_t_u_u(p_12, ((safe_rshift_func_int64_t_s_s((((**l_60) = (p_12 , (safe_div_func_int64_t_s_s((p_10 = g_57), g_37)))) ^ g_57), 54)) , 0x66))))
            { 
                return &g_45;
            }
            else
            { 
                return &g_45;
            }
        }
    }
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_5, "g_5", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_6[i][j], "g_6[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_29, "g_29", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_57, "g_57", print_hash_value);
    transparent_crc(g_71, "g_71", print_hash_value);
    transparent_crc(g_72, "g_72", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
