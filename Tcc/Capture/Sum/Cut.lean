import Tcc.Capture.Sum.Witness

/-!
# The captured cut of `tcc -run` for P = `c/tests/p3/sum.c` (PHASES 3a)

`TccSumCut c` says `c` is a machine configuration at the jump into P's
generated code, as captured by `scripts/tcc_cut.py` on the emulator and
written to Lean by `scripts/gen_tcc_cut.py`:

* `regs`: `GoodState`, PC = `tcc_baremetal_enter`, an idle HTIF mailbox,
  every GPR as captured (`Regs.regs`);
* `view`: the 6,689 bytes the program phase fetches or loads before writing
  them, present with their captured values (`View.view`): 660 bytes of host
  code (`_exit`, the HTIF system calls, `tcc_baremetal_enter`), checked equal
  to the ELF's loaded bytes by kernel `decide` (`Witness.host<i>_elf`), and
  6,029 bytes the compile phase stored (P's 136 bytes of code at
  `0x86400000`, `_runmain`, the libc/libgcc members tcc relocated into its
  arena, the format string, data words), which are observations;
* `console`: nothing printed;
* `tick`: the tick counter in range.

`cut_ok` shows the description is satisfiable (by the representative
configuration `Witness.cut`). That the machine reaches it from reset is
`CompileCut_Statement`, open.
-/

namespace Tcc.Capture.Sum

open Vsa.Machine Tcc.Boot

/-- The captured cut. -/
abbrev TccSumCut (c : Config) : Prop := CapturedCut regs enterPc view c

theorem cut_ok : TccSumCut cut := cut_captured

/-- The tcc image with P embedded, as far as the run to the cut can tell
(`ElfPin`): its entry point and `.tohost` address, and its loaded memory on
`Elf.elfImage` (every byte the compile phase reads before writing it, and the
view's host bytes; 701,193 + 660 addresses) and `Elf.elfAbsent`. The image
`c/Makefile` builds with `FLAGS=0 PROG=tests/p3/sum.c` satisfies it (ELF
SHA-256 in `results/p3/sum-cut.json`; checked by the generator, not in Lean). -/
abbrev TccSumElf (elf : ELF64File) : Prop :=
  ElfPin Elf.elfImage Elf.elfAbsent entryAddr tohostAddr elf

/-- An ELF so pinned loads the view's host bytes. -/
theorem TccSumElf.host {elf : ELF64File} (h : TccSumElf elf) :
    PartialView (initializeMemory .B64 elf) (chunkView hostChunks) :=
  h.image.mono hostView_elf

/-- PHASES 3a, open: tcc's compile phase, from reset on the image, reaches
the captured cut. -/
def CompileCut_Statement : Prop :=
  ∀ elf, TccSumElf elf → ∀ c₀, ElfReset elf c₀ → ∃ c, Steps c₀ c ∧ TccSumCut c

end Tcc.Capture.Sum
