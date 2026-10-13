/* Csmith 0cdc710315cf seed 293, flattened by scripts/csmith_flatten.py */
// Options:   --seed 293 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
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
static uint8_t
safe_lshift_func_uint8_t_u_u(uint8_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (255 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_sub_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    (si1 - si2);
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

static int32_t g_2 = 0xC64A6847;
static int32_t g_14 = 0x1A35CA42;
static int32_t *g_13[1] = {&g_14};
static int8_t g_21 = 0x6F;
static int32_t g_32 = 8;
static int8_t *g_38[1] = {(void*)0};
static int64_t g_44 = 2;
static uint32_t g_49 = 0xC9E48991;
static int16_t g_52 = 0;
static int8_t g_77[3] = {(-10),(-10),(-10)};

static int64_t  func_1(void);
static int64_t  func_5(uint32_t  p_6, uint8_t  p_7);
static int32_t * func_8(int32_t * p_9);

static int64_t  func_1(void)
{ 
    uint16_t l_25 = 8U;
    int32_t *l_60 = &g_2;
    int32_t *l_61 = (void*)0;
    int32_t *l_62 = &g_2;
    int32_t *l_63 = &g_2;
    int32_t l_64 = 0x88CB35B2;
    int32_t *l_65[4][3] = {{&g_32,&g_2,&g_32},{&g_32,&g_2,&g_32},{(void*)0,&g_32,&g_32},{(void*)0,(void*)0,&g_32}};
    int16_t l_66 = 4;
    uint64_t l_67 = 18446744073709551615U;
    int8_t l_73[1];
    uint16_t l_78 = 65535U;
    int i, j;
    for (i = 0; i < 1; i++)
        l_73[i] = 0x23;
    for (g_2 = 29; (g_2 < 8); g_2--)
    { 
        uint8_t l_22 = 2U;
        int32_t l_26 = (-1);
        int32_t *l_34 = &g_14;
        int8_t l_39 = (-1);
        if (((func_5(g_2, g_2) | g_14) ^ g_2))
        { 
            int8_t *l_20 = &g_21;
            int32_t l_30 = 0xBAC8EF34;
            int32_t l_33 = 0;
            if (((g_2 , (safe_sub_func_int16_t_s_s((~((((((*l_20) = 1) & (((++l_22) , &g_14) == &g_14)) , g_14) > g_14) , l_25)), l_26))) || l_25))
            { 
                if (l_25)
                    break;
            }
            else
            { 
                int32_t *l_31[4][1];
                int i, j;
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 1; j++)
                        l_31[i][j] = &g_32;
                }
                l_33 |= ((safe_unary_minus_func_uint16_t_u((l_25 , (((((safe_sub_func_uint8_t_u_u(l_26, ((g_14 & g_2) ^ g_2))) < l_25) , 0x9935) <= g_14) != g_21)))) >= l_30);
                l_34 = func_8(&g_14);
            }
            return g_21;
        }
        else
        { 
            int32_t l_53[3];
            int i;
            for (i = 0; i < 3; i++)
                l_53[i] = 0xB9986219;
            for (l_26 = 0; (l_26 <= 0); l_26 += 1)
            { 
                int16_t l_35 = (-1);
                int32_t *l_40 = &g_32;
                int64_t *l_43 = &g_44;
                int i;
                (*l_34) = ((l_35 || (safe_div_func_int16_t_s_s(((((g_38[0] = &g_21) == (void*)0) == g_32) == 0x364F2CA1), (*l_34)))) == g_14);
                (*l_40) |= ((*l_34) < (((&g_21 == &g_21) ^ l_39) <= l_25));
                (*l_40) ^= (9 ^ (safe_lshift_func_uint8_t_u_u(((((*l_43) |= 0xCF62392E18E6F963LL) > ((((-1) ^ g_2) , (-1)) ^ l_25)) & 247U), g_21)));
            }
            for (l_22 = 0; (l_22 <= 28); l_22 = safe_add_func_uint16_t_u_u(l_22, 4))
            { 
                uint32_t *l_48 = &g_49;
                int32_t *l_50 = &g_32;
                int16_t *l_51 = &g_52;
                (*l_34) |= (((safe_unary_minus_func_uint32_t_u(((*l_48) = 0xD34E0BB6))) , &g_21) == (void*)0);
                (*l_50) = ((*l_34) = (&g_49 != &g_49));
                (*l_50) = (((*l_51) = 7) && (l_53[1] , (*l_34)));
            }
            l_53[1] = ((*l_34) = (safe_div_func_int64_t_s_s((((safe_lshift_func_uint32_t_u_s(0xAD7B5901, 22)) > g_21) <= ((safe_lshift_func_int8_t_s_u(l_25, l_25)) && g_44)), g_14)));
        }
    }
    --l_67;
    for (g_21 = 0; (g_21 > (-11)); --g_21)
    { 
        int8_t l_72 = 0xB9;
        int32_t l_74 = (-2);
        int32_t l_75[4][1] = {{0x0FA2875F},{0x51D28A62},{0x0FA2875F},{0x51D28A62}};
        int64_t l_76 = (-8);
        int i, j;
        l_78--;
        return g_44;
    }
    return g_32;
}

static int64_t  func_5(uint32_t  p_6, uint8_t  p_7)
{ 
    g_13[0] = func_8(&g_2);
    return g_2;
}

static int32_t * func_8(int32_t * p_9)
{ 
    uint64_t l_10[4];
    int i;
    for (i = 0; i < 4; i++)
        l_10[i] = 0xF27D2703D9B95031LL;
    l_10[2]++;
    return p_9;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_32, "g_32", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_52, "g_52", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_77[i], "g_77[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
