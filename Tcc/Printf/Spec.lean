import Tcc.Boot.View
import Tcc.Clight.Model
import Vsa.Sim.BlockPilot
import Vsa.Sim.GoodState

/-!
# printf at the machine level: the specification's shape (PHASES `PrintfSpec_Statement`)

In `tcc -run P`, P's `printf` is newlib's (`libc.a`, rv64i/lp64), loaded and
relocated by tcc into its code arena at `0x86400000`. This file states what
one call of it does, in machine terms, for an image described by a
`StdioLayout` (the addresses tcc gave the program's libc; generated per image
by `scripts/gen_printf_layout.py`).

**Line buffering.** In this image `_fstat(1)` reports a character device and
`_isatty(1) = 1`, so the program's `stdout` is line buffered: a call whose
output has no newline returns with the bytes in `stdout`'s buffer, not on the
HTIF console (observed on Sail: `c/tests/p3/linebuf.c` prints nothing and
exits 7, `linebuf_nl.c` prints `42\n`; results/p3/printf-inventory.md). So the
observable a call extends is `stdoutBytes`: the console followed by the bytes
pending in `stdout`'s buffer (`pending`), not the console alone.

* `PrintfEntry L fmtAddr fmt margs c`: `c` is at `printf`'s entry with the
  format string `fmt` (NUL-terminated) at `fmtAddr` in `a0` and the variadic
  words `margs` (at most seven) in `a1..a7` (lp64);
* `PrintfReturn L out n c c'`: `c'` is back at `c`'s return address with
  `a0 = n` (sign-extended), `sp` and the callee-saved registers as in `c`,
  and `stdoutBytes` extended by exactly `out`;
* `PrintfFrame L c c'`: memory changed only in printf's footprint (its stack
  frames below `sp`, newlib's static stdio and allocator state, the heap);
* `PrintfCall`: the three, with a run between.

`PrintfSpec_Statement` (open) is the general specification: from every such
entry in a state newlib's stdio can be in (`StdioReach`) with the code
present (`CodeAt`), for every format string and arguments in the integer
subset (no floating conversion: `IntArgs`), the call returns having appended
`format rd fmt evargs` and returning its length.
-/

namespace Tcc.Printf

open LeanRV64DExecutable Vsa.Machine Vsa.Sim Tcc.Boot CC Tcc.Clight

/-! ## Reading machine memory -/

/-- A byte of machine memory as the model reads it (absent = 0). -/
def rdByte (σ : MState) (a : Nat) : Nat := ((σ.mem[a]?).getD 0).toNat

/-- A little-endian 64-bit word. -/
def rdWord (σ : MState) (a : Nat) : Nat :=
  (List.range 8).foldr (fun j acc => rdByte σ (a + j) + 256 * acc) 0

/-- The C string at `a` (bytes before the NUL), looking at most `fuel` bytes. -/
def machStr (σ : MState) : Nat → Nat → Option (List UInt8)
  | 0, _ => none
  | fuel + 1, a =>
    if rdByte σ a = 0 then some [] else ((rdByte σ a).toUInt8 :: ·) <$> machStr σ fuel (a + 1)

/-! ## Where the program's stdio lives -/

/-- The addresses tcc gave the program's newlib in one image (generated per
image by `scripts/gen_printf_layout.py` from the arena at the cut and the
cross compiler's struct layout; never written by hand). -/
structure StdioLayout where
  /-- `printf`'s entry. -/
  printf : Nat
  /-- `_impure_data`, the program's `struct _reent` (what `_impure_ptr` holds). -/
  reent : Nat
  /-- `offsetof(struct _reent, __cleanup)`: non-zero once `__sinit` ran. -/
  cleanupOff : Nat
  /-- `__sf + sizeof(FILE)`: the `FILE` that `_REENT->_stdout` points to. -/
  stdoutFile : Nat
  /-- `offsetof(FILE, _p)`: the next free byte of the buffer. -/
  pOff : Nat
  /-- `offsetof(FILE, _bf._base)`: the buffer. -/
  baseOff : Nat
  /-- The static data newlib's stdio and allocator may write (half-open
  `(base, len)` ranges in the arena), and the host's `_sbrk` break word. -/
  statics : List (Nat × Nat)
  /-- The heap (`_end` .. `__heap_end`): what `_malloc_r` may hand out. -/
  heap : Nat × Nat
  /-- A bound on the stack printf uses below `sp` (its frame and its callees'). -/
  stack : Nat
  /-- The code the call may execute: the relocated libc/libgcc members in the
  arena and the host system calls, as bytes. -/
  code : List Tcc.Boot.Chunk

/-- What the HTIF console holds, as bytes (one character per HTIF byte). -/
def consoleBytes (σ : MState) : List UInt8 := (output σ).toList.map fun ch => ch.toNat.toUInt8

/-- The bytes `stdout` holds in its buffer, not yet written: none before
`__sinit` ran (`__cleanup` still 0), else `_bf._base .. _p`. -/
def pending (L : StdioLayout) (σ : MState) : List UInt8 :=
  if rdWord σ (L.reent + L.cleanupOff) = 0 then []
  else
    let b := rdWord σ (L.stdoutFile + L.baseOff)
    let p := rdWord σ (L.stdoutFile + L.pOff)
    (List.range (p - b)).map fun i => (rdByte σ (b + i)).toUInt8

/-- **The observable of `stdout`**: the console, then the pending bytes. -/
def stdoutBytes (L : StdioLayout) (σ : MState) : List UInt8 := consoleBytes σ ++ pending L σ

/-! ## Arguments -/

/-- The machine word an event value is passed as (lp64: an `int` in a
register sign-extended; a pointer to a global as its address, from `sym`;
floats are outside the subset). -/
def encArg (sym : Ident → Option Nat) : EventVal → Option (BitVec 64)
  | .EVint i => some (i.signExtend 64)
  | .EVlong i => some i
  | .EVptr_global id ofs => (sym id).map fun a => BitVec.ofNat 64 (a + ofs.toNat)
  | _ => none

/-- The string a `%s` argument names, read from machine memory. -/
def machReader (σ : MState) (sym : Ident → Option Nat) : StrReader :=
  fun id ofs => (sym id).bind fun a => machStr σ (2 ^ 20) (a + ofs.toNat)

/-- No floating-point argument (so `format` takes no `f F e E g G`
conversion: those need an `EVfloat`). The proved-route subset. -/
def IntArgs (evargs : List EventVal) : Prop :=
  ∀ v ∈ evargs, match v with | .EVfloat _ | .EVsingle _ => False | _ => True

/-- No run of more than nine decimal digits (`k`: the digits just seen):
widths and precisions stay below `2^31`, where newlib's `int` accumulation
of them cannot overflow. Conservative: digits in the plain text count too. -/
def digitRunOK : List UInt8 → Nat → Bool
  | [], _ => true
  | c :: cs, k => if isDigitByte c then decide (k < 9) && digitRunOK cs (k + 1) else digitRunOK cs 0

/-- The callee-saved registers of the lp64 ABI, with `sp`, `gp`, `tp`. -/
def calleeSaved : List Nat := [2, 3, 4, 8, 9, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27]

/-! ## One call -/

/-- `c` is at `printf`'s entry with `fmt` at `fmtAddr` and `margs` in `a1..`. -/
structure PrintfEntry (L : StdioLayout) (fmtAddr : Nat) (fmt : List UInt8)
    (margs : List (BitVec 64)) (c : Config) : Prop where
  good : GoodState c.σ
  pc : c.σ.regs.get? Register.PC = some (BitVec.ofNat 64 L.printf)
  fmtReg : gprGet c.σ 10 = some (BitVec.ofNat 64 fmtAddr)
  nargs : margs.length ≤ 7
  args : ∀ i (h : i < margs.length), gprGet c.σ (11 + i) = some margs[i]
  fmtBytes : ∀ i (h : i < fmt.length), rdByte c.σ (fmtAddr + i) = fmt[i].toNat
  fmtNul : rdByte c.σ (fmtAddr + fmt.length) = 0
  nonul : ∀ b ∈ fmt, b ≠ 0
  payload : c.σ.regs.get? Register.htif_payload_writes = some (0#4)
  tick : c.tick < 2

/-- `c'` is `c`'s return from printf, having appended `out` to `stdout` and
returning `n`. -/
structure PrintfReturn (L : StdioLayout) (out : List UInt8) (n : Integers.Int)
    (c c' : Config) : Prop where
  good : GoodState c'.σ
  pc : ∃ ra, gprGet c.σ 1 = some ra ∧ c'.σ.regs.get? Register.PC = some ra
  ret : gprGet c'.σ 10 = some (n.signExtend 64)
  saved : ∀ r ∈ calleeSaved, gprGet c'.σ r = gprGet c.σ r
  out : stdoutBytes L c'.σ = stdoutBytes L c.σ ++ out
  payload : c'.σ.regs.get? Register.htif_payload_writes = some (0#4)
  tick : c'.tick < 2

/-- printf's footprint at entry state `c`: its stack below `sp`, the static
stdio/allocator data and the heap. -/
def Footprint (L : StdioLayout) (sp : Nat) (a : Nat) : Prop :=
  (sp - L.stack ≤ a ∧ a < sp) ∨ inRanges L.statics a = true ∨ (L.heap.1 ≤ a ∧ a < L.heap.2)

/-- Memory outside the footprint is unchanged. -/
def PrintfFrame (L : StdioLayout) (c c' : Config) : Prop :=
  ∃ sp, gprGet c.σ 2 = some sp ∧ ∀ a : Nat, ¬ Footprint L sp.toNat a → c'.σ.mem[a]? = c.σ.mem[a]?

/-- One printf call: entry, a run, the return. -/
structure PrintfCall (L : StdioLayout) (fmtAddr : Nat) (fmt : List UInt8)
    (margs : List (BitVec 64)) (out : List UInt8) (n : Integers.Int) (c c' : Config) : Prop where
  entry : PrintfEntry L fmtAddr fmt margs c
  run : Steps c c'
  ret : PrintfReturn L out n c c'

/-! ## The states stdio can be in -/

/-- The code is present. -/
def CodeAt (L : StdioLayout) (σ : MState) : Prop :=
  ∀ ch ∈ L.code, ∀ k, ch.covers k = true → σ.mem[k]? = some (ch.byte k)

/-- The states newlib's stdio can be in when printf is called: as tcc left
the static data (`first`: the bytes `init` pins, generated from the arena at
the cut, with heap room above the break), after a printf call from such a
state, and after any change outside printf's static data and heap (the
program's own stores; a program that writes the footprint, e.g. by calling
`malloc` itself, is outside). -/
inductive StdioReach (L : StdioLayout) (init : List Tcc.Boot.Chunk) (room : MState → Prop) : MState → Prop where
  | first {σ} : (∀ ch ∈ init, ∀ k, ch.covers k = true → σ.mem[k]? = some (ch.byte k)) →
      room σ → StdioReach L init room σ
  | after {c c' : Config} {fmtAddr fmt margs out n} : StdioReach L init room c.σ →
      PrintfCall L fmtAddr fmt margs out n c c' → PrintfFrame L c c' →
      StdioReach L init room c'.σ
  | frame {σ σ'} : StdioReach L init room σ →
      (∀ a : Nat, (inRanges L.statics a = true ∨ (L.heap.1 ≤ a ∧ a < L.heap.2)) →
        σ'.mem[a]? = σ.mem[a]?) →
      StdioReach L init room σ'

/-- Stack room at `sp` (`c`'s `x2`): the `stack` bytes below it, 16-byte
aligned, apart from the code, the static data and the heap. -/
structure StackRoom (L : StdioLayout) (c : Config) (sp : BitVec 64) : Prop where
  reg : gprGet c.σ 2 = some sp
  big : L.stack ≤ sp.toNat
  align : sp.toNat % 16 = 0
  apart : ∀ a, sp.toNat - L.stack ≤ a → a < sp.toNat →
    inRanges L.statics a = false ∧ (∀ ch ∈ L.code, ch.covers a = false) ∧
      ¬ (L.heap.1 ≤ a ∧ a < L.heap.2)

/-- The bytes printf reads through its arguments (the format string with its
NUL, and every `%s` string a pointer argument names) lie outside its
footprint, so its own writes do not change them. -/
structure ArgsApart (L : StdioLayout) (c : Config) (sp : BitVec 64) (sym : Ident → Option Nat)
    (fmtAddr : Nat) (fmt : List UInt8) (evargs : List EventVal) : Prop where
  fmt : ∀ i ≤ fmt.length, ¬ Footprint L sp.toNat (fmtAddr + i)
  strs : ∀ id ofs s a, EventVal.EVptr_global id ofs ∈ evargs → sym id = some a →
    machReader c.σ sym id ofs = some s → ∀ i ≤ s.length, ¬ Footprint L sp.toNat (a + ofs.toNat + i)

/-- **PHASES `PrintfSpec_Statement` (open)**: from every entry of `printf`
with the code present, in a state stdio can be in, with stack room, for every
format string and arguments of the integer subset (`IntArgs`, at most seven
variadic words, `%s` strings read from memory, widths and precisions of at
most nine digits, output shorter than `2^31` bytes so the count fits printf's
`int`), the strings it reads outside its footprint, the call returns having
appended exactly `format` of them to `stdout` and returning its length, its
writes in its footprint. -/
def PrintfSpec_Statement (L : StdioLayout) (init : List Tcc.Boot.Chunk) (room : MState → Prop) : Prop :=
  ∀ (c : Config) (sp : BitVec 64) (sym : Ident → Option Nat) (fmtAddr : Nat) (fmt : List UInt8)
    (evargs : List EventVal) (margs : List (BitVec 64)) (out : List UInt8),
    CodeAt L c.σ → StdioReach L init room c.σ →
    PrintfEntry L fmtAddr fmt margs c → StackRoom L c sp →
    evargs.mapM (encArg sym) = some margs → IntArgs evargs → digitRunOK fmt 0 = true →
    ArgsApart L c sp sym fmtAddr fmt evargs →
    format (machReader c.σ sym) fmt evargs = some out → out.length < 2 ^ 31 →
    ∃ c', PrintfCall L fmtAddr fmt margs out (Integers.Int.repr out.length) c c' ∧
      PrintfFrame L c c'

end Tcc.Printf
