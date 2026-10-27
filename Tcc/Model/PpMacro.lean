import Tcc.Model.PpDir

/-!
# TccModel: tccpp.c, part 3 — `next_nomacro`, the macro engine, `next`,
and the preprocessor's setup
-/

namespace Tcc.Model
open C

/-! ## next_nomacro -/

/-- Patch 0003, C11 Annex D.1 identifier ranges. -/
def ucn_identifier (u : UInt32) (initial : Bool) : Bool :=
  let ranges : Array (UInt32 × UInt32) := #[
    (0xa8,0xa8), (0xaa,0xaa), (0xad,0xad), (0xaf,0xaf),
    (0xb2,0xb5), (0xb7,0xba), (0xbc,0xbe), (0xc0,0xd6),
    (0xd8,0xf6), (0xf8,0x167f), (0x1681,0x180d),
    (0x180f,0x1fff), (0x200b,0x200d), (0x202a,0x202e),
    (0x203f,0x2040), (0x2054,0x2054), (0x2060,0x206f),
    (0x2070,0x218f), (0x2460,0x24ff), (0x2776,0x2793),
    (0x2c00,0x2dff), (0x2e80,0x2fff), (0x3004,0x3007),
    (0x3021,0x302f), (0x3031,0xd7ff), (0xf900,0xfd3d),
    (0xfd40,0xfdcf), (0xfdf0,0xfe44), (0xfe47,0xfffd)]
  if initial && ((u >= 0x300 && u <= 0x36f) || (u >= 0x1dc0 && u <= 0x1dff) ||
     (u >= 0x20d0 && u <= 0x20ff) || (u >= 0xfe20 && u <= 0xfe2f)) then false
  else if u >= 0x10000 then u <= 0xefffd && (u &&& 0xffff) <= 0xfffd
  else ranges.any fun (lo, hi) => lo <= u && u <= hi

/-- The slow identifier path retains raw UCN spelling and allocates the
canonical UTF-8 spelling first, exactly as patched next_nomacro. -/
def parse_ident_slow (R : Rec) (p0 : Nat) (cs0 : ByteArray) : M Nat := do
  let fuel ← fuelM
  let mut p := p0
  let mut c ← rd p
  let mut cs := cs0
  let mut spelling := cs0
  for _ in [0:fuel] do
    if (← isidnum c) &&& (IS_ID ||| IS_NUM) == 0 && c != 92 then break
    if c == 92 then
      let mut u : UInt32 := 0
      spelling := spelling.push 92
      let (c', p') ← peekc R p
      c := c'; p := p'
      spelling := spelling.push c.toUInt8
      if c != 117 && c != 85 then tcc_error_s R "invalid universal-character-name"
      let digits := if c == 117 then 4 else 8
      for _ in [0:digits] do
        let (c', p') ← peekc R p
        c := c'; p := p'
        spelling := spelling.push c.toUInt8
        let d := if c >= 48 && c <= 57 then c - 48 else
          if c >= 97 && c <= 102 then c - 97 + 10 else
          if c >= 65 && c <= 70 then c - 65 + 10 else -1
        if d < 0 then tcc_error_s R "invalid universal-character-name"
        u := (u <<< 4) ||| d.toUInt32
      if !ucn_identifier u (cs.size == 0) then
        tcc_error_s R "invalid universal-character-name in identifier"
      cs := cs ++ (← unicode_to_utf8 R u)
    else
      cs := cs.push c.toUInt8
      spelling := spelling.push c.toUInt8
    let (c', p') ← peekc R p
    c := c'; p := p'
  let canonical ← tok_alloc R cs
  let ts ← tok_alloc R spelling
  modTokSym ts.tok fun t => { t with canonical := canonical.tok }
  modify fun s => { s with tokcstr := cs, tok := ts.tok }
  return p

inductive NL where
  | redo | parseSharp | identSlow | maybeSpace | maybeNewline | parseSimple | identFast | parseNum | strConst | done | keep
  deriving BEq, Inhabited

/-- `next_nomacro()` -/
def next_nomacro (R : Rec) : M Unit := do
  let fuel ← fuelM
  let mut p ← bufPtr
  let mut lbl := NL.redo
  let mut c : Int32 := 0
  let mut t : Int32 := 0
  let mut literalPrefix : Int32 := 0
  for _ in [0:fuel] do
    match lbl with
    | .redo =>
      c ← rd p
      if c == 32 || c == 9 then
        setTok c
        p := p + 1
        lbl := .maybeSpace
      else if c == 12 || c == 11 || c == 13 then
        p := p + 1
      else if c == 92 then
        let (c', p') ← handle_stray R p
        c := c'; p := p'
        if c == 92 then
          if (← rd (p + 1)) == 117 || (← rd (p + 1)) == 85 then lbl := .identSlow
          else lbl := .parseSimple
        else if c == CH_EOF then
          let st ← get
          if st.tok_flags &&& TOK_FLAG_BOL == 0 then lbl := .maybeNewline
          else if st.parse_flags &&& PARSE_FLAG_PREPROCESS == 0 then
            setTok TOK_EOF; lbl := .done
          else if st.ifdef_stack_ptr != (← curFile).ifdef_stack_ptr then
            tcc_error_s R "missing #endif"
          else if st.include_stack.size == 0 then
            setTok TOK_EOF; lbl := .done
          else
            if st.tok_flags &&& TOK_FLAG_ENDIF != 0 then
              let f ← curFile
              match ← search_cached_include f.true_filename true with
              | some i => modify fun s => { s with cached_includes := s.cached_includes.modify i fun e => { e with ifndef_macro := f.ifndef_macro_saved } }
              | none => pure ()
              modify fun s => { s with tok_flags := s.tok_flags &&& ~~~TOK_FLAG_ENDIF }
            tcc_close
            modify fun s => { s with include_stack := s.include_stack.pop }
            p ← bufPtr
            lbl := .maybeNewline
        -- else: goto redo_no_start
      else if c == 10 then
        modFile fun f => { f with line_num := f.line_num + 1 }
        p := p + 1
        lbl := .maybeNewline
      else if c == 35 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        lbl := .parseSharp
      else if c == 36 then
        if (← isidnum 36) &&& IS_ID == 0 || (← get).parse_flags &&& PARSE_FLAG_ASM_FILE != 0 then
          lbl := .parseSimple
        else lbl := .identFast
      else if c == 76 || c == 117 || c == 85 then
        let original := c
        let (c', p') ← peekc R p
        c := c'; p := p'
        literalPrefix := original
        if original == 117 && c == 56 then
          let (c', p') ← peekc R p
          c := c'; p := p'
          if c == 34 then
            literalPrefix := 56; lbl := .strConst
          else
            p := p - 1; wrb p 56
            p := p - 1; wrb p original; c := original
            lbl := .identFast
        else if c == 39 || c == 34 then lbl := .strConst
        else
          p := p - 1; wrb p original; c := original
          lbl := .identFast
      else if (c ≥ 97 && c ≤ 122) || (c ≥ 65 && c ≤ 90) || c == 95 then
        lbl := .identFast
      else if c ≥ 48 && c ≤ 57 then
        t := c
        let (c', p') ← peekc R p
        c := c'; p := p'
        lbl := .parseNum
      else if c == 46 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if isNum c then
          t := 46; lbl := .parseNum
        else if ((← isidnum 46) &&& IS_ID != 0) && ((← isidnum c) &&& (IS_ID ||| IS_NUM) != 0) then
          p := p - 1; wrb p 46; c := 46
          lbl := .identFast
        else if c == 46 then
          let (c', p') ← peekc R p
          c := c'; p := p'
          if c == 46 then
            p := p + 1; setTok TOK_DOTS
          else
            p := p - 1; wrb p 46; setTok 46
          lbl := .done
        else
          setTok 46; lbl := .done
      else if c == 39 || c == 34 then
        literalPrefix := 0; lbl := .strConst
      else if c == 60 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 58 || c == 37 then
          p := p + 1; setTok (if c == 58 then TOK_DG_LBRACK else TOK_DG_LBRACE)
        else if c == 61 then p := p + 1; setTok TOK_LE
        else if c == 60 then
          let (c', p') ← peekc R p
          c := c'; p := p'
          if c == 61 then p := p + 1; setTok TOK_A_SHL else setTok TOK_SHL
        else setTok TOK_LT
        lbl := .done
      else if c == 62 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 61 then p := p + 1; setTok TOK_GE
        else if c == 62 then
          let (c', p') ← peekc R p
          c := c'; p := p'
          if c == 61 then p := p + 1; setTok TOK_A_SAR else setTok TOK_SAR
        else setTok TOK_GT
        lbl := .done
      else if c == 38 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 38 then p := p + 1; setTok TOK_LAND
        else if c == 61 then p := p + 1; setTok TOK_A_AND
        else setTok 38
        lbl := .done
      else if c == 124 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 124 then p := p + 1; setTok TOK_LOR
        else if c == 61 then p := p + 1; setTok TOK_A_OR
        else setTok 124
        lbl := .done
      else if c == 43 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 43 then p := p + 1; setTok TOK_INC
        else if c == 61 then p := p + 1; setTok TOK_A_ADD
        else setTok 43
        lbl := .done
      else if c == 45 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 45 then p := p + 1; setTok TOK_DEC
        else if c == 61 then p := p + 1; setTok TOK_A_SUB
        else if c == 62 then p := p + 1; setTok TOK_ARROW
        else setTok 45
        lbl := .done
      else if c == 37 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 58 then
          let (c', p') ← peekc R p
          c := c'; p := p'
          let st ← get
          if st.tok_flags &&& TOK_FLAG_BOL != 0 && st.parse_flags &&& PARSE_FLAG_PREPROCESS != 0 then
            lbl := .parseSharp
          else
            setTok TOK_DG_SHARP
            if c == 37 then
              let (c', p') ← peekc R p
              c := c'; p := p'
              if c == 58 then p := p + 1; setTok TOK_DG_PASTE
              else p := p - 1; wrb p 37
            lbl := .done
        else
          if c == 62 then p := p + 1; setTok TOK_DG_RBRACE
          else if c == 61 then p := p + 1; setTok TOK_A_MOD
          else setTok 37
          lbl := .done
      else if c == 58 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 62 then p := p + 1; setTok TOK_DG_RBRACK else setTok 58
        lbl := .done
      else if c == 33 || c == 61 || c == 42 || c == 94 then
        let c1 := c
        let tok2 := if c1 == 33 then TOK_NE else if c1 == 61 then TOK_EQ else if c1 == 42 then TOK_A_MUL
                    else if c1 == 37 then TOK_A_MOD else TOK_A_XOR
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 61 then p := p + 1; setTok tok2 else setTok c1
        lbl := .done
      else if c == 47 then
        let (c', p') ← peekc R p
        c := c'; p := p'
        if c == 42 then
          p ← parse_comment R p
          setTok 32; lbl := .maybeSpace
        else if c == 47 then
          p ← parse_line_comment R p
          setTok 32; lbl := .maybeSpace
        else if c == 61 then
          p := p + 1; setTok TOK_A_DIV; lbl := .done
        else
          setTok 47; lbl := .done
      else if c == 64 || c == 40 || c == 41 || c == 91 || c == 93 || c == 123 || c == 125 ||
              c == 44 || c == 59 || c == 63 || c == 126 then
        lbl := .parseSimple
      else
        let mut handled := false
        if c == 0xEF then
          let f ← curFile
          if (← rd (p+1)) == 0xBB && (← rd (p+2)) == 0xBF && p == f.bufStart then
            p := p + 3; handled := true
        if !handled then
          if c ≥ 0x80 && c ≤ 0xFF then lbl := .identFast
          else if (← get).parse_flags &&& PARSE_FLAG_ASM_FILE != 0 then lbl := .parseSimple
          else tcc_error R (cfmt "unrecognized character \\x%02x" [.i c.toInt])
    | .parseSharp =>
        let st ← get
        if (st.tok_flags &&& TOK_FLAG_BOL != 0) && (st.parse_flags &&& PARSE_FLAG_PREPROCESS != 0) then
          modify fun s => { s with tok_flags := s.tok_flags &&& ~~~TOK_FLAG_BOL }
          setBufPtr p
          preprocess R ((← get).tok_flags &&& TOK_FLAG_BOF)
          p ← bufPtr
          lbl := .maybeNewline
        else if c == 35 then
          p := p + 1; setTok TOK_TWOSHARPS; lbl := .done
        else if st.parse_flags &&& PARSE_FLAG_ASM_FILE != 0 then
          p ← parse_line_comment R (p - 1)
        else
          setTok 35; lbl := .done
    | .identSlow =>
      p ← parse_ident_slow R p ByteArray.empty
      lbl := .done
    | .maybeSpace =>
      if (← get).parse_flags &&& PARSE_FLAG_SPACES != 0 then
        lbl := .keep
      else
        for _ in [0:fuel] do
          if (← isidnum (← rd p)) &&& IS_SPC != 0 then p := p + 1 else break
        lbl := .redo
    | .maybeNewline =>
      modify fun s => { s with tok_flags := s.tok_flags ||| TOK_FLAG_BOL }
      if (← get).parse_flags &&& PARSE_FLAG_LINEFEED == 0 then lbl := .redo
      else
        setTok TOK_LINEFEED; lbl := .keep
    | .parseSimple =>
      setTok c
      p := p + 1
      lbl := .done
    | .identFast =>
      let p1 := p
      for _ in [0:fuel] do
        p := p + 1
        c ← rd p
        if (← isidnum c) &&& (IS_ID ||| IS_NUM) == 0 then break
      if c != 92 then
        let bytes := (← curFile).buf.extract p1 p
        let ts ← tok_alloc R bytes
        setTok ts.tok
      else
        let cs := (← curFile).buf.extract p1 p
        let (c', p') ← peekc R (p - 1)
        c := c'; p := p'
        p ← parse_ident_slow R p cs
      lbl := .done
    | .parseNum =>
      let mut cs := ByteArray.empty
      let asm := (← get).parse_flags &&& PARSE_FLAG_ASM_FILE != 0
      for _ in [0:fuel] do
        cs := cs.push t.toUInt8
        let idn := (← isidnum c) &&& (IS_ID ||| IS_NUM) != 0
        let hexAsm := asm && cs.get! 0 == 48 && cs.size > 1 && toUp (cs.get! 1).toUInt32.toInt32 == 88
        let cont := idn || c == 46 ||
          ((c == 43 || c == 45) && (((t == 101 || t == 69) && !hexAsm) || t == 112 || t == 80))
        if !cont then break
        t := c
        let (c', p') ← peekc R p
        c := c'; p := p'
      cs := cs.push 0
      modify fun s => { s with tokcstr := cs, tok := TOK_PPNUM, tokc := s.tokc.setStr cs (Int32.ofNat cs.size) }
      lbl := .done
    | .strConst =>
      let mut cs := ByteArray.empty
      if literalPrefix == 56 then cs := "u8".toUTF8
      else if literalPrefix != 0 then cs := cs.push literalPrefix.toUInt8
      cs := cs.push c.toUInt8
      let (p', cs') ← parse_pp_string R p c (some cs)
      p := p'
      cs := (cs'.getD cs).push c.toUInt8 |>.push 0
      modify fun s => { s with tokcstr := cs, tok := TOK_PPSTR, tokc := s.tokc.setStr cs (Int32.ofNat cs.size) }
      lbl := .done
    | .done =>
      modify fun s => { s with tok_flags := 0 }
      setBufPtr p
      return
    | .keep =>
      setBufPtr p
      return
  throw (.fuel "next_nomacro")

/-! ## the macro engine -/

/-- `macro_arg_subst(nested_list, macro_str, args)` -/
def macro_arg_subst (R : Rec) (nested0 : Option SymId) (macro_str0 : TokPtr) (args : SymId) :
    M (TokArr × Option SymId) := do
  let fuel ← fuelM
  let mut nested := nested0
  let mut ms := macro_str0
  let mut str : TokenString := {}
  let mut t0 : Int32 := 0
  let mut t1 : Int32 := 0
  for _ in [0:fuel] do
    let (t, cval, ms') := TOK_GET ms {}
    ms := ms'
    if t == 0 then break
    if t == 35 || t == TOK_DG_SHARP then
      -- stringize
      let mut tt : Int32 := 0
      for _ in [0:fuel] do
        tt := ms.get; ms := { ms with pos := ms.pos + 1 }
        if tt != 32 then break
      let s ← sym_find2 args (← canonical_ident tt)
      if s != 0 then
        let mut cs : ByteArray := ByteArray.empty.push 34
        let mut st : TokPtr := { arr := ((← getSym s).d.getD #[]) }
        for _ in [0:fuel] do
          if st.get == TOK_EOF then break
          let (t2, cv2, st') := TOK_GET st {}
          st := st'
          let sb ← get_tok_str t2 (some cv2)
          for ch in sb.data do
            if t2 == TOK_PPSTR && (ch == 92 || ch == 34) then cs := cs.push 92
            cs := cs.push ch
        cs := (cs.push 34).push 0
        modify fun s => { s with tokcstr := cs }
        str := tok_str_add2 str TOK_PPSTR ((default : CValue).setStr cs (Int32.ofNat cs.size))
      else
        expect R "macro parameter after '#'"
    else if t ≥ TOK_IDENT then
      let s ← sym_find2 args (← canonical_ident t)
      if s != 0 then
        let sx ← getSym s
        let mut st : TokPtr := { arr := sx.d.getD #[] }
        let mut n := 0
        let mut t2 : Int32 := 0
        for _ in [0:fuel] do
          t2 := ms.get n
          if t2 != 32 then break
          n := n + 1
        let mut addVar := false
        if t2 == TOK_PPJOIN || t1 == TOK_PPJOIN then
          if t1 == TOK_PPJOIN && t0 == 44 && (← get).gnu_ext != 0 && sx.type.t != 0 then
            let c := str.str[(str.len - 1).toNat']!
            for _ in [0:fuel] do
              str := { str with len := str.len - 1 }
              if str.str[str.len.toNat']! == 44 then break
            if st.get == TOK_EOF then
              pure ()
            else
              str := { str with len := str.len + 1 }
              if c == 32 then str := tok_str_add str c
              addVar := true
          else
            if st.get == TOK_EOF then str := tok_str_add str TOK_PLCHLDR
        else
          addVar := true
        if addVar then
          let sx ← getSym s
          match sx.e with
          | some e => st := { arr := e }
          | none =>
            let (str2, nested', _) ← R.macro_subst {} nested st
            nested := nested'
            let str2 := tok_str_add str2 TOK_EOF
            let e := str2.str.extract 0 str2.len.toNat'
            modSym s fun x => { x with e := some e }
            st := { arr := e }
        for _ in [0:fuel] do
          if st.get == TOK_EOF then break
          let (t3, cv3, st') := TOK_GET st {}
          st := st'
          str := tok_str_add2 str t3 cv3
      else
        str := tok_str_add str t
    else
      str := tok_str_add2 str t cval
    if t != 32 then
      t0 := t1; t1 := t
  str := tok_str_add str 0
  return (str.str.extract 0 str.len.toNat', nested)

/-- `macro_twosharps(ptr0)` -/
def macro_twosharps (R : Rec) (ptr0 : TokPtr) : M TokArr := do
  let fuel ← fuelM
  let mut ms1 : TokenString := {}
  modify fun s => { s with tokcstr := ByteArray.empty }
  let mut ptr := ptr0
  for _ in [0:fuel] do
    let (t1', cv1, ptr') := TOK_GET ptr {}
    ptr := ptr'
    let mut t1 := t1'
    if t1 == 0 then break
    for _ in [0:fuel] do
      let mut n := 0
      let mut t2 : Int32 := 0
      for _ in [0:fuel] do
        t2 := ptr.get n
        if t2 != 32 then break
        n := n + 1
      if t2 != TOK_PPJOIN then break
      ptr := { ptr with pos := ptr.pos + n }
      for _ in [0:fuel] do
        ptr := { ptr with pos := ptr.pos + 1 }
        t2 := ptr.get
        if !(t2 == 32 || t2 == TOK_PPJOIN) then break
      let (t2', cv2, ptr') := TOK_GET ptr {}
      ptr := ptr'
      if t2' == TOK_PLCHLDR then continue
      if t1 != TOK_PLCHLDR then
        let s1 ← get_tok_str t1 (some cv1)
        modify fun s => { s with tokcstr := s.tokcstr ++ s1 }
        t1 := TOK_PLCHLDR
      let s2 ← get_tok_str t2' (some cv2)
      modify fun s => { s with tokcstr := s.tokcstr ++ s2 }
    let cs := (← get).tokcstr
    if cs.size > 0 then
      let cs := cs.push 0
      tcc_open_bf ":paste:".toUTF8 (some cs)
      modify fun s => { s with tok_flags := 0 }
      let mut n := 0
      for _ in [0:fuel] do
        next_nomacro R
        let st ← get
        ms1 := tok_str_add2 ms1 st.tok st.tokc
        let f ← curFile
        if f.buf.get! f.buf_ptr == 0 then break
        ms1 := tok_str_add ms1 32
        let l := f.buf_ptr - f.bufStart
        let rest := Bytes.cstrAt f.buf f.buf_ptr
        tcc_warning R (cfmt "pasting \"%.*s\" and \"%s\" does not give a valid preprocessing token"
          [.i (l - n : Nat), .s (f.buf.extract (f.bufStart + n) f.buf.size), .s rest])
        n := l
      tcc_close
      modify fun s => { s with tokcstr := ByteArray.empty }
    if t1 != TOK_PLCHLDR then
      ms1 := tok_str_add2 ms1 t1 cv1
  ms1 := tok_str_add ms1 0
  return ms1.str.extract 0 ms1.len.toNat'

/-- `peek_file(ws_str)` -/
def peek_file (R : Rec) (ws0 : Option TokenString) : M (Int32 × Option TokenString) := do
  let fuel ← fuelM
  let mut ws := ws0
  let mut p := (← bufPtr) - 1
  for _ in [0:fuel] do
    let (c0, p') ← peekc R p
    p := p'
    let mut c := c0
    if c == 47 then
      let (c1, p'') ← peekc R p
      p := p''
      if c1 == 42 then
        p ← parse_comment R p
        p := p - 1; c := 32
      else if c1 == 47 then
        p ← parse_line_comment R p
        p := p - 1; c := 32
      else
        p := p - 1; wrb p 47
        setBufPtr p
        return (47, ws)
    else if c == 32 || c == 9 then
      pure ()
    else if c == 12 || c == 11 || c == 13 then
      continue
    else if c == 10 then
      modFile fun f => { f with line_num := f.line_num + 1 }
      modify fun s => { s with tok_flags := s.tok_flags ||| TOK_FLAG_BOL }
    else
      setBufPtr p
      return (c, ws)
    ws := ws.map (tok_str_add · c)
  throw (.fuel "peek_file")

/-- `next_argstream(nested_list, ws_str)` -/
def next_argstream (R : Rec) (nested0 : Option SymId) (ws0 : Option TokenString) :
    M (Int32 × Option SymId × Option TokenString) := do
  let fuel ← fuelM
  let mut nested := nested0
  for _ in [0:fuel] do
    let st ← get
    match st.macro_ptr with
    | none => break
    | some mp =>
      let mut m := mp
      let mut found : Option Int32 := none
      for _ in [0:fuel] do
        let t := m.get
        if t == 0 then break
        if ws0.isSome then
          if t != 32 then found := some t; break
          m := { m with pos := m.pos + 1 }
        else
          let (tk, cv, mp') := TOK_GET mp st.tokc
          modify fun s => { s with tok := tk, tokc := cv, macro_ptr := some mp' }
          return (tk, nested, ws0)
      match found with
      | some t => return (t, nested, ws0)
      | none =>
        end_macro
        match nested with
        | some sa =>
          if sa != 0 then
            let pr := (← getSym sa).prev
            nested := some pr
            sym_free sa
        | none => pure ()
  match ws0 with
  | some _ =>
    let (c, ws) ← peek_file R ws0
    return (c, nested, ws)
  | none =>
    next_nomacro R
    let tk ← getTok
    if tk == 9 || tk == TOK_LINEFEED then setTok 32
    return (← getTok, nested, ws0)

/-- `macro_subst_tok(tok_str, nested_list, s)` -/
def macro_subst_tok (R : Rec) (tok_str0 : TokenString) (nested0 : Option SymId) (s : SymId) :
    M (TokenString × Option SymId × Int32) := do
  let fuel ← fuelM
  let sx ← getSym s
  let v := sx.v
  let mut tok_str := tok_str0
  let mut nested := nested0
  match sx.d with
  | some d =>
    let mut mstr : TokArr := d
    if sx.type.t &&& MACRO_FUNC != 0 then
      let saved_parse_flags := (← get).parse_flags
      modify fun s => { s with parse_flags := s.parse_flags ||| PARSE_FLAG_SPACES ||| PARSE_FLAG_LINEFEED ||| PARSE_FLAG_ACCEPT_STRAYS }
      let (t, nested', ws) ← next_argstream R nested (some {})
      nested := nested'
      if t != 40 then
        modify fun s => { s with parse_flags := saved_parse_flags }
        tok_str := tok_str_add2_spc tok_str v {}
        if saved_parse_flags &&& PARSE_FLAG_SPACES != 0 then
          let ws := ws.getD {}
          for i in [0:ws.len.toNat'] do
            tok_str := tok_str_add tok_str ws.str[i]!
        return (tok_str, nested, 0)
      -- argument macro
      let mut args : SymId := 0
      let mut sa := sx.next
      let mut i : Int := 2
      let mut tt : Int32 := 0
      for _ in [0:fuel] do
        for _ in [0:fuel] do
          let (t', n', _) ← next_argstream R nested none
          tt := t'; nested := n'
          i := if tt == 32 then i else i - 1
          if !(tt == 32 || i != 0) then break
        if sa == 0 then
          if tt == 41 then break
          tcc_error R (cfmt "macro '%s' used with too many args" [.s (← get_tok_str v none)])
        let mut emptyArg := true
        let mut doneArgs := false
        for _ in [0:fuel] do
          if !emptyArg then break
          emptyArg := false
          -- empty_arg:
          let mut str : TokenString := {}
          let mut parlevel : Int := 0
          let sat := (← getSym sa).type.t
          for _ in [0:fuel] do
            if !(parlevel > 0 || (tt != 41 && (tt != 44 || sat != 0))) then break
            if tt == TOK_EOF then
              tcc_error R (cfmt "EOF in invocation of macro '%s'" [.s (← get_tok_str v none)])
            if tt == 40 then parlevel := parlevel + 1
            if tt == 41 then parlevel := parlevel - 1
            if tt == 32 then str := { str with need_spc := str.need_spc ||| 1 }
            else str := tok_str_add2_spc str tt (← get).tokc
            let (t', n', _) ← next_argstream R nested none
            tt := t'; nested := n'
          str := tok_str_add str TOK_EOF
          let sav := (← getSym sa).v
          let sa1 ← sym_push2 args (sav &&& ~~~SYM_FIELD) sat 0
          modSym sa1 fun x => { x with d := some (str.str.extract 0 str.len.toNat') }
          args := sa1
          sa := (← getSym sa).next
          if tt == 41 then
            if sa == 0 then doneArgs := true
            else if (← getSym sa).type.t != 0 && (← get).gnu_ext != 0 then
              emptyArg := true
            else
              tcc_error R (cfmt "macro '%s' used with too few args" [.s (← get_tok_str v none)])
        if doneArgs then break
        i := 1
      -- now subst each arg
      let (ms, nested') ← macro_arg_subst R nested { arr := mstr } args
      nested := nested'
      mstr := ms
      -- free memory
      let mut a := args
      for _ in [0:fuel] do
        if a == 0 then break
        let a1 := (← getSym a).prev
        sym_free a
        a := a1
      modify fun s => { s with parse_flags := saved_parse_flags }
    -- process '##'s
    let jstr ← if sx.type.t &&& MACRO_JOIN != 0 then macro_twosharps R { arr := mstr } else pure mstr
    let top := nested.getD 0
    let sa ← sym_push2 top v 0 0
    let (ts, nested', ret) ← R.macro_subst tok_str (some sa) { arr := jstr }
    tok_str := ts
    nested := nested'
    -- pop nested defined symbol
    if nested == some sa then
      let pr := (← getSym sa).prev
      nested := some pr
      sym_free sa
    return (tok_str, nested, ret)
  | none =>
    -- special macros
    let mut t : Int32 := 0
    let mut cstrval : ByteArray := ByteArray.empty
    let mut add := true
    if v == Tok.TOK___LINE__ || v == Tok.TOK___COUNTER__ then
      let n ← if v == Tok.TOK___LINE__ then pure (← curFile).line_num
              else modifyGet fun s => (s.pp_counter, { s with pp_counter := s.pp_counter + 1 })
      cstrval := cfmt "%d" [.i n.toInt]
      t := TOK_PPNUM
    else if v == Tok.TOK___FILE__ then
      cstrval := (← curFile).filename
      t := TOK_STR
    else if v == Tok.TOK___DATE__ then
      -- the image's clock is 0 (htif.c): 1970-01-01 00:00:00
      cstrval := "Jan  1 1970".toUTF8
      t := TOK_STR
    else if v == Tok.TOK___TIME__ then
      cstrval := "00:00:00".toUTF8
      t := TOK_STR
    else add := false
    if add then
      let cv := (default : CValue).setStr (cstrval.push 0) (Int32.ofNat (cstrval.size + 1))
      tok_str := tok_str_add2_spc tok_str t cv
    return (tok_str, nested, 0)

/-- `_Pragma`: expand the operand, destringize only quote/backslash,
then run the existing pragma parser on isolated file/token/macro buffers. -/
def pragma_operator (R : Rec) : M Unit := do
  let flags := (← get).parse_flags
  modify fun s => { s with parse_flags := s.parse_flags &&& ~~~(PARSE_FLAG_TOK_STR ||| PARSE_FLAG_SPACES ||| PARSE_FLAG_LINEFEED) }
  R.next
  if (← getTok) != 40 then expect R "("
  R.next
  if (← getTok) != TOK_PPSTR then expect R "string literal in _Pragma"
  let text := (← get).tokc.sdata
  let mut i := if text.get! 0 == 76 then 1 else 0
  if text.get! i != 34 then expect R "ordinary or wide string literal in _Pragma"
  i := i + 1
  let mut out := ByteArray.empty
  let fuel ← fuelM
  for _ in [0:fuel] do
    let c := text.get! i
    if c == 0 || c == 34 then break
    if c == 92 && (text.get! (i+1) == 92 || text.get! (i+1) == 34) then i := i + 1
    out := out.push (text.get! i)
    i := i + 1
  out := out.push 10
  R.next
  if (← getTok) != 41 then expect R ")"
  let saved ← get
  modify fun s => { s with
    macro_stack := [], macro_ptr := none,
    tokstr_buf := {}, unget_buf := {}, include_stack := #[] }
  let caller ← curFile
  tcc_open_bf caller.filename (some out)
  modFile fun f => { f with true_filename := caller.true_filename, line_num := caller.line_num }
  modify fun s => { s with parse_flags := PARSE_FLAG_PREPROCESS ||| PARSE_FLAG_TOK_NUM ||| PARSE_FLAG_TOK_STR ||| PARSE_FLAG_LINEFEED }
  let _ ← pragma_parse R
  let mut result : TokenString := {}
  for _ in [0:fuel] do
    let tk ← getTok
    if tk == TOK_LINEFEED || tk == TOK_EOF then break
    if (← get).output_type == TCC_OUTPUT_PREPROCESS then result ← tok_str_add_tok result
    R.next
  if (← get).output_type == TCC_OUTPUT_PREPROCESS then
    for _ in [0:fuel] do
      if (← getTok) == TOK_EOF then break
      result ← tok_str_add_tok result
      R.next
  for _ in [0:fuel] do
    if (← get).macro_stack.isEmpty then break
    end_macro
  tcc_close
  modify fun s => { s with
    macro_stack := saved.macro_stack, macro_ptr := saved.macro_ptr,
    tokstr_buf := saved.tokstr_buf, unget_buf := saved.unget_buf,
    include_stack := saved.include_stack, tok_flags := saved.tok_flags, parse_flags := flags }
  if result.len != 0 then
    result := tok_str_add result 0
    let _ ← begin_macro .heap result.str 0 1

/-- Phase 6 selects the common prefix before decoding each literal.
Separate spellings preserve escape boundaries, e.g. "\\x12" "3". -/
def parse_string_concat (R : Rec) : M Unit := do
  let flags := (← get).parse_flags
  modify fun s => { s with parse_flags := s.parse_flags &&& ~~~PARSE_FLAG_TOK_STR }
  let mut literals : Array ByteArray := #[]
  let mut literalPrefix : UInt8 := 0
  let fuel ← fuelM
  for _ in [0:fuel] do
    let st ← get
    if st.tok != TOK_PPSTR && st.tok != TOK_STR && st.tok != TOK_LSTR then break
    let spelling ← get_tok_str st.tok (some st.tokc)
    let c := spelling.get! 0
    let mut p : UInt8 := if c == 76 || c == 117 || c == 85 then c else 0
    if p == 117 && spelling.get! 1 == 56 then p := 56
    if p != 0 && literalPrefix != 0 && p != literalPrefix then tcc_error_s R "incompatible string literal prefixes"
    if p != 0 then literalPrefix := p
    literals := literals.push spelling
    R.next
    if (← getTok) == TOK_PPSTR then
      let sp := (← get).tokc.sdata
      let mut i := 0
      for _ in [0:sp.size] do
        let c := sp.get! i
        if c == 76 || c == 117 || c == 85 || c == 56 then i := i + 1 else break
      if sp.get! i != 34 then break
  unget_tok 0
  modify fun s => { s with parse_flags := flags }
  let mut out := ByteArray.empty
  for spelling in literals do
    let mut i := 0
    if spelling.get! 0 == 76 || spelling.get! 0 == 117 || spelling.get! 0 == 85 then
      i := 1
      if spelling.get! 0 == 117 && spelling.get! 1 == 56 then i := 2
    let mut sp := ByteArray.empty
    if literalPrefix == 56 then sp := "u8".toUTF8
    else if literalPrefix != 0 then sp := sp.push literalPrefix
    sp := sp ++ spelling.extract i spelling.size
    parse_string R sp sp.size
    let st ← get
    if out.size != 0 then out := out.extract 0 (out.size - (TOK_STR_SIZE st.tok).toNat')
    out := out ++ st.tokc.sdata.extract 0 st.tokc.strSize.toNat'
  modify fun s => { s with tokcstr := out, tokc := s.tokc.setStr out (Int32.ofNat out.size) }

/-- `macro_subst(tok_str, nested_list, macro_str)` -/
def macro_subst (R : Rec) (tok_str0 : TokenString) (nested0 : Option SymId) (macro_str0 : TokPtr) :
    M (TokenString × Option SymId × Int32) := do
  let fuel ← fuelM
  let mut tok_str := tok_str0
  let mut nested := nested0
  let mut ms := macro_str0
  let mut nosubst : Int32 := 0
  for _ in [0:fuel] do
    let (t0, cval, ms') := TOK_GET ms {}
    ms := ms'
    let mut t := t0
    if t == 0 || t == TOK_EOF then break
    let mut noSubst := false
    if t == (← get).tok_pragma_op && nosubst == 0 && (← get).parse_flags &&& PARSE_FLAG_PREPROCESS != 0 then
      let id ← begin_macro .heap ms.arr ms.pos 2
      pragma_operator R
      for _ in [0:fuel] do
        match (← get).macro_stack with
        | fr :: _ => if fr.id == id then break
        | [] => throw (.internal "_Pragma lost rescan frame")
        match (← get).macro_ptr with
        | some mp =>
          let (tk, cv, mp') := TOK_GET mp {}
          modify fun s => { s with macro_ptr := some mp' }
          if tk == 0 then end_macro
          else tok_str := tok_str_add2_spc tok_str tk cv
        | none => throw (.internal "_Pragma lost macro pointer")
      match (← get).macro_ptr with
      | some mp => ms := mp
      | none => throw (.internal "_Pragma lost rescan pointer")
      end_macro
    else if t ≥ TOK_IDENT then
      let s ← define_find t
      if s == 0 || nosubst != 0 then
        noSubst := true
      else if (← sym_find2 (nested.getD 0) (← canonical_ident t)) != 0 then
        t := t ||| SYM_FIELD
        noSubst := true
      else
        let id ← begin_macro .heap ms.arr ms.pos 2
        let (ts, nested', ns) ← macro_subst_tok R tok_str nested s
        tok_str := ts; nested := nested'; nosubst := ns
        match (← get).macro_stack with
        | fr :: _ =>
          if fr.id != id then break
        | [] => break
        match (← get).macro_ptr with
        | some mp => ms := mp
        | none => pure ()
        end_macro
    else if t == 32 then
      if (← get).parse_flags &&& PARSE_FLAG_SPACES != 0 then
        tok_str := { tok_str with need_spc := tok_str.need_spc ||| 1 }
    else
      noSubst := true
    if noSubst then
      tok_str := tok_str_add2_spc tok_str t cval
      if nosubst != 0 && t != 40 then nosubst := 0
      if t == Tok.TOK_DEFINED && (← get).pp_expr != 0 then nosubst := 1
  return (tok_str, nested, nosubst)

/-- `next()`, including patch 0003's common convert path. Macro tokens
retain spelling until conversion; `_Pragma` executes before the next token. -/
def next (R : Rec) : M Unit := do
  let fuel ← fuelM
  for _ in [0:fuel] do
    let st ← get
    match st.macro_ptr with
    | some mp =>
      let t := mp.get
      if TOK_HAS_VALUE t then
        let (tk, cv, mp') := tok_get mp st.tokc
        modify fun s => { s with tok := tk, tokc := cv, macro_ptr := some mp' }
        if t == TOK_LINENUM then
          modFile fun f => { f with line_num := cv.i.toUInt32.toInt32 }
          continue
      else if t == 0 then
        end_macro
        continue
      else if t == TOK_EOF then
        setTok t
        return
      else
        modify fun s => { s with macro_ptr := some { mp with pos := mp.pos + 1 } }
        let t := t &&& ~~~SYM_FIELD
        if t == 92 && (← get).parse_flags &&& PARSE_FLAG_ACCEPT_STRAYS == 0 then
          tcc_error_s R "stray '\\' in program"
        setTok t
    | none =>
      next_nomacro R
      let t ← getTok
      if t ≥ TOK_IDENT && ((← get).parse_flags &&& PARSE_FLAG_PREPROCESS != 0) then
        let s ← define_find t
        if s != 0 then
          let buf := (← get).tokstr_buf
          let (ts, _, _) ← macro_subst_tok R buf (some 0) s
          let ts := tok_str_add ts 0
          modify fun st => { st with tokstr_buf := ts }
          let _ ← begin_macro .tokstrBuf ts.str 0 0
          continue
    let pf := (← get).parse_flags
    if pf &&& (PARSE_FLAG_TOK_STR ||| PARSE_FLAG_TOK_NUM) != 0 then
      setTok (← canonical_ident (← getTok))
    let t ← getTok
    if t == (← get).tok_pragma_op && pf &&& PARSE_FLAG_PREPROCESS != 0 then
      pragma_operator R
      continue
    if pf &&& PARSE_FLAG_TOK_STR != 0 then
      if t == TOK_DG_LBRACK then setTok 91
      if t == TOK_DG_RBRACK then setTok 93
      if t == TOK_DG_LBRACE then setTok 123
      if t == TOK_DG_RBRACE then setTok 125
      if t == TOK_DG_SHARP then setTok 35
      if t == TOK_DG_PASTE then setTok TOK_TWOSHARPS
    if t == TOK_PPNUM then
      if pf &&& PARSE_FLAG_TOK_NUM != 0 then parse_number R (← get).tokc.sdata
    else if t == TOK_STR && pf &&& PARSE_FLAG_TOK_STR != 0 then
      parse_string_concat R
    else if t == TOK_PPSTR && pf &&& PARSE_FLAG_TOK_STR != 0 then
      let tc := (← get).tokc
      let mut i := 0
      for _ in [0:tc.sdata.size] do
        let c := tc.sdata.get! i
        if c == 76 || c == 117 || c == 85 || c == 56 then i := i + 1 else break
      if tc.sdata.get! i == 34 then parse_string_concat R
      else parse_string R tc.sdata (tc.strSize - 1).toNat'
    return
  throw (.fuel "next")

/-- `pp_error(cs)`: the text error1 prints for a bad #if expression -/
def pp_error (R : Rec) : M ByteArray := do
  let mut cs := cfmt "bad preprocessor expression: #%s" [.s (← get_tok_str (← get).pp_expr none)]
  -- macro_ptr = macro_stack->str (the expression's token string, from its start)
  -- the frame's string is the one begin_macro'ed by expr_preprocess
  modify fun s => match s.macro_stack with
    | fr :: _ => { s with macro_ptr := some fr.str }
    | [] => s
  let fuel ← fuelM
  for _ in [0:fuel] do
    next R
    let st ← get
    if st.tok == TOK_EOF then break
    cs := cs ++ cfmt " %s" [.s (← get_tok_str st.tok (some st.tokc))]
  return cs

/-! ## setup -/

def target_machine_defs : List String := ["__riscv", "__riscv_xlen 64", "__riscv_float_abi_soft"]
def target_os_defs : List String := ["__linux__", "__linux", "__unix__", "__unix"]
def bm_type_defs : List String := [
  "__INT8_TYPE__ signed char", "__INT16_TYPE__ short int", "__INT32_TYPE__ int",
  "__UINT8_TYPE__ unsigned char", "__UINT16_TYPE__ short unsigned int", "__UINT32_TYPE__ unsigned int",
  "__UINT64_TYPE__ long unsigned int", "__INT_LEAST8_TYPE__ signed char", "__INT_LEAST16_TYPE__ short int",
  "__INT_LEAST32_TYPE__ int", "__INT_LEAST64_TYPE__ long int", "__UINT_LEAST8_TYPE__ unsigned char",
  "__UINT_LEAST16_TYPE__ short unsigned int", "__UINT_LEAST32_TYPE__ unsigned int",
  "__UINT_LEAST64_TYPE__ long unsigned int", "__INT_FAST8_TYPE__ int", "__INT_FAST16_TYPE__ int",
  "__INT_FAST32_TYPE__ int", "__INT_FAST64_TYPE__ long int", "__UINT_FAST8_TYPE__ unsigned int",
  "__UINT_FAST16_TYPE__ unsigned int", "__UINT_FAST32_TYPE__ unsigned int",
  "__UINT_FAST64_TYPE__ long unsigned int", "__INTMAX_TYPE__ long int", "__UINTMAX_TYPE__ long unsigned int"]

/-- `putdef(cs, p)` -/
def putdef (p : String) : ByteArray :=
  cfmt "#define %s%s\n" [.s p.toUTF8, .s (if p.contains ' ' then "" else " 1").toUTF8]

/-- `tcc_predefs(s1, cs, is_asm)` -/
def tcc_predefs (is_asm : Bool) : M ByteArray := do
  let s ← get
  let mut cs := cfmt "#define __TINYC__ 9%.2s\n" [.s "28rc".toUTF8]
  for d in target_machine_defs do cs := cs ++ putdef d
  for d in target_os_defs do cs := cs ++ putdef d
  for d in bm_type_defs do cs := cs ++ putdef d
  if is_asm then cs := cs ++ putdef "__ASSEMBLER__"
  if s.output_type == TCC_OUTPUT_PREPROCESS then cs := cs ++ putdef "__TCC_PP__"
  if s.output_type == TCC_OUTPUT_MEMORY then cs := cs ++ putdef "__TCC_RUN__"
  if s.char_is_unsigned != 0 then cs := cs ++ putdef "__CHAR_UNSIGNED__"
  if s.optimize > 0 then cs := cs ++ putdef "__OPTIMIZE__"
  if s.option_pthread != 0 then cs := cs ++ putdef "_REENTRANT"
  if s.leading_underscore != 0 then cs := cs ++ putdef "__leading_underscore"
  cs := cs ++ cfmt "#define __SIZEOF_POINTER__ %d\n" [.i 8]
  cs := cs ++ cfmt "#define __SIZEOF_LONG__ %d\n" [.i 8]
  if !is_asm then
    cs := cs ++ putdef "__STDC__"
    cs := cs ++ cfmt "#define __STDC_HOSTED__ %d\n" [.i (if s.nostdlib != 0 then 0 else 1)]
    cs := cs ++ cfmt "#define __STDC_VERSION__ %dL\n" [.i s.cversion.toInt]
    cs := cs ++ "#include <tccdefs.h>\n".toUTF8
  cs := cs ++ cfmt "#define __BASE_FILE__ \"%s\"\n" [.s (← curFile).filename]
  return cs

/-- `tccpp_new(s)` -/
def tccpp_new (R : Rec) : M Unit := do
  let mut tbl : Array UInt8 := Array.replicate 257 0
  for i in [0:129] do
    let c : Int32 := Int32.ofNat i - 1
    tbl := tbl.set! i (if isSpace c then IS_SPC else if isId c then IS_ID else if isNum c then IS_NUM else 0)
  for i in [129:257] do tbl := tbl.set! i IS_ID
  modify fun s => { s with
    isidnum_table := tbl, hash_ident := {}, cached_includes_hash := Array.replicate 32 0,
    tokcstr := ByteArray.empty, unget_buf := {}, tokstr_buf := {}, tok_ident := TOK_IDENT, table_ident := #[] }
  for k in Tok.keywords do
    let _ ← tok_alloc R k.toUTF8
  let pragmaTok ← tok_alloc R "_Pragma".toUTF8
  modify fun s => { s with tok_pragma_op := pragmaTok.tok }
  define_push R Tok.TOK___LINE__ MACRO_OBJ none 0
  define_push R Tok.TOK___FILE__ MACRO_OBJ none 0
  define_push R Tok.TOK___DATE__ MACRO_OBJ none 0
  define_push R Tok.TOK___TIME__ MACRO_OBJ none 0
  define_push R Tok.TOK___COUNTER__ MACRO_OBJ none 0

/-- `preprocess_start(s1, filetype)` -/
def preprocess_start (R : Rec) (filetype : Int32) : M Unit := do
  let is_asm := filetype &&& (AFF_TYPE_ASM ||| AFF_TYPE_ASMPP) != 0
  tccpp_new R
  modify fun s => { s with
    include_stack := #[], ifdef_stack_ptr := 0, pp_expr := 0, pp_counter := 0,
    pp_debug_tok := 0, pp_debug_symv := 0, pack_stack := s.pack_stack.set! 0 0, pack_stack_ptr := 0 }
  modFile fun f => { f with ifdef_stack_ptr := 0 }
  let _ ← set_idnum 36 (if (← get).dollars_in_identifiers != 0 then IS_ID else 0)
  let _ ← set_idnum 46 (if is_asm then IS_ID else 0)
  if filetype &&& AFF_TYPE_ASM == 0 then
    let mut cstr ← tcc_predefs is_asm
    let s ← get
    if s.cmdline_defs.size > 0 then cstr := cstr ++ s.cmdline_defs
    if s.cmdline_incl.size > 0 then cstr := cstr ++ s.cmdline_incl
    let fid := (← curFile).id
    modify fun s => { s with include_stack := s.include_stack.push fid }
    tcc_open_bf "<command line>".toUTF8 (some cstr)
  modify fun s => { s with parse_flags := if is_asm then PARSE_FLAG_ASM_FILE else 0 }

/-- `preprocess_end(s1)` -/
def preprocess_end : M Unit := do
  let fuel ← fuelM
  for _ in [0:fuel] do
    if (← get).macro_stack.isEmpty then break
    end_macro
  modify fun s => { s with macro_ptr := none }
  for _ in [0:fuel] do
    if (← get).file.isEmpty then break
    tcc_close
  modify fun s => { s with cached_includes := #[] }

/-- `skip(c)` -/
def skip (R : Rec) (c : Int32) : M Unit := do
  let st ← get
  if st.tok != c then
    let tmp := pstrcpy 40 (← get_tok_str c (some st.tokc))
    tcc_error R (cfmt "'%s' expected (got '%s')" [.s tmp, .s (← get_tok_str st.tok (some st.tokc))])
  R.next

end Tcc.Model
