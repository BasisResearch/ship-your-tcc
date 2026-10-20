#!/bin/bash
# Phase 1: tcc -run on the executable Sail model. Modelled on ship-your-lua's
# c/tests/difftest.sh (commit 0a4d80e): the console filter, the host oracle
# and counting steps up to _exit's tohost store with --trace-pcs.
#   tests/run.sh [files...]        (default tests/run/*.c)
#   EMU=... JOBS=8 FLAGS=1 tests/run.sh
#   NORUN=1 tests/run.sh [files...]   re-summarise the last run
# For each program: build the image with it embedded as /prog.c (only the
# .embed section differs: checked by hashing everything else), run it on
# the Sail Lean emulator with the step markers traced (src/main.c), and
# compare stdout and the exit status with the same program compiled by the
# host's cc. Writes ../results/phase1.tsv and ../results/phase1/<name>.*:
#   .stdout    the program's output on Sail (console minus emulator noise
#              and the @@ code-dump lines)
#   .host      the host cc's output, .code.bin/.code.dis the generated code
#              (FLAGS bit 0), .isa the ISA check of it, .raw the console
# Steps: compile = _start .. tcc_baremetal_run, dump = .. tcc_baremetal_enter
# (printing the code), program = .. the tohost store in _exit (inclusive).
set -u
cd "$(dirname "$0")/.."
EMU=${EMU:-$HOME/Documents/code/ship-your-lua/riscv-lean/lean_emulator/.lake/build/bin/lean_riscv_emulator}
JOBS=${JOBS:-8}
FLAGS=${FLAGS:-1}
HOSTCC=${HOSTCC:-cc}
XB=${XB:-$(ls -d $HOME/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)}
NM=$XB/riscv-none-elf-nm
OBJDUMP=$XB/riscv-none-elf-objdump
OBJCOPY=$XB/riscv-none-elf-objcopy
READELF=$XB/riscv-none-elf-readelf
OUT=build/run
RES=../results/phase1
mkdir -p $OUT $RES
failed=0
built=()
files=("$@"); [ ${#files[@]} -eq 0 ] && files=(tests/run/*.c)

# everything in the image but .embed: section table and contents
runtime_sha() {
  $OBJCOPY -O binary -R .embed "$1" "$1.rt.bin" || return 1
  local sections
  sections=$($READELF -SW "$1") || { rm -f "$1.rt.bin"; return 1; }
  # allocated sections but .embed: name type addr size flags (not the
  # file offset, which moves with .embed's size)
  { printf '%s\n' "$sections" | sed -n 's/^ *\[ *[0-9]*\] //p' | awk '$7 ~ /A/ && $1 != ".embed" {print $1, $2, $3, $5, $7}'
    cat "$1.rt.bin"; } | sha256sum | cut -c1-16
  rm -f "$1.rt.bin"
}

# NORUN=1: only summarise the last run's outputs
[ -n "${NORUN:-}" ] || for f in "${files[@]}"; do
  n=$(basename $f .c)
  rm -f $OUT/$n.*
  if ! make -s PROG=$f FLAGS=$FLAGS ELF=$OUT/$n.elf $OUT/$n.elf > $OUT/$n.build 2>&1; then
    echo "$n BUILD-FAIL (see $OUT/$n.build)"; failed=1; continue
  fi
  [ -s $OUT/$n.elf ] || { echo "$n BUILD-FAIL (missing ELF)"; failed=1; continue; }
  built+=("$n")
  # markers
  sym() { printf '0x%x' 0x$($NM $OUT/$n.elf | awk -v s="$1" '$3==s{print $1}'); }
  exitsd=$($OBJDUMP -d --disassemble=_exit $OUT/$n.elf | awk '$3=="sd"{sub(":","",$1); print "0x"$1; exit}')
  printf '%s\n' 0x80000000 $(sym tcc_baremetal_run) $(sym tcc_baremetal_enter) $exitsd > $OUT/$n.pcs
  echo "$(sym tcc_baremetal_run) $(sym tcc_baremetal_enter) $exitsd" > $OUT/$n.markers
  # host oracle
  if $HOSTCC -O0 -w -o $OUT/$n.host.exe $f -lm 2> $OUT/$n.host.err; then
    (cd $OUT && ./$n.host.exe > $n.host 2>/dev/null; echo $? > $n.host.rc)
  else
    echo "host cc failed" > $OUT/$n.host; echo - > $OUT/$n.host.rc
  fi
done

run1() {
  n=$1
  [ -f $OUT/$n.elf ] || return
  s=$(date +%s.%N)
  $EMU $OUT/$n.elf --trace-pcs $OUT/$n.pcs > $OUT/$n.raw 2> $OUT/$n.trace
  echo $? > $OUT/$n.emu.rc
  e=$(date +%s.%N)
  echo "$e - $s" | bc > $OUT/$n.wall
  return 0
}
export -f run1; export OUT EMU
if [ -z "${NORUN:-}" ] && [ ${#built[@]} -gt 0 ]; then
  printf '%s\n' "${built[@]}" | xargs -P $JOBS -I{} bash -c 'run1 {}' || failed=1
fi

TSV=../results/phase1.tsv
[ -f $TSV ] || printf 'prog\tresult\texit\thost_exit\ttotal_steps\tcompile_steps\tdump_steps\tprogram_steps\twall_s\tcode_addr\tcode_size\tcode_isa\truntime_sha\n' > $TSV
printf '%-10s %-6s %-5s %-5s %12s %12s %9s %12s %8s %s\n' prog result exit host total compile dump program wall isa
runtime_ref=
for f in "${files[@]}"; do
  n=$(basename $f .c)
  if [ -z "${NORUN:-}" ] && ! [[ " ${built[*]} " == *" $n "* ]]; then continue; fi
  missing=0
  for ext in elf trace raw markers host host.rc emu.rc wall; do
    [ -f $OUT/$n.$ext ] || { echo "$n RUN-FAIL (missing .$ext)"; missing=1; }
  done
  if [ "$missing" = 1 ]; then failed=1; continue; fi
  if ! runtime_sha $OUT/$n.elf > $OUT/$n.rtsha; then
    echo "$n RUNTIME-FAIL (could not hash image)"; failed=1; continue
  fi
  rtsha=$(cat $OUT/$n.rtsha)
  runtime_ok=1
  if [ -z "$runtime_ref" ]; then runtime_ref=$rtsha;
  elif [ "$rtsha" != "$runtime_ref" ]; then
    echo "$n RUNTIME-FAIL (runtime differs from other images)"; runtime_ok=0
  fi
  read prun penter pexit < $OUT/$n.markers
  # T step pc npc x1..x31: xN is field 4+N
  eval $(awk -F'\t' -v r=$prun -v e=$penter -v x=$pexit '
    function h(s) { return sprintf("%d", strtonum("0x" s)) }
    $1=="T" && "0x"$3=="0x80000000" { print "sstart=" $2 ";" }
    $1=="T" && "0x"$3==r { print "srun=" $2 "; caddr=0x" $18 "; csize=" h($19) ";" }
    $1=="T" && "0x"$3==e { print "senter=" $2 ";" }
    $1=="T" && "0x"$3==x { print "sexit=" $2 "; code=" int(h($19) / 2) % 4294967296 ";" }' $OUT/$n.trace)
  : ${sstart:=} ${srun:=} ${senter:=} ${sexit:=} ${code:=} ${caddr:=} ${csize:=}
  markers_ok=0
  if [[ "$sstart:$srun:$senter:$sexit" =~ ^0:[0-9]+:[0-9]+:[0-9]+$ ]] &&
      (( srun > sstart && senter > srun && sexit >= senter )); then markers_ok=1;
  else echo "$n MARKER-FAIL (need start=0 < compile < enter <= exit)"; fi
  # Trace-mode diagnostics are on stderr. Consume only the initial,
  # size-framed code dump, preserving all subsequent program output.
  console_args=()
  (( FLAGS & 1 )) && console_args=(--code-out "$OUT/$n.code.bin")
  console_ok=1
  python3 -I ../scripts/sail_console.py "$OUT/$n.raw" "${console_args[@]}" > $OUT/$n.stdout || console_ok=0
  # the generated code (FLAGS bit 0)
  isa=-
  if [ "$markers_ok" = 1 ] && [ "$console_ok" = 1 ] && (( FLAGS & 1 )); then
    $OBJDUMP -D -b binary -m riscv:rv64 --adjust-vma=$caddr $OUT/$n.code.bin > $OUT/$n.code.dis
    # both checkers: ours on the raw bytes, the codegen lane's whitelist
    # checker (ELF input) on the bytes wrapped as an executable section
    $OBJCOPY -I binary -O elf64-littleriscv -B riscv:rv64 \
      --rename-section .data=.text,alloc,load,readonly,code,contents $OUT/$n.code.bin $OUT/$n.code.elf
    isa=ok
    [ "$(wc -c < $OUT/$n.code.bin)" = "$csize" ] || isa=FAIL
    sh tests/isa_check.sh $OBJDUMP --binary $caddr -- $OUT/$n.code.bin > $OUT/$n.isa 2>&1 || isa=FAIL
    ../scripts/check_isa.sh $OUT/$n.code.elf >> $OUT/$n.isa 2>&1 || isa=FAIL
  fi
  hr=$(cat $OUT/$n.host.rc)
  isa_ok=1
  if [ "$isa" = FAIL ] || { (( FLAGS & 1 )) && [ "$isa" != ok ]; }; then isa_ok=0; fi
  erc=$(cat $OUT/$n.emu.rc)
  if [ "$markers_ok" = 1 ] && [ "$runtime_ok" = 1 ] && [ "$isa_ok" = 1 ] && [ "$console_ok" = 1 ] &&
      ! grep -q TRACE-FUEL-OUT $OUT/$n.trace &&
      cmp -s $OUT/$n.stdout $OUT/$n.host && [ "$code" = "$hr" ] && [ "$erc" = "$hr" ]; then
    r=PASS
  else r=FAIL; failed=1; fi
  total=$([ -n "$sexit" ] && echo $((sexit + 1)) || echo -)
  comp=${srun:--}
  dump=$([ -n "$senter" ] && echo $((senter - srun)) || echo -)
  prog=$([ -n "$sexit" ] && [ -n "$senter" ] && echo $((sexit + 1 - senter)) || echo -)
  wall=$(printf '%.1f' $(cat $OUT/$n.wall))
  printf '%-10s %-6s %-5s %-5s %12s %12s %9s %12s %8s %s\n' $n $r "${code:--}" $hr $total $comp $dump $prog $wall $isa
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' $n $r "${code:--}" $hr $total $comp $dump $prog $wall "${caddr:--}" "${csize:--}" $isa $(cat $OUT/$n.rtsha) >> $TSV
  for x in stdout host raw code.bin code.dis isa; do [ -f $OUT/$n.$x ] && cp $OUT/$n.$x $RES/$n.$x; done
  unset sstart srun senter sexit code caddr csize
done
echo "runtime (all but .embed) sha256 prefixes: $(cat $(for f in "${files[@]}"; do echo $OUT/$(basename $f .c).rtsha; done) 2>/dev/null | sort | uniq -c | tr -s ' ' | tr '\n' ' ')"

exit "$failed"
