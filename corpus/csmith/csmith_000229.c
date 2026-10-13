/* Csmith 0cdc710315cf seed 229, flattened by scripts/csmith_flatten.py */
// Options:   --seed 229 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint32_t
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int32_t
safe_mod_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
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
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
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
static uint32_t
safe_rshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    (((unsigned int)right) >= 32) ?
    ((left)) :
    (left >> ((unsigned int)right));
}
static uint64_t
safe_rshift_func_uint64_t_u_s(uint64_t left, int right )
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
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
static uint16_t
safe_sub_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ui1 - ui2;
}
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

static const uint8_t g_8 = 0xE5;
static int32_t g_17 = 2;
static uint64_t g_24 = 0xF29C8DDAC0FAAB39LL;
static int32_t *g_35 = &g_17;
static int16_t g_46 = 0;
static uint64_t g_55[4] = {0U,0U,0U,0U};
static int8_t g_56[4][3] = {{0xE0,0xE0,0xE0},{0x89,(-1),0x89},{0xE0,0xE0,0xE0},{0x89,(-1),0x89}};
static int16_t g_62 = 0xCD51;
static uint32_t g_64 = 0xEDA29731;
static int16_t g_66 = 0x1D2D;
static int64_t g_68 = 1;
static uint64_t g_70 = 0x20850DA4A23C0F0ELL;
static uint64_t g_79 = 7U;
static int32_t g_86 = 2;
static int8_t g_93 = 0x1E;
static int16_t g_100[1] = {(-5)};
static uint16_t g_114 = 1U;

static int64_t  func_1(void);
static int32_t * func_2(int64_t  p_3, int64_t  p_4);
static const int32_t * func_12(int32_t * p_13, uint64_t  p_14);

static int64_t  func_1(void)
{ 
    uint64_t l_5[4][2] = {{0x443250E6DA5043C4LL,0x443250E6DA5043C4LL},{0x443250E6DA5043C4LL,0x443250E6DA5043C4LL},{0x443250E6DA5043C4LL,0x443250E6DA5043C4LL},{0x443250E6DA5043C4LL,0x443250E6DA5043C4LL}};
    int32_t **l_131 = &g_35;
    int i, j;
    (*l_131) = func_2(l_5[2][1], ((safe_rshift_func_uint32_t_u_u(g_8, (~l_5[2][1]))) | 4294967295U));
    (*l_131) = &g_86;
    return (**l_131);
}

static int32_t * func_2(int64_t  p_3, int64_t  p_4)
{ 
    for (p_3 = 0; (p_3 >= (-29)); p_3--)
    { 
        int32_t *l_16 = &g_17;
        int16_t l_18 = 7;
        const int32_t *l_125 = &g_86;
        const int32_t **l_124 = &l_125;
        (*l_124) = func_12(((+(p_3 <= 0xC1D6B3BF)) , l_16), l_18);
        for (g_46 = 0; (g_46 < 7); g_46 = safe_add_func_int16_t_s_s(g_46, 5))
        { 
            int32_t *l_130 = (void*)0;
            (*l_124) = (*l_124);
            (*l_16) = (safe_sub_func_int64_t_s_s(1, 1));
            return l_130;
        }
    }
    return &g_17;
}

static const int32_t * func_12(int32_t * p_13, uint64_t  p_14)
{ 
    int32_t **l_34[1][3];
    const int8_t l_44 = 1;
    int16_t l_117 = (-1);
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_34[i][j] = (void*)0;
    }
    for (p_14 = 0; (p_14 == 11); p_14 = safe_add_func_uint64_t_u_u(p_14, 5))
    { 
        int32_t l_29 = 0x2A284868;
        (*p_13) |= 0x30A4CCFD;
        for (g_17 = (-30); (g_17 <= 13); ++g_17)
        { 
            int32_t *l_23 = (void*)0;
            int64_t l_32 = 0x5B01D976D9935FF5LL;
            if (g_17)
                break;
            ++g_24;
            if (((g_8 >= l_29) ^ ((*p_13) == 2U)))
            { 
                return &g_17;
            }
            else
            { 
                int32_t *l_33 = &l_29;
                (*l_33) = (safe_lshift_func_int32_t_s_s((((g_17 , (l_32 = 3)) != (p_14 != p_14)) | g_17), 26));
                if (g_17)
                    continue;
                if ((*p_13))
                    break;
            }
        }
    }
    g_35 = p_13;
    for (g_17 = 0; (g_17 >= 25); g_17 = safe_add_func_uint32_t_u_u(g_17, 1))
    { 
        int16_t *l_45 = &g_46;
        const int32_t l_61 = 0x33FFCD55;
        int32_t l_69 = (-1);
        uint64_t l_74 = 0x1E5F5605658C465DLL;
        uint64_t *l_120 = (void*)0;
        uint64_t *l_121 = &g_79;
        if ((safe_sub_func_int16_t_s_s((((((0x12535B5B || g_24) == ((*l_45) = (safe_lshift_func_int32_t_s_u((((safe_lshift_func_int16_t_s_s(l_44, p_14)) <= 65535U) , 2), 2)))) | g_24) || 0x0CD5C0F4) ^ g_8), 0x5EFB)))
        { 
            uint8_t l_54 = 0xC2;
            int32_t l_65 = 0xE9D1BBA4;
            g_56[1][0] = (g_55[2] = (((safe_add_func_uint64_t_u_u((safe_mul_func_uint64_t_u_u((safe_mod_func_uint16_t_u_u(((!(l_54 ^ g_46)) , p_14), 0x6555)), p_14)), p_14)) , p_14) < 0x94C655EF));
            if (((safe_div_func_int64_t_s_s(g_24, g_56[1][0])) < (safe_div_func_uint8_t_u_u(((((-1) <= 0x9D081799B58ECE9ELL) != g_17) > 0x9E20), l_54))))
            { 
                uint32_t *l_63 = &g_64;
                g_62 = l_61;
                g_66 = (l_65 = (g_62 | (((*l_63) = g_56[1][0]) >= (((p_14 , 1U) != 0U) > (*p_13)))));
                l_65 ^= (*p_13);
            }
            else
            { 
                int32_t l_67 = 0x2A3E9F7A;
                l_69 &= (g_68 &= (g_46 && (l_67 && l_54)));
            }
            for (g_62 = 0; (g_62 <= 3); g_62 += 1)
            { 
                p_13 = p_13;
                g_70 = 4;
            }
        }
        else
        { 
            l_69 ^= (*p_13);
            if ((safe_unary_minus_func_int16_t_s((safe_sub_func_int16_t_s_s((((((((p_14 & l_74) < (((safe_mod_func_uint8_t_u_u(((safe_mod_func_uint16_t_u_u(g_56[1][0], g_46)) > (-9)), p_14)) > 0) > 1U)) & p_14) < g_17) , (void*)0) == (void*)0) || g_24), p_14)))))
            { 
                --g_79;
                p_13 = p_13;
            }
            else
            { 
                l_69 &= 9;
            }
            if ((safe_rshift_func_uint64_t_u_s((l_74 && 1), 62)))
            { 
                g_86 &= (safe_lshift_func_uint16_t_u_u(0x85FD, g_55[1]));
            }
            else
            { 
                return p_13;
            }
        }
        if ((safe_lshift_func_int16_t_s_u(((*l_45) = ((-9) || (p_14 , (safe_mod_func_int32_t_s_s(((safe_div_func_uint64_t_u_u(0xD72378B10DF2DAC4LL, l_74)) , 0xF418CC4B), 0x2127F58F))))), 12)))
        { 
            uint16_t l_94[1];
            int i;
            for (i = 0; i < 1; i++)
                l_94[i] = 0x6ABE;
            g_35 = &g_86;
            l_94[0]--;
        }
        else
        { 
            uint64_t l_99 = 1U;
            int32_t l_101 = (-7);
            for (g_70 = 0; (g_70 <= 3); g_70 += 1)
            { 
                uint64_t *l_104 = &l_99;
                uint16_t *l_113 = &g_114;
                int i;
                l_99 &= (safe_rshift_func_int64_t_s_s((-1), 26));
                l_101 = (g_100[0] = 0x54E5BA69);
                g_86 &= ((((safe_lshift_func_uint32_t_u_u((((((*l_104)++) < (g_55[g_70] ^ (safe_rshift_func_int16_t_s_s((safe_sub_func_uint16_t_u_u(((*l_113) &= ((((safe_sub_func_uint16_t_u_u(((p_13 = &g_17) != (void*)0), p_14)) ^ p_14) == p_14) <= g_24)), p_14)), 5)))) < g_55[g_70]) , g_56[3][0]), 21)) , 0x3E27) || 0x93BA) <= p_14);
            }
        }
        g_86 = (safe_mul_func_uint16_t_u_u((l_117 , (safe_lshift_func_int16_t_s_s((((*l_121)++) | g_55[2]), 6))), g_24));
    }
    return p_13;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_55[i], "g_55[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            transparent_crc(g_56[i][j], "g_56[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_62, "g_62", print_hash_value);
    transparent_crc(g_64, "g_64", print_hash_value);
    transparent_crc(g_66, "g_66", print_hash_value);
    transparent_crc(g_68, "g_68", print_hash_value);
    transparent_crc(g_70, "g_70", print_hash_value);
    transparent_crc(g_79, "g_79", print_hash_value);
    transparent_crc(g_86, "g_86", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        transparent_crc(g_100[i], "g_100[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_114, "g_114", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
