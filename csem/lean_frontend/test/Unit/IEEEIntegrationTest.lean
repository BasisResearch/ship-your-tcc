import CerbMem

/-! Independent bit pins at the handwritten seam. Full frontend checks live in
`csem/tests/ieee`; native and libgcc are testing oracles only. -/
open CerbIEEE CerbMem
local instance : CerbImplCfg.Impl := ⟨CerbImplCfg.tccRV64⟩

def expect (name : String) (ok : Bool) : IO Unit :=
  unless ok do throw (IO.userError s!"IEEE integration: {name}")

def main : IO UInt32 := do
  let f := strFvalTyped (.RealFloating .Float0) "1.0000000596046447753906250000008271806"
  let q := strFvalTyped (.RealFloating .LongDouble) "0x1.0000000000000000000000000001p0"
  expect "float literal, one rounding" (f.fmt == .binary32 && f.bits.toNat == 0x3f800001)
  expect "quad literal" (q.fmt == .binary128 && q.bits.toNat == 0x3fff0000000000000000000000000001)
  let i := fvfromintTyped (.RealFloating .Float0) (.IV .Prov_none 4611686293305294849)
  expect "integer directly to float" (i.bits.toNat == 0x5e800001)
  let z := negFval (strFvalTyped (.RealFloating .Float0) "0.0")
  expect "negative zero" (z.bits.toNat == 0x80000000)
  let n : FVal := ⟨.binary64, 0xfff8000000001234#64⟩
  expect "same-format payload" ((n.to .binary64).bits.toNat == 0xfff8000000001234)
  expect "NaN comparison" (!eqFval n n && !ltFval n n && !leFval n n)
  expect "canonical arithmetic NaN" ((opFval .FloatAdd n 1).bits.toNat == 0x7ff8000000000000)
  expect "payload-preserving extension" ((n.to .binary128).bits.toNat == 0xffff8000000001234000000000000000)
  expect "canonical conversion NaN" ((n.to .binary32).bits.toNat == 0x7fc00000)
  for (ft, v) in [(.RealFloating .Float0, z), (.RealFloating .Double, n), (.RealFloating .LongDouble, q)] do
    let bs := (memValueToBytes .empty .empty [] (.MVfloating ft v)).2
    expect "configured byte count" (bs.length == (floatingFormat ft).bytes)
    expect "exact object bytes" (bs.map (fun b => b.value.getD 0) == toBytesLE v.fmt v.bits)
  expect "signed NaN text" (CerbFloat.formatFixed 6 n == "-nan")
  expect "quad printf" (CerbFloat.formatIEEE 'a' false false false false false 0 none q ==
    "0x1.0000000000000000000000000001p+0")
  IO.println "IEEE integration: all pins passed"
  return 0
