import FWCore.ParameterSet.Config as cms

process = cms.Process("FLASHggMicroAOD")
#-------------------> BEGIN PF Jets <----------------
#Import RECO jet producer for ak4 PF and GEN jet

from RecoJets.JetProducers.ak4PFJets_cfi  import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
ak4PFJets = ak4PFJets.clone(src = 'packedPFCandidates', doAreaFastjet = True)
ak4GenJets = ak4GenJets.clone(src = 'packedGenParticles')
# The following is make patJets, but EI is done with the above
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
addJetCollection(
		process,
		postfix   = "",
		labelName = 'AK4PF',
		jetSource = cms.InputTag('ak4PFJets'),
		trackSource = cms.InputTag('unpackedTracksAndVertices'), 
		pvSource = cms.InputTag('unpackedTracksAndVertices'), 
		jetCorrections = ('AK4PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
		btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
		,algo= 'AK', rParam = 0.4
		)
#adjust MC matching
patJetGenJetMatchAK4PF.matched = "ak4GenJets"
patJetPartonMatchAK4PF.matched = "prunedGenParticles"
patJetPartons.particles = "prunedGenParticles"
#adjust PV used for Jet Corrections
patJetCorrFactorsAK4PF.primaryVertices = "offlineSlimmedPrimaryVertices"
# the following part is needed if you want to run b-tagging on the freshly made jets
# CAVEAT: it is not 100% the same b-tagging as in RECO, but performance plots are almost identical
# As tracks are not stored in miniAOD, and b-tag fwk for CMSSW < 72X does not accept candidates
# we need to recreate tracks and pv for btagging in standard reco format:
combinedSecondaryVertex.trackMultiplicityMin = 1  #needed for CMSSW < 71X
#---------> END  PF JET REPROCESSING <-------------------
