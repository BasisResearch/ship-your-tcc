import Tcc.Model.Elf

/-!
# TccModel: the -run link — libtcc.c's file layer, tccelf.c's object and
archive loading and linking, riscv64-link.c

As configured for the image: TCC_TARGET_RISCV64, TCC_BAREMETAL
(patches/0002: `bm_output_section`, unnamed local symbols of loaded
objects, the hashed `tcc_load_alacarte`, `-lgcc` after the last `-lc`),
output type TCC_OUTPUT_MEMORY, no dynamic symbols (`s1->dynsym` is NULL
for -run), no bounds checking, backtraces or debug information.
Files are read through htif.c's descriptors (`Host`).
-/

namespace Tcc.Model
open C

/-! ## constants -/

namespace RV
def R_RISCV_NONE : Int32 := 0
def R_RISCV_32 : Int32 := 1
def R_RISCV_64 : Int32 := 2
def R_RISCV_RELATIVE : Int32 := 3
def R_RISCV_COPY : Int32 := 4
def R_RISCV_JUMP_SLOT : Int32 := 5
def R_RISCV_BRANCH : Int32 := 16
def R_RISCV_JAL : Int32 := 17
def R_RISCV_CALL : Int32 := 18
def R_RISCV_CALL_PLT : Int32 := 19
def R_RISCV_GOT_HI20 : Int32 := 20
def R_RISCV_PCREL_HI20 : Int32 := 23
def R_RISCV_PCREL_LO12_I : Int32 := 24
def R_RISCV_PCREL_LO12_S : Int32 := 25
def R_RISCV_TPREL_HI20 : Int32 := 29
def R_RISCV_TPREL_LO12_I : Int32 := 30
def R_RISCV_ADD16 : Int32 := 34
def R_RISCV_ADD32 : Int32 := 35
def R_RISCV_ADD64 : Int32 := 36
def R_RISCV_SUB8 : Int32 := 37
def R_RISCV_SUB16 : Int32 := 38
def R_RISCV_SUB32 : Int32 := 39
def R_RISCV_SUB64 : Int32 := 40
def R_RISCV_ALIGN : Int32 := 43
def R_RISCV_RVC_BRANCH : Int32 := 44
def R_RISCV_RVC_JUMP : Int32 := 45
def R_RISCV_RELAX : Int32 := 51
def R_RISCV_SUB6 : Int32 := 52
def R_RISCV_SET6 : Int32 := 53
def R_RISCV_SET8 : Int32 := 54
def R_RISCV_SET16 : Int32 := 55
def R_RISCV_32_PCREL : Int32 := 57
def R_RISCV_SET_ULEB128 : Int32 := 60
def R_RISCV_SUB_ULEB128 : Int32 := 61
-- riscv64-link.c's TARGET_DEFS
def R_DATA_PTR : Int32 := R_RISCV_64
def R_JMP_SLOT : Int32 := R_RISCV_JUMP_SLOT
def R_GLOB_DAT : Int32 := R_RISCV_64
-- tcc.h's enum gotplt_entry
def NO_GOTPLT_ENTRY : Int32 := 0
def BUILD_GOT_ONLY : Int32 := 1
def AUTO_GOTPLT_ENTRY : Int32 := 2
def ALWAYS_GOTPLT_ENTRY : Int32 := 3
def EM_RISCV : UInt16 := 243
def ET_REL : UInt16 := 1
def ET_DYN : UInt16 := 3
def SHF_COMPRESSED : UInt64 := 0x800
def PAGESIZE : UInt64 := 4096
end RV

open RV

/-! ## reading files -/

/-- `full_read(fd, buf, count)`: the bytes read, or none for -1 -/
def full_read (fd : Int32) (count : Nat) : M (Option ByteArray) := do
  -- read() returns what is left up to `count`; the next read returns 0
  Host.read fd count

/-- `load_data(fd, file_offset, size)` (the bytes past a short read are 0) -/
def load_data (fd : Int32) (file_offset size : Nat) : M ByteArray := do
  let _ ← Host.lseekSet fd file_offset
  let d := (← full_read fd size).getD ByteArray.empty
  return Bytes.grow d size

/-- `struct Elf64_Ehdr`, the fields tcc reads -/
structure Ehdr where
  ident : ByteArray := ByteArray.empty
  e_type : UInt16 := 0
  e_machine : UInt16 := 0
  e_shoff : UInt64 := 0
  e_shnum : UInt16 := 0
  e_shstrndx : UInt16 := 0
  deriving Inhabited

def readEhdr (b : ByteArray) : Ehdr :=
  { ident := b.extract 0 16, e_type := Bytes.r16 b 16, e_machine := Bytes.r16 b 18,
    e_shoff := Bytes.r64 b 40, e_shnum := Bytes.r16 b 60, e_shstrndx := Bytes.r16 b 62 }

/-- `tcc_object_type(fd, h)` -/
def tcc_object_type (fd : Int32) : M (Int32 × Ehdr) := do
  let b := (← full_read fd 64).getD ByteArray.empty
  let size := b.size
  let h := readEhdr (Bytes.grow b 64)
  if size == 64 && b.extract 0 4 == ByteArray.mk #[0x7f, 69, 76, 70] then
    if h.e_type == ET_REL then return (AFF_BINTYPE_REL, h)
    if h.e_type == ET_DYN then return (AFF_BINTYPE_DYN, h)
  else if size ≥ 8 then
    if b.extract 0 8 == "!<arch>\n".toUTF8 then return (AFF_BINTYPE_AR, h)
  return (0, h)

/-- `struct Elf64_Shdr` -/
structure Shdr where
  sh_name : UInt32 := 0
  sh_type : UInt32 := 0
  sh_flags : UInt64 := 0
  sh_offset : UInt64 := 0
  sh_size : UInt64 := 0
  sh_link : UInt32 := 0
  sh_info : UInt32 := 0
  sh_addralign : UInt64 := 0
  sh_entsize : UInt64 := 0
  deriving Inhabited

def readShdr (b : ByteArray) (i : Nat) : Shdr :=
  let o := 64 * i
  { sh_name := Bytes.r32 b o, sh_type := Bytes.r32 b (o+4), sh_flags := Bytes.r64 b (o+8),
    sh_offset := Bytes.r64 b (o+24), sh_size := Bytes.r64 b (o+32), sh_link := Bytes.r32 b (o+40),
    sh_info := Bytes.r32 b (o+44), sh_addralign := Bytes.r64 b (o+48), sh_entsize := Bytes.r64 b (o+56) }

/-! ## tcc_load_object_file -/

/-- `bm_output_section(name)` (patches/0002) -/
def bm_output_section (name : ByteArray) : ByteArray := Id.run do
  let ins : List (String × String × String) :=
    [(".text", ".text", ".rela.text"), (".rodata", ".rodata", ".rela.rodata"),
     (".srodata", ".rodata", ".rela.rodata"), (".data", ".data", ".rela.data"),
     (".sdata", ".data", ".rela.data"), (".bss", ".bss", ".rela.bss"), (".sbss", ".bss", ".rela.bss")]
  let rela := name.size ≥ 6 && name.extract 0 6 == ".rela.".toUTF8
  let n := if rela then name.extract 5 name.size else name
  for (i, o, orl) in ins do
    let ib := i.toUTF8
    let len := ib.size
    if n.size ≥ len && n.extract 0 len == ib && (n.size == len || n.get! len == 46) then
      return (if rela then orl else o).toUTF8
  return name

/-- tcc's `SectionMergeInfo` -/
structure SectionMergeInfo where
  s : SecId := 0
  offset : Nat := 0
  new_section : Bool := false
  link_once : Bool := false
  deriving Inhabited

private def startsWith (b : ByteArray) (p : String) : Bool :=
  let pb := p.toUTF8
  b.size ≥ pb.size && b.extract 0 pb.size == pb

/-- `tcc_load_object_file(s1, fd, file_offset)` -/
def tcc_load_object_file (R : Rec) (fd : Int32) (file_offset : Nat) : M Int32 := do
  let _ ← Host.lseekSet fd file_offset
  let (ot, ehdr) ← tcc_object_type fd
  if ot != AFF_BINTYPE_REL || ehdr.ident.get! 5 != 1 || ehdr.e_machine != EM_RISCV then
    return ← tcc_error_noabort R "invalid object file".toUTF8
  let shnum := ehdr.e_shnum.toNat
  let shbytes ← load_data fd (file_offset + ehdr.e_shoff.toNat) (64 * shnum)
  let mut shdr : Array Shdr := (Array.range shnum).map (readShdr shbytes)
  let mut sm : Array SectionMergeInfo := Array.replicate shnum {}
  let shs := shdr[ehdr.e_shstrndx.toNat]!
  let strsec ← load_data fd (file_offset + shs.sh_offset.toNat) shs.sh_size.toNat
  let mut symtab : Option ByteArray := none
  let mut strtab : ByteArray := ByteArray.empty
  let mut nb_syms := 0
  let mut seencompressed := false
  let symtabSec := (← get).symtab_section
  -- the_end: return ret (-1 until done)
  for i in [1:shnum] do
    let mut sh := shdr[i]!
    if sh.sh_type == SHT_SYMTAB.toUInt32 then
      if symtab.isSome then
        let _ ← tcc_error_noabort R "object must contain only one symtab".toUTF8
        return -1
      nb_syms := sh.sh_size.toNat / 24
      symtab := some (← load_data fd (file_offset + sh.sh_offset.toNat) sh.sh_size.toNat)
      sm := sm.set! i { (sm[i]!) with s := symtabSec }
      sh := shdr[sh.sh_link.toNat]!
      strtab ← load_data fd (file_offset + sh.sh_offset.toNat) sh.sh_size.toNat
    if sh.sh_flags &&& SHF_COMPRESSED != 0 then seencompressed := true
  -- merge the sections
  let st ← get
  let eh := st.eh_frame_section
  for i in [1:shnum] do
    if i == ehdr.e_shstrndx.toNat then continue
    let mut sh := shdr[i]!
    if sh.sh_type == SHT_RELA.toUInt32 then sh := shdr[sh.sh_info.toNat]!
    let nm0 := Bytes.cstrAt strsec sh.sh_name.toNat
    if startsWith nm0 ".debug_" || startsWith nm0 ".stab" then
      -- !s1->do_debug
      continue
    else if startsWith nm0 ".eh_frame" then
      if eh == 0 then continue
    else if !(sh.sh_type == SHT_PROGBITS.toUInt32 || sh.sh_type == SHT_NOTE.toUInt32 ||
              sh.sh_type == SHT_NOBITS.toUInt32 || sh.sh_type == SHT_PREINIT_ARRAY.toUInt32 ||
              sh.sh_type == SHT_INIT_ARRAY.toUInt32 || sh.sh_type == SHT_FINI_ARRAY.toUInt32) then
      continue
    sh := shdr[i]!
    let sh_name := bm_output_section (Bytes.cstrAt strsec sh.sh_name.toNat)
    if sh.sh_addralign < 1 then
      sh := { sh with sh_addralign := 1 }
      shdr := shdr.set! i sh
    -- find the corresponding section
    let mut found : SecId := 0
    let mut linkOnce := false
    let sections := (← get).sections
    for j in [1:sections.size] do
      let s := sections[j]!
      let sec ← getSec s
      if sec.name != sh_name then continue
      if sh.sh_type != sec.sh_type.toUInt32 && sec.name != ".eh_frame".toUTF8 &&
         sec.name != ".note.GNU-stack".toUTF8 then
        let _ ← tcc_error_noabort R (cfmt "section type conflict: %s %02x <> %02x"
          [.s sec.name, .u sh.sh_type.toNat, .i sec.sh_type.toInt])
        return -1
      if startsWith sh_name ".gnu.linkonce" then
        linkOnce := true
        break
      found := s
      break
    if linkOnce then
      sm := sm.set! i { (sm[i]!) with link_once := true }
      continue
    let mut s := found
    if s == 0 then
      s ← new_section sh_name sh.sh_type.toInt32 (sh.sh_flags.toUInt32.toInt32 &&& ~~~SHF_GROUP)
      let al := sh.sh_addralign.toUInt32.toInt32
      let es := sh.sh_entsize.toUInt32.toInt32
      modSec s fun x => { x with sh_addralign := al, sh_entsize := es }
      sm := sm.set! i { (sm[i]!) with new_section := true }
    -- found:
    let size := sh.sh_size
    let offset ← section_add s size sh.sh_addralign.toUInt32.toInt32
    let cur ← getSec s
    if sh.sh_addralign > cur.sh_addralign.toInt64.toUInt64 then
      let al := sh.sh_addralign.toUInt32.toInt32
      modSec s fun x => { x with sh_addralign := al }
    sm := sm.set! i { (sm[i]!) with offset := offset, s := s }
    if sh.sh_type != SHT_NOBITS.toUInt32 && size != 0 then
      let _ ← Host.lseekSet fd (file_offset + sh.sh_offset.toNat)
      let d := (← full_read fd size.toNat).getD ByteArray.empty
      modSecData s fun dd => Bytes.copy dd offset d 0 d.size
    if (← getSec s).sh_flags &&& SHF_EXECINSTR != 0 then
      let _ ← section_add s 0 4
  -- second pass: sh_link and sh_info of the new sections
  for i in [1:shnum] do
    let m := sm[i]!
    if m.s == 0 || !m.new_section then continue
    let sh := shdr[i]!
    if sh.sh_link > 0 then
      let l := (sm[sh.sh_link.toNat]!).s
      modSec m.s fun x => { x with link := l }
    if sh.sh_type == SHT_RELA.toUInt32 then
      let tgt ← getSec (sm[sh.sh_info.toNat]!).s
      modSec m.s fun x => { x with sh_info := tgt.sh_num }
      let tid := (← get).sections[tgt.sh_num.toNat']!
      modSec tid fun x => { x with reloc := m.s }
  match symtab with
  | none =>
    return if (← get).nb_errors == 0 then 0 else -1
  | some symdata =>
    -- resolve symbols
    let mut old_to_new : Array Nat := Array.replicate nb_syms 0
    for i in [1:nb_syms] do
      let mut sym := readSym symdata i
      let shndx := sym.st_shndx.toUInt32.toInt32
      if shndx != SHN_UNDEF && shndx < SHN_LORESERVE then
        let m := sm[shndx.toNat']!
        if m.link_once then
          if ELF_ST_BIND sym.st_info.toInt32 != STB_LOCAL then
            let name := Bytes.cstrAt strtab sym.st_name.toNat
            let si ← find_elf_sym symtabSec name
            if si != 0 then old_to_new := old_to_new.set! i si
          continue
        if m.s == 0 then continue
        let num := (← getSec m.s).sh_num
        sym := { sym with st_shndx := num.toUInt32.toUInt16, st_value := sym.st_value + m.offset.toUInt64 }
      let name := if ELF_ST_BIND sym.st_info.toInt32 == STB_LOCAL then ByteArray.empty
                  else Bytes.cstrAt strtab sym.st_name.toNat
      let si ← set_elf_sym R symtabSec sym.st_value sym.st_size sym.st_info.toInt32 sym.st_other.toInt32
        sym.st_shndx.toUInt32.toInt32 name
      old_to_new := old_to_new.set! i si
    -- third pass: patch the relocation entries
    for i in [1:shnum] do
      let m := sm[i]!
      if m.s == 0 then continue
      let sec ← getSec m.s
      if sec.sh_type != SHT_RELA then continue
      let sh := shdr[i]!
      let offset := m.offset
      let size := sh.sh_size.toNat
      let offseti := (sm[sh.sh_info.toNat]!).offset
      let tgtLinkOnce := (sm[sh.sh_info.toNat]!).link_once
      for k in [offset / 24 : (offset + size) / 24] do
        let d := (← getSec m.s).data
        let r := readRela d (24 * k)
        let ty := R_TYPE r.r_info
        let si0 := R_SYM r.r_info
        let bad : M Int32 := do
          let _ ← tcc_error_noabort R (cfmt "Invalid relocation entry [%2d] '%s' @ %.8x"
            [.i i, .s (Bytes.cstrAt strsec sh.sh_name.toNat), .i r.r_offset.toUInt32.toInt32.toInt])
          return -1
        if si0 ≥ nb_syms then return ← bad
        let si := old_to_new[si0]!
        if si == 0 && !tgtLinkOnce && ty != R_RISCV_ALIGN && ty != R_RISCV_RELAX then
          return ← bad
        modSecData m.s fun d => writeRela d (24 * k)
          { r with r_info := R_INFO si ty, r_offset := r.r_offset + offseti.toUInt64 }
    return if (← get).nb_errors == 0 then 0 else -1

/-! ## archives -/

/-- `get_be(b, n)` -/
def get_be (b : ByteArray) (o n : Nat) : UInt64 := Id.run do
  let mut r : UInt64 := 0
  for i in [0:n] do r := (r <<< 8) ||| (b.get! (o + i)).toUInt64
  return r

/-- `struct ArchiveHeader`, as read_ar_header leaves it -/
structure ArchiveHeader where
  ar_name : ByteArray := ByteArray.empty
  ar_size : ByteArray := ByteArray.empty
  ar_fmag : ByteArray := ByteArray.empty
  deriving Inhabited

/-- `read_ar_header(fd, offset, hdr)` -/
def read_ar_header (fd : Int32) (offset : Nat) : M (Int32 × ArchiveHeader) := do
  let _ ← Host.lseekSet fd offset
  let b := (← full_read fd 60).getD ByteArray.empty
  let len := b.size
  if len != 60 then return (if len != 0 then -1 else 0, {})
  let fmag := b.extract 58 60
  if fmag != "`\n".toUTF8 then return (-1, {})
  let mut e := 16
  for _ in [0:16] do
    if e > 0 && b.get! (e - 1) == 32 then e := e - 1 else break
  let name := Bytes.cstrAt (b.extract 0 e) 0
  let size := Bytes.cstrAt (b.extract 48 57) 0
  return (60, { ar_name := name, ar_size := size, ar_fmag := fmag })

/-- `strtol(s, NULL, 0)` (as newlib's) -/
def strtol0 (s : ByteArray) : Int64 := Id.run do
  let mut i := 0
  for _ in [0:s.size] do
    let c := s.get! i
    if c == 32 || (c ≥ 9 && c ≤ 13) then i := i + 1 else break
  let mut neg := false
  if i < s.size && (s.get! i == 45 || s.get! i == 43) then
    neg := s.get! i == 45; i := i + 1
  let mut base : Nat := 10
  if i < s.size && s.get! i == 48 then
    if i + 1 < s.size && (s.get! (i+1) == 120 || s.get! (i+1) == 88) then base := 16; i := i + 2
    else base := 8
  let mut v : Nat := 0
  for _ in [0:s.size] do
    if i ≥ s.size then break
    let c := s.get! i
    let d : Nat := if c ≥ 48 && c ≤ 57 then (c - 48).toNat
      else if c ≥ 97 && c ≤ 122 then (c - 87).toNat else if c ≥ 65 && c ≤ 90 then (c - 55).toNat else 99
    if d ≥ base then break
    v := v * base + d
    i := i + 1
  let w : Int := if neg then -(v : Int) else v
  return Int64.ofInt w

/-- `bm_ar_lookup(names, tab, mask, name)` -/
def bm_ar_lookup (names : Array ByteArray) (tab : Array UInt32) (mask : UInt32) (name : ByteArray) : Int := Id.run do
  let mut h := elf_hash name &&& mask
  for _ in [0:tab.size] do
    let t := tab[h.toNat]!
    if t == 0 then return -1
    if names[(t - 1).toNat]! == name then return (t - 1).toNat
    h := (h + 1) &&& mask
  return -1

/-- `tcc_load_alacarte(s1, fd, size, entrysize)` (patches/0002) -/
def tcc_load_alacarte (R : Rec) (fd : Int32) (size entrysize : Nat) : M Int32 := do
  let data0 ← full_read fd size
  let data := (data0.getD ByteArray.empty)
  let invalid : M Int32 := do
    let _ ← tcc_error_noabort R "invalid archive".toUTF8
    return -1
  if data.size != size then return ← invalid
  let nsyms := (get_be data 0 entrysize).toNat
  let ar_index := entrysize
  let ar_names := ar_index + nsyms * entrysize
  let mut nb : Nat := 1
  for _ in [0:64] do
    if nb < 2 * nsyms then nb := nb * 2 else break
  let mut tab : Array UInt32 := Array.replicate nb 0
  let mut names : Array ByteArray := Array.mkEmpty nsyms
  let mut p := ar_names
  let mask := (nb - 1).toUInt32
  for i in [0:nsyms] do
    let nm := Bytes.cstrAt data p
    names := names.push nm
    let mut h := elf_hash nm &&& mask
    for _ in [0:nb] do
      if tab[h.toNat]! == 0 then break
      h := (h + 1) &&& mask
    tab := tab.set! h.toNat (i + 1).toUInt32
    p := p + nm.size + 1
  let symtab := (← get).symtab_section
  let mut symoff := 24
  let fuel ← fuelM
  for _ in [0:fuel] do
    let sec ← getSec symtab
    if symoff ≥ sec.data_offset then break
    let sym := readSym sec.data (symoff / 24)
    symoff := symoff + 24
    if sym.st_shndx.toUInt32.toInt32 != SHN_UNDEF || ELF_ST_BIND sym.st_info.toInt32 == STB_LOCAL then
      continue
    let strtab := (← getSec sec.link).data
    let i := bm_ar_lookup names tab mask (Bytes.cstrAt strtab sym.st_name.toNat)
    if i < 0 then continue
    let off := (get_be data (ar_index + i.toNat * entrysize) entrysize).toNat
    let (len, hdr) ← read_ar_header fd off
    if len ≤ 0 || hdr.ar_fmag != "`\n".toUTF8 then return ← invalid
    if (← tcc_load_object_file R fd (off + len.toNat')) < 0 then return -1
  return 0

/-- `tcc_load_archive(s1, fd, alacarte)` -/
def tcc_load_archive (R : Rec) (fd : Int32) (alacarte : Bool) : M Int32 := do
  let mut file_offset := 8
  let fuel ← fuelM
  for _ in [0:fuel] do
    let (len, hdr) ← read_ar_header fd file_offset
    if len == 0 then return 0
    if len < 0 then return ← tcc_error_noabort R "invalid archive".toUTF8
    file_offset := file_offset + len.toNat'
    let size := strtol0 hdr.ar_size
    if alacarte then
      if hdr.ar_name == "/".toUTF8 then return ← tcc_load_alacarte R fd size.toUInt64.toNat 4
      if hdr.ar_name == "/SYM64/".toUTF8 then return ← tcc_load_alacarte R fd size.toUInt64.toNat 8
    else
      let (ot, _) ← tcc_object_type fd
      if ot == AFF_BINTYPE_REL then
        if (← tcc_load_object_file R fd file_offset) < 0 then return -1
    file_offset := ((file_offset.toUInt64 + size.toUInt64 + 1) &&& ~~~1).toNat
  return 0

/-! ## libtcc.c's file layer -/

/-- `guess_filetype(filename)` -/
def guess_filetype (filename : ByteArray) : Int32 :=
  let ext := tcc_fileextension filename
  if ext.size > 0 then
    let e := ext.extract 1 ext.size
    if e == "S".toUTF8 then AFF_TYPE_ASMPP
    else if e == "s".toUTF8 then AFF_TYPE_ASM
    else if e == "c".toUTF8 || e == "h".toUTF8 || e == "i".toUTF8 then AFF_TYPE_C
    else AFF_TYPE_BIN
  else AFF_TYPE_C

/-- `tcc_add_binary(s1, flags, filename, fd)` -/
def tcc_add_binary (R : Rec) (flags : Int32) (filename : ByteArray) (fd : Int32) : M Int32 := do
  let saved := (← get).current_filename
  modify fun s => { s with current_filename := some filename }
  let (obj_type, _) ← tcc_object_type fd
  let _ ← Host.lseekSet fd 0
  let mut ret : Int32 := 0
  if obj_type == AFF_BINTYPE_REL then
    ret ← tcc_load_object_file R fd 0
  else if obj_type == AFF_BINTYPE_AR then
    ret ← tcc_load_archive R fd (flags &&& AFF_WHOLE_ARCHIVE == 0)
  else if obj_type == AFF_BINTYPE_DYN then
    -- -run: dlopen; CONFIG_TCC_STATIC's dlopen finds nothing
    ret := FILE_NOT_RECOGNIZED
  else
    -- tcc_load_ldscript: no linker scripts in the image's file system
    unsupported "tcc_load_ldscript"
  Host.close fd
  modify fun s => { s with current_filename := saved }
  if ret == FILE_NOT_RECOGNIZED then
    return ← tcc_error_noabort R (cfmt "%s: unrecognized file type" [.s filename])
  return ret

/-- `tcc_add_file_internal(s1, filename, flags)` -/
def tcc_add_file_internal (R : Rec) (filename : ByteArray) (flags0 : Int32) : M Int32 := do
  let mut flags := flags0
  if flags &&& AFF_TYPE_MASK == 0 then flags := flags ||| guess_filetype filename
  if (← get).output_type == TCC_OUTPUT_PREPROCESS && flags &&& AFF_TYPE_BIN != 0 then return 0
  let fd ← _tcc_open filename
  if fd < 0 then
    if flags &&& AFF_PRINT_ERROR != 0 then
      let _ ← tcc_error_noabort R (cfmt "file '%s' not found" [.s filename])
    return FILE_NOT_FOUND
  if flags &&& AFF_TYPE_BIN != 0 then
    return ← tcc_add_binary R flags filename fd
  R.tcc_compile flags filename fd

/-- `tcc_add_library_internal(s1, fmt, filename, flags, paths, nb_paths)`;
`fmt` is "%s/%s", "%s/lib%s.so" or "%s/lib%s.a", given as prefix/suffix -/
def tcc_add_library_internal (R : Rec) (pre suf : String) (filename : ByteArray) (flags : Int32)
    (paths : Array ByteArray) : M Int32 := do
  for p in paths do
    let buf := pstrcpy 1024 (p ++ pre.toUTF8 ++ filename ++ suf.toUTF8)
    let ret ← R.tcc_add_file_internal buf (flags &&& ~~~AFF_PRINT_ERROR)
    if ret != FILE_NOT_FOUND then return ret
  if flags &&& AFF_PRINT_ERROR != 0 then
    let _ ← tcc_error_noabort R (cfmt "%s '%s' not found"
      [.s (if flags &&& AFF_TYPE_LIB != 0 then "library" else "file").toUTF8, .s filename])
  return FILE_NOT_FOUND

/-- `tcc_add_dll(s, filename, flags)` -/
def tcc_add_dll (R : Rec) (filename : ByteArray) (flags : Int32) : M Int32 := do
  tcc_add_library_internal R "/" "" filename flags (← get).library_paths

/-- `tcc_add_support(s1, filename)` (CONFIG_TCC_CROSSPREFIX is empty) -/
def tcc_add_support (R : Rec) (filename : String) : M Int32 :=
  tcc_add_dll R filename.toUTF8 AFF_PRINT_ERROR

/-- `tcc_add_library(s, libraryname)` -/
def tcc_add_library (R : Rec) (libraryname0 : ByteArray) : M Int32 := do
  let s ← get
  let flags := AFF_TYPE_LIB ||| (s.filetype &&& AFF_WHOLE_ARCHIVE)
  let mut libraryname := libraryname0
  if libraryname.size > 0 && libraryname.get! 0 == 58 then
    libraryname := libraryname.extract 1 libraryname.size
  else
    let libs : List (String × String) := if s.static_link != 0 then [("/lib", ".a")]
      else [("/lib", ".so"), ("/lib", ".a")]
    for (pre, suf) in libs do
      let ret ← tcc_add_library_internal R pre suf libraryname flags (← get).library_paths
      if ret != FILE_NOT_FOUND then return ret
  tcc_add_dll R libraryname (flags ||| AFF_PRINT_ERROR)

/-- `tcc_add_pragma_libs(s1)` -/
def tcc_add_pragma_libs (R : Rec) : M Unit := do
  let libs := (← get).pragma_libs
  for l in libs do
    let _ ← tcc_add_library R l

/-! ## linking (tccelf.c) -/

/-- `tcc_add_runtime(s1)` -/
def tcc_add_runtime (R : Rec) : M Unit := do
  modify fun s => { s with filetype := 0 }
  tcc_add_pragma_libs R
  let s ← get
  if s.nostdlib == 0 then
    if s.option_pthread != 0 then
      let _ ← tcc_add_library R "pthread".toUTF8
    let _ ← tcc_add_library R "c".toUTF8
    let _ ← tcc_add_support R "libtcc1.a"
    let _ ← tcc_add_library R "c".toUTF8
    let _ ← tcc_add_library R "gcc".toUTF8

/-- `set_linker_sym(s1, name, sec, f)` -/
def set_linker_sym (R : Rec) (name0 : ByteArray) (sec : SecId) (f0 : Nat) : M Unit := do
  let mut name := name0
  let mut f := f0
  for _ in [0:name0.size + 1] do
    let s ← get
    let sym_index ← find_elf_sym s.symtab_section name
    let sym ← getElfSym s.symtab_section sym_index
    let esym_index ← find_elf_sym s.dynsymtab_section name
    let esym ← getElfSym s.dynsymtab_section esym_index
    let defined := sym.st_shndx.toUInt32.toInt32 != SHN_UNDEF ||
      (esym.st_shndx.toUInt32.toInt32 != SHN_UNDEF && esym.st_size != 0)
    let doSet : Bool ←
      if f == 1 && (!(sym_index != 0 || esym_index != 0) || defined) then pure false
      else if f ≤ 1 then
        if defined then
          tcc_warning R (cfmt "linker symbol '%s' already defined" [.s name])
          pure false
        else pure true
      else
        let a ← get_sym_attr sym_index false
        if a.linker_sym != 0 then
          let off := (← getSec sec).data_offset.toUInt64
          setElfSym s.symtab_section sym_index { sym with st_value := off }
        pure false
    if doSet then
      let si ← set_global_sym R (some name) sec none
      let a ← get_sym_attr si true
      set_sym_attr si { a with linker_sym := 1 }
    if name.size > 0 && name.get! 0 == 95 then
      name := name.extract 1 name.size
      f := f + 1
    else break

/-- `add_init_array_defines(s1, section_name)` -/
def add_init_array_defines (R : Rec) (section_name : String) : M Unit := do
  let s0 ← have_section section_name.toUTF8
  let mut s := s0
  let mut end_offset : UInt64 := 0
  if s == 0 || (← getSec s).sh_flags &&& SHF_ALLOC == 0 then
    end_offset := 0
    s := (← get).text_section
  else
    end_offset := (← getSec s).data_offset.toUInt64
  let base := (section_name.drop 1).toString
  let _ ← set_global_sym R (some (pstrcpy 1024 s!"__{base}_start".toUTF8)) s (some 0)
  let _ ← set_global_sym R (some (pstrcpy 1024 s!"__{base}_end".toUTF8)) s (some end_offset)

/-- `tcc_add_linker_symbols(s1)` -/
def tcc_add_linker_symbols (R : Rec) : M Unit := do
  let s ← get
  set_linker_sym R "_etext".toUTF8 s.text_section 0
  set_linker_sym R "_edata".toUTF8 s.data_section 0
  set_linker_sym R "_end".toUTF8 s.bss_section 0
  let _ ← set_global_sym R (some "__global_pointer$".toUTF8) s.data_section (some 0x800)
  add_init_array_defines R ".preinit_array"
  add_init_array_defines R ".init_array"
  add_init_array_defines R ".fini_array"
  let mut i := 1
  let fuel ← fuelM
  for _ in [0:fuel] do
    let secs := (← get).sections
    if i ≥ secs.size then break
    let sid := secs[i]!
    let sec ← getSec sid
    i := i + 1
    if sec.sh_flags &&& SHF_ALLOC != 0 &&
       (sec.sh_type == SHT_PROGBITS || sec.sh_type == SHT_NOBITS || sec.sh_type == SHT_STRTAB) then
      let p0 := if sec.name.size > 0 && sec.name.get! 0 == 46 then sec.name.extract 1 sec.name.size else sec.name
      let ok := p0.data.all fun c => isId c.toInt32 || isNum c.toInt32
      if ok then
        let _ ← set_global_sym R (some (pstrcpy 1024 ("__start_".toUTF8 ++ p0))) sid (some 0)
        let _ ← set_global_sym R (some (pstrcpy 1024 ("__stop_".toUTF8 ++ p0))) sid none

/-- `resolve_common_syms(s1)` -/
def resolve_common_syms (R : Rec) : M Unit := do
  let s ← get
  let st := s.symtab_section
  let n := (← getSec st).data_offset / 24
  for i in [1:n] do
    let sym ← getElfSym st i
    if sym.st_shndx.toUInt32.toInt32 == SHN_COMMON && sym.st_size != 0 then
      let v ← section_add s.bss_section sym.st_size sym.st_value.toUInt32.toInt32
      let num := (← getSec s.bss_section).sh_num
      setElfSym st i { sym with st_value := v.toUInt64, st_shndx := num.toUInt32.toUInt16 }
  if (← get).output_type != TCC_OUTPUT_DLL then
    tcc_add_linker_symbols R

/-! ## riscv64-link.c -/

/-- `code_reloc(reloc_type)` -/
def code_reloc (t : Int32) : Int32 :=
  if t == R_RISCV_BRANCH || t == R_RISCV_CALL || t == R_RISCV_JAL then 1
  else if t == R_RISCV_GOT_HI20 || t == R_RISCV_PCREL_HI20 || t == R_RISCV_PCREL_LO12_I ||
    t == R_RISCV_PCREL_LO12_S || t == R_RISCV_32_PCREL || t == R_RISCV_SET6 || t == R_RISCV_SET8 ||
    t == R_RISCV_SET16 || t == R_RISCV_SUB6 || t == R_RISCV_ADD16 || t == R_RISCV_ADD32 ||
    t == R_RISCV_ADD64 || t == R_RISCV_SUB8 || t == R_RISCV_SUB16 || t == R_RISCV_SUB32 ||
    t == R_RISCV_SUB64 || t == R_RISCV_32 || t == R_RISCV_64 || t == R_RISCV_SET_ULEB128 ||
    t == R_RISCV_SUB_ULEB128 || t == R_RISCV_TPREL_HI20 || t == R_RISCV_TPREL_LO12_I then 0
  else if t == R_RISCV_CALL_PLT then 1
  else -1

/-- `gotplt_entry_type(reloc_type)` -/
def gotplt_entry_type (t : Int32) : Int32 :=
  if t == R_RISCV_ALIGN || t == R_RISCV_RELAX || t == R_RISCV_RVC_BRANCH || t == R_RISCV_RVC_JUMP ||
     t == R_RISCV_JUMP_SLOT || t == R_RISCV_SET6 || t == R_RISCV_SET8 || t == R_RISCV_SET16 ||
     t == R_RISCV_SUB6 || t == R_RISCV_ADD16 || t == R_RISCV_SUB8 || t == R_RISCV_SUB16 ||
     t == R_RISCV_SET_ULEB128 || t == R_RISCV_SUB_ULEB128 then NO_GOTPLT_ENTRY
  else if t == R_RISCV_BRANCH || t == R_RISCV_CALL || t == R_RISCV_PCREL_HI20 ||
     t == R_RISCV_PCREL_LO12_I || t == R_RISCV_PCREL_LO12_S || t == R_RISCV_32_PCREL ||
     t == R_RISCV_ADD32 || t == R_RISCV_ADD64 || t == R_RISCV_SUB32 || t == R_RISCV_SUB64 ||
     t == R_RISCV_32 || t == R_RISCV_64 || t == R_RISCV_JAL || t == R_RISCV_CALL_PLT then AUTO_GOTPLT_ENTRY
  else if t == R_RISCV_GOT_HI20 then ALWAYS_GOTPLT_ENTRY
  else if t == R_RISCV_TPREL_HI20 || t == R_RISCV_TPREL_LO12_I then NO_GOTPLT_ENTRY
  else -1

/-- `create_plt_entry(s1, got_offset, attr)` -/
def create_plt_entry (got_offset : UInt32) : M UInt32 := do
  let plt := (← get).plt
  if (← getSec plt).data_offset == 0 then
    let _ ← section_ptr_add plt 32
  let plt_offset := (← getSec plt).data_offset
  let p ← section_ptr_add plt 16
  modSecData plt fun d => Bytes.w64 d p got_offset.toUInt64
  return plt_offset.toUInt32

/-- `relocate_plt(s1)` -/
def relocate_plt (R : Rec) : M Unit := do
  let s ← get
  if s.plt == 0 then return
  let pltSec ← getSec s.plt
  let p_end := pltSec.data_offset
  if 0 < p_end then
    let plt := pltSec.sh_addr
    let got := (← getSec s.got).sh_addr
    let off := (got - plt + 0x800) >>> 12
    if (off + ((1 : UInt64) <<< 20)) >>> 21 != 0 then
      let _ ← tcc_error_noabort R (cfmt "Failed relocating PLT (off=0x%lx, got=0x%lx, plt=0x%lx)"
        [.u off.toNat, .u got.toNat, .u plt.toNat])
    let lo := ((got - plt) &&& 0xfff).toUInt32 <<< 20
    modSecData s.plt fun d =>
      let d := Bytes.w32 d 0 ((0x397 : UInt32) ||| (off.toUInt32 <<< 12))
      let d := Bytes.w32 d 4 0x41c30333
      let d := Bytes.w32 d 8 ((0x0003be03 : UInt32) ||| lo)
      let d := Bytes.w32 d 12 0xfd430313
      let d := Bytes.w32 d 16 ((0x00038293 : UInt32) ||| lo)
      let d := Bytes.w32 d 20 0x00135313
      let d := Bytes.w32 d 24 0x0082b283
      Bytes.w32 d 28 0x000e0067
    let mut p := 32
    let fuel ← fuelM
    for _ in [0:fuel] do
      if p ≥ p_end then break
      let d := (← getSec s.plt).data
      let pc := plt + p.toUInt64
      let addr := got + Bytes.r64 d p
      let off := (addr - pc + 0x800) >>> 12
      if (off + ((1 : UInt64) <<< 20)) >>> 21 != 0 then
        let _ ← tcc_error_noabort R (cfmt "Failed relocating PLT (off=0x%lx, addr=0x%lx, pc=0x%lx)"
          [.u off.toNat, .u addr.toNat, .u pc.toNat])
      let lo := ((addr - pc) &&& 0xfff).toUInt32 <<< 20
      let pp := p
      modSecData s.plt fun d =>
        let d := Bytes.w32 d pp ((0xe17 : UInt32) ||| (off.toUInt32 <<< 12))
        let d := Bytes.w32 d (pp+4) ((0x000e3e03 : UInt32) ||| lo)
        let d := Bytes.w32 d (pp+8) 0x000e0367
        Bytes.w32 d (pp+12) 0x00000013
      p := p + 16
  let pltSec ← getSec s.plt
  if pltSec.reloc != 0 then
    let rs ← getSec pltSec.reloc
    for k in [0:rs.data_offset / 24] do
      let r := readRela rs.data (24 * k)
      let a := pltSec.sh_addr
      modSecData s.got fun d => Bytes.w64 d r.r_offset.toNat a

/-- `riscv64_record_pcrel_hi(s1, addr, val)` -/
def riscv64_record_pcrel_hi (addr val : UInt64) : M Unit :=
  modify fun s => { s with pcrel_hi_entries := s.pcrel_hi_entries.push (addr, val) }

/-- `riscv64_lookup_pcrel_hi(s1, hi_addr, &hi_val)`: (ret, hi_val) -/
def riscv64_lookup_pcrel_hi (R : Rec) (hi_addr : UInt64) (hi_val0 : UInt64) : M (Int32 × UInt64) := do
  let es := (← get).pcrel_hi_entries
  let mut i := es.size
  for _ in [0:es.size] do
    i := i - 1
    let (a, v) := es[i]!
    if a == hi_addr then return (0, v)
  return (← tcc_error_noabort R "unsupported hi/lo pcrel reloc scheme".toUTF8, hi_val0)

/-- `relocate(s1, rel, type, ptr, addr, val)`: `sec`/`off` locate `ptr` -/
def relocate (R : Rec) (rel : ElfRela) (ty : Int32) (sec : SecId) (off : Nat) (addr0 val0 : UInt64) : M Unit := do
  let s ← get
  let sym_index := R_SYM rel.r_info
  let sym ← getElfSym s.symtab_section sym_index
  let rd32 : M UInt32 := do return Bytes.r32 (← getSec sec).data off
  let rd16 : M UInt16 := do return Bytes.r16 (← getSec sec).data off
  let rd64 : M UInt64 := do return Bytes.r64 (← getSec sec).data off
  let rd8 : M UInt8 := do return (← getSec sec).data.get! off
  let wr32 (o : Nat) (v : UInt32) : M Unit := modSecData sec fun d => Bytes.w32 d o v
  let wr16 (v : UInt16) : M Unit := modSecData sec fun d => Bytes.w16 d off v
  let wr64 (v : UInt64) : M Unit := modSecData sec fun d => Bytes.w64 d off v
  let wr8 (v : UInt8) : M Unit := modSecData sec fun d => d.set! off v
  let addr := addr0
  let val := val0
  if ty == R_RISCV_ALIGN || ty == R_RISCV_RELAX then return
  else if ty == R_RISCV_BRANCH then
    let off64 := val - addr
    if (off64 + ((1 : UInt64) <<< 12)) &&& ~~~(0x1ffe : UInt64) != 0 then
      let _ ← tcc_error_noabort R (cfmt "R_RISCV_BRANCH relocation failed (val=%lx, addr=%lx)" [.u val.toNat, .u addr.toNat])
    let off32 := (off64 >>> 1).toUInt32
    wr32 off ((← rd32) &&& ~~~(0xfe000f80 : UInt32) ||| ((off32 &&& 0x800) <<< 20) ||| ((off32 &&& 0x3f0) <<< 21) |||
      ((off32 &&& 0x00f) <<< 8) ||| ((off32 &&& 0x400) >>> 3))
  else if ty == R_RISCV_JAL then
    let off64 := val - addr
    if (off64 + ((1 : UInt64) <<< 21)) &&& ~~~(((1 : UInt64) <<< 22) - 2) != 0 then
      let _ ← tcc_error_noabort R (cfmt "R_RISCV_JAL relocation failed (val=%lx, addr=%lx)" [.u val.toNat, .u addr.toNat])
    let off32 := off64.toUInt32
    wr32 off (((← rd32) &&& (0xfff : UInt32)) ||| (((off32 >>> 12) &&& 0xff) <<< 12) ||| (((off32 >>> 11) &&& 1) <<< 20) |||
      (((off32 >>> 1) &&& 0x3ff) <<< 21) ||| (((off32 >>> 20) &&& 1) <<< 31))
  else if ty == R_RISCV_CALL || ty == R_RISCV_CALL_PLT then
    let w0 ← rd32
    wr32 off ((w0 &&& (0xfff : UInt32)) ||| ((val - addr + 0x800).toUInt32 &&& ~~~(0xfff : UInt32)))
    let w1 := Bytes.r32 (← getSec sec).data (off + 4)
    wr32 (off + 4) ((w1 &&& (0xfffff : UInt32)) ||| (((val - addr).toUInt32 &&& 0xfff) <<< 20))
  else if ty == R_RISCV_PCREL_HI20 then
    let off64 := ((val - addr + 0x800).toInt64 >>> 12).toUInt64
    if (off64 + ((1 : UInt64) <<< 20)) >>> 21 != 0 then
      let nm ← symName s.symtab_section sym
      let _ ← tcc_error_noabort R (cfmt "R_RISCV_PCREL_HI20 relocation failed: off=%lx cond=%lx sym=%s"
        [.u off64.toNat, .i ((off64 + ((1 : UInt64) <<< 20)).toInt64 >>> 21).toInt, .s nm])
    wr32 off (((← rd32) &&& (0xfff : UInt32)) ||| ((off64 &&& 0xfffff).toUInt32 <<< 12))
    riscv64_record_pcrel_hi addr val
  else if ty == R_RISCV_GOT_HI20 then
    let a ← get_sym_attr sym_index false
    let val := (← getSec s.got).sh_addr + a.got_offset.toUInt64
    let off64 := ((val - addr + 0x800).toInt64 >>> 12).toUInt64
    if (off64 + ((1 : UInt64) <<< 20)) >>> 21 != 0 then
      let _ ← tcc_error_noabort R "R_RISCV_GOT_HI20 relocation failed".toUTF8
    wr32 off (((← rd32) &&& (0xfff : UInt32)) ||| ((off64 &&& 0xfffff).toUInt32 <<< 12))
    riscv64_record_pcrel_hi addr val
  else if ty == R_RISCV_PCREL_LO12_I then
    let addr := val
    let (_, val) ← riscv64_lookup_pcrel_hi R addr val
    wr32 off (((← rd32) &&& (0xfffff : UInt32)) ||| (((val - addr).toUInt32 &&& 0xfff) <<< 20))
  else if ty == R_RISCV_PCREL_LO12_S then
    let addr := val
    let (_, val) ← riscv64_lookup_pcrel_hi R addr val
    let off32 := (val - addr).toUInt32
    wr32 off (((← rd32) &&& ~~~(0xfe000f80 : UInt32)) ||| ((off32 &&& 0xfe0) <<< 20) ||| ((off32 &&& 0x01f) <<< 7))
  else if ty == R_RISCV_RVC_BRANCH then
    let off64 := val - addr
    if (off64 + ((1 : UInt64) <<< 8)) &&& ~~~(0x1fe : UInt64) != 0 then
      let _ ← tcc_error_noabort R (cfmt "R_RISCV_RVC_BRANCH relocation failed (val=%lx, addr=%lx)" [.u val.toNat, .u addr.toNat])
    let o := off64.toUInt16
    wr16 (((← rd16) &&& (0xe383 : UInt16)) ||| (((o >>> 5) &&& 1) <<< 2) ||| (((o >>> 1) &&& 3) <<< 3) |||
      (((o >>> 6) &&& 3) <<< 5) ||| (((o >>> 3) &&& 3) <<< 10) ||| (((o >>> 8) &&& 1) <<< 12))
  else if ty == R_RISCV_RVC_JUMP then
    let off64 := val - addr
    if (off64 + ((1 : UInt64) <<< 11)) &&& ~~~(0xffe : UInt64) != 0 then
      let _ ← tcc_error_noabort R (cfmt "R_RISCV_RVC_BRANCH relocation failed (val=%lx, addr=%lx)" [.u val.toNat, .u addr.toNat])
    let o := off64.toUInt16
    wr16 (((← rd16) &&& (0xe003 : UInt16)) ||| (((o >>> 5) &&& 1) <<< 2) ||| (((o >>> 1) &&& 7) <<< 3) |||
      (((o >>> 7) &&& 1) <<< 6) ||| (((o >>> 6) &&& 1) <<< 7) ||| (((o >>> 10) &&& 1) <<< 8) |||
      (((o >>> 8) &&& 3) <<< 9) ||| (((o >>> 4) &&& 1) <<< 11) ||| (((o >>> 11) &&& 1) <<< 12))
  else if ty == R_RISCV_32 then
    -- not TCC_OUTPUT_DYN: add32le(ptr, val)
    wr32 off ((← rd32) + val.toUInt32)
  else if ty == R_RISCV_64 || ty == R_RISCV_JUMP_SLOT then
    wr64 ((← rd64) + val)
  else if ty == R_RISCV_ADD64 then wr64 ((← rd64) + val)
  else if ty == R_RISCV_ADD32 then wr32 off ((← rd32) + val.toUInt32)
  else if ty == R_RISCV_SUB64 then wr64 ((← rd64) - val)
  else if ty == R_RISCV_SUB32 then wr32 off ((← rd32) - val.toUInt32)
  else if ty == R_RISCV_ADD16 then wr16 ((← rd16) + val.toUInt16)
  else if ty == R_RISCV_SUB8 then wr8 ((← rd8) - val.toUInt8)
  else if ty == R_RISCV_SUB16 then wr16 ((← rd16) - val.toUInt16)
  else if ty == R_RISCV_SET6 then wr8 (((← rd8) &&& ~~~0x3f) ||| (val.toUInt8 &&& 0x3f))
  else if ty == R_RISCV_SET8 then wr8 val.toUInt8
  else if ty == R_RISCV_SET16 then wr16 val.toUInt16
  else if ty == R_RISCV_SUB6 then
    let b ← rd8
    wr8 ((b &&& ~~~0x3f) ||| ((b - val.toUInt8) &&& 0x3f))
  else if ty == R_RISCV_32_PCREL then
    wr32 off ((← rd32) + (val - addr).toUInt32)
  else if ty == R_RISCV_SET_ULEB128 || ty == R_RISCV_SUB_ULEB128 || ty == R_RISCV_COPY || ty == R_RISCV_RELATIVE then
    return
  else if ty == R_RISCV_TPREL_HI20 || ty == R_RISCV_TPREL_LO12_I then
    -- s1->tls_start is 0 (thread-local data is refused for -run before relocation)
    let tp_offset := val
    if ty == R_RISCV_TPREL_HI20 then
      let off64 := ((tp_offset + 0x800).toInt64 >>> 12).toUInt64
      if (off64 + ((1 : UInt64) <<< 20)) >>> 21 != 0 then
        let _ ← tcc_error_noabort R "R_RISCV_TPREL_HI20 relocation failed".toUTF8
      wr32 off (((← rd32) &&& (0xfff : UInt32)) ||| ((off64 &&& 0xfffff).toUInt32 <<< 12))
    else
      wr32 off (((← rd32) &&& (0xfffff : UInt32)) ||| ((tp_offset.toUInt32 &&& 0xfff) <<< 20))
  else
    -- "FIXME: handle reloc type %x at %x [%p] to %x" prints a host pointer
    unsupported s!"relocate: reloc type {ty}"

/-! ## GOT and PLT (tccelf.c) -/

/-- `build_got(s1)` -/
def build_got (R : Rec) : M Nat := do
  let got ← new_section ".got".toUTF8 SHT_PROGBITS (SHF_ALLOC ||| SHF_WRITE)
  modify fun s => { s with got := got }
  modSec got fun x => { x with sh_entsize := 4 }
  let _ ← section_ptr_add got (3 * 8)
  let num := (← getSec got).sh_num
  set_elf_sym R (← get).symtab_section 0 0 (ELF_ST_INFO STB_GLOBAL STT_OBJECT) 0 num "_GLOBAL_OFFSET_TABLE_".toUTF8

/-- `put_got_entry(s1, dyn_reloc_type, sym_index)` (s1->dynsym is NULL for -run) -/
def put_got_entry (dyn_reloc_type : Int32) (sym_index : Nat) : M SymAttrX := do
  let need_plt_entry := dyn_reloc_type == R_JMP_SLOT
  let attr ← get_sym_attr sym_index true
  if (if need_plt_entry then attr.plt_offset else attr.got_offset) != 0 then return attr
  if need_plt_entry then
    if (← get).plt == 0 then
      let plt ← new_section ".plt".toUTF8 SHT_PROGBITS (SHF_ALLOC ||| SHF_EXECINSTR)
      modify fun s => { s with plt := plt }
      modSec plt fun x => { x with sh_entsize := 4 }
  let s ← get
  let got_offset := (← getSec s.got).data_offset
  let _ ← section_ptr_add s.got 8
  let sym ← getElfSym s.symtab_section sym_index
  let name ← symName s.symtab_section sym
  put_elf_reloca s.symtab_section s.got got_offset.toUInt64 dyn_reloc_type sym_index 0
  let mut attr := attr
  if need_plt_entry then
    let plt_offset ← create_plt_entry got_offset.toUInt32
    let len := min name.size (200 - 5)
    let plt_name := name.extract 0 len ++ "@plt".toUTF8
    let pnum := (← getSec (← get).plt).sh_num
    let ps ← put_elf_sym (← get).symtab_section plt_offset.toUInt64 0 (ELF_ST_INFO STB_GLOBAL STT_FUNC) 0 pnum (some plt_name)
    attr := { attr with plt_offset := plt_offset, plt_sym := Int32.ofNat ps }
  else
    attr := { attr with got_offset := got_offset.toUInt32 }
  set_sym_attr sym_index attr
  return attr

/-- `build_got_entries(s1, got_sym)` -/
def build_got_entries (R : Rec) (got_sym0 : Nat) : M Unit := do
  let mut got_sym := got_sym0
  for pass in [0:2] do
    let mut i := 1
    let fuel ← fuelM
    for _ in [0:fuel] do
      let secs := (← get).sections
      if i ≥ secs.size then break
      let sid := secs[i]!
      i := i + 1
      let sec ← getSec sid
      if sec.sh_type != SHT_RELA then continue
      let symtab := (← get).symtab_section
      if sec.link != symtab then continue
      let nrel := sec.data_offset / 24
      for k in [0:nrel] do
        let r := readRela (← getSec sid).data (24 * k)
        let ty := R_TYPE r.r_info
        let gotplt_entry := gotplt_entry_type ty
        if gotplt_entry == -1 then
          let _ ← tcc_error_noabort R (cfmt "Unknown relocation type for got: %d" [.i ty.toInt])
          continue
        let sym_index := R_SYM r.r_info
        let sym ← getElfSym symtab sym_index
        if gotplt_entry == NO_GOTPLT_ENTRY then continue
        if gotplt_entry == AUTO_GOTPLT_ENTRY then
          let shn := sym.st_shndx.toUInt32.toInt32
          if shn == SHN_UNDEF then
            pure ()   -- s1->dynsym is NULL for -run
          else if shn == SHN_ABS then
            if sym.st_value == 0 then continue
          else continue
        let reloc_type0 := code_reloc ty
        if reloc_type0 == -1 then
          let _ ← tcc_error_noabort R (cfmt "Unknown relocation type: %d" [.i ty.toInt])
          continue
        let mut reloc_type := reloc_type0
        if reloc_type != 0 then
          if pass != 0 then continue
          reloc_type := R_JMP_SLOT
        else
          if pass != 1 then continue
          reloc_type := R_GLOB_DAT
        if (← get).got == 0 then got_sym ← build_got R
        if gotplt_entry == BUILD_GOT_ONLY then continue
        let attr ← put_got_entry reloc_type sym_index
        if reloc_type == R_JMP_SLOT then
          modSecData sid fun d => writeRela d (24 * k) { r with r_info := R_INFO attr.plt_sym.toNat' ty }
  let s ← get
  if s.plt != 0 then
    let pr := (← getSec s.plt).reloc
    if pr != 0 then
      let gn := (← getSec s.got).sh_num
      modSec pr fun x => { x with sh_info := gn }
  if got_sym != 0 then
    let gsz := (← getSec s.got).data_offset.toUInt64
    let e ← getElfSym s.symtab_section got_sym
    setElfSym s.symtab_section got_sym { e with st_size := gsz }

/-! ## relocation -/

/-- `relocate_syms(s1, symtab, do_resolve)` -/
def relocate_syms (R : Rec) (symtab : SecId) (do_resolve : Int32) : M Unit := do
  let n := (← getSec symtab).data_offset / 24
  for i in [1:n] do
    let sym ← getElfSym symtab i
    let sh_num := sym.st_shndx.toUInt32.toInt32
    if sh_num == SHN_UNDEF then
      if do_resolve == 2 then continue
      let s ← get
      let name := Bytes.cstrAt (← getSec (← getSec s.symtab_section).link).data sym.st_name.toNat
      -- do_resolve: CONFIG_TCC_STATIC's dlsym has an empty table under TCC_BAREMETAL
      if do_resolve == 0 && s.dynsym != 0 then
        if (← find_elf_sym s.dynsym name) != 0 then continue
      if name == "_fp_hw".toUTF8 then continue
      if ELF_ST_BIND sym.st_info.toInt32 == STB_WEAK then
        setElfSym symtab i { sym with st_value := 0 }
      else
        let _ ← tcc_error_noabort R (cfmt "unresolved reference to '%s'" [.s name])
    else if sh_num < SHN_LORESERVE then
      let s ← get
      let a := (← getSec s.sections[sh_num.toNat']!).sh_addr
      setElfSym symtab i { sym with st_value := sym.st_value + a }

/-- `relocate_section(s1, s, sr)` -/
def relocate_section (R : Rec) (s sr : SecId) : M Unit := do
  let nrel := (← getSec sr).data_offset / 24
  for k in [0:nrel] do
    let sec ← getSec s
    if sec.data.size == 0 then continue
    let rel := readRela (← getSec sr).data (24 * k)
    let st := (← get).symtab_section
    let sym ← getElfSym st (R_SYM rel.r_info)
    let ty := R_TYPE rel.r_info
    let tgt := sym.st_value + rel.r_addend.toUInt64
    -- no dwarf sections (dwlo = dwhi = 0)
    let addr := sec.sh_addr + rel.r_offset
    relocate R rel ty s rel.r_offset.toNat addr tgt
  -- SHF_ALLOC relocation sections: sr->link = s1->dynsym (NULL)
  if (← getSec sr).sh_flags &&& SHF_ALLOC != 0 then
    modSec sr fun x => { x with link := 0 }
  modify fun st => { st with pcrel_hi_entries := #[] }

/-- `relocate_sections(s1)` -/
def relocate_sections (R : Rec) : M Unit := do
  let mut i := 1
  let fuel ← fuelM
  for _ in [0:fuel] do
    let secs := (← get).sections
    if i ≥ secs.size then break
    let sr := secs[i]!
    i := i + 1
    let srs ← getSec sr
    if srs.sh_type != SHT_RELA then continue
    let s := secs[srs.sh_info.toNat']!
    -- output MEMORY: the GOT is relocated too
    relocate_section R s sr
    if (← getSec sr).sh_flags &&& SHF_ALLOC != 0 then
      let a := (← getSec s).sh_addr
      let n := (← getSec sr).data_offset / 24
      modSecData sr fun d => Id.run do
        let mut d := d
        for k in [0:n] do
          let r := readRela d (24 * k)
          d := writeRela d (24 * k) { r with r_offset := r.r_offset + a }
        return d

end Tcc.Model
