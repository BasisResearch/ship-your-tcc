# GNU typedef alignment: compiler and executable model

Patch 0003 and the model now preserve declared GNU typedef alignment in valid
type queries, including chained aliases, qualifiers, `typeof`, nested pointer
referents, arrays and function declarators. The original full compiler on Sail
now prints **`4 16 16 32 | 4 1 1 5 | 16`**, agreeing with GCC. Sizes and the
original 16/1 member offsets remain unchanged. This is finite executable
validation, not a compiler correctness or representation theorem.

The work starts at `5ec6094`. The original failed RV64/full-compiler measurements
remain in [the pinned review](../review/gnu-typedef-alignment-2026-10-28.md).
Their archive and all 49 members were verified before reuse. A separately
rebuilt baseline compiler, using pristine vendor plus the original patches,
fails 24 of the final 32 positive native controls. Its actual source/binary
hashes, commands and observations are retained alongside the successful runs.

## Repair

`parse_btype` previously moved typedef attributes into `AttributeDef`; the
query recovered only `CType`, then inspected a popped vstack slot. The repair
uses three distinct pieces of information:

* `CType.aligned` carries GNU type/alias alignment independently of `type_size`.
  Symbol creation and partial reference copies preserve it. A pointer resets
  its outer alignment while retaining its referent's complete type. Arrays
  retain element alignment; function construction keeps the return type and
  clears inherited alignment before capturing the function's own attributes.
* `AttributeDef.aligned_from_typedef` distinguishes inherited alignment from
  an explicit declaration attribute. Derivation clears only inherited
  alignment; other symbol/function attributes keep their existing treatment.
  The array constraint checks element size against declared type alignment.
* `SValue.query_aligned` carries live declaration-expression alignment. An
  identifier supplies it; fresh values initialize it to zero. Address-taking,
  indirection, casts, operations, comma expressions and member selection clear
  it. `expr_type` captures it before popping, separately from the returned
  type. Neither popped entries nor a surviving relocation symbol supply it.

The corresponding Gen1–Gen5 actions mirror the C changes. Unary promotion and
common-type construction retain alias alignment only when the resulting type
retains that alias; explicit casts produce an ordinary outer value type.
`typeof(&x)` and direct `__alignof__(&x)` both retain pointer alignment even
when `x` is explicitly aligned. An aligned object does not change `typeof(x)`.
The parser's existing ghost actions and code-generation paths remain in place.

The maintained patch is generated against pristine TinyCC 43c7708, including
the small `riscv64-gen.c` return-type initialization hunk. Applying all patches
with `--fuzz=0` succeeds. No vendor, generated Lean, CSem, Supported/Fits or sum
cut source changed. `type_size` itself is unchanged.

## C construction and reset audit

The retained inventory covers 160 declarations in `tccgen`, `riscv64-gen`,
`tccelf`, `libtcc`, `tccrun`, `tccpp`, `tccasm` and `riscv64-asm`, plus an
overapproximate 924-row type/value assignment index. The source review follows
these initialization rules rather than assuming Lean defaults initialize C:

| family | rule checked in the C closure |
|---|---|
| canonical global types | static zero initialization; fixed canonical tag/reference initialization; pointer construction resets outer alignment; `func_vt` resets the complete type |
| fresh automatic CType values | zero initialization before tag/reference construction; initialized aggregate literals zero omitted fields |
| symbol pool / scope / label values | `sym_push2` zeroes the complete Sym; `sym_push`, external creation and partial type/reference patching copy alignment with the reference; copied symbols copy the whole record |
| fresh SValue constructors | `vsetc` copies the complete CType and initializes expression alignment; partial spill/move/store/atomic/conditional/structure-return scratch values are zero initialized |
| swap/rotation/duplicate and saved values | dominating whole-value copy; switch and initializer containers receive their saved value/type before use |
| RV64 backend | all three temporary Sym labels already use `{0}`; `origtype` is a dominating full copy and is restored; the other CType temporary copies the complete input; `gfunc_sret` explicitly initializes the fresh scalar return alignment |
| assembler adapters | temporary SValues copy complete operands before register/tag views; they do not construct types from uninitialized alignment fields |
| temporary backend tag views | load/store/register classification does not read the alignment fields; complete saved types restore expression-facing types; semantic operations clear expression metadata and select their result type |

A freshly compiled xPack RV64I/LP64 C probe checks `sizeof(CType)=24`, tag/ref/
alignment offsets `0/8/16`, `sizeof(SValue)=64`, and expression-alignment offset
`28`. The actual object and source are archived. All three materialized compiler
source trees have identical closure-file hashes.

`Tcc.Model.TypeAlignmentRepr_Statement imageBoundary` is an **open F6 obligation**.
`CTypeAt` and `AlignmentBoundaryRep` state exact byte fields, symbol-address
mapping and live expression metadata. F6 must supply actual pinned-image
reachability/synchronization boundaries; these tests do not instantiate them.
The ordinary axiom audit of the new definitions reports only `propext` and
`Quot.sound`. The rest of the compiler/state representation remains F6 work.

## Final checks

| check | observation |
|---|---|
| focused native GCC / patched tcc | 32 positive programs return/print the explicit expected result; seven invalid constraints diagnose and reject |
| compiler C11 lane | 181 positive and 22 negative cases pass, including preprocessing round trips and cross-object ISA checks |
| backend on Sail | 76/76 cross-tcc vs RV64 GCC program pairs match; all 76 host GCC controls match; zero forbidden-instruction objects/ELFs |
| focused independent fast arenas | 33 successful identical arenas; seven matching refusals reported separately; exit 0 |
| selected broad independent fast arenas | 1,496 identical arenas / 1,618 attempts; 122 separate refusals; no arena/model/harness differences; exit 0 |
| same-input semantic-probe arenas | 28/28 identical successful arenas |
| explicit capped build | `typedef-build-4`, exit 0; 3,195 source hashes verified |
| full repository/CSem gate | `typedef-gate-3`, exit 0; the same 3,195 source hashes verified |

The broad command explicitly selects `tests run tcc p3 compcert tcctests csmith
model c11 gen-cgen`. Its 1,200 C-generator inputs yield 1,082 successful arenas;
all 200 Csmith inputs in that selection match. These counts are a selected
finite comparison, not universal coverage or a replacement for existing gaps.
Refusals never contribute successful-arena coverage.

All three final full `tcc -run` Sail images reach their traced program exit:

| input | stdout | compile / program / total steps |
|---|---|---:|
| original pinned repro | `4 16 16 32 \| 4 1 1 5 \| 16` | 13,572,381 / 6,088 / 13,578,494 |
| expression address / cast | `expression-address=0` | 13,509,586 / 2,969 / 13,512,580 |
| function / typeof alias | `function-typeof=0` | 13,516,529 / 2,844 / 13,519,398 |

Builds use the root lock and `systemd-run --user --scope -p MemoryMax=12G`,
with sufficient memory checked. Source files remain frozen through matching
completion receipts. The local direct build and two optional native arena
captures encounter the denied systemd bus. The latter remain two harness
errors and add no native arena coverage. No uncapped replacement was used;
full ordinary Sail executions above and fast arena comparisons are separate.

## CSem and separate GNU questions

The same header-free inputs retain every tested attribute and expression.
GCC and patched tcc return zero on all 26 GNU probes, and the model matches
their tcc arenas. The GNU CSem checkpoint gives 16 defined-zero results, three
defined-one discrepancies (`deref`, `unary-types`, `common-types`), two function
query elaboration errors, and five expression-alignof parser rejections. Its
negative matrix diagnoses/refuses four inputs but accepts invalid arrays of
functions, overaligned elements, and void. These are retained required
spec/constraint follow-ups, never successful compiler comparisons.

The GNU executable matches the lead's archived checkpoint binary manifest;
its historical `gnu-full-1` request/receipt and binary hash are retained. That
worker's sources have since changed for injected bindings; the report does
not certify its current workspace from the old receipt. The separately pinned
OCaml engine at key `3a9bc451ab1fd535523882e559223dab323f5c3a2dfd33cf15fc0fc564b13c16`
has verified artifact/overlay manifests but rejects the GNU typedef spellings,
including three attribute-placement probes. The separately pinned root raw executable predates GNU adoption and also
rejects the GNU input; that observation does not describe current main source.
On the identical ordinary C11 control, that historical checked
CSem, the GNU checkpoint, pinned OCaml, GCC and patched tcc agree on defined
zero; the model produces the same tcc arena. No attributes are stripped to create an agreement.

GNU expression-alignof on **explicitly aligned members** remains its separate
question. The original probe still prints `16 1 16 1 16 16` with host GCC and
`16 1 16 1 16 1` with patched tcc. This repair removes enclosing-object metadata
on member selection; it does not silently choose GCC's member policy.
GNU function-type alignment is also target-specific: the raw function probe
prints `4 4 4` with RV64 GCC/Sail and `1 1 1` with host GCC and tcc. Positive
derivation controls compare with the same engine's ordinary function type;
the raw difference and initial failed 65/66 batch are retained separately.

## Pins, failures and ledger suggestions

[manifest.json](typedef-alignment/manifest.json) hashes every member of
[observations.tar.gz](typedef-alignment/observations.tar.gz). Its primary hashes
name the actual materialized C sources and Lean modules; the archive includes
full `source-pins.json`, `binary-pins.json`, ordered image-file pins, commands,
source freezes/receipts, final ELFs/traces, focused independent arena pairs,
constructor/layout evidence and baseline observations. Pins include the actual
model executable, host/cross tcc, libraries and each full image; a dirty revision
alone is not used as provenance. The final sources match their completed build
and gate receipts. The Lua inventory/mandatory table was consulted; no library
or machine proof was copied or claimed byte-identical by this task.

Retained unsuccessful approaches include the query-only attribute carrier,
live-symbol address inference, the intermediate function-reference alignment
leak (`1 16 1`), build 1's record indentation error, the absent-cache first
gate, the initial CSem harness attempts and the denied optional native captures.
The adopted oracle cache was linked locally only after manifest verification;
no shared cache, upstream toolchain or other worktree was modified.
`tasks/DEAD-ENDS.md` records these so they are not reassigned as repair strategies.

Suggested STATUS/PHASES text for the lead: adopt this compiler/model typedef
query repair after review, with the final finite checks above. Keep
`TypeAlignmentRepr_Statement` open under F6, alongside compiler refinement,
whole-C coverage, ghost/parser agreement and fuel adequacy. Keep the GNU CSem
value/array-constraint discrepancies and explicitly aligned-member expression
question as required separate work. Supported/Fits and the headline theorem
remain unchanged; the frozen sum artifacts remain unchanged.
