import Tcc.Model.Libtcc

/-!
# TccModel: tccpp.c, part 1 — characters, comments, strings, token strings

The buffer pointer `p` of tcc's scanning functions is an index into the
current file's `buf`; functions that move it return the new index.
-/

namespace Tcc.Model
open C

/-! ## the current buffer -/

@[inline] def rd (p : Nat) : M Int32 := do
  match (← get).file with
  | f :: _ => return (f.buf.get! p).toUInt32.toInt32
  | [] => return 0

@[inline] def wrb (p : Nat) (c : Int32) : M Unit := modFileBuf fun b => b.set! p c.toUInt8

@[inline] def bufPtr : M Nat := do return (← curFile).buf_ptr
@[inline] def setBufPtr (p : Nat) : M Unit := modFile fun f => { f with buf_ptr := p }

@[inline] def isidnum (c : Int32) : M UInt8 := do
  return (← get).isidnum_table[(c + 1).toNat']!

/-- `tcc_trigraphs(buf, len)`: C11 phase 1 on a physical byte block.
The refiller holds up to two trailing question marks for the next block.
Synthetic token/paste/pragma buffers never enter this translation. -/
def tcc_trigraphs (buf : ByteArray) : ByteArray := Id.run do
  let mut out := ByteArray.empty
  let mut i := 0
  for _ in [0:buf.size] do
    if i ≥ buf.size then break
    let mut c := buf.get! i
    if c == 63 && i + 2 < buf.size && buf.get! (i + 1) == 63 then
      let r : Option UInt8 := match (buf.get! (i + 2)).toNat with
        | 61 => some 35 | 47 => some 92 | 39 => some 94
        | 40 => some 91 | 41 => some 93 | 33 => some 124
        | 60 => some 123 | 62 => some 125 | 45 => some 126
        | _ => none
      if let some v := r then
        c := v
        i := i + 2
    out := out.push c
    i := i + 1
  return out

/-- `handle_eob()` -/
def handle_eob (R : Rec) : M Int32 := do
  let bf ← curFile
  if bf.buf_ptr ≥ bf.buf_end then
    let mut data := ByteArray.empty
    if bf.fd ≥ 0 then
      if (← get).cversion ≥ 201112 && (← get).parse_flags &&& PARSE_FLAG_ASM_FILE == 0 then
        -- Each empty non-EOF result accumulates another question mark;
        -- after at most three reads one byte must be emitted.
        let mut pending := bf.trigraph_pending
        for _ in [0:3] do
          let some raw ← Host.read bf.fd (IO_BUF_SIZE - pending)
            | tcc_error_s R "could not read source file"
          modify fun s => { s with total_bytes := s.total_bytes + UInt32.ofNat raw.size }
          let input := ByteArray.mk (Array.replicate pending (63 : UInt8)) ++ raw
          pending := 0
          if raw.size > 0 then
            for _ in [0:2] do
              if pending < input.size && input.get! (input.size - pending - 1) == 63 then
                pending := pending + 1
              else break
          modFile fun f => { f with trigraph_pending := pending }
          data := tcc_trigraphs (input.extract 0 (input.size - pending))
          if data.size > 0 || raw.size == 0 then break
      else
        let some raw ← Host.read bf.fd IO_BUF_SIZE
          | tcc_error_s R "could not read source file"
        data := raw
        modify fun s => { s with total_bytes := s.total_bytes + UInt32.ofNat data.size }
    let len := data.size
    modFile fun f =>
      let b := f.buf
      let f := { f with buf := ByteArray.empty }
      let b := Bytes.copy b f.bufStart data 0 len
      let b := b.set! (f.bufStart + len) CH_EOB.toUInt8
      { f with buf := b, buf_ptr := f.bufStart, buf_end := f.bufStart + len }
  let bf ← curFile
  if bf.buf_ptr < bf.buf_end then
    return (bf.buf.get! bf.buf_ptr).toUInt32.toInt32
  else
    setBufPtr bf.buf_end
    return CH_EOF

/-- `next_c()` -/
def next_c (R : Rec) : M Int32 := do
  modFile fun f => { f with buf_ptr := f.buf_ptr + 1 }
  let bf ← curFile
  let ch := (bf.buf.get! bf.buf_ptr).toUInt32.toInt32
  if ch == CH_EOB && bf.buf_ptr ≥ bf.buf_end then handle_eob R else return ch

/-- `handle_stray_noerror(err)` -/
def handle_stray_noerror (R : Rec) (err : Bool) : M Int32 := do
  let fuel ← fuelM
  for _ in [0:fuel] do
    let ch ← next_c R
    if ch != 92 then return ch
    let ch ← next_c R
    if ch == 10 then
      modFile fun f => { f with line_num := f.line_num + 1 }
    else
      let mut ch := ch
      let mut newl := false
      if ch == 13 then
        ch ← next_c R
        if ch == 10 then
          modFile fun f => { f with line_num := f.line_num + 1 }
          newl := true
        else
          modFile fun f => { f with buf_ptr := f.buf_ptr - 1 }
          wrb (← bufPtr) 13
      if !newl then
        if err && ch != 117 && ch != 85 then tcc_error_s R "stray '\\' in program"
        modFile fun f => { f with buf_ptr := f.buf_ptr - 1 }
        wrb (← bufPtr) 92
        return 92
  throw (.fuel "handle_stray_noerror")

/-- `ninp()` -/
@[inline] def ninp (R : Rec) : M Int32 := handle_stray_noerror R false

/-- `handle_bs(&p)` -/
def handle_bs (R : Rec) (p : Nat) : M (Int32 × Nat) := do
  setBufPtr (p - 1)
  let c ← ninp R
  return (c, ← bufPtr)

/-- `handle_stray(&p)` -/
def handle_stray (R : Rec) (p : Nat) : M (Int32 × Nat) := do
  setBufPtr (p - 1)
  let c ← handle_stray_noerror R ((← get).parse_flags &&& PARSE_FLAG_ACCEPT_STRAYS == 0)
  return (c, ← bufPtr)

/-- `PEEKC(c, p)` -/
@[inline] def peekc (R : Rec) (p : Nat) : M (Int32 × Nat) := do
  let p := p + 1
  let c ← rd p
  if c == 92 then handle_stray R p else return (c, p)

/-- `skip_spaces()` -/
def skip_spaces (R : Rec) : M Int32 := do
  modFile fun f => { f with buf_ptr := f.buf_ptr - 1 }
  let fuel ← fuelM
  for _ in [0:fuel] do
    let ch ← ninp R
    if (← isidnum ch) &&& IS_SPC == 0 then return ch
  throw (.fuel "skip_spaces")

/-- `parse_line_comment(p)` -/
def parse_line_comment (R : Rec) (p0 : Nat) : M Nat := do
  let fuel ← fuelM
  let mut p := p0
  let mut c : Int32 := 0
  let mut adv := true
  for _ in [0:fuel] do
    if adv || !(c == 10 || c == 92) then
      for _ in [0:fuel] do
        p := p + 1
        c ← rd p
        if c == 10 || c == 92 then break
    if c == 10 then return p
    let (c', p') ← handle_bs R p
    c := c'; p := p'
    if c == CH_EOF then return p
    adv := c == 92
  throw (.fuel "parse_line_comment")

/-- `parse_comment(p)` -/
def parse_comment (R : Rec) (p0 : Nat) : M Nat := do
  let fuel ← fuelM
  let mut p := p0
  let mut c : Int32 := 0
  let mut adv := true
  for _ in [0:fuel] do
    if adv || !(c == 10 || c == 42 || c == 92) then
      for _ in [0:fuel] do
        p := p + 1
        c ← rd p
        if c == 10 || c == 42 || c == 92 then break
    if c == 10 then
      modFile fun f => { f with line_num := f.line_num + 1 }
      adv := true
    else if c == 42 then
      for _ in [0:fuel] do
        p := p + 1
        c ← rd p
        if c != 42 then break
      if c == 92 then
        let (c', p') ← handle_bs R p
        c := c'; p := p'
      if c == 47 then return p + 1
      -- check_eof
      if c == CH_EOF then tcc_error_s R "unexpected end of file in comment"
      adv := c == 92
    else
      let (c', p') ← handle_bs R p
      c := c'; p := p'
      if c == CH_EOF then tcc_error_s R "unexpected end of file in comment"
      adv := c == 92
  throw (.fuel "parse_comment")

/-- `parse_pp_string(p, sep, str)`: returns the new p and the string
(`str` is none when skipping) -/
def parse_pp_string (R : Rec) (p0 : Nat) (sep : Int32) (str0 : Option ByteArray) : M (Nat × Option ByteArray) := do
  let fuel ← fuelM
  let mut p := p0
  let mut str := str0
  let mut c : Int32 := 0
  let mut adv := true
  let unterminated : M (Nat × Option ByteArray) := do
    modify fun s => { s with tok_flags := s.tok_flags &&& ~~~TOK_FLAG_BOL }
    tcc_error R (cfmt "missing terminating %c character" [.i sep.toInt])
  for _ in [0:fuel] do
    if adv then
      p := p + 1
      c ← rd p
    adv := true
    -- redo:
    if c == sep then
      return (p + 1, str)
    else if c == 92 then
      let (c', p') ← handle_bs R p
      c := c'; p := p'
      if c == CH_EOF then
        return ← unterminated
      else if c == 92 then
        str := str.map (·.push 92)
        p := p + 1
        c ← rd p
        if c == 92 then
          let (c', p') ← handle_bs R p
          c := c'; p := p'
          if c == CH_EOF then return ← unterminated
        -- add_char
        str := str.map (·.push c.toUInt8)
      else
        adv := false   -- goto redo
    else if c == 10 then
      -- add_lf (ACCEPT_LF_IN_STRINGS = 0)
      if str.isSome then return ← unterminated
      else return (p, str)
    else if c == 13 then
      p := p + 1
      c ← rd p
      if c == 92 then
        let (c', p') ← handle_bs R p
        c := c'; p := p'
      if c == 10 then
        if str.isSome then return ← unterminated
        else return (p, str)
      if c == CH_EOF then return ← unterminated
      str := str.map (·.push 13)
      adv := false   -- goto redo
    else
      str := str.map (·.push c.toUInt8)
  throw (.fuel "parse_pp_string")

/-! ## token strings -/

/-- `tok_str_add(s, t)` -/
def tok_str_add (s : TokenString) (t : Int32) : TokenString :=
  let len := s.len.toNat'
  let str := if len < s.str.size then s.str.set! len t else s.str.push t
  { s with str := str, len := s.len + 1 }

/-- pack bytes into little-endian int words (as memcpy into an int array) -/
def packBytes (b : ByteArray) : Array Int32 := Id.run do
  let nw := (b.size + 3) / 4
  let mut out : Array Int32 := Array.mkEmpty nw
  for w in [0:nw] do
    let mut v : UInt32 := 0
    for k in [0:4] do
      let i := 4 * w + k
      if i < b.size then v := v ||| ((b.get! i).toUInt32 <<< (8 * k).toUInt32)
    out := out.push v.toInt32
  return out

/-- `tok_str_add2(s, t, cv)` -/
def tok_str_add2 (s : TokenString) (t : Int32) (cv : CValue) : TokenString := Id.run do
  let mut s := tok_str_add s t
  if t == TOK_CINT || t == TOK_CUINT || t == TOK_CCHAR || t == TOK_LCHAR || t == TOK_U16CHAR || t == TOK_U32CHAR || t == TOK_CFLOAT || t == TOK_LINENUM || t == TOK_PPLINE then
    s := tok_str_add s (cv.tab 0).toInt32
  else if t == TOK_PPNUM || t == TOK_PPSTR || TOK_IS_STR t then
    let size := cv.strSize
    s := tok_str_add s size
    for w in packBytes (cv.sdata.extract 0 size.toNat') do s := tok_str_add s w
  else if t == TOK_CDOUBLE || t == TOK_CLLONG || t == TOK_CULLONG || t == TOK_CLONG || t == TOK_CULONG then
    s := tok_str_add s (cv.tab 0).toInt32
    s := tok_str_add s (cv.tab 1).toInt32
  else if t == TOK_CLDOUBLE then
    s := tok_str_add s (cv.tab 0).toInt32
    s := tok_str_add s (cv.tab 1).toInt32
    s := tok_str_add s (cv.tab 2).toInt32
    s := tok_str_add s (cv.tab 3).toInt32
  return s

/-- `tok_str_add_tok(s)`: the current token, with line info -/
def tok_str_add_tok (s : TokenString) : M TokenString := do
  let ln := (← curFile).line_num
  let mut s := s
  if ln != s.last_line_num then
    s := { s with last_line_num := ln }
    s := tok_str_add2 s TOK_LINENUM (CValue.ofI ln.toInt64.toUInt64)
  let st ← get
  return tok_str_add2 s st.tok st.tokc

/-- `tok_str_add2_spc(s, t, cv)` -/
def tok_str_add2_spc (s : TokenString) (t : Int32) (cv : CValue) : TokenString :=
  let s := if s.need_spc == 3 then tok_str_add s 32 else s
  tok_str_add2 { s with need_spc := 2 } t cv

/-- Macro token provenance does not move the physical input cursor. -/
def tok_str_add2_line (s : TokenString) (t : Int32) (cv : CValue) (line : Int32) : TokenString :=
  let s := if s.need_spc == 3 then tok_str_add s 32 else s
  let s := tok_str_add2 { s with need_spc := 2 } TOK_PPLINE (CValue.ofI line.toInt64.toUInt64)
  tok_str_add2 s t cv

/-- unpack `n` bytes from int words starting at word `w0` -/
def unpackBytes (a : TokArr) (w0 n : Nat) : ByteArray := Id.run do
  let mut out := ByteArray.empty
  for i in [0:n] do
    let w := (a[w0 + i / 4]?.getD 0).toUInt32
    out := out.push (w >>> (8 * (i % 4)).toUInt32).toUInt8
  return out

/-- `tok_get(&t, &p, cv)`: the token, the updated value, the new pointer -/
def tok_get (p : TokPtr) (cv0 : CValue) : Int32 × CValue × TokPtr := Id.run do
  let t := p.get 0
  let mut q := p.pos + 1
  let mut cv := cv0
  let at_ (k : Nat) : Int32 := p.arr[k]?.getD 0
  if t == TOK_CINT || t == TOK_CCHAR || t == TOK_LCHAR || t == TOK_U16CHAR || t == TOK_U32CHAR || t == TOK_LINENUM || t == TOK_PPLINE then
    cv := cv.setI (at_ q).toInt64.toUInt64; q := q + 1
  else if t == TOK_CUINT then
    cv := cv.setI (at_ q).toUInt32.toUInt64; q := q + 1
  else if t == TOK_CFLOAT then
    cv := cv.setTab 0 (at_ q).toUInt32; q := q + 1
  else if TOK_IS_STR t || t == TOK_PPNUM || t == TOK_PPSTR then
    let size := at_ q
    q := q + 1
    cv := cv.setStr (unpackBytes p.arr q size.toNat') size
    q := q + (size.toNat' + 3) / 4
  else if t == TOK_CDOUBLE || t == TOK_CLLONG || t == TOK_CULLONG || t == TOK_CLONG || t == TOK_CULONG then
    cv := (cv.setTab 0 (at_ q).toUInt32).setTab 1 (at_ (q+1)).toUInt32; q := q + 2
  else if t == TOK_CLDOUBLE then
    cv := (((cv.setTab 0 (at_ q).toUInt32).setTab 1 (at_ (q+1)).toUInt32).setTab 2 (at_ (q+2)).toUInt32).setTab 3 (at_ (q+3)).toUInt32
    q := q + 4
  return (t, cv, { p with pos := q })

/-- `TOK_GET(&t, &p, cv)` -/
@[inline] def TOK_GET (p : TokPtr) (cv : CValue) : Int32 × CValue × TokPtr :=
  let t := p.get 0
  if TOK_HAS_VALUE t then tok_get p cv else (t, cv, { p with pos := p.pos + 1 })

/-! ## macro_stack (begin_macro / end_macro) -/

/-- `begin_macro(str, alloc)`: `ref` says which TokenString it is; `arr`
is its `str` (from offset `pos`) -/
def begin_macro (ref : TSRef) (arr : TokArr) (pos : Nat) (alloc : Int32) : M Nat := do
  let id ← freshId
  let s ← get
  let ln : Int32 := match s.file with
    | f :: _ => f.line_num
    | [] => 0
  let fr : MacroFrame := {
    ref := ref
    id := id
    prev_ptr := s.macro_ptr
    save_line_num := ln
    save_tok_line := s.tok_line
    alloc := alloc
    str := ({ arr := arr, pos := pos } : TokPtr) }
  set { s with macro_stack := fr :: s.macro_stack, macro_ptr := some { arr := arr, pos := pos } }
  return id

/-- `end_macro()` -/
def end_macro : M Unit := do
  let s ← get
  match s.macro_stack with
  | fr :: rest =>
    set { s with macro_stack := rest, macro_ptr := fr.prev_ptr, tok_line := fr.save_tok_line }
    modFile fun f => { f with line_num := fr.save_line_num }
    if fr.alloc == 0 then
      match fr.ref with
      | .tokstrBuf => modify fun s => { s with tokstr_buf := { s.tokstr_buf with len := 0, need_spc := 0 } }
      | .ungetBuf => modify fun s => { s with unget_buf := { s.unget_buf with len := 0, need_spc := 0 } }
      | .heap => pure ()
  | [] => pure ()

end Tcc.Model
