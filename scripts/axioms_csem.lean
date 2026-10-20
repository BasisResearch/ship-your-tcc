import Tcc.CSem.Statement

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
