#!/bin/bash
# Oracle 1: CompCert's reference interpreter (`ccomp -interp`, CompCert C / Csem).
#
#   scripts/run_ccomp_interp.sh [--stats] prog.c [-- extra ccomp flags]
#
# Flags always passed: -interp -quiet $CERTORA_CFLAGS (default
# "-fstruct-passing", see scripts/certora_env.sh) -I<dir of prog.c>.
#
# stdout : the bytes the program printed (only printf exists in -interp)
# exit   : the program's exit status (low 8 bits of main's result), or
#          120 ccomp rejected the program (parse/type error, unsupported feature)
#          126 stuck: undefined behaviour, or an external other than printf
#              (the interpreter's "Stuck state ..." diagnosis goes to stderr)
set -uo pipefail
here=$(cd "$(dirname "$0")" && pwd)
. "$here/certora_env.sh"

stats=0; prog=""; extra=()
while [ $# -gt 0 ]; do
  case "$1" in
    --stats) stats=1 ;;
    --) shift; extra=("$@"); break ;;
    *) prog="$1" ;;
  esac
  shift
done
[ -n "$prog" ] || { echo "usage: $0 [--stats] prog.c [-- flags]" >&2; exit 125; }
work=$(mktemp -d "${TMPDIR:-/tmp}/ccinterp.XXXXXX")
trap 'rm -rf "$work"' EXIT
srcdir=$(cd "$(dirname "$prog")" && pwd)
t0=$(date +%s%N)
# shellcheck disable=SC2086
"$CCOMP" -interp -quiet $CERTORA_CFLAGS -I"$srcdir" "${extra[@]}" "$prog" \
  > "$work/out" 2> "$work/err"
rc=$?
t1=$(date +%s%N)
if grep -qE "error(s)? detected|^ccomp: error" "$work/err"; then
  cat "$work/out"
  echo "[run_ccomp_interp] ccomp rejected the program: $(grep -m3 -i error "$work/err" | tr '\n' ' ')" >&2
  exit 120
fi
cat "$work/err" >&2
if [ $rc -eq 126 ] && grep -q "^ERROR: Undefined behavior" "$work/out"; then
  # split the program's output from the interpreter's stuck-state diagnosis,
  # which starts with "Stuck state: " (possibly mid-line)
  python3 -I - "$work/out" <<'EOF'
import sys
data = open(sys.argv[1], 'rb').read()
i = data.rfind(b'Stuck state: ')
if i < 0:
    i = data.rfind(b'ERROR: Undefined behavior')
sys.stdout.buffer.write(data[:i])
sys.stdout.flush()
sys.stderr.buffer.write(b'[run_ccomp_interp] STUCK (undefined behaviour or unsupported external): ' + data[i:].replace(b'\n', b' ')[:2000] + b'\n')
EOF
  rc=126
else
  cat "$work/out"
fi
[ $stats -eq 1 ] && echo "[run_ccomp_interp] total_ms=$(( (t1 - t0) / 1000000 ))" >&2
exit $rc
