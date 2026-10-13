#!/bin/bash
# Oracle 2: Certora's Lean Clight interpreter on `clightgen -lean`'s output.
#
#   scripts/run_lean_clight.sh [options] prog.c [-- extra clightgen flags]
#
#   --keep        keep the work dir (prints its path on stderr)
#   --stats       print steps / load / run times on stderr
#   --fuel N      stop after N Clight steps (default: unbounded; use timeout(1))
#   --lean FILE   also copy the generated Lean file to FILE
#
# stdout : the bytes the program printed (printf/putchar/puts via the
#          runner's external-call model, see lean/ClightRun/ClightRun/Run.lean)
# exit   : the program's exit status (low 8 bits), or
#          120 clightgen rejected the program
#          124 out of fuel, 125 harness error (generated file did not elaborate, ...)
#          126 stuck: undefined behaviour, or a construct the interpreter does
#              not execute (unknown external/builtin, volatile, ...)
#          127 outside the printf model (%p, %n, long double, ...)
#          A one-line "[clight_run] ..." / "[run_lean_clight] ..." reason goes to stderr.
set -uo pipefail
here=$(cd "$(dirname "$0")" && pwd)
. "$here/certora_env.sh"

keep=0; stats=(); fuel=(); leanout=""; prog=""; extra=()
while [ $# -gt 0 ]; do
  case "$1" in
    --keep) keep=1 ;;
    --stats) stats=(--stats) ;;
    --fuel) fuel=(--fuel "$2"); shift ;;
    --lean) leanout="$2"; shift ;;
    --) shift; extra=("$@"); break ;;
    *) prog="$1" ;;
  esac
  shift
done
[ -n "$prog" ] || { echo "usage: $0 [--keep] [--stats] [--fuel N] prog.c [-- flags]" >&2; exit 125; }
[ -x "$CLIGHT_RUN" ] || { echo "[run_lean_clight] $CLIGHT_RUN not built (scripts/build_certora_lean.sh runner)" >&2; exit 125; }

work=$(mktemp -d "${TMPDIR:-/tmp}/leanclight.XXXXXX")
if [ $keep -eq 0 ]; then trap 'rm -rf "$work"' EXIT; else echo "[run_lean_clight] work dir: $work" >&2; fi
srcdir=$(cd "$(dirname "$prog")" && pwd)
# copy to a fixed basename so the generated namespace is always `Prog`
cp "$prog" "$work/prog.c"
t0=$(date +%s%N)
# shellcheck disable=SC2086
if ! "$CLIGHTGEN" -lean -normalize -fprintf-builtin $CERTORA_CFLAGS -I"$srcdir" "${extra[@]}" \
      -o "$work/Prog.lean" "$work/prog.c" 2> "$work/clightgen.err"; then
  echo "[run_lean_clight] clightgen rejected the program: $(head -3 "$work/clightgen.err" | tr '\n' ' ')" >&2
  exit 120
fi
t1=$(date +%s%N)
[ -n "$leanout" ] && cp "$work/Prog.lean" "$leanout"
"$CLIGHT_RUN" "$work/Prog.lean" "${stats[@]}" "${fuel[@]}"
rc=$?
if [ ${#stats[@]} -gt 0 ]; then
  echo "[run_lean_clight] clightgen_ms=$(( (t1 - t0) / 1000000 )) total_ms=$(( ($(date +%s%N) - t0) / 1000000 ))" >&2
fi
exit $rc
