/* rv64emu: a small, fast RV64I + HTIF interpreter for the tcc -run image.
 *
 * F3's fast reference (DESIGN-COMPILER.md §4.1, PHASES.md F3): it runs the
 * SAME bare-metal image the executable Sail model runs (c/build/base.elf
 * runtime + a per-program .embed), several thousand times faster, and
 * dumps tcc's -run arena at the hand-off to the generated code. It is a
 * testing tool only; the Sail model stays the reference of record, and
 * scripts/model_diff.py spot-checks this emulator against it (step counts
 * of the compile phase and console bytes).
 *
 *   rv64emu ELF [--max-steps N] [--arena-out FILE] [--stop-at-handoff]
 *               [--log-strtorx FILE] [--quiet]
 *
 * --stop-at-handoff: stop before executing tcc_baremetal_enter's first
 * instruction, independently of the generated program's behaviour. Return
 * host status 0 and report exit=- status=handoff (no program exit occurred).
 * compile_steps still counts to tcc_baremetal_run, as in ordinary runs.
 * With this option, the arena is written only at that traced entry marker;
 * its entry/code metadata is saved at tcc_baremetal_run.
 *
 * --log-strtorx: for each call of newlib's _strtorx_l during the compile
 * phase (tcc's strtold for long double literals), one line with the six
 * bytes 10..15 of the destination long double as a 48-bit hex number:
 * _strtorx_l writes only bytes 0..9 (the x87 format), so these are
 * uninitialised stack contents; TccModel takes them as an oracle input.
 *
 * Semantics follow the subset of the Sail model the image uses: RV64I
 * (no M/F/D/A/C, no CSRs), registers and memory zero at reset, PC = ELF
 * entry, and the HTIF mailbox `tohost` (found in the ELF symbol table):
 * device 1 / command 1 writes a console byte, an odd value exits with
 * status value >> 1. Any other instruction stops with exit status 126.
 *
 * Arena dump (--arena-out, without --stop-at-handoff): when the PC first reaches the image's
 * `tcc_baremetal_run` (patches/0002: tcc_run calls it with prog_main in a0
 * and the program's code address/size in a4/a5, after tcc_relocate), the
 * file gets the F3 arena format (also written by the Lean model's driver
 * `tcc_model`, read by scripts/model_diff.py):
 *   "TCCARENA 1\n"
 *   "base <hex>\n"        __tccrun_start
 *   "entry <hex>\n"       a0 (prog_main = runmain.o's _runmain)
 *   "code <hex> <hex>\n"  a4, a5 (the program's own .text: address, size)
 *   "size <hex>\n"        bytes that follow: the arena up to its last
 *                         non-zero byte (the arena starts zeroed)
 *   <size raw bytes>
 * If tcc stops before that point (a compile or link error), no arena is
 * written and the console (tcc's diagnostics) and exit status are the
 * observation; the exit status of rv64emu is the program's.
 * The console goes to stdout; a summary line goes to stderr:
 *   rv64emu: steps=<n> compile_steps=<n|-> exit=<n> status=<exit|limit|trap>
 * The numeric prefix remains compatible with existing ordinary-run parsers.
 * Handoff-only completion instead reports:
 *   rv64emu: steps=<n> compile_steps=<n> exit=- status=handoff
 */
#include <elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAM_BASE 0x80000000ULL
#define RAM_SIZE (128ULL << 20)

static uint8_t *ram;
static uint64_t x[32], pc;

static inline uint8_t *addr(uint64_t a, uint64_t n) {
    if (a < RAM_BASE || a > RAM_BASE + RAM_SIZE || n > RAM_BASE + RAM_SIZE - a) {
        fprintf(stderr, "rv64emu: access fault at 0x%llx (pc 0x%llx)\n",
                (unsigned long long)a, (unsigned long long)pc);
        exit(126);
    }
    return ram + (a - RAM_BASE);
}

static uint64_t sym_tohost, sym_run, sym_enter, sym_tccrun_start, sym_tccrun_end, sym_strtorx;

static void load_elf(const char *path, uint64_t *entry) {
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); exit(2); }
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *b = malloc(n);
    if (fread(b, 1, n, f) != (size_t)n) { perror("read"); exit(2); }
    fclose(f);
    Elf64_Ehdr *eh = (Elf64_Ehdr *)b;
    *entry = eh->e_entry;
    Elf64_Phdr *ph = (Elf64_Phdr *)(b + eh->e_phoff);
    for (int i = 0; i < eh->e_phnum; i++) {
        if (ph[i].p_type != PT_LOAD) continue;
        if (ph[i].p_filesz)
            memcpy(addr(ph[i].p_paddr, ph[i].p_filesz), b + ph[i].p_offset, ph[i].p_filesz);
    }
    Elf64_Shdr *sh = (Elf64_Shdr *)(b + eh->e_shoff);
    for (int i = 0; i < eh->e_shnum; i++) {
        if (sh[i].sh_type != SHT_SYMTAB) continue;
        Elf64_Sym *sy = (Elf64_Sym *)(b + sh[i].sh_offset);
        const char *str = (const char *)(b + sh[sh[i].sh_link].sh_offset);
        size_t ns = sh[i].sh_size / sizeof *sy;
        for (size_t k = 0; k < ns; k++) {
            const char *nm = str + sy[k].st_name;
            if (!strcmp(nm, "tohost")) sym_tohost = sy[k].st_value;
            else if (!strcmp(nm, "tcc_baremetal_run")) sym_run = sy[k].st_value;
            else if (!strcmp(nm, "tcc_baremetal_enter")) sym_enter = sy[k].st_value;
            else if (!strcmp(nm, "__tccrun_start")) sym_tccrun_start = sy[k].st_value;
            else if (!strcmp(nm, "__tccrun_end")) sym_tccrun_end = sy[k].st_value;
            else if (!strcmp(nm, "_strtorx_l")) sym_strtorx = sy[k].st_value;
        }
    }
    free(b);
}

static void dump_arena(const char *path, uint64_t entry, uint64_t code, uint64_t size) {
    if (sym_tccrun_start < RAM_BASE || sym_tccrun_end <= sym_tccrun_start ||
        sym_tccrun_end > RAM_BASE + RAM_SIZE ||
        entry < sym_tccrun_start || entry >= sym_tccrun_end ||
        code < sym_tccrun_start || code >= sym_tccrun_end || size > sym_tccrun_end - code) {
        fprintf(stderr, "rv64emu: invalid arena bounds or handoff metadata\n");
        exit(2);
    }
    uint64_t len = sym_tccrun_end - sym_tccrun_start;
    uint8_t *a = addr(sym_tccrun_start, len);
    while (len && a[len - 1] == 0) len--;
    FILE *f = fopen(path, "wb");
    if (!f) { perror(path); exit(2); }
    int bad = fprintf(f, "TCCARENA 1\nbase %llx\nentry %llx\ncode %llx %llx\nsize %llx\n",
                      (unsigned long long)sym_tccrun_start, (unsigned long long)entry,
                      (unsigned long long)code, (unsigned long long)size,
                      (unsigned long long)len) < 0;
    if (fwrite(a, 1, len, f) != len) bad = 1;
    if (fclose(f) != 0) bad = 1;
    if (bad) {
        fprintf(stderr, "rv64emu: arena write failed: %s\n", path);
        exit(2);
    }
}

#define SEXT(v, b) ((int64_t)((uint64_t)(v) << (64 - (b))) >> (64 - (b)))

int main(int argc, char **argv) {
    const char *elf = NULL, *arena_out = NULL, *strtorx_log = NULL;
    FILE *strtorx_f = NULL;
    uint64_t max_steps = 4000000000ULL;
    int quiet = 0, stop_at_handoff = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--max-steps") && i + 1 < argc) max_steps = strtoull(argv[++i], 0, 0);
        else if (!strcmp(argv[i], "--arena-out") && i + 1 < argc) arena_out = argv[++i];
        else if (!strcmp(argv[i], "--quiet")) quiet = 1;
        else if (!strcmp(argv[i], "--stop-at-handoff")) stop_at_handoff = 1;
        else if (!strcmp(argv[i], "--log-strtorx") && i + 1 < argc) strtorx_log = argv[++i];
        else elf = argv[i];
    }
    if (!elf) { fprintf(stderr, "usage: rv64emu ELF [--max-steps N] [--arena-out F] [--stop-at-handoff]\n"); return 2; }
    ram = calloc(1, RAM_SIZE);
    load_elf(elf, &pc);
    if (stop_at_handoff && (!sym_run || !sym_enter)) {
        fprintf(stderr, "rv64emu: stop-at-handoff requires run and enter symbols\n");
        return 2;
    }
    if (strtorx_log && !(strtorx_f = fopen(strtorx_log, "w"))) { perror(strtorx_log); return 2; }
    uint64_t steps = 0, compile_steps = 0;
    uint64_t arena_entry = 0, arena_code = 0, arena_size = 0;
    int compiled = 0, handoff = 0;
    const char *status = "limit";
    int code = 124;
    static char out[1 << 16];
    size_t outn = 0;
#define PUTC(c) do { out[outn++] = (c); if (outn == sizeof out) { fwrite(out, 1, outn, stdout); outn = 0; } } while (0)
    for (; steps < max_steps; steps++) {
        if (sym_run && pc == sym_run && !compiled) {
            compiled = 1;
            compile_steps = steps;
            arena_entry = x[10]; arena_code = x[14]; arena_size = x[15];
            if (arena_out && !stop_at_handoff)
                dump_arena(arena_out, arena_entry, arena_code, arena_size);
        }
        if (stop_at_handoff && pc == sym_enter) {
            if (!compiled) {
                fprintf(stderr, "rv64emu: enter reached without compiler completion\n");
                return 2;
            }
            if (arena_out) dump_arena(arena_out, arena_entry, arena_code, arena_size);
            handoff = 1; code = 0; goto done;
        }
        if (pc == sym_strtorx && strtorx_f && !compiled) {
            /* gdtoa's _strtorx_l(ptr, s, se, rounding, L, loc) writes 10 bytes
               of the 16-byte long double at L (x87 layout); log the other 6,
               which are whatever the stack held (TccModel's ld oracle) */
            uint8_t *q = addr(x[14] + 10, 6);
            fprintf(strtorx_f, "%02x%02x%02x%02x%02x%02x\n", q[5], q[4], q[3], q[2], q[1], q[0]);
        }
        uint32_t in = *(uint32_t *)addr(pc, 4);
        uint64_t npc = pc + 4;
        unsigned rd = (in >> 7) & 31, rs1 = (in >> 15) & 31, rs2 = (in >> 20) & 31;
        unsigned f3 = (in >> 12) & 7, f7 = in >> 25;
        int64_t immi = (int32_t)in >> 20;
        uint64_t a = x[rs1], b = x[rs2], r = 0;
        int wr = 1;
        switch (in & 0x7f) {
        case 0x37: r = (int64_t)(int32_t)(in & 0xfffff000); break;            /* lui */
        case 0x17: r = pc + (int64_t)(int32_t)(in & 0xfffff000); break;       /* auipc */
        case 0x6f: {                                                           /* jal */
            int64_t imm = ((in >> 31) << 20) | (((in >> 12) & 0xff) << 12) |
                          (((in >> 20) & 1) << 11) | (((in >> 21) & 0x3ff) << 1);
            r = npc; npc = pc + SEXT(imm, 21); break;
        }
        case 0x67: r = npc; npc = (a + immi) & ~1ULL; break;                  /* jalr */
        case 0x63: {                                                           /* branches */
            int64_t imm = ((in >> 31) << 12) | (((in >> 7) & 1) << 11) |
                          (((in >> 25) & 0x3f) << 5) | (((in >> 8) & 0xf) << 1);
            int t;
            switch (f3) {
            case 0: t = a == b; break; case 1: t = a != b; break;
            case 4: t = (int64_t)a < (int64_t)b; break; case 5: t = (int64_t)a >= (int64_t)b; break;
            case 6: t = a < b; break; case 7: t = a >= b; break;
            default: goto illegal;
            }
            if (t) npc = pc + SEXT(imm, 13);
            wr = 0; break;
        }
        case 0x03: {                                                           /* loads */
            uint64_t ea = a + immi;
            switch (f3) {
            case 0: r = (int8_t)*addr(ea, 1); break;
            case 1: { int16_t v; memcpy(&v, addr(ea, 2), 2); r = v; break; }
            case 2: { int32_t v; memcpy(&v, addr(ea, 4), 4); r = v; break; }
            case 3: memcpy(&r, addr(ea, 8), 8); break;
            case 4: r = *addr(ea, 1); break;
            case 5: { uint16_t v; memcpy(&v, addr(ea, 2), 2); r = v; break; }
            case 6: { uint32_t v; memcpy(&v, addr(ea, 4), 4); r = v; break; }
            default: goto illegal;
            }
            break;
        }
        case 0x23: {                                                           /* stores */
            uint64_t ea = a + (((int64_t)(int32_t)in >> 25) * 32 + (int64_t)((in >> 7) & 31));
            unsigned n = 1u << f3;
            if (f3 > 3) goto illegal;
            memcpy(addr(ea, n), &b, n);
            if (ea == sym_tohost && n == 8) {
                if (b & 1) {
                    if ((b >> 48) == 0) { code = (int)(uint32_t)(b >> 1); status = "exit"; steps++; goto done; }
                }
                if ((b >> 56) == 1 && ((b >> 48) & 0xff) == 1) PUTC((char)(b & 0xff));
                memset(addr(ea, 8), 0, 8);
            }
            wr = 0; break;
        }
        case 0x13: {                                                           /* op-imm */
            unsigned sh = (in >> 20) & 63;
            switch (f3) {
            case 0: r = a + immi; break;
            case 1: r = a << sh; break;
            case 2: r = (int64_t)a < immi; break;
            case 3: r = a < (uint64_t)immi; break;
            case 4: r = a ^ immi; break;
            case 5: r = (in >> 30) & 1 ? (uint64_t)((int64_t)a >> sh) : a >> sh; break;
            case 6: r = a | immi; break;
            case 7: r = a & immi; break;
            }
            break;
        }
        case 0x1b: {                                                           /* op-imm-32 */
            unsigned sh = (in >> 20) & 31;
            switch (f3) {
            case 0: r = (int64_t)(int32_t)(a + immi); break;
            case 1: r = (int64_t)(int32_t)((uint32_t)a << sh); break;
            case 5: r = (in >> 30) & 1 ? (int64_t)((int32_t)a >> sh)
                                      : (int64_t)(int32_t)((uint32_t)a >> sh); break;
            default: goto illegal;
            }
            break;
        }
        case 0x33:                                                             /* op */
            if (f7 == 1) goto illegal;
            switch (f3) {
            case 0: r = f7 == 0x20 ? a - b : a + b; break;
            case 1: r = a << (b & 63); break;
            case 2: r = (int64_t)a < (int64_t)b; break;
            case 3: r = a < b; break;
            case 4: r = a ^ b; break;
            case 5: r = f7 == 0x20 ? (uint64_t)((int64_t)a >> (b & 63)) : a >> (b & 63); break;
            case 6: r = a | b; break;
            case 7: r = a & b; break;
            }
            break;
        case 0x3b:                                                             /* op-32 */
            if (f7 == 1) goto illegal;
            switch (f3) {
            case 0: r = (int64_t)(int32_t)(f7 == 0x20 ? a - b : a + b); break;
            case 1: r = (int64_t)(int32_t)((uint32_t)a << (b & 31)); break;
            case 5: r = f7 == 0x20 ? (int64_t)((int32_t)a >> (b & 31))
                                   : (int64_t)(int32_t)((uint32_t)a >> (b & 31)); break;
            default: goto illegal;
            }
            break;
        case 0x0f: wr = 0; if (f3 != 0) goto illegal; break;                   /* fence */
        default:
        illegal:
            fprintf(stderr, "rv64emu: illegal instruction 0x%08x at 0x%llx\n", in,
                    (unsigned long long)pc);
            code = 126; status = "trap"; goto done;
        }
        if (wr && rd) x[rd] = r;
        pc = npc;
    }
done:
    fwrite(out, 1, outn, stdout);
    fflush(stdout);
    if (strtorx_f) fclose(strtorx_f);
    if (!quiet) {
        if (handoff)
            fprintf(stderr, "rv64emu: steps=%llu compile_steps=%llu exit=- status=handoff\n",
                    (unsigned long long)steps, (unsigned long long)compile_steps);
        else if (compiled)
            fprintf(stderr, "rv64emu: steps=%llu compile_steps=%llu exit=%d status=%s\n",
                    (unsigned long long)steps, (unsigned long long)compile_steps, code, status);
        else
            fprintf(stderr, "rv64emu: steps=%llu compile_steps=- exit=%d status=%s\n",
                    (unsigned long long)steps, code, status);
    }
    return code;
}
