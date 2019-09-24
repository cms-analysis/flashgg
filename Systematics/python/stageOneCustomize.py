import FWCore.ParameterSet.Config as cms

class StageOneCustomize():
    """
    Customizaton class for STXS stage 1 analysis
    """
    
    def __init__(self, process, customize, metaConditions):
        self.process = process
        self.customize = customize
        self.metaConditions = metaConditions
        self.tagList = [
            ["LOGICERROR",0], ["NOTAG",0], ["RECO_0J_Tag0",0], ["RECO_0J_Tag1",0], ["RECO_0J_Tag2",0],
            ["RECO_1J_PTH_0_60_Tag0",0], ["RECO_1J_PTH_0_60_Tag1",0], ["RECO_1J_PTH_60_120_Tag0",0], ["RECO_1J_PTH_60_120_Tag1",0], 
            ["RECO_1J_PTH_120_200_Tag0",0], ["RECO_1J_PTH_120_200_Tag1",0], ["RECO_1J_PTH_GT200",0], 
            ["RECO_GE2J_PTH_0_60_Tag0",0], ["RECO_GE2J_PTH_0_60_Tag1",0], ["RECO_GE2J_PTH_60_120_Tag0",0], ["RECO_GE2J_PTH_60_120_Tag1",0], 
            ["RECO_GE2J_PTH_120_200_Tag0",0], ["RECO_GE2J_PTH_120_200_Tag1",0], ["RECO_GE2J_PTH_GT200_Tag0",0], ["RECO_GE2J_PTH_GT200_Tag1",0], 
            ["RECO_VBFTOPO_JET3VETO_Tag0",0], ["RECO_VBFTOPO_JET3VETO_Tag1",0], ["RECO_VBFTOPO_JET3_Tag0",0], ["RECO_VBFTOPO_JET3_Tag1",0], ["RECO_VBFTOPO_REST",0], ["RECO_VBFTOPO_BSM",0],
            ["RECO_TTH_LEP",0], ["RECO_TTH_HAD",0] 
        ]
        if self.customize.processId == "Data": 
            self.tagList.pop(1) ## remove NoTag for data
        self.customizeTagSequence()


    def variablesToDump(self):
        ws_variables = [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            "dZ[40,-20.,20.]:=(tagTruth().genPV().z-diPhoton().vtx().z)",
            "centralObjectWeight[1,-999999.,999999.] := centralWeight",
            "stage1bin[39,-8.5,30.5] := tagTruth().HTXSstage1orderedBin"
        ]
        ntup_variables = [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            "stage1bin[39,-8.5,30.5] := tagTruth().HTXSstage1orderedBin"
        ]
    
        if self.customize.dumpWorkspace:
            return ws_variables
        else:
            return ntup_variables


    def systematicVariables(self):
        systematicVariables = [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            "stage1bin[39,-8.5,30.5] := tagTruth().HTXSstage1orderedBin"
        ]
        return systematicVariables


    def customizeTagSequence(self):
        self.process.load("flashgg.Taggers.flashggStageOneCombinedTag_cfi")

        ## remove unneeded tags
        self.process.flashggTagSequence.remove(self.process.flashggVBFDiPhoDiJetMVA)
        self.process.flashggTagSequence.remove(self.process.flashggTHQLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHDiLeptonTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHHadronicTag)
        self.process.flashggTagSequence.remove(self.process.flashggVHMetTag)
        self.process.flashggTagSequence.remove(self.process.flashggZHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggWHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggVHLeptonicLooseTag)
        self.process.flashggTagSequence.remove(self.process.flashggVHHadronicTag)
        self.process.flashggTagSequence.remove(self.process.flashggVBFTag)
        self.process.flashggTagSequence.replace(self.process.flashggUntagged,self.process.flashggStageOneCombinedTag)

        ## customize meta conditions - need to add all the category thresholds here eventually
        #self.process.flashggDoubleHTag.JetIDLevel=cms.string(str(self.metaConditions["doubleHTag"]["jetID"]))

        ## set tag priorities, stage 1 sorting
        self.process.flashggTagSorter.TagPriorityRanges = cms.VPSet(
            cms.PSet(TagName = cms.InputTag('flashggStageOneCombinedTag'))
        )
        self.process.flashggTagSorter.DoStageOneRecoTags = True

        ## set the stage one tag merging
        self.process.flashggSystTagMerger = cms.EDProducer("StageOneTagMerger",src=cms.VInputTag("flashggTagSorter"))
