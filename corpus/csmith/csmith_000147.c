/* Csmith 0cdc710315cf seed 147, flattened by scripts/csmith_flatten.py */
// Options:   --seed 147 --ccomp --no-argc --no-volatiles --no-volatile-pointers --no-packed-struct --no-bitfields --no-builtins --no-inline-function --concise --max-funcs 3 --max-block-depth 3 --max-block-size 3 --max-expr-complexity 4 --max-array-dim 2 --max-array-len-per-dim 4 --max-struct-fields 4 --max-union-fields 3 --max-pointer-depth 2

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
static int16_t
safe_div_func_int16_t_s_s(int16_t si1, int16_t si2 )
{
  return
    ((si2 == 0) || ((si1 == (-32767-1)) && (si2 == (-1)))) ?
    ((si1)) :
    (si1 / si2);
}
static int32_t
safe_lshift_func_int32_t_s_s(int32_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32) || (left > (2147483647 >> ((int)right)))) ?
    ((left)) :
    (left << ((int)right));
}
static uint64_t
safe_mul_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2 )
{
  return ((unsigned long long)ui1) * ((unsigned long long)ui2);
}
static uint8_t
safe_mul_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ((unsigned int)ui1) * ((unsigned int)ui2);
}
static int64_t
safe_rshift_func_int64_t_s_s(int64_t left, int right )
{
  return
    ((left < 0) || (((int)right) < 0) || (((int)right) >= 32))?
    ((left)) :
    (left >> ((int)right));
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
safe_sub_func_int8_t_s_s(int8_t si1, int8_t si2 )
{
  return
    (si1 - si2);
}
static uint8_t
safe_sub_func_uint8_t_u_u(uint8_t ui1, uint8_t ui2 )
{
  return ui1 - ui2;
}
/* ---- generated program ---- */
static long __undefined;

union U0 {
   uint16_t  f0;
   uint8_t  f1;
   int32_t  f2;
};

static uint64_t g_10 = 0U;
static int64_t g_15 = (-1);
static uint64_t g_19 = 0U;
static uint64_t *g_18 = &g_19;
static int32_t g_21 = 0x80BFB3A4;
static union U0 g_25 = {65534U};
static union U0 *g_24 = &g_25;
static int32_t *g_30 = &g_21;
static int8_t g_44 = 0;
static int64_t g_52[2][2] = {{(-8),(-8)},{(-8),(-8)}};

static uint32_t  func_1(void);
static int32_t  func_2(uint32_t  p_3, int8_t  p_4);
static int32_t * func_22(int8_t  p_23);

static uint32_t  func_1(void)
{ 
    uint64_t *l_9 = &g_10;
    int32_t l_13 = 0xB6EC79CF;
    int64_t *l_14 = &g_15;
    int32_t *l_51[4];
    int i;
    for (i = 0; i < 4; i++)
        l_51[i] = &l_13;
    g_52[1][0] = func_2((safe_mul_func_uint8_t_u_u((safe_mul_func_uint64_t_u_u(((*l_9)--), ((*l_14) = l_13))), ((safe_div_func_int16_t_s_s((l_9 == g_18), (-1))) , 0x4F))), l_13);
    return g_52[1][0];
}

static int32_t  func_2(uint32_t  p_3, int8_t  p_4)
{ 
    int32_t *l_20 = &g_21;
    int32_t l_37 = 9;
    union U0 l_42[1][4] = {{{0xF2AA},{0xF2AA},{0xF2AA},{0xF2AA}}};
    int32_t **l_46[2][1];
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 1; j++)
            l_46[i][j] = &l_20;
    }
    (*l_20) = g_10;
    g_30 = func_22(p_4);
    if ((~((p_3 , 0x1C33) || ((((0x278EE3F2 | 0x8DCFD8F2) , (*l_20)) & p_4) >= (*l_20)))))
    { 
        int32_t l_36[2];
        int i;
        for (i = 0; i < 2; i++)
            l_36[i] = 1;
        l_37 |= (safe_lshift_func_int32_t_s_s((((*l_20) = (safe_sub_func_uint8_t_u_u(0U, ((0xA3888AD88CA0E2C2LL ^ ((p_4 == p_4) && l_36[1])) , 1)))) || 0xEC73), 5));
        (*g_30) |= ((0x5C48968F <= (safe_add_func_uint64_t_u_u(1U, (*g_18)))) ^ p_4);
    }
    else
    { 
        int8_t *l_43 = &g_44;
        int32_t *l_45 = &g_25.f2;
        (*l_45) = ((safe_sub_func_int8_t_s_s((-2), (l_42[0][2] , ((*l_43) = (((*l_20) = (*g_30)) || p_4))))) ^ 1);
        (*l_20) = ((l_46[1][0] = &g_30) == &g_30);
        (*l_20) = (safe_rshift_func_int64_t_s_s(p_4, (safe_rshift_func_int64_t_s_u(g_10, 15))));
    }
    return (*g_30);
}

static int32_t * func_22(int8_t  p_23)
{ 
    union U0 **l_26 = &g_24;
    int32_t *l_27 = (void*)0;
    int32_t **l_28 = (void*)0;
    int32_t **l_29 = &l_27;
    (*l_26) = g_24;
    (*l_29) = l_27;
    return &g_21;
}

int main (void)
{
    int i, j;
    int print_hash_value = 0;
    platform_main_begin();
    crc32_gentab();
    func_1();
    transparent_crc(g_10, "g_10", print_hash_value);
    transparent_crc(g_15, "g_15", print_hash_value);
    transparent_crc(g_19, "g_19", print_hash_value);
    transparent_crc(g_21, "g_21", print_hash_value);
    transparent_crc(g_44, "g_44", print_hash_value);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            transparent_crc(g_52[i][j], "g_52[i][j]", print_hash_value);
            if (print_hash_value) printf("index = [%d][%d]\n", i, j);

        }
    }
    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);
    return 0;
}
