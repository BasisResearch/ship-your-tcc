/* strings and libc number formatting/parsing */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void reverse(char *s) { int i = 0, j = (int)strlen(s) - 1; while (i < j) { char t = s[i]; s[i++] = s[j]; s[j--] = t; } }

static void itoa_(long v, char *buf, int base)
{
    int neg = v < 0, n = 0;
    unsigned long u = neg ? -(unsigned long)v : (unsigned long)v;
    do { int d = (int)(u % base); buf[n++] = d < 10 ? '0' + d : 'a' + d - 10; u /= base; } while (u);
    if (neg) buf[n++] = '-';
    buf[n] = 0;
    reverse(buf);
}

int main(void)
{
    char buf[128], *end;
    const char *nums[] = {"3.14159", "-2.5e-3", "1e308", "0x1p4", "  42abc", "6.02214076e23"};
    int i;
    strcpy(buf, "hello, ");
    strcat(buf, "world");
    printf("%s %zu %d\n", buf, strlen(buf), strcmp(buf, "hello") > 0);
    for (i = 0; i < 6; i++) {
        double d = strtod(nums[i], &end);
        printf("%s -> %.17g rest='%s'\n", nums[i], d, end);
    }
    printf("%ld %ld %d\n", strtol("-123456789012", 0, 10), strtol("ff", 0, 16), atoi("  -77x"));
    itoa_(-1234567890123L, buf, 10); printf("%s\n", buf);
    itoa_(255, buf, 2); printf("%s\n", buf);
    itoa_(0x7fffffffffffffffL, buf, 36); printf("%s\n", buf);
    snprintf(buf, sizeof buf, "%08.3f|%-8g|%+e|%5.1f%%", 3.14159, 2.5, -1234.5678, 99.95);
    printf("%s\n", buf);
    snprintf(buf, sizeof buf, "%x %o %lu %c %5s %.2s", 255u, 8u, 18446744073709551615UL, 'q', "ab", "xyz");
    printf("%s\n", buf);
    {
        char s[] = "The quick brown fox";
        int counts[26] = {0};
        for (i = 0; s[i]; i++) if (s[i] >= 'a' && s[i] <= 'z') counts[s[i] - 'a']++;
        for (i = 0; i < 26; i++) if (counts[i]) printf("%c%d", 'a' + i, counts[i]);
        printf("\n");
        printf("%s\n", strchr(s, 'q'));
        memset(s, '*', 3);
        printf("%s %d\n", s, memcmp(s, "***", 3));
    }
    return 0;
}
