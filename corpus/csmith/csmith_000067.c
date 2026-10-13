/* Csmith 0cdc710315cf seed 67, flattened by scripts/csmith_flatten.py */
// Options:   --seed 67 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint16_t
safe_add_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 + ui2;
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static int16_t
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int16_t
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int32_t
safe_lshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (2147483647 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int8_t
safe_mod_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
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
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
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
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint64_t
safe_sub_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint32_t  f0;
};

static uint32_t g_6 = 0x63F9E923;
static const union U0 g_25 = {4294967295U};
static const union U0 g_28 = {0U};
static const union U0 *g_27 = &g_28;
static union U0 g_36 = {0xE9843C6B};
static uint32_t g_40[3] = {0x52129E5E,0x52129E5E,0x52129E5E};
static int32_t g_44 = (-6);
static int32_t *g_43 = &g_44;
static int32_t g_51 = (-1);
static uint64_t g_65 = 0x3F87FBC07B36282DLL;
static uint8_t g_67 = 0x53;
static uint8_t *g_70 = &g_67;
static uint8_t **g_69 = &g_70;
static int64_t g_82 = 0xC7D9ACC98CB2CAB9LL;
static int32_t g_86 = (-6);
static uint64_t g_123 = 0U;
static int8_t g_127 = (-6);
static uint32_t *g_149 = &g_6;
static int64_t g_159[4] = {0xF2CFB6CEFEB39733LL,0xF2CFB6CEFEB39733LL,0xF2CFB6CEFEB39733LL,0xF2CFB6CEFEB39733LL};
static uint16_t g_161 = 0xA437;
static int8_t g_181 = 0x80;
static uint32_t g_222 = 0xEE824057;

static const int16_t  func_1(void);
static const uint64_t  func_7(uint32_t  p_8, uint64_t  p_9, int32_t  p_10);
static uint8_t  func_12(uint16_t  p_13, int64_t  p_14, uint32_t  p_15, uint32_t * const  p_16);

static const int16_t  func_1(void)
{ 
    uint16_t l_4 = 65527U;
    uint32_t *l_5 = &g_6;
    int64_t *l_108[3];
    int8_t *l_126 = &g_127;
    int32_t l_170 = 1;
    int i;
    for (i = 0; i < 3; i++)
        l_108[i] = &g_82;
    if ((safe_add_func_uint16_t_u_u((l_4 >= 0), (((*l_5) = (l_4 , l_4)) <= l_4))))
    { 
        uint32_t l_11 = 0x9C96C797;
        int64_t l_23 = 1;
        int32_t *l_85 = &g_86;
        (*l_85) ^= (func_7((l_11 >= func_12(l_11, (safe_sub_func_uint64_t_u_u((safe_add_func_uint32_t_u_u((safe_lshift_func_uint8_t_u_s(((((g_6 & l_23) ^ 8) | g_6) == l_4), l_4)), l_4)), l_4)), l_4, l_5)), g_6, g_6) >= l_23);
        for (l_11 = 11; (l_11 >= 56); l_11++)
        { 
            uint16_t l_91[2];
            int i;
            for (i = 0; i < 2; i++)
                l_91[i] = 5U;
            for (g_51 = 2; (g_51 >= 0); g_51 -= 1)
            { 
                (*g_43) |= 0xF29A3864;
                g_43 = &g_44;
            }
            (*l_85) = ((*l_85) > ((safe_div_func_int64_t_s_s((l_91[0] ^ (safe_add_func_uint16_t_u_u((((void*)0 != &g_36) | (*g_70)), (*l_85)))), 0xC60D0FB0CE4C8D1ELL)) , 0));
        }
    }
    else
    { 
        uint16_t l_98 = 65535U;
lbl_112:
        l_98 = (safe_mul_func_uint8_t_u_u((safe_lshift_func_int32_t_s_s((-9), 9)), 0));
        for (g_6 = 26; (g_6 <= 46); ++g_6)
        { 
            for (g_86 = (-12); (g_86 < (-28)); g_86 = safe_sub_func_int32_t_s_s(g_86, 7))
            { 
                int64_t *l_110 = &g_82;
                int64_t **l_109 = &l_110;
                int32_t l_111 = 1;
                (*g_43) ^= ((safe_lshift_func_int32_t_s_u((!g_65), ((safe_add_func_int16_t_s_s((l_108[0] != ((*l_109) = &g_82)), l_98)) , l_111))) | l_111);
                if (l_4)
                    goto lbl_112;
            }
            (*g_43) = (safe_sub_func_uint32_t_u_u(l_98, ((((*g_43) <= ((safe_mod_func_uint16_t_u_u(g_36.f0, 0xCD41)) != 0x37)) < (*g_43)) >= l_98)));
        }
    }
    if ((g_28.f0 < (((safe_lshift_func_int16_t_s_u(((safe_add_func_int8_t_s_s(((*l_126) = (safe_rshift_func_uint8_t_u_s(((*g_70) ^= g_123), (safe_sub_func_uint16_t_u_u(((((&g_51 == (void*)0) || (-3)) | g_40[1]) & g_65), l_4))))), 9U)) , (-8)), 3)) >= l_4) && l_4)))
    { 
        int32_t **l_128 = &g_43;
        (*l_128) = &g_86;
    }
    else
    { 
        g_51 |= (((((l_108[0] != (void*)0) >= 0x0368A9B892D6D572LL) , l_4) == g_82) & (*g_43));
    }
    for (g_123 = 0; (g_123 <= 11); g_123 = safe_add_func_uint64_t_u_u(g_123, 3))
    { 
        uint16_t l_140 = 0x35B2;
        int8_t *l_143[2];
        uint32_t *l_150 = &g_6;
        uint64_t *l_191 = &g_65;
        int32_t l_192 = 0x1DEEF009;
        int i;
        for (i = 0; i < 2; i++)
            l_143[i] = &g_127;
    }
    return l_170;
}

static const uint64_t  func_7(uint32_t  p_8, uint64_t  p_9, int32_t  p_10)
{ 
    uint32_t l_49 = 0x04C3D8FF;
    const int32_t *l_84 = &g_44;
    const int32_t **l_83 = &l_84;
    for (p_10 = 0; (p_10 <= 2); p_10 += 1)
    { 
        int32_t *l_50 = &g_51;
        uint64_t *l_64 = &g_65;
        uint8_t *l_66 = &g_67;
        uint8_t **l_71 = &g_70;
        int64_t *l_80 = (void*)0;
        int64_t *l_81 = &g_82;
        int i;
        (*l_50) ^= ((((g_40[p_10] > (+(g_40[p_10] | (((safe_mul_func_int16_t_s_s(p_9, g_40[1])) && l_49) | 1)))) , g_44) ^ p_8) != p_9);
        if (((safe_mod_func_int8_t_s_s((safe_rshift_func_int16_t_s_s(((safe_rshift_func_uint32_t_u_s(4294967294U, ((safe_mul_func_uint8_t_u_u(((*l_66) = (0x6C49 != (safe_sub_func_uint64_t_u_u(((*l_64) = ((safe_mul_func_int8_t_s_s((&l_49 != &p_8), 0x5E)) | p_9)), 0x03458E53ADD1EB10LL)))), 0xDA)) <= g_6))) > (*l_50)), 8)), p_9)) && p_9))
        { 
            return p_8;
        }
        else
        { 
            int32_t l_68[2];
            int i;
            for (i = 0; i < 2; i++)
                l_68[i] = 0xC86549E1;
            if ((*g_43))
                break;
            if ((((l_68[1] = g_67) || ((255U > (*l_50)) ^ 0x8470FB2E)) >= (*l_50)))
            { 
                return l_68[1];
            }
            else
            { 
                g_69 = (l_71 = g_69);
                if (p_9)
                    break;
            }
            (*l_50) ^= (*g_43);
        }
        (*l_50) = (((g_40[p_10]--) | ((safe_add_func_uint32_t_u_u(7U, ((safe_rshift_func_int64_t_s_s(0x02DE648FB317E5C3LL, ((*l_81) &= (safe_add_func_uint16_t_u_u(p_8, p_10))))) , g_25.f0))) > 0U)) == 0xD880);
    }
    (*l_83) = ((p_10 <= (((*g_27) , g_40[2]) | g_82)) , (void*)0);
    (*g_43) = p_9;
    return g_36.f0;
}

static uint8_t  func_12(uint16_t  p_13, int64_t  p_14, uint32_t  p_15, uint32_t * const  p_16)
{ 
    const union U0 *l_24 = &g_25;
    const union U0 **l_26[4][2];
    union U0 *l_35 = &g_36;
    int32_t l_37[1];
    const uint16_t l_38 = 0U;
    int32_t *l_39[3][2] = {{(void*)0,(void*)0},{(void*)0,(void*)0},{(void*)0,(void*)0}};
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 2; j++)
            l_26[i][j] = &l_24;
    }
    for (i = 0; i < 1; i++)
        l_37[i] = 0;
    g_27 = l_24;
    g_40[1] = (!(((safe_mod_func_uint64_t_u_u(((((~p_13) , ((-1) != (safe_lshift_func_int16_t_s_s((((((((void*)0 != l_35) >= 0x56) < p_15) && g_28.f0) , g_28.f0) | 0x05E60D1F283019C3LL), l_37[0])))) != g_36.f0) < 0x302C), p_14)) <= 0xF0F5) | l_38));
    for (p_14 = 0; (p_14 <= 2); p_14 += 1)
    { 
        int32_t **l_45 = &g_43;
        int i;
        (*l_45) = (((safe_rshift_func_int8_t_s_s(g_40[p_14], 2)) != (7U || p_13)) , g_43);
        if (p_14)
            continue;
    }
    return g_40[2];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_25.f0, "g_25.f0", print_hash_value);
    transparent_crc(g_28.f0, "g_28.f0", print_hash_value);
    transparent_crc(g_36.f0, "g_36.f0", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_40[i], "g_40[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_67, "g_67", print_hash_value);
    transparent_crc(g_82, "g_82", print_hash_value);
    transparent_crc(g_86, "g_86", print_hash_value);
    transparent_crc(g_123, "g_123", print_hash_value);
    transparent_crc(g_127, "g_127", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_159[i], "g_159[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_161, "g_161", print_hash_value);
    transparent_crc(g_181, "g_181", print_hash_value);
    transparent_crc(g_222, "g_222", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
