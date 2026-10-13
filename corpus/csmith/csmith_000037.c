/* Csmith 0cdc710315cf seed 37, flattened by scripts/csmith_flatten.py */
// Options:   --seed 37 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_4 = 0x96CA2CB5;
static int32_t **g_28 = (void*)0;
static int16_t g_32 = (-1);
static int8_t g_36 = 0x65;
static int32_t g_37 = (-1);
static uint64_t g_39 = 1U;
static uint8_t g_49 = 0x89;
static uint16_t g_53 = 8U;
static int16_t g_64 = (-7);
static int32_t g_75 = 0;
static uint64_t g_81 = 0xFA6B6B07C152A3E9LL;
static int64_t g_87 = 0x5641E62B9925C1F9LL;
static uint64_t g_88 = 0xCBEB01B519BE197FLL;

static int16_t  func_1(void);
static int32_t * func_6(int64_t  p_7, uint16_t  p_8, const int64_t  p_9);
static int16_t  func_12(int8_t  p_13);

static int16_t  func_1(void)
{ 
    int32_t *l_3 = &g_4;
    int32_t **l_2 = &l_3;
    uint16_t l_5[3];
    int32_t l_71 = 2;
    int32_t l_85 = 0x258FD7CB;
    int32_t l_86[4][2] = {{0x72762B6C,0x44EC192D},{0x44EC192D,0x72762B6C},{0x44EC192D,0x44EC192D},{0x72762B6C,0x44EC192D}};
    int16_t l_91 = 0xC1E6;
    int i, j;
    for (i = 0; i < 3; i++)
        l_5[i] = 0x9244;
    (*l_2) = (void*)0;
    for (g_4 = 0; (g_4 <= 2); g_4 += 1)
    { 
        int8_t l_14 = (-7);
        int32_t **l_33 = (void*)0;
        int32_t l_67 = 0xC085588C;
        int32_t l_68 = (-5);
        int32_t l_70 = 0xBA4BA9E7;
        int32_t l_74 = 0xC80536D0;
        int i;
        if (((((*l_2) = func_6((safe_mul_func_uint16_t_u_u((l_5[g_4] <= g_4), func_12(l_14))), g_4, g_4)) != (void*)0) != l_5[g_4]))
        { 
            int16_t *l_30 = (void*)0;
            int16_t *l_31 = &g_32;
            int32_t l_34 = 2;
            int16_t l_35 = 0x87E9;
            int32_t **l_50[2][1];
            uint32_t l_59 = 0U;
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_50[i][j] = &l_3;
            }
            if ((safe_add_func_uint16_t_u_u(l_5[g_4], (((safe_mul_func_int64_t_s_s((((safe_div_func_int8_t_s_s((safe_mul_func_int16_t_s_s((l_34 = (safe_div_func_uint32_t_u_u(((g_28 == (((+((*l_31) &= ((g_4 == l_14) , (-3)))) || 0xB6BD) , l_33)) >= g_4), 0x08BEE6DA))), 0xC82F)), g_4)) > 0x06ED55C5) | 1U), l_35)) >= g_4) >= (**l_2)))))
            { 
                int32_t *l_38[3][2];
                int i, j;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_38[i][j] = &l_34;
                }
                if (l_35)
                    break;
                g_39--;
            }
            else
            { 
                uint8_t *l_48 = &g_49;
                const int32_t l_51[1][1] = {{(-1)}};
                int32_t *l_52 = &g_37;
                int i, j;
                (*l_2) = (l_52 = ((safe_mul_func_uint16_t_u_u(((safe_add_func_uint8_t_u_u(l_34, ((*l_48) = 247U))) & ((l_50[0][0] != g_28) > 0x16)), l_51[0][0])) , &g_37));
                --g_53;
                (*l_2) = (g_37 , func_6(g_37, (((((safe_unary_minus_func_int16_t_s(((*l_31) = (((safe_rshift_func_int8_t_s_s((-7), g_36)) == g_39) != g_49)))) != 0x91CD3C6E) != 1) != (*l_52)) < l_59), g_4));
            }
        }
        else
        { 
            int64_t l_66 = 0x8EDA297315BB970ELL;
            int32_t l_84[1];
            int i;
            for (i = 0; i < 1; i++)
                l_84[i] = 0;
            for (g_36 = 2; (g_36 >= 0); g_36 -= 1)
            { 
                int32_t *l_62 = (void*)0;
                int32_t *l_63 = &g_37;
                if ((*l_3))
                    break;
                g_64 |= (((safe_mul_func_int32_t_s_s(0x811E3584, ((*l_63) = (-7)))) && g_49) , 0x63317A7F);
            }
            for (g_39 = 0; (g_39 <= 2); g_39 += 1)
            { 
                int32_t *l_65 = &g_37;
                int32_t *l_69 = &l_68;
                int32_t *l_72 = &l_68;
                int32_t *l_73 = &l_71;
                int32_t *l_76 = &l_68;
                int32_t *l_77 = &l_68;
                int32_t *l_78 = &l_74;
                int32_t *l_79 = (void*)0;
                int32_t *l_80[1][2];
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_80[i][j] = &g_75;
                }
                g_81++;
                g_88++;
                return l_91;
            }
        }
    }
    return g_81;
}

static int32_t * func_6(int64_t  p_7, uint16_t  p_8, const int64_t  p_9)
{ 
    int32_t *l_16 = &g_4;
    int32_t **l_17 = &l_16;
    (*l_17) = l_16;
    return &g_4;
}

static int16_t  func_12(int8_t  p_13)
{ 
    int32_t l_15 = (-1);
    return l_15;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_36, "g_36", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_75, "g_75", print_hash_value);
    transparent_crc(g_81, "g_81", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    transparent_crc(g_88, "g_88", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
