import Tcc.Printf.Rows
import Tcc.Printf.Sum.Layout
import Tcc.Prog.Sum.Machine.Calls

/-!
# P's printf call, at the machine level, from the replay

`sum.c`'s one call `printf("%d\n", 55)` as a `PrintfCall` of the image's
newlib: at the replay's row for printf's entry (`printfEntry0`, item 52) the
machine is at `printf` (`0x86400148`) with the format string `"%d\n"` at
`0x86410b68` in `a0` and `55` in `a1`; the replay runs it to the return row
(`printfRet0`, item 634, back at `0x86400074`), where `a0 = 3`, `sp` and the
callee-saved registers are as at entry, and `stdout` (console then pending
buffer) went from empty to `"55\n"`: the console holds it and the buffer is
empty (`_p = _bf._base`; before the call `__sinit` had not run). Every fact
is a kernel `decide` on the rows' literals (`Tcc.Printf.Rows`).
-/

namespace Tcc.Printf.Sum

open LeanRV64DExecutable Vsa.Machine Vsa.Sim Tcc.Boot Tcc.Capture.Sum Tcc.Prog.Sum.Machine

/-- `"%d\n"`. -/
def fmt0 : List UInt8 := [0x25, 0x64, 0x0a]
/-- `"55\n"`. -/
def out0 : List UInt8 := [0x35, 0x35, 0x0a]
/-- `55` as printf's first variadic word. -/
def args0 : List (BitVec 64) := [0x37#64]

theorem entry0 (c : Config) (h : printfEntry0 c) : PrintfEntry layout 0x86410b68 fmt0 args0 c where
  good := h.good
  pc := h.pc
  fmtReg := gprGet_row h (by decide +kernel)
  nargs := by decide
  args := by
    intro i hi
    match i, hi with
    | 0, _ => exact gprGet_row h (by decide +kernel +revert)
  fmtBytes := by
    intro i hi
    match i, hi with
    | 0, _ => exact rdByte_row h (by decide +kernel +revert)
    | 1, _ => exact rdByte_row h (by decide +kernel +revert)
    | 2, _ => exact rdByte_row h (by decide +kernel +revert)
  fmtNul := rdByte_row h (b := 0#8) (by decide +kernel)
  nonul := by decide
  payload := h.payload
  tick := h.tick

theorem ret0 (c c' : Config) (h : printfEntry0 c) (h' : printfRet0 c') :
    PrintfReturn layout out0 (CC.Integers.Int.repr 3) c c' where
  good := h'.good
  pc := ⟨0x86400074#64, gprGet_row h (by decide +kernel), h'.pc⟩
  ret := gprGet_row h' (by decide +kernel)
  saved := by
    intro r hr
    exact gprGet_rows h h' (by revert r hr; decide +kernel)
  out := by
    have hc : pending layout c.σ = [] := by
      apply pending_of_cleanup
      rw [rdWord_row h (by decide +kernel)]
      decide +kernel
    have hc' : pending layout c'.σ = [] := by
      apply pending_of_empty
      rw [rdWord_row h' (by decide +kernel), rdWord_row h' (by decide +kernel)]
      decide +kernel
    simp only [stdoutBytes, hc, hc', consoleBytes_of_row h, consoleBytes_of_row h']
    decide
  payload := h'.payload
  tick := h'.tick

/-- **P's printf call is a `PrintfCall` of the image's newlib.** -/
theorem call0 (c : Config) (h : printfEntry0 c) :
    ∃ c', PrintfCall layout 0x86410b68 fmt0 args0 out0 (CC.Integers.Int.repr 3) c c' := by
  obtain ⟨c', hs, h'⟩ := printf_call0 c h
  exact ⟨c', entry0 c h, hs, ret0 c c' h h'⟩

end Tcc.Printf.Sum
