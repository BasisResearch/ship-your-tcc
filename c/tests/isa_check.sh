#!/bin/sh
# isa_check.sh OBJDUMP [--binary VMA] -- FILE...
# Disassembles every code section of each FILE (ELF executables, objects or
# archives; with --binary, raw RV64 code loaded at VMA) and fails if any
# instruction is outside plain RV64I + Zicsr: the M, A, F, D or C
# extensions, fence.i (Zifencei: the executable Sail model traps on it),
# ecall, or a word objdump could not decode. ebreak (__builtin_trap) and
# unnamed CSR accesses are reported as warnings (isa_filter.awk).
set -u
OBJDUMP=$1; shift
bin=
if [ "$1" = "--binary" ]; then bin=$2; shift 2; fi
[ "$1" = "--" ] && shift
bad=0
for f in "$@"; do
  if [ -n "$bin" ]; then
    dis=$("$OBJDUMP" -D -b binary -m riscv:rv64 --adjust-vma="$bin" -M no-aliases "$f" 2>/dev/null)
  else
    dis=$("$OBJDUMP" -d -M no-aliases "$f" 2>/dev/null)
  fi
  out=$(printf '%s\n' "$dis" | awk -f "$(dirname "$0")/isa_filter.awk")
  total=$(printf '%s\n' "$dis" | grep -cE '^ *[0-9a-f]+:	')
  nf=$(printf '%s\n' "$out" | grep -c '^FAIL' || true)
  nw=$(printf '%s\n' "$out" | grep -c '^WARN' || true)
  if [ "$nf" -gt 0 ]; then
    echo "FAIL $f: $nf of $total instructions outside RV64I ($nw warnings)"
    printf '%s\n' "$out" | grep '^FAIL' | head -20
    bad=1
  else
    echo "ok   $f: $total instructions, all RV64I ($nw warnings: $(printf '%s\n' "$out" | grep '^WARN' | cut -d: -f1 | sort | uniq -c | tr -s ' ' | tr '\n' ';'))"
  fi
done
exit $bad
