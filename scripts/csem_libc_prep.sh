#!/usr/bin/env bash
# csem_libc_prep.sh — pristine oracle libc Core + twelve Cabs metadata TUs.
# Outputs live in a locked, content-addressed subdirectory of OUTDIR
# (default: this pin's worktree-local build directory). Always check the
# upstream Core hash pin; never reuse metadata from a different runtime path.
# Prints csem_run arguments, one per line, after csem/scripts/libc_prep.sh.
set -euo pipefail
source "$(dirname "$0")/csem_env.sh"
base=${1:-$_csem_local/libc}
up=$CSEM_UPSTREAM
cd "$_repo"
key=$(python3 - "$CSEM_ORACLE" "$CSEM_RUNTIME" "$up" <<'PY'
import hashlib, pathlib, shutil, sys
oracle, runtime, upstream = map(pathlib.Path, sys.argv[1:])
h = hashlib.sha256()
# Runtime paths are behaviour: location admission uses exact resolved dirs.
for p in (runtime, upstream):
    h.update(str(p.resolve()).encode() + b'\0')
paths = [oracle, upstream/'tests/libc/libc.core.sha256',
         upstream/'_build/default/runtime/libc/libc.co',
         pathlib.Path('scripts/csem_libc_prep.sh'),
         pathlib.Path('vendor/CSEM_REV'), pathlib.Path(shutil.which('cc'))]
paths += sorted(p for p in (upstream/'runtime/libc').rglob('*') if p.is_file())
paths += sorted(p for p in (runtime/'lib/cerberus-lib/runtime').rglob('*') if p.is_file())
for p in paths:
    h.update(str(p.resolve()).encode() + b'\0' + p.read_bytes())
print(h.hexdigest())
PY
)
out=$base/$key
mkdir -p "$out"
exec 9>"$out/.lock"
flock 9
if [ -f "$out/complete" ] && (cd "$out" && sha256sum -c artifacts.sha256 >/dev/null 2>&1); then
  cat "$out/args"
  exit 0
fi
rm -f "$out/complete"
pin_sha=$(awk '{print $1}' "$up/tests/libc/libc.core.sha256")
"$CSEM_ORACLE" --runtime="$CSEM_RUNTIME" --nolibc --pp=core --pp_core_out="$out/libc.core" \
  "$up/_build/default/runtime/libc/libc.co" >/dev/null
got=$(sha256sum "$out/libc.core" | awk '{print $1}')
[ "$got" = "$pin_sha" ] || { echo "csem_libc_prep: libc.core dump $got differs from the fork's pin $pin_sha" >&2; exit 1; }
args=$out/args
printf '%s\n' --libc "$out/libc.core" > "$args"
for tu in ctype stdio stdlib string time utime unistd stat uio internal vfscanf signal; do
  (cd "$up/runtime/libc" && "$CSEM_ORACLE" --runtime="$CSEM_RUNTIME" --nolibc --cabs-json \
     -I include -I include/posix "src/$tu.c" > "$out/libc_$tu.json")
  printf '%s\n' --libc-tu "$out/libc_$tu.json" >> "$args"
done
(cd "$out" && sha256sum libc.core libc_*.json args > artifacts.sha256)
printf '%s\n' "$key" > "$out/complete"
cat "$args"
