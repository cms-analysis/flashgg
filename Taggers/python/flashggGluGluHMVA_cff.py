import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.Taggers.flashggVBFMVA_cff import pujidPtBin1_loose, pujidPtBin2_loose, pujidPtBin1_medium, pujidPtBin2_medium, pujidPtBin1_tight, pujidPtBin2_tight 

# legacy GluGluH MVA
flashggGluGluHMVA = cms.EDProducer('FlashggGluGluHMVAProducer',
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               DiPhotonMVATag=cms.InputTag('flashggDiPhotonMVA'),
                               MVAMethod = cms.string("Multi"),
                               UsePuJetID  = cms.bool(False),
                               UseJetID    = cms.bool(True),
                               merge3rdJet = cms.bool(False),
                               thirdJetDRCut = cms.double(1.8),
                               JetIDLevel    = cms.string("Tight2017"),
                               ## define the pujid working point 
                               pujidWpPtBin1 = cms.vdouble(pujidPtBin1_tight), ## WP for 20 < pT < 30 
                               pujidWpPtBin2 = cms.vdouble(pujidPtBin2_tight), ## WP for 30 < pT < 50
                               rmsforwardCut = cms.double(3.0), # default was 0.03 , running on loose pujid
                               MinDijetMinv  = cms.double(0.0),
                               DrJetPhoton   = cms.double(0.4), # Keep the same value for now, should be set later to 0.4
                               ggHMVAweightfile = cms.FileInPath("flashgg/Taggers/data/STXSmodels/nClassesGGHModel.xml")
)
