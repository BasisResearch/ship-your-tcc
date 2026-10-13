/* Csmith 0cdc710315cf seed 13, flattened by scripts/csmith_flatten.py */
// Options:   --seed 13 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int64_t
safe_add_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (9223372036854775807L -si2))) || ((si1<0) && (si2<0) && (si1 < ((-9223372036854775807L-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
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
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = 0x4FDA0E60;
static int64_t g_22 = 1;
static uint8_t g_27 = 0x3E;
static int32_t g_55 = 0x2C910760;
static int32_t *g_54[2] = {&g_55,&g_55};
static int32_t *g_56[4] = {&g_55,&g_55,&g_55,&g_55};
static uint8_t *g_61 = &g_27;
static uint8_t **g_60 = &g_61;
static int32_t g_75 = 0xF6860338;

static int32_t  func_1(void);
static int32_t * func_6(const int32_t * p_7);
static const int32_t * func_8(uint8_t  p_9);

static int32_t  func_1(void)
{ 
    uint8_t l_2[2];
    int32_t l_30 = 0xA488D514;
    int i;
    for (i = 0; i < 2; i++)
        l_2[i] = 0xFC;
    for (g_3 = 0; g_3 < 2; g_3 = g_3 + 1)
    {
        l_2[g_3] = 0x51;
    }
    for (g_3 = 0; (g_3 != (-11)); g_3--)
    { 
        int64_t *l_21[4][3] = {{&g_22,(void*)0,&g_22},{&g_22,&g_22,(void*)0},{(void*)0,(void*)0,(void*)0},{(void*)0,&g_22,&g_22}};
        const int32_t l_28 = 0xFBC43CC9;
        uint8_t *l_29[4];
        int32_t **l_65 = &g_56[3];
        int32_t **l_66 = &g_54[0];
        int i, j;
        for (i = 0; i < 4; i++)
            l_29[i] = &l_2[1];
        (*l_66) = ((*l_65) = func_6(func_8((l_30 = (((safe_add_func_uint16_t_u_u((safe_mul_func_int16_t_s_s(((safe_mod_func_int64_t_s_s(((((safe_div_func_uint8_t_u_u(((g_3 != (g_27 &= ((!(safe_lshift_func_uint16_t_u_s(((g_22 = 7) && (safe_mod_func_int32_t_s_s((safe_rshift_func_uint64_t_u_s(0xAFF63588C556123DLL, 42)), g_22))), 7))) , g_3))) , l_2[1]), 1U)) ^ l_28) , l_28) <= g_3), l_2[0])) ^ g_3), 0xD9A5)), g_3)) <= 0x7C39C9D9403132D6LL) ^ g_3)))));
        for (g_22 = (-11); (g_22 <= (-13)); g_22 = safe_sub_func_int32_t_s_s(g_22, 9))
        { 
            int32_t *l_72 = &g_55;
            int32_t *l_73 = (void*)0;
            int32_t *l_74[4][1] = {{&g_75},{(void*)0},{&g_75},{(void*)0}};
            int32_t l_76 = 0x39852136;
            int i, j;
            for (l_30 = (-23); (l_30 != (-17)); ++l_30)
            { 
                int32_t l_71 = 0;
                l_71 ^= l_30;
            }
            if (l_2[0])
                continue;
            l_76 &= ((*l_72) &= l_30);
        }
    }
    return l_2[0];
}

static int32_t * func_6(const int32_t * p_7)
{ 
    int16_t l_34 = (-1);
    int32_t l_43 = (-3);
    int32_t *l_64 = &g_55;
lbl_57:
    for (g_27 = 5; (g_27 != 25); g_27 = safe_add_func_uint16_t_u_u(g_27, 5))
    { 
        uint8_t *l_39 = &g_27;
        uint8_t **l_40 = (void*)0;
        uint8_t **l_41 = &l_39;
        int32_t l_42 = 0x98072F25;
        int64_t *l_44 = &g_22;
        const int32_t *l_46 = &l_43;
        const int32_t **l_45 = &l_46;
        l_43 ^= ((l_34 && (safe_mul_func_int8_t_s_s(0, (safe_mod_func_int32_t_s_s((((*l_41) = l_39) != (void*)0), 1))))) , l_42);
        (*l_45) = func_8((&g_22 != l_44));
    }
    for (g_27 = 0; (g_27 < 29); g_27 = safe_add_func_int64_t_s_s(g_27, 1))
    { 
        uint16_t l_49 = 0U;
        int32_t *l_52 = (void*)0;
        int32_t **l_53[1];
        int i;
        for (i = 0; i < 1; i++)
            l_53[i] = &l_52;
        g_55 = ((l_49 ^ (safe_sub_func_uint64_t_u_u(((g_54[1] = l_52) != (g_56[3] = &l_43)), l_34))) | l_43);
        if (l_49)
            goto lbl_57;
    }
    (*l_64) = (safe_mul_func_int8_t_s_s(((void*)0 == g_60), (((((safe_div_func_uint8_t_u_u(0xFF, l_43)) | 0xDD50170A2C9EA21FLL) >= 18446744073709551615U) , l_34) ^ l_43)));
    return &g_55;
}

static const int32_t * func_8(uint8_t  p_9)
{ 
    const int32_t *l_31 = &g_3;
    return l_31;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_27, "g_27", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_75, "g_75", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
