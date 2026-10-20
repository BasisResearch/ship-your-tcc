/-
A plain ISO C11 configuration: trigraphs on, no extension, no
implementation-defined identifier characters, only the predefined macros
§6.10.8.1 requires. With it, `phases14` is the standard's phases 1–4 alone
(the unit tests of `Spec/Pre/Test.lean` run the standard's own examples
under it as well as under tcc's configuration).
-/
import Spec.Pre.Types

namespace CSem.Pre

/-- No extension. -/
def extNone : Ext where
  includeNext := false
  pragmaOnce := false
  pushPopMacro := false
  counter := false
  hasInclude := false
  warning := false
  linemarker := false
  hashBang := false
  namedVariadic := false
  commaPaste := false
  emptyVariadic := false
  binaryLiterals := false
  redefine := false

/-- ISO C11 with a hosted implementation, `intmax_t` of 64 bits, signed
plain `char`, a single include directory `/include`. -/
def cfgIso : PreCfg where
  trigraphs := true
  extraIdChar := fun _ => false
  extraSpace := fun _ => false
  predefs := bs "#define __STDC__ 1\n#define __STDC_HOSTED__ 1\n#define __STDC_VERSION__ 201112L\n"
  predefsName := bs "<predefined>"
  mainName := bs "/prog.c"
  includePaths := []
  sysIncludePaths := [bs "/include"]
  hdrJoinSpaces := true
  date := bs "Jan  1 1970"
  time := bs "00:00:00"
  maxIncludeDepth := 15
  maxIfDepth := 63
  charSigned := true
  pragmaExpand := false
  ext := extNone

end CSem.Pre
