#!/usr/bin/env bash
# Prepare the fork's image runtime and matching libc bodies/metadata.
# The ordinary csem_libc_prep.sh keeps the pristine oracle baseline.
set -euo pipefail
source "$(dirname "$0")/csem_env.sh"
cd "$_repo"
out=$_repo/build/csem-image-libc
prefix=$_repo/build/csem-image-prefix
runtime=$prefix/lib/cerberus-lib/runtime
mkdir -p "$out"
exec 9>"$out/.lock"
flock 9
key=$(python3 - "$CSEM_ORACLE" <<'PY'
import hashlib, pathlib, subprocess, sys
h = hashlib.sha256()
paths = subprocess.check_output(['git', 'ls-files', '-z', 'csem/runtime']).split(b'\0')
for raw in sorted(p for p in paths if p):
    p = pathlib.Path(raw.decode())
    h.update(raw + b'\0' + p.read_bytes())
for p in [pathlib.Path(sys.argv[1]), pathlib.Path('scripts/csem_image_libc_prep.sh')]:
    h.update(p.read_bytes())
print(h.hexdigest())
PY
)
if [[ ! -f "$out/stamp" || $(cat "$out/stamp") != "$key" ]]; then
  mkdir -p "$(dirname "$runtime")"
  # This directory contains only this script's generated runtime copy.
  rm -rf "$runtime"
  cp -a csem/runtime "$runtime"
  tus=(ctype stdio stdlib string time utime unistd stat uio internal vfscanf signal)
  sources=()
  for tu in "${tus[@]}"; do sources+=("src/$tu.c"); done
  (
    cd "$runtime/libc"
    "$CSEM_ORACLE" --runtime="$prefix" --nolibc -I include -I include/posix \
      --sequentialise --rewrite -o "$out/libc.co" "${sources[@]}" > "$out/compile.log" 2>&1
    "$CSEM_ORACLE" --runtime="$prefix" --nolibc --pp=core \
      --pp_core_out="$out/libc.core" "$out/libc.co" > "$out/dump.log" 2>&1
    for tu in "${tus[@]}"; do
      "$CSEM_ORACLE" --runtime="$prefix" --nolibc --cabs-json \
        -I include -I include/posix "src/$tu.c" > "$out/libc_$tu.json"
    done
  )
  {
    printf '%s\n' --runtime "$prefix" --libc "$out/libc.core"
    for tu in "${tus[@]}"; do printf '%s\n' --libc-tu "$out/libc_$tu.json"; done
  } > "$out/args"
  sha256sum "$out/libc.core" "$out"/libc_*.json > "$out/artifacts.sha256"
  printf '%s\n' "$key" > "$out/stamp"
fi
cat "$out/args"
