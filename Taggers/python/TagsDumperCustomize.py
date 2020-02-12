import FWCore.ParameterSet.Config as cms

def customizeTagsDumper(process, customize):
    if customize.doStageOne:
        process.load("flashgg.Taggers.stageOneDiphotonTagDumper_cfi")
        process.tagsDumper.className = "StageOneDiPhotonTagDumper" 
    else:
        process.load("flashgg.Taggers.diphotonTagDumper_cfi")
        process.tagsDumper.className = "DiPhotonTagDumper"
    
    process.tagsDumper.src = "flashggSystTagMerger"
    process.tagsDumper.processId = "test"
    process.tagsDumper.dumpTrees = customize.dumpTrees
    process.tagsDumper.dumpWorkspace = customize.dumpWorkspace
    process.tagsDumper.dumpHistos = False
    process.tagsDumper.quietRooFit = True
    process.tagsDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")
    process.tagsDumper.splitPdfByStage0Bin = cms.untracked.bool(customize.doHTXS)
    process.tagsDumper.splitPdfByStage1Bin = cms.untracked.bool(customize.doStageOne)
    
    if customize.options.WeightName:
        lheProduct = customize.dataset[1]["LHESourceName"].split("_")
        process.tagsDumper.LHEEventProduct = cms.untracked.InputTag( str(lheProduct[1]) , str(lheProduct[2]) , str(lheProduct[3]) )
        process.tagsDumper.LHEWeightName = cms.untracked.string(customize.options.WeightName)
