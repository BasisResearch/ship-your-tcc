#!/usr/bin/env bash
# Isolated OCaml engine from the SAME shared sources as the generated Lean.
# --path: require a completed, content-addressed build and print its root.
# --build: called by csem_toolchain.sh fork-oracle under shared lock + 12G scope.
# The explicit semantic source manifest is overlaid verbatim. Other local frontend
# changes include Lean-only FS seams and are not OCaml generator inputs.
# Pristine upstream and its observations remain untouched.
set -euo pipefail
source "$(dirname "$0")/csem_env.sh"
cd "$_repo"
# Shared semantic repairs only. Keep this list explicit: unrelated local FS
# sources contain Lean-only seams and must not enter the OCaml overlay.
shared_sources=(
  csem/frontend/model/ctype.lem
  csem/frontend/model/cabs_to_ail.lem
  csem/frontend/model/cabs_to_ail_effect.lem
  csem/frontend/model/ail/genTyping.lem
  csem/frontend/model/desugaring_init.lem
)
# The shared Mem API already passes the complete layout table. Both
# handwritten adapters must honor it even before the runtime table is set.
adapter_sources=(csem/lean_frontend/CerbMem.lean csem/lean_frontend/CerberusImpl.lean)
adapter_patches=(patches/csem/0001-union-layout-tags.patch patches/csem/0002-gnu-typedef-alignment.patch patches/csem/0003-gnu-aggregate-layout.patch patches/csem/0004-tag-layout-arity.patch)
# Pin every patched handwritten adapter, including arity-only non-concrete
# shims. Their inclusion in the manifest does not claim layout support in
# symbolic/CHERI backends; the differential engine is the concrete backend.
mapfile -t patched_adapter_paths < <(sed -n 's#^+++ b/##p' "${adapter_patches[@]}" | sort -u)
key() {
  python3 - "${shared_sources[@]}" "${adapter_sources[@]}" "${adapter_patches[@]}" <<'PY'
import hashlib, sys
from pathlib import Path
h = hashlib.sha256()
for p in [Path('vendor/CSEM_REV'), Path('scripts/csem_fork_oracle.sh')] + [Path(p) for p in sys.argv[1:]]:
    h.update(str(p).encode() + b'\0' + p.read_bytes())
print(h.hexdigest())
PY
}
input_hash=$(key)
fork="$_repo/build/fork-oracle/$input_hash"
if [ -f "$fork/.complete" ] && (cd "$fork" && sha256sum -c .artifacts.sha256 .overlay.sha256 >/dev/null 2>&1); then
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
for source in "${shared_sources[@]}"; do
  cp "$source" "$fork/${source#csem/}"
done
for adapter_patch in "${adapter_patches[@]}"; do
  patch -d "$fork" -p1 --fuzz=0 < "$adapter_patch" >&2
done
printf '%s\n' "${shared_sources[@]}" > "$fork/.shared-sources"
sha256sum "${adapter_sources[@]}" "${adapter_patches[@]}" > "$fork/.adapter-inputs.sha256"
sha256sum "${shared_sources[@]}" > "$fork/.shared-sources.sha256"
(cd "$fork" && for source in "${shared_sources[@]}"; do sha256sum "${source#csem/}"; done > .overlay.sha256
 sha256sum "${patched_adapter_paths[@]}" >> .overlay.sha256)
[ "$(key)" = "$input_hash" ] || { echo "csem_fork_oracle: inputs changed while copying" >&2; exit 2; }
csem_opam make -C "$fork" prelude-src >&2
(cd "$fork" && csem_opam dune build -j 4 backend/driver/main.exe cerberus-lib.install cerberus.install) >&2
(cd "$fork" && sha256sum _build/default/backend/driver/main.exe > .artifacts.sha256)
[ "$(key)" = "$input_hash" ] || { echo "csem_fork_oracle: inputs changed during build" >&2; exit 2; }
printf '%s\n' "$input_hash" > "$fork/.complete"
printf '%s\n' "$fork"
