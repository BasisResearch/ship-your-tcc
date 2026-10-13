/* Csmith 0cdc710315cf seed 151, flattened by scripts/csmith_flatten.py */
// Options:   --seed 151 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_add_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 + si2);
}
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int64_t
safe_div_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-9223372036854775807L-1)) && (si2 == (-1)))) ?
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
static uint8_t
safe_div_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
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
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
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
static int16_t
safe_mod_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 % si2);
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
static int32_t
safe_mul_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    (((si1 > 0) && (si2 > 0) && (si1 > (2147483647 / si2))) || ((si1 > 0) && (si2 <= 0) && (si2 < ((-2147483647-1) / si1))) || ((si1 <= 0) && (si2 > 0) && (si1 < ((-2147483647-1) / si2))) || ((si1 <= 0) && (si2 <= 0) && (si1 != 0) && (si2 < (2147483647 / si1)))) ?
    ((si1)) :
    si1 * si2;
}
static uint16_t
safe_mul_func_uint16_t_u_u(uint16_t ui1, uint16_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static uint32_t
safe_mul_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
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
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
}
static uint8_t
safe_rshift_func_uint8_t_u_s(uint8_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32)) ?
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
static int64_t
safe_sub_func_int64_t_s_s(int64_t si1, int64_t si2 )
{
  return
    (((si1^si2) & (((si1 ^ ((si1^si2) & (~9223372036854775807L)))-si2)^si2)) < 0) ?
    ((si1)) :
    (si1 - si2);
}
/* ---- generated program ---- */
static long __undefined;

struct S0 {
   const uint8_t  f0;
};

union U1 {
   uint8_t  f0;
   const uint32_t  f1;
   const struct S0  f2;
};

static union U1 *g_10[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
static int32_t g_13 = (-1);
static uint64_t g_22 = 1U;
static int32_t g_51 = 0xDF100F83;
static int32_t *g_52 = &g_51;
static uint32_t g_65 = 4294967290U;
static int32_t g_75 = (-1);
static int32_t * const g_74 = &g_75;
static int32_t * const *g_73 = &g_74;
static uint16_t g_79 = 0x87DC;
static int16_t g_125 = 0xCA48;
static int16_t *g_124[2] = {&g_125,&g_125};

static int8_t  func_1(void);
static int8_t  func_5(int32_t  p_6);
static const uint32_t  func_26(int32_t ** p_27, uint64_t * p_28);

static int8_t  func_1(void)
{ 
    uint32_t l_4 = 0xD13034D0;
    int32_t *l_30 = &g_13;
    int32_t **l_29 = &l_30;
    union U1 l_37[2][1] = {{{0xBB}},{{0xBB}}};
    int32_t l_61 = 0x6C3CE99C;
    int16_t l_108 = (-1);
    uint16_t l_109[3];
    uint32_t l_110 = 8U;
    int i, j;
    for (i = 0; i < 3; i++)
        l_109[i] = 0x2115;
    if (((safe_add_func_int32_t_s_s(0xBF506E60, 0xCEAFD94F)) , l_4))
    { 
        int64_t l_7 = 1;
        int32_t *l_12[2][2] = {{&g_13,&g_13},{&g_13,&g_13}};
        int i, j;
        g_13 = (func_5(l_7) > 0x08);
        for (l_7 = 0; (l_7 > (-3)); --l_7)
        { 
            int32_t *l_16 = &g_13;
            if ((g_13 |= (l_16 != l_12[0][0])))
            { 
                int16_t l_17 = 0x55FD;
                (*l_16) = (l_17 |= ((((*l_16) != g_13) < g_13) | 0x36));
            }
            else
            { 
                int32_t **l_18 = &l_12[0][1];
                (*l_18) = (void*)0;
                if (l_4)
                    break;
            }
            if (g_13)
                continue;
        }
    }
    else
    { 
        uint64_t *l_21 = &g_22;
        int32_t l_25 = (-8);
        l_25 = (safe_lshift_func_uint16_t_u_s(((--(*l_21)) || l_25), 11));
    }
    (**l_29) = ((l_4 != 0) && func_26(l_29, &g_22));
    if ((l_37[1][0] , ((**l_29) = g_22)))
    { 
        const int32_t *l_38 = &g_13;
        const int32_t **l_39 = &l_38;
        (**l_29) |= g_22;
        (*l_39) = l_38;
    }
    else
    { 
        union U1 *l_44 = &l_37[1][0];
        int16_t l_69 = 0xA420;
        int32_t l_78[2];
        int32_t * const *l_112 = &l_30;
        int i;
        for (i = 0; i < 2; i++)
            l_78[i] = 1;
lbl_98:
        for (g_13 = 3; (g_13 >= 0); g_13 -= 1)
        { 
            uint32_t l_68 = 0x3D2215AC;
            if ((safe_div_func_uint8_t_u_u((safe_sub_func_int64_t_s_s(0xBB389E7675DAAFE4LL, ((void*)0 != l_44))), 255U)))
            { 
                const int64_t l_49 = 0;
                int32_t *l_50 = &g_51;
                (*l_50) ^= ((safe_lshift_func_int64_t_s_s((g_13 , (0xD4EE ^ (safe_div_func_int32_t_s_s(((l_49 & g_22) | 0xDC), g_13)))), 35)) | 0x66);
                g_52 = &g_13;
            }
            else
            { 
                int16_t l_53 = (-10);
                int32_t *l_54 = &g_51;
                int32_t *l_55 = &g_51;
                int32_t *l_56 = &g_51;
                int32_t *l_57 = &g_51;
                int32_t *l_58 = &g_51;
                int32_t *l_59 = &g_51;
                int32_t *l_60 = &g_51;
                int32_t *l_62 = (void*)0;
                int32_t *l_63 = &g_51;
                int32_t *l_64[1][2];
                int i, j;
                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 2; j++)
                        l_64[i][j] = &l_61;
                }
                g_65++;
            }
            return l_68;
        }
        (**l_29) &= l_69;
        if (((((safe_mod_func_uint8_t_u_u(((!(l_78[0] = (((((((g_73 = &g_52) != (void*)0) | (safe_mul_func_int32_t_s_s((&l_44 != &g_10[1]), (*g_52)))) > (**l_29)) , 1) , (*l_30)) ^ g_51))) , 0xD6), 0xDD)) | g_79) ^ (-1)) , (**l_29)))
        { 
            int32_t l_87 = 0xAE9A1548;
            if ((*g_52))
            { 
                uint16_t *l_80 = &g_79;
                uint32_t *l_83 = (void*)0;
                uint32_t *l_84 = &l_4;
                int32_t l_97 = 0x87D51EEA;
                (**g_73) = (((*l_80) &= (g_65 <= 0xB57C)) < 1);
                (*g_74) |= ((safe_mul_func_uint32_t_u_u(((*l_84)++), (g_65 < g_79))) , l_87);
                (**g_73) |= (((safe_sub_func_int64_t_s_s(((**l_29) && (safe_add_func_int8_t_s_s((~g_79), (safe_lshift_func_uint16_t_u_u(g_22, 9))))), g_22)) | l_97) > 0);
            }
            else
            { 
                int32_t l_99[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_99[i] = 0;
                if (g_22)
                    goto lbl_98;
                (*g_52) = 0xDF4734A2;
                (**l_29) = l_99[0];
            }
            for (g_51 = 3; (g_51 >= 0); g_51 -= 1)
            { 
                int16_t *l_111 = &l_69;
                l_110 ^= (safe_mod_func_int16_t_s_s((safe_div_func_uint16_t_u_u((((safe_rshift_func_int8_t_s_s(((safe_div_func_int64_t_s_s(g_13, (g_75 | (((((g_65 <= 0x306C0CD1CB4528E6LL) & (-1)) | (*l_30)) >= l_87) , g_13)))) , g_75), 1)) | l_108) != l_109[2]), 0x3C15)), 0x9768));
                l_112 = (((*l_111) = g_75) , &g_74);
            }
        }
        else
        { 
            int64_t l_126 = 1;
            uint8_t l_135 = 1U;
            for (g_75 = 3; (g_75 >= 0); g_75 -= 1)
            { 
                uint64_t l_121[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_121[i] = 0x5B7EE776C1F1C6DCLL;
                (*g_52) &= (safe_mod_func_int32_t_s_s((-1), (safe_rshift_func_uint8_t_u_s((safe_sub_func_int64_t_s_s(((safe_lshift_func_uint32_t_u_u((((void*)0 == &g_52) && l_121[0]), 14)) & g_13), 3)), 0))));
            }
            if (((safe_sub_func_int32_t_s_s((g_51 >= (g_124[1] != &l_69)), 0)) == g_75))
            { 
                return (**l_29);
            }
            else
            { 
                union U1 l_134 = {1U};
                int16_t *l_136 = &l_108;
                uint16_t l_137[1];
                int i;
                for (i = 0; i < 1; i++)
                    l_137[i] = 0x80AB;
                (*g_74) = ((**l_29) = ((l_126 <= ((*l_136) = (l_126 & (safe_mod_func_uint8_t_u_u(((+((safe_mod_func_uint16_t_u_u((safe_mul_func_uint16_t_u_u((g_79 &= (l_134 , l_135)), l_126)), (**l_112))) > g_65)) > l_135), g_65))))) <= (**l_29)));
                return l_137[0];
            }
        }
    }
    return (**l_29);
}

static int8_t  func_5(int32_t  p_6)
{ 
    union U1 *l_8 = (void*)0;
    union U1 **l_9 = &l_8;
    int32_t l_11 = 0x93DDA37B;
    g_10[3] = ((*l_9) = l_8);
    return l_11;
}

static const uint32_t  func_26(int32_t ** p_27, uint64_t * p_28)
{ 
    int32_t l_33 = 1;
    int32_t *l_36 = &l_33;
    (**p_27) |= (safe_rshift_func_int16_t_s_s(l_33, (safe_add_func_int8_t_s_s(0, l_33))));
    (*l_36) = (g_13 = 0xF87662B0);
    return g_13;
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_13, "g_13", print_hash_value);
    transparent_crc(g_22, "g_22", print_hash_value);
    transparent_crc(g_51, "g_51", print_hash_value);
    transparent_crc(g_65, "g_65", print_hash_value);
    transparent_crc(g_75, "g_75", print_hash_value);
    transparent_crc(g_79, "g_79", print_hash_value);
    transparent_crc(g_125, "g_125", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
