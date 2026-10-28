import CerbStopSites
/-
  Hand-written Lean implementations for Cerberus implementation-defined behaviour.
  Corresponds to: ocaml_frontend/ocaml_implementation.ml (DefaultImpl module).
  No process state: the enum's compatible type is program data (an ambient reader of
  the lem model, see below); every function here is a function of its arguments.

  These functions are referenced by `declare lean target_rep` in implementation.lem
  and work with the Lem-generated types from IntegerType.lean and Ctype.lean.

  ABI: a PARAMETER since ship-your-tcc F1 (DESIGN-COMPILER.md §3.3): every
  layout/signedness fact below reads `CerbImplCfg.Impl.params` (the instance
  reader `impl_params`, frontend/model/lean_impl.lem). At
  `CerbImplCfg.cerberusDefault` each function is Cerberus DefaultImpl's
  (LP64, x86_64-apple-darwin) exactly as before; `CerbImplCfg.tccRV64` is
  tcc's riscv64 lp64 ABI.
-/

import IntegerType
import Ctype
import CerbImplCfg

namespace CerberusImpl

def bitfields_supported [CerbImplCfg.Impl] (_ : Unit) : Bool :=
  CerbImplCfg.Impl.params.bitfields == .tcc


/-- Builtins.translate_errno: retain the complete upstream name domain, but
read the pinned newlib 4.5.0 sys/errno.h values for RV64. Unknown names fail
as they do in builtins.lem. The focused probe checks observable values. -/
def translate_errno [CerbImplCfg.Impl] : String → Int
  | "__cerbvar_EDOM" => if CerbImplCfg.Impl.params.newlibErrno then 33 else 1
  | "__cerbvar_EILSEQ" => if CerbImplCfg.Impl.params.newlibErrno then 138 else 2
  | "__cerbvar_ERANGE" => if CerbImplCfg.Impl.params.newlibErrno then 34 else 3
  | "__cerbvar_E2BIG" => if CerbImplCfg.Impl.params.newlibErrno then 7 else 4
  | "__cerbvar_EACCES" => if CerbImplCfg.Impl.params.newlibErrno then 13 else 5
  | "__cerbvar_EADDRINUSE" => if CerbImplCfg.Impl.params.newlibErrno then 112 else 6
  | "__cerbvar_EADDRNOTAVAIL" => if CerbImplCfg.Impl.params.newlibErrno then 125 else 7
  | "__cerbvar_EAFNOSUPPORT" => if CerbImplCfg.Impl.params.newlibErrno then 106 else 8
  | "__cerbvar_EAGAIN" => if CerbImplCfg.Impl.params.newlibErrno then 11 else 9
  | "__cerbvar_EALREADY" => if CerbImplCfg.Impl.params.newlibErrno then 120 else 10
  | "__cerbvar_EBADF" => if CerbImplCfg.Impl.params.newlibErrno then 9 else 11
  | "__cerbvar_EBADMSG" => if CerbImplCfg.Impl.params.newlibErrno then 77 else 12
  | "__cerbvar_EBUSY" => if CerbImplCfg.Impl.params.newlibErrno then 16 else 13
  | "__cerbvar_ECANCELED" => if CerbImplCfg.Impl.params.newlibErrno then 140 else 14
  | "__cerbvar_ECHILD" => if CerbImplCfg.Impl.params.newlibErrno then 10 else 15
  | "__cerbvar_ECONNABORTED" => if CerbImplCfg.Impl.params.newlibErrno then 113 else 16
  | "__cerbvar_ECONNREFUSED" => if CerbImplCfg.Impl.params.newlibErrno then 111 else 17
  | "__cerbvar_ECONNRESET" => if CerbImplCfg.Impl.params.newlibErrno then 104 else 18
  | "__cerbvar_EDEADLK" => if CerbImplCfg.Impl.params.newlibErrno then 45 else 19
  | "__cerbvar_EDESTADDRREQ" => if CerbImplCfg.Impl.params.newlibErrno then 121 else 20
  | "__cerbvar_EDQUOT" => if CerbImplCfg.Impl.params.newlibErrno then 132 else 21
  | "__cerbvar_EEXIST" => if CerbImplCfg.Impl.params.newlibErrno then 17 else 22
  | "__cerbvar_EFAULT" => if CerbImplCfg.Impl.params.newlibErrno then 14 else 23
  | "__cerbvar_EFBIG" => if CerbImplCfg.Impl.params.newlibErrno then 27 else 24
  | "__cerbvar_EHOSTUNREACH" => if CerbImplCfg.Impl.params.newlibErrno then 118 else 25
  | "__cerbvar_EIDRM" => if CerbImplCfg.Impl.params.newlibErrno then 36 else 26
  | "__cerbvar_EINPROGRESS" => if CerbImplCfg.Impl.params.newlibErrno then 119 else 27
  | "__cerbvar_EINTR" => if CerbImplCfg.Impl.params.newlibErrno then 4 else 28
  | "__cerbvar_EINVAL" => if CerbImplCfg.Impl.params.newlibErrno then 22 else 29
  | "__cerbvar_EIO" => if CerbImplCfg.Impl.params.newlibErrno then 5 else 30
  | "__cerbvar_EISCONN" => if CerbImplCfg.Impl.params.newlibErrno then 127 else 31
  | "__cerbvar_EISDIR" => if CerbImplCfg.Impl.params.newlibErrno then 21 else 32
  | "__cerbvar_ELOOP" => if CerbImplCfg.Impl.params.newlibErrno then 92 else 33
  | "__cerbvar_EMFILE" => if CerbImplCfg.Impl.params.newlibErrno then 24 else 34
  | "__cerbvar_EMLINK" => if CerbImplCfg.Impl.params.newlibErrno then 31 else 35
  | "__cerbvar_EMSGSIZE" => if CerbImplCfg.Impl.params.newlibErrno then 122 else 36
  | "__cerbvar_EMULTIHOP" => if CerbImplCfg.Impl.params.newlibErrno then 74 else 37
  | "__cerbvar_ENAMETOOLONG" => if CerbImplCfg.Impl.params.newlibErrno then 91 else 38
  | "__cerbvar_ENETDOWN" => if CerbImplCfg.Impl.params.newlibErrno then 115 else 39
  | "__cerbvar_ENETRESET" => if CerbImplCfg.Impl.params.newlibErrno then 126 else 40
  | "__cerbvar_ENETUNREACH" => if CerbImplCfg.Impl.params.newlibErrno then 114 else 41
  | "__cerbvar_ENFILE" => if CerbImplCfg.Impl.params.newlibErrno then 23 else 42
  | "__cerbvar_ENOBUFS" => if CerbImplCfg.Impl.params.newlibErrno then 105 else 43
  | "__cerbvar_ENODATA" => if CerbImplCfg.Impl.params.newlibErrno then 61 else 44
  | "__cerbvar_ENODEV" => if CerbImplCfg.Impl.params.newlibErrno then 19 else 45
  | "__cerbvar_ENOENT" => if CerbImplCfg.Impl.params.newlibErrno then 2 else 46
  | "__cerbvar_ENOEXEC" => if CerbImplCfg.Impl.params.newlibErrno then 8 else 47
  | "__cerbvar_ENOLCK" => if CerbImplCfg.Impl.params.newlibErrno then 46 else 48
  | "__cerbvar_ENOLINK" => if CerbImplCfg.Impl.params.newlibErrno then 67 else 49
  | "__cerbvar_ENOMEM" => if CerbImplCfg.Impl.params.newlibErrno then 12 else 50
  | "__cerbvar_ENOMSG" => if CerbImplCfg.Impl.params.newlibErrno then 35 else 51
  | "__cerbvar_ENOPROTOOPT" => if CerbImplCfg.Impl.params.newlibErrno then 109 else 52
  | "__cerbvar_ENOSPC" => if CerbImplCfg.Impl.params.newlibErrno then 28 else 53
  | "__cerbvar_ENOSR" => if CerbImplCfg.Impl.params.newlibErrno then 63 else 54
  | "__cerbvar_ENOSTR" => if CerbImplCfg.Impl.params.newlibErrno then 60 else 55
  | "__cerbvar_ENOSYS" => if CerbImplCfg.Impl.params.newlibErrno then 88 else 56
  | "__cerbvar_ENOTCONN" => if CerbImplCfg.Impl.params.newlibErrno then 128 else 57
  | "__cerbvar_ENOTDIR" => if CerbImplCfg.Impl.params.newlibErrno then 20 else 58
  | "__cerbvar_ENOTEMPTY" => if CerbImplCfg.Impl.params.newlibErrno then 90 else 59
  | "__cerbvar_ENOTRECOVERABLE" => if CerbImplCfg.Impl.params.newlibErrno then 141 else 60
  | "__cerbvar_ENOTSOCK" => if CerbImplCfg.Impl.params.newlibErrno then 108 else 61
  | "__cerbvar_ENOTSUP" => if CerbImplCfg.Impl.params.newlibErrno then 134 else 62
  | "__cerbvar_ENOTTY" => if CerbImplCfg.Impl.params.newlibErrno then 25 else 63
  | "__cerbvar_ENXIO" => if CerbImplCfg.Impl.params.newlibErrno then 6 else 64
  | "__cerbvar_EOPNOTSUPP" => if CerbImplCfg.Impl.params.newlibErrno then 95 else 65
  | "__cerbvar_EOVERFLOW" => if CerbImplCfg.Impl.params.newlibErrno then 139 else 66
  | "__cerbvar_EOWNERDEAD" => if CerbImplCfg.Impl.params.newlibErrno then 142 else 67
  | "__cerbvar_EPERM" => if CerbImplCfg.Impl.params.newlibErrno then 1 else 68
  | "__cerbvar_EPIPE" => if CerbImplCfg.Impl.params.newlibErrno then 32 else 69
  | "__cerbvar_EPROTO" => if CerbImplCfg.Impl.params.newlibErrno then 71 else 70
  | "__cerbvar_EPROTONOSUPPORT" => if CerbImplCfg.Impl.params.newlibErrno then 123 else 71
  | "__cerbvar_EPROTOTYPE" => if CerbImplCfg.Impl.params.newlibErrno then 107 else 72
  | "__cerbvar_EROFS" => if CerbImplCfg.Impl.params.newlibErrno then 30 else 73
  | "__cerbvar_ESPIPE" => if CerbImplCfg.Impl.params.newlibErrno then 29 else 74
  | "__cerbvar_ESRCH" => if CerbImplCfg.Impl.params.newlibErrno then 3 else 75
  | "__cerbvar_ESTALE" => if CerbImplCfg.Impl.params.newlibErrno then 133 else 76
  | "__cerbvar_ETIME" => if CerbImplCfg.Impl.params.newlibErrno then 62 else 77
  | "__cerbvar_ETIMEDOUT" => if CerbImplCfg.Impl.params.newlibErrno then 116 else 78
  | "__cerbvar_ETXTBSY" => if CerbImplCfg.Impl.params.newlibErrno then 26 else 79
  | "__cerbvar_EWOULDBLOCK" => if CerbImplCfg.Impl.params.newlibErrno then 11 else 80
  | "__cerbvar_EXDEV" => if CerbImplCfg.Impl.params.newlibErrno then 18 else 81
  | name => failwithI ("Unknown errno code: " ++ name)


/-! ## Target Configuration -/

/-- Maximum alignment in bytes (LP64).
    Corresponds to: DefaultImpl.max_alignment in ocaml_implementation.ml:151-152 -/
def max_alignment [CerbImplCfg.Impl] : Nat := CerbImplCfg.Impl.params.maxAlignment

/-- DefaultImpl.sizeof_pointer / alignof_pointer — ocaml_implementation.ml:
    117-121 `Some 8` (the record fields impl_mem.ml reads at :153-158,
    :219-225, :1160-1164, :2134; zero-discrepancy literal census #2 — CerbMem
    reads these instead of its own literal). -/
def sizeof_pointer [CerbImplCfg.Impl] : Option Nat := some CerbImplCfg.Impl.params.sizeofPointer
def alignof_pointer [CerbImplCfg.Impl] : Option Nat := some CerbImplCfg.Impl.params.alignofPointer

/-! ## Integer Type Properties -/

/-! ## The enum's compatible type is PROGRAM DATA (program-data parameters E-A, 2026-09-20;
    `docs/2026-09-20_program-data-parameters-EA-DA-record.md`)

The OCaml keeps DefaultImpl's mutable `registered_enums` (ocaml_implementation.ml:
124-150: `register_enum` decides GCC's rule and pushes, `typeof_enum` looks it up
and fails on an unregistered tag); the Lean side has NO registry and no
effect-erased read any more: the map from enum tag to its compatible integer
type is an ambient READER of the lem model (`declare {lean} reader val
enum_definitions`, implementation.lem) — the desugarer records the decided
type in `A.sigma.enum_definitions`, the elaborator carries it into the Core
file's `enumDefs`, linking merges it, and the entries pass the map wherever
the generated code takes `_lemReader_enum_definitions`. The lem-side layout
vals (`Implementation.sizeof_ity` etc.) are SHARED wrappers that normalise
FIRST through the one reader consumer `Implementation.normalise_integerType`
(its rep `normalise_integerType` below) and then call the type-only reps
below on the normalised type; the hand-written memory model receives the map
through its `reader_consumer` stubs (CerbMem) and resolves an `Enum` exactly
where impl_mem.ml resolves it through the registry. Keys compare by
`symbolEquality` (digest + number, description-insensitive — the
`Symbol.symbol_compare` parity of ocaml_implementation.ml:137,145). -/

/-- The enum map: tag ↦ its compatible integer type (the value of the lem reader
    `Implementation.enum_definitions`; the same information the OCaml registry
    holds, as data). -/
abbrev EnumDefs := Fmap sym integerType

/-- Fail-closed stub backing implementation.lem's `enum_definitions` target-coverage
    rep. The reader mechanism rewrites every applied `enum_definitions ()` site to the
    `_lemReader_enum_definitions` parameter, so this constant is never emitted at an
    applied call site; if it ever runs, that is a reader-lifting defect — fail loudly
    (the CerbTags.tagDefsUnreachable shape; `failwithI`, seam hygiene 2026-09-19). -/
def enumDefinitionsUnreachable (_ : Unit) : EnumDefs :=
  failwithI (CerbStop.render (.invariant (CerbStopSites.enumReader "CerberusImpl.enumDefinitionsUnreachable: applied enum_definitions () site survived reader lifting")))

/-- The named lookup — `Ocaml_implementation.typeof_enum` (ocaml_implementation.ml:
    144-150) over the map instead of the registry: the registered compatible type,
    or the oracle's `failwith` text on an unregistered tag (REACHABLE on a Core-text
    input naming an enum tag no C source declared — the .core path's map is empty, as
    the oracle's registry is there). -/
def lookupEnum (enumDefs : EnumDefs) (tag_sym : sym) : integerType :=
  -- the spine scan with `symbolEquality` (digest + number, description-insensitive) —
  -- the same lookup shape as the tag lookups (CerbTagsWf.lookupEntry) and the
  -- `Symbol.symbol_compare` parity of the OCaml registry (ocaml_implementation.ml:145)
  match (fmapElements enumDefs).find? (fun (k, _) => symbolEquality k tag_sym) with
  | some (_, ity) => ity
  | none => failwithI (CerbStop.render (.upstreamCrash (CerbStopSites.enumMissing s!"Ocaml_implementation.typeof_enum: '{show_symbol tag_sym}' was not registered")))

/-- Resolve an `Enum` through the map and leave every other type as it is — the
    `match ity with Enum tag_sym -> typeof_enum tag_sym | _ -> ity` step the OCaml
    layout functions perform before their own match (ocaml_implementation.ml:80-85,
    :56-57; impl_mem.ml:2369-2372, :2407-2410). -/
def resolveEnum (enumDefs : EnumDefs) : integerType → integerType
  | .Enum0 tag_sym => lookupEnum enumDefs tag_sym
  | ity => ity

/-- register_enum — the Lean rep of `Implementation.register_enum`: the pure `true`.
    The OCaml rep still writes DefaultImpl's registry (ocaml_implementation.ml:129-142)
    and answers `false` on a duplicate tag; on Lean the compatible type is recorded in
    the sigma by the desugarer instead, and the duplicate case is caught by the
    `Map.member tag_sym st.tag_definitions` check just before this call
    (cabs_to_ail_effect.lem register_tag_definition) — so both engines take the same
    branch and this value is never consulted. -/
def register_enum (_ : sym) (_ : List Int) : Bool := true

/-- The compatible integer type of an enumerated type with enumerator values
    `ns` — the implementation parameter `enumRule` (implementation.lem's
    shared body is the `gcc` rule). `tcc`: tccgen.c struct_decl ("set integral
    type of the enum"): no negative value → `unsigned int`, or `unsigned long`
    if the largest exceeds `UINT_MAX`; else `int`, or `long` if a value is
    outside `int`. -/
def enum_compatible_type [CerbImplCfg.Impl] (ns : List Int) : integerType :=
  let anyNeg := ns.any (fun n => n < 0)
  match CerbImplCfg.Impl.params.enumRule with
  | .gcc => if anyNeg then .Signed .Int_ else .Unsigned .Int_
  | .tcc =>
    if !anyNeg then
      if ns.any (fun n => n > 4294967295) then .Unsigned .Long else .Unsigned .Int_
    else if ns.any (fun n => n > 2147483647 || n < -2147483648) then .Signed .Long
    else .Signed .Int_

/-! ## Integer Type Properties -/

/-- Whether an integer type is signed.
    Corresponds to: Common.is_signed_ity in ocaml_implementation.ml:79-107,
    instantiated with ~typeof_enum ~char_is_signed:true as DefaultImpl does
    (ocaml_implementation.ml:257). OCaml first resolves Enum through
    typeof_enum, then matches; on Lean the resolution happens BEFORE this
    function — the lem wrapper `Implementation.is_signed_ity` resolves ONLY an
    enum through the reader (`Implementation.resolve_enum`, the OCaml's own
    shape; NOT the alias table — audit E4: `Signed (IntN_t 128)` is `true` on
    both engines), CerbMem resolves through `resolveEnum` — so the `Enum` arm
    here is the OCaml's `assert false`: unreachable by construction, a loud
    leaf. -/
def is_signed_ity [CerbImplCfg.Impl] (ity : integerType) : Bool :=
  match ity with
  | .Char0 => CerbImplCfg.Impl.params.charSigned  -- DefaultImpl: char_is_signed = true
  | .Bool0 => false
  | .Signed _ => true
  | .Unsigned _ => false
  | .Enum0 _ => failwithI (CerbStop.render (.upstreamCrash (CerbStopSites.signedEnum "assert false: Common.is_signed_ity reached an un-normalised Enum (ocaml_implementation.ml:95-96; the callers resolve it first)")))
  | .Size_t => false    -- STD §7.19#2
  | .Wchar_t => true
  | .Wint_t => true
  | .Ptrdiff_t => true  -- STD §7.19#2
  | .Ptraddr_t => false

/-! ## Type normalisation — the alias table and `normalise_integerType_`

    DefaultImpl's `type_alias_map` (ocaml_implementation.ml:154-171): the
    three N-families alias through ONE function `n_t_aliases` (8 → Ichar,
    16 → Short, 32 → Int_, 64 → Long, `_ → None`); `Intmax_t`/`Intptr_t` →
    Long; `wchar_t`/`wint_t` → Signed Int_; `size_t` → Unsigned Long;
    `ptrdiff_t` → Signed Long. `Common.normalise_integerType_` (:37-66)
    applies `Option.get` to the N-family lookups (:39-44), so a width
    outside {8, 16, 32, 64} — `__cerbty_int128_t`, builtins.lem:19/53 —
    is an uncaught `Invalid_argument "option is None"` on the oracle,
    mirrored as a fail-stop (zero-discrepancy Z2-I-03, ruling Q4).
    Zero-discrepancy Z2-I-01: the previous `normalise_integerType` had NO
    `Signed/Unsigned (IntN_t | Int_leastN_t | Int_fastN_t | Intmax_t |
    Intptr_t)` aliasing, so ailTypesAux.lem:302-303's `(Signed (IntN_t _),
    _) -> fail ()` arms — "inaccessible because of the normalisation" on
    the oracle — were reachable on Lean through the DIRECT `__cerbty_intN_t`
    spellings (the shared <stdint.h> typedefs int32_t as plain `signed
    int`, so ordinary C never produced IntN_t): pin
    tests/immaculate/nolibc/zd-z2i01-cerbty-int32-uac.c. Z2-I-02: the
    previous `.Ptraddr_t => .Unsigned .Long` arm computed where the
    OCaml's `| ity -> ity` (:65-66) keeps Ptraddr_t (and the shared model
    then errors "WIP … Ptraddr_t") — mirrored. -/

/-- `n_t_aliases` — ocaml_implementation.ml:155-160. -/
def n_t_aliases : Nat → Option integerBaseType
  | 8 => some .Ichar
  | 16 => some .Short
  | 32 => some .Int_
  | 64 => some .Long
  | _ => none

/-- `aux_ibty` of `Common.normalise_integerType_` — ocaml_implementation.ml:38-50,
    with `Option.get` (:40/:42/:44) mirrored as a fail-stop. -/
def aux_ibty : integerBaseType → integerBaseType
  | .IntN_t n | .Int_leastN_t n | .Int_fastN_t n =>
    match n_t_aliases n with
    | some ibty => ibty
    -- KIND classification left to the operator (Z2 record §10): the model
    -- WRITES `Option.get` on its own alias table (a deliberate "must be
    -- aliased" assertion, kind 1) but the missing entry is a width the
    -- implementation never defined (kind 2 shape); either reading gives no
    -- meaning to `__cerbty_int128_t` on DefaultImpl — a loud fail-stop
    | none => failwithI (CerbStop.render (.upstreamCrash (CerbStopSites.aliasMissing s!"DefaultImpl.type_alias_map has no alias for an N-family width of {n} (ocaml_implementation.ml:39-44, :155-160: Option.get raises on the oracle); zero-discrepancy Z2-I-03")))
  | .Intmax_t => .Long     -- :45-46, :162
  | .Intptr_t => .Long     -- :47-48, :163
  | ibty => ibty           -- :49-50

/-- The alias part of `Common.normalise_integerType_` (ocaml_implementation.ml:37-66
    instantiated with DefaultImpl's type_alias_map, :154-171): every arm but the
    Enum one. An `Enum` cannot be normalised without the program's enum map, so
    here it is the UNREACHABLE-BY-INVARIANT leaf: the type-only layout reps below
    are called on types the lem wrappers (`Implementation.sizeof_ity` etc.) have
    already normalised through `normalise_integerType`, and CerbMem resolves an
    Enum through `resolveEnum` before it reaches them. -/
def normalise_aliases : integerType → integerType
  | .Signed ibty => .Signed (aux_ibty ibty)      -- :52-53
  | .Unsigned ibty => .Unsigned (aux_ibty ibty)  -- :54-55
  | .Enum0 _ => failwithI (CerbStop.render (.invariant (CerbStopSites.normaliseEnum "CerberusImpl.normalise_aliases: an un-normalised Enum reached a type-only layout rep (Implementation.normalise_integerType / CerbMem resolveEnum normalise first — unreachable by construction)")))
  | .Wchar_t => .Signed .Int_                    -- :58-59, :164 (TODO: check — upstream's own note)
  | .Wint_t => .Signed .Int_                     -- :60-61, :165
  | .Size_t => .Unsigned .Long                   -- :62-63, :166
  | .Ptrdiff_t => .Signed .Long                  -- :64-65, :167
  | ity => ity                                   -- :65-66 (Char, Bool, Ptraddr_t)

/-- `Common.normalise_integerType_` in full, over the enum map: the Enum arm is
    the lookup (:56-57 `Enum tag_sym -> typeof_enum tag_sym`), every other arm the
    alias table. -/
def normalise_integerType_in (enumDefs : EnumDefs) : integerType → integerType
  | .Enum0 tag_sym => lookupEnum enumDefs tag_sym
  | ity => normalise_aliases ity

/-- THE reader consumer (`declare {lean} reader_consumer val normalise_integerType`,
    implementation.lem): the generated call sites pass every declared reader in the
    global sorted order — `enum_definitions`, then `tagDefs` — before the type; only
    the enum map is consulted. -/
def normalise_integerType (enumDefs : EnumDefs)
    (_tagDefs : Fmap sym (CerbLocation.Loc × tag_definition)) (ity : integerType) : integerType :=
  normalise_integerType_in enumDefs ity

/-- Size of an integer type in bytes.
    Corresponds to: DefaultImpl.sizeof_ity in ocaml_implementation.ml:172-201,
    arm for arm: the type is normalised FIRST (:173), so the N-families,
    Intmax_t/Intptr_t, Enum, wchar_t/wint_t/size_t/ptrdiff_t never reach
    the match — their arms are `assert false` (:188-193, :195-200) and are
    mirrored as fail-stops (unreachable after a total normalisation). Since
    E-A (2026-09-20) the normalisation here is the ALIAS part only: an Enum is
    resolved before this function (the lem wrapper / CerbMem's resolveEnum),
    so this is a function of the type alone.
    Zero-discrepancy Z2-I-01/03: the previous per-width arithmetic
    (`(n+7)/8`, nested `if`s) computed a size for EVERY width where the
    oracle aliases four widths and crashes on the rest. -/
def sizeof_ity [CerbImplCfg.Impl] (ity : integerType) : Option Nat :=
  let p := CerbImplCfg.Impl.params
  match normalise_aliases ity with
  | .Char0 | .Bool0 => some 1                                            -- :174-176
  | .Signed ibty | .Unsigned ibty =>                                     -- :177-193
    some (match ibty with
      | .Ichar => 1
      | .Short => p.sizeofShort
      | .Int_ => p.sizeofInt
      | .Long => p.sizeofLong
      | .LongLong => p.sizeofLongLong
      | .IntN_t _ | .Int_leastN_t _ | .Int_fastN_t _ | .Intmax_t | .Intptr_t =>
        failwithI (CerbStop.render (.upstreamCrash (CerbStopSites.sizeofBase "assert false: DefaultImpl.sizeof_ity reached an un-normalised base type (ocaml_implementation.ml:188-193)"))))
  | .Enum0 _ | .Wchar_t | .Wint_t | .Size_t | .Ptrdiff_t =>
    failwithI (CerbStop.render (.upstreamCrash (CerbStopSites.sizeofType "assert false: DefaultImpl.sizeof_ity reached an un-normalised type (ocaml_implementation.ml:195-200)")))
  | .Ptraddr_t => some p.sizeofPointer                                                 -- :201

/-- Precision (number of value bits) of an integer type.
    Corresponds to: Common.precision_ity in ocaml_implementation.ml -/
def precision_ity [CerbImplCfg.Impl] (ity : integerType) : Option Nat :=
  match sizeof_ity ity with
  | some n =>
    if is_signed_ity ity then some (8 * n - 1) else some (8 * n)
  | none => none

/-- Size of a floating type in bytes.
    Corresponds to: DefaultImpl.sizeof_fty in ocaml_implementation.ml:206-212,
    which returns 8 for ALL three real floating types — including its
    literal `(* TODO:hack ==> 4 *)` on Float and `(* TODO:hack ==> 16 *)`
    on LongDouble (OCaml's float IS 64-bit, so every floating_value is
    represented in 8 bytes; the natural 4/16 sizes are explicitly hacked
    to 8 there). We mirror the BEHAVIOR, hack included, for byte-level
    parity with the OCaml concrete memory model. -/
def sizeof_fty [CerbImplCfg.Impl] : floatingType → Option Nat
  | .RealFloating .Float0 => some CerbImplCfg.Impl.params.float.size      -- DefaultImpl: Some 8 (* TODO:hack ==> 4 *)
  | .RealFloating .Double => some CerbImplCfg.Impl.params.double.size
  | .RealFloating .LongDouble => some CerbImplCfg.Impl.params.longDouble.size  -- DefaultImpl: Some 8 (* TODO:hack ==> 16 *)

/-- The value format of a real floating type (ship-your-tcc F1: CerbImplCfg;
    DefaultImpl holds all three in an OCaml float, binary64). -/
def format_fty [CerbImplCfg.Impl] : floatingType → CerbImplCfg.FloatFormat
  | .RealFloating .Float0 => CerbImplCfg.Impl.params.float.format
  | .RealFloating .Double => CerbImplCfg.Impl.params.double.format
  | .RealFloating .LongDouble => CerbImplCfg.Impl.params.longDouble.format

/-- Alignment of an integer type in bytes.
    Corresponds to: DefaultImpl.alignof_ity in ocaml_implementation.ml:214-243
    — textually the same table as sizeof_ity (:172-201), normalisation
    first, `assert false` on the un-normalised arms; hence `= sizeof_ity`. -/
def alignof_ity [CerbImplCfg.Impl] (ity : integerType) : Option Nat :=
  let p := CerbImplCfg.Impl.params
  match normalise_aliases ity with
  | .Char0 | .Bool0 => some 1
  | .Signed ibty | .Unsigned ibty =>
    some (match ibty with
      | .Ichar => 1
      | .Short => p.alignofShort
      | .Int_ => p.alignofInt
      | .Long => p.alignofLong
      | .LongLong => p.alignofLongLong
      | .IntN_t _ | .Int_leastN_t _ | .Int_fastN_t _ | .Intmax_t | .Intptr_t =>
        failwithI "assert false: DefaultImpl.alignof_ity reached an un-normalised base type (ocaml_implementation.ml:230-235)")
  | .Enum0 _ | .Wchar_t | .Wint_t | .Size_t | .Ptrdiff_t =>
    failwithI "assert false: DefaultImpl.alignof_ity reached an un-normalised type (ocaml_implementation.ml:237-242)"
  | .Ptraddr_t => some p.alignofPointer

/-- Alignment of a floating type in bytes.
    Corresponds to: DefaultImpl.alignof_fty in ocaml_implementation.ml:247-253
    — 8 for all three real floating types, same TODO:hack comments as
    sizeof_fty (see above); behavior mirrored, hack included. -/
def alignof_fty [CerbImplCfg.Impl] : floatingType → Option Nat
  | .RealFloating .Float0 => some CerbImplCfg.Impl.params.float.align      -- DefaultImpl: Some 8 (* TODO:hack ==> 4 *)
  | .RealFloating .Double => some CerbImplCfg.Impl.params.double.align
  | .RealFloating .LongDouble => some CerbImplCfg.Impl.params.longDouble.align  -- DefaultImpl: Some 8 (* TODO:hack ==> 16 *)

/-- Alignment of a full ctype, including struct/union via tag definitions.
    Corresponds to: Ocaml_implementation.alignof_proxy in ocaml_implementation.ml
    (:444-513 `alignof ?tagDefs`). The pseudo tagDefs maps ail_identifier (= sym)
    to member lists. A reader CONSUMER (`declare {lean} reader_consumer val
    alignof_ty`, implementation.lem — record erratum W1): the OCaml resolves an
    integer member type with `(get ()).alignof_ity`, i.e. through the enum
    registry, so this rep takes the readers (`enum_definitions`, `tagDefs`, sorted
    order) and resolves an Enum through the map before the type-only `alignof_ity`. -/
partial def alignof_ty [CerbImplCfg.Impl] (enumDefs : EnumDefs)
    (_tagDefs : Fmap sym (CerbLocation.Loc × tag_definition))
    (tagDefs : Fmap sym (aggregate_layout × List (Option alignment × ctype)))
    (ty : ctype) : Option Nat :=
  let lookupTag (tag : sym) : Option (aggregate_layout × List (Option alignment × ctype)) :=
    -- arc-6 S3: Fmap is no longer a raw assoc list; scan the enumerated
    -- spine with the SAME BEq predicate as before (bit-identical result).
    (fmapElements tagDefs).find? (fun (k, _) => k == tag) |>.map Prod.snd
  let foldMembers (layout : aggregate_layout) (members : List (Option alignment × ctype)) : Option Nat :=
    members.foldl (fun acc_opt (align_opt, mty) =>
      let al_opt := if is_unnamed_bitfield mty then some 1 else match effective_member_alignment layout.aggregate_packed align_opt with
        | none => alignof_ty enumDefs _tagDefs tagDefs mty
        -- ISO-fix register R7 (VALIDATION.md §2; the R3 class): upstream
        -- ocaml_implementation.ml:483/:501 is `Some (Z.to_int al_n)` (Z.Overflow
        -- on a huge _Alignas); the Lem signature (implementation.lem:27,
        -- `-> maybe nat`) is unbounded, so the unbounded value is kept here.
        | some (AlignInteger n) => some n.toNat
        | some (AlignType al_ty) => alignof_ty enumDefs _tagDefs tagDefs al_ty
      match acc_opt, al_opt with
      | some acc, some al => some (max al acc)
      | _, _ => none
    ) (some 1)
  let annots := match ty with | Ctype annots _ => annots
  match gnu_typedef_alignment annots with
  | some n => some n.toNat
  | none =>
    match ty with
    | Ctype _ Void0 => none
    | Ctype _ (Basic (Integer ity)) => alignof_ity (resolveEnum enumDefs ity)
    | Ctype _ (Basic (Floating fty)) => alignof_fty fty
    | Ctype _ (Array0 elem_ty _) => alignof_ty enumDefs _tagDefs tagDefs elem_ty
    | Ctype _ (Function _ _ _) => none
    | Ctype _ (FunctionNoParams _) => none
    | Ctype _ (Pointer _ _) => some CerbImplCfg.Impl.params.alignofPointer  -- DefaultImpl: 8 (LP64)
    | Ctype _ (Atomic atom_ty) => alignof_ty enumDefs _tagDefs tagDefs atom_ty
    | Ctype _ (Struct tag_sym) =>
      match lookupTag tag_sym with
      | some (layout, members) => (foldMembers layout members).map (aggregate_align layout)
      | none => none
    | Ctype _ (Union0 tag_sym) =>
      match lookupTag tag_sym with
      | some (layout, members) => (foldMembers layout members).map (aggregate_align layout)
      | none => none
    | Ctype _ Byte => some 1

/-! ## alignof_ty fail paths — DECLARED (zero-discrepancy Z2-I-04)

    `alignof_ty` above answers `none` for `void`, function types and an
    unknown struct/union tag where `Ocaml_implementation.alignof_proxy`
    (ocaml_implementation.ml:446-447, :464-466 `assert false`; :491/:509
    `Pmap.find` → `Not_found`) crashes. Reachability: Ail typing rejects
    `sizeof`/`alignof`/`_Alignas` of void, function and incomplete types
    before elaboration (the shared front end), and every complete struct/
    union tag is in the tag map it was elaborated with; the `none` is
    consumed by the layout family's own panics (CerbMem.alignofCtype
    "requires a complete implementation alignof …"). Declared, not
    mirrored: a crash-for-crash change on an unreachable input. -/

end CerberusImpl
