import FWCore.ParameterSet.Config as cms

# add the following lines to the module used to configure the RandomNumberGeneratorService
# process.RandomNumberGeneratorService.flashggRandomizedPhotons = cms.PSet(
#           initialSeed = cms.untracked.uint32(16253245)
#         # engineName = cms.untracked.string('TRandom3') # optional, default to HepJamesRandom if absent
#         )

flashggRandomizedPhotons = cms.EDProducer("FlashggRandomizedPhotonProducer",
                                    src = cms.InputTag("flashggPhotons"),
                                    # labels of various gaussian random numbers with mean=0, sigma=1
                                    # to be associated with the photon object
                                    labels = cms.vstring("rnd_g_E")
                                    )
