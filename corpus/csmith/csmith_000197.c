/* Csmith 0cdc710315cf seed 197, flattened by scripts/csmith_flatten.py */
// Options:   --seed 197 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_add_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1>0) && (si2>0) && (si1 > (2147483647 -si2))) || ((si1<0) && (si2<0) && (si1 < ((-2147483647-1)-si2)))) ?
    ((si1)) :
    (si1 + si2);
}
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
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
static int8_t
safe_unary_minus_func_int8_t_s(int8_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 0;
static int16_t g_8 = 0x43C4;
static uint16_t g_11[2][2] = {{2U,2U},{2U,2U}};
static int32_t g_28 = 0xA4CCFD66;
static int32_t *g_27[1][3] = {{&g_28,&g_28,&g_28}};
static uint8_t g_45 = 0xEF;
static uint32_t g_58 = 1U;
static uint64_t g_61 = 0x459CADFD6F6066FALL;
static uint16_t g_62[4] = {65535U,65535U,65535U,65535U};
static int8_t g_63[4] = {(-1),(-1),(-1),(-1)};

static int64_t  func_1(void);
static int32_t  func_16(uint64_t  p_17, int32_t  p_18, uint8_t  p_19);
static uint64_t  func_20(int32_t * p_21, const int8_t  p_22, int32_t * p_23, uint64_t  p_24, int64_t  p_25);

static int64_t  func_1(void)
{ 
    uint32_t *l_74 = &g_58;
    int32_t l_79 = 0xCB35B2DF;
    int32_t **l_80 = &g_27[0][0];
    if (g_2)
    { 
        int32_t *l_3 = &g_2;
        int32_t *l_4 = (void*)0;
        int32_t *l_5 = &g_2;
        int32_t *l_6 = &g_2;
        int32_t *l_7 = (void*)0;
        int32_t *l_9 = &g_2;
        int32_t *l_10[2][3] = {{&g_2,&g_2,&g_2},{&g_2,&g_2,&g_2}};
        int i, j;
        ++g_11[1][1];
        for (g_2 = (-15); (g_2 != 24); ++g_2)
        { 
            int32_t *l_26 = &g_2;
            uint64_t *l_60 = &g_61;
            int32_t l_64 = 0;
            if (g_11[0][0])
                break;
            l_64 = (g_63[0] |= func_16(((*l_60) = (func_20((g_27[0][0] = l_26), (0x48112DEF906D8BC0LL ^ ((((g_2 || g_11[1][1]) && g_11[1][1]) > 255U) | 0x4868)), &g_2, (*l_26), (*l_26)) == 0x3DC28BF2FEA27655LL)), g_8, g_62[2]));
        }
        for (g_45 = 0; (g_45 > 43); g_45 = safe_add_func_uint32_t_u_u(g_45, 9))
        { 
            uint32_t *l_68 = (void*)0;
            uint32_t **l_67 = &l_68;
            uint32_t *l_69 = (void*)0;
            (*l_3) = (((*l_67) = &g_58) != l_69);
            for (g_8 = 3; (g_8 >= 0); g_8 -= 1)
            { 
                uint32_t l_70 = 0xBF663549;
                return l_70;
            }
            return g_61;
        }
    }
    else
    { 
        uint32_t * const l_73 = &g_58;
        const int32_t l_76 = 0xC99B0BF1;
        for (g_45 = (-5); (g_45 <= 8); g_45 = safe_add_func_uint32_t_u_u(g_45, 4))
        { 
            int32_t *l_75 = &g_28;
            (*l_75) |= (l_73 != l_74);
            if (l_76)
                break;
        }
    }
    for (g_61 = 0; (g_61 <= 48); ++g_61)
    { 
        l_79 = ((((l_79 & (((l_79 ^ g_11[1][1]) , (void*)0) != &g_61)) ^ 0xEF089E30) > 0x164A) > 4294967295U);
    }
    (*l_80) = &g_28;
    return (**l_80);
}

static int32_t  func_16(uint64_t  p_17, int32_t  p_18, uint8_t  p_19)
{ 
    return p_17;
}

static uint64_t  func_20(int32_t * p_21, const int8_t  p_22, int32_t * p_23, uint64_t  p_24, int64_t  p_25)
{ 
    int16_t l_31[4];
    int32_t l_59 = 0x2E18E6F9;
    int i;
    for (i = 0; i < 4; i++)
        l_31[i] = (-3);
    for (g_28 = 0; (g_28 > 5); g_28++)
    { 
        l_31[0] |= (*p_21);
        return p_22;
    }
    for (g_28 = (-26); (g_28 == (-14)); ++g_28)
    { 
        int32_t l_47 = (-1);
        for (p_24 = (-29); (p_24 <= 57); p_24 = safe_add_func_int16_t_s_s(p_24, 7))
        { 
            uint8_t *l_44 = &g_45;
            int32_t **l_46 = &g_27[0][0];
            uint32_t *l_57 = &g_58;
            (*l_46) = (g_2 , ((1U <= (+(safe_div_func_uint32_t_u_u(((((((*l_44) = (safe_add_func_int32_t_s_s((safe_add_func_int8_t_s_s((safe_unary_minus_func_int8_t_s(l_31[0])), g_2)), 0U))) , p_25) && p_22) , g_2) & g_28), 0xD8F3B1AD)))) , p_23));
            if (((void*)0 == p_23))
            { 
                return p_25;
            }
            else
            { 
                if (l_47)
                    break;
                if (l_47)
                    break;
                if (l_31[0])
                    continue;
            }
            l_59 = (safe_sub_func_uint8_t_u_u((((*l_57) &= (safe_unary_minus_func_int64_t_s(((safe_div_func_uint8_t_u_u((safe_lshift_func_uint8_t_u_u((safe_lshift_func_uint64_t_u_s(((**l_46) , p_22), 4)), p_25)), g_28)) , p_24)))) , g_58), 0x41));
        }
        return p_24;
    }
    return p_22;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_8, "g_8", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_11[i][j], "g_11[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_45, "g_45", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_62[i], "g_62[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_63[i], "g_63[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
