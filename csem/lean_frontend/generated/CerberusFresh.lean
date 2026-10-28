/-
  Fresh name generation and digest operations.
  Corresponds to: Cerb_fresh and Digest modules in OCaml.

  This is a leaf module with no imports from generated code,
  avoiding circular dependencies in the build.

  DIGEST REPRESENTATION (arc 5 / S2, documented-deliberate divergence):
  OCaml's `Digest.t` is the RAW 16-byte MD5 string; a Lean `String` must
  be valid UTF-8, so we store digests in `Digest.to_hex` form (32-char
  lowercase hex, `md5Hex` below). Consequences, each checked:
    * equality: hex is injective on raw digests — `digest_compare = 0`
      agrees exactly with OCaml `Digest.compare = 0` (the only digest
      property the model consumes: symbolEquality/symbol_compare
      (symbol.lem:153-162 via Eq/Ord instances) and
      from_same_translation_unit (symbol.lem:286-288)).
    * ordering: lowercase hex is byte-wise order-isomorphic to the raw
      bytes ('0'..'9','a'..'f' ASCII order = nibble order), so
      digest_compare orders any two digests exactly as OCaml
      Digest.compare (= String.compare on raw bytes) does — symbol-keyed
      map iteration order is preserved.
    * string_of_digest (OCaml `Digest.to_hex`) becomes the identity: our
      rep already IS the to_hex image, so printed digests match OCaml
      byte-for-byte.
  What gets digested is the SAME on both sides since zero-discrepancy Z3
  (census row Z-28): OCaml digests the .c file (`Cerb_fresh.set_digest
  filename` = Digest.file, pipeline.ml:181, util/cerb_fresh.ml:88-94);
  the Lean pipeline never sees the .c, so the oracle's --cabs-json path
  records that very digest in the JSON (`digest` field, main.ml) and
  the driver passes it per TU (CabsImport.parseJson, fail-closed on a
  JSON without it; the `Digest` instance below). Until Z3 the Lean side hashed the cabs-json text — a
  different value, under which every program global sorted after every
  libc global in libc mode: symbol digests ORDER the linked globals
  (Core_linking.merge_globs' min-(digest, number) topological choice),
  so the value is behaviour, not provenance.
-/

set_option autoImplicit true

namespace CerberusFresh

/-- First available C symbol number. Concrete function pointers serialize the
symbol number as their address, and zero bytes denote NULL. The OCaml frontend
has consumed runtime symbols before translating C; the pure Lean runtime parser
uses separate hash-based symbols, so its C supply must explicitly reserve zero.
Every pipeline starts here and threads the returned supply across all TUs. -/
def initialSymbolSupply : Nat := 1

theorem initialSymbolSupply_pos : 0 < initialSymbolSupply := by decide

/-- Compare two digests (hex-form strings). Returns negative, zero, or
    positive. Corresponds to: Digest.compare (order-isomorphic on the hex
    representation — see the module note). -/
def digest_compare (x y : String) : Int :=
  if x < y then -1 else if x == y then 0 else 1

/-- Convert digest to hex string.
    Corresponds to: Digest.to_hex — the identity here, because our digest
    representation is already the to_hex image (see the module note). -/
def string_of_digest (x : String) : String := x

/-! ## MD5 (RFC 1321), pure

The digest as a total, kernel-reducible function over the UTF-8 bytes
(F1b, DESIGN-COMPILER §3.4: it replaces the native `cerb_md5_hex`). Every
loop is structural recursion on a count; byte reads are `getD` reads of
indices below the padded length. -/

namespace MD5

/-- `T[i] = floor(abs(sin(i+1)) * 2^32)` (RFC 1321 §3.4). -/
def T : Array UInt32 := #[
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391]

/-- The per-operation left-rotate amounts (RFC 1321 §3.4, rounds 1–4). -/
def S : Array UInt32 := #[
  7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
  5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21]

/-- The chaining state `A, B, C, D`. -/
structure State where
  a : UInt32
  b : UInt32
  c : UInt32
  d : UInt32

/-- RFC 1321 §3.3's initial state. -/
def State.init : State := ⟨0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476⟩

def rotl (x c : UInt32) : UInt32 := (x <<< c) ||| (x >>> (32 - c))

/-- Little-endian 32-bit word `i` of the block at byte offset `off`. -/
def word (m : ByteArray) (off i : Nat) : UInt32 :=
  let byte (k : Nat) : UInt32 := (m[off + 4 * i + k]?.getD 0).toUInt32
  byte 0 ||| (byte 1 <<< 8) ||| (byte 2 <<< 16) ||| (byte 3 <<< 24)

/-- Operation `i` (0 ≤ i < 64) of the compression function. -/
def step (m : ByteArray) (off : Nat) (i : Nat) (s : State) : State :=
  let (f, g) :=
    if i < 16 then ((s.b &&& s.c) ||| (~~~s.b &&& s.d), i)
    else if i < 32 then ((s.d &&& s.b) ||| (~~~s.d &&& s.c), (5 * i + 1) % 16)
    else if i < 48 then (s.b ^^^ s.c ^^^ s.d, (3 * i + 5) % 16)
    else (s.c ^^^ (s.b ||| ~~~s.d), (7 * i) % 16)
  let x := s.a + f + T.getD i 0 + word m off g
  { a := s.d, b := s.b + rotl x (S.getD i 0), c := s.b, d := s.c }

/-- Operations `i … i+n-1`. -/
def steps (m : ByteArray) (off : Nat) : Nat → Nat → State → State
  | 0, _, s => s
  | n + 1, i, s => steps m off n (i + 1) (step m off i s)

/-- One 64-byte block at offset `off`. -/
def block (m : ByteArray) (off : Nat) (s : State) : State :=
  let r := steps m off 64 0 s
  ⟨s.a + r.a, s.b + r.b, s.c + r.c, s.d + r.d⟩

/-- Blocks `k … k+n-1`. -/
def blocks (m : ByteArray) : Nat → Nat → State → State
  | 0, _, s => s
  | n + 1, k, s => blocks m n (k + 1) (block m (64 * k) s)

/-- `n` zero bytes pushed onto `b`. -/
def pushZeros (b : ByteArray) : Nat → ByteArray
  | 0 => b
  | n + 1 => pushZeros (b.push 0) n

/-- The 8 little-endian bytes of `v` pushed onto `b`. -/
def pushLE64 (b : ByteArray) (v : Nat) : ByteArray :=
  (List.range 8).foldl (fun acc k => acc.push (v / 2 ^ (8 * k) % 256).toUInt8) b

/-- RFC 1321 §3.1–3.2: `0x80`, zeros to 56 mod 64, the bit length mod 2^64. -/
def pad (msg : ByteArray) : ByteArray :=
  let len := msg.size
  pushLE64 (pushZeros (msg.push 0x80) ((55 + 64 - len % 64) % 64)) (8 * len % 2 ^ 64)

/-- The 16 digest bytes: the little-endian bytes of `A, B, C, D`. -/
def digestBytes (msg : ByteArray) : List UInt8 :=
  let m := pad msg
  let s := blocks m (m.size / 64) 0 State.init
  ([s.a, s.b, s.c, s.d] : List UInt32).flatMap fun (w : UInt32) =>
    [w.toUInt8, (w >>> 8).toUInt8, (w >>> 16).toUInt8, (w >>> 24).toUInt8]

def hexDigit (n : UInt8) : Char :=
  if n < 10 then Char.ofNat (48 + n.toNat) else Char.ofNat (87 + n.toNat)

/-- Lowercase hex, two digits per byte (OCaml `Digest.to_hex`'s `%02x`). -/
def hex (bs : List UInt8) : String :=
  String.ofList (bs.flatMap fun (b : UInt8) => [hexDigit (b >>> 4), hexDigit (b &&& 0xf)])

end MD5

/-- MD5 of a string's UTF-8 bytes as 32-char lowercase hex (RFC 1321).
    Equals OCaml `Digest.to_hex (Digest.string s)`. Pure and total since
    ship-your-tcc F1b (2026-10-21): it replaces the native MD5 extern.
    Checked against RFC 1321 and Python hashlib vectors in
    test/Unit/FreshIntTest.lean, including padding boundaries and UTF-8;
    the kernel computes the empty-string and abc vectors there. -/
def md5Hex (s : String) : String := MD5.hex (MD5.digestBytes s.toUTF8)

/-- The frontend's per-TU symbol digest, as an instance-implicit parameter.
    Corresponds to: the `Cerb_fresh.digest` ref cell (util/cerb_fresh.ml:7-10),
    which `Cerb_fresh.set_digest` writes before each TU's frontend
    (backend/common/pipeline.ml:181) and every `Symbol.fresh` reads
    (symbol.lem:238 `Symbol (digest()) …`).

    THE DIGEST IS A PARAMETER (ship-your-tcc F1b, DESIGN-COMPILER §3.4,
    2026-10-21): lem's instance reader `declare {lean} reader val
    Symbol.digest = instance `CerberusFresh.Digest.digest``
    (frontend/model/lean_digest.lem; the `CerbGlobal.Switches` mechanism)
    makes every read of `Symbol.digest ()` the projection
    `CerberusFresh.Digest.digest`, and every definition that transitively
    reads it binds `[CerberusFresh.Digest]`. NO instance exists in the
    library, the seams or the generated tree: the driver supplies the TU's
    digest per TU (`CerbDriver.frontendTU`), a statement or theorem supplies
    its own (`letI : CerberusFresh.Digest := ⟨d⟩`, or `@f ⟨d⟩`).

    DELETED with it: the native mutable global (`cerb_digest_get`/
    `cerb_digest_set`), its kernel-opaque reader `digest` (`opaque … :=
    fun _ => ""` implemented by the global), the BaseIO accessors
    `digestIO`/`setDigestIO`, and the evaluation barrier `forceIO`
    (`cerb_force_thunk`) that kept the compiler from sinking a pure stage
    call past the next TU's set. With the digest an argument of the stage
    calls there is no state to order against, and the kernel sees which
    digest each desugared symbol carries. native/md5.c and the
    `CSemNative` Lake package are gone. -/
class Digest where
  digest : String

end CerberusFresh
