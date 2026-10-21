import Tcc.Model.Link

/-!
# TccModel: tccrun.c's -run path under TCC_BAREMETAL (patches/0002)

`tcc_run` adds the image's porting layer (`tcc_baremetal_syms`, main.c)
and runmain.o, relocates the program into the static arena at
`__tccrun_start` (`rt_mem`, `tcc_relocate_ex`) and stops where tcc calls
the image's `tcc_baremetal_run`: the arena's bytes are then in
`TccSt.arena` (offset 0 is `run_ptr`), and `RunResult` holds what
`tcc_baremetal_run` receives (prog_main, the program's code address and
size).
-/

namespace Tcc.Model
open C RV

/-- what tcc_run hands to the image's tcc_baremetal_run -/
structure RunResult where
  entry : UInt64 := 0       -- prog_main = get_sym_addr("_runmain")
  code : UInt64 := 0        -- __tcc_baremetal_code
  codeSize : UInt64 := 0    -- bm_code_size
  deriving Inhabited

/-- c/src/main.c's `tcc_baremetal_syms`: (name tcc adds, image symbol) -/
def tcc_baremetal_syms : List (String × String) :=
  [("errno", "errno"), ("_read", "_read"), ("_write", "_write"), ("_open", "_open"),
   ("_close", "_close"), ("_lseek", "_lseek"), ("_fstat", "_fstat"), ("_stat", "_stat"),
   ("_isatty", "_isatty"), ("_link", "_link"), ("_unlink", "_unlink"), ("_sbrk", "_sbrk"),
   ("_exit", "_exit"), ("_kill", "_kill"), ("_getpid", "_getpid"),
   ("_gettimeofday", "_gettimeofday"), ("_times", "_times"), ("_mkdir", "mkdir"),
   ("__clear_cache", "tcc_baremetal_sync_icache")]

def imageSym (name : String) : M UInt64 := do
  match (← get).imageSyms.get? name with
  | some a => return a
  | none => internal s!"image symbol {name} missing"

def PAGEALIGN (n : UInt64) : UInt64 := n + ((0 - n) &&& (PAGESIZE - 1))

/-- `rt_mem(s1, size)`: ptr_diff (0), or -1 -/
def rt_mem (R : Rec) (size : Int32) : M Int32 := do
  let st ← imageSym "__tccrun_start"
  let en ← imageSym "__tccrun_end"
  if size.toInt64 > (en - st).toInt64 then
    return ← tcc_error_noabort R (cfmt "tccrun: program needs %d bytes, the code arena has %d"
      [.i size.toInt, .i (en - st).toUInt32.toInt32.toInt])
  modify fun s => { s with run_ptr := st, run_size := size.toUInt32.toUInt64,
                            arena := Bytes.zeros size.toUInt32.toNat }
  return 0

/-- `cleanup_symbols(s1)`: remove all STB_LOCAL symbols -/
def cleanup_symbols : M Unit := do
  let s := (← get).symtab_section
  let sec ← getSec s
  let old := sec.data
  let oldstr := (← getSec sec.link).data
  let end_sym := sec.data_offset / 24
  modSec s fun x => { x with data_offset := 0 }
  modSec sec.link fun x => { x with data_offset := 0 }
  modSec sec.hash fun x => { x with data_offset := 0 }
  init_symtab s
  for i in [1:end_sym] do
    let e := readSym old i
    if ELF_ST_BIND e.st_info.toInt32 == STB_LOCAL then continue
    let name := Bytes.cstrAt oldstr e.st_name.toNat
    let _ ← put_elf_sym s e.st_value e.st_size e.st_info.toInt32 e.st_other.toInt32
      e.st_shndx.toUInt32.toInt32 (some name)

/-- `cleanup_sections(s1)`: free all sections but the symbol table's -/
def cleanup_sections : M Unit := do
  let st ← get
  let sym := st.symtab_section
  let symsec ← getSec sym
  let keep (x : SecId) : Bool := x == sym || x == symsec.link || x == symsec.hash
  for i in [1:st.sections.size] do
    let x := st.sections[i]!
    if x == 0 then continue
    if keep x then modSec x fun y => { y with data := y.data.extract 0 y.data_offset }
    else
      modSec x fun y => { y with data := ByteArray.empty, data_offset := 0 }
      modify fun s => { s with sections := s.sections.set! i 0 }
  for i in [0:st.priv_sections.size] do
    let x := st.priv_sections[i]!
    if x == 0 then continue
    if keep x then modSec x fun y => { y with data := y.data.extract 0 y.data_offset }
    else
      modSec x fun y => { y with data := ByteArray.empty, data_offset := 0 }
      modify fun s => { s with priv_sections := s.priv_sections.set! i 0 }

/-- `tcc_relocate_ex(s1, ptr, ptr_diff)`: the size (when `ptr` is NULL), 0, or -1 -/
def tcc_relocate_ex (R : Rec) (ptr : UInt64) (ptr_diff : UInt32) : M Int32 := do
  if ptr == 0 then
    modify fun s => { s with nb_errors := 0 }
    tcc_add_runtime R
    resolve_common_syms R
    build_got_entries R 0
  let mut offset : UInt32 := 0
  let mut copy := 0
  let mem := ptr
  let shf : Array Int32 := #[SHF_ALLOC ||| SHF_EXECINSTR, SHF_ALLOC, SHF_ALLOC ||| SHF_WRITE]
  for _ in [0:5] do   -- redo:
    if (← get).nb_errors != 0 then return -1
    if copy == 3 then return 0
    for k in [0:3] do
      let mut n : UInt64 := 0
      let mut addr : UInt64 := 0
      let secs := (← get).sections
      for i in [1:secs.size] do
        let sid := secs[i]!
        let s ← getSec sid
        if shf[k]! != s.sh_flags &&& (SHF_ALLOC ||| SHF_WRITE ||| SHF_EXECINSTR) then continue
        let length := s.data_offset.toUInt32
        if copy == 2 then
          if addr == 0 then addr := s.sh_addr
          n := (s.sh_addr - addr) + length.toUInt64
          continue
        if copy != 0 then
          -- final step: copy section data to memory
          let p := (if k == 0 then s.sh_addr + ptr_diff.toUInt64 else s.sh_addr) - (← get).run_ptr
          let bytes := if s.data.size == 0 || s.sh_type == SHT_NOBITS then Bytes.zeros length.toNat
                       else s.data.extract 0 length.toNat
          modify fun st =>
            let a := st.arena
            let st := { st with arena := ByteArray.empty }
            { st with arena := Bytes.copy (Bytes.grow a (p.toNat + length.toNat)) p.toNat bytes 0 length.toNat }
          continue
        if s.sh_flags &&& SHF_TLS != 0 && length != 0 then
          return ← tcc_error_noabort R "thread-local storage not supported with -run".toUTF8
        let mut align := s.sh_addralign
        n := n + 1
        if n == 1 then
          -- start a new page for different permissions (k <= CONFIG_RUNMEM_RO = 0)
          if k ≤ 0 then align := PAGESIZE.toUInt32.toInt32
        let al := align
        modSec sid fun x => { x with sh_addralign := al }
        addr := if k != 0 then mem + ptr_diff.toUInt64 else mem
        offset := offset + ((0 - (addr + offset.toUInt64)) &&& (align.toInt64.toUInt64 - 1)).toUInt32
        let sa := if mem != 0 then addr + offset.toUInt64 else 0
        modSec sid fun x => { x with sh_addr := sa }
        offset := offset + length
      -- copy == 2: protect_pages: no page protection on the bare machine (the
      -- image's tcc_baremetal_sync_icache for the code changes no memory)
    if mem == 0 then return (PAGEALIGN offset.toUInt64).toUInt32.toInt32
    copy := copy + 1
    if copy == 2 then continue
    if copy == 3 then
      cleanup_symbols
      cleanup_sections
      continue
    -- relocate symbols
    relocate_syms R (← get).symtab_section 1
    if (← get).nb_errors != 0 then continue
    relocate_plt R
    relocate_sections R
  internal "tcc_relocate_ex"

/-- `tcc_relocate(s1)` -/
def tcc_relocate (R : Rec) : M Int32 := do
  if (← get).run_ptr != 0 then
    let _ ← tcc_error_noabort R "'tcc_relocate()' twice is no longer supported".toUTF8
    throw (.exit (-1))
  let size ← tcc_relocate_ex R 0 0
  if size < 0 then return -1
  let ptr_diff ← rt_mem R size
  if ptr_diff < 0 then return -1
  tcc_relocate_ex R (← get).run_ptr ptr_diff.toUInt32

/-- `tcc_run(s1, argc, argv)` up to the call of the image's
`tcc_baremetal_run`: `.ok r` when tcc reaches it, `.error ret` otherwise -/
def tcc_run (R : Rec) : M (Except Int32 RunResult) := do
  let s ← get
  let text ← getSec s.text_section
  modify fun st => { st with bm_code_size := text.data_offset.toUInt64 }
  let _ ← set_elf_sym R s.symtab_section 0 0 (ELF_ST_INFO STB_GLOBAL STT_NOTYPE) 0 text.sh_num
    "__tcc_baremetal_code".toUTF8
  for (nm, img) in tcc_baremetal_syms do
    tcc_add_symbol R nm.toUTF8 (← imageSym img)
  tcc_add_symbol R "__rt_exit".toUTF8 (← imageSym "rt_exit")
  modify fun st => { st with run_main := "_runmain".toUTF8 }
  let _ ← tcc_add_support R "runmain.o"
  if (← tcc_relocate R) < 0 then return .error (-1)
  match ← get_sym_addr R "_runmain".toUTF8 true with
  | none => return .error (-1)
  | some entry =>
    let code ← get_sym_addr R "__tcc_baremetal_code".toUTF8 true
    return .ok { entry := entry, code := code.getD 0xffffffffffffffff, codeSize := (← get).bm_code_size }

end Tcc.Model
