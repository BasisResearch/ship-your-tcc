#!/bin/bash
# Who calls what, and when: trace the entry pcs of some functions (names
# given) on the Sail Lean emulator and summarise, per function, the number
# of calls, and per (function, caller) the number of calls and the summed
# third argument (a2: the length for memcpy/memset/memmove).
#   tests/callers.sh ELF OUTDIR fn...
# The trace rows (OUTDIR/trace) also give the step of every call, so phase
# boundaries can be read off them (first/last call of a function).
set -eu
ELF=$1; OUT=$2; shift 2
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
XB=$(ls -d $HOME/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
mkdir -p "$OUT"
"$XB/riscv-none-elf-nm" -n --defined-only "$ELF" | awk '$2 ~ /^[tTwW]$/ {print $1, $3}' > "$OUT/syms"
: > "$OUT/pcs"
for f in "$@"; do
  a=$(awk -v s="$f" '$2==s{print $1; exit}' "$OUT/syms")
  if [ -n "$a" ]; then echo "0x$a" >> "$OUT/pcs"; echo "$a $f" >> "$OUT/names"; else echo "not found (inlined?): $f"; fi
done
"$EMU" "$ELF" --trace-pcs "$OUT/pcs" > "$OUT/raw" 2> "$OUT/trace" || true
python3 -I - "$OUT" <<'EOF'
import bisect, sys, collections
out = sys.argv[1]
syms = sorted((int(a, 16), n) for a, n in (l.split() for l in open(out + "/syms")))
addrs = [a for a, _ in syms]
def fn(pc):
    i = bisect.bisect_right(addrs, pc) - 1
    return syms[i][1] if i >= 0 else "?"
names = dict((int(a, 16), n) for a, n in (l.split() for l in open(out + "/names")))
calls = collections.Counter(); by = collections.Counter(); arg = collections.Counter()
first = {}; last = {}
for l in open(out + "/trace"):
    t = l.rstrip("\n").split("\t")
    if t[0] != "T": continue
    step, pc = int(t[1]), int(t[2], 16)
    ra, a2 = int(t[4], 16), int(t[3 + 12], 16)   # xN is t[3+N]
    f = names.get(pc, hex(pc))
    calls[f] += 1
    by[(f, fn(ra))] += 1; arg[(f, fn(ra))] += a2
    first.setdefault(f, step); last[f] = step
with open(out + "/callers.txt", "w") as o:
    for f, c in calls.most_common():
        o.write("%s: %d calls, steps %d..%d\n" % (f, c, first[f], last[f]))
        for (g, h), c2 in sorted(by.items(), key=lambda x: -x[1]):
            if g == f and c2 * 50 >= c:
                o.write("    %8d  from %-28s a2 sum %d\n" % (c2, h, arg[(g, h)]))
EOF
cat "$OUT/callers.txt"
