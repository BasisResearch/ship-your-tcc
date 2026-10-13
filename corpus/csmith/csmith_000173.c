/* Csmith 0cdc710315cf seed 173, flattened by scripts/csmith_flatten.py */
// Options:   --seed 173 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int8_t
safe_div_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-128)) && (si2 == (-1)))) ?
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
static uint32_t
safe_div_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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
static uint16_t
safe_lshift_func_uint16_t_u_u(uint16_t left, unsigned int right )
{
  return
    ((((unsigned int)right) >= 32) || (left > (65535 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static int16_t
safe_mul_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    si1 * si2;
}
static int8_t
safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    si1 * si2;
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int8_t
safe_rshift_func_int8_t_s_s(int8_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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

struct S0 {
   const uint8_t  f0;
   uint8_t  f1;
   int32_t  f2;
   int64_t  f3;
};

struct S1 {
   uint32_t  f0;
   int8_t  f1;
   uint8_t  f2;
   uint16_t  f3;
};

union U2 {
   int16_t  f0;
};

static uint16_t g_8 = 0x8047;
static uint16_t g_14 = 0xE800;
static int32_t g_17 = (-1);
static int8_t g_25 = 0xD6;
static uint8_t g_26 = 0x9A;
static uint32_t g_35 = 2U;
static int32_t g_58 = 0xE65C5BCC;
static uint64_t g_63 = 1U;
static const struct S0 g_70 = {0x2D,251U,3,1};
static union U2 g_72[4] = {{0xD45A},{0xD45A},{0xD45A},{0xD45A}};
static struct S1 g_73 = {0x83FFB38B,1,255U,0x0DDF};
static int16_t g_83 = (-1);
static int8_t g_84 = 1;
static uint32_t g_95 = 0x5E3342DF;
static int32_t *g_105 = &g_58;
static int32_t **g_104 = &g_105;
static int32_t g_118 = 3;
static int32_t **g_119 = (void*)0;

static int8_t  func_1(void);
static int32_t * func_46(union U2  p_47, int32_t ** p_48, struct S1  p_49, uint64_t  p_50);
static union U2  func_51(const uint16_t * p_52);

static int8_t  func_1(void)
{ 
    int64_t l_2 = (-5);
    int32_t l_24[2];
    int32_t *l_38 = &l_24[0];
    int8_t l_74 = 0x8C;
    int64_t l_120[1];
    int i;
    for (i = 0; i < 2; i++)
        l_24[i] = (-10);
    for (i = 0; i < 1; i++)
        l_120[i] = 0xC7E2165FCA2CAD0FLL;
    if (l_2)
    { 
        int64_t l_3[3][4] = {{(-1),(-1),0x8F3D4210E164CAF1LL,(-1)},{(-1),0xC90D29729CF30836LL,0xC90D29729CF30836LL,(-1)},{0xC90D29729CF30836LL,(-1),0xC90D29729CF30836LL,0xC90D29729CF30836LL}};
        int32_t *l_29 = &g_17;
        int32_t *l_30 = &l_24[1];
        int32_t *l_31 = &l_24[0];
        int32_t *l_32 = &l_24[0];
        int32_t *l_33 = &l_24[0];
        int32_t *l_34[2];
        int i, j;
        for (i = 0; i < 2; i++)
            l_34[i] = &l_24[0];
        for (l_2 = 0; (l_2 <= 2); l_2 += 1)
        { 
            uint16_t *l_13 = &g_14;
            if ((l_3[1][3] < (safe_mul_func_int16_t_s_s((safe_lshift_func_int16_t_s_u(g_8, ((*l_13) = (safe_mul_func_int8_t_s_s((safe_div_func_uint32_t_u_u(0xE5B14F4E, 0x53589FE5)), l_3[2][1]))))), 0x02AF))))
            { 
                int32_t *l_16 = &g_17;
                int32_t **l_15 = &l_16;
                (*l_15) = (void*)0;
                return g_14;
            }
            else
            { 
                int32_t *l_18 = &g_17;
                int32_t *l_19 = &g_17;
                int32_t *l_20 = &g_17;
                int32_t *l_21 = (void*)0;
                int32_t *l_22 = (void*)0;
                int32_t *l_23[3];
                int i;
                for (i = 0; i < 3; i++)
                    l_23[i] = &g_17;
                --g_26;
                return g_8;
            }
        }
        g_35--;
    }
    else
    { 
        int32_t **l_113 = &g_105;
        uint16_t *l_114 = &g_73.f3;
        int64_t l_115 = 1;
        const int8_t l_116 = 0;
lbl_106:
        if ((l_38 == &g_17))
        { 
            (*l_38) ^= 3;
        }
        else
        { 
            const uint16_t *l_53[2][4] = {{(void*)0,(void*)0,(void*)0,(void*)0},{(void*)0,(void*)0,(void*)0,(void*)0}};
            int i, j;
            for (l_2 = 0; (l_2 != (-20)); l_2 = safe_sub_func_uint32_t_u_u(l_2, 5))
            { 
                int32_t *l_45 = &g_17;
                const uint16_t **l_54 = &l_53[1][0];
                int32_t **l_102 = &l_38;
            }
        }
        if ((+(((((g_104 == &l_38) <= 0) > 0x11800AB3) == (*l_38)) , (*l_38))))
        { 
            if (g_26)
                goto lbl_106;
        }
        else
        { 
            (*g_104) = &l_24[0];
        }
        if (((safe_div_func_uint16_t_u_u(((((safe_mul_func_uint8_t_u_u((safe_rshift_func_int8_t_s_s(((((0x2720855C5D77E079LL | (((*l_114) = ((l_113 = l_113) == (void*)0)) || 1)) || 2U) & 0xEE56) < g_73.f0), g_70.f1)), 1)) , (*l_38)) == 65534U) & l_115), l_116)) , 7))
        { 
            int32_t *l_117 = &g_118;
            (*l_117) ^= ((((*g_104) == (*g_104)) & (func_51(&g_8) , (*l_38))) , (**l_113));
            (**g_104) &= (&l_116 != &g_25);
        }
        else
        { 
            (*g_104) = func_46(g_72[2], g_119, g_73, (**l_113));
            return g_73.f3;
        }
    }
    return l_120[0];
}

static int32_t * func_46(union U2  p_47, int32_t ** p_48, struct S1  p_49, uint64_t  p_50)
{ 
    int32_t *l_75 = &g_58;
    int32_t *l_76 = &g_58;
    int32_t *l_77 = &g_17;
    int32_t *l_78 = &g_58;
    int32_t *l_79 = &g_58;
    int32_t *l_80 = &g_17;
    int32_t *l_81 = &g_58;
    int32_t *l_82[3][4] = {{&g_17,&g_17,&g_17,&g_17},{&g_17,&g_17,&g_58,&g_17},{&g_17,&g_17,&g_17,&g_17}};
    uint64_t l_85 = 0xE0369C31B0F82792LL;
    struct S0 l_90 = {254U,247U,0xCB54A3E8,0x205D61439AB163B6LL};
    int8_t *l_100[4];
    uint8_t *l_101 = &l_90.f1;
    int i, j;
    for (i = 0; i < 4; i++)
        l_100[i] = &g_25;
    l_85++;
    g_17 = (((((safe_div_func_int32_t_s_s((l_90 , (safe_lshift_func_uint16_t_u_u((safe_sub_func_uint8_t_u_u(((*l_101) = (g_26 = (g_95 | (safe_div_func_int8_t_s_s(((*l_79) |= (safe_lshift_func_int16_t_s_s(g_73.f1, g_70.f2))), g_72[3].f0))))), 0x65)), p_49.f1))), g_72[3].f0)) != g_8) == 0xA49A9C08A91624B3LL) < 0xCE17D650891F15DELL) != 0xE9516CEA);
    return l_78;
}

static union U2  func_51(const uint16_t * p_52)
{ 
    int64_t l_60 = (-4);
    int32_t l_61 = 0x4D80A992;
    int32_t l_62 = 1;
    int32_t *l_71 = &g_58;
    for (g_17 = 0; (g_17 < (-6)); --g_17)
    { 
        int32_t *l_57 = &g_58;
        int32_t *l_59[3][3] = {{&g_58,&g_58,&g_58},{&g_58,&g_17,&g_58},{&g_58,&g_58,&g_58}};
        int i, j;
        g_63++;
    }
    (*l_71) ^= (safe_sub_func_uint8_t_u_u((safe_add_func_uint64_t_u_u((&g_17 != (g_70 , &l_61)), l_62)), l_61));
    return g_72[3];
}

int main (void)
{
    int i;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_8, "g_8", print_hash_value);
    transparent_crc(g_14, "g_14", print_hash_value);
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_25, "g_25", print_hash_value);
    transparent_crc(g_26, "g_26", print_hash_value);
    transparent_crc(g_35, "g_35", print_hash_value);
    transparent_crc(g_58, "g_58", print_hash_value);
    transparent_crc(g_63, "g_63", print_hash_value);
    transparent_crc(g_70.f0, "g_70.f0", print_hash_value);
    transparent_crc(g_70.f1, "g_70.f1", print_hash_value);
    transparent_crc(g_70.f2, "g_70.f2", print_hash_value);
    transparent_crc(g_70.f3, "g_70.f3", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        transparent_crc(g_72[i].f0, "g_72[i].f0", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_73.f0, "g_73.f0", print_hash_value);
    transparent_crc(g_73.f1, "g_73.f1", print_hash_value);
    transparent_crc(g_73.f2, "g_73.f2", print_hash_value);
    transparent_crc(g_73.f3, "g_73.f3", print_hash_value);
    transparent_crc(g_83, "g_83", print_hash_value);
    transparent_crc(g_84, "g_84", print_hash_value);
    transparent_crc(g_95, "g_95", print_hash_value);
    transparent_crc(g_118, "g_118", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
