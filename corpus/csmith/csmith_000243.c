/* Csmith 0cdc710315cf seed 243, flattened by scripts/csmith_flatten.py */
// Options:   --seed 243 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static uint16_t
safe_div_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 / ui2);
}
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
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
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint8_t
safe_mod_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return
    (ui2 == 0) ?
    ((ui1)) :
    (ui1 % ui2);
}
static int64_t
safe_mul_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (9223372036854775807L / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-9223372036854775807L-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-9223372036854775807L-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (9223372036854775807L / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static int16_t
safe_rshift_func_int16_t_s_s(int16_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static int8_t
safe_rshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32)) ?
    ((left)) :
    (left >> ((unsigned int)right));
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
static uint32_t
safe_sub_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 - ui2;
}
static uint8_t
safe_unary_minus_func_uint8_t_u(uint8_t ui )
{
  return -ui;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_2 = 0x99FF6065;
static int32_t g_6 = 0x485BF25C;
static int8_t g_21 = 0x16;
static uint16_t g_28 = 0xD88C;
static int16_t g_39 = (-10);
static int16_t g_41 = 2;
static int16_t *g_40 = &g_41;
static int32_t g_46 = 0x85C68AB8;
static int64_t g_55[4] = {0xA16D6AE4D7D9C74ELL,0xA16D6AE4D7D9C74ELL,0xA16D6AE4D7D9C74ELL,0xA16D6AE4D7D9C74ELL};
static int32_t g_58 = 0xDBE6A916;
static uint16_t g_60 = 0x229B;
static int64_t g_93 = 0x066996F54E6A7687LL;
static int16_t g_94 = 0xF2FD;
static int32_t g_96 = 0x2CF448C3;
static int16_t g_97 = 0x49E8;
static int16_t g_99[3] = {0x8971,0x8971,0x8971};
static int8_t g_102[2][1] = {{0xF7},{0xF7}};
static uint8_t g_103 = 255U;

static int32_t  func_1(void);
static uint8_t  func_33(int16_t  p_34, int16_t * p_35, uint16_t  p_36, int8_t  p_37);
static int16_t  func_47(uint32_t  p_48, int16_t  p_49);

static int32_t  func_1(void)
{ 
    uint64_t l_5[1];
    uint16_t l_22 = 0x33C0;
    int32_t *l_114 = &g_96;
    int32_t *l_115 = &g_2;
    uint64_t l_116 = 1U;
    int32_t *l_117 = (void*)0;
    int32_t *l_118[1];
    int i;
    for (i = 0; i < 1; i++)
        l_5[i] = 1U;
    for (i = 0; i < 1; i++)
        l_118[i] = (void*)0;
    for (g_2 = 0; (g_2 != (-13)); --g_2)
    { 
        int16_t l_23 = 6;
        int32_t *l_25 = (void*)0;
        int32_t **l_24 = &l_25;
        uint32_t l_27 = 0x7B400765;
        for (g_6 = 0; (g_6 <= 0); g_6 += 1)
        { 
            int32_t **l_7 = (void*)0;
            int32_t *l_9 = &g_2;
            int32_t **l_8 = &l_9;
            int8_t *l_20[4][2];
            int16_t *l_26 = &l_23;
            int i, j;
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 2; j++)
                    l_20[i][j] = &g_21;
            }
            (*l_8) = &g_2;
            if (((safe_sub_func_int16_t_s_s(((*l_26) = (safe_mod_func_uint8_t_u_u((((safe_rshift_func_int16_t_s_s(((((0x4DF6F97812A3CC92LL < (safe_mod_func_int8_t_s_s(((l_22 |= ((((safe_sub_func_uint16_t_u_u(l_5[g_6], g_2)) || (*l_9)) <= g_6) , l_5[0])) > 0x5A), g_6))) , g_2) == 0x8EE3F2F3) , l_23), l_23)) , l_24) == (void*)0), (**l_8)))), 0xE9E5)) == l_27))
            { 
                return l_22;
            }
            else
            { 
                int16_t *l_38 = &g_39;
                int i;
                --g_28;
                g_58 ^= (g_2 > (safe_div_func_int32_t_s_s(((l_5[g_6] ^= (((func_33(((*l_38) = ((*l_26) = g_28)), g_40, g_6, g_6) <= g_102[1][0]) , &l_25) != &l_25)) == 6), 1)));
            }
        }
    }
    g_46 ^= (l_5[0] , (g_6 = (safe_rshift_func_int8_t_s_u((safe_mul_func_int64_t_s_s((safe_sub_func_uint32_t_u_u((safe_mod_func_uint64_t_u_u((((0xFB5D9459 > ((*l_115) = ((*l_114) = l_5[0]))) && l_116) <= g_103), g_103)), g_55[2])), g_102[0][0])), g_102[1][0]))));
    for (l_22 = 0; l_22 < 1; l_22 = l_22 + 1)
    {
        l_5[l_22] = 0U;
    }
    return g_2;
}

static uint8_t  func_33(int16_t  p_34, int16_t * p_35, uint16_t  p_36, int8_t  p_37)
{ 
    int32_t *l_44 = &g_6;
    int32_t l_59 = 0xA536CA9A;
    int32_t l_95 = 0x814B4F51;
    int32_t l_98 = 0;
    int32_t l_100[1][3];
    int64_t l_101 = (-1);
    int i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 3; j++)
            l_100[i][j] = 9;
    }
    for (p_34 = 0; (p_34 <= (-22)); p_34 = safe_sub_func_int32_t_s_s(p_34, 1))
    { 
        int32_t *l_45 = &g_46;
        const int16_t *l_50 = (void*)0;
        int16_t l_89 = 1;
        int32_t *l_92[1][2];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 2; j++)
                l_92[i][j] = &l_59;
        }
        (*l_45) = (l_44 != (void*)0);
        if ((p_34 != func_47((g_55[2] = ((l_50 = l_50) == (((safe_rshift_func_uint16_t_u_s((safe_add_func_uint64_t_u_u(((void*)0 == &l_44), p_36)), (*l_45))) < (*l_44)) , (void*)0))), (*g_40))))
        { 
            int32_t *l_57[3][1];
            uint16_t *l_71 = &g_60;
            int64_t *l_80 = (void*)0;
            int64_t *l_81 = &g_55[1];
            int i, j;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 1; j++)
                    l_57[i][j] = &g_46;
            }
            ++g_60;
            if ((((safe_mod_func_uint32_t_u_u((0x82 | (safe_mod_func_uint64_t_u_u((safe_div_func_int32_t_s_s((safe_rshift_func_uint16_t_u_s(((*l_71)++), 12)), p_37)), ((*l_81) = (safe_div_func_uint16_t_u_u((safe_lshift_func_uint16_t_u_s(((safe_add_func_int32_t_s_s(p_36, 1)) , (*l_45)), (*g_40))), 0xC17F)))))), 0x0CCCF04A)) , (void*)0) == (void*)0))
            { 
                return g_60;
            }
            else
            { 
                return p_37;
            }
        }
        else
        { 
            int32_t *l_90 = &g_6;
            int32_t **l_91 = &l_44;
            (*l_91) = ((safe_sub_func_int64_t_s_s((safe_unary_minus_func_uint8_t_u(((*l_45) = (safe_add_func_int16_t_s_s(((((*g_40) || (safe_add_func_uint32_t_u_u(((-1) > l_89), p_37))) && 0x0B7F2DBF905AFB09LL) && 0x3419813DA773CE50LL), (*g_40)))))), 0x7CC540EDC169712DLL)) , l_90);
        }
        g_103++;
    }
    return g_39;
}

static int16_t  func_47(uint32_t  p_48, int16_t  p_49)
{ 
    int32_t * const *l_56 = (void*)0;
    g_46 &= (((5 == (l_56 == l_56)) == (-10)) == 7);
    return (*g_40);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_2, "g_2", print_hash_value);
    transparent_crc(g_6, "g_6", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_28, "g_28", print_hash_value);
    transparent_crc(g_39, "g_39", print_hash_value);
    transparent_crc(g_41, "g_41", print_hash_value);
    transparent_crc(g_46, "g_46", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_55[i], "g_55[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_60, "g_60", print_hash_value);
    transparent_crc(g_93, "g_93", print_hash_value);
    transparent_crc(g_94, "g_94", print_hash_value);
    transparent_crc(g_96, "g_96", print_hash_value);
    transparent_crc(g_97, "g_97", print_hash_value);
    for (i = 0; i < 3; i++)
    {
        transparent_crc(g_99[i], "g_99[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_102[i][j], "g_102[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_103, "g_103", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
