#!/usr/bin/env bash
# Isolated OCaml engine from the SAME shared sources as the generated Lean.
# --path: require a completed, content-addressed build and print its root.
# --build: called by csem_toolchain.sh fork-oracle under shared lock + 12G scope.
# The shared initializer source is overlaid verbatim. Other local frontend
# changes include Lean-only FS seams and are not OCaml generator inputs.
# Pristine upstream and its observations remain untouched.
set -euo pipefail
source "$(dirname "$0")/csem_env.sh"
cd "$_repo"
key() {
  python3 - <<'PY'
import hashlib
from pathlib import Path
h = hashlib.sha256()
for p in [Path('vendor/CSEM_REV'), Path('scripts/csem_fork_oracle.sh')] + [Path('csem/frontend/model/desugaring_init.lem')]:
    if p.is_file():
        h.update(str(p).encode() + b'\0' + p.read_bytes())
print(h.hexdigest())
PY
}
input_hash=$(key)
fork="$_csem_local/fork-oracle/$input_hash"
if [ -f "$fork/.complete" ] && (cd "$fork" && sha256sum -c .artifacts.sha256 >/dev/null 2>&1); then
  printf '%s\n' "$fork"
  exit 0
fi
if [ "${1:-}" != --build ]; then
  echo "csem_fork_oracle: no matching shared-source engine; run scripts/csem_toolchain.sh fork-oracle" >&2
  exit 2
fi
[ "$(git -C "$CSEM_UPSTREAM" rev-parse HEAD)" = "$_csem_up_pin" ]
want=$_csem_lem_pin
have=$(csem_opam lem -v 2>&1)
case "$have" in *"g${want:0:7}"*) ;; *) echo "csem_fork_oracle: generator pin mismatch: $have" >&2; exit 2;; esac
[ ! -e "$fork" ] || { echo "csem_fork_oracle: incomplete build at $fork; retain it for diagnosis" >&2; exit 2; }
mkdir -p "$fork"
tar -C "$CSEM_UPSTREAM" --exclude='./.git' --exclude='./_build' --exclude='./.lake' --exclude='./lean_frontend/.lake' -cf - . | tar -C "$fork" -xf -
cp csem/frontend/model/desugaring_init.lem "$fork/frontend/model/desugaring_init.lem"
[ "$(key)" = "$input_hash" ] || { echo "csem_fork_oracle: inputs changed while copying" >&2; exit 2; }
csem_opam make -C "$fork" prelude-src >&2
(cd "$fork" && csem_opam dune build -j 4 backend/driver/main.exe cerberus-lib.install cerberus.install) >&2
(cd "$fork" && sha256sum _build/default/backend/driver/main.exe > .artifacts.sha256)
printf '%s\n' "$input_hash" > "$fork/.complete"
printf '%s\n' "$fork"
