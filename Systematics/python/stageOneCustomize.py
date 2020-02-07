import FWCore.ParameterSet.Config as cms

class StageOneCustomize():
    """
    Customizaton class for STXS stage 1 analysis
    """
    
    def __init__(self, process, customize, metaConditions):
        self.process = process
        self.customize = customize
        self.metaConditions = metaConditions
        #self.modifyForttH = False
        self.modifyForttH = True
        self.tagList = [
            ["LOGICERROR",0], ["NOTAG",0], ["RECO_0J_PTH_0_10_Tag0",0], ["RECO_0J_PTH_0_10_Tag1",0], ["RECO_0J_PTH_GT10_Tag0",0], ["RECO_0J_PTH_GT10_Tag1",0],
            ["RECO_1J_PTH_0_60_Tag0",0], ["RECO_1J_PTH_0_60_Tag1",0], ["RECO_1J_PTH_60_120_Tag0",0], ["RECO_1J_PTH_60_120_Tag1",0], 
            ["RECO_1J_PTH_120_200_Tag0",0], ["RECO_1J_PTH_120_200_Tag1",0],
            ["RECO_GE2J_PTH_0_60_Tag0",0], ["RECO_GE2J_PTH_0_60_Tag1",0], ["RECO_GE2J_PTH_60_120_Tag0",0], ["RECO_GE2J_PTH_60_120_Tag1",0], 
            ["RECO_GE2J_PTH_120_200_Tag0",0], ["RECO_GE2J_PTH_120_200_Tag1",0], ["RECO_PTH_GT200_Tag0",0], ["RECO_PTH_GT200_Tag1",0], 
            ["RECO_VBFTOPO_VHHAD_Tag0",0],  ["RECO_VBFTOPO_VHHAD_Tag1",0],
            ["RECO_VBFTOPO_JET3VETO_LOWMJJ",0], ["RECO_VBFTOPO_JET3VETO_HIGHMJJ",0], ["RECO_VBFTOPO_JET3_LOWMJJ",0], ["RECO_VBFTOPO_JET3_HIGHMJJ",0], ["RECO_VBFTOPO_BSM",0],
            ["RECO_VBFLIKEGGH",0], 
            ["RECO_TTH_HAD_LOW_Tag0",0], ["RECO_TTH_HAD_LOW_Tag1",0], ["RECO_TTH_HAD_LOW_Tag2",0], ["RECO_TTH_HAD_LOW_Tag3",0],
            ["RECO_TTH_HAD_HIGH_Tag0",0], ["RECO_TTH_HAD_HIGH_Tag1",0], ["RECO_TTH_HAD_HIGH_Tag2",0], ["RECO_TTH_HAD_HIGH_Tag3",0],
            ["RECO_WH_LEP_LOW_Tag0",0], ["RECO_WH_LEP_LOW_Tag1",0], ["RECO_WH_LEP_LOW_Tag2",0], 
            ["RECO_WH_LEP_HIGH_Tag0",0], ["RECO_WH_LEP_HIGH_Tag1",0], ["RECO_WH_LEP_HIGH_Tag2",0], 
            ["RECO_ZH_LEP",0], 
            ["RECO_TTH_LEP_LOW_Tag0",0], ["RECO_TTH_LEP_LOW_Tag1",0], ["RECO_TTH_LEP_LOW_Tag2",0], ["RECO_TTH_LEP_LOW_Tag3",0],
            ["RECO_TTH_LEP_HIGH_Tag0",0], ["RECO_TTH_LEP_HIGH_Tag1",0], ["RECO_TTH_LEP_HIGH_Tag2",0], ["RECO_TTH_LEP_HIGH_Tag3",0],
            ["RECO_THQ_LEP",0]
        ]
        if self.customize.processId == "Data": 
            self.tagList.pop(1) ## remove NoTag for data
        self.tagPriorityRanges = cms.VPSet(
	          cms.PSet(TagName = cms.InputTag('flashggTHQLeptonicTag')),
            cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag')), 
            cms.PSet(TagName = cms.InputTag('flashggZHLeptonicTag')),
            cms.PSet(TagName = cms.InputTag('flashggWHLeptonicTag')),
            cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag')),   
            cms.PSet(TagName = cms.InputTag('flashggStageOneCombinedTag'))
        )
        self.customizeTagSequence()


    def variablesToDump(self):
        ws_variables = [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            "dZ[40,-20.,20.]:=(tagTruth().genPV().z-diPhoton().vtx().z)",
            "centralObjectWeight[1,-999999.,999999.] := centralWeight",
            #"stage1p1bin[50,-8.5,41.5] := tagTruth().HTXSstage1p1orderedBin"
            "stage1p2bin[57,-8.5,48.5] := tagTruth().HTXSstage1p2orderedBin"
        ]
        ntup_variables = [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            #"stage1p1bin[50,-8.5,41.5] := tagTruth().HTXSstage1p1orderedBin"
            "stage1p2bin[57,-8.5,48.5] := tagTruth().HTXSstage1p2orderedBin"
        ]
    
        if self.customize.dumpWorkspace:
            return ws_variables
        else:
            return ntup_variables


    def systematicVariables(self):
        systematicVariables = [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            #"stage1p1bin[50,-8.5,41.5] := tagTruth().HTXSstage1p1orderedBin"
            "stage1p2bin[57,-8.5,48.5] := tagTruth().HTXSstage1p2orderedBin"
        ]
        return systematicVariables


    def customizeTagSequence(self):
        self.process.load("flashgg.Taggers.flashggStageOneCombinedTag_cfi")

        ## remove unneeded tags
        self.process.flashggTagSequence.remove(self.process.flashggVBFDiPhoDiJetMVA)
        #self.process.flashggTagSequence.remove(self.process.flashggTHQLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHDiLeptonTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHHadronicTag)
        self.process.flashggTagSequence.remove(self.process.flashggVHMetTag)
        #self.process.flashggTagSequence.remove(self.process.flashggZHLeptonicTag)
        #self.process.flashggTagSequence.remove(self.process.flashggWHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggVHLeptonicLooseTag)
        self.process.flashggTagSequence.remove(self.process.flashggVHHadronicTag)
        self.process.flashggTagSequence.remove(self.process.flashggVBFTag)
        self.process.flashggTagSequence.replace(self.process.flashggUntagged,self.process.flashggStageOneCombinedTag)

        ## customize from meta conditions - category thresholds set here
        self.process.flashggStageOneCombinedTag.rawDiphoBounds = cms.vdouble( self.metaConditions["stageOneCombinedTag"]["rawDiphoBounds"] )
        self.process.flashggStageOneCombinedTag.rawDijetBounds = cms.vdouble( self.metaConditions["stageOneCombinedTag"]["rawDijetBounds"] )
        self.process.flashggStageOneCombinedTag.rawGghBounds   = cms.vdouble( self.metaConditions["stageOneCombinedTag"]["rawGghBounds"] )
        self.process.flashggStageOneCombinedTag.rawVhHadBounds = cms.vdouble( self.metaConditions["stageOneCombinedTag"]["rawVhHadBounds"] )

        ## set the pre-firing to be applied
        if bool(self.metaConditions["L1Prefiring"]["isRelevant"]):
            self.metaConditions["L1Prefiring"]["applyToCentral"] = "true"

        ## set tag priorities
        self.process.flashggTagSorter.TagPriorityRanges = self.tagPriorityRanges
        self.process.flashggTagSorter.isGluonFusion = cms.bool(bool(self.customize.processId.count("ggh")))
        self.process.flashggTagSorter.applyNNLOPSweight = cms.bool(self.customize.applyNNLOPSweight)

        ## set the tag merging
        self.process.flashggSystTagMerger = cms.EDProducer("TagMerger",src=cms.VInputTag("flashggTagSorter"))

    ## this adds in the ttH tags with their correct, modified systematics workflow
    def modifyWorkflowForttH(self, systlabels, phosystlabels, metsystlabels, jetsystlabels):
        # Set lists of systematics for each tag
        for tag in ["flashggTTHLeptonicTag", "flashggTTHHadronicTag"]:
            getattr(self.process, tag).DiPhotonSuffixes = cms.vstring(phosystlabels)
            getattr(self.process, tag).JetsSuffixes = cms.vstring(jetsystlabels)
            getattr(self.process, tag).MetSuffixes = cms.vstring(metsystlabels)
    
            getattr(self.process, tag).ModifySystematicsWorkflow = cms.bool(True)
            getattr(self.process, tag).UseLargeMVAs = cms.bool(True) # enable memory-intensive MVAs

        print 'ED DEBUG process.p before was %s'%self.process.p
        print
    
        self.process.p.remove(self.process.flashggTagSorter)
        self.process.p.replace(self.process.flashggSystTagMerger, cms.Sequence(self.process.flashggTTHLeptonicTag + self.process.flashggTTHHadronicTag)*self.process.flashggTagSorter*self.process.flashggSystTagMerger)
    
        for systlabel in systlabels:
            if systlabel == "":
                continue
            self.process.p.remove(getattr(self.process, 'flashggTagSorter' + systlabel))
            self.process.p.replace(self.process.flashggSystTagMerger, getattr(self.process, 'flashggTagSorter' + systlabel) * self.process.flashggSystTagMerger) 
            modifiedPriorityRanges = cms.VPSet(
	              cms.PSet(TagName = cms.InputTag('flashggTHQLeptonicTag')),
                cms.PSet(TagName = cms.InputTag('flashggTTHLeptonicTag', systlabel)),
                cms.PSet(TagName = cms.InputTag('flashggZHLeptonicTag')),
                cms.PSet(TagName = cms.InputTag('flashggWHLeptonicTag')),
                cms.PSet(TagName = cms.InputTag('flashggTTHHadronicTag', systlabel)),
                cms.PSet(TagName = cms.InputTag('flashggStageOneCombinedTag'))
            )
            setattr(getattr(self.process, 'flashggTagSorter'+systlabel), 'TagPriorityRanges', modifiedPriorityRanges)

        print 'ED DEBUG process.p before was %s'%self.process.p
        print
        #exit('ED DEBUG exit now')
