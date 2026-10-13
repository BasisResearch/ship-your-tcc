/* Csmith 0cdc710315cf seed 20, flattened by scripts/csmith_flatten.py */
// Options:   --seed 20 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
safe_lshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (32767 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
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
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
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
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = 0x7C7CE53D;
static int32_t * const g_2 = &g_3;
static int32_t g_6 = 0xD03248C6;
static int32_t * const *g_28 = (void*)0;
static int32_t g_42 = 1;
static int32_t ** const g_45 = (void*)0;
static int32_t *g_47 = (void*)0;
static int32_t **g_46 = &g_47;
static uint8_t g_64 = 246U;
static int64_t g_66 = 8;
static int16_t g_94[2][3] = {{(-6),(-1),(-6)},{(-6),(-1),(-6)}};
static int8_t g_97 = 0;
static int8_t g_107 = (-1);
static int64_t g_108[1][4] = {{1,1,1,1}};
static uint64_t g_110 = 0x8026BCB33D27CF1ALL;
static int64_t *g_132 = &g_66;
static int64_t **g_131 = &g_132;

static const int32_t  func_1(void);
static int32_t * func_7(int8_t  p_8, int32_t * p_9);
static int32_t ** const  func_20(int16_t  p_21, int32_t  p_22);

static const int32_t  func_1(void)
{ 
    int32_t *l_4 = &g_3;
    int32_t *l_5 = &g_6;
    l_4 = g_2;
    (*l_5) |= (*g_2);
    l_4 = func_7((safe_add_func_int64_t_s_s(0xBF2121F90EF307D7LL, (safe_mul_func_uint64_t_u_u((safe_rshift_func_int8_t_s_u(0x0E, 2)), (safe_div_func_uint32_t_u_u(((-4) < (*l_5)), 0x23A266D2)))))), &g_3);
    return (*l_5);
}

static int32_t * func_7(int8_t  p_8, int32_t * p_9)
{ 
    int32_t *l_18 = (void*)0;
    int32_t *l_19 = &g_6;
    uint8_t l_27 = 1U;
    uint32_t l_69[1];
    int64_t *l_81 = &g_66;
    int64_t **l_80 = &l_81;
    uint32_t l_95[1][2];
    int32_t l_105 = 0xEE040EEA;
    int32_t l_106 = 0xCF6B01AB;
    int32_t l_109 = 0xDB549FED;
    int i, j;
    for (i = 0; i < 1; i++)
        l_69[i] = 4294967294U;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_95[i][j] = 0U;
    }
    (*l_19) |= ((*g_2) = (*p_9));
    g_46 = func_20((((*p_9) = (safe_lshift_func_int64_t_s_s(((void*)0 == &l_19), 57))) >= (~((!(((p_8 <= p_8) , 3U) != p_8)) | 0x120DD93BA6AED1D2LL))), l_27);
    if ((safe_lshift_func_uint64_t_u_s((p_8 >= (((safe_sub_func_uint8_t_u_u(((((&p_9 == &p_9) ^ p_8) , 1U) || (*p_9)), p_8)) ^ p_8) <= 4294967289U)), (*l_19))))
    { 
        int8_t l_56 = 8;
        uint8_t *l_63 = &l_27;
        int64_t *l_65 = &g_66;
        uint16_t l_67[4];
        int32_t l_68 = 0x9B5F814D;
        int i;
        for (i = 0; i < 4; i++)
            l_67[i] = 0xDEA6;
        l_68 |= ((*l_19) = (safe_add_func_int8_t_s_s(((l_56 <= ((((((*l_65) ^= (safe_sub_func_int64_t_s_s((safe_lshift_func_int64_t_s_s(((safe_mul_func_uint8_t_u_u(((*l_63) = g_3), g_6)) , g_64), g_3)), 0xC064292628F0FC5ALL))) , 0x4D) || l_67[3]) == p_8) >= (*l_19))) != (*g_2)), 0xB0)));
    }
    else
    { 
        int64_t **l_82 = &l_81;
        int32_t l_85 = 0x356F5961;
        int32_t l_104 = (-8);
        int32_t **l_128[1][4] = {{&l_18,&l_18,&l_18,&l_18}};
        int32_t **l_134 = (void*)0;
        int32_t **l_135 = &g_47;
        int i, j;
        for (l_27 = 0; (l_27 <= 0); l_27 += 1)
        { 
            int32_t **l_70 = &l_18;
            int i;
            if (l_69[l_27])
                break;
            (*l_70) = p_9;
            for (g_3 = 0; (g_3 >= 0); g_3 -= 1)
            { 
                uint64_t l_71 = 0xFFEA361F882D7787LL;
                int32_t **l_74 = (void*)0;
                l_71++;
                (*l_19) &= 1;
                p_9 = (*g_28);
            }
        }
        for (g_66 = 1; (g_66 == (-2)); g_66 = safe_sub_func_int32_t_s_s(g_66, 4))
        { 
            uint8_t *l_78 = (void*)0;
            uint8_t ** const l_77 = &l_78;
            uint8_t **l_79 = &l_78;
            int32_t l_103 = 0xF3B42422;
            int32_t **l_129 = &g_47;
            int64_t **l_133 = &g_132;
            for (p_8 = 0; (p_8 <= 0); p_8 += 1)
            { 
                int i;
                l_79 = l_77;
                if (l_69[p_8])
                    continue;
            }
            l_82 = l_80;
            if (((*l_19) = (safe_lshift_func_int64_t_s_s(l_85, (((safe_div_func_int8_t_s_s((safe_div_func_int8_t_s_s((safe_add_func_int64_t_s_s(0x35D5AA0404C6930ALL, (g_94[0][1] = (safe_mul_func_int8_t_s_s((p_8 || 1U), 0U))))), 1U)), g_64)) , l_95[0][0]) > 0xA30E)))))
            { 
                uint16_t l_96[3];
                int32_t *l_98 = &l_85;
                int32_t *l_99 = &l_85;
                int32_t *l_100 = &g_42;
                int32_t *l_101 = &g_6;
                int32_t *l_102[3][2];
                int i, j;
                for (i = 0; i < 3; i++)
                    l_96[i] = 65535U;
                for (i = 0; i < 3; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_102[i][j] = &g_42;
                }
                if (l_96[2])
                    break;
                g_110--;
                if (l_104)
                    continue;
            }
            else
            { 
                int32_t **l_123 = &g_47;
                int32_t l_130 = 0x2C646EC4;
                l_103 = (((safe_mul_func_uint32_t_u_u((((((((4294967287U <= ((safe_mul_func_int64_t_s_s(((safe_lshift_func_int16_t_s_s((safe_mod_func_int64_t_s_s((safe_mul_func_int32_t_s_s(((((*l_123) = p_9) == (void*)0) || 255U), (*p_9))), p_8)), g_3)) <= p_8), g_107)) ^ l_103)) > l_104) < g_94[0][1]) >= l_103) != (*l_19)) , 0x7C) || (**l_123)), (*p_9))) != 0xDD07) || (**l_123));
                (*l_19) |= (safe_div_func_int8_t_s_s(p_8, (safe_sub_func_int32_t_s_s(((((l_129 = (g_46 = l_128[0][0])) != ((((((l_130 |= ((((-1) ^ p_8) , (**g_28)) , g_94[0][1])) <= 0xEA95DD02E5D8015DLL) < 0x55DA1D3BC2F6D645LL) != g_66) && 0x63E91B5B2358A988LL) , &g_47)) , g_131) != l_133), (*p_9)))));
                g_131 = l_82;
            }
        }
        (*l_135) = (*g_28);
    }
    return p_9;
}

static int32_t ** const  func_20(int16_t  p_21, int32_t  p_22)
{ 
    int32_t *l_40 = &g_3;
    int32_t **l_39 = &l_40;
    int32_t *l_41 = &g_42;
    g_28 = &g_2;
    (*l_41) ^= (safe_add_func_uint64_t_u_u((((safe_div_func_int8_t_s_s(g_3, (safe_add_func_uint8_t_u_u((safe_rshift_func_int32_t_s_u((((safe_mul_func_int8_t_s_s(((void*)0 != l_39), (**l_39))) & (**l_39)) | (**l_39)), p_21)), (-6))))) , g_6) ^ p_21), p_22));
    for (g_42 = 0; (g_42 == 27); g_42++)
    { 
        (*l_39) = (void*)0;
    }
    return g_45;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_42, "g_42", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_66, "g_66", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_94[i][j], "g_94[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_97, "g_97", print_hash_value);
    transparent_crc(g_107, "g_107", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_108[i][j], "g_108[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_110, "g_110", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
