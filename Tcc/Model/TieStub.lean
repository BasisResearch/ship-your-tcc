import Tcc.Model.Run

/-!
# TccModel: a test-only knot for the link half

Not part of the model: `Tie.lean` (all of tccpp/tccgen/riscv64-gen) is
the real knot. This one ties only the file layer and the linker, and its
`tcc_compile` does to the sections exactly what compiling an EMPTY C file
does (tcc_compile → tccelf_begin_file, tccgen_compile's tcc_debug_start
(the STT_FILE symbol) and tcc_eh_frame_end (the .eh_frame terminator),
tccelf_end_file), so the linker can be checked against the reference on
programs whose code comes from object files (`tcc -run x.o /prog.c` with
an empty /prog.c). Every other field throws.
-/

namespace Tcc.Model

deriving instance Inhabited for Rec

namespace TieStub

/-- what `tcc_compile(s1, filetype, filename, fd)` does for an empty file -/
def stubCompile (R : Rec) (_filetype : Int32) (filename : ByteArray) (fd : Int32) : M Int32 := do
  modify fun s => { s with error_set_jmp_enabled := 1 }
  tcc_open_bf filename none
  modFile fun f => { f with fd := fd }
  tccelf_begin_file
  -- tcc_debug_start: filename = file->prev ? file->prev->filename : file->filename
  let f ← curFile
  let s ← get
  let _ ← put_elf_sym s.symtab_section 0 0 (C.ELF_ST_INFO C.STB_LOCAL C.STT_FILE) 0 C.SHN_ABS (some f.filename)
  tcc_eh_frame_end
  tccelf_end_file R
  tcc_close
  modify fun s => { s with error_set_jmp_enabled := 0 }
  return if (← get).nb_errors != 0 then -1 else 0

def stubThrow {α : Type} : M α := throw (.unsupported "TieStub: tccpp/tccgen are not tied in")

/-- the knot: depth bounded by `n` -/
def tie : Nat → Rec
  | 0 => default
  | n + 1 =>
    { (default : Rec) with
      tcc_compile := fun ft fn fd => stubCompile (tie n) ft fn fd
      tcc_add_file_internal := fun f fl => tcc_add_file_internal (tie n) f fl
      pp_error := stubThrow }

end TieStub
end Tcc.Model
