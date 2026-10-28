import Spec.Pre.Tcc.Cfg

namespace CSem.Pre.Tcc
/-- Fixed image predefines for a separately compiled file. The only variable
is the compiler's actual filename in __BASE_FILE__; this does not consume an
exported opening witness or model-produced predefined text. -/
def predefsFor (filename : String) : CSem.Pre.Bytes :=
  bs (predefsRun.replace "#define __BASE_FILE__ \"/prog.c\"\n"
    ("#define __BASE_FILE__ \"" ++ filename ++ "\"\n"))
end CSem.Pre.Tcc
