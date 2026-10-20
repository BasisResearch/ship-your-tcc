import Tcc.Model.Run

/-!
# TccModel: tcc.c's `main` for the image's command line

c/src/main.c calls tcc's main (renamed tcc_main) with gen_embed.sh's argv:
`{"tcc", args..., "-run", "/prog.c"}`, or with libraries after the file
`{"tcc", args..., "/prog.c", largs..., "-run", "--"}`. This module runs
that path (tcc_new's defaults are `TccSt`'s; tcc_parse_args for the
options the image passes; tcc_set_output_type; the per-file loop;
tcc_run) and reports where tcc stopped.
-/

namespace Tcc.Model
open C

/-- where tcc's main ended -/
inductive MainOutcome where
  /-- tcc reached the image's `tcc_baremetal_run` with the arena in `TccSt.arena` -/
  | ran (r : RunResult)
  /-- tcc's main returned (or exit()ed) with this status before running the program -/
  | exited (code : Int32)
  deriving Inhabited

/-- `args_parser_add_file(s, filename, filetype)` -/
def args_parser_add_file (filename : ByteArray) (filetype : Int32) : M Unit :=
  modify fun s => { s with
    files := s.files.push (filetype, filename),
    nb_libraries := if filetype &&& AFF_TYPE_LIB != 0 then s.nb_libraries + 1 else s.nb_libraries }

/-- `tcc_parse_args(s, &argc, &argv)` for the options the image passes
(`-w`, `-run`, `-l`, `-I`, `-D`, `--`, files): (ret, program argv) -/
def tcc_parse_args (R : Rec) (argv : Array ByteArray) : M (Int32 × Array ByteArray) := do
  let mut optind := 1
  let mut run : Option ByteArray := none
  let mut empty := true
  for _ in [0:argv.size] do
    if optind ≥ argv.size then break
    let r := argv[optind]!
    optind := optind + 1
    if r.size == 0 || r.get! 0 != 45 || r.size == 1 then
      args_parser_add_file r (← get).filetype
      empty := false
      if run.isSome then break
      continue
    if r.size == 2 && r.get! 1 == 45 then
      if run.isSome then break
      continue
    let opt := r.extract 1 r.size
    if opt == "w".toUTF8 then
      modify fun s => { s with warn_none := 1 }
    else if opt.size ≥ 3 && opt.extract 0 3 == "run".toUTF8 then
      run := some (opt.extract 3 opt.size)
      if (← get).output_type != 0 then
        tcc_warning R (cfmt "-%s: overriding compiler action already specified" [.s "run".toUTF8])
      modify fun s => { s with output_type := TCC_OUTPUT_MEMORY }
    else if opt.size ≥ 1 && opt.get! 0 == 108 then
      let mut a := opt.extract 1 opt.size
      if a.size == 0 then
        if optind ≥ argv.size then
          return (← tcc_error_noabort R (cfmt "argument to '%s' is missing" [.s r]), #[])
        a := argv[optind]!
        optind := optind + 1
      args_parser_add_file a (AFF_TYPE_LIB ||| ((← get).filetype &&& ~~~AFF_TYPE_MASK))
    else if opt.size ≥ 1 && opt.get! 0 == 73 then
      let mut a := opt.extract 1 opt.size
      if a.size == 0 then
        a := argv[optind]!
        optind := optind + 1
      let lp := (← get).tcc_lib_path
      let cf := ((← get).file.head?).map (·.true_filename)
      modify fun s => { s with include_paths := s.include_paths ++ tcc_split_path lp cf a }
    else
      unsupported s!"tcc option {Bytes.toStr r}"
    empty := false
  match run with
  | some o =>
    if o.size != 0 then unsupported "-run<options>"
    return (0, argv.extract (optind - 1) argv.size)
  | none => return (if empty then 1 else 0, #[])

/-- `tcc_set_output_type(s, output_type)` for TCC_OUTPUT_MEMORY -/
def tcc_set_output_type (output_type : Int32) : M Int32 := do
  modify fun s => { s with output_type := output_type }
  let s ← get
  if s.nostdinc == 0 then
    let cf := (s.file.head?).map (·.true_filename)
    let sp := tcc_split_path s.tcc_lib_path cf "{B}/include:/include".toUTF8
    modify fun st => { st with sysinclude_paths := st.sysinclude_paths ++ sp }
  tccelf_new
  let s ← get
  if s.nostdlib_paths == 0 then
    let cf := (s.file.head?).map (·.true_filename)
    let lp := tcc_split_path s.tcc_lib_path cf "{B}:/lib".toUTF8
    modify fun st => { st with library_paths := st.library_paths ++ lp }
  let cf := ((← get).file.head?).map (·.true_filename)
  let cp := tcc_split_path (← get).tcc_lib_path cf "/lib".toUTF8
  modify fun st => { st with crt_paths := st.crt_paths ++ cp }
  return if (← get).nb_errors != 0 then -1 else 0

/-- tcc.c's `main(argc, argv)` up to the hand-off to the program -/
def tcc_main (R : Rec) (argv : Array ByteArray) : M MainOutcome := do
  modify fun s => { s with tcc_lib_path := "/lib/tcc".toUTF8 }
  let (opt, _pargv) ← tcc_parse_args R argv
  if opt != 0 then
    return .exited (if opt < 0 then 1 else 0)
  let s ← get
  if s.files.size == 0 then
    let _ ← tcc_error_noabort R "no input files".toUTF8
  if (← get).nb_errors != 0 then return .exited 1
  -- set_environment: the image's environment is empty
  if (← get).output_type == 0 then modify fun s => { s with output_type := TCC_OUTPUT_EXE }
  let mut ret ← tcc_set_output_type (← get).output_type
  let mut n := 0
  let files := (← get).files
  for _ in [0:files.size] do
    if ret != 0 then break
    let (ty, name) := files[n]!
    modify fun s => { s with filetype := ty }
    if ty &&& AFF_TYPE_LIB != 0 then
      ret ← tcc_add_library R name
    else
      ret ← R.tcc_add_file_internal name (ty ||| AFF_PRINT_ERROR)
    n := n + 1
  if ret == 0 then
    if (← get).output_type == TCC_OUTPUT_MEMORY then
      match ← tcc_run R with
      | .ok r => return .ran r
      | .error e => ret := e
    else
      unsupported "output types other than -run"
  if ret != 0 && (← get).nb_errors != 0 then ret := 1
  return .exited ret

end Tcc.Model
