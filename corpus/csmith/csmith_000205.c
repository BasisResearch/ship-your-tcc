/* Csmith 0cdc710315cf seed 205, flattened by scripts/csmith_flatten.py */
// Options:   --seed 205 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ui1 + ui2;
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
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int32_t
safe_rshift_func_int32_t_s_u(int32_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
    ((left)) :
    (left >> ((int)right));
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = (-2);
static int32_t g_20 = 0x047D2038;
static uint32_t g_21 = 5U;
static int8_t g_46 = 0;
static int32_t g_60 = 0xEAFE2AE0;
static const int8_t *g_64 = (void*)0;
static const int8_t **g_63 = &g_64;
static int32_t *g_66 = &g_60;
static int32_t **g_65 = &g_66;
static uint32_t g_74 = 4294967295U;
static uint8_t g_101 = 1U;
static int64_t g_104 = 0x6C0B5FFC7B43BFD2LL;
static uint64_t g_113 = 0x541361E08BDFDF76LL;

static int8_t  func_1(void);
static int32_t * func_6(uint8_t  p_7, int32_t * p_8, uint32_t  p_9);
static uint32_t  func_11(int32_t * const  p_12, uint32_t  p_13, int32_t * p_14, int64_t  p_15);

static int8_t  func_1(void)
{ 
    int32_t *l_10 = &g_2;
    int16_t l_16 = 0;
    int32_t *l_17 = (void*)0;
    for (g_2 = 0; (g_2 == (-16)); g_2--)
    { 
        int64_t l_5 = 0x0E60F951FE77DD68LL;
        if (l_5)
            break;
    }
    (*g_65) = func_6(g_2, l_10, func_11(&g_2, l_16, l_17, g_2));
    if ((g_2 , (g_46 == ((safe_unary_minus_func_int16_t_s(((*l_10) ^ (*l_10)))) , g_46))))
    { 
        (*g_65) = (*g_65);
    }
    else
    { 
        int16_t l_95[2];
        const int8_t *l_98 = &g_46;
        uint64_t l_111 = 18446744073709551608U;
        int i;
        for (i = 0; i < 2; i++)
            l_95[i] = 0x3EFF;
        for (l_16 = 1; (l_16 >= 0); l_16 -= 1)
        { 
            const int8_t **l_99 = &l_98;
            uint8_t *l_100 = &g_101;
            int64_t *l_102 = (void*)0;
            int64_t *l_103 = &g_104;
            int8_t *l_110 = (void*)0;
            int i;
            if (l_95[l_16])
                break;
            if ((safe_rshift_func_uint64_t_u_s(g_20, ((*l_103) = (((*l_100) |= ((((*l_99) = l_98) == (void*)0) < (*g_66))) <= 0xBE)))))
            { 
                int64_t **l_109 = &l_103;
                uint64_t *l_112 = &g_113;
                int32_t l_114 = 0;
                (**g_65) = (((safe_add_func_uint16_t_u_u(((-1) >= ((((((safe_mul_func_uint64_t_u_u(((((*l_112) &= (((((*l_109) = &g_104) != ((l_110 == (*l_99)) , &g_104)) , g_21) > l_111)) , 1U) & 1), 0x39C3AD06F0504CB0LL)) , l_95[0]) > g_46) , l_114) >= 0U) , l_95[l_16])), 0x65FC)) >= g_46) | 0x95);
            }
            else
            { 
                uint8_t l_121[2];
                int32_t *l_122 = &g_20;
                int i;
                for (i = 0; i < 2; i++)
                    l_121[i] = 0x08;
                (*l_122) |= (safe_rshift_func_int64_t_s_s(((safe_rshift_func_int32_t_s_s((l_95[l_16] , (safe_sub_func_int64_t_s_s(0x58067933A92D2993LL, ((g_21 , 0xAE) != l_121[1])))), 18)) > (-1)), (*l_10)));
                return g_2;
            }
        }
        return (*l_10);
    }
    return (*l_10);
}

static int32_t * func_6(uint8_t  p_7, int32_t * p_8, uint32_t  p_9)
{ 
    int64_t l_33[3];
    int32_t *l_35 = &g_20;
    int32_t **l_34 = &l_35;
    const int8_t *l_62 = &g_46;
    const int8_t **l_61 = &l_62;
    int i;
    for (i = 0; i < 3; i++)
        l_33[i] = 0x02C910760B96BC97LL;
lbl_47:
    (*p_8) = (((0x3F0A < (p_9 > g_21)) && p_9) | 0xD0FE);
lbl_93:
    for (g_2 = 0; (g_2 <= (-16)); g_2--)
    { 
        int32_t *l_32[2][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
        int i, j;
        if (((((((void*)0 == l_32[0][3]) ^ (((g_21 , g_21) , l_33[2]) >= g_21)) == 0) & g_20) | p_7))
        { 
            if ((*p_8))
                break;
            l_34 = &p_8;
        }
        else
        { 
            int16_t l_36 = 0x7981;
            for (g_21 = 0; (g_21 <= 1); g_21 += 1)
            { 
                int8_t *l_45 = &g_46;
                int i, j;
                l_36 = (g_20 == g_2);
                (*l_35) = ((safe_rshift_func_int32_t_s_u((((safe_mul_func_int64_t_s_s((l_33[g_21] = (p_7 == (safe_lshift_func_int32_t_s_s(((safe_add_func_int16_t_s_s(g_21, (((*l_45) |= 0x5E) , l_36))) ^ p_7), l_36)))), 1)) ^ 0x8CDD50170A2C9EA2LL) , 1), g_20)) && 0);
            }
            if (g_20)
                goto lbl_47;
        }
        for (g_21 = (-27); (g_21 < 24); g_21 = safe_add_func_uint16_t_u_u(g_21, 9))
        { 
            if (p_7)
                goto lbl_47;
            return p_8;
        }
    }
    if (((**l_34) = (*l_35)))
    { 
        uint64_t *l_54[3];
        int32_t l_55[4][4] = {{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)},{(-1),(-1),(-1),(-1)}};
        int32_t l_58 = 0;
        int32_t *l_59 = &g_60;
        int i, j;
        for (i = 0; i < 3; i++)
            l_54[i] = (void*)0;
        (*l_35) = (**l_34);
        for (g_20 = 0; (g_20 >= 18); g_20 = safe_add_func_uint64_t_u_u(g_20, 7))
        { 
            (*p_8) = 0x3301997F;
        }
        (*l_59) ^= (safe_add_func_uint64_t_u_u(p_9, (((((((((l_55[1][3] = (*l_35)) , (((safe_rshift_func_int64_t_s_u((p_7 < p_9), 8)) | p_9) <= 0x2740D6A5A8E70C49LL)) <= l_58) && l_55[1][2]) > (**l_34)) , 0xF045C746EA38556ALL) & l_58) >= l_58) & (**l_34))));
    }
    else
    { 
        int32_t l_71 = 0x05D21490;
        uint32_t *l_72 = &g_21;
        uint32_t *l_73 = &g_74;
        int8_t *l_86 = &g_46;
        g_63 = l_61;
        if ((((&l_35 != g_65) , (safe_rshift_func_int8_t_s_s((((*l_73) = ((*l_72) |= (safe_mod_func_uint8_t_u_u((0U < p_7), l_71)))) == g_20), 4))) || 0x3186))
        { 
            int32_t *l_77[3][1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_77[i][j] = &g_2;
            }
            for (g_74 = (-8); (g_74 != 43); g_74 = safe_add_func_int16_t_s_s(g_74, 1))
            { 
                return l_77[2][0];
            }
        }
        else
        { 
            int32_t *l_84 = &g_20;
            int32_t l_85 = 0x155E3342;
            int32_t *l_87[3][4] = {{&l_85,&l_85,&l_85,&l_85},{&l_85,&l_85,&l_85,&l_85},{&l_85,&l_85,&l_85,&l_85}};
            int i, j;
            l_71 = (safe_sub_func_int64_t_s_s((safe_add_func_int16_t_s_s((safe_mul_func_int32_t_s_s(((*g_65) != (l_84 = (*l_34))), 1)), 1)), l_85));
            for (g_60 = 0; (g_60 <= 2); g_60 += 1)
            { 
                int i;
                (*p_8) |= l_33[g_60];
                (*p_8) = (l_86 == (*g_63));
            }
            l_87[1][2] = (void*)0;
        }
        for (p_9 = 0; (p_9 >= 35); p_9++)
        { 
            uint64_t l_90 = 18446744073709551613U;
            l_90++;
            if (g_46)
                goto lbl_93;
        }
    }
    return p_8;
}

static uint32_t  func_11(int32_t * const  p_12, uint32_t  p_13, int32_t * p_14, int64_t  p_15)
{ 
    int32_t *l_18 = (void*)0;
    int32_t *l_19[1][2];
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
            l_19[i][j] = &g_20;
    }
    ++g_21;
    g_20 = ((0x51F1E283 >= (safe_add_func_int64_t_s_s((safe_mod_func_int64_t_s_s(((((*p_12) || ((-1) & p_15)) == p_13) != p_15), g_20)), 0xFAF14186E1459F45LL))) <= g_20);
    return g_2;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_20, "g_20", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_101, "g_101", print_hash_value);
    transparent_crc(g_104, "g_104", print_hash_value);
    transparent_crc(g_113, "g_113", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
