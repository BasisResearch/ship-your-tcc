-- check.sh stage 6b: the audit of patch 0002 (Tcc/Audit/CertoraInstance.lean),
-- the only theorems allowed to depend on Certora's axioms
-- CC.externalFunctionsSem, CC.inlineAssemblySem, CC.externalFunctionsSemDeterm,
-- CC.inlineAssemblySemDeterm (besides propext, Classical.choice, Quot.sound).
import Tcc

#print axioms Tcc.Audit.step_eq
#print axioms Tcc.Audit.externalCall_eq
#print axioms Tcc.Audit.externalCallOrig_eq
#print axioms Tcc.Audit.stepOrig_iff
#print axioms Tcc.Audit.stepWith_determ_certora
