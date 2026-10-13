/* Csmith 0cdc710315cf seed 135, flattened by scripts/csmith_flatten.py */
// Options:   --seed 135 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int32_t
safe_div_func_int32_t_s_s(int32_t si1, int32_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-2147483647-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int8_t
safe_lshift_func_int8_t_s_u(int8_t left, unsigned int right )
{
  return
    ((left < 0) || (((unsigned int)right) >= 32) || (left > (127 >> ((unsigned int)right)))) ?
    ((left)) :
    (left << ((unsigned int)right));
}
static uint16_t
safe_lshift_func_uint16_t_u_s(uint16_t left, int right )
{
  return
    ((((int)right) < 0) || (((int)right) >= 32) || (left > (65535 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint32_t
safe_mod_func_uint32_t_u_u(uint32_t ui1, uint32_t ui2 )
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

union U0 {
   int64_t  f0;
   int16_t  f1;
   int8_t  f2;
};

union U1 {
   uint16_t  f0;
   uint64_t  f1;
};

static union U1 g_16 = {0U};
static uint64_t g_17 = 0U;
static int32_t g_24 = (-1);
static int32_t *g_26 = &g_24;
static uint32_t g_30 = 4294967295U;
static uint64_t g_55 = 0xE3CEC334572090F9LL;
static uint32_t g_61 = 0x84923552;
static int8_t g_66[2][1] = {{(-2)},{(-2)}};
static int16_t g_73 = 0x5BA8;
static int16_t *g_72[4] = {(void*)0,(void*)0,(void*)0,(void*)0};
static union U0 g_84 = {0x27BBAC67CE96EA65LL};
static union U0 *g_83 = &g_84;
static int8_t g_100[4][4] = {{(-3),(-3),(-3),(-3)},{(-3),(-3),(-3),(-3)},{(-3),(-3),(-3),(-3)},{(-3),(-3),(-3),(-3)}};
static int64_t g_101 = (-7);
static uint64_t g_103 = 0xBFA59640E1577BE7LL;
static uint16_t g_121 = 0x33A1;
static uint64_t g_138 = 0xC673911152AC1009LL;
static int64_t g_156 = 0x2A04F454AEDB1328LL;
static int32_t **g_163 = &g_26;
static int32_t g_165 = 0x74765F4E;
static uint8_t g_173 = 0x7A;
static int64_t g_193 = 0xFAD5DA70D91B73B2LL;
static int16_t g_195 = 0x9688;
static int8_t g_197 = (-1);
static uint8_t g_198 = 0xD0;
static int8_t g_211[2] = {0,0};
static uint16_t g_212 = 0x201D;

static int64_t  func_1(void);
static uint8_t  func_5(int32_t  p_6, const uint16_t  p_7, uint16_t  p_8);
static union U0  func_9(union U0  p_10, union U1  p_11, uint64_t  p_12, int64_t  p_13, int16_t  p_14);

static int64_t  func_1(void)
{ 
    const uint64_t l_4 = 0U;
    union U0 l_15 = {9};
    int32_t l_18[2];
    uint8_t *l_172 = &g_173;
    int16_t l_192 = 0xBE86;
    int i;
    for (i = 0; i < 2; i++)
        l_18[i] = 0x033C2251;
    if ((safe_sub_func_uint8_t_u_u(l_4, (l_18[1] = ((*l_172) = func_5((func_9(l_15, g_16, g_17, l_18[0], g_17) , 0xB0C0B0BD), g_84.f0, l_18[0]))))))
    { 
        int32_t *l_174 = (void*)0;
        g_26 = l_174;
    }
    else
    { 
        uint16_t l_177 = 0xD02E;
        uint16_t *l_180 = &l_177;
        int32_t l_194 = (-6);
        int64_t l_210 = 0x2FEFF4B0332E89E7LL;
        int8_t *l_228[3];
        int i;
        for (i = 0; i < 3; i++)
            l_228[i] = &g_100[1][1];
        for (g_73 = 0; (g_73 < 6); g_73 = safe_add_func_int16_t_s_s(g_73, 6))
        { 
            (*g_26) = (g_61 | 0x37);
            (*g_26) = l_177;
        }
        if ((safe_div_func_int32_t_s_s((l_180 == &g_121), 4294967293U)))
        { 
            int8_t l_196[4][3] = {{(-1),(-1),(-1)},{0x4F,(-1),0x4F},{(-1),(-1),(-1)},{0x4F,(-1),0x4F}};
            int i, j;
            for (g_101 = 17; (g_101 == 26); g_101 = safe_add_func_uint64_t_u_u(g_101, 4))
            { 
                int32_t *l_183 = &g_24;
                int32_t *l_184 = &l_18[1];
                int32_t *l_185 = &l_18[1];
                int32_t *l_186 = &g_165;
                int32_t *l_187 = &l_18[0];
                int32_t *l_188 = &g_24;
                int32_t *l_189 = &l_18[1];
                int32_t *l_190 = &l_18[1];
                int32_t *l_191[2];
                int i;
                for (i = 0; i < 2; i++)
                    l_191[i] = &g_165;
                g_198--;
            }
        }
        else
        { 
            int64_t l_206 = 0xD603A4C91E03B00BLL;
            int32_t l_209[1];
            int16_t l_215 = 5;
            int i;
            for (i = 0; i < 1; i++)
                l_209[i] = 3;
            for (l_15.f1 = 0; (l_15.f1 > (-23)); l_15.f1 = safe_sub_func_int16_t_s_s(l_15.f1, 8))
            { 
                int32_t *l_203 = &l_18[0];
                int32_t *l_204 = &l_18[0];
                int32_t *l_205 = &l_18[1];
                int32_t *l_207 = &l_18[0];
                int32_t *l_208[2];
                uint32_t l_216[3][4] = {{3U,3U,3U,3U},{3U,3U,3U,3U},{3U,3U,3U,3U}};
                int i, j;
                for (i = 0; i < 2; i++)
                    l_208[i] = &g_165;
                g_212--;
                ++l_216[2][0];
                if ((*g_26))
                    continue;
            }
        }
        (*g_26) = (((((safe_lshift_func_uint16_t_u_s(((safe_mod_func_uint8_t_u_u((safe_sub_func_int64_t_s_s((safe_add_func_int64_t_s_s(((((g_211[0] = l_194) > (g_84.f2 = (~4))) , ((l_4 <= 0x81DB) | l_210)) > (*g_26)), 5U)), l_18[0])), 1U)) == g_55), l_192)) , 0x3DB9) || 0xB92E) ^ g_66[1][0]) ^ (-8));
    }
    return g_73;
}

static uint8_t  func_5(int32_t  p_6, const uint16_t  p_7, uint16_t  p_8)
{ 
    int32_t **l_162 = &g_26;
    const uint32_t l_171 = 0U;
    (*g_26) &= 0;
    if ((&g_84 == &g_84))
    { 
        for (g_24 = 0; (g_24 <= (-19)); g_24 = safe_sub_func_uint8_t_u_u(g_24, 5))
        { 
            int32_t *l_164 = &g_165;
            (*l_164) |= ((l_162 = &g_26) != (g_163 = (void*)0));
        }
    }
    else
    { 
        union U0 **l_170[2][3];
        int i, j;
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 3; j++)
                l_170[i][j] = &g_83;
        }
        if ((safe_mod_func_uint32_t_u_u((**l_162), ((safe_lshift_func_int8_t_s_u((g_138 <= ((l_170[1][1] != (void*)0) >= l_171)), 3)) || g_121))))
        { 
            (**g_163) &= p_7;
        }
        else
        { 
            (**g_163) = (**g_163);
        }
    }
    return p_6;
}

static union U0  func_9(union U0  p_10, union U1  p_11, uint64_t  p_12, int64_t  p_13, int16_t  p_14)
{ 
    int16_t l_19[1];
    union U0 l_20 = {5};
    int32_t *l_23 = &g_24;
    int32_t l_53 = 0x3783EA6D;
    int32_t l_102 = 0xFB9A3054;
    union U0 **l_107 = &g_83;
    int i;
    for (i = 0; i < 1; i++)
        l_19[i] = 0xE6BC;
    for (p_14 = 0; (p_14 >= 0); p_14 -= 1)
    { 
        return l_20;
    }
    for (g_16.f1 = 0; (g_16.f1 == 54); g_16.f1++)
    { 
        int32_t **l_25[1][3];
        int i, j;
        for (i = 0; i < 1; i++)
        {
            for (j = 0; j < 3; j++)
                l_25[i][j] = &l_23;
        }
        g_26 = l_23;
    }
    if (p_14)
    { 
        for (p_11.f0 = 0; (p_11.f0 <= 0); p_11.f0 += 1)
        { 
            uint32_t *l_29 = &g_30;
            int32_t l_31 = 3;
            int i;
            l_31 ^= (l_19[p_11.f0] <= (safe_sub_func_uint32_t_u_u(p_14, (((*l_29) = (((l_19[p_11.f0] | (-1)) ^ p_10.f0) != (*l_23))) , 7))));
        }
    }
    else
    { 
        const int16_t l_45 = (-1);
        int32_t l_52 = 0x1CC0D681;
        int32_t l_54 = 8;
        uint8_t l_74 = 0xE9;
        uint16_t *l_120[4][4] = {{&g_121,&g_121,&g_121,&g_121},{&g_121,&g_121,&g_121,&g_121},{&g_121,&g_121,&g_121,&g_121},{&g_121,&g_121,&g_121,&g_121}};
        uint64_t l_125 = 18446744073709551612U;
        uint64_t l_153 = 0xCC3DE63DF0479FFELL;
        int i, j;
    }
    return (**l_107);
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_17, "g_17", print_hash_value);
    transparent_crc(g_24, "g_24", print_hash_value);
    transparent_crc(g_30, "g_30", print_hash_value);
    transparent_crc(g_55, "g_55", print_hash_value);
    transparent_crc(g_61, "g_61", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
        {
            transparent_crc(g_66[i][j], "g_66[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_73, "g_73", print_hash_value);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            transparent_crc(g_100[i][j], "g_100[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    transparent_crc(g_101, "g_101", print_hash_value);
    transparent_crc(g_103, "g_103", print_hash_value);
    transparent_crc(g_121, "g_121", print_hash_value);
    transparent_crc(g_138, "g_138", print_hash_value);
    transparent_crc(g_156, "g_156", print_hash_value);
    transparent_crc(g_165, "g_165", print_hash_value);
    transparent_crc(g_173, "g_173", print_hash_value);
    transparent_crc(g_193, "g_193", print_hash_value);
    transparent_crc(g_195, "g_195", print_hash_value);
    transparent_crc(g_197, "g_197", print_hash_value);
    transparent_crc(g_198, "g_198", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        transparent_crc(g_211[i], "g_211[i]", print_hash_value);
        if (print_hash_value) printf("index = [%d]\n", i);

    }
    transparent_crc(g_212, "g_212", print_hash_value);
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
