-- check.sh --compile-cut stage 7b: each must depend on at most propext, Classical.choice, Quot.sound.
import Tcc.Ccp.EndToEnd

-- the evaluator run of tcc's compile phase and its pieces (lib TccCompileCut)
#print axioms Tcc.Ccp.Sum.run_all
#print axioms Tcc.Ccp.Sum.tableOK
#print axioms Tcc.Ccp.Sum.mem0_pinned
#print axioms Tcc.Ccp.Sum.code_pinned
#print axioms Tcc.Ccp.Sum.resetFacts
#print axioms Tcc.Ccp.Sum.st_cut_view
-- PHASES 3a, proved
#print axioms Tcc.Ccp.Sum.compileCut_proved
-- the end-to-end theorem for P with no machine premise
#print axioms Tcc.EndToEnd.tcc_run_sum_closed
#print axioms Tcc.EndToEnd.tcc_run_sum_halts_closed
