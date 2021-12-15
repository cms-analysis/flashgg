import FWCore.ParameterSet.Config as cms

class AnomalousCouplingsCustomize():
    """
    Customizaton class for Anomalous Couplings analysis (only VBF implemented)
    """
    
    def __init__(self, process, customize, metaConditions):
        self.process = process
        self.customize = customize
        self.metaConditions = metaConditions
        self.modifyForttH = True
        self.tagList = [
            ["LOGICERROR",0], ["NOTAG",0], 
            ["RECO_VBF_Tag0",0],
        ]
        if self.customize.processId == "Data": 
            self.tagList.pop(1) ## remove NoTag for data
        self.acVariable = []
        self.tagPriorityRanges = cms.VPSet(
            cms.PSet(TagName = cms.InputTag('flashggVBFTag')),
        )
        self.customizeTagSequence()


    def variablesToDump(self):
        ws_variables = []
        ws_variables += self.acVariable 
        ws_variables += [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass",
            "dZ[40,-20.,20.]:=(tagTruth().genPV().z-diPhoton().vtx().z)",
            "NNLOPSweight[1,-999999.,999999.] := tagTruth().weight(\"NNLOPSweight\")",
            "btagReshapeNorm_TTH_LEP[1,-999999.,999999.] := weight(\"btagReshapeNorm_TTH_LEP\")",
            "btagReshapeNorm_TTH_HAD[1,-999999.,999999.] := weight(\"btagReshapeNorm_TTH_HAD\")",
            "btagReshapeNorm_THQ_LEP[1,-999999.,999999.] := weight(\"btagReshapeNorm_THQ_LEP\")",
            "centralObjectWeight[1,-999999.,999999.] := centralWeight"
        ]

        import flashgg.Taggers.VBFTagVariables as var
        new_variables = [
            "dipho_pt             := diPhoton.pt",
            "dijet_pt             := VBFMVA.dijet_pt",
        ]
        
        more_jet_vars = [
            "n_rec_jets               := GluGluHMVA.n_rec_jets",
            "dijet_leadPhi            := GluGluHMVA.dijet_leadPhi",
            "dijet_subleadPhi         := GluGluHMVA.dijet_subleadPhi",
            "dijet_leadPUMVA          := GluGluHMVA.dijet_leadPUMVA",
            "dijet_subleadPUMVA       := GluGluHMVA.dijet_subleadPUMVA",
        ]
        
        VBF_mva_probs = [
            "vbfMvaResult_prob_bkg := VBFMVA.vbfMvaResult_prob_bkg()",
            "vbfMvaResult_prob_ggH := VBFMVA.vbfMvaResult_prob_ggH()",
            "vbfMvaResult_prob_VBF := VBFMVA.vbfMvaResult_prob_VBF()",
            "vbfDnnResult_prob_bkg := VBFMVA.vbfDnnResult_prob_bkg()",
            "vbfDnnResult_prob_ggH := VBFMVA.vbfDnnResult_prob_ggH()",
            "vbfDnnResult_prob_VBF := VBFMVA.vbfDnnResult_prob_VBF()",
        ]

        all_variables = var.dipho_variables + var.dijet_variables + more_jet_vars + VBF_mva_probs

        ntup_variables = ws_variables + all_variables
    
        if self.customize.dumpWorkspace:
            return ws_variables
        else:
            return ntup_variables


    def systematicVariables(self):
        systematicVariables = [] 
        systematicVariables += self.acVariable 
        systematicVariables += [
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass"
        ]
        return systematicVariables

    def noTagVariables(self):
        noTagVariables = []
        noTagVariables += self.acVariable
        for direction in ["Up","Down"]:
            noTagVariables.append("THU_ggH_Mu%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_Mu%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_Res%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_Res%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_Mig01%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_Mig01%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_Mig12%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_Mig12%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_VBF2j%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_VBF2j%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_VBF3j%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_VBF3j%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_PT60%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_PT60%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_PT120%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_PT120%s01sigma\")" % (direction,direction))
            noTagVariables.append("THU_ggH_qmtop%s01sigma[1,-999999.,999999.] := getTheoryWeight(\"THU_ggH_qmtop%s01sigma\")" % (direction,direction))
        return noTagVariables


    def customizeTagSequence(self):
        self.process.flashggTagSequence.remove(self.process.flashggVHMetTag)
        self.process.flashggTagSequence.remove(self.process.flashggWHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggZHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggVHHadronicTag)
        self.process.flashggTagSequence.remove(self.process.flashggUntagged)
        self.process.flashggTagSequence.remove(self.process.flashggTTHHadronicTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHLeptonicTag)
        self.process.flashggTagSequence.remove(self.process.flashggTTHDiLeptonTag)
        self.process.flashggTagSequence.remove(self.process.flashggTHQLeptonicTag)
        # OPTIONS FOR VBF DUMPER
        from flashgg.Taggers.flashggVBFMVA_cff import pujidPtBin1_loose, pujidPtBin2_loose, pujidPtBin1_medium, pujidPtBin2_medium, pujidPtBin1_tight, pujidPtBin2_tight 
        
        mva_wp = {
            "none"  : [
                [],
                [],
                []
            ],
            "tight" : [
                pujidPtBin1_tight,
                pujidPtBin2_tight
            ],
            "medium": [
                pujidPtBin1_medium,
                pujidPtBin2_medium
            ],
            "loose" : [
                pujidPtBin1_loose,
                pujidPtBin2_loose
            ]
        }
        # Use JetID
        self.process.flashggVBFMVA.UseJetID      = cms.bool(True) #fixme
        self.process.flashggVBFMVA.JetIDLevel    = cms.string("Tight2017") #cms.string("Loose")
        self.process.flashggVBFMVA.DrJetPhoton   = cms.double(0.4) # this is the right number
        # Relax all selection on VBF tag
        self.process.flashggVBFTag.Boundaries             = cms.vdouble(-2.0,2.0)
        self.process.flashggVBFTag.SetArbitraryNonGoldMC  = cms.bool(False)
        self.process.flashggVBFTag.DropNonGoldData        = cms.bool(False)
        self.process.flashggVBFTag.RequireVBFPreselection = cms.bool(False)
        #OPtions for MVA
        self.process.flashggVBFMVA.rmsforwardCut = cms.double(self.customize.forwardJetRMSCut)
        self.process.flashggVBFMVA.pujidWpPtBin1 = cms.vdouble(mva_wp[self.customize.pujidWP][0])
        self.process.flashggVBFMVA.pujidWpPtBin2 = cms.vdouble(mva_wp[self.customize.pujidWP][1])
        # MVA method
        self.process.flashggVBFMVA.MVAMethod = cms.string("DNNMulti")
        self.process.flashggVBFMVA.vbfDNNpbfile = cms.FileInPath("flashgg/Taggers/data/vbfdnn_3classes_smAndCPoddAndL1_2021-09-08.pb")
