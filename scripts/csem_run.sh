#!/usr/bin/env bash
# csem_run.sh — run csem_run (the CSem oracle) with the project's paths.
#
#   scripts/csem_run.sh [--libc] [--first] [cerberus-lean options] FILE.c
#
# Sets CSEM_ORACLE/CSEM_RUNTIME (scripts/csem_env.sh), LEAN_ABORT_ON_PANIC=1
# (the fork's driver refuses without it), a private TMPDIR for the Cabs JSON,
# and with --libc the fork's C-library artifacts (scripts/csem_libc_prep.sh,
# stored by input hash in the pin-local build directory). This is the command F1 adds
# to scripts/diff3.py as the CSem oracle.
set -uo pipefail
source "$(dirname "$0")/csem_env.sh"
libc=()
if [ "${1:-}" = "--libc" ]; then
  shift
  args=$("$_repo/scripts/csem_libc_prep.sh") || exit 2
  mapfile -t libc <<< "$args"
fi
scratch=$(mktemp -d "${TMPDIR:-/tmp}/csem-run.XXXXXX")
trap 'rm -rf "$scratch"' EXIT
LEAN_ABORT_ON_PANIC=1 TMPDIR=$scratch "$CSEM_LEAN_BIN" "${libc[@]}" "$@"
