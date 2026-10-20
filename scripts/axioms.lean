-- check.sh stage 6: each must depend on at most propext, Classical.choice, Quot.sound.
import Tcc

-- Phase 3, Clight side (P = c/tests/p3/sum.c)
#print axioms Tcc.Prog.Sum.prints
#print axioms Tcc.Prog.Sum.terminates
#print axioms Tcc.Prog.Sum.unique
#print axioms Tcc.Prog.Sum.prints_modelX
#print axioms Tcc.Prog.Sum.runPfProgram_eq
#print axioms Tcc.Prog.Sum.expectedOut_eq_utf8
-- generic machinery
#print axioms Tcc.Clight.runPfProgram_prints
#print axioms Tcc.Clight.runPf_sound
#print axioms Tcc.Clight.runPf_unique
#print axioms Tcc.Clight.premises_satisfiable
#print axioms Tcc.Clight.modelX_printfModel
#print axioms Tcc.Clight.modelX_extDeterm
-- patch 0002: the parametrised CCLib results, for every X
#print axioms CC.stepWith_determ
#print axioms CC.doStepWith_sound
#print axioms CC.doStepWith_complete
#print axioms CC.doExternalCallWith_sound
#print axioms CC.externalCallWith_determ
