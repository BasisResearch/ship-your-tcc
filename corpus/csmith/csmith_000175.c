/* Csmith 0cdc710315cf seed 175, flattened by scripts/csmith_flatten.py */
// Options:   --seed 175 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
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
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint16_t
safe_rshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
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
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
static int64_t
safe_unary_minus_func_int64_t_s(int64_t si )
{
  return
    (si==(-9223372036854775807L-1)) ?
    ((si)) :
    -si;
}
/* ---- generated program ---- */
static long __undefined;

static uint32_t g_2[2][1] = {{0xB5F5C184},{0xB5F5C184}};
static int32_t g_4 = 0xBD7B1C2A;
static int32_t *g_7 = &g_4;
static int16_t g_9 = (-1);
static int16_t g_11 = 1;
static uint16_t g_62 = 0x496A;
static uint32_t g_74 = 0x03CC2ED1;
static uint16_t *g_88 = &g_62;
static uint16_t ** const g_87 = &g_88;

static uint8_t  func_1(void);
static int8_t  func_12(uint64_t  p_13, uint64_t  p_14, uint8_t  p_15);
static uint8_t  func_16(uint16_t  p_17, int8_t  p_18, uint8_t  p_19);

static uint8_t  func_1(void)
{ 
    int32_t *l_3 = &g_4;
    int16_t *l_8 = &g_9;
    int16_t *l_10 = &g_11;
    int32_t l_98[3][1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_98[i][j] = 0;
    }
    (*l_3) = g_2[1][0];
    (*l_3) = ((safe_div_func_int16_t_s_s(((*l_10) = ((&g_4 != (g_7 = &g_4)) < ((*l_8) = g_4))), 0x996F)) | g_2[0][0]);
    (*g_7) = (func_12(((func_16(((g_4 || (safe_mul_func_uint16_t_u_u((*l_3), (safe_unary_minus_func_int64_t_s(((safe_div_func_uint8_t_u_u((safe_mod_func_int16_t_s_s(((9U != g_2[1][0]) != g_9), g_11)), g_4)) < g_4)))))) , g_2[0][0]), (*l_3), g_2[1][0]) <= g_2[1][0]) , g_2[1][0]), l_98[2][0], g_62) ^ g_2[1][0]);
    return g_4;
}

static int8_t  func_12(uint64_t  p_13, uint64_t  p_14, uint8_t  p_15)
{ 
    uint16_t l_110[3];
    int32_t l_111 = 1;
    int i;
    for (i = 0; i < 3; i++)
        l_110[i] = 1U;
    (*g_7) = (0x3328DBD8298C9807LL > (safe_mod_func_int16_t_s_s((safe_sub_func_int16_t_s_s(0, p_13)), g_11)));
    l_111 &= ((safe_mul_func_int8_t_s_s(((safe_mul_func_uint8_t_u_u((((!0xECE75099) ^ (safe_mul_func_uint32_t_u_u(l_110[0], (p_15 >= 1U)))) , 1U), p_13)) <= g_62), g_74)) > l_110[0]);
    return g_2[1][0];
}

static uint8_t  func_16(uint16_t  p_17, int8_t  p_18, uint8_t  p_19)
{ 
    int16_t *l_29 = &g_11;
    int16_t **l_28 = &l_29;
    int32_t l_43 = 0x3027FE13;
    int32_t **l_55 = &g_7;
    int32_t **l_56 = &g_7;
    for (g_11 = 0; (g_11 >= 0); g_11 -= 1)
    { 
        int8_t l_27 = (-8);
        int16_t **l_30 = &l_29;
        uint8_t l_36 = 5U;
        uint8_t l_44 = 8U;
        uint16_t *l_64 = (void*)0;
        uint16_t *l_66 = &g_62;
        uint16_t **l_65 = &l_66;
        int32_t l_92 = 0;
        if (l_27)
        { 
            int64_t l_31 = 0x17F95E19F8C51723LL;
            l_30 = l_28;
            if (((l_31 < (((l_27 != (safe_sub_func_uint64_t_u_u(((safe_add_func_uint32_t_u_u(((((((-1) == g_2[1][0]) >= g_2[1][0]) && l_31) > p_17) && p_17), l_36)) , p_18), 3))) & g_2[0][0]) , g_4)) == p_18))
            { 
                int32_t *l_37 = &g_4;
                int32_t **l_38 = &l_37;
                int32_t l_45 = (-1);
                (*g_7) = (1 && g_4);
                (*l_38) = l_37;
                l_45 &= (((*l_37) & (g_2[0][0] < (l_44 = (safe_sub_func_uint8_t_u_u(((safe_sub_func_int32_t_s_s((((0x3725AC95 || p_17) >= 251U) && l_43), (*g_7))) ^ p_17), l_43))))) & g_2[0][0]);
            }
            else
            { 
                int16_t l_46[3][3] = {{0x672F,0x672F,0x672F},{(-2),(-2),(-2)},{0x672F,0x672F,0x672F}};
                int i, j;
                (*g_7) &= (l_46[1][2] , 0x42427020);
                return p_18;
            }
        }
        else
        { 
            int16_t l_63 = 0x68B2;
            for (p_17 = 0; (p_17 <= 0); p_17 += 1)
            { 
                uint16_t *l_61 = &g_62;
                int i, j;
            }
        }
        if (((l_64 == ((*l_65) = (void*)0)) <= (safe_lshift_func_int8_t_s_u(g_62, g_11))))
        { 
            int32_t *l_69 = &l_43;
            int32_t *l_70 = &g_4;
            int32_t *l_71 = &g_4;
            int32_t *l_72 = (void*)0;
            int32_t *l_73 = &l_43;
            ++g_74;
        }
        else
        { 
            uint64_t l_89 = 0x30550D74AE622E34LL;
            int8_t *l_97 = &l_27;
            for (p_17 = (-25); (p_17 > 49); ++p_17)
            { 
                uint8_t *l_91 = &l_36;
                if ((*g_7))
                    break;
                (**l_56) = ((safe_sub_func_int32_t_s_s((!(+p_19)), (((safe_div_func_uint32_t_u_u(((safe_mul_func_uint8_t_u_u((g_87 != (p_19 , &g_88)), 0x0C)) > 4294967295U), l_89)) == g_11) , (**l_56)))) <= p_17);
                l_92 |= (((l_89 == (+(0xE5CD ^ ((((*l_91) ^= (**l_55)) | 1) & 0xD1)))) == (**g_87)) || (**l_56));
            }
            (*g_7) = ((safe_rshift_func_uint16_t_u_s((((*l_97) = ((safe_mul_func_uint64_t_u_u(p_18, l_36)) & (1U >= p_18))) >= p_17), g_2[0][0])) , (*g_7));
        }
        return p_19;
    }
    return g_9;
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
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_2[i][j], "g_2[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_9, "g_9", print_hash_value);
    transparent_crc(g_11, "g_11", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
