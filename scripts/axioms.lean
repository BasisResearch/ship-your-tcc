-- check.sh stage 6: each must depend on at most propext, Classical.choice, Quot.sound.
import Tcc
import Tcc.Capture.Sum.Cut
import Tcc.Prog.Sum.Machine.Replay
import Tcc.EndToEnd.Sum
import Tcc.Ccp.Sound
import Tcc.Printf.Sum.Model

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
-- Phase 3, machine side: the captured cut of P (Tcc/Capture/Sum, Tcc/Boot)
#print axioms Tcc.Boot.cutConfig_captured
#print axioms Tcc.Boot.chunkView_sub
#print axioms Tcc.Boot.ElfReset.pc
#print axioms Tcc.Capture.Sum.cut_ok
#print axioms Tcc.Capture.Sum.hostView_elf
#print axioms Tcc.Capture.Sum.TccSumElf.host
#print axioms Tcc.Capture.Sum.tohostAddr_eq
#print axioms Tcc.Capture.Sum.sum_halts_from_reset
#print axioms Vsa.Machine.Halts.deterministic
#print axioms Vsa.Machine.Diverges.not_halts
#print axioms Tcc.Prog.Sum.Machine.sim_B0
#print axioms Tcc.Prog.Sum.Machine.replay
#print axioms Tcc.Prog.Sum.Machine.sum_halts
#print axioms Tcc.EndToEnd.tcc_run_sum
#print axioms Tcc.EndToEnd.tcc_run_sum_halts
#print axioms Tcc.Boot.row_jalr
#print axioms Tcc.Boot.row_putchar
#print axioms Tcc.Boot.row_exit
#print axioms Tcc.Boot.row_step
#print axioms Tcc.Boot.writeLog_get
-- CompileCut pilot: the generic evaluator-to-Sail soundness (Tcc/Ccp)
#print axioms Tcc.Ccp.run_sound
#print axioms Tcc.Ccp.exec_sound
#print axioms Tcc.Ccp.sim_cut
#print axioms Tcc.Ccp.tableOK_of_all
#print axioms Tcc.Ccp.run_trans
#print axioms Tcc.Ccp.store_bytes
#print axioms Tcc.Ccp.load_bytes
-- PrintfSpec: the printf read off the image (Tcc/Printf)
#print axioms Tcc.Clight.runPf_soundOn
#print axioms Tcc.Clight.runPfProgram_printsOn
#print axioms Tcc.Printf.machX_shape
#print axioms Tcc.Printf.machX_extDeterm
#print axioms Tcc.Printf.machX_model
#print axioms Tcc.Printf.machX_out
#print axioms Tcc.Printf.imgMakes_of_spec
#print axioms Tcc.Printf.Sum.entry0
#print axioms Tcc.Printf.Sum.ret0
#print axioms Tcc.Printf.Sum.call0
#print axioms Tcc.Printf.Sum.imgMakes0
#print axioms Tcc.Printf.Sum.prints_sumX
