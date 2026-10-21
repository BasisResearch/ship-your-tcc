import Tcc.Model.Syms

/-!
# TccModel: tccelf.c, part 1 — sections, ELF symbols, relocation
entries; tccdbg.c's `.eh_frame` (unwind tables are on by default)

Section data are byte arrays sized as tcc's `data_allocated`, so ELF
symbols (24 bytes), relocations (Elf64_Rela, 24 bytes) and the symbol hash
table (ints) live in them exactly as in tcc.
-/

namespace Tcc.Model
open C

/-! ## ELF records in section data -/

structure ElfSym where
  st_name : UInt32 := 0
  st_info : UInt8 := 0
  st_other : UInt8 := 0
  st_shndx : UInt16 := 0
  st_value : UInt64 := 0
  st_size : UInt64 := 0
  deriving Inhabited

def readSym (d : ByteArray) (i : Nat) : ElfSym :=
  let o := 24 * i
  { st_name := Bytes.r32 d o, st_info := d.get! (o+4), st_other := d.get! (o+5),
    st_shndx := Bytes.r16 d (o+6), st_value := Bytes.r64 d (o+8), st_size := Bytes.r64 d (o+16) }

def writeSym (d : ByteArray) (i : Nat) (s : ElfSym) : ByteArray :=
  let o := 24 * i
  let d := Bytes.w32 d o s.st_name
  let d := d.set! (o+4) s.st_info
  let d := d.set! (o+5) s.st_other
  let d := Bytes.w16 d (o+6) s.st_shndx
  let d := Bytes.w64 d (o+8) s.st_value
  Bytes.w64 d (o+16) s.st_size

structure ElfRela where
  r_offset : UInt64 := 0
  r_info : UInt64 := 0
  r_addend : Int64 := 0
  deriving Inhabited

def readRela (d : ByteArray) (o : Nat) : ElfRela :=
  { r_offset := Bytes.r64 d o, r_info := Bytes.r64 d (o+8), r_addend := (Bytes.r64 d (o+16)).toInt64 }

def writeRela (d : ByteArray) (o : Nat) (r : ElfRela) : ByteArray :=
  Bytes.w64 (Bytes.w64 (Bytes.w64 d o r.r_offset) (o+8) r.r_info) (o+16) r.r_addend.toUInt64

@[inline] def R_SYM (info : UInt64) : Nat := (info >>> 32).toNat
@[inline] def R_TYPE (info : UInt64) : Int32 := (info &&& 0xffffffff).toUInt32.toInt32
@[inline] def R_INFO (sym : Nat) (ty : Int32) : UInt64 := (sym.toUInt64 <<< 32) + ty.toUInt32.toUInt64

/-- the ELF symbol `i` of section `s` -/
def getElfSym (s : SecId) (i : Nat) : M ElfSym := do
  return readSym (← getSec s).data i
def setElfSym (s : SecId) (i : Nat) (e : ElfSym) : M Unit :=
  modSecData s fun d => writeSym d i e

/-! ## sections -/

/-- `new_section(s1, name, sh_type, sh_flags)` -/
def new_section (name : ByteArray) (sh_type sh_flags : Int32) : M SecId := do
  let align : Int32 :=
    if sh_type == SHT_GNU_versym then 2
    else if sh_type == SHT_HASH || sh_type == SHT_GNU_HASH || sh_type == SHT_REL || sh_type == SHT_RELA ||
            sh_type == SHT_DYNSYM || sh_type == SHT_SYMTAB || sh_type == SHT_DYNAMIC ||
            sh_type == SHT_GNU_verneed || sh_type == SHT_GNU_verdef then 8
    else if sh_type == SHT_STRTAB then 1
    else 8
  let s ← get
  let id := s.secs.size
  let priv := sh_flags &&& SHF_PRIVATE != 0
  let sec : Section := {
    name := name, sh_type := sh_type, sh_flags := sh_flags, sh_addralign := align,
    sh_num := if priv then 0 else Int32.ofNat s.sections.size }
  modify fun s =>
    let a := s.secs
    let s := { s with secs := #[] }
    let s := { s with secs := a.push sec }
    if priv then { s with priv_sections := s.priv_sections.push id }
    else { s with sections := s.sections.push id }
  return id

/-- `section_realloc(sec, new_size)` -/
def section_realloc (sec : SecId) (new_size : Nat) : M Unit := do
  let s ← getSec sec
  let mut size := s.data.size
  if size == 0 then size := 1
  let fuel ← fuelM
  for _ in [0:fuel] do
    if size ≥ new_size then break
    size := size * 2
  modSecData sec fun d => Bytes.grow d size

/-- `section_add(sec, size, align)`: the aligned offset -/
def section_add (sec : SecId) (size : UInt64) (align : Int32) : M Nat := do
  let s ← getSec sec
  let al := align.toInt64.toUInt64
  let offset := ((s.data_offset.toUInt64 + al - 1) &&& (0 - al)).toNat
  let offset1 := offset + size.toNat
  if s.sh_type != SHT_NOBITS && offset1 > s.data.size then
    section_realloc sec offset1
  modSec sec fun s => { s with data_offset := offset1,
                                sh_addralign := if align > s.sh_addralign then align else s.sh_addralign }
  return offset

/-- `section_ptr_add(sec, size)`: the offset of the reserved bytes -/
def section_ptr_add (sec : SecId) (size : UInt64) : M Nat := section_add sec size 1

/-- `section_reserve(sec, size)` -/
def section_reserve (sec : SecId) (size : Nat) : M Unit := do
  let s ← getSec sec
  if size > s.data.size then section_realloc sec size
  modSec sec fun s => if size > s.data_offset then { s with data_offset := size } else s

/-- `have_section(s1, name)` -/
def have_section (name : ByteArray) : M SecId := do
  let s ← get
  for i in [1:s.sections.size] do
    let id := s.sections[i]!
    if s.secs[id]!.name == name then return id
  return 0

/-- `find_section(s1, name)` -/
def find_section (name : ByteArray) : M SecId := do
  let s ← have_section name
  if s != 0 then return s
  new_section name SHT_PROGBITS SHF_ALLOC

/-! ## string and symbol tables -/

/-- `put_elf_str(s, sym)` -/
def put_elf_str (s : SecId) (str : ByteArray) : M UInt32 := do
  let len := str.size + 1
  let off ← section_ptr_add s len.toUInt64
  modSecData s fun d => (Bytes.copy d off str 0 str.size).set! (off + str.size) 0
  return off.toUInt32

/-- `elf_hash(name)` -/
def elf_hash (name : ByteArray) : UInt32 := Id.run do
  let mut h : UInt32 := 0
  for c in name.data do
    h := (h <<< 4) + c.toUInt32
    let g := h &&& 0xf0000000
    if g != 0 then h := h ^^^ (g >>> 24)
    h := h &&& ~~~g
  return h

/-- `HASH_MOD(h, n)` (patches/0002) -/
@[inline] def HASH_MOD (h : UInt32) (n : UInt32) : UInt32 :=
  if n &&& (n - 1) != 0 then h % n else h &&& (n - 1)

@[inline] def readI32 (d : ByteArray) (i : Nat) : Int32 := (Bytes.r32 d (4*i)).toInt32
@[inline] def writeI32 (d : ByteArray) (i : Nat) (v : Int32) : ByteArray := Bytes.w32 d (4*i) v.toUInt32

/-- the name of ELF symbol `e` in `symtab` -/
def symName (symtab : SecId) (e : ElfSym) : M ByteArray := do
  let st ← getSec symtab
  return Bytes.cstrAt (← getSec st.link).data e.st_name.toNat

/-- `rebuild_hash(s, nb_buckets)` -/
def rebuild_hash (s : SecId) (nb_buckets0 : UInt32) : M Unit := do
  let sec ← getSec s
  let strtab := (← getSec sec.link).data
  let nb_syms := sec.data_offset / 24
  let hs := sec.hash
  let hd0 := (← getSec hs).data
  let nb_buckets := if nb_buckets0 == 0 then (readI32 hd0 0).toUInt32 else nb_buckets0
  modSec hs fun h => { h with data_offset := 0 }
  let off ← section_ptr_add hs ((2 + nb_buckets.toNat + nb_syms) * 4).toUInt64
  let syms := (← getSec s).data
  modSecData hs fun d => Id.run do
    let base := off / 4
    let mut d := writeI32 d base (Int32.ofNat nb_buckets.toNat)
    d := writeI32 d (base + 1) (Int32.ofNat nb_syms)
    let hb := base + 2
    for i in [0:nb_buckets.toNat + 1] do d := writeI32 d (hb + i) 0
    let mut p := hb + nb_buckets.toNat + 1
    for sym_index in [1:nb_syms] do
      let e := readSym syms sym_index
      if ELF_ST_BIND e.st_info.toInt32 != STB_LOCAL then
        let h := (HASH_MOD (elf_hash (Bytes.cstrAt strtab e.st_name.toNat)) nb_buckets).toNat
        d := writeI32 d p (readI32 d (hb + h))
        d := writeI32 d (hb + h) (Int32.ofNat sym_index)
      else
        d := writeI32 d p 0
      p := p + 1
    return d

/-- `put_elf_sym(s, value, size, info, other, shndx, name)` -/
def put_elf_sym (s : SecId) (value size : UInt64) (info other shndx : Int32) (name : Option ByteArray) : M Nat := do
  let off ← section_ptr_add s 24
  let sec ← getSec s
  let name_offset ← match name with
    | some n => if n.size > 0 then put_elf_str sec.link n else pure 0
    | none => pure 0
  let e : ElfSym := { st_name := name_offset, st_value := value, st_size := size, st_info := info.toUInt8,
                      st_other := other.toUInt8, st_shndx := shndx.toUInt32.toUInt16 }
  modSecData s fun d => writeSym d (off / 24) e
  let sym_index := off / 24
  let hs := sec.hash
  if hs != 0 then
    let poff ← section_ptr_add hs 4
    let hd := (← getSec hs).data
    if ELF_ST_BIND info != STB_LOCAL then
      let nbuckets := readI32 hd 0
      let strtab := (← getSec sec.link).data
      let h := (HASH_MOD (elf_hash (Bytes.cstrAt strtab name_offset.toNat)) nbuckets.toUInt32).toNat
      modSecData hs fun d =>
        let d := writeI32 d (poff / 4) (readI32 d (2 + h))
        let d := writeI32 d (2 + h) (Int32.ofNat sym_index)
        writeI32 d 1 (readI32 d 1 + 1)
      modSec hs fun h => { h with nb_hashed_syms := h.nb_hashed_syms + 1 }
      if (← getSec hs).nb_hashed_syms > 2 * nbuckets then
        rebuild_hash s (2 * nbuckets).toUInt32
    else
      modSecData hs fun d =>
        let d := writeI32 d (poff / 4) 0
        writeI32 d 1 (readI32 d 1 + 1)
  return sym_index

/-- `find_elf_sym(s, name)` -/
def find_elf_sym (s : SecId) (name : ByteArray) : M Nat := do
  let sec ← getSec s
  let hs := sec.hash
  if hs == 0 then return 0
  let hd := (← getSec hs).data
  let nbuckets := readI32 hd 0
  let h := (HASH_MOD (elf_hash name) nbuckets.toUInt32).toNat
  let mut sym_index := readI32 hd (2 + h)
  let strtab := (← getSec sec.link).data
  let fuel ← fuelM
  for _ in [0:fuel] do
    if sym_index == 0 then break
    let e := readSym sec.data sym_index.toNat'
    if Bytes.cstrAt strtab e.st_name.toNat == name then return sym_index.toNat'
    sym_index := readI32 hd (2 + nbuckets.toNat' + sym_index.toNat')
  return 0

/-- `IS_BSS(s1, ndx)` -/
def IS_BSS (ndx : Int32) : M Bool := do
  if ndx == SHN_COMMON then return true
  let s ← get
  if ndx.toNat' < s.sections.size then
    return s.secs[s.sections[ndx.toNat']!]!.sh_type == SHT_NOBITS
  return false

def ST_ASM_SET : UInt8 := 0x04

/-- `set_elf_sym(s, value, size, info, other, shndx, name)` -/
def set_elf_sym (R : Rec) (s : SecId) (value size : UInt64) (info other shndx : Int32) (name : ByteArray) : M Nat := do
  let sym_bind := ELF_ST_BIND info
  let sym_type := ELF_ST_TYPE info
  let sym_vis := ELF_ST_VISIBILITY other
  if sym_bind != STB_LOCAL then
    let sym_index ← find_elf_sym s name
    if sym_index != 0 then
      let esym ← getElfSym s sym_index
      if esym.st_value == value && esym.st_size == size && esym.st_info == info.toUInt8 &&
         esym.st_other == other.toUInt8 && esym.st_shndx == shndx.toUInt32.toUInt16 then
        return sym_index
      let mut doPatch := false
      let mut esym := esym
      if esym.st_shndx.toUInt32.toInt32 != SHN_UNDEF then
        let esym_bind := ELF_ST_BIND esym.st_info.toInt32
        let esym_vis := ELF_ST_VISIBILITY esym.st_other.toInt32
        let new_vis := if esym_vis == STV_DEFAULT then sym_vis
                       else if sym_vis == STV_DEFAULT then esym_vis
                       else if esym_vis < sym_vis then esym_vis else sym_vis
        esym := { esym with st_other := ((esym.st_other.toInt32 &&& ~~~3) ||| new_vis).toUInt8 }
        if shndx == SHN_UNDEF then pure ()
        else if sym_bind == STB_GLOBAL && esym_bind == STB_WEAK then doPatch := true
        else if sym_bind == STB_WEAK && esym_bind == STB_GLOBAL then pure ()
        else if sym_bind == STB_WEAK && esym_bind == STB_WEAK then pure ()
        else if sym_vis == STV_HIDDEN || sym_vis == STV_INTERNAL then pure ()
        else if (← getSec s).sh_flags &&& SHF_DYNSYM != 0 then pure ()
        else if !(← IS_BSS shndx) && (← IS_BSS esym.st_shndx.toUInt32.toInt32) then doPatch := true
        else if ← IS_BSS shndx then pure ()
        else if esym.st_other &&& ST_ASM_SET != 0 then doPatch := true
        else
          setElfSym s sym_index esym
          let _ ← tcc_error_noabort R (cfmt "link symbol '%s' defined twice" [.s name])
          return sym_index
      else
        esym := { esym with st_other := other.toUInt8 }
        doPatch := true
      if doPatch then
        esym := { esym with st_info := (ELF_ST_INFO sym_bind sym_type).toUInt8,
                            st_shndx := shndx.toUInt32.toUInt16, st_value := value, st_size := size }
      setElfSym s sym_index esym
      return sym_index
  put_elf_sym s value size (ELF_ST_INFO sym_bind sym_type) other shndx (some name)

/-- `set_global_sym(s1, name, sec, offs)` (`offs = none` is tcc's -1) -/
def set_global_sym (R : Rec) (name : Option ByteArray) (sec : SecId) (offs : Option UInt64) : M Nat := do
  let shn : Int32 ←
    if sec != 0 then pure (← getSec sec).sh_num
    else pure (if offs != some 0 || name.isNone then SHN_ABS else SHN_UNDEF)
  let offs ← match offs with
    | none => if sec != 0 then pure (← getSec sec).data_offset.toUInt64 else pure (0xffffffffffffffff : UInt64)
    | some o => pure o
  set_elf_sym R (← get).symtab_section offs 0 (ELF_ST_INFO (if name.isSome then STB_GLOBAL else STB_LOCAL) STT_NOTYPE)
    0 shn (name.getD ByteArray.empty)

/-- `tcc_add_symbol(s1, name, val)` -/
def tcc_add_symbol (R : Rec) (name : ByteArray) (val : UInt64) : M Unit := do
  let _ ← set_global_sym R (some name) 0 (some val)

/-- `get_sym_addr(s1, name, err, forc)` (`none` is tcc's -1) -/
def get_sym_addr (R : Rec) (name : ByteArray) (err : Bool) : M (Option UInt64) := do
  let st := (← get).symtab_section
  let i ← find_elf_sym st name
  let e ← getElfSym st i
  if i == 0 || e.st_shndx.toUInt32.toInt32 == SHN_UNDEF then
    if err then let _ ← tcc_error_noabort R (cfmt "%s not defined" [.s name])
    return none
  return some e.st_value

/-! ## relocations -/

/-- `put_elf_reloca(symtab, s, offset, type, symbol, addend)` -/
def put_elf_reloca (symtab s : SecId) (offset : UInt64) (ty : Int32) (symbol : Nat) (addend : Int64) : M Unit := do
  let sec ← getSec s
  let mut sr := sec.reloc
  if sr == 0 then
    let buf := ".rela".toUTF8 ++ sec.name
    let symflags := (← getSec symtab).sh_flags
    sr ← new_section (pstrcpy 256 buf) SHT_RELA symflags
    let sr' := sr
    modSec sr' fun x => { x with sh_entsize := 24, link := symtab, sh_info := sec.sh_num }
    modSec s fun x => { x with reloc := sr' }
  let off ← section_ptr_add sr 24
  modSecData sr fun d => writeRela d off { r_offset := offset, r_info := R_INFO symbol ty, r_addend := addend }

/-- `get_sym_attr(s1, index, alloc)` -/
def get_sym_attr (index : Nat) (alloc : Bool) : M SymAttrX := do
  let s ← get
  if index ≥ s.sym_attrs.size then
    if !alloc then return s.sym_attrs[0]?.getD {}
    let mut n := 1
    let fuel ← fuelM
    for _ in [0:fuel] do
      if index < n then break
      n := n * 2
    modify fun s => { s with sym_attrs := s.sym_attrs ++ Array.replicate (n - s.sym_attrs.size) {} }
  return (← get).sym_attrs[index]!

def set_sym_attr (index : Nat) (a : SymAttrX) : M Unit :=
  modify fun s => { s with sym_attrs := s.sym_attrs.set! index a }

/-- `update_relocs(s1, s, old_to_new_syms, first_sym)` -/
def update_relocs (s : SecId) (old_to_new : Array Nat) (first_sym : Nat) : M Unit := do
  let st ← get
  for i in [1:st.sections.size] do
    let sr := st.sections[i]!
    let sec ← getSec sr
    if sec.sh_type == SHT_RELA && sec.link == s then
      let n := sec.data_offset / 24
      modSecData sr fun d => Id.run do
        let mut d := d
        for k in [0:n] do
          let r := readRela d (24 * k)
          let sym_index := R_SYM r.r_info
          let ty := R_TYPE r.r_info
          if sym_index < first_sym then continue
          let ni := old_to_new[sym_index - first_sym]!
          d := writeRela d (24 * k) { r with r_info := R_INFO ni ty }
        return d

/-! ## per-file setup -/

/-- `init_symtab(s)` -/
def init_symtab (s : SecId) : M Unit := do
  let sec ← getSec s
  let _ ← put_elf_str sec.link ByteArray.empty
  let _ ← section_ptr_add s 24
  let off ← section_ptr_add sec.hash ((2 + 1 + 1) * 4)
  modSecData sec.hash fun d =>
    let b := off / 4
    writeI32 (writeI32 (writeI32 (writeI32 d b 1) (b+1) 1) (b+2) 0) (b+3) 0

/-- `new_symtab(...)` -/
def new_symtab (symtab_name : String) (sh_type sh_flags : Int32) (strtab_name hash_name : String)
    (hash_sh_flags : Int32) : M SecId := do
  let symtab ← new_section symtab_name.toUTF8 sh_type sh_flags
  modSec symtab fun x => { x with sh_entsize := 24 }
  let strtab ← new_section strtab_name.toUTF8 SHT_STRTAB sh_flags
  modSec symtab fun x => { x with link := strtab }
  let hash ← new_section hash_name.toUTF8 SHT_HASH hash_sh_flags
  modSec hash fun x => { x with sh_entsize := 4, link := symtab }
  modSec symtab fun x => { x with hash := hash }
  init_symtab symtab
  return symtab

/-! ## .eh_frame (tccdbg.c) -/

def dwarf_data1 (s : SecId) (v : UInt8) : M Unit := do
  let o ← section_ptr_add s 1
  modSecData s fun d => d.set! o v
def dwarf_data4 (s : SecId) (v : UInt32) : M Unit := do
  let o ← section_ptr_add s 4
  modSecData s fun d => Bytes.w32 d o v
def dwarf_uleb128 (s : SecId) (v0 : UInt64) : M Unit := do
  let mut v := v0
  for _ in [0:10] do
    let byte := (v &&& 0x7f).toUInt8
    v := v >>> 7
    dwarf_data1 s (byte ||| (if v != 0 then 0x80 else 0))
    if v == 0 then break
def dwarf_sleb128 (s : SecId) (v0 : Int64) : M Unit := do
  let mut v := v0
  let e := v0 >>> 63
  let last : UInt8 := (e &&& 0x40).toUInt64.toUInt8
  for _ in [0:10] do
    let byte := (v &&& 0x7f).toUInt64.toUInt8
    v := v >>> 7
    let more := v != e || (byte &&& 0x40) != last
    dwarf_data1 s (byte ||| (if more then 0x80 else 0))
    if !more then break

def DW_CFA_advance_loc : UInt8 := 0x40
def DW_CFA_offset : UInt8 := 0x80
def DW_CFA_restore : UInt8 := 0xc0
def DW_CFA_nop : UInt8 := 0
def DW_CFA_advance_loc4 : UInt8 := 0x04
def DW_CFA_def_cfa : UInt8 := 0x0c
def DW_CFA_def_cfa_offset : UInt8 := 0x0e
def FDE_ENCODING : UInt8 := 0x1b
def R_RISCV_32_PCREL : Int32 := 57

/-- `tcc_eh_frame_start(s1)` -/
def tcc_eh_frame_start : M Unit := do
  if (← get).unwind_tables == 0 then return
  let eh ← new_section ".eh_frame".toUTF8 SHT_PROGBITS SHF_ALLOC
  modify fun s => { s with eh_frame_section := eh }
  let start := (← getSec eh).data_offset
  modify fun s => { s with eh_start := start.toUInt64 }
  dwarf_data4 eh 0
  dwarf_data4 eh 0
  dwarf_data1 eh 1
  dwarf_data1 eh 122   -- 'z'
  dwarf_data1 eh 82    -- 'R'
  dwarf_data1 eh 0
  modSecData eh fun d => d.set! (start + 8) 3
  dwarf_uleb128 eh 1
  dwarf_sleb128 eh (-4)
  dwarf_uleb128 eh 1
  dwarf_uleb128 eh 1
  dwarf_data1 eh FDE_ENCODING
  dwarf_data1 eh DW_CFA_def_cfa
  dwarf_uleb128 eh 2
  dwarf_uleb128 eh 0
  for _ in [0:4] do
    if ((← getSec eh).data_offset - start) &&& 3 == 0 then break
    dwarf_data1 eh DW_CFA_nop
  let len := (← getSec eh).data_offset - start - 4
  modSecData eh fun d => Bytes.w32 d start len.toUInt32

/-- `tcc_eh_frame_end(s1)` -/
def tcc_eh_frame_end : M Unit := do
  let eh := (← get).eh_frame_section
  if eh == 0 then return
  dwarf_data4 eh 0

/-- `dwarf_get_section_sym(s)` -/
def dwarf_get_section_sym (s : SecId) : M Nat := do
  put_elf_sym (← get).symtab_section 0 0 (ELF_ST_INFO STB_LOCAL STT_SECTION) 0 (← getSec s).sh_num none

/-- `tcc_debug_frame_end(s1, size)` (riscv64) -/
def tcc_debug_frame_end (size0 : Int32) : M Unit := do
  let s ← get
  let eh := s.eh_frame_section
  if eh == 0 then return
  let eh_section_sym ← dwarf_get_section_sym s.text_section
  let fde_start := (← getSec eh).data_offset
  dwarf_data4 eh 0
  dwarf_data4 eh (fde_start.toUInt32 - s.eh_start.toUInt32 + 4)
  put_elf_reloca s.symtab_section eh (← getSec eh).data_offset.toUInt64 R_RISCV_32_PCREL eh_section_sym 0
  dwarf_data4 eh s.func_ind.toUInt32
  dwarf_data4 eh size0.toUInt32
  dwarf_data1 eh 0
  dwarf_data1 eh (DW_CFA_advance_loc + 4)
  dwarf_data1 eh DW_CFA_def_cfa_offset
  dwarf_uleb128 eh 16
  dwarf_data1 eh (DW_CFA_advance_loc + 8)
  dwarf_data1 eh (DW_CFA_offset + 1)
  dwarf_uleb128 eh 2
  dwarf_data1 eh (DW_CFA_offset + 8)
  dwarf_uleb128 eh 4
  dwarf_data1 eh (DW_CFA_advance_loc + 8)
  dwarf_data1 eh DW_CFA_def_cfa
  dwarf_uleb128 eh 8
  dwarf_uleb128 eh 0
  dwarf_data1 eh DW_CFA_advance_loc4
  let text := (← getSec s.cur_text_section).data
  let mut size := size0
  let fuel ← fuelM
  for _ in [0:fuel] do
    if !(size ≥ 4 && Bytes.r32 text (s.func_ind + size - 4).toNat' != 0x00008067) then break
    size := size - 4
  dwarf_data4 eh (size - 36).toUInt32
  dwarf_data1 eh DW_CFA_def_cfa
  dwarf_uleb128 eh 2
  dwarf_uleb128 eh 16
  dwarf_data1 eh (DW_CFA_advance_loc + 4)
  dwarf_data1 eh (DW_CFA_restore + 1)
  dwarf_data1 eh (DW_CFA_advance_loc + 4)
  dwarf_data1 eh (DW_CFA_restore + 8)
  dwarf_data1 eh (DW_CFA_advance_loc + 4)
  dwarf_data1 eh DW_CFA_def_cfa_offset
  dwarf_uleb128 eh 0
  for _ in [0:4] do
    if ((← getSec eh).data_offset - fde_start) &&& 3 == 0 then break
    dwarf_data1 eh DW_CFA_nop
  let len := (← getSec eh).data_offset - fde_start - 4
  modSecData eh fun d => Bytes.w32 d fde_start len.toUInt32

/-- `tccelf_new(s)` -/
def tccelf_new : M Unit := do
  modify fun s => { s with sections := #[0] }
  let text ← new_section ".text".toUTF8 SHT_PROGBITS (SHF_ALLOC ||| SHF_EXECINSTR)
  let data ← new_section ".data".toUTF8 SHT_PROGBITS (SHF_ALLOC ||| SHF_WRITE)
  let rodata ← new_section ".data.ro".toUTF8 SHT_PROGBITS SHF_ALLOC
  let bss ← new_section ".bss".toUTF8 SHT_NOBITS (SHF_ALLOC ||| SHF_WRITE)
  let common ← new_section ".common".toUTF8 SHT_NOBITS SHF_PRIVATE
  modSec common fun x => { x with sh_num := SHN_COMMON }
  modify fun s => { s with text_section := text, data_section := data, rodata_section := rodata,
                            bss_section := bss, common_section := common }
  let symtab ← new_symtab ".symtab" SHT_SYMTAB 0 ".strtab" ".hashtab" SHF_PRIVATE
  modify fun s => { s with symtab_section := symtab }
  let dyn ← new_symtab ".dynsymtab" SHT_SYMTAB (SHF_PRIVATE ||| SHF_DYNSYM) ".dynstrtab" ".dynhashtab" SHF_PRIVATE
  modify fun s => { s with dynsymtab_section := dyn }
  let _ ← get_sym_attr 0 true
  tcc_eh_frame_start

/-- `tccelf_begin_file(s1)` -/
def tccelf_begin_file : M Unit := do
  let s ← get
  for i in [1:s.sections.size] do
    let id := s.sections[i]!
    modSec id fun x => { x with sh_offset := x.data_offset.toUInt64 }
  let st := s.symtab_section
  modSec st fun x => { x with reloc := x.hash, hash := 0 }

/-- `tccelf_end_file(s1)` -/
def tccelf_end_file (R : Rec) : M Unit := do
  let s := (← get).symtab_section
  let sec ← getSec s
  let first_sym := sec.sh_offset.toNat / 24
  let nb_syms := sec.data_offset / 24 - first_sym
  modSec s fun x => { x with data_offset := x.sh_offset.toNat }
  modSec sec.link fun x => { x with data_offset := x.sh_offset.toNat }
  modSec s fun x => { x with hash := x.reloc, reloc := 0 }
  let mut tr : Array Nat := Array.replicate nb_syms 0
  let outputObj := (← get).output_type == TCC_OUTPUT_OBJ
  for i in [0:nb_syms] do
    let data := (← getSec s).data
    let mut e := readSym data (first_sym + i)
    if e.st_shndx.toUInt32.toInt32 == SHN_UNDEF then
      let mut sym_bind := ELF_ST_BIND e.st_info.toInt32
      let mut sym_type := ELF_ST_TYPE e.st_info.toInt32
      if sym_bind == STB_LOCAL then sym_bind := STB_GLOBAL
      if sym_bind == STB_GLOBAL && outputObj then
        if sym_type != STT_TLS then sym_type := STT_NOTYPE
      e := { e with st_info := (ELF_ST_INFO sym_bind sym_type).toUInt8 }
      setElfSym s (first_sym + i) e
    let strtab := (← getSec sec.link).data
    let nm := Bytes.cstrAt strtab e.st_name.toNat
    let ni ← set_elf_sym R s e.st_value e.st_size e.st_info.toInt32 e.st_other.toInt32
      e.st_shndx.toUInt32.toInt32 nm
    tr := tr.set! i ni
  update_relocs s tr first_sym

end Tcc.Model
