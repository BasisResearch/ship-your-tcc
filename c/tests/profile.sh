#!/bin/bash
# Flat profile of an image on the Sail Lean emulator: every executed pc is
# counted (--trace-all, aggregated on the fly, nothing stored), split into
# phases at the first execution of given functions, and mapped to the
# enclosing ELF symbol ("[arena]" for the code tcc generated).
#   tests/profile.sh ELF OUTDIR [fn...]
# The phases are: start .. first fn1, fn1 .. first fn2, ... Writes
# OUTDIR/pcs.count (phase pc count) and OUTDIR/profile.txt (per phase: the
# steps, then the functions by self steps).
set -eu
ELF=$1; OUT=$2; shift 2
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
XB=$(ls -d $HOME/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
mkdir -p "$OUT"
"$XB/riscv-none-elf-nm" -n --defined-only "$ELF" | awk '$2 ~ /^[tTwW]$/ {print $1, $3}' > "$OUT/syms"
marks=
for f in "$@"; do
  a=$(awk -v s="$f" '$2==s{print $1; exit}' "$OUT/syms")
  [ -n "$a" ] || { echo "no symbol $f" >&2; exit 1; }
  marks="$marks $(printf %x 0x$a)"
done
"$EMU" "$ELF" --trace-all 2>&1 >"$OUT/raw" | \
  mawk -F'\t' -v marks="$marks" '
    BEGIN { n = split(marks, m, " "); for (i = 1; i <= n; i++) idx[m[i]] = i; ph = 0 }
    $1=="T" { if (($3 in idx) && idx[$3] == ph + 1) ph++; c[ph " " $3]++ }
    END { for (k in c) print k, c[k] }' > "$OUT/pcs.count"
python3 -I - "$OUT" "$@" <<'EOF'
import bisect, sys, collections
out, names = sys.argv[1], ["start"] + sys.argv[2:]
syms = sorted((int(a, 16), n) for a, n in (l.split() for l in open(out + "/syms")))
addrs = [a for a, _ in syms]
prof = collections.defaultdict(collections.Counter)
for l in open(out + "/pcs.count"):
    ph, pc, c = l.split(); ph = int(ph); pc = int(pc, 16); c = int(c)
    if 0x86400000 <= pc < 0x86800000:
        f = "[arena: tcc -run code]"
    else:
        i = bisect.bisect_right(addrs, pc) - 1
        f = syms[i][1] if i >= 0 else "?"
    prof[ph][f] += c
total = sum(sum(p.values()) for p in prof.values())
with open(out + "/profile.txt", "w") as o:
    o.write("total %d\n" % total)
    for ph in sorted(prof):
        p = prof[ph]; t = sum(p.values())
        o.write("\n== phase %d: %s .. %s: %d steps (%.1f%%)\n" % (ph, names[ph],
                names[ph + 1] if ph + 1 < len(names) else "end", t, 100.0 * t / total))
        for f, c in p.most_common(25):
            o.write("%12d %6.2f%%  %s\n" % (c, 100.0 * c / t, f))
EOF
cat "$OUT/profile.txt"
