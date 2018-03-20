import FWCore.ParameterSet.Config as cms

def variablesToDump(customize):
    return []

def tagList(customize,process):
    return [ ["DoubleHTag",6] ]


def customizeTagSequence(customize,process)
    process.load("flashgg.Taggers.flashggDoubleHTag_cff")

    ## customize here (regression, kin-fit, MVA...)
    
    process.flashggTagSequence.extend( process.flashggDoubleHTagSequence )

    if customize.doubleHTagsOnly:
        process.flashggTagSequence.remove(process.flashggVBFTag)
        process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
        process.flashggTagSequence.remove(process.flashggTTHHadronicTag)
        process.flashggTagSequence.remove(process.flashggVHEtTag)
        process.flashggTagSequence.remove(process.flashggVHLooseTag)
        process.flashggTagSequence.remove(process.flashggVHTightTag)
        process.flashggTagSequence.remove(process.flashggVHMetTag)
        process.flashggTagSequence.remove(process.flashggWHLeptonicTag)
        process.flashggTagSequence.remove(process.flashggZHLeptonicTag)
        process.flashggTagSequence.remove(process.flashggVHLeptonicLooseTag)
        process.flashggTagSequence.remove(process.flashggVHHadronicTag)
    
