#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("Validation")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )

from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents", -1)
customize.setDefault("targetLumi", 1.e+3)

customize.options.register('inputfiles',
                           "", # default value
                           VarParsing.VarParsing.multiplicity.list, # singleton or list
                           VarParsing.VarParsing.varType.string,          # string, int, or float
                           "inputfiles")
customize.options.register('outputfile',
                           "", # default value
                           VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                           VarParsing.VarParsing.varType.string,          # string, int, or float
                           "outputfile")

customize.parse()

from flashgg.Validation.diphoton_technical_validation_cfi import diphotonDumper

process.dumper = diphotonDumper

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
                                customize.inputfiles
                            )
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(customize.outputfile)
)

dumper_sequence = cms.Sequence(process.dumper)
process.path = cms.Path(dumper_sequence)
