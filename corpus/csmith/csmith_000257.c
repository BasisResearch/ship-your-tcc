/* Csmith 0cdc710315cf seed 257, flattened by scripts/csmith_flatten.py */
// Options:   --seed 257 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint64_t
safe_add_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
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
static int8_t
safe_lshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (127 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint8_t
safe_lshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (255 >> ((int)right)))) ?
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
static int32_t
safe_sub_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~2147483647)))-si2)^si2)) < 0) ?
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

static int16_t g_10 = 0;
static int32_t g_23 = 1;
static uint64_t g_26 = 0xFECBE92E3F9150FALL;
static const int32_t *g_42 = &g_23;
static const int32_t **g_41 = &g_42;
static const uint64_t g_47 = 0x502870B1B9B96406LL;
static const uint64_t *g_46 = &g_47;
static const uint64_t **g_45 = &g_46;
static const uint64_t **g_48 = (void*)0;
static int64_t g_53 = (-1);
static uint32_t g_60[4] = {4294967288U,4294967288U,4294967288U,4294967288U};
static uint8_t g_61 = 1U;
static int16_t g_62[3][4] = {{0xA337,0xA337,0xA337,0xA337},{0xA337,0xA337,0xA337,0xA337},{0xA337,0xA337,0xA337,0xA337}};
static uint16_t g_66 = 0xEA5C;
static uint64_t *g_93 = &g_26;
static uint8_t g_98 = 255U;

static uint16_t  func_1(void);
static int32_t  func_5(int64_t  p_6, const int8_t  p_7, int64_t  p_8, uint16_t  p_9);
static int8_t  func_11(int64_t  p_12, const int64_t  p_13, int32_t  p_14);

static uint16_t  func_1(void)
{ 
    uint16_t l_2 = 0x3D06;
    uint32_t l_21 = 0U;
    int32_t *l_22[2][2];
    int8_t l_24 = 0xA6;
    uint64_t *l_25 = &g_26;
    const int32_t l_101 = 0x3271FE85;
    uint8_t l_102[2];
    uint32_t l_103 = 0x49FB5A3A;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
            l_22[i][j] = &g_23;
    }
    for (i = 0; i < 2; i++)
        l_102[i] = 247U;
    l_2++;
    g_98 &= func_5(g_10, ((g_66 &= ((func_11((((*l_25) = (safe_div_func_int32_t_s_s(((g_23 = (safe_mul_func_uint64_t_u_u(((safe_lshift_func_uint16_t_u_s(((g_10 & (0 != l_2)) , l_2), l_2)) , g_10), l_21))) == l_24), g_10))) , g_23), g_10, g_10) , (*g_46)) , 0x5DE17A8F3E709F62LL)) || (*g_46)), g_10, g_47);
    l_102[0] = (safe_add_func_uint16_t_u_u(((((((g_60[0] | 0) != g_26) , &g_62[0][3]) == &g_62[2][3]) <= g_47) > g_23), l_101));
    return l_103;
}

static int32_t  func_5(int64_t  p_6, const int8_t  p_7, int64_t  p_8, uint16_t  p_9)
{ 
    uint64_t l_76 = 0x57ECD02AC2FF03E5LL;
    const int32_t * const l_96 = &g_23;
    int32_t l_97 = 0x070DDB65;
    for (g_66 = 0; (g_66 <= 3); g_66 += 1)
    { 
        uint16_t l_75 = 0x884F;
        int32_t *l_77[2];
        uint64_t *l_83[2][4] = {{&g_26,&g_26,&g_26,&g_26},{&g_26,&g_26,&g_26,&g_26}};
        uint64_t **l_82 = &l_83[1][2];
        int16_t *l_84 = &g_62[2][2];
        int i, j;
        for (i = 0; i < 2; i++)
            l_77[i] = (void*)0;
        g_23 = (safe_add_func_uint64_t_u_u(((safe_mul_func_int64_t_s_s((safe_div_func_uint8_t_u_u((((safe_sub_func_int32_t_s_s(l_75, g_23)) , l_76) <= p_7), p_7)), 0x1429742485FC0193LL)) != 251U), 1));
        if ((**g_41))
            continue;
        if ((~((&l_76 != (void*)0) | (~(((*l_84) = (safe_lshift_func_int8_t_s_s((((*l_82) = &g_26) == &l_76), g_53))) != l_76)))))
        { 
            int64_t l_87 = 0;
            int32_t *l_88 = &g_23;
            int32_t **l_89 = &l_88;
            l_87 ^= ((safe_mod_func_int64_t_s_s(0x77089988BF30E8C0LL, l_76)) < ((((g_23 = l_76) | p_7) , g_53) , g_60[2]));
            if (p_9)
                break;
            (*g_41) = ((*l_89) = l_88);
        }
        else
        { 
            for (g_53 = 0; (g_53 <= 3); g_53 += 1)
            { 
                uint64_t *l_92 = &l_76;
                uint64_t *l_94 = &l_76;
                int32_t **l_95 = &l_77[0];
                (*g_41) = (void*)0;
                (*g_41) = ((*l_95) = ((((safe_sub_func_uint8_t_u_u(((l_92 != (l_94 = (g_93 = &l_76))) || (g_10 | l_76)), g_26)) > 0x83FDF714E35B4FB6LL) & 4294967293U) , &g_23));
            }
            (*g_41) = (*g_41);
        }
        for (l_75 = 0; (l_75 <= 3); l_75 += 1)
        { 
            g_23 ^= ((*g_41) == (p_9 , l_96));
            g_23 ^= (l_97 = (p_8 , p_9));
            (*g_41) = (*g_41);
        }
    }
    (*g_41) = &l_97;
    for (g_10 = 0; g_10 < 4; g_10 = g_10 + 1)
    {
        g_60[g_10] = 0U;
    }
    return (*l_96);
}

static int8_t  func_11(int64_t  p_12, const int64_t  p_13, int32_t  p_14)
{ 
    uint16_t l_27[1];
    int32_t * const l_36 = &g_23;
    int8_t l_37 = 0x24;
    int32_t *l_40 = &g_23;
    int64_t l_54 = (-2);
    int32_t l_55[4];
    uint64_t *l_65 = (void*)0;
    int i;
    for (i = 0; i < 1; i++)
        l_27[i] = 0x1BB0;
    for (i = 0; i < 4; i++)
        l_55[i] = 0xFF2B9ACC;
    for (p_12 = 0; (p_12 >= 0); p_12 -= 1)
    { 
        int32_t *l_28 = &g_23;
        int32_t *l_29 = (void*)0;
        int32_t *l_30 = &g_23;
        uint64_t l_31 = 0x254981C5860BC23CLL;
        ++l_31;
    }
    if (((safe_mul_func_uint32_t_u_u((((&p_14 == l_36) != (g_10 < 0xBB323FA9)) , p_13), l_37)) >= g_23))
    { 
        int32_t *l_38[2][4] = {{(void*)0,&g_23,(void*)0,(void*)0},{&g_23,&g_23,&g_23,&g_23}};
        int32_t **l_39[2][2] = {{&l_38[1][1],&l_38[1][1]},{&l_38[1][1],&l_38[1][1]}};
        const uint64_t *l_44 = &g_26;
        const uint64_t **l_43 = &l_44;
        int i, j;
        l_40 = l_38[1][1];
        if (((void*)0 != g_41))
        { 
            int64_t *l_51 = (void*)0;
            int64_t *l_52 = &g_53;
            g_48 = (g_45 = l_43);
            l_55[0] = ((*l_36) = (safe_lshift_func_uint8_t_u_s((((((p_12 > (((*l_52) = 0x069719EFCC72FDDDLL) , (l_54 &= ((((&g_53 == &p_13) >= g_53) | 65535U) , 1)))) || 0xD8D4885C3B36352BLL) == p_12) ^ (-1)) , 3U), 6)));
        }
        else
        { 
            uint32_t l_56 = 0xE7D51263;
            int64_t l_59 = 7;
            if ((**g_41))
            { 
                l_56++;
                return l_59;
            }
            else
            { 
                g_61 = (g_60[2] = ((*l_36) &= (l_56 <= 1U)));
                g_62[0][3] = (g_23 &= 0xFD8DD93F);
                return g_47;
            }
        }
    }
    else
    { 
        (*l_40) = ((((p_14 && (((((g_10 | ((~(!(l_65 == (void*)0))) , 0x0CFC)) | p_12) & (**g_45)) || p_13) < g_47)) , g_53) == (*l_40)) && p_12);
        (*g_41) = (*g_41);
        (*g_41) = (*g_41);
    }
    return g_60[2];
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_47, "g_47", print_hash_value);
    transparent_crc(g_53, "g_53", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_60[i], "g_60[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_61, "g_61", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_62[i][j], "g_62[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_66, "g_66", print_hash_value);
    transparent_crc(g_98, "g_98", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
