#!/usr/bin/env bash
# csem_run_image.sh — the CSem oracle with the tcc -run image's file system:
# scripts/csem_run.sh --libc with the image's embedded file table as the
# initial state (CerbFS.fs_initial_state_of; DESIGN-COMPILER.md §3.3-3.4).
#
#   scripts/csem_run_image.sh [csem_run options] FILE.c
#
# The table is the one scripts/run_tcc_sail.sh embeds: /prog.c = FILE.c first,
# then c/build/fs.list ($CSEM_IMAGE_FS_LIST; default: the main checkout's,
# which has the built image).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
src=$(readlink -f "${@: -1}")
list=${CSEM_IMAGE_FS_LIST:-$HOME/Documents/code/ship-your-tcc/c/build/fs.list}
[ -s "$list" ] || { echo "csem_run_image: no file list $list (build the image: c/Makefile)" >&2; exit 2; }
args=$("$here/csem_image_libc_prep.sh")
mapfile -t libc <<< "$args"
exec "$here/csem_run.sh" "${libc[@]}" --impl tcc-rv64 --address-space-top 18446744073709551615 --image-fs-file "/prog.c=$src" --image-fs "$list" "${@:1:$#-1}" "$src"
