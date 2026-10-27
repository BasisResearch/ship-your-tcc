/-
Macro replacement, C11 §6.10.3, by Prosser's algorithm.

The standard specifies rescanning (§6.10.3.4) informally: a macro's name is
not replaced while its own replacement is being rescanned, and such a
non-replaced name "is no longer available for further replacement even if it
is later (re)examined". Prosser's algorithm (the one the ANSI committee's
rationale was written against; D. Prosser, "Complete Algorithm for
ANSI/ISO C Macro Expansion", X3J11/86-196) makes this precise with *hide
sets*: every token carries the set of macro names it may not invoke.

    expand(TS):
      TS = {}                      → {}
      TS = T^HS • TS', T ∈ HS      → T^HS • expand(TS')
      T object-like macro          → expand(subst(ts(T), {}, {}, HS ∪ {T}) • TS')
      TS = T^HS • ( • args • )^HS' • TS'', T function-like
                                   → expand(subst(ts(T), fp(T), args,
                                                   (HS ∩ HS') ∪ {T}) • TS'')
      otherwise                    → T^HS • expand(TS')

`subst` is written from §6.10.3.1–3 directly:
* a parameter occurrence not adjacent to `#` or `##` is replaced by its
  argument "after all macros contained therein have been expanded" — the
  argument is expanded once (`expand` with no further tokens available,
  §6.10.3.1p1), the first time it is needed;
* `# param` is replaced by the stringized raw argument (§6.10.3.2p2);
* a parameter adjacent to `##` is replaced by the raw argument, or by a
  placemarker if the argument has no tokens (§6.10.3.3p2);
* each `##` of the replacement list then concatenates its neighbours, left
  to right (§6.10.3.3p3; the order is unspecified); placemarkers vanish;
* the hide set `HS` is added to every resulting token; a token made by `##`
  starts from the intersection of its operands' hide sets.

The expander also performs the operators that work on the token sequence
during rescanning: `defined` (§6.10.1p1, in `#if` only; also when it is
produced by macro replacement, which §6.10.1p4 leaves undefined — tcc and gcc
evaluate it), `__has_include` (`Ext.hasInclude`), and `_Pragma`
(§6.10.9), which becomes a pragma marker in the output.

Fuel: `expand` is structurally recursive on a fuel argument; one unit per
step, arguments are expanded with the remaining fuel.
-/
import Spec.Pre.Lex

namespace CSem.Pre

/-- The dynamic predefined macros (§6.10.8.1) and `__COUNTER__`. -/
inductive Builtin
  | line | file | date | time | counter
  deriving DecidableEq, Repr, Inhabited

/-- An item of a replacement list, with parameters and operators resolved at
`#define` time. `param i ws plain`: occurrence of parameter `i`; `plain` iff
it is not an operand of `##` (§6.10.3.1). `str i ws`: `# param`.
`commaPaste comma i`: the GNU `, ## __VA_ARGS__` (`Ext.commaPaste`). -/
inductive RItem
  | tok (t : Tok)
  | param (i : Nat) (ws : Bool) (plain : Bool)
  | str (i : Nat) (ws : Bool)
  | paste
  | commaPaste (comma : Tok) (i : Nat)
  deriving Repr, Inhabited

/-- A macro definition (§6.10.3). `params = none`: object-like. `body`: the
replacement list as written (for the identity test of §6.10.3p2). -/
structure MacroDef where
  params : Option (List Bytes)
  variadic : Bool
  body : List Tok
  items : List RItem
  builtin : Option Builtin
  deriving Repr, Inhabited

/-- The macro table. -/
abbrev Macros := Std.TreeMap Bytes MacroDef compare

/-- What the expander needs to know besides the tokens. `hasInc next h`:
does the header named by `h` (a header-name spelling with its delimiters)
exist, searched as by `#include` (`next`: `#include_next`). -/
structure ExpCtx where
  cfg : PreCfg
  macros : Macros
  pushed : List (Bytes × Option MacroDef)
  file : Bytes
  lineDelta : Int
  inIf : Bool
  hasInc : Bool → Bytes → Bool

def ExpCtx.err (ctx : ExpCtx) (k : ErrKind) (line : Nat) (msg : String) : PreError :=
  ⟨k, ctx.file, line, msg⟩

def isPunct (t : Tok) (s : String) : Bool := t.kind = .punct ∧ t.spell = bs s

def hsUnion (a b : List Bytes) : List Bytes := a ++ b.filter (fun x => !a.contains x)
def hsInter (a b : List Bytes) : List Bytes := a.filter (fun x => b.contains x)

/-- A `"…"` spelling with `"` and `\` escaped (§6.10.3.2p2, §6.10.8.1 `__FILE__`). -/
def escapeStr (b : Bytes) : Bytes :=
  b.foldr (fun c acc => if c = 34 ∨ c = 92 then 92 :: c :: acc else c :: acc) []

def quoted (b : Bytes) : Bytes := 34 :: escapeStr b ++ [34]

/-- §6.10.3.2p2: the spelling of the character string literal made by `#`
from an argument's preprocessing tokens. White space between tokens becomes
one space, leading and trailing white space is deleted, and `"`/`\` inside
character constants and string literals are escaped. An invalid result is
undefined behaviour (`none`). -/
def stringize (cfg : PreCfg) (ts : List Tok) : Option Bytes :=
  let body := (ts.zipIdx.map (fun (t, j) =>
      (if j > 0 ∧ t.ws then [32] else []) ++
      (if t.kind = .strLit ∨ t.kind = .charConst then escapeStr t.spell else t.spell))).flatten
  let s := 34 :: body ++ [34]
  match lexOne cfg s with
  | some (.strLit, _) => some s
  | _ => none

/-- A substituted replacement list before `##` is applied. -/
inductive SItem
  | tok (t : Tok)
  | pm      -- placemarker (§6.10.3.3p2)
  | paste   -- a `##` operator of the replacement list
  deriving Repr, Inhabited

/-- §6.10.3.3p3: concatenate two operands of `##`. -/
def glue (ctx : ExpCtx) (line : Nat) : SItem → SItem → Except PreError SItem
  | .pm, .tok t => .ok (.tok { t with line := line })
  | .tok t, .pm => .ok (.tok { t with line := line })
  | .pm, x => .ok x
  | x, .pm => .ok x
  | .tok a, .tok b =>
    match lexOne ctx.cfg (a.spell ++ b.spell) with
    | some (k, s) => .ok (.tok { a with kind := k, spell := s, hs := hsInter a.hs b.hs, line := line })
    | none => .error (ctx.err .macro line
        s!"pasting does not give a valid preprocessing token (§6.10.3.3p3)")
  | _, _ => .error (ctx.err .macro line "'##' operand")

/-- Apply the `##` operators left to right; `pend`: a `##` is waiting for
its right operand. -/
def pasteAll (ctx : ExpCtx) (line : Nat) : List SItem → List SItem → Bool → Except PreError (List SItem)
  | [], out, _ => .ok out.reverse
  | .paste :: rest, out, _ => pasteAll ctx line rest out true
  | x :: rest, out, pend =>
    if pend then
      match out with
      | l :: out' => do
        let r ← glue ctx line l x
        pasteAll ctx line rest (r :: out') false
      | [] => .error (ctx.err .macro line "'##' at the start of a replacement list")
    else pasteAll ctx line rest (x :: out) false

def setWs (ws : Bool) : List Tok → List Tok
  | [] => []
  | t :: ts => { t with ws := ws } :: ts

/-- §6.10.3.1–3: the replacement list of `m` with its parameters replaced by
the arguments (`raw`: as written, `exps`: macro-expanded, for the needed
ones), `#` and `##` applied, and the hide set `hs` added (Prosser's
`subst` + `hsadd`). `inv` is the macro-name token: replacement-list tokens
and paste results use its line, while substituted arguments retain their
own token locations. Its white space goes to the first result token. -/
def substitute (ctx : ExpCtx) (m : MacroDef) (raw : List (List Tok))
    (exps : List (Nat × List Tok)) (hs : List Bytes) (inv : Tok) : Except PreError (List Tok) := do
  let getRaw (i : Nat) : List Tok := raw.getD i []
  let getExp (i : Nat) : List Tok := ((exps.find? (·.1 = i)).map (·.2)).getD []
  let items ← m.items.mapM (fun it =>
    match it with
    | .tok t => pure [SItem.tok { t with line := inv.line }]
    | .paste => pure [SItem.paste]
    | .str i ws =>
      match stringize ctx.cfg (getRaw i) with
      | some s => pure [SItem.tok { kind := .strLit, spell := s, ws := ws, line := inv.line, hs := [] }]
      | none => throw (ctx.err .macro inv.line "'#' does not give a valid string literal (§6.10.3.2p2)")
    | .param i ws plain =>
      if plain then pure ((setWs ws (getExp i)).map SItem.tok)
      else
        let r := getRaw i
        if r.isEmpty then pure [SItem.pm] else pure ((setWs ws r).map SItem.tok)
    | .commaPaste comma i =>
      if (getRaw i).isEmpty then pure []
      else pure (SItem.tok { comma with line := inv.line } :: (getExp i).map SItem.tok))
  let pasted ← pasteAll ctx inv.line items.flatten [] false
  let toks := pasted.filterMap (fun x => match x with | .tok t => some t | _ => none)
  pure (setWs inv.ws (toks.map (fun t => { t with hs := hsUnion t.hs hs })))

/-- The parameters whose expanded argument is needed, in order of first use. -/
def neededArgs (items : List RItem) : List Nat :=
  items.foldl (fun acc it =>
    match it with
    | .param i _ true => if acc.contains i then acc else acc ++ [i]
    | .commaPaste _ i => if acc.contains i then acc else acc ++ [i]
    | _ => acc) []

/-- §6.10.3p10–12: split the tokens after `(` into arguments at the commas
not inside nested parentheses, up to the matching `)`. Once `nNamed`
arguments are complete, a variadic macro's last argument takes the rest,
commas included. Result: arguments, the `)` token, the rest. -/
def splitArgs (nNamed : Nat) (variadic : Bool) :
    List Tok → Nat → List Tok → List (List Tok) → Option (List (List Tok) × Tok × List Tok)
  | [], _, _, _ => none
  | t :: rest, depth, cur, args =>
    if isPunct t ")" ∧ depth = 0 then some ((cur.reverse :: args).reverse, t, rest)
    else if isPunct t "," ∧ depth = 0 ∧ ¬(variadic ∧ args.length = nNamed) then
      splitArgs nNamed variadic rest depth [] (cur.reverse :: args)
    else if isPunct t "(" then splitArgs nNamed variadic rest (depth + 1) (t :: cur) args
    else if isPunct t ")" then splitArgs nNamed variadic rest (depth - 1) (t :: cur) args
    else splitArgs nNamed variadic rest depth (t :: cur) args

/-- §6.10.3p4: match the arguments against the parameters. -/
def matchArity (ctx : ExpCtx) (line : Nat) (ps : List Bytes) (variadic : Bool)
    (args : List (List Tok)) : Except PreError (List (List Tok)) :=
  let nNamed := if variadic then ps.length - 1 else ps.length
  if ¬variadic then
    if ps.length = 0 then
      (if (match args with | [[]] => true | _ => false) then .ok [] else .error (ctx.err .macro line "macro takes no arguments"))
    else if args.length = ps.length then .ok args
    else .error (ctx.err .macro line s!"macro needs {ps.length} arguments, got {args.length} (§6.10.3p4)")
  else if args.length = nNamed + 1 then .ok args
  else if args.length = nNamed ∧ ctx.cfg.ext.emptyVariadic then .ok (args ++ [[]])
  else .error (ctx.err .macro line s!"too few arguments to variadic macro (§6.10.3p4)")

def numTok (t : Tok) (n : Nat) : Tok :=
  { t with kind := .ppNumber, spell := bs (toString n) }

/-- The token a dynamic predefined macro expands to (§6.10.8.1). -/
def builtinTok (ctx : ExpCtx) (ctr : Nat) (b : Builtin) (t : Tok) : Nat × Tok :=
  match b with
  | .line => (ctr, { t with kind := .ppNumber, spell := bs (toString ((t.line : Int) + ctx.lineDelta)) })
  | .file => (ctr, { t with kind := .strLit, spell := quoted ctx.file })
  | .date => (ctr, { t with kind := .strLit, spell := 34 :: ctx.cfg.date ++ [34] })
  | .time => (ctr, { t with kind := .strLit, spell := 34 :: ctx.cfg.time ++ [34] })
  | .counter => (ctr + 1, numTok t ctr)

/-- §6.10.1p1: `defined identifier` or `defined ( identifier )`. -/
def definedOperand : List Tok → Option (Bytes × List Tok)
  | x :: rest =>
    if x.kind = .ident then some (x.key, rest)
    else if isPunct x "(" then
      match rest with
      | y :: z :: rest' => if y.kind = .ident ∧ isPunct z ")" then some (y.key, rest') else none
      | _ => none
    else none
  | [] => none

def isDefinedName (ctx : ExpCtx) (n : Bytes) : Bool :=
  ctx.macros.contains n ∨
    (ctx.cfg.ext.hasInclude ∧ (n = bs "__has_include" ∨ n = bs "__has_include_next"))

/-- The spelling, with delimiters, of the header named by tokens `<` … `>`
(§6.10.2p4: implementation-defined joining). -/
def joinHeader (cfg : PreCfg) (ts : List Tok) : Bytes :=
  (ts.zipIdx.map (fun (t, j) => (if cfg.hdrJoinSpaces ∧ j > 0 ∧ t.ws then [32] else []) ++ t.spell)).flatten

/-- The operand of `__has_include ( … )`: a header-name, a string literal, or
`<` tokens `>`. -/
def hasIncludeOperand (cfg : PreCfg) : List Tok → Option (Bytes × List Tok)
  | lp :: rest =>
    if ¬isPunct lp "(" then none
    else
      match rest with
      | h :: rp :: rest' =>
        if (h.kind = .headerName ∨ (h.kind = .strLit ∧ h.spell.head? = some 34)) ∧ isPunct rp ")" then
          some (h.spell, rest')
        else if isPunct h "<" then
          let inner := rest.tail.takeWhile (fun t => ¬isPunct t ">")
          match rest.tail.drop inner.length with
          | gt :: rp' :: rest'' =>
            if isPunct gt ">" ∧ isPunct rp' ")" then some (60 :: joinHeader cfg inner ++ [62], rest'') else none
          | _ => none
        else none
      | _ => none
  | [] => none

/-- §6.10.9: destringize the operand of `_Pragma`: delete an `L` prefix and
the quotes, replace `\"` by `"` and `\\` by `\`. -/
def destringize (s : Bytes) : Bytes :=
  let s := if s.head? = some 76 then s.tail else s
  let inner := (s.drop 1).dropLast
  let rec go : Bytes → Bytes → Bytes
    | 92 :: c :: rest, acc => if c = 34 ∨ c = 92 then go rest (c :: acc) else go rest (c :: 92 :: acc)
    | c :: rest, acc => go rest (c :: acc)
    | [], acc => acc.reverse
  go inner []

def markerTok (k : PPKind) (t : Tok) : Tok := { t with kind := k, spell := [] }

/-- The tokens of a `_Pragma` operand, lexed by phase 3 (§6.10.9). -/
def pragmaToks (ctx : ExpCtx) (t : Tok) (s : Bytes) : Except PreError (List Tok) := do
  let items ← phase3 ctx.cfg ctx.file ((destringize s).map (fun c => (⟨c, t.line⟩ : PChar)))
  pure (items.filterMap (fun i => match i with | .tok x => some { x with line := t.line } | .nl _ => none))

/-- Execute a macro-stack pragma immediately, so rescanning sees its effect.
The spelling in the string operand is destringized before identifier lookup. -/
def macroPragma (ctx : ExpCtx) (ts : List Tok) : Option ExpCtx :=
  match ts with
  | [o, lp, s, rp] =>
    if ctx.cfg.ext.pushPopMacro ∧ o.kind = .ident ∧ isPunct lp "(" ∧
        s.kind = .strLit ∧ isPunct rp ")" ∧ s.spell.head? = some 34 then
      let n := identifierKey (destringize s.spell)
      if o.spell = bs "push_macro" then
        some { ctx with pushed := (n, ctx.macros.get? n) :: ctx.pushed }
      else if o.spell = bs "pop_macro" then
        match ctx.pushed.find? (·.1 = n) with
        | some (_, d) =>
          let macros := match d with | some d => ctx.macros.insert n d | none => ctx.macros.erase n
          some { ctx with pushed := ctx.pushed.eraseP (·.1 = n), macros }
        | none => some ctx
      else none
    else none
  | _ => none

/-- Expansion returns all effects, including effects of prescanning arguments. -/
structure Expansion where
  ctx : ExpCtx
  counter : Nat
  tokens : List Tok

/-- Prosser's `expand`, with the rescanning-time operators. `ctr`: the
`__COUNTER__` value. `acc`: output so far, reversed. -/
def expand (ctx : ExpCtx) : Nat → Nat → List Tok → List Tok → Except PreError Expansion
  | 0, _, inp, _ => .error (ctx.err .fuel ((inp.head?.map (·.line)).getD 0) "macro expansion fuel")
  | _ + 1, ctr, [], acc => .ok ⟨ctx, ctr, acc.reverse⟩
  | fuel + 1, ctr, t :: rest, acc =>
    if t.kind ≠ .ident then expand ctx fuel ctr rest (t :: acc)
    else if ctx.inIf ∧ t.spell = bs "defined" then
      match definedOperand rest with
      | some (n, rest') =>
        expand ctx fuel ctr rest' (numTok t (if isDefinedName ctx n then 1 else 0) :: acc)
      | none => .error (ctx.err .ifExpr t.line "'defined' needs an identifier (§6.10.1p1)")
    else if t.hs.contains t.key then expand ctx fuel ctr rest (t :: acc)
    else
      match ctx.macros.get? t.key with
      | some m =>
        match m.builtin with
        | some b =>
          let (ctr', r) := builtinTok ctx ctr b t
          expand ctx fuel ctr' rest (r :: acc)
        | none =>
          match m.params with
          | none => do
            let out ← substitute ctx m [] [] (hsUnion t.hs [t.key]) t
            expand ctx fuel ctr (out ++ rest) acc
          | some ps =>
            match rest with
            | lp :: rest1 =>
              if isPunct lp "(" then
                let nNamed := if m.variadic then ps.length - 1 else ps.length
                match splitArgs nNamed m.variadic rest1 0 [] [] with
                | none => .error (ctx.err .macro t.line
                    "unterminated argument list invoking macro (§6.10.3p4)")
                | some (args, rp, rest2) => do
                  let args ← matchArity ctx rp.line ps m.variadic args
                  let (expanded, exps) ← (neededArgs m.items).foldlM
                    (fun (st : Expansion × List (Nat × List Tok)) i => do
                      let e ← expand st.1.ctx fuel st.1.counter (args.getD i []) []
                      pure (e, st.2 ++ [(i, e.tokens)])) (⟨ctx, ctr, []⟩, [])
                  let out ← substitute expanded.ctx m args exps (hsUnion (hsInter t.hs rp.hs) [t.key])
                    t
                  expand expanded.ctx fuel expanded.counter (out ++ rest2) acc
              else expand ctx fuel ctr rest (t :: acc)
            | [] => expand ctx fuel ctr rest (t :: acc)
      | none =>
        if ctx.inIf ∧ ctx.cfg.ext.hasInclude ∧
            (t.spell = bs "__has_include" ∨ t.spell = bs "__has_include_next") then
          match hasIncludeOperand ctx.cfg rest with
          | some (h, rest') =>
            let found := ctx.hasInc (t.spell = bs "__has_include_next") h
            expand ctx fuel ctr rest' (numTok t (if found then 1 else 0) :: acc)
          | none =>
            -- otherwise the parenthesized tokens are macro-replaced first and
            -- must then form a header name (as for `#include`, §6.10.2p4)
            match rest with
            | lp :: rest1 =>
              if isPunct lp "(" then
                match splitArgs 0 true rest1 0 [] [] with
                | some ([inner], rp, rest2) => do
                  let ex ← expand ctx fuel ctr inner []
                  match hasIncludeOperand ex.ctx.cfg (lp :: ex.tokens ++ [rp]) with
                  | some (h, _) =>
                    let found := ctx.hasInc (t.spell = bs "__has_include_next") h
                    expand ex.ctx fuel ex.counter rest2 (numTok t (if found then 1 else 0) :: acc)
                  | none => .error (ctx.err .ifExpr t.line "malformed __has_include")
                | _ => .error (ctx.err .ifExpr t.line "malformed __has_include")
              else .error (ctx.err .ifExpr t.line "malformed __has_include")
            | [] => .error (ctx.err .ifExpr t.line "malformed __has_include")
        else if ¬ctx.inIf ∧ t.spell = bs "_Pragma" then
          match rest with
          | lp :: more =>
            if isPunct lp "(" then
              match splitArgs 0 true more 0 [] [] with
              | some ([inner], _, rest') => do
                let ex ← expand ctx fuel ctr inner []
                match ex.tokens with
                | [s] =>
                  if s.kind = .strLit then do
                    let ps ← pragmaToks ex.ctx t s.spell
                    match macroPragma ex.ctx ps with
                    | some ctx' => expand ctx' fuel ex.counter rest' acc
                    | none => expand ex.ctx fuel ex.counter rest' (markerTok .pragmaEnd t :: ps.reverse ++ markerTok .pragmaBegin t :: acc)
                  else .error (ctx.err .directive t.line "_Pragma operand must expand to a string literal (§6.10.9)")
                | _ => .error (ctx.err .directive t.line "_Pragma operand must expand to one string literal (§6.10.9)")
              | _ => .error (ctx.err .directive t.line "_Pragma needs ( string-literal ) (§6.10.9)")
            else .error (ctx.err .directive t.line "_Pragma needs ( string-literal ) (§6.10.9)")
          | _ => .error (ctx.err .directive t.line "_Pragma needs ( string-literal ) (§6.10.9)")
        else expand ctx fuel ctr rest (t :: acc)

end CSem.Pre
