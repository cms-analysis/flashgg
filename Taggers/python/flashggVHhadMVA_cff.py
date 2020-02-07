import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag
import flashgg.Taggers.PUJID_wps as pujid

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
                               # changes loose to another working point, or comment if you want to disable pujid
                               pujidWpPtBin1 = cms.vdouble([0.69, -0.35, -0.26, -0.21]), # cms.vdouble(pujid.loose[0]),
                               pujidWpPtBin2 = cms.vdouble([0.86, -0.1 , -0.05, -0.01]), # cms.vdouble(pujid.loose[1]),
                               pujidWpPtBin3 = cms.vdouble([0.95,  0.28,  0.31,  0.28]), # cms.vdouble(pujid.loose[2]), 
                               #UseLegacyMVA = cms.bool(True),
                               rmsforwardCut = cms.double(3.0), # default was 0.03 , running on loose pujid
                               MinDijetMinv  = cms.double(0.0),
                               DrJetPhoton   = cms.double(0.4), # Keep the same value for now, should be set later to 0.4
                               vhHadMVAweightfile = cms.FileInPath("flashgg/Taggers/data/vhHadDataDrivenMerged.xml")
)
