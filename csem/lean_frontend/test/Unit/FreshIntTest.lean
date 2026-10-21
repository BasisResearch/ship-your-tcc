/-
  Test: supply-threaded fresh symbols + the digest parameter and MD5.

  Effect-retirement C1 rewrite (charter section 7.1): the old
  runEffectful/freshIntIO distinctness probes died with the mechanism —
  `fresh_int` is a lem SUPPLY on the Lean target, so `Symbol.fresh` is
  the pure lifted `fresh : Nat → Unit → sym × Nat`. The tests now pin
  the THREADING LAWS (monotonicity + distinctness through a lifted
  chain, kernel-visible). Since ship-your-tcc F1b the digest is the
  instance-implicit `[CerberusFresh.Digest]` and `md5Hex` is pure Lean;
  the native global, its barrier test and native/md5.c are gone.
-/

import Symbol
import CerberusFresh
import LemLib

def assertNotEqual {α : Type} [BEq α] [Repr α] (label : String) (a b : α) : IO Bool := do
  if a == b then
    IO.println s!"  ✗ FAIL {label}: both values are {repr a}"
    return false
  else
    return true

def assertEqual {α : Type} [BEq α] [Repr α] (label : String) (got expected : α) : IO Bool := do
  if got == expected then
    return true
  else
    IO.println s!"  ✗ FAIL {label}: got {repr got}, expected {repr expected}"
    return false

/-- Kernel-checked threading laws: the exact draw sequence of a lifted
    chain is a defeq fact (the L1 supply pins' pattern, TestSupplyCheck):
    each draw returns the current supply and advances by one. -/
example : LemLib.supplySplit 100 = (100, 101) := rfl

def testRunDigest : String := "900150983cd24fb0d6963f7d28e17f72"

def symId : sym → Nat := fun | Symbol _ n _ => n

def testSupplyThreading : IO Bool := do
  -- the digest the native global held before any TU set it (F1b: a parameter)
  letI : CerberusFresh.Digest := ⟨""⟩
  IO.println "test: Symbol.fresh threads the supply (monotone, distinct, returned)"
  let (s1, sup1) := @fresh ⟨""⟩ 500 ()
  let (s2, sup2) := @fresh ⟨""⟩ sup1 ()
  let (s3, sup3) := @fresh ⟨""⟩ sup2 ()
  IO.println s!"  got ids: {symId s1}, {symId s2}, {symId s3}; final supply {sup3}"
  let okA ← assertEqual "first draw uses the seed" (symId s1) 500
  let okB ← assertEqual "supply advances by one per draw" (sup1, sup2, sup3) (501, 502, 503)
  let ok12 ← assertNotEqual "s1 vs s2" (symId s1) (symId s2)
  let ok23 ← assertNotEqual "s2 vs s3" (symId s2) (symId s3)
  let ok13 ← assertNotEqual "s1 vs s3" (symId s1) (symId s3)
  if okA && okB && ok12 && ok23 && ok13 then
    IO.println "  ✓ PASS"
    return true
  else
    return false

def testSupplyFamily : IO Bool := do
  -- the digest the native global held before any TU set it (F1b: a parameter)
  letI : CerberusFresh.Digest := ⟨""⟩
  IO.println "test: the fresh* family draws exactly one id each and composes"
  -- fresh_pretty_with_id bakes the drawn id into the description
  let (sp, supP) := @fresh_pretty_with_id ⟨""⟩ 700 (fun n => s!"while_{n}")
  let okP ← assertEqual "fresh_pretty_with_id id" (symId sp) 700
  let okP2 ← assertEqual "fresh_pretty_with_id supply" supP 701
  let okP3 ← assertEqual "fresh_pretty_with_id descr"
    (match sp with | Symbol _ _ (SD_Id s) => s | _ => "<not SD_Id>") "while_700"
  -- fresh_given_int is pure (no draw): the explicit-seed builder
  let sg := fresh_given_int testRunDigest 900
  let okG ← assertEqual "fresh_given_int id" (symId sg) 900
  if okP && okP2 && okP3 && okG then
    IO.println "  ✓ PASS"
    return true
  else
    return false

def testMd5Hex : IO Bool := do
  IO.println "test: CerberusFresh.md5Hex matches OCaml Digest.to_hex (RFC 1321 vectors)"
  -- Expected values are OCaml `Digest.to_hex (Digest.string s)` output,
  -- probed 2026-08-19 on this machine's switch (identical to the RFC 1321
  -- appendix A.5 test-suite values):
  --   ""    -> d41d8cd98f00b204e9800998ecf8427e
  --   "abc" -> 900150983cd24fb0d6963f7d28e17f72
  --   "The quick brown fox jumps over the lazy dog"
  --         -> 9e107d9d372bb6826bd81d3542a419d6
  let ok1 ← assertEqual "md5(\"\")" (CerberusFresh.md5Hex "")
              "d41d8cd98f00b204e9800998ecf8427e"
  let ok2 ← assertEqual "md5(\"abc\")" (CerberusFresh.md5Hex "abc")
              "900150983cd24fb0d6963f7d28e17f72"
  let ok3 ← assertEqual "md5(fox)"
              (CerberusFresh.md5Hex "The quick brown fox jumps over the lazy dog")
              "9e107d9d372bb6826bd81d3542a419d6"
  -- padding/block coverage: a-z is 26 bytes (one padded block);
  -- A-Za-z0-9 is 62 bytes (remainder >= 56: the two-block padding path);
  -- the 80-byte digits vector (RFC 1321 A.5) has one full block plus a
  -- 16-byte tail.
  -- RFC 1321 A.5: alphabet, A-Za-z0-9, and digits-x8 vectors
  let ok4 ← assertEqual "md5(a-z)" (CerberusFresh.md5Hex "abcdefghijklmnopqrstuvwxyz")
              "c3fcd3d76192e4007dfb496cca67e13b"
  let ok5 ← assertEqual "md5(A-Za-z0-9)"
              (CerberusFresh.md5Hex "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
              "d174ab98d277d9f5a5611c2c9f419d9f"
  let ok6 ← assertEqual "md5(digits x8)"
              (CerberusFresh.md5Hex
                "12345678901234567890123456789012345678901234567890123456789012345678901234567890")
              "57edf4a22be3c955ac49da2e2107b67a"
  -- ship-your-tcc F1b (pure md5Hex): UTF-8 multi-byte inputs and the byte
  -- lengths at the padding boundaries (55/56, 63/64/65, 119/120, 128);
  -- expected values are Python 3 `hashlib.md5(s.encode()).hexdigest()`.
  let pyVectors : List (String × String) := [
    ("é", "66ddcd97cfdeabb2f6fb8a999b4bc76f"),
    ("日本語", "00110af8b4393ef3f72c50be5b332bec"),
    ("😀", "2a02eac39d716a70ecf37579185927b6"),
    ("Ünïcödé — ∀x. x ≠ 😀", "a157b0cfd201120ed5342045fcedf574"),
    (String.ofList (List.replicate 55 'a'), "ef1772b6dff9a122358552954ad0df65"),
    (String.ofList (List.replicate 56 'a'), "3b0c8ac703f828b04c6c197006d17218"),
    (String.ofList (List.replicate 63 'a'), "b06521f39153d618550606be297466d5"),
    (String.ofList (List.replicate 64 'a'), "014842d480b571495a4a0363793f7367"),
    (String.ofList (List.replicate 65 'a'), "c743a45e0d2e6a95cb859adae0248435"),
    (String.ofList (List.replicate 119 'a'), "8a7bd0732ed6a28ce75f6dabc90e1613"),
    (String.ofList (List.replicate 120 'a'), "5f61c0ccad4cac44c75ff505e1f1e537"),
    (String.ofList (List.replicate 128 'a'), "e510683b3f5ffe4093d021808bc6ff70")]
  let mut ok7 := true
  for (inp, want) in pyVectors do
    let ok ← assertEqual s!"md5(<{inp.utf8ByteSize} bytes>)" (CerberusFresh.md5Hex inp) want
    ok7 := ok7 && ok
  if ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7 then
    IO.println "  ✓ PASS"
    return true
  else
    return false

/-- `md5Hex` is a pure, kernel-reducible definition (ship-your-tcc F1b): the
    kernel itself computes the RFC 1321 vectors (`decide +kernel` is kernel
    evaluation, no extra axiom). -/
example : CerberusFresh.md5Hex "" = "d41d8cd98f00b204e9800998ecf8427e" := by decide +kernel
example : CerberusFresh.md5Hex "abc" = "900150983cd24fb0d6963f7d28e17f72" := by decide +kernel

/-- The digest is an instance-implicit parameter (ship-your-tcc F1b): a fresh
    symbol carries the supplied digest, by `rfl`. -/
example (d : String) : digest_of_sym (@fresh ⟨d⟩ 7 ()).1 = d := rfl

def testDigestParameter : IO Bool := do
  IO.println "test: per-TU digest parameter (Symbol.fresh carries the supplied digest)"
  -- Symbol.fresh stamps the digest of the `CerberusFresh.Digest` instance in
  -- scope (symbol.lem: Symbol (digest()) id SD_None; the instance reader of
  -- lean_digest.lem). Two draws under two TU digests carry different
  -- digests; two under the same one are from the same TU. (Until F1b this
  -- tested the native global and its IO barrier: the draws had to be
  -- IO-positioned so a let-sunk draw did not see the later TU's digest.)
  let dA := CerberusFresh.md5Hex "tu-A"
  let dB := CerberusFresh.md5Hex "tu-B"
  let s1 := (@fresh ⟨dA⟩ 0 ()).1
  let s2 := (@fresh ⟨dB⟩ 1 ()).1
  let s3 := (@fresh ⟨dB⟩ 2 ()).1
  let ok1 ← assertEqual "digest of fresh under A" (digest_of_sym s1) dA
  let ok2 ← assertEqual "digest of fresh under B" (digest_of_sym s2) dB
  let ok3 ← assertEqual "cross-TU from_same_translation_unit"
              (from_same_translation_unit s1 s2) false
  let ok4 ← assertEqual "same-TU from_same_translation_unit"
              (from_same_translation_unit s2 s3) true
  if ok1 && ok2 && ok3 && ok4 then
    IO.println "  ✓ PASS"
    return true
  else
    return false

def main : IO UInt32 := do
  let mut passed := 0
  let mut failed := 0
  for test in [testSupplyThreading, testSupplyFamily, testMd5Hex, testDigestParameter] do
    if ← test then
      passed := passed + 1
    else
      failed := failed + 1
  IO.println s!"\n{passed} passed, {failed} failed"
  return if failed == 0 then 0 else 1
