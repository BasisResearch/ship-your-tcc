# Reads `objdump -d -M no-aliases` output; prints the instruction lines that
# are not plain RV64I (+ Zicsr), each prefixed by a class:
#   FAIL C|M|A|F/D|ecall|undecoded  (compressed encodings, the M, A,
#        F, D extensions, ecall, anything objdump could not decode)
#   WARN fence.i (Zifencei: the executable Sail model traps on it; only
#        libtcc1.a's __clear_cache has one)
#   WARN ebreak  (__builtin_trap: stops the machine where reached)
#   WARN csr     (a SYSTEM-opcode word objdump does not name, e.g. libgcc's
#        `csrr a0, vlenb` in vector-unwinding code)
/^ *[0-9a-f]+:\t/ {
  split($0, t, "\t")
  enc = t[2]; gsub(/ /, "", enc)
  ins = t[3]; sub(/^ +/, "", ins)
  split(ins, w, /[ \t]+/); m = w[1]
  why = ""
  if (length(enc) == 4) why = "FAIL C"
  else if (m ~ /^c\./) why = "FAIL C"
  else if (m ~ /^(mul|mulh|mulhsu|mulhu|mulw|div|divu|divw|divuw|rem|remu|remw|remuw)$/) why = "FAIL M"
  else if (m ~ /^(lr|sc|amo)/) why = "FAIL A"
  else if (m == "fence.i") why = "WARN fence.i"
  else if (m ~ /^f/ && m !~ /^fence(\.tso)?$/) why = "FAIL F/D"
  else if (m == "ecall") why = "FAIL ecall"
  else if (m == "ebreak") why = "WARN ebreak"
  else if (m ~ /^(\.insn|\.word|\.short|\.byte|unknown|\(bad\))/) {
    # SYSTEM opcode (0x73) with funct3 != 0: a CSR instruction
    lo = substr(enc, length(enc) - 1, 2)
    f3 = substr(enc, length(enc) - 3, 1)
    if (length(enc) == 8 && (lo == "73" || lo == "f3") && f3 != "0" && f3 != "8") why = "WARN csr"
    else why = "FAIL undecoded"
  }
  else if (ins ~ /(^|[ ,])f(t|s|a)[0-9]+/) why = "FAIL F/D register"
  if (why != "") print why ": " $0
}
