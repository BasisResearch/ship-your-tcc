/-
Translation phase 4 (C11 §5.1.1.2p1): "Preprocessing directives are
executed, macro invocations are expanded, and _Pragma unary operator
expressions are executed. … A #include preprocessing directive causes the
named header or source file to be processed from phase 1 through phase 4,
recursively. All preprocessing directives are then deleted."

Structure of a file (§6.10p1–2): a directive line starts with `#` as the
first token of a line (after phase 3's comments became white space) and ends
at the next new-line. Everything between directive lines is text; its macro
invocations may span lines (§6.10.3p10) but not directives (§6.10.3p11
makes a directive inside an argument list undefined; the specification
rejects it: the invocation is unterminated). So phase 4 alternates between
one directive line and one maximal run of text lines, which is expanded as
one token sequence ("as if they formed the rest of the preprocessing file").

Directives (§6.10.1–6.10.7): conditional inclusion (with the skipped-group
rule of §6.10.1p6), `#include` (§6.10.2), `#define`/`#undef` (§6.10.3,
§6.10.3.5), `#line` (§6.10.4), `#error` (§6.10.5), `#pragma` (§6.10.6), the
null directive (§6.10.7); `# non-directive` is ignored. Extensions per
`PreCfg.ext`.
-/
import Spec.Pre.IfExpr

namespace CSem.Pre

/-- A file being processed. `path`: the path it was opened with (its
directory is searched first for `#include "…"`, §6.10.2p3, and it keys
`#pragma once`, after resolution); `presumed`/`lineDelta`: `#line` state (§6.10.4: presumed
name and presumed line − physical line); `foundAt`: index of the search
place it was found in (for `#include_next`); `level`: include nesting
(main file 1). -/
structure FileCtx where
  path : Bytes
  presumed : Bytes
  lineDelta : Int
  foundAt : Option Nat
  level : Nat
  isMain : Bool

/-- One level of conditional inclusion (§6.10.1). `active`: the current group
is processed; `taken`: some group of this `#if` has been processed;
`elseSeen`: `#else` was seen; `outer`: the enclosing group is processed. -/
structure CondFrame where
  active : Bool
  taken : Bool
  elseSeen : Bool
  outer : Bool

/-- The state threaded through phase 4. -/
structure PState where
  macros : Macros
  counter : Nat
  /-- the files that had `#pragma once`, by their resolved path (the file,
  not the spelling it was named by) -/
  once : List (List Bytes)
  pushed : List (Bytes × Option MacroDef)
  /-- the output so far, reversed -/
  out : List PPToken

def fileErr (fc : FileCtx) (k : ErrKind) (line : Nat) (msg : String) : PreError :=
  ⟨k, fc.presumed, line, msg⟩

/-- Split off the tokens of the current line (up to the next new-line), and
the physical line of that new-line. -/
def takeLine : List LItem → List Tok → (List Tok × Nat × List LItem)
  | [], acc => (acc.reverse, 0, [])
  | .nl l :: rest, acc => (acc.reverse, l, rest)
  | .tok t :: rest, acc => takeLine rest (t :: acc)

/-- Split off a maximal run of text lines: everything up to a `#` that
starts a line. `bol`: at the start of a line. -/
def takeText : List LItem → Bool → List Tok → (List Tok × List LItem)
  | [], _, acc => (acc.reverse, [])
  | .nl l :: rest, _, acc => takeText rest true acc
  | .tok t :: rest, bol, acc =>
    if bol ∧ isHashTok t.kind t.spell then (acc.reverse, .tok t :: rest)
    else takeText rest false (t :: acc)

/-- Skip text lines of a skipped group (up to a `#` that starts a line). -/
def skipText : List LItem → Bool → List LItem
  | [], _ => []
  | .nl _ :: rest, _ => skipText rest true
  | .tok t :: rest, bol =>
    if bol ∧ isHashTok t.kind t.spell then .tok t :: rest else skipText rest false

/-- The dynamic predefined macros as table entries. -/
def builtinDef (b : Builtin) : MacroDef := ⟨none, false, [], [], some b⟩

/-- The initial macro table: `__FILE__`, `__LINE__`, `__DATE__`, `__TIME__`
(§6.10.8.1) and `__COUNTER__` if enabled; the rest come from `cfg.predefs`. -/
def initMacros (cfg : PreCfg) : Macros :=
  let m : Macros := ∅
  let m := m.insert (bs "__LINE__") (builtinDef .line)
  let m := m.insert (bs "__FILE__") (builtinDef .file)
  let m := m.insert (bs "__DATE__") (builtinDef .date)
  let m := m.insert (bs "__TIME__") (builtinDef .time)
  if cfg.ext.counter then m.insert (bs "__COUNTER__") (builtinDef .counter) else m

/-! ### `#define` (§6.10.3) -/

/-- Parse the parameter list after `(`: identifiers separated by commas, an
optional final `...` (or GNU `name...`). Returns names (the variadic one as
`__VA_ARGS__` or its GNU name), variadic flag, rest. -/
def parseParams (cfg : PreCfg) : List Tok → List Bytes → Option (List Bytes × Bool × List Tok)
  | [], _ => none
  | t :: rest, acc =>
    if isPunct t ")" ∧ acc = [] then some ([], false, rest)
    else if isPunct t "..." then
      match rest with
      | r :: rest' => if isPunct r ")" then some ((bs "__VA_ARGS__" :: acc).reverse, true, rest') else none
      | [] => none
    else if t.kind = .ident then
      match rest with
      | r :: rest' =>
        if isPunct r ")" then some ((t.spell :: acc).reverse, false, rest')
        else if isPunct r "," then parseParams cfg rest' (t.spell :: acc)
        else if cfg.ext.namedVariadic ∧ isPunct r "..." then
          match rest' with
          | r2 :: rest'' => if isPunct r2 ")" then some ((t.spell :: acc).reverse, true, rest'') else none
          | [] => none
        else none
      | [] => none
    else none

/-- Second pass over a replacement list: a parameter next to a `##` is not
"plain" (§6.10.3.1: it is replaced by the raw argument); with
`Ext.commaPaste`, `,` `##` variadic-parameter becomes `commaPaste`. -/
def markPlain (commaPaste : Bool) (vaIdx : Option Nat) :
    List RItem → Option RItem → List RItem → List RItem
  | [], _, acc => acc.reverse
  | .param i ws _ :: rest, prev, acc =>
    let nextPaste : Bool := match rest.head? with | some .paste => true | _ => false
    let prevPaste : Bool := match prev with | some .paste => true | _ => false
    let it' := RItem.param i ws (!(nextPaste || prevPaste))
    markPlain commaPaste vaIdx rest (some it') (it' :: acc)
  | .paste :: .param i ws p :: rest, prev, .tok c :: acc =>
    if commaPaste ∧ isPunct c "," ∧ vaIdx = some i then
      markPlain commaPaste vaIdx rest (some (.commaPaste c i)) (.commaPaste c i :: acc)
    else markPlain commaPaste vaIdx (.param i ws p :: rest) (some .paste) (.paste :: .tok c :: acc)
  | it :: rest, _, acc => markPlain commaPaste vaIdx rest (some it) (it :: acc)
termination_by l => l.length

/-- Resolve a replacement list into `RItem`s (§6.10.3.1–3), checking the
constraints: `#` is followed by a parameter (§6.10.3.2p1, function-like
only), `##` is not at either end (§6.10.3.3p1). -/
def resolveBody (cfg : PreCfg) (params : Option (List Bytes)) (variadic : Bool)
    (body : List Tok) : Except String (List RItem) := do
  let ps := params.getD []
  let idx (t : Tok) : Option Nat := if t.kind = .ident then ps.idxOf? t.spell else none
  let isFunc := params.isSome
  let isHash (t : Tok) : Bool := isFunc ∧ (isPunct t "#" ∨ isPunct t "%:")
  let isPaste (t : Tok) : Bool := isPunct t "##" ∨ isPunct t "%:%:"
  match body.head?, body.getLast? with
  | some f, _ => if isPaste f then throw "'##' at the start of a replacement list (§6.10.3.3p1)"
  | _, _ => pure ()
  match body.getLast? with
  | some l => if isPaste l then throw "'##' at the end of a replacement list (§6.10.3.3p1)"
  | none => pure ()
  -- first pass: `#` param, `##`, tokens
  let rec pass1 : List Tok → List RItem → Except String (List RItem)
    | [], acc => pure acc.reverse
    | t :: rest, acc =>
      if isHash t then
        match rest with
        | p :: rest' =>
          match idx p with
          | some i => pass1 rest' (.str i t.ws :: acc)
          | none => throw "'#' is not followed by a macro parameter (§6.10.3.2p1)"
        | [] => throw "'#' is not followed by a macro parameter (§6.10.3.2p1)"
      else if isPaste t then pass1 rest (.paste :: acc)
      else
        match idx t with
        | some i => pass1 rest (.param i t.ws true :: acc)
        | none => pass1 rest (.tok t :: acc)
  let items ← pass1 body []
  -- second pass: parameters next to `##` are not "plain"; GNU `, ## __VA_ARGS__`
  let vaIdx := if variadic then some (ps.length - 1) else none
  pure (markPlain cfg.ext.commaPaste vaIdx items none [])

/-- §6.10.3p2: two replacement lists are identical (same tokens, spellings
and white-space separation; leading/trailing white space excluded). -/
def sameBody : List Tok → List Tok → Bool
  | [], [] => true
  | a :: as, b :: bs' =>
    a.kind = b.kind ∧ a.spell = b.spell ∧ sameBody as bs' ∧
      (as.isEmpty ∨ (as.head!.ws = bs'.head!.ws))
  | _, _ => false

def sameDef (a b : MacroDef) : Bool :=
  a.params = b.params ∧ a.variadic = b.variadic ∧ a.builtin = b.builtin ∧ sameBody a.body b.body

/-- Execute `#define` (the tokens after `define`). -/
def doDefine (cfg : PreCfg) (fc : FileCtx) (line : Nat) (st : PState) (ts : List Tok) :
    Except PreError PState := do
  let err (m : String) := fileErr fc .macro line m
  match ts with
  | [] => throw (err "#define needs a macro name (§6.10.3)")
  | n :: rest =>
    if n.kind ≠ .ident then throw (err "macro name must be an identifier (§6.10.3)")
    if n.spell = bs "defined" then throw (err "'defined' cannot be defined (§6.10.8p2)")
    -- function-like iff `(` follows the name with no white space (§6.10.3p3 / grammar)
    let (params, variadic, body) ←
      match rest with
      | lp :: rest' =>
        if isPunct lp "(" ∧ ¬lp.ws then
          match parseParams cfg rest' [] with
          | some (ps, v, b) => pure (some ps, v, b)
          | none => throw (err "malformed macro parameter list (§6.10.3)")
        else pure (none, false, rest)
      | [] => pure (none, false, [])
    -- §6.10.3p3: white space is required after an object-like macro name
    -- (a constraint for some characters; the specification accepts it, as tcc).
    let ps := params.getD []
    if ps.eraseDups.length ≠ ps.length then throw (err "duplicate macro parameter (§6.10.3p6)")
    if (ps.dropLast.contains (bs "__VA_ARGS__")) ∨ (¬variadic ∧ ps.contains (bs "__VA_ARGS__")) then
      throw (err "__VA_ARGS__ as a parameter name (§6.10.3p5)")
    let vaName := if variadic then ps.getLast? else none
    if body.any (fun t => t.kind = .ident ∧ t.spell = bs "__VA_ARGS__") ∧ vaName ≠ some (bs "__VA_ARGS__") then
      throw (err "__VA_ARGS__ outside a variadic macro (§6.10.3p5)")
    let items ← match resolveBody cfg params variadic body with
      | .ok i => pure i
      | .error m => throw (err m)
    let d : MacroDef := ⟨params, variadic, body, items, none⟩
    match st.macros.get? n.spell with
    | some old =>
      if sameDef old d then pure st
      else if cfg.ext.redefine then pure { st with macros := st.macros.insert n.spell d }
      else throw (err "macro redefined differently (§6.10.3p2)")
    | none => pure { st with macros := st.macros.insert n.spell d }

/-! ### `#include` (§6.10.2) -/

/-- The directory part of a path, with its final `/` (empty if none). -/
def dirOf (p : Bytes) : Bytes :=
  let r := p.reverse.dropWhile (· ≠ 47)
  r.reverse

/-- The places searched, in order: for `"…"` the including file's directory
first (§6.10.2p3), then `includePaths ++ sysIncludePaths` (from index
`start`, for `#include_next`). Returns the opened path, the place index, and
the contents. -/
def searchHeader (cfg : PreCfg) (fs : FileSys) (fc : FileCtx) (quote next : Bool) (name : Bytes) :
    Option (Bytes × Option Nat × Bytes) :=
  if name.head? = some 47 then
    (fs.lookup name).map (fun c => (name, none, c))
  else
    let here : Option (Bytes × Option Nat × Bytes) :=
      if quote ∧ ¬(next ∧ ¬fc.isMain) then
        let p := dirOf fc.path ++ name
        (fs.lookup p).map (fun c => (p, none, c))
      else none
    match here with
    | some r => some r
    | none =>
      let start := if next then (match fc.foundAt with | some i => i + 1 | none => 0) else 0
      let dirs := (cfg.includePaths ++ cfg.sysIncludePaths).zipIdx.drop start
      dirs.findSome? (fun (d, i) =>
        let p := d ++ [47] ++ name
        (fs.lookup p).map (fun c => (p, some i, c)))

/-- The header named by a header-name spelling (with delimiters). -/
def headerOf (s : Bytes) : Option (Bool × Bytes) :=
  match s.head?, s.getLast? with
  | some 60, some 62 => if s.length ≥ 2 then some (false, (s.drop 1).dropLast) else none
  | some 34, some 34 => if s.length ≥ 2 then some (true, (s.drop 1).dropLast) else none
  | _, _ => none

/-! ### Pragmas (§6.10.6) -/

/-- The output form of a pragma. -/
def pragmaOut (ts : List Tok) : List PPToken :=
  ⟨.pragmaBegin, []⟩ :: ts.map (fun t => ⟨t.kind, t.spell⟩) ++ [⟨.pragmaEnd, []⟩]

/-- Pragmas executed in phase 4 (`once`, `push_macro`, `pop_macro`, when
enabled): `some st'`; others: `none` (they go to the output). -/
def phase4Pragma (cfg : PreCfg) (fc : FileCtx) (st : PState) (ts : List Tok) : Option PState :=
  match ts with
  | [o] => if cfg.ext.pragmaOnce ∧ o.kind = .ident ∧ o.spell = bs "once" then
             some { st with once := FileSys.normComponents fc.path :: st.once } else none
  | [o, lp, s, rp] =>
    if cfg.ext.pushPopMacro ∧ o.kind = .ident ∧ isPunct lp "(" ∧ s.kind = .strLit ∧ isPunct rp ")"
       ∧ s.spell.head? = some 34 then
      let n := (s.spell.drop 1).dropLast
      if o.spell = bs "push_macro" then
        some { st with pushed := (n, st.macros.get? n) :: st.pushed }
      else if o.spell = bs "pop_macro" then
        match st.pushed.find? (·.1 = n) with
        | some (_, d) =>
          let pushed := st.pushed.eraseP (·.1 = n)
          let macros := match d with | some d => st.macros.insert n d | none => st.macros.erase n
          some { st with pushed := pushed, macros := macros }
        | none => some st
      else none
    else none
  | _ => none

/-- Emit expanded text: tokens go to the output; pragma markers (from
`_Pragma`) are executed if they are phase-4 pragmas, else output. A `'` or
`"` that is not part of a literal is undefined behaviour (§6.4p3): error. -/
def emitText (cfg : PreCfg) (fc : FileCtx) : Nat → PState → List Tok → Except PreError PState
  | _, st, [] => .ok st
  | 0, _, t :: _ => .error (fileErr fc .fuel t.line "emit fuel")
  | fuel + 1, st, t :: rest =>
    if t.kind = .pragmaBegin then
      let body := rest.takeWhile (·.kind ≠ .pragmaEnd)
      let rest' := (rest.drop body.length).drop 1
      match phase4Pragma cfg fc st body with
      | some st' => emitText cfg fc fuel st' rest'
      | none => emitText cfg fc fuel { st with out := (pragmaOut body).reverse ++ st.out } rest'
    else if t.kind = .other ∧ (t.spell = [39] ∨ t.spell = [34]) then
      .error (fileErr fc .lexical t.line "unmatched quote (§6.4p3)")
    else emitText cfg fc fuel { st with out := ⟨t.kind, t.spell⟩ :: st.out } rest

/-! ### The phase-4 driver -/

def ExpCtx.ofState (cfg : PreCfg) (fs : FileSys) (fc : FileCtx) (st : PState) (inIf : Bool) : ExpCtx :=
  { cfg := cfg, macros := st.macros, file := fc.presumed, lineDelta := fc.lineDelta, inIf := inIf,
    hasInc := fun next h =>
      match headerOf h with
      | some (q, n) => (searchHeader cfg fs fc q next n).isSome
      | none => false }

def isActive : List CondFrame → Bool
  | [] => true
  | f :: _ => f.active

/-- Process a file's phase-3 items. Structurally recursive on fuel; an
`#include` processes the included file with the remaining fuel. -/
def procFile (cfg : PreCfg) (fs : FileSys) : Nat → PState → FileCtx → List LItem → Bool →
    List CondFrame → Except PreError PState
  | 0, _, fc, _, _, _ => .error (fileErr fc .fuel 0 "phase 4 fuel")
  | fuel + 1, st, fc, items, bof, conds =>
    match items with
    | [] =>
      if conds.isEmpty then .ok st
      else .error (fileErr fc .directive 0 "unterminated conditional (#if without #endif)")
    | .nl _ :: rest => procFile cfg fs fuel st fc rest bof conds
    | .tok h :: rest =>
      if isHashTok h.kind h.spell then
        let (ln, nlLine, rest') := takeLine rest []
        let line := h.line
        let err (k : ErrKind) (m : String) := fileErr fc k line m
        let active := isActive conds
        match ln with
        | [] => procFile cfg fs fuel st fc rest' false conds        -- null directive (§6.10.7)
        | d :: args =>
          let name := if d.kind = .ident then d.spell else []
          -- conditional directives are examined even in skipped groups (§6.10.1p6)
          if name = bs "if" ∨ name = bs "ifdef" ∨ name = bs "ifndef" then
            if conds.length ≥ cfg.maxIfDepth then .error (err .limit "#if nesting too deep")
            else if ¬active then
              procFile cfg fs fuel st fc rest' false (⟨false, true, false, false⟩ :: conds)
            else do
              let (c, st) ←
                if name = bs "if" then do
                  let ctx := ExpCtx.ofState cfg fs fc st true
                  let (ctr, ex) ← expand ctx fuel st.counter args []
                  let c ← evalIf ctx line ex
                  pure (c, { st with counter := ctr })
                else
                  match args with
                  | x :: _ =>
                    if x.kind = .ident then
                      let dfd := isDefinedName (ExpCtx.ofState cfg fs fc st false) x.spell
                      pure (if name = bs "ifdef" then dfd else !dfd, st)
                    else throw (err .directive "#ifdef/#ifndef needs an identifier (§6.10.1)")
                  | [] => throw (err .directive "#ifdef/#ifndef needs an identifier (§6.10.1)")
              procFile cfg fs fuel st fc rest' false (⟨c, c, false, true⟩ :: conds)
          else if name = bs "elif" then
            match conds with
            | [] => .error (err .directive "#elif without #if")
            | f :: fs' =>
              if f.elseSeen then .error (err .directive "#elif after #else")
              else if ¬f.outer ∨ f.taken then
                procFile cfg fs fuel st fc rest' false ({ f with active := false } :: fs')
              else do
                let ctx := ExpCtx.ofState cfg fs fc st true
                let (ctr, ex) ← expand ctx fuel st.counter args []
                let c ← evalIf ctx line ex
                procFile cfg fs fuel { st with counter := ctr } fc rest' false
                  ({ f with active := c, taken := c } :: fs')
          else if name = bs "else" then
            match conds with
            | [] => .error (err .directive "#else without #if")
            | f :: fs' =>
              if f.elseSeen then .error (err .directive "#else after #else")
              else
                let c := f.outer ∧ ¬f.taken
                procFile cfg fs fuel st fc rest' false (⟨c, f.taken || c, true, f.outer⟩ :: fs')
          else if name = bs "endif" then
            match conds with
            | [] => .error (err .directive "#endif without #if")
            | _ :: fs' => procFile cfg fs fuel st fc rest' false fs'
          else if ¬active then procFile cfg fs fuel st fc rest' false conds
          else if name = bs "define" then do
            let st ← doDefine cfg fc line st args
            procFile cfg fs fuel st fc rest' false conds
          else if name = bs "undef" then
            match args with
            | x :: _ =>
              if x.kind = .ident then
                procFile cfg fs fuel { st with macros := st.macros.erase x.spell } fc rest' false conds
              else .error (err .directive "#undef needs an identifier (§6.10.3.5)")
            | [] => .error (err .directive "#undef needs an identifier (§6.10.3.5)")
          else if name = bs "include" ∨ (cfg.ext.includeNext ∧ name = bs "include_next") then do
            let next := name = bs "include_next"
            -- §6.10.2p2–4: a header-name, or macro-replaced tokens forming one
            let hdr ← match args with
              | h :: _ =>
                if h.kind = .headerName then pure h.spell
                else do
                  let ctx := ExpCtx.ofState cfg fs fc st false
                  let (_, ex) ← expand ctx fuel st.counter args []
                  match ex with
                  | [s] =>
                    if s.kind = .strLit ∧ s.spell.head? = some 34 then pure s.spell
                    else throw (err .include "#include expects \"FILENAME\" or <FILENAME> (§6.10.2p4)")
                  | lt :: more =>
                    match more.getLast? with
                    | some gt =>
                      if isPunct lt "<" ∧ isPunct gt ">" then
                        pure (60 :: joinHeader cfg more.dropLast ++ [62])
                      else throw (err .include "#include expects \"FILENAME\" or <FILENAME> (§6.10.2p4)")
                    | none => throw (err .include "#include expects \"FILENAME\" or <FILENAME> (§6.10.2p4)")
                  | [] => throw (err .include "#include expects \"FILENAME\" or <FILENAME> (§6.10.2p4)")
              | [] => throw (err .include "#include expects \"FILENAME\" or <FILENAME> (§6.10.2p4)")
            match headerOf hdr with
            | none => throw (err .include "bad header name")
            | some (quote, nm) =>
              match searchHeader cfg fs fc quote next nm with
              | none => throw (err .include s!"header not found (§6.10.2p2)")
              | some (path, foundAt, contents) =>
                if st.once.contains (FileSys.normComponents path) then procFile cfg fs fuel st fc rest' false conds
                else if fc.level + 1 > cfg.maxIncludeDepth then
                  throw (err .limit "#include nested too deeply (§5.2.4.1)")
                else do
                  let items' ← phases123 cfg path contents
                  let fc' : FileCtx := ⟨path, path, 0, foundAt, fc.level + 1, false⟩
                  let st ← procFile cfg fs fuel st fc' items' true []
                  procFile cfg fs fuel st fc rest' false conds
          else if name = bs "line" ∨ (cfg.ext.linemarker ∧ d.kind = .ppNumber) then do
            -- §6.10.4: digit-sequence ["s-char-sequence"], else macro-replaced
            let toks ← if name = bs "line" then do
                match args with
                | a :: _ =>
                  if a.kind = .ppNumber then pure args
                  else do
                    let (_, ex) ← expand (ExpCtx.ofState cfg fs fc st false) fuel st.counter args []
                    pure ex
                | [] => pure []
              else pure ln
            match toks with
            | n :: more =>
              if n.kind ≠ .ppNumber ∨ ¬n.spell.all isDigit then
                throw (err .directive "#line needs a digit sequence (§6.10.4p3)")
              else
                let v : Nat := n.spell.foldl (fun a c => a * 10 + (c - 48).toNat) 0
                if name = bs "line" ∧ (v = 0 ∨ v > 2147483647) then
                  throw (err .directive "#line number out of range (§6.10.4p3)")
                else
                  let presumed ← match more with
                    | [] => pure fc.presumed
                    | s :: extra =>
                      if s.kind = .strLit ∧ s.spell.head? = some 34 ∧ (extra.isEmpty ∨ name ≠ bs "line") then
                        pure ((s.spell.drop 1).dropLast)
                      else throw (err .directive "#line: bad file name (§6.10.4p4)")
                  let fc' := { fc with presumed := presumed, lineDelta := (v : Int) - ((nlLine + 1 : Nat) : Int) }
                  procFile cfg fs fuel st fc' rest' false conds
            | [] => throw (err .directive "#line needs a digit sequence (§6.10.4p3)")
          else if name = bs "error" then
            let txt := (args.zipIdx.map (fun (t, j) => (if j > 0 ∧ t.ws then [32] else []) ++ t.spell)).flatten
            .error (err .errorDirective s!"#error {(String.fromUTF8? (ByteArray.mk txt.toArray)).getD "<bytes>"}")
          else if cfg.ext.warning ∧ name = bs "warning" then
            procFile cfg fs fuel st fc rest' false conds
          else if name = bs "pragma" then do
            match phase4Pragma cfg fc st args with
            | some st' => procFile cfg fs fuel st' fc rest' false conds
            | none =>
              let isStdc : Bool := match args.head? with | some o => o.spell = bs "STDC" | none => false
              let (st, body) ← if cfg.pragmaExpand ∧ ¬isStdc then do
                  let (ctr, ex) ← expand (ExpCtx.ofState cfg fs fc st false) fuel st.counter args.tail []
                  pure ({ st with counter := ctr }, args.take 1 ++ ex)
                else pure (st, args)
              procFile cfg fs fuel { st with out := (pragmaOut body).reverse ++ st.out } fc rest' false conds
          else if cfg.ext.hashBang ∧ bof ∧ fc.isMain ∧ isPunct d "!" then
            procFile cfg fs fuel st fc rest' false conds
          else
            -- `# non-directive`: ignored
            procFile cfg fs fuel st fc rest' false conds
      else if ¬isActive conds then
        procFile cfg fs fuel st fc (skipText rest false) false conds
      else do
        let (txt, rest') := takeText (.tok h :: rest) true []
        let ctx := ExpCtx.ofState cfg fs fc st false
        let (ctr, ex) ← expand ctx fuel st.counter txt []
        let st ← emitText cfg fc (ex.length + 1) { st with counter := ctr } ex
        procFile cfg fs fuel st fc rest' false conds

end CSem.Pre
