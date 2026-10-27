import Tcc.Model.Host
import Tcc.Model.Rec

/-!
# TccModel: libtcc.c (strings, messages, the input-file layer) and
`get_tok_str` (tccpp.c), which the messages need
-/

namespace Tcc.Model
open C

/-! ## strings (libtcc.c) -/

/-- `pstrcpy(buf, size, s)`: at most size-1 bytes -/
def pstrcpy (size : Nat) (s : ByteArray) : ByteArray :=
  if size == 0 then ByteArray.empty else s.extract 0 (min s.size (size - 1))

/-- `pstrcat(buf, size, s)` -/
def pstrcat (buf : ByteArray) (size : Nat) (s : ByteArray) : ByteArray :=
  if buf.size < size then buf ++ pstrcpy (size - buf.size) s else buf

/-- the offset of tcc_basename(name) -/
def basenameOff (name : ByteArray) : Nat := Id.run do
  let mut p := name.size
  for _ in [0:name.size] do
    if p > 0 && name.get! (p - 1) != 47 then p := p - 1 else break
  return p

def tcc_basename (name : ByteArray) : ByteArray := name.extract (basenameOff name) name.size

/-- tcc_fileextension: from the last '.' of the basename, or empty -/
def tcc_fileextension (name : ByteArray) : ByteArray := Id.run do
  let b := basenameOff name
  let mut e : Option Nat := none
  for i in [b:name.size] do
    if name.get! i == 46 then e := some i
  match e with
  | some i => return name.extract i name.size
  | none => return ByteArray.empty

/-- `tcc_split_path(s, ary, nb, in)` -/
def tcc_split_path (libPath : ByteArray) (curFileName : Option ByteArray) (inp : ByteArray) : Array ByteArray := Id.run do
  let mut out : Array ByteArray := #[]
  let mut p := 0
  let n := inp.size
  for _ in [0:n + 1] do
    let mut str := ByteArray.empty
    let mut q := p
    for _ in [0:n + 1] do
      if q ≥ n then break
      let c := inp.get! q
      if c == 58 then break   -- PATHSEP ':'
      if c == 123 && q + 2 < n && inp.get! (q+1) != 0 && inp.get! (q+2) == 125 then
        let c1 := inp.get! (q+1)
        q := q + 3
        if c1 == 66 then str := str ++ libPath           -- {B}
        if c1 == 102 then                                -- {f}
          match curFileName with
          | some f =>
            let b := basenameOff f
            if b > 0 then str := str ++ f.extract 0 (b - 1) else str := str.push 46
          | none => pure ()
      else
        str := str.push c
        q := q + 1
    if str.size > 0 then out := out.push str
    if q ≥ n then break
    p := q + 1
  return out

/-! ## get_tok_str (tccpp.c) -/

def tok_two_chars : List (UInt8 × UInt8 × Int32) :=
  [(60,61,TOK_LE), (62,61,TOK_GE), (33,61,TOK_NE), (38,38,TOK_LAND), (124,124,TOK_LOR),
   (43,43,TOK_INC), (45,45,TOK_DEC), (61,61,TOK_EQ), (60,60,TOK_SHL), (62,62,TOK_SAR),
   (43,61,TOK_A_ADD), (45,61,TOK_A_SUB), (42,61,TOK_A_MUL), (47,61,TOK_A_DIV), (37,61,TOK_A_MOD),
   (38,61,TOK_A_AND), (94,61,TOK_A_XOR), (124,61,TOK_A_OR), (45,62,TOK_ARROW), (46,46,TOK_TWODOTS),
   (35,35,TOK_TWOSHARPS)]

/-- tccpp.c's add_char -/
def add_char (cs : ByteArray) (c : Int32) : ByteArray := Id.run do
  let mut cs := cs
  if c == 39 || c == 34 || c == 92 then cs := cs.push 92
  if c ≥ 32 && c ≤ 126 then cs := cs.push c.toUInt8
  else
    cs := cs.push 92
    if c == 10 then cs := cs.push 110
    else
      cs := cs.push ((48 : Int32) + ((c >>> 6) &&& 7)).toUInt8
      cs := cs.push ((48 : Int32) + ((c >>> 3) &&& 7)).toUInt8
      cs := cs.push ((48 : Int32) + (c &&& 7)).toUInt8
  return cs

/-- `get_tok_str(v, cv)` (`none` for tcc's NULL) -/
def get_tok_str_st (s : TccSt) (v : Int32) (cv : Option CValue) : Option ByteArray :=
  let cv := cv.getD {}
  if v == TOK_CINT || v == TOK_CUINT || v == TOK_CLONG || v == TOK_CULONG || v == TOK_CLLONG || v == TOK_CULLONG then
    some (cfmt "%llu" [.u cv.i.toNat])
  else if v == TOK_CCHAR || v == TOK_LCHAR || v == TOK_U16CHAR || v == TOK_U32CHAR then
    let b := if v == TOK_CCHAR then ByteArray.empty else
      ByteArray.empty.push (if v == TOK_U16CHAR then 117 else if v == TOK_U32CHAR then 85 else 76)
    -- add_char(cv->i) with the int value
    some ((add_char (b.push 39) cv.i.toUInt32.toInt32).push 39)
  else if v == TOK_PPNUM || v == TOK_PPSTR then
    some (Bytes.cstrAt cv.sdata 0)
  else if TOK_IS_STR v then
    Id.run do
      let mut b := if v == TOK_STR then ByteArray.empty else
        ByteArray.empty.push (if v == TOK_U16STR then 117 else if v == TOK_U32STR then 85 else 76)
      b := b.push 34
      if v == TOK_STR then
        let len := cv.strSize.toInt - 1
        for i in [0:len.toNat] do b := add_char b (cv.sdata.get! i).toUInt32.toInt32
      else
        let width := (TOK_STR_SIZE v).toNat'
        let len := cv.strSize.toInt / (Int.ofNat width) - 1
        for i in [0:len.toNat] do
          let c := if width == 2 then (Bytes.r16 cv.sdata (2*i)).toUInt32 else Bytes.r32 cv.sdata (4*i)
          b := add_char b c.toInt32
      return some (b.push 34)
  else if v == TOK_DG_LBRACK then some "<:".toUTF8
  else if v == TOK_DG_RBRACK then some ":>".toUTF8
  else if v == TOK_DG_LBRACE then some "<%".toUTF8
  else if v == TOK_DG_RBRACE then some "%>".toUTF8
  else if v == TOK_DG_SHARP then some "%:".toUTF8
  else if v == TOK_DG_PASTE then some "%:%:".toUTF8
  else if v == TOK_CFLOAT then some "<float>".toUTF8
  else if v == TOK_CDOUBLE then some "<double>".toUTF8
  else if v == TOK_CLDOUBLE then some "<long double>".toUTF8
  else if v == TOK_LINENUM || v == TOK_PPLINE then some "<linenumber>".toUTF8
  else if v == TOK_LT then some "<".toUTF8
  else if v == TOK_GT then some ">".toUTF8
  else if v == TOK_DOTS then some "...".toUTF8
  else if v == TOK_A_SHL then some "<<=".toUTF8
  else if v == TOK_A_SAR then some ">>=".toUTF8
  else if v == TOK_EOF then some "<eof>".toUTF8
  else if v == 0 then some "<no name>".toUTF8
  else
    let v := v &&& ~~~(SYM_FIELD ||| SYM_STRUCT)
    if v < TOK_IDENT then
      match tok_two_chars.find? (fun (_, _, t) => t == v) with
      | some (a, b, _) => some ((ByteArray.empty.push a).push b)
      | none =>
        if v ≥ 127 || (v < 32 && !isSpace v && v != 10) then
          some (cfmt "<\\x%02x>" [.i v.toInt])
        else some (ByteArray.empty.push v.toUInt8)
    else if v < s.tok_ident then
      some (s.table_ident[(v - TOK_IDENT).toNat']!).str
    else if v ≥ SYM_FIRST_ANOM then
      some (cfmt "L.%u" [.u (v - SYM_FIRST_ANOM).toNat'])
    else none

def get_tok_str (v : Int32) (cv : Option CValue) : M ByteArray := do
  return (get_tok_str_st (← get) v cv).getD ByteArray.empty

/-- Project an unconverted tcc token, without reading phase-7 values. -/
def ppObservation : M PPObservation := do
  let s ← get
  -- Observer metadata is carried through macro token strings in a PPSTR
  -- payload beginning with NUL, impossible for a physical literal spelling.
  -- It is never enabled on ordinary compilation's phase-7 path.
  if s.tok == TOK_PPSTR && s.tokc.strSize ≥ 11 && s.tokc.sdata.get! 0 == 0 then
    let kind := match (s.tokc.sdata.get! 1).toNat with
      | 73 => PPClass.ident | 78 => .number | 67 => .charLit | 83 => .stringLit
      | 80 => .punct | 91 => .pragmaBegin | 93 => .pragmaEnd | _ => .other
    return ⟨kind, s.tokc.sdata.extract 10 (s.tokc.strSize - 1).toNat', (Bytes.r64 s.tokc.sdata 2).toNat⟩
  let spell ← get_tok_str s.tok (some s.tokc)
  let kind := if s.tok == TOK_PPNUM then PPClass.number
    else if TOK_IS_STR s.tok then PPClass.stringLit
    else if s.tok == TOK_PPSTR then
      if spell.get! 0 == 39 ||
          ((spell.get! 0 == 76 || spell.get! 0 == 117 || spell.get! 0 == 85) && spell.get! 1 == 39)
        then PPClass.charLit else PPClass.stringLit
    else if s.tok ≥ TOK_IDENT then PPClass.ident
    else if ["[", "]", "(", ")", "{", "}", ".", "->", "++", "--", "&", "*", "+", "-",
      "~", "!", "/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "^", "|", "&&",
      "||", "?", ":", ";", "...", "=", "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=",
      "^=", "|=", ",", "#", "##", "<:", ":>", "<%", "%>", "%:", "%:%:"].any
        (fun p => p.toUTF8 == spell) then PPClass.punct else PPClass.other
  return ⟨kind, spell, 0⟩

/-- In-band observation metadata, distinct from every lexed literal. This
lets eager macro expansion carry an event to its position in the stream. -/
def PPObservation.payload (o : PPObservation) (event : Nat) : CValue := Id.run do
  let k : UInt8 := match o.kind with
    | .ident => 73 | .number => 78 | .charLit => 67 | .stringLit => 83
    | .punct => 80 | .other => 79 | .pragmaBegin => 91 | .pragmaEnd => 93
  let hdr := Bytes.w64 ((ByteArray.empty.push 0).push k ++ Bytes.zeros 8) 2 event.toUInt64
  let b := (hdr ++ o.spell).push 0
  return (default : CValue).setStr b (Int32.ofNat b.size)

/-! ## messages (libtcc.c error1) -/

inductive ErrMode where
  | warn | noabort | error
  deriving BEq, Inhabited

/-- the warn_* field tcc_warning_c selected (offset from warn_none) -/
def warnField (s : TccSt) (n : Int32) : Int32 :=
  match n.toNat' with
  | 0 => s.warn_none | 1 => s.warn_all | 2 => s.warn_error | 3 => s.warn_write_strings
  | 4 => s.warn_unsupported | 5 => s.warn_implicit_function_declaration
  | 6 => s.warn_discarded_qualifiers | _ => 0

def WARN_ON : Int32 := 1
def WARN_ERR : Int32 := 2
def WARN_NOE : Int32 := 4

/-- `error1(mode, fmt, ...)` with the message already formatted; `line`
is the explicit `%i:` line or 0. Returns whether tcc longjmps. -/
def error1 (R : Rec) (mode0 : ErrMode) (msg : ByteArray) (line0 : Int32 := 0) : M Bool := do
  let s ← get
  let mut mode := mode0
  if mode == .warn then
    if s.warn_error != 0 then mode := .error
    if s.warn_num != 0 then
      let wopt := warnField s s.warn_num
      modify fun s => { s with warn_num := 0 }
      if wopt &&& WARN_ON == 0 then return false
      if wopt &&& WARN_ERR != 0 then mode := .error
      if wopt &&& WARN_NOE != 0 then mode := .warn
    if s.warn_none != 0 then return false
  let s ← get
  let mut cs := ByteArray.empty
  let mut line := line0
  let mut f : Option BufferedFile := none
  if s.error_set_jmp_enabled != 0 then
    f := s.file.find? (fun b => b.filename.size == 0 || b.filename.get! 0 != 58)
  match f with
  | some fb =>
    for pid in s.include_stack do
      match s.file.find? (·.id == pid) with
      | some pf => cs := cs ++ cfmt "In file included from %s:%d:\n" [.s pf.filename, .i (pf.line_num - 1).toInt]
      | none => pure ()
    if line == 0 then
      line := fb.line_num - (if (s.tok_flags &&& TOK_FLAG_BOL != 0) && s.macro_ptr.isNone then 1 else 0)
    cs := cs ++ cfmt "%s:%d: " [.s fb.filename, .i line.toInt]
  | none =>
    match s.current_filename with
    | some cf => cs := cs ++ cfmt "%s: " [.s cf]
    | none => cs := cs ++ "tcc: ".toUTF8
  cs := cs ++ (if mode == .warn then "warning: ".toUTF8 else "error: ".toUTF8)
  if s.pp_expr > 1 then
    cs := cs ++ (← R.pp_error)
  else
    cs := cs ++ msg
  putConsole (cs.push 10)
  if mode != .warn then modify fun s => { s with nb_errors := s.nb_errors + 1 }
  return mode == .error && (← get).error_set_jmp_enabled != 0

/-- `tcc_error(fmt, ...)`: never returns -/
def tcc_error (R : Rec) (msg : ByteArray) : M α := do
  let jmp ← error1 R .error msg
  if jmp then throw .longjmp else throw (.exit 1)

def tcc_error_s (R : Rec) (fmt : String) (args : List CArg := []) : M α :=
  tcc_error R (cfmt fmt args)

/-- `tcc_error_noabort(fmt, ...)`: returns -1 -/
def tcc_error_noabort (R : Rec) (msg : ByteArray) : M Int32 := do
  let jmp ← error1 R .noabort msg
  if jmp then throw .longjmp
  return -1

def tcc_warning (R : Rec) (msg : ByteArray) : M Unit := do
  let jmp ← error1 R .warn msg
  if jmp then throw .longjmp

/-- `tcc_warning_c(warn_x)(...)`: `n` selects the warn_ field -/
def tcc_warning_c (R : Rec) (n : Int32) (msg : ByteArray) : M Unit := do
  modify fun s => { s with warn_num := n }
  tcc_warning R msg

def WARNC_all : Int32 := 1
def WARNC_error : Int32 := 2
def WARNC_write_strings : Int32 := 3
def WARNC_unsupported : Int32 := 4
def WARNC_implicit_function_declaration : Int32 := 5
def WARNC_discarded_qualifiers : Int32 := 6

/-- libtcc.c's FlagDef, with field access replacing C byte offsets. -/
structure FlagDef where
  name : String
  read : TccSt → Int32
  write : TccSt → Int32 → TccSt
  all : Bool := false
  invert : Bool := false

def options_W : Array FlagDef := #[
  { name := "all", read := (·.warn_all), write := fun s v => { s with warn_all := v }, all := true },
  { name := "error", read := (·.warn_error), write := fun s v => { s with warn_error := v } },
  { name := "write-strings", read := (·.warn_write_strings), write := fun s v => { s with warn_write_strings := v } },
  { name := "unsupported", read := (·.warn_unsupported), write := fun s v => { s with warn_unsupported := v } },
  { name := "implicit-function-declaration", read := (·.warn_implicit_function_declaration),
    write := fun s v => { s with warn_implicit_function_declaration := v }, all := true },
  { name := "discarded-qualifiers", read := (·.warn_discarded_qualifiers),
    write := fun s v => { s with warn_discarded_qualifiers := v }, all := true } ]

def options_f : Array FlagDef := #[
  { name := "unsigned-char", read := (·.char_is_unsigned), write := fun s v => { s with char_is_unsigned := v } },
  { name := "signed-char", read := (·.char_is_unsigned), write := fun s v => { s with char_is_unsigned := v }, invert := true },
  { name := "common", read := (·.nocommon), write := fun s v => { s with nocommon := v }, invert := true },
  { name := "leading-underscore", read := (·.leading_underscore), write := fun s v => { s with leading_underscore := v } },
  { name := "ms-extensions", read := (·.ms_extensions), write := fun s v => { s with ms_extensions := v } },
  { name := "dollars-in-identifiers", read := (·.dollars_in_identifiers), write := fun s v => { s with dollars_in_identifiers := v } },
  { name := "test-coverage", read := (·.test_coverage), write := fun s v => { s with test_coverage := v } },
  { name := "reverse-funcargs", read := (·.reverse_funcargs), write := fun s v => { s with reverse_funcargs := v } },
  { name := "gnu89-inline", read := (·.gnu89_inline), write := fun s v => { s with gnu89_inline := v } },
  { name := "asynchronous-unwind-tables", read := (·.unwind_tables), write := fun s v => { s with unwind_tables := v } } ]

def options_m : Array FlagDef := #[
  { name := "ms-bitfields", read := (·.ms_bitfields), write := fun s v => { s with ms_bitfields := v } } ]

/-- `set_flag`: the `no-` and `error=` forms, warning masks,
`-Wall` propagation and FD_INVERT. -/
def set_flag (flags : Array FlagDef) (name0 : ByteArray) : M Int32 := do
  let mut name := name0
  let mut value : Int32 := 1
  let mut mask : Int32 := 0
  if name.extract 0 3 == "no-".toUTF8 then
    name := name.extract 3 name.size
    value := 0
  if (flags[0]?.map (·.all)).getD false && name.extract 0 6 == "error=".toUTF8 then
    name := name.extract 6 name.size
    value := if value != 0 then WARN_ON ||| WARN_ERR else WARN_NOE
    mask := WARN_ON
  let mut ret : Int32 := -1
  for flag in flags do
    if ret != 0 then
      if name != flag.name.toUTF8 then continue
    else if !flag.all then continue
    modify fun s => flag.write s ((flag.read s &&& mask) ||| (value ^^^ b2i flag.invert))
    if ret != 0 then
      ret := 0
      if name != "all".toUTF8 then break
  return ret

/-- `expect(msg)` -/
def expect (R : Rec) (msg : String) : M α := tcc_error R (cfmt "%s expected" [.s msg.toUTF8])

/-! ## the input-file layer (libtcc.c) -/

/-- room in front of the buffer for tcc's ungets (BufferedFile.unget[4]) -/
def UNGET_ROOM : Nat := 4

/-- `tcc_open_bf(s1, filename, initlen)`: a new current file whose buffer
holds `init` (or is empty, to be read in 8 KiB blocks, when `init` is none) -/
def tcc_open_bf (filename : ByteArray) (init : Option ByteArray) : M Unit := do
  let id ← freshId
  let s ← get
  let initlen := (init.map (·.size)).getD 0
  let buflen := if init.isSome then initlen else IO_BUF_SIZE
  let mut buf := Bytes.zeros (UNGET_ROOM + buflen + 1)
  match init with
  | some b => buf := Bytes.copy buf UNGET_ROOM b 0 b.size
  | none => pure ()
  buf := buf.set! (UNGET_ROOM + initlen) CH_EOB.toUInt8
  let fname := pstrcpy 1024 filename
  let bf : BufferedFile := {
    buf := buf, buf_ptr := UNGET_ROOM, buf_end := UNGET_ROOM + initlen, bufStart := UNGET_ROOM,
    fd := -1, line_num := 1, ifdef_stack_ptr := s.ifdef_stack_ptr,
    prev_tok_flags := s.tok_flags, filename := fname, true_filename := fname, id := id }
  set { s with file := bf :: s.file, tok_flags := TOK_FLAG_BOL ||| TOK_FLAG_BOF }

/-- `tcc_close()` -/
def tcc_close : M Unit := do
  let s ← get
  match s.file with
  | bf :: rest =>
    if bf.fd > 0 then
      Host.close bf.fd
      modify fun s => { s with total_lines := s.total_lines + bf.line_num - 1 }
    modify fun s => { s with file := rest, tok_flags := bf.prev_tok_flags }
  | [] => pure ()

/-- `_tcc_open(s1, filename)`: the descriptor or -1 -/
def _tcc_open (filename : ByteArray) : M Int32 := do
  if filename == "-".toUTF8 then return 0
  Host.openRd filename

/-- `tcc_open(s1, filename)` -/
def tcc_open (filename : ByteArray) : M Int32 := do
  let fd ← _tcc_open filename
  if fd < 0 then return -1
  tcc_open_bf filename none
  modFile fun f => { f with fd := fd }
  return 0

end Tcc.Model
