#!/bin/bash
# Usage: scripts/check_isa.sh [-q] <elf|.o|.a>...
#
# Fails (exit 1) if any executable section contains an instruction outside
# RV64I + fence/fence.i (ecall/ebreak are accepted but reported), i.e. any
# M, A, F, D, Q, Zicsr, compressed (C) or undecodable word, printing each
# offender. Robustness:
#   - objdump -d -M no-aliases: every compressed instruction prints with
#     its real "c." mnemonic, and no alias hides a CSR/FP operation;
#   - instructions are checked against a WHITELIST of RV64I mnemonics, so
#     anything unknown (including Zb*, Zicsr, vector) fails;
#   - independently, the encoding length is checked: objdump prints 16-bit
#     parcels as 4 hex digits, 32-bit ones as 8; anything not 8 fails;
#   - all-zero parcels are accepted only as alignment padding, i.e. when
#     followed by objdump's "..." or the next symbol (linkers pad between
#     input sections with zeros); counted as zero-pad, anywhere else BAD;
#   - undecodable words (".insn", ".word", "unimp", "(bad)") are reported
#     as "data-or-unknown": tcc puts no data in .text, so these would mean
#     either data in .text or an unsupported instruction -- both fail;
#   - ELF header: the RVC flag and a non-soft float ABI are reported.
# OBJDUMP/READELF may be overridden; default is the xpack toolchain.
set -uo pipefail
quiet=0
[ "${1:-}" = -q ] && { quiet=1; shift; }
XPACK=${XPACK:-$(ls -d "$HOME"/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin 2>/dev/null | head -1)}
OBJDUMP=${OBJDUMP:-$XPACK/riscv-none-elf-objdump}
READELF=${READELF:-$XPACK/riscv-none-elf-readelf}
[ $# -gt 0 ] || { echo "usage: $0 [-q] <elf|.o|.a>..." >&2; exit 2; }

rc=0
for f in "$@"; do
  if [ ! -f "$f" ]; then echo "$f: no such file"; rc=1; continue; fi
  out=$("$OBJDUMP" -d -M no-aliases "$f" 2>&1) || { echo "$f: objdump failed"; echo "$out" | head -3; rc=1; continue; }
  res=$(awk -v file="$f" '
    BEGIN {
      n = split("lui auipc jal jalr beq bne blt bge bltu bgeu lb lh lw lbu lhu lwu ld sb sh sw sd addi slti sltiu xori ori andi slli srli srai add sub sll slt sltu xor srl sra or and addiw slliw srliw sraiw addw subw sllw srlw sraw fence fence.i fence.tso pause", ok, " ")
      for (i = 1; i <= n; i++) allowed[ok[i]] = 1
      bad = 0; total = 0; note = 0; member = file; sect = "?"
    }
    function flushpad() { if (pend != "") { bad++; printf "BAD  %s [zero word inside code: data-or-unknown]\n", pend; pend = "" } }
    function padok() { if (pend != "") { pad++; pend = "" } }
    /^[ \t]*\.\.\.$/ || /^[0-9a-f]+ <.*>:$/ { padok(); next }
    /^In archive / { padok(); next }
    /:[ \t]+file format / { member = $1; sub(/:$/, "", member); next }
    /^Disassembly of section / { padok(); sect = $4; sub(/:$/, "", sect); next }
    /^[ \t]*[0-9a-f]+:\t/ {
      line = $0
      nf = split(line, F, "\t")
      addr = F[1]; gsub(/[ :]/, "", addr)
      hex = F[2]; gsub(/ /, "", hex)
      mn = (nf >= 3) ? F[3] : ""; gsub(/ /, "", mn)
      ops = (nf >= 4) ? F[4] : ""
      total++
      cat = ""
      flushpad()
      if (hex ~ /^0+$/) { pend = sprintf("%s(%s) %s %s: %s %s %s", file, member, sect, addr, hex, mn, ops); next }
      if (mn == "" || mn ~ /^\.(insn|word|short|byte|2byte|4byte)/ || mn == "unimp" || mn ~ /bad/) cat = "data-or-unknown"
      else if (length(hex) != 8) cat = "C(16-bit)"
      else if (mn ~ /^c\./) cat = "C"
      else if (mn in allowed) cat = ""
      else if (mn == "ecall" || mn == "ebreak") { note++; printf "NOTE %s(%s) %s %s: %s %s %s\n", file, member, sect, addr, hex, mn, ops; next }
      else if (mn ~ /^(mul|div|rem)/) cat = "M"
      else if (mn ~ /^(lr\.|sc\.|amo)/) cat = "A"
      else if (mn ~ /^f/ ) cat = "F/D/Q"
      else if (mn ~ /^csr/) cat = (ops ~ /(fflags|frm|fcsr)/) ? "F(csr)" : "Zicsr"
      else cat = "other"
      if (cat != "") { bad++; printf "BAD  %s(%s) %s %s: %s %s %s [%s]\n", file, member, sect, addr, hex, mn, ops, cat }
    }
    END { padok(); printf "SUMMARY %s insns=%d bad=%d notes=%d zero-pad=%d\n", file, total, bad, note, pad }
  ' <<< "$out")
  hdr=$("$READELF" -h "$f" 2>/dev/null | awk -F: '/Flags/ {print $2}' | sort -u)
  hdrbad=""
  while read -r fl; do
    [ -z "$fl" ] && continue
    v=$(( ${fl%%,*} ))   # e_flags: bit 0 = RVC, bits 1-2 = float ABI (0 = soft)
    (( v & 1 )) && hdrbad="$hdrbad RVC-flag"
    (( v & 6 )) && hdrbad="$hdrbad float-ABI($fl)"
  done <<< "$hdr"
  nbad=$(grep -c '^BAD' <<< "$res")
  if [ "$nbad" -gt 0 ] || [ -n "$hdrbad" ]; then
    rc=1
    grep -E '^(BAD|NOTE)' <<< "$res"
    [ -n "$hdrbad" ] && echo "BAD  $f: ELF header:$hdrbad"
    echo "FAIL $(grep '^SUMMARY' <<< "$res" | cut -d' ' -f2-)"
  else
    [ $quiet = 1 ] || { grep '^NOTE' <<< "$res"; echo "OK   $(grep '^SUMMARY' <<< "$res" | cut -d' ' -f2-)"; }
  fi
done
exit $rc
