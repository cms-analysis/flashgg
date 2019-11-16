import FWCore.ParameterSet.Config as cms
from HHWWggCandidateDumper_cfi import FlashggHHWWggCandidate

# from flashggHHWWggTag_cfi import flashggHHWWggTag
from flashggHHWWggCandidate_cfi import FlashggHHWWggCandidate

flashggHHWWggTagSequence = cms.Sequence( FlashggHHWWggCandidate )
