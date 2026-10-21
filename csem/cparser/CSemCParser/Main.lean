import CSemCParser
import CabsImport

/-! # `csem_cparse`: the Lean C front end on the command line (F1a)

    csem_cparse --json [--gnu] PP FILE         Cabs JSON of the Lean parse
    csem_cparse --cmp  [--gnu] PP FILE ORACLE  compare with menhir's Cabs JSON

`PP` is the preprocessed text (what Cerberus's `cpp` step produces, see
scripts/csem_parse_cmp.py), `FILE` the C file's name as given to `cerberus`
(the lexer's file name), `ORACLE` the output of `cerberus --cabs-json FILE`
or `-` when menhir refused the file. `--cmp` prints one line
`STATUS<TAB>detail` (statuses: scripts/csem_parse_cmp.py). The comparison is
on JSON values: the Lean Cabs printed by `CabsExport` against the oracle's
document without its `digest` field; the oracle document is also read back by
`CabsImport` and printed by `CabsExport`, which must reproduce it (else
`ROUNDTRIP`: a printer bug, not a parser divergence). -/

open Lean (Json)
open CSemCParser

/-- The first path at which two JSON values differ. -/
partial def firstDiff (a b : Json) (path : String) : Option String :=
  match a, b with
  | .obj ka, .obj kb =>
    let keys := (ka.foldl (fun acc k _ => acc.push k) #[]) ++ (kb.foldl (fun acc k _ => acc.push k) #[])
    keys.foldl (fun r k => r <|> (match ka.get? k, kb.get? k with
      | some x, some y => firstDiff x y s!"{path}.{k}"
      | _, _ => some s!"{path}.{k} (missing on one side)")) none
  | .arr xs, .arr ys =>
    if xs.size != ys.size then some s!"{path} (lengths {xs.size} vs {ys.size})"
    else (List.range xs.size).foldl (fun r i => r <|> firstDiff xs[i]! ys[i]! s!"{path}[{i}]") none
  | x, y => if x == y then none else some s!"{path}: {x.compress.take 160} vs {y.compress.take 160}"

def stripDigest (j : Json) : Json :=
  match j with
  | .obj kvs => .obj (kvs.erase "digest")
  | _ => j

def main (args : List String) : IO UInt32 := do
  let gnuFlag := args.contains "--gnu"
  let args := args.filter (· != "--gnu")
  let cfg : ParseCfg := { gnu := gnuFlag }
  match args with
  | ["--json", pp, file] =>
    let src ← IO.FS.readBinFile pp
    match frontEnd cfg file src with
    | .ok tu => IO.println (CabsExport.translationUnit tu).compress; return 0
    | .error e => IO.eprintln e; return 1
  | ["--cmp", pp, file, oracle] =>
    let src ← IO.FS.readBinFile pp
    let t0 ← IO.monoMsNow
    let lean := frontEnd cfg file src
    let t1 ← IO.monoMsNow
    let timing := s!"lean {t1 - t0} ms"
    if oracle == "-" then
      match lean with
      | .ok _ => IO.println s!"LEAN_ONLY\t{timing}"; return 1
      | .error e => IO.println s!"BOTH_REJECT\t{e}"
      return 0
    let text ← IO.FS.readFile oracle
    let oj ← match Json.parse text with
      | .ok j => pure (stripDigest j)
      | .error e => IO.println s!"ROUNDTRIP\toracle JSON does not parse: {e}"; return 1
    match CabsImport.jsonToTranslationUnit oj with
    | .error e => IO.println s!"ROUNDTRIP\tCabsImport refuses the oracle JSON: {e}"; return 1
    | .ok otu =>
      let rt := CabsExport.translationUnit otu
      if rt != oj then
        IO.println s!"ROUNDTRIP\t{(firstDiff rt oj "").getD "?"}"
        return 1
      match lean with
      | .error e => IO.println s!"ORACLE_ONLY\t{e}"; return 1
      | .ok tu =>
        let lj := CabsExport.translationUnit tu
        if lj == oj then IO.println s!"AGREE\t{timing}"
        else
          IO.println s!"DIFF\t{(firstDiff lj oj "").getD "?"}"
          return 1
      return 0
  | _ =>
    IO.eprintln "usage: csem_cparse --json [--gnu] PP FILE | --cmp [--gnu] PP FILE ORACLE.json|-"
    return 2
