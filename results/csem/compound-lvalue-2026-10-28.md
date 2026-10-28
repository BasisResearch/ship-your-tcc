# Compound-assignment lvalue evaluation, 2026-10-28

Pristine pinned Cerberus `704c4e697845127bced66c93fa0763eb51527ca4` and
current checked raw Lean both evaluate a compound-assignment lvalue twice.
The minimal complete reproducer is
[results/repro/csem_compound_lvalue_twice.c](../repro/csem_compound_lvalue_twice.c):

```c
int x,n;
int *f(void){++n;return &x;}
int main(void){*f()+=1;return n;}
```

Both semantic engines produce only defined exit2 outcomes (five enumerated
schedules each); strict-C11 GCC and patched tcc return1. The callback counter
therefore directly witnesses duplicate evaluation. C11 requires the lvalue to
be evaluated once in compound assignment. [N1570 §6.5.16.2p3](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf#page=121)

Six additional complete inputs distinguish wrong results from spurious UB:

| Input shape | Checked Lean and pristine OCaml | GCC / patched tcc |
|---|---|---|
| `a[i++] += 3` | UB035 unsequenced race | Defined expected result, exit0 |
| `a[i++] *= 4` | UB035 unsequenced race | Defined expected result, exit0 |
| `*p++ += 3` | UB035 unsequenced race | Defined expected result, exit0 |
| `get()->x += 3`, callback counter checked | All defined exit1 | Exit0 |
| `*get() += 3`, callback counter checked | All defined exit1 | Exit0 |
| `a[i++] = 4`, ordinary assignment control | Defined exit0 | Exit0 |

All source bytes, commands, stdout, stderr, status codes and exact executable
hashes are retained. The pristine checkout's git status is empty. The local
raw binary comes from the passing alignment main refresh at `f993eaf`, with
its 3,640-input request and completed receipt included. The diagnostic capture
is independent of the pending bitfield implementation. No new Lean build,
compiler image, native Sail observation or sum cut is claimed here.

The bitfield worker first noticed the shared lowering issue during source
review. This lead probe establishes that it affects ordinary scalar lvalues
and is inherited upstream, rather than introduced by new bitfield metadata.
CSEM-14 is ready for Kiran's review; nothing has been sent externally. Repair
belongs in shared single-evaluation lowering, with the original captures
retained. Neither a changed Supported premise nor treating these defined C
programs as semantic exclusions is an acceptable repair.

The portable evidence directory retains 140 verified files, including the
six-case matrix, minimal counter and original source-pinned drivers. Run
`python3 results/csem/compound-lvalue-2026-10-28/verify.py` to check the record.
These finite observations do not discharge universal semantic or compiler
correctness obligations.
