/* Csmith 0cdc710315cf seed 19, flattened by scripts/csmith_flatten.py */
// Options:   --seed 19 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static uint32_t
safe_add_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ui1 + ui2;
}
static int64_t
safe_lshift_func_int64_t_s_u(int64_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (9223372036854775807L >> ((unsigned int)right)))) ?
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
static uint16_t
safe_mod_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
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
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
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
static int16_t
safe_unary_minus_func_int16_t_s(int16_t si )
{
  return
    -si;
}
/* ---- generated program ---- */
static long __undefined;

static int32_t g_3 = 0xCD795364;
static int32_t g_4 = 0x4543BEB7;
static int32_t g_15 = 0x43AF7806;
static int32_t g_17 = 0xB002891B;
static int16_t g_23 = 0xB622;
static uint16_t g_24 = 0x88CA;
static int64_t g_37 = (-1);
static int32_t *g_68 = &g_15;
static uint8_t g_74 = 0x95;
static uint32_t g_84 = 0xEDBFDF84;
static int32_t g_88[2] = {0xC0D6AF10,0xC0D6AF10};
static int32_t g_90 = 0x686747B3;

static int8_t  func_1(void);
static uint8_t  func_9(int8_t  p_10);
static int32_t  func_28(uint64_t  p_29, const int32_t * p_30, const int16_t  p_31, int32_t * p_32);

static int8_t  func_1(void)
{ 
    int64_t l_2[3][1];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 1; j++)
            l_2[i][j] = 0x99FF606579763266LL;
    }
    for (g_3 = 0; (g_3 >= 0); g_3 -= 1)
    { 
        int32_t l_85 = 0xF01DDAC0;
        for (g_4 = 0; (g_4 <= 0); g_4 += 1)
        { 
            int32_t *l_86 = (void*)0;
            int32_t *l_87 = &g_15;
            int32_t *l_89 = &g_90;
            int i, j;
            (*l_89) |= ((((((*l_87) = ((((0xE66C == (safe_mod_func_int64_t_s_s((safe_lshift_func_int64_t_s_u((func_9((safe_add_func_uint32_t_u_u(l_2[g_3][g_3], l_2[g_3][g_3]))) & g_3), g_84)), l_2[1][0]))) == l_2[g_3][g_3]) > 4294967286U) >= l_85)) <= g_4) < g_88[0]) | l_85) , 8);
        }
        (*g_68) = (*g_68);
        for (g_37 = 0; (g_37 <= 0); g_37 += 1)
        { 
            for (g_15 = 0; (g_15 <= 0); g_15 += 1)
            { 
                return g_37;
            }
        }
    }
    return l_2[0][0];
}

static uint8_t  func_9(int8_t  p_10)
{ 
    uint32_t l_13[4] = {0xC863CC1D,0xC863CC1D,0xC863CC1D,0xC863CC1D};
    int32_t l_59 = 0x0913DB3B;
    int64_t *l_81 = (void*)0;
    int32_t *l_82 = &l_59;
    int32_t l_83 = 0x43A4231C;
    int i;
    for (p_10 = 3; (p_10 >= 0); p_10 -= 1)
    { 
        int32_t l_46 = 6;
        int32_t **l_77 = &g_68;
        int i;
        if ((~l_13[p_10]))
        { 
            int16_t l_27[2][1];
            const int32_t *l_33 = (void*)0;
            int i, j;
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < 1; j++)
                    l_27[i][j] = 0x3F6D;
            }
            for (g_15 = 0; (g_15 <= 3); g_15 += 1)
            { 
                int32_t *l_16 = &g_17;
                int32_t *l_18 = &g_17;
                int32_t *l_19 = &g_17;
                int32_t *l_20 = &g_17;
                int32_t *l_21 = &g_17;
                int32_t *l_22[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_22[i] = &g_17;
                g_24--;
                if (l_27[0][0])
                    break;
            }
            for (g_24 = 0; (g_24 <= 3); g_24 += 1)
            { 
                int64_t *l_36 = &g_37;
                int32_t l_47 = 0x8003DBE6;
                int32_t *l_48 = &g_15;
                l_46 |= func_28(p_10, l_33, (safe_lshift_func_uint8_t_u_s((((*l_36) |= p_10) < p_10), p_10)), &g_17);
                (*l_48) = (((&g_3 != &g_4) >= (l_13[3] && 0U)) != l_47);
            }
        }
        else
        { 
            int32_t *l_60 = (void*)0;
            int32_t *l_61 = &g_17;
            int64_t *l_66 = &g_37;
            int64_t **l_67 = &l_66;
            (*l_61) |= (l_59 = (safe_rshift_func_int32_t_s_u(((!l_13[p_10]) ^ ((((safe_mod_func_uint16_t_u_u((safe_lshift_func_uint32_t_u_u((safe_unary_minus_func_int16_t_s((4294967294U <= (safe_rshift_func_int64_t_s_u(0, 4))))), l_13[2])), p_10)) , g_15) == p_10) < (-6))), 8)));
            if (((safe_sub_func_int32_t_s_s(((safe_add_func_int32_t_s_s((((*l_67) = l_66) == (void*)0), p_10)) , g_17), 0xAEEA3ADE)) | g_24))
            { 
                if (p_10)
                    break;
                g_68 = (void*)0;
            }
            else
            { 
                int32_t *l_69 = &g_17;
                int32_t *l_70 = &g_15;
                int32_t *l_71 = &g_17;
                int32_t *l_72[4][4] = {{&l_59,&l_59,&l_59,&l_59},{&l_59,&l_59,&l_59,&l_59},{&l_59,&l_59,&l_59,&l_59},{&l_59,&l_59,&l_59,&l_59}};
                int32_t l_73 = 0x3419813D;
                int i, j;
                ++g_74;
            }
        }
        (*l_77) = &g_17;
    }
    (*l_82) = ((*g_68) = ((!(((((safe_rshift_func_int8_t_s_s((g_17 , (-3)), 6)) ^ ((l_81 != (void*)0) >= l_13[0])) < g_24) & 7U) < l_59)) && l_13[0]));
    return l_83;
}

static int32_t  func_28(uint64_t  p_29, const int32_t * p_30, const int16_t  p_31, int32_t * p_32)
{ 
    int64_t l_40 = 0;
    int32_t l_41 = 0x0FA2E85C;
    int64_t l_42[4][3] = {{0x9BA1F1C18A0A82B5LL,0x9BA1F1C18A0A82B5LL,0x9BA1F1C18A0A82B5LL},{0,0x77CB11B1996FCD4DLL,0},{0x9BA1F1C18A0A82B5LL,0x9BA1F1C18A0A82B5LL,0x9BA1F1C18A0A82B5LL},{0,0x77CB11B1996FCD4DLL,0}};
    int i, j;
lbl_45:
    l_41 = (safe_mul_func_uint16_t_u_u((l_40 ^ g_15), l_40));
    for (g_37 = 2; (g_37 >= 0); g_37 -= 1)
    { 
        int32_t *l_43 = &g_3;
        for (g_23 = 0; (g_23 <= 2); g_23 += 1)
        { 
            (*p_32) |= l_41;
        }
        if (g_37)
            continue;
        for (g_23 = 0; (g_23 <= 2); g_23 += 1)
        { 
            int32_t **l_44 = &l_43;
            (*l_44) = l_43;
            if (l_40)
                goto lbl_45;
            for (l_41 = 2; (l_41 >= 0); l_41 -= 1)
            { 
                return g_15;
            }
        }
    }
    return g_4;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_3, "g_3", print_hash_value);
    transparent_crc(g_4, "g_4", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_23, "g_23", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_37, "g_37", print_hash_value);
    transparent_crc(g_74, "g_74", print_hash_value);
    transparent_crc(g_84, "g_84", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_88[i], "g_88[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_90, "g_90", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
