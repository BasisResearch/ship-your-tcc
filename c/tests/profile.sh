#!/bin/bash
# Flat profile of an image on the Sail Lean emulator: every executed pc is
# counted (--trace-all, aggregated on the fly, nothing stored), then mapped
# to the enclosing ELF symbol, or to "arena" for the code tcc generated.
#   tests/profile.sh ELF OUTDIR [STOP_PC]
# Writes OUTDIR/pcs.count (pc count), OUTDIR/profile.txt (per function,
# sorted), and, with STOP_PC (e.g. tcc_baremetal_run's address), only
# counts steps before the first time STOP_PC executes (the compile phase).
set -eu
ELF=$1; OUT=$2; STOP=${3:-}
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
XB=$(ls -d $HOME/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
mkdir -p "$OUT"
stop=${STOP#0x}
"$EMU" "$ELF" --trace-all 2>&1 >"$OUT/raw" | \
  mawk -F'\t' -v stop="$stop" '
    $1=="T" { if ($3 == stop) { done = 1 } if (!done) c[$3]++ }
    END { for (k in c) print k, c[k] }' > "$OUT/pcs.count"
"$XB/riscv-none-elf-nm" -n --defined-only "$ELF" | awk '$2 ~ /^[tTwW]$/ {print $1, $3}' > "$OUT/syms"
python3 -I - "$OUT" <<'EOF'
import bisect, sys, collections
out = sys.argv[1]
syms = [(int(a, 16), n) for a, n in (l.split() for l in open(out + "/syms"))]
syms.sort()
addrs = [a for a, _ in syms]
prof = collections.Counter()
total = 0
for l in open(out + "/pcs.count"):
    pc, c = l.split(); pc = int(pc, 16); c = int(c)
    total += c
    if pc >= 0x86400000 and pc < 0x86800000:
        prof["[arena: tcc -run code]"] += c; continue
    i = bisect.bisect_right(addrs, pc) - 1
    prof[syms[i][1] if i >= 0 else "?"] += c
with open(out + "/profile.txt", "w") as f:
    f.write("total %d\n" % total)
    for n, c in prof.most_common():
        f.write("%12d %6.2f%%  %s\n" % (c, 100.0 * c / total, n))
EOF
head -40 "$OUT/profile.txt"
