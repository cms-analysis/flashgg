import flashgg.Taggers.dumperConfigTools as cfgTools
process.load("flashgg.Taggers.FlashggTagAndProbeProducer_cfi")
process.load("flashgg.Taggers.tagAndProbeDumper_cfi")    
from flashgg.Taggers.FlashggTagAndProbeProducer_cfi import flashggTagAndProbe

process.flashggTagAndProbe = flashggTagAndProbe
process.flashggTagAndProbe.diphotonsSrc = "kinDiPhotons"
process.flashggTagAndProbe.tagSelection = "%s && pt > 40 && (?hasUserCand('eleMatch')?userCand('eleMatch').passTightId:0) && hasPixelSeed && egChargedHadronIso < 20 && egChargedHadronIso/pt < 0.3" % matchTriggerPaths 
process.flashggTagAndProbe.probeSelection = "egChargedHadronIso < 20 && egChargedHadronIso/pt < 0.3"

from flashgg.Taggers.tagAndProbeDumper_cfi import tagAndProbeDumper
tagAndProbeDumper.dumpTrees = True
cfgTools.addCategories(tagAndProbeDumper,
                       [
        ("Reject", "diPhoton.mass < 50 || diPhoton.mass > 130", -1),
        ("All", "1", 0)
        # ("EBHighR9", "abs(getProbe.superCluster.eta)<1.4442 && getProbe.full5x5_r9>0.94", 0),
        # ("EBLowR9", "abs(getProbe.superCluster.eta)<1.4442 && getProbe.full5x5_r9<=0.94", 0),
        # ("EEHighR9", "abs(getProbe.superCluster.eta)>1.566 && getProbe.full5x5_r9>0.94", 0),
        # ("EELowR9", "abs(getProbe.superCluster.eta)>1.566 && getProbe.full5x5_r9<=0.94", 0)
        ],
                       variables=dumpCfg.getTnPVariables(process.flashggTagAndProbe.idSelection.variables,variables),
                       histograms=[]
                       )    
tnp_sequence = cms.Sequence(flashggTagAndProbe+tagAndProbeDumper)
process.p = cms.Path(tnp_sequence)
