/* Csmith 0cdc710315cf seed 230, flattened by scripts/csmith_flatten.py */
// Options:   --seed 230 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
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
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 0x7E2799AC;
static int32_t g_7 = 0;
static int64_t g_17[3] = {0,0,0};
static int8_t g_18 = 0x72;
static uint32_t g_37[1][1] = {{0x5701832D}};
static uint16_t g_49 = 65535U;
static uint64_t g_55 = 18446744073709551613U;
static int64_t g_58 = 0xF961DFA23986B299LL;
static int32_t **g_62 = (void*)0;
static uint8_t g_64 = 1U;
static int32_t *g_70 = &g_7;
static const int32_t *g_74 = &g_7;
static uint64_t g_93 = 0x4BE07DECC76B58F1LL;
static int8_t g_101[1][2] = {{0xA0,0xA0}};
static int32_t g_102 = 0x53341C0C;
static int64_t g_103 = 0xF057967921D28FF5LL;
static uint64_t g_104 = 0xE7FEFA8C4D3BBE63LL;
static int64_t g_128 = 0x8970CE46C46319CDLL;

static int8_t  func_1(void);
static const int32_t  func_11(int8_t  p_12);
static int32_t  func_22(uint8_t  p_23);

static int8_t  func_1(void)
{ 
    uint16_t l_8 = 1U;
    int32_t l_96 = (-1);
    int32_t l_99 = 0;
    int32_t l_100 = 0x8AE43225;
    for (g_2 = 22; (g_2 == 11); --g_2)
    { 
        int32_t *l_5 = (void*)0;
        int32_t *l_6 = &g_7;
        int32_t l_47 = 0x9AD537E8;
        uint64_t *l_54 = &g_55;
        int32_t *l_63 = (void*)0;
        uint64_t *l_78 = &g_55;
        if (((*l_6) ^= g_2))
        { 
            --l_8;
            if (g_2)
                continue;
        }
        else
        { 
            if (func_11(g_2))
            { 
                g_7 &= 0;
            }
            else
            { 
                int32_t *l_45 = &g_7;
                int32_t *l_46 = &g_7;
                int32_t *l_48[1][4] = {{&l_47,&l_47,&l_47,&l_47}};
                int32_t **l_52 = &l_6;
                int i, j;
                --g_49;
                (*l_52) = &g_7;
            }
            for (g_7 = 0; (g_7 <= 2); g_7 += 1)
            { 
                int i;
                return g_17[g_7];
            }
        }
        g_64 |= (+((((g_58 = ((*l_54)--)) || (safe_add_func_uint64_t_u_u((g_2 , (((((~((void*)0 != g_62)) ^ l_8) > (-1)) | 0xE206C165) == l_8)), l_8))) <= g_49) > (*l_6)));
    }
    for (l_8 = (-22); (l_8 >= 42); ++l_8)
    { 
        uint16_t l_84 = 0x9120;
        int32_t *l_86 = &g_7;
        int32_t l_87 = 6;
        int32_t l_92 = 0;
        for (g_49 = 0; (g_49 < 56); g_49++)
        { 
            return g_2;
        }
        if (l_84)
        { 
            int32_t *l_85 = &g_7;
            int32_t *l_88 = &g_7;
            int32_t *l_89 = &g_7;
            int32_t *l_90 = &g_7;
            int32_t *l_91[1][1];
            int i, j;
            for (i = 0; i < 1; i++)
            {
                for (j = 0; j < 1; j++)
                    l_91[i][j] = &g_7;
            }
            l_86 = l_85;
            ++g_93;
            if ((*g_70))
            { 
                (*l_90) = (*g_70);
                if ((*g_74))
                    break;
            }
            else
            { 
                int64_t l_97 = 1;
                int32_t l_98 = 0x52146605;
                g_104--;
            }
        }
        else
        { 
            int32_t **l_118 = (void*)0;
            int32_t **l_119 = &l_86;
            uint8_t *l_127[1];
            uint16_t l_129 = 0U;
            uint8_t l_130 = 0x6D;
            int i;
            for (i = 0; i < 1; i++)
                l_127[i] = (void*)0;
            for (g_55 = 0; (g_55 <= 0); g_55 += 1)
            { 
                uint16_t *l_115 = &g_49;
                int32_t **l_116 = (void*)0;
                int32_t **l_117 = &l_86;
                int i, j;
                (*l_117) = (((*g_70) = ((safe_add_func_int64_t_s_s(g_37[g_55][g_55], (safe_add_func_int32_t_s_s(g_101[g_55][g_55], (safe_lshift_func_uint16_t_u_s(((*l_115) |= (safe_mul_func_uint16_t_u_u(0x50BF, (-1)))), 9)))))) || g_101[g_55][(g_55 + 1)])) , (void*)0);
            }
            (*l_119) = (void*)0;
            l_92 &= (((safe_mod_func_int64_t_s_s((((safe_mul_func_int8_t_s_s((safe_add_func_uint8_t_u_u(((void*)0 != &g_2), (!((g_128 &= (g_64 &= (((*g_70) ^= (((void*)0 != &g_49) ^ 1U)) || g_58))) | g_49)))), g_37[0][0])) && l_129) != l_99), l_8)) <= l_130) , l_99);
        }
        if ((*g_74))
            continue;
    }
    return l_8;
}

static const int32_t  func_11(int8_t  p_12)
{ 
    int32_t *l_13 = &g_7;
    int32_t *l_14 = &g_7;
    int32_t *l_15 = &g_7;
    int32_t *l_16[3][2];
    uint64_t l_19 = 18446744073709551614U;
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 2; j++)
            l_16[i][j] = &g_7;
    }
    l_19++;
    (*l_13) |= func_22(p_12);
    return g_37[0][0];
}

static int32_t  func_22(uint8_t  p_23)
{ 
    int32_t l_24[1];
    int32_t *l_25 = &l_24[0];
    int32_t *l_26 = (void*)0;
    int32_t *l_27 = &l_24[0];
    int32_t *l_28 = (void*)0;
    int32_t *l_29 = &g_7;
    int32_t *l_30 = &g_7;
    int32_t *l_31 = &l_24[0];
    int32_t *l_32 = (void*)0;
    int32_t *l_33 = (void*)0;
    int32_t *l_34 = &l_24[0];
    int32_t *l_35 = &l_24[0];
    int32_t *l_36[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
    uint8_t l_44 = 254U;
    int i;
    for (i = 0; i < 1; i++)
        l_24[i] = 0xC1B0FE2C;
    ++g_37[0][0];
    (*l_34) ^= (((safe_div_func_int8_t_s_s(((safe_mul_func_int64_t_s_s((g_17[2] , (&l_24[0] == &g_7)), g_2)) <= 255U), l_44)) ^ p_23) & g_37[0][0]);
    return p_23;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_7, "g_7", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_17[i], "g_17[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_18, "g_18", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_37[i][j], "g_37[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_49, "g_49", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_101[i][j], "g_101[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_102, "g_102", print_hash_value);
    transparent_crc(g_103, "g_103", print_hash_value);
    transparent_crc(g_104, "g_104", print_hash_value);
    transparent_crc(g_128, "g_128", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
