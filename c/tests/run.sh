#!/bin/bash
# Phase 1: tcc -run on the executable Sail model.
#   tests/run.sh [files...]        (default tests/run/*.c)
#   EMU=... JOBS=8 FLAGS=1 tests/run.sh
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
XB=$(ls -d $HOME/toolchains/xpack-riscv-none-elf-gcc-15.2.0-*/bin | head -1)
NM=$XB/riscv-none-elf-nm
OBJDUMP=$XB/riscv-none-elf-objdump
OBJCOPY=$XB/riscv-none-elf-objcopy
READELF=$XB/riscv-none-elf-readelf
OUT=build/run
RES=../results/phase1
mkdir -p $OUT $RES
files=("$@"); [ ${#files[@]} -eq 0 ] && files=(tests/run/*.c)

# everything in the image but .embed: section table and contents
runtime_sha() {
  { $READELF -SW "$1" | grep -E '^ +\[ *[0-9]+\]' | grep -v ' \.embed ' | sed 's/^ *\[ *[0-9]*\]//'
    $OBJCOPY -O binary -R .embed "$1" /dev/stdout; } | sha256sum | cut -c1-16
}

for f in "${files[@]}"; do
  n=$(basename $f .c)
  rm -f $OUT/$n.*
  if ! make -s PROG=$f FLAGS=$FLAGS ELF=$OUT/$n.elf $OUT/$n.elf > $OUT/$n.build 2>&1; then
    echo "$n BUILD-FAIL (see $OUT/$n.build)"; continue
  fi
  runtime_sha $OUT/$n.elf > $OUT/$n.rtsha
  # markers
  sym() { $NM $OUT/$n.elf | awk -v s="$1" '$3==s{print "0x"$1}'; }
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
}
export -f run1; export OUT EMU
[ -n "${NORUN:-}" ] || for f in "${files[@]}"; do basename $f .c; done | xargs -P $JOBS -I{} bash -c 'run1 {}'

TSV=../results/phase1.tsv
[ -f $TSV ] || printf 'prog\tresult\texit\thost_exit\ttotal_steps\tcompile_steps\tdump_steps\tprogram_steps\twall_s\tcode_addr\tcode_size\tcode_isa\truntime_sha\n' > $TSV
printf '%-10s %-6s %-5s %-5s %12s %12s %9s %12s %8s %s\n' prog result exit host total compile dump program wall isa
for f in "${files[@]}"; do
  n=$(basename $f .c)
  [ -f $OUT/$n.trace ] || continue
  read prun penter pexit < $OUT/$n.markers
  # T step pc npc x1..x31: xN is field 4+N
  eval $(awk -F'\t' -v r=$prun -v e=$penter -v x=$pexit '
    function h(s) { return sprintf("%d", strtonum("0x" s)) }
    $1=="T" && "0x"$3==r { print "srun=" $2 "; caddr=0x" $18 "; csize=" h($19) ";" }
    $1=="T" && "0x"$3==e { print "senter=" $2 ";" }
    $1=="T" && "0x"$3==x { print "sexit=" $2 "; code=" int(h($19) / 2) % 4294967296 ";" }' $OUT/$n.trace)
  : ${srun:=} ${senter:=} ${sexit:=} ${code:=} ${caddr:=} ${csize:=}
  grep -vE '^(TODO: cancel_reservation|PC = 0x|htif_tohost = 0x|SUCCESS$|FAILURE)' $OUT/$n.raw | grep -v '^@@' > $OUT/$n.stdout
  # the generated code (FLAGS bit 0)
  isa=-
  if grep -q '^@@TCCBM code' $OUT/$n.raw; then
    grep '^@@' $OUT/$n.raw | grep -v '^@@TCCBM' | cut -c3- | tr -d '\n' | xxd -r -p > $OUT/$n.code.bin
    $OBJDUMP -D -b binary -m riscv:rv64 --adjust-vma=$caddr $OUT/$n.code.bin > $OUT/$n.code.dis
    if [ -x ../scripts/check_isa.sh ]; then
      ../scripts/check_isa.sh $OUT/$n.code.bin > $OUT/$n.isa 2>&1 && isa=ok || isa=FAIL
    else
      sh tests/isa_check.sh $OBJDUMP --binary $caddr -- $OUT/$n.code.bin > $OUT/$n.isa 2>&1 && isa=ok || isa=FAIL
    fi
  fi
  hr=$(cat $OUT/$n.host.rc)
  if [ -n "$sexit" ] && cmp -s $OUT/$n.stdout $OUT/$n.host && [ "$code" = "$hr" ]; then r=PASS; else r=FAIL; fi
  total=$([ -n "$sexit" ] && echo $((sexit + 1)) || echo -)
  comp=${srun:--}
  dump=$([ -n "$senter" ] && echo $((senter - srun)) || echo -)
  prog=$([ -n "$sexit" ] && [ -n "$senter" ] && echo $((sexit + 1 - senter)) || echo -)
  wall=$(printf '%.1f' $(cat $OUT/$n.wall))
  printf '%-10s %-6s %-5s %-5s %12s %12s %9s %12s %8s %s\n' $n $r "${code:--}" $hr $total $comp $dump $prog $wall $isa
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' $n $r "${code:--}" $hr $total $comp $dump $prog $wall "${caddr:--}" "${csize:--}" $isa $(cat $OUT/$n.rtsha) >> $TSV
  for x in stdout host raw code.bin code.dis isa; do [ -f $OUT/$n.$x ] && cp $OUT/$n.$x $RES/$n.$x; done
  unset srun senter sexit code caddr csize
done
echo "runtime (all but .embed) sha256 prefixes: $(cat $(for f in "${files[@]}"; do echo $OUT/$(basename $f .c).rtsha; done) 2>/dev/null | sort | uniq -c | tr -s ' ' | tr '\n' ' ')"
