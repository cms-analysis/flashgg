import FWCore.ParameterSet.Config as cms

from flashggDoubleHTag_cfi import flashggDoubleHTag
from flashggVBFDoubleHTag_cfi import flashggVBFDoubleHTag


flashggDoubleHTagSequence = cms.Sequence( flashggVBFDoubleHTag* flashggDoubleHTag )
