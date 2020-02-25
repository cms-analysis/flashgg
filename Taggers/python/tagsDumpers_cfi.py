import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.tagsDumpConfig_cff import tagsDumpConfig

def createTagDumper (tagName): 
    tagDumper = cms.EDAnalyzer('CutBased'+tagName+'Dumper',
                               **tagsDumpConfig.parameters_())    
    tagDumper.className = cms.untracked.string("CutBased"+tagName+"Dumper")
    tagDumper.src = cms.InputTag("flashgg"+tagName)
    tagDumper.processId = cms.string(dict[tagName])
    # split tree, histogram and datasets 
    tagDumper.nameTemplate = "$PROCESS_$SQRTS_$LABEL_$SUBCAT"

    return tagDumper

dict = {'UntaggedTag': 'untagged',
        'VBFTag': 'vbfh',                
        'TTHLeptonicTag': 'tth',
	      'THQLeptonicTag': 'thq',
        'TTHDiLeptonTag': 'tth',
        'TTHHadronicTag': 'tth',
        'VHLooseTag': 'vh',
        'VHTightTag': 'vh',
        'VHHadronicTag': 'vh',
        'ZPlusJetTag':'zjet',
        'WHLeptonicTag': 'vh',
        'VHLeptonicLooseTag': 'vh',
        'ZHLeptonicTag': 'vh'}
