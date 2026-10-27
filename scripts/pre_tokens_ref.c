/* Internal observation harness, compiled by build_pre_tokens_ref.py against
 * pristine TinyCC 43c7708 + this repository's patches. LGPL-2.1-or-later,
 * like the tcc translation unit it includes. No model-derived token data. */
#define USING_GLOBALS
#include "tcc.h"

static const char *raw_root;
static int raw_capture_pragmas = 1;
static TokenString *raw_pending;
static unsigned long long raw_next_event = 1;
static unsigned char *raw_seen;
static size_t raw_seen_size;
static int raw_open(const char *p, int flags, ...)
{
    char path[8192];
    if (!raw_root || p[0] != '/') { errno = ENOENT; return -1; }
    if (snprintf(path, sizeof path, "%s%s", raw_root, p) >= sizeof path) {
        errno = ENAMETOOLONG; return -1;
    }
    return open(path, flags, 0600);
}
static time_t raw_time(time_t *p) { if (p) *p = 0; return 0; }
#define open raw_open
#define time raw_time
#include "libtcc.c"
#undef open
#undef time

static FILE *raw_output;
static unsigned raw_count;
static void raw_bytes(int kind, const unsigned char *p, size_t n)
{
    size_t i;
    if (raw_pending) {
        CValue cv;
        unsigned char *b = tcc_malloc(n + 11);
        unsigned long long id = raw_next_event++;
        if (!b) exit(2);
        memset(&cv, 0, sizeof cv);
        b[0] = 0; b[1] = kind;
        for (i = 0; i < 8; ++i) b[2+i] = id >> (8*i);
        memcpy(b+10, p, n); b[n+10] = 0;
        cv.str.data = (char *)b; cv.str.size = n+11;
        tok_str_add2(raw_pending, TOK_PPSTR, &cv);
        tcc_free(b);
        return;
    }
    fprintf(raw_output, "%c %zu ", kind, n);
    for (i = 0; i < n; ++i) fprintf(raw_output, "%02x", p[i]);
    fputc('\n', raw_output);
    ++raw_count;
}
static void raw_marker(int kind) { raw_bytes(kind, (const unsigned char *)"", 0); }
static void raw_observe(void)
{
    const char *p = get_tok_str(tok, &tokc);
    int kind = 'O';
    static const char *puncts[] = {
        "[", "]", "(", ")", "{", "}", ".", "->", "++", "--", "&", "*", "+", "-",
        "~", "!", "/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|",
        "&&", "||", "?", ":", ";", "...", "=", "*=", "/=", "%=", "+=", "-=", "<<=",
        ">>=", "&=", "^=", "|=", ",", "#", "##", "<:", ":>", "<%", "%>", "%:", "%:%:"
    };
    size_t i;
    if (tok == TOK_PPSTR && tokc.str.size >= 11 && !tokc.str.data[0]) {
        unsigned long long id = 0;
        for (i = 0; i < 8; ++i) id |= (unsigned long long)(unsigned char)tokc.str.data[2+i] << (8*i);
        if (!id || id >= raw_next_event) exit(2);
        if (id >= raw_seen_size) {
            size_t old = raw_seen_size;
            raw_seen_size = raw_next_event;
            raw_seen = tcc_realloc(raw_seen, raw_seen_size);
            memset(raw_seen+old, 0, raw_seen_size-old);
        }
        if (raw_seen[id]) return;
        raw_seen[id] = 1;
        raw_bytes((unsigned char)tokc.str.data[1], (const unsigned char *)tokc.str.data+10, tokc.str.size-11);
        return;
    }
    if (!p) { fprintf(stderr, "raw observer: missing spelling\n"); exit(2); }
    if (tok == TOK_PPNUM) kind = 'N';
    else if (TOK_IS_STR(tok)) kind = 'S';
    else if (tok == TOK_PPSTR) {
        kind = p[0] == '\'' || ((p[0] == 'L' || p[0] == 'u' || p[0] == 'U') && p[1] == '\'') ? 'C' : 'S';
    } else if (tok >= TOK_IDENT) kind = 'I';
    else for (i = 0; i < sizeof puncts / sizeof *puncts; ++i)
        if (!strcmp(p, puncts[i])) { kind = 'P'; break; }
    raw_bytes(kind, (const unsigned char *)p, strlen(p));
}

int main(int argc, char **argv)
{
    TCCState *s;
    int failed = 0, c;
    if ((argc != 3 && argc != 4) || strcmp(argv[1], "--root") || (argc == 4 && strcmp(argv[3], "--plain"))) {
        fprintf(stderr, "usage: pre_tokens_ref --root DIR\n"); return 2;
    }
    raw_root = argv[2];
    raw_capture_pragmas = argc == 3;
    raw_output = tmpfile();
    if (!raw_output) { perror("tmpfile"); return 2; }
    s = tcc_new();
    s->cversion = 201112;
    s->warn_none = 1;
    s->output_type = TCC_OUTPUT_MEMORY;
    tcc_add_sysinclude_path(s, "/lib/tcc/include");
    tcc_add_sysinclude_path(s, "/include");
    tcc_enter_state(s);
    s->error_set_jmp_enabled = 1;
    puts("TCCPP 1");
    if (setjmp(s->error_jmp_buf)) failed = 1;
    else {
        if (tcc_open(s, "/prog.c") < 0) { fputs("missing /prog.c\n", stderr); return 2; }
        preprocess_start(s, AFF_TYPE_C);
        tccgen_init(s);
        parse_flags = PARSE_FLAG_PREPROCESS;
        for (;;) { next(); if (tok == TOK_EOF) break; raw_observe(); }
    }
    if (failed || s->nb_errors) {
        puts("ERROR refuse 7463635f6572726f72");
    } else {
        if (fflush(raw_output) || fseek(raw_output, 0, SEEK_SET)) return 2;
        while ((c = fgetc(raw_output)) != EOF) if (putchar(c) == EOF) return 2;
        if (ferror(raw_output)) return 2;
        printf("STATE %td %d %d %d\n", s->pack_stack_ptr - s->pack_stack,
               *s->pack_stack_ptr, pp_counter, s->nb_pragma_libs);
        printf("END %u\n", raw_count);
    }
    tccgen_finish(s);
    preprocess_end(s);
    s->error_set_jmp_enabled = 0;
    tcc_exit_state(s);
    tcc_delete(s);
    if (fclose(raw_output) || fflush(stdout) || ferror(stdout)) return 2;
    return failed ? 1 : 0;
}
