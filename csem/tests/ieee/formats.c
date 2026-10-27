/* Format retention: arithmetic, explicit casts, function args/returns,
   conditional conversions, object stores/loads and integer conversion. */
static float sf(float x) { return x + 1.0f; }
static long double qf(long double x) { return x + 1.0L; }
int main(void) {
  float f = 16777216.0f;
  long double q = 0x1p100L;
  unsigned long long n = 9007199254740993ULL;
  float direct = (float)4611686293305294849LL;
  float narrow = (float)0x1.0000010000000000000000000001p0L;
  long double fromint = (long double)n;
  long double mixed = q + n;
  long double cond = 1 ? q + 1.0L : 1.0;
  if (sizeof f != 4 || sizeof q != 16) return 1;
  if (sf(f) != f || f + 1.0f != f) return 2;
  if (qf(q) - q != 1.0L) return 3;
  if (fromint - 9007199254740992.0L != 1.0L) return 4;
  if (mixed - q != 9007199254740993.0L || cond - q != 1.0L) return 5;
  if (direct != 0x1.000002p62f) return 6;
  if (narrow != 0x1.000002p0f) return 7;
  return 0;
}
