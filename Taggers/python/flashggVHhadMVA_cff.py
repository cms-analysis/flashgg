import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
from flashgg.Taggers.flashggVBFMVA_cff import pujidPtBin1_loose, pujidPtBin2_loose, pujidPtBin1_medium, pujidPtBin2_medium, pujidPtBin1_tight, pujidPtBin2_tight 

# legacy VHhad MVA
flashggVHhadMVA = cms.EDProducer('FlashggVHhadMVAProducer',
                               DiPhotonTag=cms.InputTag('flashggPreselectedDiPhotons'),
                               #JetTag=cms.InputTag('flashggSelectedJets'),
                               inputTagJets= UnpackedJetCollectionVInputTag,
                               MVAMethod = cms.string("BDTG"),
                               #MVAMethod = cms.string("Multi"),
                               UsePuJetID  = cms.bool(False),
                               UseJetID    = cms.bool(True),
                               merge3rdJet = cms.bool(False),
                               thirdJetDRCut = cms.double(1.8),
                               JetIDLevel    = cms.string("Tight2017"),
                               ## define the pujid working point 
                               pujidWpPtBin1 = cms.vdouble(pujidPtBin1_tight), ## WP for 20 < pT < 30 
                               pujidWpPtBin2 = cms.vdouble(pujidPtBin2_tight), ## WP for 30 < pT < 50
                               #UseLegacyMVA = cms.bool(True),
                               rmsforwardCut = cms.double(3.0), # default was 0.03 , running on loose pujid
                               MinDijetMinv  = cms.double(0.0),
                               DrJetPhoton   = cms.double(0.4), # Keep the same value for now, should be set later to 0.4
                               vhHadMVAweightfile = cms.FileInPath("flashgg/Taggers/data/STXSmodels/vhHadDataDrivenMerged.xml")
)
