import FWCore.ParameterSet.Config as cms

# add the following lines to the module used to configure the RandomNumberGeneratorService
# process.RandomNumberGeneratorService.flashggRandomizedPhotons = cms.PSet(
#           initialSeed = cms.untracked.uint32(16253245)
#         # engineName = cms.untracked.string('TRandom3') # optional, default to HepJamesRandom if absent
#         )

flashggRandomizedMuons = cms.EDProducer("FlashggRandomizedMuonProducer",
                                    src = cms.InputTag("flashggMuons"),
                                    # labels of various random numbers
                                    # to be associated with the muon object
                                    labels = cms.vstring("rnd_u_P"),
                                    pdf = cms.string("uniform")
                                    )
