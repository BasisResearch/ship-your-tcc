import Tcc.Model.CompilerOpening
-- Definitions, open obligations and the local replay theorem; parser correctness remains open.
#print axioms Tcc.Model.Ghost.runRaw
#print axioms Tcc.Model.Ghost.build
#print axioms Tcc.Model.Ghost.Erasure_Statement
#print axioms Tcc.Model.Ghost.ParserAgreement_Statement
#print axioms Tcc.Model.Ghost.RawErasure_Statement
#print axioms Tcc.Model.Ghost.RawConstruction_Statement
#print axioms Tcc.Model.Ghost.RawParserAgreement_Statement
#print axioms Tcc.Model.Ghost.NormalizationAgreement_Statement

#print axioms Tcc.Model.Ghost.PhysicalTransformObligations
#print axioms Tcc.Model.Ghost.PhysicalOriginMap_Statement

#print axioms Tcc.Model.Ghost.PhysicalReadView_Statement

#print axioms Tcc.Model.Ghost.InjectedAuthorityTransport_Statement
#print axioms Tcc.Model.Ghost.InjectedAuthorityErasure_Statement

#print axioms Tcc.Model.Ghost.AssociationReplayErasure_Statement

-- Local universal snapshot restoration; no parser-success assumptions.
#print axioms Tcc.Model.Ghost.erase_syntaxFields
#print axioms Tcc.Model.Ghost.associationReplayErasure
