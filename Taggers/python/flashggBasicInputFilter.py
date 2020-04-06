import FWCore.ParameterSet.Config as cms

flashggBasicInputFilter = cms.EDFilter(
    "GenericDiPhotonCandidateSelector",
    src = cms.InputTag("flashggDiPhotons"),
    rho = cms.InputTag("fixedGridRhoAll"),
    cut = cms.string(
        "leadingPhoton.full5x5_r9 >= 0. && subLeadingPhoton.full5x5_r9 >= 0."
        ),
    categories = cms.VPSet(cms.PSet(cut = cms.string("full5x5_r9>0."), selection=cms.VPSet(cms.PSet(min=cms.string("0.0"))))),
    variables = cms.vstring("full5x5_r9")
    )
