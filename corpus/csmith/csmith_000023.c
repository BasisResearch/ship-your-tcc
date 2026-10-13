/* Csmith 0cdc710315cf seed 23, flattened by scripts/csmith_flatten.py */
// Options:   --seed 23 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint8_t
safe_add_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 + ui2;
}
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
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
safe_lshift_func_int16_t_s_u(int16_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (32767 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
safe_lshift_func_uint32_t_u_u(uint32_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (4294967295U >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint64_t
safe_lshift_func_uint64_t_u_s(uint64_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (18446744073709551615UL >> ((int)right)))) ?
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
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int32_t
safe_rshift_func_int32_t_s_s(int32_t left, int right )
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
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 5;
static int32_t g_12[3] = {(-1),(-1),(-1)};
static int64_t g_24 = (-1);
static int32_t g_40 = 0x85B3D925;
static int32_t *g_39 = &g_40;
static uint64_t g_65 = 0xABF6A420A61B3E23LL;
static int32_t g_74 = 1;
static uint64_t g_76 = 0x2FB512259947D9BALL;
static int32_t g_87 = 1;
static uint32_t g_89[1][4] = {{1U,1U,1U,1U}};
static uint16_t g_95 = 0U;
static int32_t g_126 = 0;
static const int32_t g_127 = 0x5ECB2B9D;
static int32_t g_130 = 5;
static uint8_t g_143 = 0x87;
static uint8_t g_150 = 0xEB;
static int8_t g_157 = 0xA0;
static int8_t g_159 = 0x85;
static uint8_t g_165 = 0xED;

static uint8_t  func_1(void);
static int64_t  func_19(int64_t  p_20, const int8_t  p_21);
static const uint16_t  func_32(const uint16_t  p_33, int8_t  p_34, int16_t  p_35);

static uint8_t  func_1(void)
{ 
    uint16_t l_7[3];
    int32_t l_133 = 1;
    uint64_t l_148 = 18446744073709551611U;
    int32_t **l_154 = &g_39;
    int i;
    for (i = 0; i < 3; i++)
        l_7[i] = 2U;
    for (g_2 = 0; (g_2 <= (-15)); g_2 = safe_sub_func_int16_t_s_s(g_2, 1))
    { 
        uint16_t l_144 = 1U;
        uint16_t l_147 = 0U;
        int32_t l_171 = 0;
    }
    return g_65;
}

static int64_t  func_19(int64_t  p_20, const int8_t  p_21)
{ 
    int64_t l_75 = 0xCACF6402D6FA87DCLL;
    int32_t l_80 = 1;
    int32_t l_83 = 0;
    int32_t l_84 = 0xA2DBBA47;
    int32_t l_88[2][1];
    int32_t **l_115 = &g_39;
    int64_t *l_125 = &l_75;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_88[i][j] = 0xEF461E62;
    }
    for (g_24 = 18; (g_24 <= 4); g_24--)
    { 
        uint64_t l_37 = 8U;
        int32_t * const l_68 = (void*)0;
        int32_t l_81 = 0xCD0122BF;
        int32_t l_85 = 8;
        int32_t l_86 = 0;
        int32_t *l_106 = &g_40;
        if ((safe_add_func_uint8_t_u_u(p_21, (safe_lshift_func_uint64_t_u_s((func_32(((((g_24 , ((+l_37) | 1U)) != p_21) >= g_12[1]) != 0x07865E1BFEDAACDALL), l_37, g_2) == 6U), p_21)))))
        { 
            int32_t **l_69[2];
            int32_t **l_70 = (void*)0;
            int32_t **l_71 = &g_39;
            int i;
            for (i = 0; i < 2; i++)
                l_69[i] = (void*)0;
            (*l_71) = l_68;
            for (p_20 = 0; (p_20 > (-19)); p_20--)
            { 
                return p_21;
            }
            ++g_76;
        }
        else
        { 
            int32_t *l_79[2];
            int8_t l_82 = 1;
            uint16_t *l_94 = &g_95;
            int i;
            for (i = 0; i < 2; i++)
                l_79[i] = &g_74;
            --g_89[0][3];
            g_40 &= (l_88[1][0] || ((safe_mul_func_uint16_t_u_u((--(*l_94)), (safe_rshift_func_uint32_t_u_u((safe_lshift_func_int16_t_s_u((safe_lshift_func_int8_t_s_s((safe_div_func_uint8_t_u_u((p_20 , (p_21 != g_87)), g_89[0][3])), 0)), g_89[0][3])), 5)))) || (*g_39)));
            for (l_81 = 1; (l_81 >= 0); l_81 -= 1)
            { 
                int i;
                if (p_21)
                    break;
            }
        }
        (*l_106) = l_83;
    }
    if ((safe_sub_func_uint8_t_u_u((safe_sub_func_uint64_t_u_u((safe_rshift_func_int32_t_s_s(((safe_div_func_int32_t_s_s((l_115 != ((safe_lshift_func_int8_t_s_u(g_65, (safe_lshift_func_uint32_t_u_u(((+(safe_lshift_func_uint32_t_u_u(((((safe_lshift_func_uint8_t_u_u((((*l_125) ^= ((g_40 , &g_39) == l_115)) && g_12[2]), 5)) >= p_21) , g_74) && p_20), 16))) >= g_65), g_2)))) , l_115)), 0xAF60329E)) >= 0x20), 27)), g_89[0][3])), g_126)))
    { 
        return g_89[0][1];
    }
    else
    { 
        for (l_84 = 0; (l_84 <= 0); l_84 += 1)
        { 
            for (l_83 = 0; (l_83 <= 0); l_83 += 1)
            { 
                int i, j;
                (*l_115) = &l_88[l_83][l_84];
                if (l_88[(l_83 + 1)][l_83])
                    continue;
            }
            for (l_83 = 0; (l_83 <= 0); l_83 += 1)
            { 
                int i, j;
                if (p_21)
                    break;
                l_88[l_83][l_83] &= p_20;
            }
        }
    }
    (*l_115) = (*l_115);
    return g_24;
}

static const uint16_t  func_32(const uint16_t  p_33, int8_t  p_34, int16_t  p_35)
{ 
    int32_t *l_38 = &g_12[1];
    int32_t l_47[1];
    uint32_t l_48 = 0x29BAD2CE;
    int i;
    for (i = 0; i < 1; i++)
        l_47[i] = (-8);
lbl_46:
    g_39 = l_38;
    for (p_35 = (-10); (p_35 != (-10)); p_35 = safe_add_func_uint16_t_u_u(p_35, 4))
    { 
        const int64_t *l_43 = &g_24;
        int32_t l_55 = 0x6E19CCD2;
        int32_t l_58 = 0x86B464BB;
        int32_t l_59 = 0x87147D86;
        int32_t l_63 = 4;
        int32_t l_64 = 0x21916DC9;
        if (((void*)0 != l_43))
        { 
            if ((*g_39))
                break;
            for (p_34 = 29; (p_34 >= (-19)); p_34 = safe_sub_func_int32_t_s_s(p_34, 9))
            { 
                if ((*g_39))
                    break;
            }
            if (p_35)
                goto lbl_46;
        }
        else
        { 
            for (p_34 = 0; (p_34 >= 0); p_34 -= 1)
            { 
                int i;
                if (l_47[p_34])
                    break;
            }
            if (l_48)
                continue;
        }
        for (p_34 = (-10); (p_34 > 2); ++p_34)
        { 
            uint32_t l_51[4] = {0U,0U,0U,0U};
            int32_t *l_53 = &g_40;
            int32_t *l_56 = (void*)0;
            int32_t *l_57 = (void*)0;
            int32_t *l_60 = (void*)0;
            int32_t *l_61 = &l_59;
            int32_t *l_62[2];
            int i;
            for (i = 0; i < 2; i++)
                l_62[i] = (void*)0;
            if (l_48)
                goto lbl_46;
            if ((0x32CA >= l_51[2]))
            { 
                int32_t **l_52[1][4] = {{&g_39,&g_39,&g_39,&g_39}};
                int i, j;
                l_53 = &g_2;
                g_40 ^= (g_12[1] != ((((-1) <= ((+(0U > p_33)) > (-1))) < 0x91) , (*g_39)));
            }
            else
            { 
                return p_35;
            }
            g_65--;
        }
    }
    return (*l_38);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_12[i], "g_12[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_40, "g_40", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_76, "g_76", print_hash_value);
    transparent_crc(g_87, "g_87", print_hash_value);
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_89[i][j], "g_89[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_95, "g_95", print_hash_value);
    transparent_crc(g_126, "g_126", print_hash_value);
    transparent_crc(g_127, "g_127", print_hash_value);
    transparent_crc(g_130, "g_130", print_hash_value);
    transparent_crc(g_143, "g_143", print_hash_value);
    transparent_crc(g_150, "g_150", print_hash_value);
    transparent_crc(g_157, "g_157", print_hash_value);
    transparent_crc(g_159, "g_159", print_hash_value);
    transparent_crc(g_165, "g_165", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
