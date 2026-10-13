/* Csmith 0cdc710315cf seed 70, flattened by scripts/csmith_flatten.py */
// Options:   --seed 70 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static int64_t
safe_lshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (9223372036854775807L >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int64_t
safe_mod_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
}
static uint64_t
safe_mod_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
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
static int64_t
safe_rshift_func_int64_t_s_u(int64_t left, unsigned int right )
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
safe_rshift_func_uint64_t_u_u(uint64_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = (-7);
static int16_t g_23 = (-1);
static uint64_t g_44 = 0x7F77771FDA6603FALL;
static uint8_t g_48 = 0x0E;
static int32_t g_60 = 0;
static int16_t g_62 = 0;
static int8_t g_70 = (-1);
static int32_t g_80 = (-1);
static int8_t *g_92 = (void*)0;
static int8_t * const *g_91 = &g_92;

static int32_t  func_1(void);
static int16_t  func_10(int32_t * p_11, int32_t * const  p_12, int16_t  p_13, uint16_t  p_14, uint64_t  p_15);
static int32_t * func_16(int8_t  p_17, uint16_t  p_18, int32_t * p_19);

static int32_t  func_1(void)
{ 
    int64_t l_2[3][4] = {{0x555B7CEFA9D19AC1LL,0x9AC7894801897A4ALL,0x555B7CEFA9D19AC1LL,0x555B7CEFA9D19AC1LL},{0x9AC7894801897A4ALL,0x9AC7894801897A4ALL,0,0x9AC7894801897A4ALL},{0x9AC7894801897A4ALL,0x555B7CEFA9D19AC1LL,0x555B7CEFA9D19AC1LL,0x9AC7894801897A4ALL}};
    int32_t * const l_72 = &g_3;
    int i, j;
    for (g_3 = 2; (g_3 >= 0); g_3 -= 1)
    { 
        uint64_t l_4[4];
        int32_t * const l_9 = &g_3;
        int16_t *l_22 = &g_23;
        uint64_t l_73 = 18446744073709551613U;
        int64_t l_88 = 0x85EBC95BC5B3697ELL;
        int32_t *l_90 = &g_3;
        int32_t **l_89 = &l_90;
        int i;
        for (i = 0; i < 4; i++)
            l_4[i] = 18446744073709551615U;
        if (l_4[3])
            break;
        (*l_89) = ((safe_lshift_func_int64_t_s_s((safe_sub_func_uint32_t_u_u(((void*)0 == l_9), (func_10(func_16((((*l_22) = ((safe_div_func_int16_t_s_s(0x4ADC, (*l_9))) != 18446744073709551614U)) , (*l_9)), (*l_9), &g_3), l_72, l_73, (*l_9), g_60) , l_88))), 16)) , l_72);
        g_91 = g_91;
        for (g_60 = 0; (g_60 <= 2); g_60 += 1)
        { 
            uint64_t *l_102 = &l_4[1];
            uint64_t **l_101 = &l_102;
            int32_t l_109 = 0x97626F05;
            int i, j;
            if ((safe_sub_func_uint8_t_u_u((safe_sub_func_int64_t_s_s(0x760DC054819E5EE4LL, (safe_mul_func_int32_t_s_s(((safe_rshift_func_int32_t_s_u((((((*l_101) = &g_44) != (((*l_22) = (l_2[g_60][g_3] , l_2[g_60][g_3])) , &g_44)) != 0x96B8) <= 1), (*l_9))) ^ l_2[g_60][g_3]), g_60)))), 0U)))
            { 
                (*l_89) = &g_60;
                if (g_23)
                    break;
                if (g_23)
                    continue;
            }
            else
            { 
                int32_t l_110[2];
                int i, j;
                for (i = 0; i < 2; i++)
                    l_110[i] = 0xE7FEFA8C;
                l_110[1] = (((safe_lshift_func_uint32_t_u_u((safe_mul_func_int8_t_s_s((safe_rshift_func_uint16_t_u_u(((l_2[g_60][(g_3 + 1)] = g_48) | g_44), l_109)), (*l_72))), 15)) != (*l_9)) | 0xE2);
                (*l_89) = &l_110[0];
                return l_110[0];
            }
        }
    }
    return g_44;
}

static int16_t  func_10(int32_t * p_11, int32_t * const  p_12, int16_t  p_13, uint16_t  p_14, uint64_t  p_15)
{ 
    int8_t l_78[4] = {0x77,0x77,0x77,0x77};
    int32_t l_87 = 0x2FEC8B64;
    int i;
    if ((g_60 > (safe_rshift_func_uint64_t_u_u((g_44 = (safe_mul_func_uint64_t_u_u(g_62, l_78[1]))), g_60))))
    { 
        int32_t *l_79 = &g_80;
        (*l_79) |= g_60;
    }
    else
    { 
        int32_t *l_81 = (void*)0;
        int32_t *l_82 = &g_60;
        int8_t * const l_86 = &g_70;
        int8_t * const *l_85 = &l_86;
        (*l_82) = l_78[1];
        for (g_62 = (-1); (g_62 < (-5)); --g_62)
        { 
            for (g_60 = 3; (g_60 >= 0); g_60 -= 1)
            { 
                int i;
                if (l_78[g_60])
                    break;
                l_85 = l_85;
                l_87 = ((void*)0 == &p_15);
            }
            return p_13;
        }
    }
    return p_13;
}

static int32_t * func_16(int8_t  p_17, uint16_t  p_18, int32_t * p_19)
{ 
    uint16_t l_24[1][4];
    int32_t l_45[1];
    int8_t l_61 = 0x0A;
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
            l_24[i][j] = 65535U;
    }
    for (i = 0; i < 1; i++)
        l_45[i] = 3;
    for (p_17 = 0; (p_17 >= 0); p_17 -= 1)
    { 
        int16_t *l_35 = &g_23;
        int32_t l_36 = 0xA426232C;
        int32_t l_57[2];
        uint32_t l_63 = 0x334807DA;
        int8_t *l_68 = &l_61;
        int8_t *l_69 = &g_70;
        int32_t *l_71 = &l_45[0];
        int i;
        for (i = 0; i < 2; i++)
            l_57[i] = 1;
        if (((0x66EFE0CD1B7E280BLL < ((safe_mod_func_uint64_t_u_u((safe_rshift_func_int64_t_s_u(((safe_mod_func_int64_t_s_s((safe_sub_func_uint8_t_u_u((((((safe_add_func_uint8_t_u_u((&g_3 == &g_3), p_17)) , (void*)0) == l_35) && g_3) == 0xA1BC), l_36)), g_23)) , 1), 12)), 0x701832D748AEE511LL)) && 0)) & g_23))
        { 
            uint64_t *l_42 = (void*)0;
            uint64_t *l_43 = &g_44;
            int32_t l_56 = 0x06C165AA;
            if ((p_17 <= (l_45[0] = (!(((safe_mul_func_uint8_t_u_u(((((*l_43) = ((safe_sub_func_uint32_t_u_u(0x9188B9AB, p_18)) || 4U)) < l_36) || g_23), 0)) <= p_18) == l_24[0][1])))))
            { 
                int32_t *l_46 = &l_36;
                int32_t *l_47[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_47[i] = &l_45[0];
                --g_48;
            }
            else
            { 
                int32_t *l_51 = &l_45[0];
                if (g_3)
                    break;
                return p_19;
            }
            if (g_23)
                break;
            for (p_18 = 0; (p_18 <= 0); p_18 += 1)
            { 
                int i, j;
                l_56 = (safe_rshift_func_int32_t_s_u((l_24[p_17][p_18] | ((((0x77F1 && (safe_div_func_uint32_t_u_u((0xD714E288 && g_23), 2U))) < l_24[0][2]) >= 0x76C1) | p_17)), p_17));
            }
        }
        else
        { 
            int32_t *l_58 = &l_36;
            int32_t *l_59[2];
            int32_t **l_66 = (void*)0;
            int32_t **l_67 = &l_59[0];
            int i;
            for (i = 0; i < 2; i++)
                l_59[i] = &l_45[0];
            --l_63;
            for (p_18 = 0; (p_18 <= 0); p_18 += 1)
            { 
                return &g_60;
            }
            (*l_67) = (void*)0;
        }
        (*l_71) &= (g_44 && ((*l_69) &= ((*l_68) ^= g_44)));
        for (g_48 = 0; (g_48 <= 0); g_48 += 1)
        { 
            (*l_71) &= (*p_19);
        }
    }
    return p_19;
}

int main (void)
{
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    transparent_crc(g_48, "g_48", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_80, "g_80", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
