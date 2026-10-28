import Tcc.CSem.Statement
import Tcc.CSem.IEEE
import CerbMemAllocatorProofs

/-! Axiom audit of the F0 statement over CSem (scripts/check.sh --csem):
every row within {propext, Classical.choice, Quot.sound}. The definitions'
cones include the fork's opaque native seams (opaque constants, not axioms). -/

#print axioms Tcc.TccRunCorrect_Statement
#print axioms Tcc.TccppCorrect_Statement
#print axioms Tcc.TccRunCorrectWitnessSum_Statement
#print axioms Tcc.CSem.outcomes
#print axioms Tcc.CSem.NoUB
#print axioms Tcc.CSem.verdicts
#print axioms Tcc.instDecidableSupported

-- Recovered arithmetic, the sole rounding optimization and exact byte bridge.
#print axioms CerbIEEE.Fast.shiftRight_eq
#print axioms CerbIEEE.add_spec
#print axioms CerbIEEE.sub_spec
#print axioms CerbIEEE.mul_spec
#print axioms CerbIEEE.div_spec
#print axioms CerbIEEE.sqrt_spec
#print axioms CerbIEEE.ofInt_spec
#print axioms CerbIEEE.ofBytesLE_toBytesLE
#print axioms CerbIEEE.unpack_pack_unpack
#print axioms Tcc.CSem.IEEE.RV64LibgccIEEE_Statement
#print axioms CerbIEEE.convert_not_nan
#print axioms CerbIEEE.FormatConversionValue_Statement
#print axioms Tcc.CSem.IEEE.TccLiteralIEEE_Statement
#print axioms Tcc.CSem.IEEE.NewlibIEEE_Statement

#print axioms CerbMem.allocator_default_preserved
#print axioms CerbMem.oracleAnswerOk_sound
#print axioms CerbMem.oracleAllocate_active_sound
#print axioms Tcc.CSem.verdictsWithAllocation

#print axioms CerbMem.allocator_oracle_active_sound
#print axioms CerbMem.oracleAllocate_accepts
#print axioms CerbMem.oracleAllocate_rejects
#print axioms Tcc.CSem.halts_of_allocation_resolution

#print axioms CerbMem.allocator_active_placement_sound

-- Every new allocation proof, plus the two adapted legacy bump proofs.
#print axioms Tcc.CSem.verdicts_default
#print axioms CerbMem.allocator_below_request_kills
#print axioms CerbMem.allocator_active_sound
