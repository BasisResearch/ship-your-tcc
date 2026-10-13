/* Csmith 0cdc710315cf seed 41, flattened by scripts/csmith_flatten.py */
// Options:   --seed 41 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_unary_minus_func_uint64_t_u(uint64_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static const uint8_t g_9[1] = {1U};
static int8_t g_13 = 1;
static int32_t g_20 = 0xA7B445AC;
static int32_t **g_22 = (void*)0;
static int8_t g_30 = 6;
static int32_t g_32 = (-2);
static int64_t g_44 = 0xF2C67A95534FD39ELL;
static int8_t g_45[3] = {0xDF,0xDF,0xDF};
static int64_t *g_54 = (void*)0;
static int64_t **g_53 = &g_54;
static int16_t g_65 = 0xDE8B;
static int16_t g_67 = (-6);
static int64_t g_68 = (-10);
static uint64_t g_69 = 0xE763FF6366113C50LL;
static int8_t g_76 = 0x83;

static const uint16_t  func_1(void);
static int32_t  func_4(const uint8_t  p_5, int8_t  p_6, int8_t  p_7, uint8_t  p_8);
static uint8_t  func_10(int64_t  p_11);

static const uint16_t  func_1(void)
{ 
    int8_t *l_12 = &g_13;
    int32_t l_72 = 1;
    int32_t *l_77 = (void*)0;
    int32_t *l_78 = &g_20;
    g_76 |= (func_4(g_9[0], (func_10(((5 ^ ((*l_12) |= ((g_9[0] > g_9[0]) <= 0x3F5047C4))) , g_9[0])) < l_72), g_67, l_72) < 0U);
    (*l_78) &= l_72;
    return (*l_78);
}

static int32_t  func_4(const uint8_t  p_5, int8_t  p_6, int8_t  p_7, uint8_t  p_8)
{ 
    int32_t *l_73 = &g_20;
    int32_t **l_74 = &l_73;
    (*l_73) = 0xD3A61860;
    (*l_74) = l_73;
    (**l_74) = ((p_8 == ((safe_unary_minus_func_uint64_t_u(((g_9[0] & g_45[1]) >= (*l_73)))) , (**l_74))) ^ g_30);
    return (**l_74);
}

static uint8_t  func_10(int64_t  p_11)
{ 
    int8_t *l_14[3][4] = {{&g_13,&g_13,&g_13,&g_13},{&g_13,&g_13,&g_13,&g_13},{&g_13,&g_13,&g_13,&g_13}};
    int32_t *l_19 = &g_20;
    uint8_t l_46 = 248U;
    int32_t *l_51 = &g_20;
    int32_t *l_62 = &g_32;
    int32_t *l_63[3];
    int16_t l_64 = (-1);
    int16_t l_66[3][3];
    int i, j;
    for (i = 0; i < 3; i++)
        l_63[i] = &g_20;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
            l_66[i][j] = 7;
    }
    if ((((((l_14[2][1] = l_14[2][1]) == ((safe_div_func_uint64_t_u_u((((*l_19) |= p_11) < (g_13 > 0x9E)), g_13)) , (void*)0)) , p_11) | p_11) , 0x2D6273D5))
    { 
        int32_t **l_21 = &l_19;
        (*l_21) = &g_20;
        g_22 = l_21;
        for (g_13 = 0; (g_13 != (-23)); g_13--)
        { 
            for (p_11 = 0; (p_11 == (-6)); --p_11)
            { 
                uint32_t l_27 = 0U;
                if ((**g_22))
                    break;
                l_27--;
                (**l_21) &= p_11;
            }
        }
    }
    else
    { 
        int32_t *l_31 = &g_32;
        int64_t *l_43[3][2] = {{(void*)0,(void*)0},{&g_44,(void*)0},{(void*)0,&g_44}};
        uint64_t l_55 = 18446744073709551606U;
        int i, j;
        (*l_19) &= (p_11 , ((((g_30 != g_9[0]) , (void*)0) == (void*)0) & 65527U));
        (*l_31) &= (*l_19);
        if (((safe_rshift_func_uint16_t_u_u((safe_mul_func_int64_t_s_s(((((1U <= ((safe_div_func_int16_t_s_s(((safe_div_func_int64_t_s_s((g_45[1] &= ((((safe_mul_func_int8_t_s_s(0x93, (*l_31))) ^ p_11) , (*l_19)) , (*l_31))), (*l_19))) <= 0), 0x7835)) , g_13)) || l_46) & (*l_31)) > (*l_19)), (-8))), p_11)) | p_11))
        { 
            int32_t *l_47 = (void*)0;
            for (g_32 = 1; (g_32 >= 0); g_32 -= 1)
            { 
                if (p_11)
                    break;
            }
            l_19 = l_47;
            for (p_11 = 0; (p_11 == (-12)); p_11 = safe_sub_func_uint64_t_u_u(p_11, 5))
            { 
                int32_t **l_50 = &l_19;
                int64_t **l_52 = &l_43[2][0];
                l_51 = ((*l_50) = l_47);
                g_53 = l_52;
                (*l_50) = l_51;
            }
        }
        else
        { 
            return l_55;
        }
    }
    g_20 = ((*l_62) = (safe_mod_func_int16_t_s_s((safe_add_func_int16_t_s_s((safe_rshift_func_int32_t_s_u((((l_46 >= p_11) > (g_9[0] == 4U)) <= g_20), 18)), g_13)), 0x56A7)));
    ++g_69;
    return (*l_62);
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_9[i], "g_9[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_45[i], "g_45[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_67, "g_67", print_hash_value);
    transparent_crc(g_68, "g_68", print_hash_value);
    transparent_crc(g_69, "g_69", print_hash_value);
    transparent_crc(g_76, "g_76", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
