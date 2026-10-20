/-!
# TccModel: machine-level helpers

The executable Lean model of tcc (DESIGN-COMPILER.md §4.1, PHASES.md F3)
mirrors tcc's C code as compiled for the image (riscv-none-elf-gcc, rv64i,
lp64): `int` is `Int32`, `unsigned` is `UInt32`, `long`/`addr_t` are
64-bit, `char` is unsigned. This module has the byte-level helpers the
rest of the model shares: little-endian access to section buffers (tcc's
`read32le` & co.), C strings, and a `printf` subset with newlib's output
for the formats tcc uses in its messages (`cfmt`).
-/

namespace Tcc.Model

/-! ## integers -/

@[inline] def i32 (n : Int) : Int32 := Int32.ofInt n
@[inline] def u32 (n : Nat) : UInt32 := UInt32.ofNat n
@[inline] def _root_.Int32.toNat' (x : Int32) : Nat := x.toInt.toNat
@[inline] def _root_.Int32.toUInt64' (x : Int32) : UInt64 := x.toInt64.toUInt64
@[inline] def _root_.UInt16.toInt32' (x : UInt16) : Int32 := x.toUInt32.toInt32
@[inline] def _root_.Int32.toUInt16' (x : Int32) : UInt16 := x.toUInt32.toUInt16
@[inline] def _root_.Int64.toUInt32' (x : Int64) : UInt32 := x.toUInt64.toUInt32
@[inline] def _root_.Int32.toUInt8 (x : Int32) : UInt8 := x.toUInt32.toUInt8
@[inline] def _root_.Int32.toUInt64 (x : Int32) : UInt64 := x.toInt64.toUInt64
@[inline] def _root_.UInt8.toInt32 (x : UInt8) : Int32 := x.toUInt32.toInt32
@[inline] def b2i (b : Bool) : Int32 := if b then 1 else 0
@[inline] def bit (x : Int32) (m : Int32) : Bool := x &&& m != 0


/-! ## byte buffers (little endian, as tcc's read32le/write32le) -/

namespace Bytes

@[inline] def r8 (b : ByteArray) (o : Nat) : UInt8 := b.get! o
@[inline] def r16 (b : ByteArray) (o : Nat) : UInt16 :=
  (b.get! o).toUInt16 ||| ((b.get! (o+1)).toUInt16 <<< 8)
@[inline] def r32 (b : ByteArray) (o : Nat) : UInt32 :=
  (b.get! o).toUInt32 ||| ((b.get! (o+1)).toUInt32 <<< 8) |||
  ((b.get! (o+2)).toUInt32 <<< 16) ||| ((b.get! (o+3)).toUInt32 <<< 24)
@[inline] def r64 (b : ByteArray) (o : Nat) : UInt64 :=
  (r32 b o).toUInt64 ||| ((r32 b (o+4)).toUInt64 <<< 32)

@[inline] def w8 (b : ByteArray) (o : Nat) (v : UInt8) : ByteArray := b.set! o v
@[inline] def w16 (b : ByteArray) (o : Nat) (v : UInt16) : ByteArray :=
  (b.set! o v.toUInt8).set! (o+1) (v >>> 8).toUInt8
@[inline] def w32 (b : ByteArray) (o : Nat) (v : UInt32) : ByteArray :=
  (((b.set! o v.toUInt8).set! (o+1) (v >>> 8).toUInt8).set! (o+2) (v >>> 16).toUInt8).set! (o+3) (v >>> 24).toUInt8
@[inline] def w64 (b : ByteArray) (o : Nat) (v : UInt64) : ByteArray :=
  w32 (w32 b o v.toUInt32) (o+4) (v >>> 32).toUInt32

/-- `n` zero bytes -/
def zeros (n : Nat) : ByteArray := ⟨Array.replicate n 0⟩

/-- grow `b` with zeros to at least `n` bytes -/
def grow (b : ByteArray) (n : Nat) : ByteArray :=
  if b.size ≥ n then b else b ++ zeros (n - b.size)

/-- `memcpy(dst + o, src + so, n)` -/
@[inline] def copy (dst : ByteArray) (o : Nat) (src : ByteArray) (so n : Nat) : ByteArray :=
  src.copySlice so dst o n

/-- the C string at offset `o` (up to, not including, the first NUL) -/
def cstrAt (b : ByteArray) (o : Nat) : ByteArray := Id.run do
  let mut e := o
  for _ in [o:b.size] do
    if b.get! e == 0 then break
    e := e + 1
  return b.extract o e

/-- strlen of the C string at `o` -/
def strlenAt (b : ByteArray) (o : Nat) : Nat := (cstrAt b o).size

def ofString (s : String) : ByteArray := s.toUTF8

/-- bytes as a Lean string (latin-1 view: each byte one char) -/
def toStr (b : ByteArray) : String := Id.run do
  let mut s := ""
  for c in b.data do s := s.push (Char.ofNat c.toNat)
  return s

end Bytes

instance : Hashable ByteArray where
  hash b := Id.run do
    let mut h : UInt64 := 7
    for c in b.data do h := mixHash h c.toUInt64
    return h

/-! ## C character classes (tcc.h) -/

@[inline] def isSpace (c : Int32) : Bool := c == 32 || c == 9 || c == 11 || c == 12 || c == 13
@[inline] def isId (c : Int32) : Bool := (c ≥ 97 && c ≤ 122) || (c ≥ 65 && c ≤ 90) || c == 95
@[inline] def isNum (c : Int32) : Bool := c ≥ 48 && c ≤ 57
@[inline] def isOct (c : Int32) : Bool := c ≥ 48 && c ≤ 55
@[inline] def toUp (c : Int32) : Int32 := if c ≥ 97 && c ≤ 122 then c - 97 + 65 else c

/-! ## printf subset (newlib's output for the conversions tcc's messages use) -/

inductive CArg where
  | i (v : Int)          -- signed integer argument (%d %i %c %ld %lld)
  | u (v : Nat)          -- unsigned argument (%u %x %X %o %llu %lx); also %d of an unsigned value
  | s (v : ByteArray)    -- %s
  deriving Inhabited

private def digits (n : Nat) (base : Nat) (upper : Bool) : List Char :=
  let ds := Nat.toDigits base n
  if upper then ds.map Char.toUpper else ds

/-- `snprintf` without the size limit: the subset of C's printf used by
tcc's messages and names (`%d %i %u %x %X %o %c %s %p %%`, flags `-0 +#`,
width and precision, `*`, length modifiers `h hh l ll z`). The integer
argument is reduced to the conversion's width (32 bits unless `l`/`ll`/`z`). -/
def cfmt (fmt : String) (args : List CArg) : ByteArray := Id.run do
  let f := fmt.toUTF8
  let mut out : ByteArray := ByteArray.empty
  let mut i := 0
  let mut as := args
  let next (as : List CArg) : CArg × List CArg :=
    match as with | a :: r => (a, r) | [] => (.i 0, [])
  for _ in [0:f.size] do
    if i ≥ f.size then break
    let c := f.get! i
    if c != 37 then
      out := out.push c; i := i + 1
    else
      i := i + 1
      let mut left := false; let mut zero := false; let mut plus := false
      let mut space := false; let mut alt := false
      for _ in [0:8] do
        let d := f.get! i
        if d == 45 then left := true; i := i + 1
        else if d == 48 then zero := true; i := i + 1
        else if d == 43 then plus := true; i := i + 1
        else if d == 32 then space := true; i := i + 1
        else if d == 35 then alt := true; i := i + 1
        else break
      let mut width : Nat := 0
      if f.get! i == 42 then
        let (a, r) := next as; as := r
        match a with
        | .i v => if v < 0 then left := true; width := v.natAbs else width := v.toNat
        | .u v => width := v
        | .s _ => pure ()
        i := i + 1
      else
        for _ in [0:10] do
          let d := f.get! i
          if d ≥ 48 && d ≤ 57 then width := width * 10 + (d.toNat - 48); i := i + 1 else break
      let mut prec : Option Nat := none
      if f.get! i == 46 then
        i := i + 1
        if f.get! i == 42 then
          let (a, r) := next as; as := r
          match a with
          | .i v => prec := if v < 0 then none else some v.toNat
          | .u v => prec := some v
          | .s _ => pure ()
          i := i + 1
        else
          let mut p := 0
          for _ in [0:10] do
            let d := f.get! i
            if d ≥ 48 && d ≤ 57 then p := p * 10 + (d.toNat - 48); i := i + 1 else break
          prec := some p
      let mut lng : Int := 0
      for _ in [0:3] do
        let d := f.get! i
        if d == 108 || d == 122 || d == 106 || d == 116 then lng := lng + 2; i := i + 1
        else if d == 104 then lng := lng - 1; i := i + 1
        else break
      let conv := f.get! i
      i := i + 1
      let bits := if lng ≥ 2 then 64 else 32
      let pad (body : List Char) (sign : String) (isNum : Bool) : ByteArray := Id.run do
        let len := body.length + sign.length
        let mut o := ByteArray.empty
        if len ≥ width then
          o := o ++ sign.toUTF8 ++ (String.ofList body).toUTF8
        else if left then
          o := o ++ sign.toUTF8 ++ (String.ofList body).toUTF8 ++ Bytes.zeros 0
          for _ in [0:width - len] do o := o.push 32
        else if zero && isNum && prec.isNone then
          o := o ++ sign.toUTF8
          for _ in [0:width - len] do o := o.push 48
          o := o ++ (String.ofList body).toUTF8
        else
          for _ in [0:width - len] do o := o.push 32
          o := o ++ sign.toUTF8 ++ (String.ofList body).toUTF8
        return o
      let precDigits (ds : List Char) : List Char :=
        match prec with
        | some p => if p == 0 && ds == ['0'] then [] else List.replicate (p - ds.length) '0' ++ ds
        | none => ds
      if conv == 37 then out := out.push 37
      else if conv == 100 || conv == 105 then
        let (a, r) := next as; as := r
        let v : Int := match a with
          | .i v => v | .u v => (v : Int) | .s _ => 0
        let m : Int := 2 ^ bits
        let v := ((v % m) + m) % m
        let v := if v ≥ m / 2 then v - m else v
        let v := if lng == -1 then (let w := ((v % 65536) + 65536) % 65536; if w ≥ 32768 then w - 65536 else w)
                 else if lng == -2 then (let w := ((v % 256) + 256) % 256; if w ≥ 128 then w - 256 else w) else v
        let sign := if v < 0 then "-" else if plus then "+" else if space then " " else ""
        out := out ++ pad (precDigits (digits v.natAbs 10 false)) sign true
      else if conv == 117 || conv == 120 || conv == 88 || conv == 111 then
        let (a, r) := next as; as := r
        let v : Int := match a with
          | .i v => v | .u v => (v : Int) | .s _ => 0
        let m : Int := 2 ^ bits
        let v := (((v % m) + m) % m).toNat
        let v := if lng == -1 then v % 65536 else if lng == -2 then v % 256 else v
        let base := if conv == 111 then 8 else if conv == 117 then 10 else 16
        let ds := precDigits (digits v base (conv == 88))
        let pre := if alt && v != 0 then (if conv == 120 then "0x" else if conv == 88 then "0X" else "") else ""
        let ds := if alt && conv == 111 && ds.head? != some '0' then '0' :: ds else ds
        out := out ++ pad ds pre true
      else if conv == 99 then
        let (a, r) := next as; as := r
        let v : Nat := match a with | .i v => (v % 256).toNat | .u v => v % 256 | .s _ => 0
        let body := ByteArray.empty.push v.toUInt8
        if width > 1 then
          if left then
            out := out ++ body
            for _ in [0:width-1] do out := out.push 32
          else
            for _ in [0:width-1] do out := out.push 32
            out := out ++ body
        else out := out ++ body
      else if conv == 115 then
        let (a, r) := next as; as := r
        let sb := match a with | .s b => b | _ => "(null)".toUTF8
        let sb := match prec with | some p => sb.extract 0 (min p sb.size) | none => sb
        if sb.size ≥ width then out := out ++ sb
        else if left then
          out := out ++ sb
          for _ in [0:width - sb.size] do out := out.push 32
        else
          for _ in [0:width - sb.size] do out := out.push 32
          out := out ++ sb
      else if conv == 112 then
        let (a, r) := next as; as := r
        let v : Nat := match a with | .i v => v.toNat | .u v => v | .s _ => 0
        out := out ++ pad (digits v 16 false) "0x" true
      else
        out := out.push 37 |>.push conv
  return out

end Tcc.Model
