import CerbIEEE

/-!
# `ieee-test`: differential tests of `CerbIEEE` (F1b, results/csem/ieee-2026-10-27.md)

    ieee-test native N SEED      binary64/binary32 vs Lean's native Float/Float32
    ieee-test vectors FILE       binary128 vectors (gen_vectors f128)
    ieee-test literals FILE      literal vectors (gen_vectors strto)
    ieee-test printf FILE STYLE  printf vectors (gen_vectors printf; STYLE glibc|newlib)
    ieee-test sail FILE          four-column gen_vectors sailc output; exact NaN bits
    ieee-test bench N            µs per operation

Exit status 0 iff every comparison agrees. NaN results are compared as "is a
NaN" (native code and x86 libgcc may propagate payloads and signs); that
every NaN CerbIEEE produces is the canonical one is counted separately.
-/

open CerbIEEE

namespace IEEETest

/-! ## Counters -/

structure Tally where
  name : String
  total : Nat := 0
  nanCases : Nat := 0
  bad : Nat := 0
  shown : Nat := 0

def Tally.add (t : Tally) (ok nanCase : Bool) (msg : Unit → String) : IO Tally := do
  let t := { t with total := t.total + 1, nanCases := t.nanCases + (if nanCase then 1 else 0) }
  if ok then return t
  if t.shown < 8 then IO.println s!"MISMATCH {t.name}: {msg ()}"
  return { t with bad := t.bad + 1, shown := t.shown + 1 }

def Tally.report (t : Tally) : IO Bool := do
  IO.println s!"{t.name}\t{t.total}\tnan:{t.nanCases}\tmismatch:{t.bad}"
  return t.bad == 0

/-! ## A small PRNG (xorshift64) -/

structure Rng where
  s : UInt64

def Rng.next (r : Rng) : UInt64 × Rng :=
  let x := r.s
  let x := x ^^^ (x <<< 13)
  let x := x ^^^ (x >>> 7)
  let x := x ^^^ (x <<< 17)
  (x, ⟨x⟩)

def hex (n : Nat) (digits : Nat) : String :=
  let s := String.ofList (Nat.toDigits 16 n)
  String.ofList (List.replicate (digits - s.length) '0') ++ s

/-! ## Operand generators -/

def edges64 : List UInt64 :=
  [0, 0x8000000000000000, 1, 0x8000000000000001, 0x000fffffffffffff, 0x0010000000000000,
   0x8010000000000000, 0x3ff0000000000000, 0xbff0000000000000, 0x3ff0000000000001,
   0x3fefffffffffffff, 0x7fefffffffffffff, 0xffefffffffffffff, 0x7ff0000000000000,
   0xfff0000000000000, 0x7ff8000000000000, 0xfff8000000000000, 0x7ff0000000000001,
   0x7ff8000000001234, 0x3fe0000000000000, 0x4000000000000000, 0x3fb999999999999a,
   0x43e0000000000000, 0xc3e0000000000000, 0x41e0000000000000, 0x3ca0000000000000,
   0x0000000000000002, 0x0008000000000000, 0x7fe0000000000000, 0x4340000000000000]

def edges32 : List UInt32 :=
  [0, 0x80000000, 1, 0x80000001, 0x007fffff, 0x00800000, 0x80800000, 0x3f800000, 0xbf800000,
   0x3f800001, 0x3f7fffff, 0x7f7fffff, 0xff7fffff, 0x7f800000, 0xff800000, 0x7fc00000,
   0xffc00000, 0x7f800001, 0x7fc01234, 0x3f000000, 0x40000000, 0x3dcccccd, 0x5f000000,
   0xdf000000, 0x4f000000, 0x33800000, 0x00400000, 0x7f000000, 0x4b800000]

/-- A binary64 operand: random bits, an edge, an extreme exponent, a sparse
mantissa, or a moderate exponent. -/
def gen64 (r : Rng) : UInt64 × Rng :=
  let (k, r) := r.next
  let (a, r) := r.next
  let (b, r) := r.next
  let sign : UInt64 := (b >>> 63) <<< 63
  let frac := a &&& 0x000fffffffffffff
  match k % 6 with
  | 0 => (a, r)
  | 1 => (edges64[(b % edges64.length.toUInt64).toNat]!, r)
  | 2 =>
    let ex : UInt64 := if b % 2 == 0 then (b >>> 8) % 6 else (2041 : UInt64) + (b >>> 8) % 7
    (sign ||| (ex <<< 52) ||| frac, r)
  | 3 =>
    let ex : UInt64 := (1023 : UInt64) + (b >>> 8) % 120 - 60
    (sign ||| (ex <<< 52) ||| (frac &&& (b >>> 3) &&& (a >>> 7)), r)
  | _ =>
    let ex : UInt64 := (1023 : UInt64) + (b >>> 8) % 1400 - 700
    (sign ||| (ex <<< 52) ||| frac, r)

/-- A second operand close to `x` in exponent (cancellation, ties). -/
def near64 (x : UInt64) (r : Rng) : UInt64 × Rng :=
  let (k, r) := r.next
  let (a, r) := r.next
  let ex := (x >>> 52) &&& 0x7ff
  match k % 3 with
  | 0 =>
    let e2 := ex + (a >>> 50) % 120 - 60
    let e2 := if e2 > 0x7fe then (if ex > 1000 then 0x7fe else 0) else e2
    (((a >>> 63) <<< 63) ||| (e2 <<< 52) ||| (a &&& 0x000fffffffffffff), r)
  | 1 =>  -- half an ulp of x (an exact tie for x + y)
    let e2 := if ex > 53 then ex - 53 else 0
    (((a >>> 63) <<< 63) ||| (e2 <<< 52), r)
  | _ => (x ^^^ 0x8000000000000000 ^^^ (a &&& 0xff), r)

def gen32 (r : Rng) : UInt32 × Rng :=
  let (k, r) := r.next
  let (a, r) := r.next
  let (b, r) := r.next
  let a32 := a.toUInt32
  let sign : UInt32 := (b.toUInt32 >>> 31) <<< 31
  let frac := a32 &&& 0x007fffff
  match k % 6 with
  | 0 => (a32, r)
  | 1 => (edges32[(b % edges32.length.toUInt64).toNat]!, r)
  | 2 =>
    let ex : UInt32 := if b % 2 == 0 then ((b >>> 8) % 6).toUInt32 else (248 : UInt32) + ((b >>> 8) % 7).toUInt32
    (sign ||| (ex <<< 23) ||| frac, r)
  | 3 =>
    let ex : UInt32 := (127 : UInt32) + ((b >>> 8) % 40).toUInt32 - 20
    (sign ||| (ex <<< 23) ||| (frac &&& (b >>> 3).toUInt32 &&& (a >>> 37).toUInt32), r)
  | _ =>
    let ex : UInt32 := (127 : UInt32) + ((b >>> 8) % 200).toUInt32 - 100
    (sign ||| (ex <<< 23) ||| frac, r)

def near32 (x : UInt32) (r : Rng) : UInt32 × Rng :=
  let (k, r) := r.next
  let (a, r) := r.next
  let a32 := a.toUInt32
  let ex := (x >>> 23) &&& 0xff
  match k % 3 with
  | 0 =>
    let e2 := ex + ((a >>> 40) % 60).toUInt32 - 30
    let e2 := if e2 > 0xfe then (if ex > 100 then 0xfe else 0) else e2
    (((a32 >>> 31) <<< 31) ||| (e2 <<< 23) ||| (a32 &&& 0x007fffff), r)
  | 1 =>
    let e2 := if ex > 24 then ex - 24 else 0
    (((a32 >>> 31) <<< 31) ||| (e2 <<< 23), r)
  | _ => (x ^^^ 0x80000000 ^^^ (a32 &&& 0xff), r)

/-! ## binary64 / binary32 vs native -/

def b64 (u : UInt64) : Bits .binary64 := u.toBitVec
def b32 (u : UInt32) : Bits .binary32 := u.toBitVec

def isNaN64 (u : UInt64) : Bool := (u >>> 52) &&& 0x7ff == 0x7ff && u &&& 0x000fffffffffffff != 0
def isNaN32 (u : UInt32) : Bool := (u >>> 23) &&& 0xff == 0xff && u &&& 0x007fffff != 0

/-- Compare our bits with native bits: equal, or both NaN with ours canonical. -/
def cmp64 (ours : Bits .binary64) (nat : UInt64) : Bool × Bool :=
  if isNaN64 nat then (ours == nan .binary64, true) else (ours.toNat == nat.toNat, false)
def cmp32 (ours : Bits .binary32) (nat : UInt32) : Bool × Bool :=
  if isNaN32 nat then (ours == nan .binary32, true) else (ours.toNat == nat.toNat, false)

def cmp64Class (ours : Bits .binary64) (nat : UInt64) : Bool × Bool :=
  if isNaN64 nat then (isNaN .binary64 ours, true) else cmp64 ours nat

def runNative (n : Nat) (seed : UInt64) : IO Bool := do
  let mut r : Rng := ⟨seed * 2654435761 + 88172645463325252⟩
  let mut tA : Tally := { name := "f64 add" }
  let mut tS : Tally := { name := "f64 sub" }
  let mut tM : Tally := { name := "f64 mul" }
  let mut tD : Tally := { name := "f64 div" }
  let mut tQ : Tally := { name := "f64 sqrt" }
  let mut tC : Tally := { name := "f64 cmp(6)" }
  let mut tI : Tally := { name := "f64 ofInt(i64)" }
  let mut tT : Tally := { name := "f64 toInt(i64)" }
  let mut tN : Tally := { name := "f64->f32" }
  let mut uA : Tally := { name := "f32 add" }
  let mut uS : Tally := { name := "f32 sub" }
  let mut uM : Tally := { name := "f32 mul" }
  let mut uD : Tally := { name := "f32 div" }
  let mut uQ : Tally := { name := "f32 sqrt" }
  let mut uC : Tally := { name := "f32 cmp(6)" }
  let mut uI : Tally := { name := "f32 ofInt(i64)" }
  let mut uT : Tally := { name := "f32 toInt(i64)" }
  let mut uW : Tally := { name := "f32->f64" }
  let mut tB : Tally := { name := "bytes roundtrip" }
  for _ in [0:n] do
    -- binary64
    let (x, r1) := gen64 r
    let (k, r2) := r1.next
    let (y, r3) := if k % 2 == 0 then near64 x r2 else gen64 r2
    r := r3
    let fx := Float.ofBits x
    let fy := Float.ofBits y
    let msg := fun (_ : Unit) => s!"x={hex x.toNat 16} y={hex y.toNat 16}"
    let (ok, nc) := cmp64 (add .binary64 (b64 x) (b64 y)) (fx + fy).toBits
    tA ← tA.add ok nc msg
    let (ok, nc) := cmp64 (sub .binary64 (b64 x) (b64 y)) (fx - fy).toBits
    tS ← tS.add ok nc msg
    let (ok, nc) := cmp64 (mul .binary64 (b64 x) (b64 y)) (fx * fy).toBits
    tM ← tM.add ok nc msg
    let (ok, nc) := cmp64 (div .binary64 (b64 x) (b64 y)) (fx / fy).toBits
    tD ← tD.add ok nc msg
    let (ok, nc) := cmp64 (sqrt .binary64 (b64 x)) fx.sqrt.toBits
    tQ ← tQ.add ok nc msg
    let ours := [eq .binary64 (b64 x) (b64 y), ne .binary64 (b64 x) (b64 y),
      lt .binary64 (b64 x) (b64 y), le .binary64 (b64 x) (b64 y),
      gt .binary64 (b64 x) (b64 y), ge .binary64 (b64 x) (b64 y)]
    let nat := [fx == fy, fx != fy, decide (fx < fy), decide (fx ≤ fy), decide (fx > fy),
      decide (fx ≥ fy)]
    tC ← tC.add (ours == nat) false msg
    let i : Int64 := Int64.ofInt ((x.toNat : Int) >>> (y % 64).toNat) * (if k % 3 == 0 then -1 else 1)
    let (ok, nc) := cmp64 (ofInt .binary64 i.toInt) i.toFloat.toBits
    tI ← tI.add ok nc (fun _ => s!"i={i.toInt}")
    match toInt? .binary64 (b64 x) with
    | some v =>
      if -9223372036854775808 ≤ v ∧ v ≤ 9223372036854775807 then
        tT ← tT.add (v == fx.toInt64.toInt) false msg
    | none => tT ← tT.add (fx.isNaN || fx.isInf) false msg
    let (ok, nc) := cmp32 (convert .binary64 .binary32 (b64 x)) fx.toFloat32.toBits
    tN ← tN.add ok nc msg
    tB ← tB.add (ofBytesLE? .binary64 (toBytesLE .binary64 (b64 x)) == some (b64 x)) false msg
    -- binary32
    let (a, r4) := gen32 r
    let (k2, r5) := r4.next
    let (b, r6) := if k2 % 2 == 0 then near32 a r5 else gen32 r5
    r := r6
    let ga := Float32.ofBits a
    let gb := Float32.ofBits b
    let msg := fun (_ : Unit) => s!"a={hex a.toNat 8} b={hex b.toNat 8}"
    let (ok, nc) := cmp32 (add .binary32 (b32 a) (b32 b)) (ga + gb).toBits
    uA ← uA.add ok nc msg
    let (ok, nc) := cmp32 (sub .binary32 (b32 a) (b32 b)) (ga - gb).toBits
    uS ← uS.add ok nc msg
    let (ok, nc) := cmp32 (mul .binary32 (b32 a) (b32 b)) (ga * gb).toBits
    uM ← uM.add ok nc msg
    let (ok, nc) := cmp32 (div .binary32 (b32 a) (b32 b)) (ga / gb).toBits
    uD ← uD.add ok nc msg
    let (ok, nc) := cmp32 (sqrt .binary32 (b32 a)) ga.sqrt.toBits
    uQ ← uQ.add ok nc msg
    let ours := [eq .binary32 (b32 a) (b32 b), ne .binary32 (b32 a) (b32 b),
      lt .binary32 (b32 a) (b32 b), le .binary32 (b32 a) (b32 b),
      gt .binary32 (b32 a) (b32 b), ge .binary32 (b32 a) (b32 b)]
    let nat := [ga == gb, ga != gb, decide (ga < gb), decide (ga ≤ gb), decide (ga > gb),
      decide (ga ≥ gb)]
    uC ← uC.add (ours == nat) false msg
    let (ok, nc) := cmp32 (ofInt .binary32 i.toInt) i.toFloat32.toBits
    uI ← uI.add ok nc (fun _ => s!"i={i.toInt}")
    match toInt? .binary32 (b32 a) with
    | some v =>
      if -9223372036854775808 ≤ v ∧ v ≤ 9223372036854775807 then
        uT ← uT.add (v == ga.toInt64.toInt) false msg
    | none => uT ← uT.add (ga.isNaN || ga.isInf) false msg
    let (ok, nc) := cmp64Class (convert .binary32 .binary64 (b32 a)) ga.toFloat.toBits
    uW ← uW.add ok nc msg
  let mut allOk := true
  for t in [tA, tS, tM, tD, tQ, tC, tI, tT, tN, tB, uA, uS, uM, uD, uQ, uC, uI, uT, uW] do
    allOk := (← t.report) && allOk
  return allOk

/-! ## Vector files -/

def parseHex (s : String) : Nat :=
  s.toList.foldl (fun a c => a * 16 + Literal.hexVal c) 0

def parseInt (s : String) : Int :=
  match s.toList with
  | '-' :: rest => -((Literal.digitsToNat rest : Nat) : Int)
  | cs => (Literal.digitsToNat cs : Nat)

def q (s : String) : Bits .binary128 := BitVec.ofNat 128 (parseHex s)
def qcmp (ours : Bits .binary128) (exp : Nat) : Bool × Bool :=
  if isNaN .binary128 (BitVec.ofNat 128 exp) then (ours == nan .binary128, true)
  else (ours.toNat == exp, false)

def runVectors (path : String) (exactNaN : Bool := false) : IO Bool := do
  let qcmp := fun (ours : Bits .binary128) (expected : Nat) =>
    if exactNaN then (ours.toNat == expected, isNaN .binary128 (q (hex expected 32)))
    else qcmp ours expected
  let extcmp := fun (ours : Bits .binary128) (expected : Nat) =>
    if !exactNaN && isNaN .binary128 (BitVec.ofNat 128 expected) then (isNaN .binary128 ours, true)
    else qcmp ours expected
  let lines := (← IO.FS.readFile path).splitOn "\n"
  let mut ts : List (String × Tally) := []
  for line in lines do
    match line.splitOn "\t" with
    | [op, a, b, res] =>
      let (ok, nc) : Bool × Bool := match op with
        | "add" => qcmp (add .binary128 (q a) (q b)) (parseHex res)
        | "sub" => qcmp (sub .binary128 (q a) (q b)) (parseHex res)
        | "mul" => qcmp (mul .binary128 (q a) (q b)) (parseHex res)
        | "div" => qcmp (div .binary128 (q a) (q b)) (parseHex res)
        | "sqrt" => qcmp (sqrt .binary128 (q a)) (parseHex res)
        | "cmp" =>
          let x := q a
          let y := q b
          let bit := fun (c : Bool) => if c then '1' else '0'
          (String.ofList [bit (eq .binary128 x y), bit (ne .binary128 x y), bit (lt .binary128 x y),
            bit (le .binary128 x y), bit (gt .binary128 x y), bit (ge .binary128 x y)] == res, false)
        | "fixdi" | "fixsi" => (toInt? .binary128 (q a) == some (parseInt res), false)
        | "floatdi" | "floatsi" => qcmp (ofInt .binary128 (parseInt a)) (parseHex res)
        | "extenddf" =>
          extcmp (convert .binary64 .binary128 (BitVec.ofNat 64 (parseHex a))) (parseHex res)
        | "extendsf" =>
          extcmp (convert .binary32 .binary128 (BitVec.ofNat 32 (parseHex a))) (parseHex res)
        | "truncdf" =>
          let r := convert .binary128 .binary64 (q a)
          let e := parseHex res
          if isNaN .binary64 (BitVec.ofNat 64 e) then ((if exactNaN then r.toNat == e else r == nan .binary64), true)
          else (r.toNat == e, false)
        | "truncsf" =>
          let r := convert .binary128 .binary32 (q a)
          let e := parseHex res
          if isNaN .binary32 (BitVec.ofNat 32 e) then ((if exactNaN then r.toNat == e else r == nan .binary32), true)
          else (r.toNat == e, false)
        | _ => (false, false)
      let t := (ts.lookup op).getD { name := s!"f128 {op}" }
      let t ← t.add ok nc (fun _ => line)
      ts := (op, t) :: ts.filter (·.1 != op)
    | _ => unless line.isEmpty do throw (IO.userError s!"malformed vector: {line}")
  let mut allOk := !ts.isEmpty
  for (_, t) in ts.reverse do
    allOk := (← t.report) && allOk
  return allOk

def runLiterals (path : String) : IO Bool := do
  let lines := (← IO.FS.readFile path).splitOn "\n"
  let mut t32 : Tally := { name := "literal f32 (strtof)" }
  let mut t64 : Tally := { name := "literal f64 (strtod)" }
  let mut t128 : Tally := { name := "literal f128 (strtof128)" }
  for line in lines do
    match line.splitOn "\t" with
    | ["lit", s, h32, h64, h128] =>
      let ok32 := (ofLiteral .binary32 s).map (·.toNat) == some (parseHex h32)
      let ok64 := (ofLiteral .binary64 s).map (·.toNat) == some (parseHex h64)
      let ok128 := (ofLiteral .binary128 s).map (·.toNat) == some (parseHex h128)
      t32 ← t32.add ok32 false (fun _ => s!"{s} want {h32}")
      t64 ← t64.add ok64 false (fun _ => s!"{s} want {h64}")
      t128 ← t128.add ok128 false (fun _ => s!"{s} want {h128}")
    | _ => pure ()
  let a ← t32.report
  let b ← t64.report
  let c ← t128.report
  return a && b && c

/-- Parse a printf conversion like `%-+014.5e` (a trailing `|` is kept as text). -/
def parseSpec (s : String) : Option (PrintfSpec × String) := do
  let cs := s.toList
  let cs ← match cs with | '%' :: r => some r | _ => none
  let flags := cs.takeWhile (fun c => c = '-' || c = '+' || c = ' ' || c = '#' || c = '0')
  let cs := cs.drop flags.length
  let w := cs.takeWhile Char.isDigit
  let cs := cs.drop w.length
  let (prec, cs) := match cs with
    | '.' :: r => (some (Literal.digitsToNat (r.takeWhile Char.isDigit)), r.dropWhile Char.isDigit)
    | _ => (none, cs)
  match cs with
  | c :: rest =>
    some ({ conv := c, minus := flags.contains '-', plus := flags.contains '+',
            space := flags.contains ' ', hash := flags.contains '#', zero := flags.contains '0',
            width := Literal.digitsToNat w, precision := prec }, String.ofList rest)
  | [] => none

def runPrintf (path : String) (style : HexStyle) : IO Bool := do
  let lines := (← IO.FS.readFile path).splitOn "\n"
  let mut td : Tally := { name := s!"printf double ({repr style})" }
  let mut tq : Tally := { name := s!"printf binary128 ({repr style})" }
  for line in lines do
    match line.splitOn "\t" with
    | [kind, spec, h, out] =>
      match parseSpec spec with
      | some (sp, tail) =>
        if kind == "pd" then
          let s := format sp .binary64 (BitVec.ofNat 64 (parseHex h)) style ++ tail
          td ← td.add (s == out) false (fun _ => s!"{spec} {h}: want [{out}] got [{s}]")
        else if kind == "pq" then
          let s := format sp .binary128 (BitVec.ofNat 128 (parseHex h)) style ++ tail
          tq ← tq.add (s == out) false (fun _ => s!"{spec} {h}: want [{out}] got [{s}]")
      | none => pure ()
    | _ => pure ()
  let a ← td.report
  let b ← tq.report
  return a && b

/-! ## Benchmark -/

def timeIt (label : String) (n : Nat) (act : Nat → Nat) : IO Unit := do
  let t0 ← IO.monoNanosNow
  let mut acc := 0
  for i in [0:n] do
    acc := acc + act i
  let t1 ← IO.monoNanosNow
  IO.println s!"bench\t{label}\t{(t1 - t0) / n / 1000}.{((t1 - t0) / n % 1000) / 100} us/op\t(acc {acc % 7})"

def runBench (n : Nat) : IO Unit := do
  let xs : Array (Bits .binary64) := (List.range 64).toArray.map fun i =>
    ofLiteral .binary64 s!"{i + 1}.{(i * 7919) % 1000}e{(i % 9) - 4}" |>.getD (zero .binary64 false)
  let qs : Array (Bits .binary128) := xs.map (convert .binary64 .binary128)
  let fs : Array (Bits .binary32) := xs.map (convert .binary64 .binary32)
  let big := (ofLiteral .binary128 "1e4000").getD (zero .binary128 false)
  let tiny := (ofLiteral .binary128 "1e-4000").getD (zero .binary128 false)
  timeIt "f64 add" n fun i => (add .binary64 xs[i % 64]! xs[(i * 7) % 64]!).toNat % 2
  timeIt "f64 mul" n fun i => (mul .binary64 xs[i % 64]! xs[(i * 7) % 64]!).toNat % 2
  timeIt "f64 div" n fun i => (div .binary64 xs[i % 64]! xs[(i * 7) % 64]!).toNat % 2
  timeIt "f64 sqrt" n fun i => (sqrt .binary64 xs[i % 64]!).toNat % 2
  timeIt "f64 lt" n fun i => if lt .binary64 xs[i % 64]! xs[(i * 7) % 64]! then 1 else 0
  timeIt "f32 add" n fun i => (add .binary32 fs[i % 64]! fs[(i * 7) % 64]!).toNat % 2
  timeIt "f128 add" n fun i => (add .binary128 qs[i % 64]! qs[(i * 7) % 64]!).toNat % 2
  timeIt "f128 mul" n fun i => (mul .binary128 qs[i % 64]! qs[(i * 7) % 64]!).toNat % 2
  timeIt "f128 div" n fun i => (div .binary128 qs[i % 64]! qs[(i * 7) % 64]!).toNat % 2
  timeIt "f128 sqrt" n fun i => (sqrt .binary128 qs[i % 64]!).toNat % 2
  timeIt "f128 add 1e4000+1e-4000 (worst gap)" (n / 10 + 1) fun i =>
    (add .binary128 (if i % 2 == 0 then big else qs[i % 64]!) tiny).toNat % 2
  timeIt "ofInt f64" n fun i => (ofInt .binary64 (i * 1234567)).toNat % 2
  timeIt "convert f64->f32" n fun i => (convert .binary64 .binary32 xs[i % 64]!).toNat % 2
  timeIt "literal f64 \"0.1\"" (n / 10 + 1) fun _ => ((ofLiteral .binary64 "0.1").getD 0).toNat % 2
  timeIt "literal f128 \"1.2345e-4900\"" (n / 100 + 1) fun _ =>
    ((ofLiteral .binary128 "1.2345e-4900").getD 0).toNat % 2
  timeIt "printf %g f64" (n / 10 + 1) fun i => (format { conv := 'g' } .binary64 xs[i % 64]!).length
  timeIt "printf %.17g f64" (n / 10 + 1) fun i =>
    (format { conv := 'g', precision := some 17 } .binary64 xs[i % 64]!).length
  timeIt "printf %Lf 1e4000" (n / 1000 + 1) fun _ => (format { conv := 'f' } .binary128 big).length

end IEEETest

open IEEETest in
def main (args : List String) : IO UInt32 := do
  match args with
  | ["native", n, seed] =>
    return if ← runNative n.toNat! seed.toNat!.toUInt64 then 0 else 1
  | ["vectors", path] => return if ← runVectors path then 0 else 1
  | ["sail", path] => return if ← runVectors path true then 0 else 1
  | ["literals", path] => return if ← runLiterals path then 0 else 1
  | ["printf", path, style] =>
    let st := if style == "glibc" then HexStyle.glibc else HexStyle.newlib
    return if ← runPrintf path st then 0 else 1
  | ["bench", n] => runBench n.toNat!; return 0
  | _ =>
    IO.eprintln "usage: ieee-test native N SEED | vectors F | sail F | literals F | printf F glibc|newlib | bench N"
    return 2
