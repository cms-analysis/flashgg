import FWCore.ParameterSet.Config as cms

from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDefaultOutputCommand
tagDefaultOutputCommand = microAODDefaultOutputCommand + cms.untracked.vstring("keep *_flashgg*_*_*")
