import Tcc.Model.Syms
import Tcc.Model.SoftFloat

/-!
# TccModel: tccpp.c, part 2 — skipping, defines, includes, #if,
directives, literals and `next_nomacro`
-/

namespace Tcc.Model
open C

/-! ## preprocess_skip -/

/-- `preprocess_skip()` -/
def preprocess_skip (R : Rec) : M Unit := do
  let fuel ← fuelM
  let mut p ← bufPtr
  let mut a : Int32 := 0
  let mut start_of_line := true
  let mut in_warn_or_error := false
  for _ in [0:fuel] do
    let c ← rd p
    if c == 32 || c == 9 || c == 12 || c == 11 || c == 13 then
      p := p + 1
      continue
    if c == 10 then
      modFile fun f => { f with line_num := f.line_num + 1 }
      p := p + 1
      start_of_line := true; in_warn_or_error := false
      continue
    if c == 92 then
      let (c', p') ← handle_bs R p
      p := p'
      if c' == CH_EOF then expect R "#endif"
      if c' == 92 then p := p + 1
      continue
    if (c == 34 || c == 39) && !in_warn_or_error then
      modify fun s => { s with tok_flags := s.tok_flags &&& ~~~TOK_FLAG_BOL }
      let (p', _) ← parse_pp_string R p c none
      p := p'
    else if c == 47 && !in_warn_or_error then
      p := p + 1
      let (c', p') ← handle_bs R p
      p := p'
      if c' == 42 then p ← parse_comment R p
      else if c' == 47 then p ← parse_line_comment R p
      continue
    else if c == 35 then
      p := p + 1
      if start_of_line then
        setBufPtr p
        R.next_nomacro
        p ← bufPtr
        let tok ← getTok
        if a == 0 && (tok == Tok.TOK_ELSE || tok == Tok.TOK_ELIF || tok == Tok.TOK_ENDIF) then
          setBufPtr p
          return
        if tok == Tok.TOK_IF || tok == Tok.TOK_IFDEF || tok == Tok.TOK_IFNDEF then a := a + 1
        else if tok == Tok.TOK_ENDIF then a := a - 1
        else if tok == Tok.TOK_ERROR || tok == Tok.TOK_WARNING then in_warn_or_error := true
        else if tok == TOK_LINEFEED then
          start_of_line := true; in_warn_or_error := false
          continue
        else if (← get).parse_flags &&& PARSE_FLAG_ASM_FILE != 0 then
          p ← parse_line_comment R (p - 1)
      else if (← get).parse_flags &&& PARSE_FLAG_ASM_FILE != 0 then
        p ← parse_line_comment R (p - 1)
    else
      p := p + 1
    start_of_line := false
  throw (.fuel "preprocess_skip")

/-! ## defines -/

/-- `macro_is_equal(a, b)` (clobbers tokcstr, as tcc does) -/
def macro_is_equal (a0 b0 : Option TokArr) : M Bool := do
  match a0, b0 with
  | some a, some b =>
    let fuel ← fuelM
    let mut pa : TokPtr := { arr := a }
    let mut pb : TokPtr := { arr := b }
    for _ in [0:fuel] do
      if pa.get == 0 || pb.get == 0 then break
      let (t, cv, pa') := TOK_GET pa {}
      pa := pa'
      let sa ← get_tok_str t (some cv)
      modify fun s => { s with tokcstr := sa.push 0 }
      let (t, cv, pb') := TOK_GET pb {}
      pb := pb'
      let sb ← get_tok_str t (some cv)
      if sa != sb then return false
    return !(pa.get != 0 || pb.get != 0)
  | _, _ => return true

/-- `define_push(v, macro_type, str, first_arg)` -/
def define_push (R : Rec) (v macro_type : Int32) (str : Option TokArr) (first_arg : SymId) : M Unit := do
  let o ← define_find v
  let top := (← get).define_stack
  let s ← sym_push2 top v macro_type 0
  modify fun st => { st with define_stack := s }
  modSym s fun x => { x with d := str, next := first_arg }
  modTokSym v fun ts => { ts with sym_define := s }
  if o != 0 then
    let od := (← getSym o).d
    if !(← macro_is_equal od str) then
      tcc_warning R (cfmt "%s redefined" [.s (← get_tok_str v none)])

/-- `define_undef(s)` -/
def define_undef (s : SymId) : M Unit := do
  let v := (← getSym s).v
  let st ← get
  if v ≥ TOK_IDENT && v < st.tok_ident then
    modTokSym v fun ts => { ts with sym_define := 0 }

/-- `free_defines(b)` -/
def free_defines (b : SymId) : M Unit := do
  let fuel ← fuelM
  for _ in [0:fuel] do
    let ds := (← get).define_stack
    if ds == b then return
    let top ← getSym ds
    modify fun s => { s with define_stack := top.prev }
    define_undef ds
    sym_free ds

/-- `skip_to_eol(warn)` -/
def skip_to_eol (R : Rec) (warn : Bool) : M Unit := do
  if (← getTok) == TOK_LINEFEED then return
  if warn then tcc_warning R "extra tokens after directive".toUTF8
  let fuel ← fuelM
  for _ in [0:fuel] do
    if (← get).macro_stack.isEmpty then break
    end_macro
  let p ← bufPtr
  let p ← parse_line_comment R (p - 1)
  setBufPtr p
  R.next_nomacro

/-! ## includes -/

/-- `search_cached_include(s1, filename, add)`: the index in
cached_includes, or none -/
def search_cached_include (filename : ByteArray) (add : Bool) : M (Option Nat) := do
  let basename := tcc_basename filename
  let mut h : UInt32 := 1
  for c in basename.data do
    h := h + (h <<< 5) + (h >>> 27) + c.toUInt32
  let hh := (h &&& 31).toNat
  let s ← get
  let mut i := s.cached_includes_hash[hh]!
  let fuel ← fuelM
  for _ in [0:fuel] do
    if i == 0 then break
    let e := s.cached_includes[(i - 1).toNat']!
    if filename == e.filename then return some (i - 1).toNat'
    -- normalized_PATHCMP: realpath is the identity in the image (main.c)
    if e.once != 0 && basename == tcc_basename e.filename && filename == e.filename then
      return some (i - 1).toNat'
    i := e.hash_next
  if !add then return none
  let e : CachedInclude := { filename := filename, hash_next := s.cached_includes_hash[hh]! }
  let n := s.cached_includes.size
  set { s with cached_includes := s.cached_includes.push e,
               cached_includes_hash := s.cached_includes_hash.set! hh (Int32.ofNat (n + 1)) }
  return some n

/-- `parse_include(s1, do_next, test)` -/
def parse_include (R : Rec) (do_next : Bool) (test : Bool) : M Int32 := do
  let c0 ← skip_spaces R
  let mut name := ByteArray.empty
  let mut c := c0
  if c0 == 60 || c0 == 34 then
    let p ← bufPtr
    let (p', str) ← parse_pp_string R p (if c0 == 60 then 62 else c0) (some ByteArray.empty)
    let str := str.getD ByteArray.empty
    modify fun s => { s with tokcstr := str }
    setBufPtr p'
    name := pstrcpy 1024 str     -- pstrncpy(name, sizeof name, tokcstr.data, i)
    -- pstrncpy copies i bytes then NUL; the name ends at the first NUL
    name := Bytes.cstrAt name 0
    R.next_nomacro
  else
    modify fun s => { s with parse_flags := PARSE_FLAG_PREPROCESS ||| PARSE_FLAG_LINEFEED |||
                                             (s.parse_flags &&& PARSE_FLAG_ASM_FILE) }
    let fuel ← fuelM
    for _ in [0:fuel] do
      R.next
      let i : Int := (name.size : Int) - 1
      if i > 0 && ((name.get! 0 == 34 && name.get! i.toNat == 34) ||
                   (name.get! 0 == 60 && name.get! i.toNat == 62)) then break
      if (← getTok) == TOK_LINEFEED then
        tcc_error_s R "'#include' expects \"FILENAME\" or <FILENAME>"
      let st ← get
      name := pstrcat name 1024 (← get_tok_str st.tok (some st.tokc))
    c := (name.get! 0).toUInt32.toInt32
    name := name.extract 1 (name.size - 1)
  if !test then skip_to_eol R true
  let cf ← curFile
  let mut i : Int32 := if do_next then cf.include_next_index else -1
  let fuel ← fuelM
  for _ in [0:fuel] do
    i := i + 1
    let mut buf := ByteArray.empty
    if i == 0 then
      if !(name.size > 0 && name.get! 0 == 47) then continue
      buf := ByteArray.empty
    else if i == 1 then
      if c != 34 then continue
      let p := (← curFile).true_filename
      buf := pstrcpy 1024 (p.extract 0 (basenameOff p))
    else
      let st ← get
      let j := (i - 2).toNat'
      let k : Int := (j : Int) - st.include_paths.size
      let p ←
        if k < 0 then pure st.include_paths[j]!
        else if k < st.sysinclude_paths.size then pure st.sysinclude_paths[k.toNat]!
        else if test then return 0
        else tcc_error R (cfmt "include file '%s' not found" [.s name])
      buf := pstrcpy 1024 p
      buf := pstrcat buf 1024 "/".toUTF8
    buf := pstrcat buf 1024 name
    match ← search_cached_include buf false with
    | some ei =>
      let e := (← get).cached_includes[ei]!
      if (← define_find e.ifndef_macro) != 0 || e.once != 0 then
        return 1
    | none => pure ()
    if (← tcc_open buf) ≥ 0 then break
  if test then
    tcc_close
  else
    let st ← get
    if st.include_stack.size ≥ INCLUDE_STACK_SIZE then
      tcc_error_s R "#include recursion too deep"
    -- push previous file on stack
    let prevId := match st.file with | _ :: p :: _ => p.id | _ => 0
    modify fun s => { s with include_stack := s.include_stack.push prevId }
    modFile fun f => { f with include_next_index := i }
  return 1

/-! ## #if -/

/-- `expr_preprocess(s1)` -/
def expr_preprocess (R : Rec) : M Bool := do
  let t0 ← getTok
  let mut str : TokenString := {}
  modify fun s => { s with pp_expr := 1 }
  let fuel ← fuelM
  let mut t : Int32 := 0
  for _ in [0:fuel] do
    R.next
    t ← getTok
    let tok := t
    if tok < TOK_IDENT then
      if tok == TOK_LINEFEED || tok == TOK_EOF then break
      if tok ≥ TOK_STR && tok ≤ TOK_CLDOUBLE then
        tcc_error_s R "invalid constant in preprocessor expression"
    else
      let mut cval : Int32 := 0
      if tok == Tok.TOK_DEFINED then
        modify fun s => { s with parse_flags := s.parse_flags &&& ~~~PARSE_FLAG_PREPROCESS }
        R.next
        t ← getTok
        if t == 40 then R.next
        modify fun s => { s with parse_flags := s.parse_flags ||| PARSE_FLAG_PREPROCESS }
        let tk ← getTok
        if tk < TOK_IDENT then expect R "identifier after 'defined'"
        if (← define_find tk) != 0 || tk == Tok.TOK___HAS_INCLUDE || tk == Tok.TOK___HAS_INCLUDE_NEXT then
          cval := 1
        if t == 40 then
          R.next
          if (← getTok) != 41 then expect R "')'"
      else if tok == Tok.TOK___HAS_INCLUDE || tok == Tok.TOK___HAS_INCLUDE_NEXT then
        t := tok
        R.next
        if (← getTok) != 40 then expect R "'('"
        cval ← parse_include R (t - Tok.TOK___HAS_INCLUDE != 0) true
        if (← getTok) != 41 then expect R "')'"
      else
        cval := 0
      -- c_number:
      modify fun s => { s with tok := TOK_CLLONG, tokc := s.tokc.setI cval.toInt64.toUInt64 }
    str ← tok_str_add_tok str
  if str.len == 0 then
    tcc_error R (cfmt "#%s with no expression" [.s (← get_tok_str t0 none)])
  str := tok_str_add str TOK_EOF
  modify fun s => { s with pp_expr := t0 }
  let tsave ← getTok
  let _ ← begin_macro .heap str.str 0 1
  R.next
  let gm ← gmark
  let c ← R.expr_const
  gdrop gm
  if (← getTok) != TOK_EOF then tcc_error_s R "..."
  modify fun s => { s with pp_expr := 0 }
  end_macro
  setTok tsave
  return c != 0

/-! ## #define -/

/-- `set_idnum(c, val)`: the previous value -/
def set_idnum (c : Int32) (v : UInt8) : M UInt8 := do
  let i := (c + 1).toNat'
  modifyGet fun s => (s.isidnum_table[i]!, { s with isidnum_table := s.isidnum_table.set! i v })

/-- `parse_define()` -/
def parse_define (R : Rec) : M Unit := do
  let v ← getTok
  if v < TOK_IDENT || v == Tok.TOK_DEFINED then
    let st ← get
    tcc_error R (cfmt "invalid macro name '%s'" [.s (← get_tok_str st.tok (some st.tokc))])
  let saved_parse_flags := (← get).parse_flags
  let mut first : SymId := 0
  let mut t := MACRO_OBJ
  modify fun s => { s with parse_flags := (s.parse_flags &&& ~~~PARSE_FLAG_ASM_FILE) ||| PARSE_FLAG_SPACES }
  R.next_nomacro
  modify fun s => { s with parse_flags := s.parse_flags &&& ~~~PARSE_FLAG_SPACES }
  let fuel ← fuelM
  if (← getTok) == 40 then
    let dotid ← set_idnum 46 0
    R.next_nomacro
    let mut last : SymId := 0     -- the last arg sym (ps = &last->next)
    if (← getTok) != 41 then
      for _ in [0:fuel] do
        let mut varg ← getTok
        R.next_nomacro
        let mut is_vaargs : Int32 := 0
        if varg == TOK_DOTS then
          varg := Tok.TOK___VA_ARGS__
          is_vaargs := 1
        else if (← getTok) == TOK_DOTS && (← get).gnu_ext != 0 then
          is_vaargs := 1
          R.next_nomacro
        if varg < TOK_IDENT then tcc_error_s R "bad macro parameter list"
        let s ← sym_push2 (← get).define_stack (varg ||| SYM_FIELD) is_vaargs 0
        modify fun st => { st with define_stack := s }
        if last == 0 then first := s else modSym last fun x => { x with next := s }
        last := s
        let tk ← getTok
        if tk == 41 then break
        if tk != 44 || is_vaargs != 0 then tcc_error_s R "bad macro parameter list"
        R.next_nomacro
    modify fun s => { s with parse_flags := s.parse_flags ||| PARSE_FLAG_SPACES }
    R.next_nomacro
    t := MACRO_FUNC
    let _ ← set_idnum 46 dotid
  modify fun s => { s with parse_flags := s.parse_flags ||| PARSE_FLAG_ACCEPT_STRAYS ||| PARSE_FLAG_SPACES ||| PARSE_FLAG_LINEFEED }
  let mut str : TokenString := {}
  let mut t0 : Int32 := 0
  for _ in [0:fuel] do
    let tk ← getTok
    if tk == TOK_LINEFEED || tk == TOK_EOF then break
    if isSpace tk then
      str := { str with need_spc := str.need_spc ||| 1 }
    else
      let mut tk := tk
      if TOK_TWOSHARPS == tk then
        if t0 == 0 then tcc_error_s R "'##' cannot appear at either end of macro"
        tk := TOK_PPJOIN
        setTok tk
        t := t ||| MACRO_JOIN
      str := tok_str_add2_spc str tk (← get).tokc
      t0 := tk
    R.next_nomacro
  modify fun s => { s with parse_flags := saved_parse_flags }
  str := tok_str_add str 0
  if t0 == TOK_PPJOIN then tcc_error_s R "'##' cannot appear at either end of macro"
  define_push R v t (some (str.str.extract 0 str.len.toNat')) first

/-! ## #pragma, #line -/

/-- `tccpp_putfile(filename)` -/
def tccpp_putfile (filename : ByteArray) : M Unit := do
  let f ← curFile
  let mut buf := ByteArray.empty
  if !(filename.size > 0 && filename.get! 0 == 47) then
    buf := pstrcpy 1024 f.true_filename
    buf := buf.extract 0 (basenameOff buf)
  buf := pstrcat buf 1024 filename
  if f.filename == buf then return
  modFile fun f => { f with filename := pstrcpy 1024 buf }

/-- `unget_tok(last_tok)` -/
def unget_tok (last_tok : Int32) : M Unit := do
  let s ← get
  let useStatic := s.unget_buf.len == 0
  let mut str : TokenString := if useStatic then s.unget_buf else {}
  if s.tok != TOK_EOF then str := tok_str_add2 str s.tok s.tokc
  str := tok_str_add str 0
  if useStatic then
    modify fun s => { s with unget_buf := str }
    let _ ← begin_macro .ungetBuf str.str 0 0
  else
    let _ ← begin_macro .heap str.str 0 1
  setTok last_tok

/-- `pragma_parse(s1)`: whether the pragma was handled -/
def pragma_parse (R : Rec) : M Bool := do
  R.next_nomacro
  let tok ← getTok
  let err : M Bool := tcc_error_s R "malformed #pragma directive"
  if tok == Tok.TOK_push_macro || tok == Tok.TOK_pop_macro then
    let t := tok
    R.next
    if (← getTok) != 40 then return ← err
    R.next
    if (← getTok) != TOK_STR then return ← err
    let tc := (← get).tokc
    let v := (← tok_alloc R (tc.sdata.extract 0 (tc.strSize - 1).toNat')).tok
    R.next
    if (← getTok) != 41 then return ← err
    let mut s : SymId := 0
    if t == Tok.TOK_push_macro then
      let fuel ← fuelM
      for _ in [0:fuel] do
        s ← define_find v
        if s != 0 then break
        define_push R v 0 none 0
      let s' := s
      modSym s' fun x => { x with type := { x.type with ref := s' } }
    else
      let fuel ← fuelM
      s := (← get).define_stack
      for _ in [0:fuel] do
        if s == 0 then break
        let x ← getSym s
        if x.v == v && x.type.ref == s then
          modSym s fun x => { x with type := { x.type with ref := 0 } }
          break
        s := x.prev
    if s != 0 then
      let hasD := (← getSym s).d.isSome
      modTokSym v fun ts => { ts with sym_define := if hasD then s else 0 }
    else
      tcc_warning R "unbalanced #pragma pop_macro".toUTF8
    modify fun st => { st with pp_debug_tok := t, pp_debug_symv := v }
  else if tok == Tok.TOK_once then
    match ← search_cached_include (← curFile).true_filename true with
    | some i => modify fun s => { s with cached_includes := s.cached_includes.modify i fun e => { e with once := 1 } }
    | none => pure ()
  else if (← get).output_type == TCC_OUTPUT_PREPROCESS then
    unget_tok 32
    unget_tok Tok.TOK_PRAGMA
    unget_tok 35
    unget_tok TOK_LINEFEED
    return true
  else if tok == Tok.TOK_pack then
    R.next
    -- skip('(')
    if (← getTok) != 40 then
      let st ← get
      tcc_error R (cfmt "'%s' expected (got '%s')" [.s "(".toUTF8, .s (← get_tok_str st.tok (some st.tokc))])
    R.next
    if (← getTok) == Tok.TOK_ASM_pop then
      R.next
      if (← get).pack_stack_ptr == 0 then tcc_error_s R "out of pack stack"
      modify fun s => { s with pack_stack_ptr := s.pack_stack_ptr - 1 }
    else
      let mut v : Int32 := 0
      let mut doSet := true
      if (← getTok) != 41 then
        let mut skipNum := false
        if (← getTok) == Tok.TOK_ASM_push then
          R.next
          let st ← get
          if st.pack_stack_ptr ≥ PACK_STACK_SIZE - 1 then tcc_error_s R "out of pack stack"
          v := st.pack_stack[st.pack_stack_ptr]!
          modify fun s => { s with pack_stack_ptr := s.pack_stack_ptr + 1 }
          if (← getTok) != 44 then skipNum := true
          else R.next
        if !skipNum then
          if (← getTok) != TOK_CINT then return ← err
          v := (← get).tokc.i.toUInt32.toInt32
          if v < 1 || v > 16 || (v &&& (v - 1)) != 0 then return ← err
          R.next
      if doSet then
        modify fun s => { s with pack_stack := s.pack_stack.set! s.pack_stack_ptr v }
    if (← getTok) != 41 then return ← err
  else if tok == Tok.TOK_comment then
    R.next
    if (← getTok) != 40 then
      let st ← get
      tcc_error R (cfmt "'%s' expected (got '%s')" [.s "(".toUTF8, .s (← get_tok_str st.tok (some st.tokc))])
    R.next
    let t ← getTok
    R.next
    if (← getTok) != 44 then
      let st ← get
      tcc_error R (cfmt "'%s' expected (got '%s')" [.s ",".toUTF8, .s (← get_tok_str st.tok (some st.tokc))])
    R.next
    if (← getTok) != TOK_STR then return ← err
    let p := Bytes.cstrAt (← get).tokc.sdata 0
    R.next
    if (← getTok) != 41 then return ← err
    if t == Tok.TOK_lib then
      modify fun s => { s with pragma_libs := s.pragma_libs.push p }
    else if t == Tok.TOK_option then
      unsupported "#pragma comment(option)"
  else
    let st ← get
    tcc_warning_c R WARNC_all (cfmt "#pragma %s ignored" [.s (← get_tok_str st.tok (some st.tokc))])
    return false
  R.next
  return true

/-- `preprocess(is_bof)` -/
def preprocess (R : Rec) (is_bof0 : Int32) : M Unit := do
  let saved_parse_flags := (← get).parse_flags
  modify fun s => { s with parse_flags := PARSE_FLAG_PREPROCESS ||| PARSE_FLAG_TOK_NUM ||| PARSE_FLAG_TOK_STR |||
                                           PARSE_FLAG_LINEFEED ||| (s.parse_flags &&& PARSE_FLAG_ASM_FILE) }
  R.next_nomacro
  let fuel ← fuelM
  let mut is_bof := is_bof0
  -- `the_end` skips the final skip_to_eol(1)
  let mut theEnd := false
  let mut ignore := false
  for _ in [0:fuel] do   -- redo:
    let tok ← getTok
    let mut c : Int32 := 0
    let mut testSkip := false
    if tok == Tok.TOK_DEFINE then
      modify fun s => { s with pp_debug_tok := tok }
      R.next_nomacro
      modify fun s => { s with pp_debug_symv := s.tok }
      parse_define R
    else if tok == Tok.TOK_UNDEF then
      modify fun s => { s with pp_debug_tok := tok }
      R.next_nomacro
      modify fun s => { s with pp_debug_symv := s.tok }
      let s ← define_find (← getTok)
      if s != 0 then define_undef s
      R.next_nomacro
    else if tok == Tok.TOK_INCLUDE || tok == Tok.TOK_INCLUDE_NEXT then
      let _ ← parse_include R (tok - Tok.TOK_INCLUDE != 0) false
      theEnd := true
    else if tok == Tok.TOK_IFNDEF || tok == Tok.TOK_IFDEF then
      c := if tok == Tok.TOK_IFNDEF then 1 else 0
      R.next_nomacro
      let tk ← getTok
      if tk < TOK_IDENT then
        tcc_error R (cfmt "invalid argument for '#if%sdef'" [.s (if c != 0 then "n" else "").toUTF8])
      if is_bof != 0 then
        if c != 0 then modFile fun f => { f with ifndef_macro := tk }
      if (← define_find tk) != 0 || tk == Tok.TOK___HAS_INCLUDE || tk == Tok.TOK___HAS_INCLUDE_NEXT then
        c := c ^^^ 1
      R.next_nomacro
      -- do_if:
      let st ← get
      if st.ifdef_stack_ptr ≥ IFDEF_STACK_SIZE then tcc_error_s R "memory full (ifdef)"
      modify fun s => { s with ifdef_stack := s.ifdef_stack.set! s.ifdef_stack_ptr c, ifdef_stack_ptr := s.ifdef_stack_ptr + 1 }
      testSkip := true
    else if tok == Tok.TOK_IF then
      c := if ← expr_preprocess R then 1 else 0
      let st ← get
      if st.ifdef_stack_ptr ≥ IFDEF_STACK_SIZE then tcc_error_s R "memory full (ifdef)"
      modify fun s => { s with ifdef_stack := s.ifdef_stack.set! s.ifdef_stack_ptr c, ifdef_stack_ptr := s.ifdef_stack_ptr + 1 }
      testSkip := true
    else if tok == Tok.TOK_ELSE then
      R.next_nomacro
      let st ← get
      if st.ifdef_stack_ptr == 0 then tcc_error_s R "#else without matching #if"
      let top := st.ifdef_stack[st.ifdef_stack_ptr - 1]!
      if top &&& 2 != 0 then tcc_error_s R "#else after #else"
      c := top ^^^ 3
      modify fun s => { s with ifdef_stack := s.ifdef_stack.set! (s.ifdef_stack_ptr - 1) c }
      -- test_else:
      if (← get).ifdef_stack_ptr == (← curFile).ifdef_stack_ptr + 1 then
        modFile fun f => { f with ifndef_macro := 0 }
      testSkip := true
    else if tok == Tok.TOK_ELIF then
      let st ← get
      if st.ifdef_stack_ptr == 0 then tcc_error_s R "#elif without matching #if"
      c := st.ifdef_stack[st.ifdef_stack_ptr - 1]!
      if c > 1 then tcc_error_s R "#elif after #else"
      if c == 1 then
        skip_to_eol R false
        c := 0
      else
        c := if ← expr_preprocess R then 1 else 0
        modify fun s => { s with ifdef_stack := s.ifdef_stack.set! (s.ifdef_stack_ptr - 1) c }
      if (← get).ifdef_stack_ptr == (← curFile).ifdef_stack_ptr + 1 then
        modFile fun f => { f with ifndef_macro := 0 }
      testSkip := true
    else if tok == Tok.TOK_ENDIF then
      R.next_nomacro
      let st ← get
      let f ← curFile
      if st.ifdef_stack_ptr ≤ f.ifdef_stack_ptr then tcc_error_s R "#endif without matching #if"
      modify fun s => { s with ifdef_stack_ptr := s.ifdef_stack_ptr - 1 }
      let st ← get
      if f.ifndef_macro != 0 && st.ifdef_stack_ptr == f.ifdef_stack_ptr then
        modFile fun f => { f with ifndef_macro_saved := f.ifndef_macro, ifndef_macro := 0 }
        modify fun s => { s with tok_flags := s.tok_flags ||| TOK_FLAG_ENDIF }
    else if tok == Tok.TOK_LINE || (tok == TOK_PPNUM && (← get).parse_flags &&& PARSE_FLAG_ASM_FILE == 0) then
      let mut cc : Bool := false
      if tok == Tok.TOK_LINE then
        modify fun s => { s with parse_flags := s.parse_flags &&& ~~~PARSE_FLAG_TOK_NUM }
        R.next
        if (← getTok) != TOK_PPNUM then tcc_error_s R "wrong #line format"
        cc := true
      -- _line_num:
      let q := Bytes.cstrAt (← get).tokc.sdata 0
      let mut n : Int32 := 0
      for ch in q.data do
        if !(ch ≥ 48 && ch ≤ 57) then tcc_error_s R "wrong #line format"
        n := n * 10 + ch.toUInt32.toInt32 - 48
      modify fun s => { s with parse_flags := s.parse_flags &&& ~~~PARSE_FLAG_TOK_STR }
      R.next
      if (← getTok) != TOK_LINEFEED then
        let tc := (← get).tokc
        if (← getTok) != TOK_PPSTR || tc.sdata.get! 0 != 34 then tcc_error_s R "wrong #line format"
        -- tokc.str.data[size - 2] = 0; tccpp_putfile(data + 1)
        let d := tc.sdata.set! (tc.strSize - 2).toNat' 0
        tccpp_putfile (Bytes.cstrAt d 1)
        R.next
        skip_to_eol R cc
      let f ← curFile
      if f.fd > 0 then modify fun s => { s with total_lines := s.total_lines + f.line_num - n }
      modFile fun f => { f with line_num := n }
    else if tok == TOK_PPNUM then
      ignore := true
    else if tok == Tok.TOK_ERROR || tok == Tok.TOK_WARNING then
      let mut buf := ByteArray.empty
      let mut ch ← skip_spaces R
      for _ in [0:fuel] do
        if ch == 10 || ch == CH_EOF then break
        if buf.size < 1023 then buf := buf.push ch.toUInt8
        ch ← ninp R
      if tok == Tok.TOK_ERROR then tcc_error R (cfmt "#error %s" [.s buf])
      else tcc_warning R (cfmt "#warning %s" [.s buf])
      R.next_nomacro
    else if tok == Tok.TOK_PRAGMA then
      if !(← pragma_parse R) then ignore := true
    else if tok == TOK_LINEFEED then
      theEnd := true
    else
      if saved_parse_flags &&& PARSE_FLAG_ASM_FILE != 0 then ignore := true
      else if tok == 33 && is_bof != 0 then ignore := true
      else
        let st ← get
        tcc_warning R (cfmt "ignoring unknown preprocessing directive #%s" [.s (← get_tok_str st.tok (some st.tokc))])
        ignore := true
    if testSkip then
      if c &&& 1 == 0 then
        skip_to_eol R true
        preprocess_skip R
        is_bof := 0
        continue   -- goto redo
    break
  if ignore then
    skip_to_eol R false
  else if !theEnd then
    skip_to_eol R true
  modify fun s => { s with parse_flags := saved_parse_flags }

/-! ## string and number literals -/

/-- `unicode_to_utf8` -/
def unicode_to_utf8 (R : Rec) (uc : UInt32) : M ByteArray := do
  let b := ByteArray.empty
  if uc < 0x80 then return b.push uc.toUInt8
  else if uc < 0x800 then return (b.push (192 + uc / 64).toUInt8).push (128 + uc % 64).toUInt8
  else if uc - 0xd800 < 0x800 then tcc_error R (cfmt "0x%x is not a valid universal character" [.u uc.toNat])
  else if uc < 0x10000 then
    return ((b.push (224 + uc / 4096).toUInt8).push (128 + uc / 64 % 64).toUInt8).push (128 + uc % 64).toUInt8
  else if uc < 0x110000 then
    return (((b.push (240 + uc / 262144).toUInt8).push (128 + uc / 4096 % 64).toUInt8).push (128 + uc / 64 % 64).toUInt8).push (128 + uc % 64).toUInt8
  else tcc_error R (cfmt "0x%x is not a valid universal character" [.u uc.toNat])

/-- `parse_escape_string(outstr, buf, is_long)` -/
def parse_escape_string (R : Rec) (buf : ByteArray) (is_long : Bool) : M ByteArray := do
  let mut out := ByteArray.empty
  let mut p := 0
  let at_ (i : Nat) : Int32 := if i < buf.size then (buf.get! i).toUInt32.toInt32 else 0
  let fuel ← fuelM
  let addc (out : ByteArray) (c : Int32) : ByteArray :=
    if !is_long then out.push c.toUInt8
    else Bytes.w32 (out ++ Bytes.zeros 4) out.size c.toUInt32
  for _ in [0:fuel] do
    let mut c := at_ p
    if c == 0 then break
    if c == 92 then
      p := p + 1
      c := at_ p
      if c ≥ 48 && c ≤ 55 then
        let mut n := c - 48
        p := p + 1
        c := at_ p
        if isOct c then
          n := n * 8 + c - 48
          p := p + 1
          c := at_ p
          if isOct c then
            n := n * 8 + c - 48
            p := p + 1
        out := addc out n
        continue
      else if c == 120 || c == 117 || c == 85 then
        let mut i : Int32 := if c == 120 then 0 else if c == 117 then 4 else 8
        p := p + 1
        let mut n : UInt32 := 0
        let mut addHex := false
        for _ in [0:fuel] do
          let mut d := at_ p
          if d ≥ 97 && d ≤ 102 then d := d - 97 + 10
          else if d ≥ 65 && d ≤ 70 then d := d - 65 + 10
          else if isNum d then d := d - 48
          else if i ≥ 0 then expect R "more hex digits in universal-character-name"
          else
            addHex := true; break
          n := n * 16 + d.toUInt32
          p := p + 1
          i := i - 1
          if i == 0 then break
        if is_long || addHex then
          out := addc out n.toInt32
        else
          out := out ++ (← unicode_to_utf8 R n)
        continue
      else
        if c == 97 then c := 7
        else if c == 98 then c := 8
        else if c == 102 then c := 12
        else if c == 110 then c := 10
        else if c == 114 then c := 13
        else if c == 116 then c := 9
        else if c == 118 then c := 11
        else if c == 101 && (← get).gnu_ext != 0 then c := 27
        else if c == 39 || c == 34 || c == 92 || c == 63 then pure ()
        else
          if c ≥ 33 && c ≤ 126 then
            tcc_warning R (cfmt "unknown escape sequence: '\\%c'" [.i c.toInt])
          else
            tcc_warning R (cfmt "unknown escape sequence: '\\x%x'" [.i c.toInt])
    else if is_long && c ≥ 0x80 then
      let mut cont := 0
      let mut n : Int32 := 0
      let mut skip := 0
      let mut bad := false
      if c < 0xC2 then skip := 1; bad := true
      else if c ≤ 0xDF then cont := 1; n := c &&& 0x1f
      else if c ≤ 0xEF then cont := 2; n := c &&& 0xf
      else if c ≤ 0xF4 then cont := 3; n := c &&& 0x7
      else skip := 1; bad := true
      if !bad then
        for i in [1:cont+1] do
          let mut l : Int32 := 0x80
          let mut h : Int32 := 0xBF
          if i == 1 then
            if c == 0xE0 then l := 0xA0
            else if c == 0xED then h := 0x9F
            else if c == 0xF0 then l := 0x90
            else if c == 0xF4 then h := 0x8F
          let pi := at_ (p + i)
          if pi < l || pi > h then
            skip := i; bad := true; break
          n := (n <<< 6) ||| (pi &&& 0x3f)
      if !bad then
        p := p + 1 + cont
        out := addc out n
        continue
      tcc_warning R (cfmt "ill-formed UTF-8 subsequence starting with: '\\x%x'" [.i c.toInt])
      p := p + skip
      out := addc out 0xFFFD
      continue
    p := p + 1
    out := addc out c
  if !is_long then return out.push 0
  else return addc out 0

/-- `parse_string(s, len)` -/
def parse_string (R : Rec) (s0 : ByteArray) (len0 : Nat) : M Unit := do
  let mut s := s0
  let mut len := len0
  let is_long := s.size > 0 && s.get! 0 == 76
  if is_long then s := s.extract 1 s.size; len := len - 1
  let sep := s.get! 0
  s := s.extract 1 s.size
  len := len - 2
  let p := s.extract 0 len
  let str ← parse_escape_string R p is_long
  modify fun st => { st with tokcstr := str }
  if sep == 39 then
    let char_size := if is_long then 4 else 1
    let tok := if is_long then TOK_LCHAR else TOK_CCHAR
    let n : Int := (str.size / char_size : Nat) - 1
    if n < 1 then tcc_error_s R "empty character constant"
    if n > 1 then tcc_warning_c R WARNC_all "multi-character character constant".toUTF8
    let mut c : Int32 := 0
    for i in [0:n.toNat] do
      if is_long then c := (Bytes.r32 str (4*i)).toInt32
      else c := (c <<< 8) ||| (str.get! i).toUInt32.toInt32
    modify fun st => { st with tok := tok, tokc := st.tokc.setI c.toInt64.toUInt64 }
  else
    modify fun st => { st with tok := if is_long then TOK_LSTR else TOK_STR,
                               tokc := st.tokc.setStr str (Int32.ofNat str.size) }

/-- `bn_lshift(bn, shift, or_val)` on a 128-bit number -/
def bn_lshift (bn : Nat) (shift : Nat) (or_val : Nat) : Nat × Nat :=
  if bn / 2 ^ (128 - shift) != 0 then (bn, shift)
  else ((bn * 2 ^ shift + or_val) % 2 ^ 128, 0)

/-- `parse_number(p)` -/
def parse_number (R : Rec) (src : ByteArray) : M Unit := do
  let at_ (i : Nat) : Int32 := if i < src.size then (src.get! i).toUInt32.toInt32 else 0
  let mut p := 0
  let mut q := ByteArray.empty   -- token_buf
  let mut ch := at_ p
  p := p + 1
  let t0 := ch
  ch := at_ p
  p := p + 1
  q := q.push t0.toUInt8
  let mut b : Int32 := 10
  let mut fracDirect := false
  if t0 == 46 then
    fracDirect := true
  else if t0 == 48 then
    if ch == 120 || ch == 88 then
      q := ByteArray.empty; ch := at_ p; p := p + 1; b := 16
    else if (← get).tcc_ext != 0 && (ch == 98 || ch == 66) then
      q := ByteArray.empty; ch := at_ p; p := p + 1; b := 2
  let fuel ← fuelM
  if !fracDirect then
    for _ in [0:fuel] do
      let mut t : Int32 := 0
      if ch ≥ 97 && ch ≤ 102 then t := ch - 97 + 10
      else if ch ≥ 65 && ch ≤ 70 then t := ch - 65 + 10
      else if isNum ch then t := ch - 48
      else break
      if t ≥ b then break
      if q.size ≥ STRING_MAX_SIZE then tcc_error_s R "number too long"
      q := q.push ch.toUInt8
      ch := at_ p
      p := p + 1
  if fracDirect || ch == 46 || ((ch == 101 || ch == 69) && b == 10) || ((ch == 112 || ch == 80) && (b == 16 || b == 2)) then
    if b != 10 then
      -- hexadecimal or binary floats
      let shift := if b == 16 then 4 else 1
      let mut bn : Nat := 0
      let mut frac_bits : Int := 0
      for c in q.data do
        let c := c.toUInt32.toInt32
        let t := if c ≥ 97 then c - 97 + 10 else if c ≥ 65 then c - 65 + 10 else c - 48
        let (bn', r) := bn_lshift bn shift t.toNat'
        bn := bn'; frac_bits := frac_bits - r
      if ch == 46 then
        ch := at_ p; p := p + 1
        for _ in [0:fuel] do
          let mut t := ch
          if t ≥ 97 && t ≤ 102 then t := t - 97 + 10
          else if t ≥ 65 && t ≤ 70 then t := t - 65 + 10
          else if t ≥ 48 && t ≤ 57 then t := t - 48
          else break
          if t ≥ b then tcc_error_s R "invalid digit"
          let (bn', r) := bn_lshift bn shift t.toNat'
          bn := bn'; frac_bits := frac_bits - r + shift
          ch := at_ p; p := p + 1
      if ch != 112 && ch != 80 then expect R "exponent"
      ch := at_ p; p := p + 1
      let mut s : Int := 1
      let mut exp_val : Int := 0
      if ch == 43 then ch := at_ p; p := p + 1
      else if ch == 45 then s := -1; ch := at_ p; p := p + 1
      if ch < 48 || ch > 57 then expect R "exponent digits"
      for _ in [0:fuel] do
        if !(ch ≥ 48 && ch ≤ 57) then break
        if exp_val < 100000000 then exp_val := exp_val * 10 + (ch - 48).toInt
        ch := at_ p; p := p + 1
      exp_val := exp_val * s
      -- d = bn[3]*2^96 + bn[2]*2^64 + bn[1]*2^32 + bn[0] in long double, then ldexpl
      let w (k : Nat) : Nat := (bn / 2 ^ (32 * k)) % 2 ^ 32
      let f := SF.F128
      let d := SF.add f (SF.add f (SF.add f (SF.ofInt f (w 3 * 2 ^ 96)) (SF.ofInt f (w 2 * 2 ^ 64))) (SF.ofInt f (w 1 * 2 ^ 32))) (SF.ofInt f (w 0))
      let d := match SF.decode f d with
        | .fin n m e => SF.roundQ f n m (e + exp_val - frac_bits) false
        | _ => d
      let t := toUp ch
      if t == 70 then
        ch := at_ p; p := p + 1
        let fb := SF.cvt f SF.F32 d
        modify fun st => { st with tok := TOK_CFLOAT, tokc := st.tokc.setFbits (UInt32.ofNat fb) }
      else if t == 76 then
        ch := at_ p; p := p + 1
        modify fun st => { st with
          tok := TOK_CLDOUBLE,
          tokc := { st.tokc with lo := UInt64.ofNat (d % 2^64), hi := UInt64.ofNat (d / 2^64) } }
      else
        let db := SF.cvt f SF.F64 d
        modify fun st => { st with tok := TOK_CDOUBLE, tokc := st.tokc.setI (UInt64.ofNat db) }
    else
      -- decimal floats
      if ch == 46 || fracDirect then
        if !fracDirect then
          if q.size ≥ STRING_MAX_SIZE then tcc_error_s R "number too long"
          q := q.push ch.toUInt8
          ch := at_ p; p := p + 1
        for _ in [0:fuel] do
          if !(ch ≥ 48 && ch ≤ 57) then break
          if q.size ≥ STRING_MAX_SIZE then tcc_error_s R "number too long"
          q := q.push ch.toUInt8
          ch := at_ p; p := p + 1
      if ch == 101 || ch == 69 then
        if q.size ≥ STRING_MAX_SIZE then tcc_error_s R "number too long"
        q := q.push ch.toUInt8
        ch := at_ p; p := p + 1
        if ch == 45 || ch == 43 then
          if q.size ≥ STRING_MAX_SIZE then tcc_error_s R "number too long"
          q := q.push ch.toUInt8
          ch := at_ p; p := p + 1
        if ch < 48 || ch > 57 then expect R "exponent digits"
        for _ in [0:fuel] do
          if !(ch ≥ 48 && ch ≤ 57) then break
          if q.size ≥ STRING_MAX_SIZE then tcc_error_s R "number too long"
          q := q.push ch.toUInt8
          ch := at_ p; p := p + 1
      let t := toUp ch
      let txt := Bytes.toStr q
      if t == 70 then
        ch := at_ p; p := p + 1
        -- newlib's strtof: (float) strtod
        let db := SF.ofDecimal SF.F64 txt
        let fb := SF.cvt SF.F64 SF.F32 db
        modify fun st => { st with tok := TOK_CFLOAT, tokc := st.tokc.setFbits (UInt32.ofNat fb) }
      else if t == 76 then
        ch := at_ p; p := p + 1
        -- newlib strtold = _strtorx (x87 layout in bytes 0..9; 10..15 unwritten)
        let st0 ← get
        let g := (st0.ldGarbage[st0.ldGarbageIdx]?).getD 0
        modify fun st => { st with ldGarbageIdx := st.ldGarbageIdx + 1 }
        let d := SF.ofDecimalX87 txt + (g.toNat % 2 ^ 48) * 2 ^ 80
        modify fun st => { st with
          tok := TOK_CLDOUBLE,
          tokc := { st.tokc with lo := UInt64.ofNat (d % 2^64), hi := UInt64.ofNat (d / 2^64) } }
      else
        let db := SF.ofDecimal SF.F64 txt
        modify fun st => { st with tok := TOK_CDOUBLE, tokc := st.tokc.setI (UInt64.ofNat db) }
  else
    -- integer number
    let mut qi := 0
    if b == 10 && q.size > 0 && q.get! 0 == 48 then
      b := 8; qi := 1
    let mut n : UInt64 := 0
    let mut ov := false
    for k in [qi:q.size] do
      let c := (q.get! k).toUInt32.toInt32
      let t := if c ≥ 97 then c - 97 + 10 else if c ≥ 65 then c - 65 + 10 else c - 48
      if t ≥ b then tcc_error_s R "invalid digit"
      let n1 := n
      n := n * b.toInt64.toUInt64 + t.toInt64.toUInt64
      if n1 ≥ 0x1000000000000000 && n / b.toInt64.toUInt64 != n1 then ov := true
    let mut lcount := 0
    let mut ucount := 0
    let p1 := p
    for _ in [0:fuel] do
      let t := toUp ch
      if t == 76 then
        if lcount ≥ 2 then tcc_error_s R "three 'l's in integer constant"
        if lcount != 0 && at_ (p - 1) != ch then
          tcc_error R (cfmt "incorrect integer suffix: %s" [.s (Bytes.cstrAt src p1)])
        lcount := lcount + 1
        ch := at_ p; p := p + 1
      else if t == 85 then
        if ucount ≥ 1 then tcc_error_s R "two 'u's in integer constant"
        ucount := ucount + 1
        ch := at_ p; p := p + 1
      else break
    if (← get).pp_expr != 0 then lcount := 2
    if ucount == 0 && b == 10 then
      if lcount ≤ 0 then
        if n ≥ 0x80000000 then lcount := 1
      if n ≥ 0x8000000000000000 then ov := true; ucount := 1
    else
      if lcount ≤ 0 then
        if n ≥ 0x100000000 then lcount := 1
        else if n ≥ 0x80000000 then ucount := 1
      if n ≥ 0x8000000000000000 then ucount := 1
    if ov then tcc_warning R "integer constant overflow".toUTF8
    let mut tok := TOK_CINT
    if lcount != 0 then
      tok := TOK_CLONG
      if lcount == 2 then tok := TOK_CLLONG
    if ucount != 0 then tok := tok + 1
    modify fun st => { st with tok := tok, tokc := st.tokc.setI n }
  if ch != 0 then tcc_error_s R "invalid number"

end Tcc.Model
