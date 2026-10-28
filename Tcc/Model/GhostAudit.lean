import Tcc.Model.MultiTU
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

#print axioms Tcc.Model.Ghost.MultiTUConstruction_Statement
#print axioms Tcc.Model.Ghost.MultiTUErasure_Statement

#print axioms Tcc.Model.Ghost.DeferredFunctionErasure_Statement

#print axioms Tcc.Model.Ghost.isolatedGhostSyntax_erasure
#print axioms Tcc.Model.Ghost.deferredFunctionErasure

-- Pure physical-transform proofs; read/refill and whole-run contracts remain open.
#print axioms Tcc.Model.Ghost.PhysicalProof.cell_start
#print axioms Tcc.Model.Ghost.PhysicalProof.cell_bounds
#print axioms Tcc.Model.Ghost.PhysicalProof.array_push_cons
#print axioms Tcc.Model.Ghost.PhysicalProof.byte_push_cons
#print axioms Tcc.Model.Ghost.PhysicalProof.loop_trace
#print axioms Tcc.Model.Ghost.PhysicalProof.table_matcher_eq
#print axioms Tcc.Model.Ghost.PhysicalProof.block_loop
#print axioms Tcc.Model.Ghost.PhysicalProof.listCells_nil
#print axioms Tcc.Model.Ghost.PhysicalProof.listCells_one
#print axioms Tcc.Model.Ghost.PhysicalProof.listCells_pair
#print axioms Tcc.Model.Ghost.PhysicalProof.listCells_nonQ
#print axioms Tcc.Model.Ghost.PhysicalProof.replacement_spec
#print axioms Tcc.Model.Ghost.PhysicalProof.replacement_not_newline
#print axioms Tcc.Model.Ghost.PhysicalProof.normalize_nonCR
#print axioms Tcc.Model.Ghost.PhysicalProof.listCells_head_nonLF
#print axioms Tcc.Model.Ghost.PhysicalProof.normalize_CR
#print axioms Tcc.Model.Ghost.PhysicalProof.advance_offset
#print axioms Tcc.Model.Ghost.PhysicalProof.block_trace
#print axioms Tcc.Model.Ghost.PhysicalProof.block_erasure
#print axioms Tcc.Model.Ghost.PhysicalProof.block_shape
#print axioms Tcc.Model.Ghost.PhysicalProof.get_drop
#print axioms Tcc.Model.Ghost.PhysicalProof.suffix_length
#print axioms Tcc.Model.Ghost.PhysicalProof.suffix_drop
#print axioms Tcc.Model.Ghost.PhysicalProof.cells_suffix
#print axioms Tcc.Model.Ghost.PhysicalProof.list_read_view
#print axioms Tcc.Model.Ghost.PhysicalProof.byteList_loop
#print axioms Tcc.Model.Ghost.PhysicalProof.byteList
#print axioms Tcc.Model.Ghost.PhysicalProof.get_bounded
#print axioms Tcc.Model.Ghost.PhysicalProof.block_bytes_list
#print axioms Tcc.Model.Ghost.PhysicalProof.cell_slice
#print axioms Tcc.Model.Ghost.PhysicalProof.cell_properties
#print axioms Tcc.Model.Ghost.PhysicalProof.trace_properties
#print axioms Tcc.Model.Ghost.PhysicalProof.annotate_raw
#print axioms Tcc.Model.Ghost.PhysicalProof.block_raw
#print axioms Tcc.Model.Ghost.PhysicalProof.origin_map
#print axioms Tcc.Model.Ghost.PhysicalProof.read_view
#print axioms Tcc.Model.Ghost.physicalBlockErasure
#print axioms Tcc.Model.Ghost.physicalBlockShape
#print axioms Tcc.Model.Ghost.physicalOriginMap
#print axioms Tcc.Model.Ghost.physicalReadView
