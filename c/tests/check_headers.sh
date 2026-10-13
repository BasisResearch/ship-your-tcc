#!/bin/bash
# check_headers.sh GCC ORIG_INC STRIPPED_INC [files.c...]
# The image embeds comment-stripped copies of newlib's headers
# (src/strip_headers.py). Check that gcc, given each file (default:
# tests/run/*.c and every top-level header that compiles on its own),
# preprocesses it to the same token stream (whitespace collapsed) and the
# same macro set (-dM) with the stripped tree as with the original.
set -u
GCC=$1; ORIG=$2; STRIP=$3; shift 3
cd "$(dirname "$0")/.."
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
files=("$@")
if [ ${#files[@]} -eq 0 ]; then
  files=(tests/run/*.c)
  for h in $(cd "$ORIG" && ls *.h); do
    printf '#include <%s>\n' "$h" > "$T/h_$h.c"
    "$GCC" -march=rv64i -mabi=lp64 -fsyntax-only "$T/h_$h.c" 2>/dev/null && files+=("$T/h_$h.c")
  done
fi
norm() { if [ "$1" = "-E -P" ]; then sed 's/^[[:space:]]*//' | tr -s ' \t\n' '   '; else sed 's/[[:space:]]\+/ /g' | LC_ALL=C sort; fi; }
bad=0
for f in "${files[@]}"; do
  for mode in "-E -P" "-E -dM"; do
    a=$("$GCC" -march=rv64i -mabi=lp64 $mode -nostdinc -isystem "$(dirname "$("$GCC" -print-libgcc-file-name)")/include" -isystem "$ORIG" "$f" 2>&1 | norm "$mode")
    b=$("$GCC" -march=rv64i -mabi=lp64 $mode -nostdinc -isystem "$(dirname "$("$GCC" -print-libgcc-file-name)")/include" -isystem "$STRIP" "$f" 2>&1 | norm "$mode")
    if [ "$a" != "$b" ]; then echo "DIFFER ($mode): $f"; bad=1; fi
  done
done
echo "checked ${#files[@]} files: $([ $bad = 0 ] && echo identical || echo DIFFERENCES)"
exit $bad
