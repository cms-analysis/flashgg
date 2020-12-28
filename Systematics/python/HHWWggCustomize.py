import FWCore.ParameterSet.Config as cms

class HHWWggCustomize():
    """
    HH->WWgg process customizaton class
    """

    def __init__(self, process, customize, metaConditions):
        """constructor for HHWWggCustomize Class

        Arguments:
            process {[type]} -- [description]
            customize {[type]} -- [description]
            metaConditions {[type]} -- [description]

        Member functions:
            process --
            customize --
            metaConditions --
            tagList -- Categories:  0 : Semi-Leptonic  channel
                                    1 : Fully Hadronic channel (jets selection based on min W and H mass OR pT depending on input flag. min WH by default)
                                    2 : Fully-leptonic channel
                                    3 : Untagged --> Meets no criteria but want to save event to output
        """
        self.process = process
        self.customize = customize
        self.metaConditions = metaConditions
        self.tagList = [ ["HHWWggTag",4] ] # definitions above
        self.customizeTagSequence()

    def variablesToDump(self):

        ##- Variables for the nominal output tag tree (non systematic trees for each category)
        variables = []

        ##-- Gen Level Variables
        gen_vars = [
            "genMhh := genMhh()",
            "genCosThetaStar_CS := getGenCosThetaStar_CS()",           
            "genAbsCosThetaStar_CS := abs(getGenCosThetaStar_CS())"        
        ]

        #-- Cut flow variables
        cutFlowVars = [
            "passPS[2,0,2] := Cut_Variables[0]", # 
            "passPhotonSels[2,0,2] := Cut_Variables[1]", # 
            # "passbVeto[2,0,2] := Cut_Variables[2]",
            # "ExOneLep[2,0,2] := Cut_Variables[3]",
            # "AtLeast2GoodJets[2,0,2] := Cut_Variables[4]",
            # "AtLeast4GoodJets[2,0,2] := Cut_Variables[5]",
            # "AtLeast4GoodJets0Lep[2,0,2] := Cut_Variables[6]",
            "mW1_40To160[2,0,2] := Cut_Variables[7]",
            "mW1_65To105[2,0,2] := Cut_Variables[8]",
            "mW2_0To160[2,0,2] := Cut_Variables[9]",
            "mH_105To160[2,0,2] := Cut_Variables[10]",
            "mH_40To210[2,0,2] := Cut_Variables[11]",
            # "AtLeastTwoLeps:= Cut_Variables[12]",
            # "TwoDiffLeps:= Cut_Variables[13]",
            # "TwoGoodMuons:= Cut_Variables[14]",
            # "TwoGoodEles:= Cut_Variables[15]",
            # "passLepDR:= Cut_Variables[16]",
            # "passMetPt:= Cut_Variables[17]",
	        "FL_Lep_Flavor :=Cut_Variables[18]"
        ]

        #-- b scores
        bScores = []
        # for jeti in range(0,6):
            # var1 = "jet" + str(jeti) + "_DeepFlavourScore[2,0,2] := ? JetVector.size() >= " + str(jeti + 1) + " ? JetVector[" + str(jeti) + "].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 "
            # var2 = "jet" + str(jeti) + "_DeepCSVScore[2,0,2] := ? JetVector.size() >= " + str(jeti + 1) + " ? JetVector[" + str(jeti) + "].bDiscriminator('pfDeepCSVJetTags:probb') : -99 "
            # bScores.append(var1)
            # bScores.append(var2)

        # inital / final photon energies for scaling and smearing validation
        phoEs = [
          "lp_E[100,0,100] := Leading_Photon.p4().E()",
          "slp_E[100,0,100] := Subleading_Photon.p4().E()",
          "lp_initE[100,0,100] := Leading_Photon.energyAtStep('initial')",
          "slp_initE[100,0,100] := Subleading_Photon.energyAtStep('initial')", # also want final energies
        ]

        # variables for miscellaneous debugging
        debugVars=[
            "leadPhoMVA[2,0,2]:=lp_Hgg_MVA",
            "subleadPhoMVA[2,0,2]:=slp_Hgg_MVA",
            "Leading_Photon_pt:=lp_pt",
            "Subleading_Photon_pt:=slp_pt"
        ]

        ##-- W, H variables 
        HHVariables=[
            "W1Candidate_E := dijet.E()",
            "W1Candidate_M := dijet.M()",
            "W1Candidate_pt := dijet.pt()",
            "W1Candidate_px := dijet.px()",
            "W1Candidate_py := dijet.py()",
            "W1Candidate_pz := dijet.pz()",
            "W1Candidate_eta := dijet.eta()",
            "W1Candidate_phi := dijet.phi()",

            "W2Candidate_E := dijet2.E()",
            "W2Candidate_M := dijet2.M()",
            "W2Candidate_pt := dijet2.pt()",
            "W2Candidate_px := dijet2.px()",
            "W2Candidate_py := dijet2.py()",
            "W2Candidate_pz := dijet2.pz()",
            "W2Candidate_eta := dijet2.eta()",
            "W2Candidate_phi := dijet2.phi()",

            "HWWCandidate_E := HWW.E()",
            "HWWCandidate_M := HWW.M()",
            "HWWCandidate_pt := HWW.pt()",
            "HWWCandidate_px := HWW.px()",
            "HWWCandidate_py := HWW.py()",
            "HWWCandidate_pz := HWW.pz()",
            "HWWCandidate_eta := HWW.eta()",
            "HWWCandidate_phi := HWW.phi()",

            "HGGCandidate_E := HGG.E()",
            "HGGCandidate_M := HGG.M()",
            "HGGCandidate_pt := HGG.pt()",
            "HGGCandidate_px := HGG.px()",
            "HGGCandidate_py := HGG.py()",
            "HGGCandidate_pz := HGG.pz()",
            "HGGCandidate_eta := HGG.eta()",
            "HGGCandidate_phi := HGG.phi()",

            "HHCandidate_E := HH.E()",
            "HHCandidate_M := HH.M()",
            "HHCandidate_pt := HH.pt()",
            "HHCandidate_px := HH.px()",
            "HHCandidate_py := HH.py()",
            "HHCandidate_pz := HH.pz()",
            "HHCandidate_eta := HH.eta()",
            "HHCandidate_phi := HH.phi()",

            # Delta eta between two higgs
            "DeltaEta_HH := HWW.eta() - HGG.eta()",
            # Delta phi between two higgs
            "DeltaPhi_HH := HWW.phi() - HGG.phi()",
            # Delta R between two higgs
            "DeltaR_HH := sqrt((HWW.phi() - HGG.phi())*(HWW.phi() - HGG.phi()) + (HWW.eta() - HGG.eta())*(HWW.eta() - HGG.eta()))",
            # cos (HWW, HGG): cos(theta) between Higgs decaying to W's and Higgs decaying to two photons
            "cosThetaHH := cos(HWW.theta() - HGG.theta())",
            # cos (W1, W2): cos(theta) between two w-boson system
            "cosThetaWW := cos(dijet.theta() - dijet2.theta())",
            # Delta Phi between Higgs decaying to two photons and the selected leading jet for the w-candidate
            # "DeltaPhi_HGG_SelectedLeadJet := HGG.phi() - goodJets[JetIndex[0]].p4().phi()",
            # Detta R between Higgs decaying to two photons and the selected leading jet for the w-candidate
            # "DeltaR_HGG_SelectedLeadJet := sqrt( (HGG.phi() - goodJets[JetIndex[0]].p4().phi())*(HGG.phi() - goodJets[JetIndex[0]].p4().phi()) +  (HGG.eta() - goodJets[JetIndex[0]].p4().eta())*(HGG.eta() - goodJets[JetIndex[0]].p4().eta()) ) "
        ]

        FL_vars =[
          "lp_pt                               := Leading_Photon.p4().pt()",
          "lp_eta                              := Leading_Photon.p4().eta()",
          "lp_SC_eta                           := Leading_Photon.superCluster().eta()",
          "lp_phi                              := Leading_Photon.p4().phi()",
          "lp_E                                := Leading_Photon.p4().E()",
          "lp_initE                            := Leading_Photon.energyAtStep('initial')",
          "lp_r9                               := Leading_Photon.old_r9()",
          "lp_full5x5_r9                       := Leading_Photon.full5x5_r9()",
          "lp_Hgg_MVA                          := lp_Hgg_MVA()",
          "lp_passElectronVeto                 := Leading_Photon.passElectronVeto()",
          "lp_hasPixelSeed                     := Leading_Photon.hasPixelSeed",
          # Subleading Photon
          # slp = Subleading Photon
          "slp_pt                              := Subleading_Photon.p4().pt()",
          "slp_eta                             := Subleading_Photon.p4().eta()",
          "slp_SC_eta                          := Subleading_Photon.superCluster().eta()",
          "slp_phi                             := Subleading_Photon.p4().phi()",
          "slp_E                               := Subleading_Photon.p4().E()",
          "slp_initE                           := Subleading_Photon.energyAtStep('initial')",
          "slp_r9                              := Subleading_Photon.old_r9()",
          "slp_full5x5_r9                      := Subleading_Photon.full5x5_r9()",
          "slp_Hgg_MVA                         := slp_Hgg_MVA()",
          "slp_passElectronVeto                := Subleading_Photon.passElectronVeto()",
          "slp_hasPixelSeed                    := Subleading_Photon.hasPixelSeed",

          # DiPhoton(s)
        #  "n_dipho                             := diphoVector.size()",
        #  "dipho_MVA                           := dipho_MVA()",
        #  "CMS_hgg_mass                        := CMS_hgg_mass() ", # for cuts within HHWWggCandidate.cc before workspace
       #   "leading_dpho_pt                     := ? leading_dpho.pt() != 0 ? leading_dpho.pt() : -99",
        #  "leading_dpho_eta                    := ? leading_dpho.eta() != 0 ? leading_dpho.eta() : -99",
         # "leading_dpho_phi                    := ? leading_dpho.phi() != 0 ? leading_dpho.phi() : -99",
         # "leading_dpho_E                      := ? leading_dpho.E() != 0 ? leading_dpho.E() : -99",

          # Electrons
          # If there is no leading electron (electronVector_.size() == 0) or no subleading electron (electronVector_.size() <= 1) plot -99
          "Leading_lepton_pt                     := Leading_lepton.pt() ",
          "Leading_lepton_eta                    := Leading_lepton.eta()",
          "Leading_lepton_phi                    := Leading_lepton.phi()",
          "Leading_lepton_E                      := Leading_lepton.E()",
          "Subleading_lepton_pt                  := Subleading_lepton.pt()",
          "Subleading_lepton_eta                 := Subleading_lepton.eta()",
          "Subleading_lepton_phi                 := Subleading_lepton.phi()",
          "Subleading_lepton_E                   := Subleading_lepton.E()",
          # If there is no leading muon (muonVector_.size() == 0) or no subleading muon (muonVector_.size() <= 1) plot -99
          "Met_pt                              := MET.pt()"
        ]

        vars = ["E","pt","px","py","pz","eta","phi"]
        objects = ["Leading_Photon","Subleading_Photon","Electron","Muon","MET","Leading_Jet","Subleading_Jet","Sub2leading_Jet","Sub3leading_Jet"]
        finalStateVars = []
        finalStateVars.append("Leading_Photon_MVA:=lp_Hgg_MVA")
        finalStateVars.append("Subleading_Photon_MVA:=slp_Hgg_MVA")

        vertex_variables = []
        vertex_variables.append("GenVtx_z := GenVtx_z()")
        vertex_variables.append("HggVtx_z := HggVtx_z()")
        vertex_variables.append("ZeroVtx_z := ZeroVtx_z()")

        for obj in objects:
            for var in vars:
                vname = "%s.p4().%s()"%(obj,var)
                vtitle = "%s_%s"%(obj,var)
                entry = "%s:=%s"%(vtitle,vname)
                finalStateVars.append(entry)

        # Add all object vars
        # p4_variables = ["E","pt","eta","phi"]
        p4_variables = ["E","pt","px","py","pz","eta","phi"]
        checkN = 5
        # allCheck = 7
        # goodCheck = 3
        objectVectors = []
        objs = ["Electrons","Muons","Jets"]
        vecTypes = ["all","good"]
        for t in vecTypes:
            for o in objs:
                objVec = "%s%s"%(t,o)
                objectVectors.append(objVec)

        print"vecs:",objectVectors
        for objV in objectVectors:
            vtitle = "N_%s"%(objV)
            vname = "%s.size()"%(objV)
            entry = "%s:=%s"%(vtitle,vname)
            finalStateVars.append(entry)
            for v in p4_variables:
                for i in range(checkN):
                    vtitle = "%s_%s_%s"%(objV,i,v)
                    vname = "? %s.size() >= %s ? %s[%s].p4().%s() : -999"%(objV,i+1,objV,i,v)
                    entry = "%s:=%s"%(vtitle,vname)
                    finalStateVars.append(entry)
            if("Electrons" in objV):
                eVars = ["passLooseId","passMediumId","passTightId","passMVALooseId","passMVAMediumId","passMVATightId"]
                for eV in eVars:
                    for i in range(checkN):
                        vtitle = "%s_%s_%s"%(objV,i,eV)
                        vname = "? %s.size() >= %s ? %s[%s].%s() : -999"%(objV,i+1,objV,i,eV)
                        entry = "%s:=%s"%(vtitle,vname)
                        finalStateVars.append(entry)



            # if("Muons" in objV):
            #     mVars = ["pfIsolationR04().sumChargedHadronPt","pfIsolationR04().sumNeutralHadronEt","pfIsolationR04().sumPhotonEt",
            #              "pfIsolationR04().sumPUPt"]
            #     mVarTitles = ["sumChargedHadronPt","sumNeutralHadronEt","sumPhotonEt","sumPUPt"]
            #     for imV,mV in enumerate(mVars):
            #         mVarTitle = mVarTitles[imV]
            #         for i in range(checkN):
            #             vtitle = "%s_%s_%s"%(objV,i,mVarTitle)
            #             vname = "? %s.size() >= %s ? %s[%s].%s() : -999"%(objV,i+1,objV,i,mV)
            #             entry = "%s:=%s"%(vtitle,vname)
            #             finalStateVars.append(entry)

            # var1 = "jet" + str(jeti) + "_DeepFlavourScore[2,0,2] := ? JetVector.size() >= " + str(jeti + 1) + " ? JetVector[" + str(jeti) + "].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 "
            if("Jets" in objV):
                NtoCheck = 5 
                # if(objV == "goodJets"): NtoCheck = 10 # want to save more good jet information for checking btags per event 
                bscores = ["bDiscriminator('mini_pfDeepFlavourJetTags:probb')","bDiscriminator('pfDeepCSVJetTags:probb')",
                           "bDiscriminator('mini_pfDeepFlavourJetTags:probbb')","bDiscriminator('pfDeepCSVJetTags:probbb')",
                           "bDiscriminator('mini_pfDeepFlavourJetTags:problepb')"
                        #    "bDiscriminator('mini_pfDeepFlavourJetTags:probb') + bDiscriminator('mini_pfDeepFlavourJetTags:probbb') + bDiscriminator('mini_pfDeepFlavourJetTags:problepb')"
                           ]

                btitles = ["bDiscriminator_mini_pfDeepFlavourJetTags_probb","bDiscriminator_pfDeepCSVJetTags_probb",
                           "bDiscriminator_mini_pfDeepFlavourJetTags_probbb","bDiscriminator_pfDeepCSVJetTags_probbb",
                           "bDiscriminator_mini_pfDeepFlavourJetTags_problepb"
                        #    "DeepFlavourScore"
                          ]
                for ib,bscore in enumerate(bscores):
                    btitle = btitles[ib]
                    for i in range(NtoCheck):
                        vtitle = "%s_%s_%s"%(objV,i,btitle)
                        vname = "? %s.size() >= %s ? %s[%s].%s : -999"%(objV,i+1,objV,i,bscore)
                        entry = "%s:=%s"%(vtitle,vname)
                        finalStateVars.append(entry)

        # Save extra Muon variables
        muon_vars = [] 
        nMuons = 5 # highest 5 pT muons (no selections applied)
        nVars = 6 # 5 IDs and Isolation
        extraMuonVars = ["isLooseMuon","isMediumMuon","isTightMuon","isSoftMuon","isHighPtMuon","muonIso"]
        for m in range(0,nMuons):
            for n in range(0,nVars):
                muonVarTitle = extraMuonVars[n]
                i = m*nVars + n
                vname = "MuonVars[%s]"%(i)
                vtitle = "allMuons_%s_%s"%(m,muonVarTitle)
                entry = "%s:=%s"%(vtitle,vname)
                muon_vars.append(entry)

        # # extraMuonVars_shorter = ["isLooseMuon","isMediumMuon","isTightMuon","isSoftMuon","isHighPtMuon"]
        # extraMuonVars_shorter = ["isLooseMuon","isMediumMuon","isSoftMuon","isHighPtMuon"]

        # # for var in extraMuonVars_shorter:
        # for m in range(0,nMuons):
        #     for n in range(0,len(extraMuonVars_shorter)):
        #         muonVarTitle = extraMuonVars_shorter[n]
        #         vtitle = "goodMuons_%s_%s"%(m,muonVarTitle)
        #         # vtitle = "%s_%s_%s"%(objV,i,eV)
        #         if(muonVarTitle == "isTightMuon"): vname = "? goodMuons.size() >= %s ? goodMuons[%s].%s(*ZeroVertex()) : -999"%(m+1,m,muonVarTitle)
        #         else: vname = "? goodMuons.size() >= %s ? goodMuons[%s].%s() : -999"%(m+1,m,muonVarTitle)
        #         entry = "%s:=%s"%(vtitle,vname)
        #         finalStateVars.append(entry)     

        # Save extra Jet variables
        jet_vars = [] 
        nJets = 5 # highest 5 pT muons (no selections applied)
        nVars = 4 # 4 IDs
        # nVars = 5 # 4 IDs + 1 PU ID
        # nVars = 12 # 4 IDs + 8 PU ID's
        extraJetVars = ["passLoose","passTight","passTight2017","passTight2018"]
        # extraJetVars = ["passLoose","passTight","passTight2017","passTight2018","passesJetPUIdLoose"]
                        # "passesJetPuIdnone","passesJetPuIdloose","passesJetPuIdmedium","passesJetPuIdtight"
                        # "passesJetPuIdmixed","passesJetPuIdforward_loose","passesJetPuIdforward_medium","passesJetPuIdforward_tight"
                        # ]
        for j in range(0,nJets):
            for n in range(0,nVars):
                jetVarTitle = extraJetVars[n]
                #   JetVars_[jetIndex*numVars + 0] = passLoose;
                #   JetVars_[jetIndex*numVars + 1] = passTight;
                #   JetVars_[jetIndex*numVars + 2] = passTight2017;
                #   JetVars_[jetIndex*numVars + 3] = passTight2018;

                #   JetVars_[jetIndex*numVars + 4] = passesJetPuIdnone;
                #   JetVars_[jetIndex*numVars + 5] = passesJetPuIdloose;
                #   JetVars_[jetIndex*numVars + 6] = passesJetPuIdmedium;
                #   JetVars_[jetIndex*numVars + 7] = passesJetPuIdtight;
                #   JetVars_[jetIndex*numVars + 8] = passesJetPuIdmixed;
                #   JetVars_[jetIndex*numVars + 9] = passesJetPuIdforward_loose;
                #   JetVars_[jetIndex*numVars + 10] = passesJetPuIdforward_medium;
                #   JetVars_[jetIndex*numVars + 11] = passesJetPuIdforward_tight;
                i = j*nVars + n
                vname = "JetVars[%s]"%(i)
                vtitle = "allJets_%s_%s"%(j,jetVarTitle)
                entry = "%s:=%s"%(vtitle,vname)
                jet_vars.append(entry)
                # finalStateVars.append(entry)

        # for removal of prompt-prompt events from QCD and GJet samples
        finalStateVars.append("Leading_Photon_genMatchType:=Leading_Photon.genMatchType()")
        finalStateVars.append("Subleading_Photon_genMatchType:=Subleading_Photon.genMatchType()")
        finalStateVars.append("dipho_MVA                           := dipho_MVA()")
        finalStateVars.append("dipho_pt                           := dipho_pt()")

        ##-- Save Central Scale Factor values for ntuple flexibility and studies 
        PhotonScaleFactors = ["LooseMvaSF", "PreselSF", "TriggerWeight", "electronVetoSF"]
        LeptonScaleFactors = ["ElectronIDWeight", "ElectronRecoWeight", "MuonIDWeight", "MuonIsoWeight"]
        # LeptonScaleFactors = ["ElectronIDWeight", "ElectronRecoWeight", "MuonMediumIDWeight", "MuonLooseRelISOWeight"]
        # LeptonScaleFactors = ["ElectronIDWeight", "ElectronRecoWeight", "MuonTightIDWeight", "MuonTightRelISOWeight"]
        JetScaleFactors = ["JetBTagCutWeight","JetBTagReshapeWeight"]
        ScaleFactorLabels = []

        for PSF in PhotonScaleFactors: ScaleFactorLabels.append(PSF)
        for LSF in LeptonScaleFactors: ScaleFactorLabels.append(LSF)
        for JSF in JetScaleFactors: ScaleFactorLabels.append(JSF)

        ScaleFactorVariables = []
        for SF in ScaleFactorLabels:
            variableLabel = "%sCentral := weight(\"%sCentral\")"%(SF,SF)

            ##-- By default, running with systematics on saves up/down values. If not running with syst, set saving of up/down SFs here 
            if(not self.customize.doSystematics):
                variableLabelUp = "%sUp01sigma := weight(\"%sUp01sigma\")"%(SF,SF)
                variableLabelDown = "%sDown01sigma := weight(\"%sDown01sigma\")"%(SF,SF)
                ScaleFactorVariables.append(variableLabelUp)
                ScaleFactorVariables.append(variableLabelDown)

            # print"variableLabel:",variableLabel
            ScaleFactorVariables.append(variableLabel)
        ScaleFactorVariables.append("prefireWeightCentral := weight(\"prefireWeightCentral\")")
        if(not self.customize.doSystematics):
            ScaleFactorVariables.append("prefireWeightUp01sigma := weight(\"prefireWeightUp01sigma\")")
            ScaleFactorVariables.append("prefireWeightDown01sigma := weight(\"prefireWeightDown01sigma\")")  
        ScaleFactorVariables.append("DiphoCentralWeight := DiphoCentralWeight()")

        if self.customize.saveHHWWggFinalStateVars:
            variables += cutFlowVars
            variables += ScaleFactorVariables
            variables += vertex_variables
            variables += gen_vars            
            variables += finalStateVars
            variables += FL_vars
            variables += muon_vars
            variables += jet_vars 

        if self.customize.doHHWWggDebug:
            variables += debugVars

        return variables

        # if self.customize.dumpWorkspace == False :
        #     return variables
        # else :
        #     return var_workspace

    def systematicVariables(self):
        ##-- Save Scale Factor values for ntuple flexibility and studies 
        PhotonScaleFactors = ["LooseMvaSF", "PreselSF", "TriggerWeight", "electronVetoSF"]
        LeptonScaleFactors = ["ElectronIDWeight", "ElectronRecoWeight", "MuonIDWeight", "MuonIsoWeight"]
        # LeptonScaleFactors = ["ElectronIDWeight", "ElectronRecoWeight", "MuonMediumIDWeight", "MuonLooseRelISOWeight"]
        # LeptonScaleFactors = ["ElectronIDWeight", "ElectronRecoWeight", "MuonTightIDWeight", "MuonTightRelISOWeight"]
        JetScaleFactors = ["JetBTagCutWeight","JetBTagReshapeWeight"]
        ScaleFactorLabels = []

        for PSF in PhotonScaleFactors: ScaleFactorLabels.append(PSF)
        for LSF in LeptonScaleFactors: ScaleFactorLabels.append(LSF)
        for JSF in JetScaleFactors: ScaleFactorLabels.append(JSF)

        ScaleFactorVariables = []
        for SF in ScaleFactorLabels:
            variableLabel = "%sCentral := weight(\"%sCentral\")"%(SF,SF)
            variableLabelUp = "%sUp01sigma := weight(\"%sUp01sigma\")"%(SF,SF)
            variableLabelDown = "%sDown01sigma := weight(\"%sDown01sigma\")"%(SF,SF)
            ScaleFactorVariables.append(variableLabel)
            ScaleFactorVariables.append(variableLabelUp)
            ScaleFactorVariables.append(variableLabelDown)
        ScaleFactorVariables.append("prefireWeightCentral := weight(\"prefireWeightCentral\")")
        ScaleFactorVariables.append("prefireWeightUp01sigma := weight(\"prefireWeightUp01sigma\")")
        ScaleFactorVariables.append("prefireWeightDown01sigma := weight(\"prefireWeightDown01sigma\")")  
        ScaleFactorVariables.append("DiphoCentralWeight := DiphoCentralWeight()")

        systematicVariables=[
            "CMS_hgg_mass[160,100,180]:=diPhoton().mass"
        ]

        for ScaleFactorVar in ScaleFactorVariables:
            systematicVariables.append(ScaleFactorVar)

        return systematicVariables


    def variablesToDumpData():
        variables = [] 
        return variables

    def customizeTagSequence(self):
        self.process.load("flashgg.Taggers.flashggHHWWggTag_cff")

        ##-- Customize HHWWggTaggProducer Parameters Based on MetaConditions and cmsRun Flags

        ##-- Misc Parameters 
        if self.customize.HHWWgguseZeroVtx:
            self.process.flashggHHWWggTag.HHWWgguseZeroVtx = cms.bool(True)
        if self.customize.doHHWWggTagCutFlow or self.customize.saveHHWWggFinalStateVars:  ##-- set true for either case because finalstate vars only saved during cutflow
            self.process.flashggHHWWggTag.doHHWWggTagCutFlowAnalysis = cms.bool(True)
        # if self.customize.doHHWWggNonResAnalysis:
            # self.process.flashggHHWWggTag.doHHWWggNonResAnalysis = cms.bool(True)
        if self.customize.doHHWWggFHptOrdered:
            self.process.flashggHHWWggTag.doHHWWggFHptOrdered = cms.bool(True)
        if self.customize.doHHWWggFHminWHJets:
            self.process.flashggHHWWggTag.doHHWWggFHminWHJets = cms.bool(True)
        if self.customize.doHHWWggFHminWHLead2Jet:
            self.process.flashggHHWWggTag.doHHWWggFHminWHLead2Jet = cms.bool(True)
        if self.customize.doHHWWggFHminHiggsMassOnly:
            self.process.flashggHHWWggTag.doHHWWggFHminHiggsMassOnly = cms.bool(True)
        if self.customize.doHHWWggDebug:
            self.process.flashggHHWWggTag.doHHWWggDebug = cms.bool(True)
        self.process.flashggHHWWggTag.HHWWggAnalysisChannel = self.customize.HHWWggAnalysisChannel
        self.process.flashggHHWWggTag.FillUntagged = self.customize.FillUntagged

        ##-- Jets
        self.process.flashggHHWWggTag.JetIDLevel=cms.string(str(self.metaConditions["HHWWggTag"]["jetID"]))

        ##-- Electrons 
        Ele_ID_version = str(self.metaConditions["HHWWggTag"]["Ele_ID_version"])

        # Match Ele_ID_version with ID 
        ElectronID_Dict = {
            "cutBasedElectronID-Fall17-94X-V2-loose" : "passLooseId",
            "mvaEleID-Fall17-iso-V2-wp90" : "passMVAMediumId"
        }

        self.process.flashggHHWWggTag.ElectronID = cms.string(ElectronID_Dict[Ele_ID_version])

        ##-- Muons 
        MuonID = str(self.metaConditions["HHWWggTag"]["MUON_ID"])
        MuonIso = str(self.metaConditions["HHWWggTag"]["MUON_ISO"])

        # Match MUON_ISO in metaconditions with selection value 
        MuonISO_Dict = {
            "TightRel" : 0.15,
            "LooseRel" : 0.25 
        }        

        self.process.flashggHHWWggTag.MuonID = cms.string(MuonID)
        self.process.flashggHHWWggTag.muPFIsoSumRelThreshold = cms.double(MuonISO_Dict[MuonIso])     

        ##-- Remove Single Higgs Tags 
        print'Removing single Higgs tags'
        if self.customize.HHWWggTagsOnly:
            self.process.flashggTagSequence.remove(self.process.flashggVBFTag)
            self.process.flashggTagSequence.remove(self.process.flashggTTHLeptonicTag)
            self.process.flashggTagSequence.remove(self.process.flashggTTHHadronicTag)
            self.process.flashggTagSequence.remove(self.process.flashggVHEtTag)
            self.process.flashggTagSequence.remove(self.process.flashggVHLooseTag)
            self.process.flashggTagSequence.remove(self.process.flashggVHTightTag)
            self.process.flashggTagSequence.remove(self.process.flashggVHMetTag)
            self.process.flashggTagSequence.remove(self.process.flashggWHLeptonicTag)
            self.process.flashggTagSequence.remove(self.process.flashggZHLeptonicTag)
            self.process.flashggTagSequence.remove(self.process.flashggVHLeptonicLooseTag)
            self.process.flashggTagSequence.remove(self.process.flashggVHHadronicTag)
            self.process.flashggTagSequence.remove(self.process.flashggVBFMVA)
            self.process.flashggTagSequence.remove(self.process.flashggVBFDiPhoDiJetMVA)
            self.process.flashggTagSequence.remove(self.process.flashggTTHDiLeptonTag)
            self.process.flashggTagSequence.remove(self.process.flashggUntagged)
            self.process.flashggTagSequence.remove(self.process.flashggUntagged)
            self.process.flashggTagSequence.remove(self.process.flashggTHQLeptonicTag)
            self.process.flashggTagSequence.remove(self.process.flashggVHhadMVA)

        self.process.flashggTagSequence.replace(self.process.flashggTagSorter,self.process.flashggHHWWggTagSequence*self.process.flashggTagSorter)
        self.process.flashggTagSorter.TagPriorityRanges = cms.VPSet( cms.PSet(TagName = cms.InputTag('flashggHHWWggTag')) )

    def HHWWggTagMerger(self,systlabels=[]):
        self.process.p.remove(self.process.flashggTagSorter)
        self.process.p.replace(self.process.flashggSystTagMerger,self.process.flashggHHWWggTagSequence*self.process.flashggTagSorter*self.process.flashggSystTagMerger)
        # print'process.p = ',self.process.p

        for systlabel in systlabels:
           if systlabel!='':
             self.process.p.remove(getattr(self.process,'flashggTagSorter'+systlabel))
             self.process.p.replace(self.process.flashggSystTagMerger,getattr(self.process, 'flashggTagSorter'+systlabel)*self.process.flashggSystTagMerger)
           setattr(getattr(self.process, 'flashggTagSorter'+systlabel), 'TagPriorityRanges', cms.VPSet( cms.PSet(TagName = cms.InputTag('flashggHHWWggTag')) ))
        #    setattr(getattr(self.process, 'flashggTagSorter'+systlabel), 'TagPriorityRanges', cms.VPSet( cms.PSet(TagName = cms.InputTag('flashggHHWWggTag', systlabel)) ))

        # print 'from loop after:',process.flashggSystTagMerger.src



    def HHWWggTagRunSequence(self,systlabels,jetsystlabels,phosystlabels):
        
        ##-- Non Resonant reweighting
        if self.customize.doubleHReweight>0:
            print"self.customize.doubleHReweight:",self.customize.doubleHReweight
            self.addNodesReweighting()

    ##-- Non-Resonant Reweighting 
    def addNodesReweighting(self):
        if self.customize.doubleHReweight > 0 :
            # from flashgg.Taggers.flashggHHWWggReweight_cfi import flashggHHWWggReweight
            from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight
            self.process.flashggDoubleHReweight = flashggDoubleHReweight
            self.process.flashggDoubleHReweight.doReweight = self.customize.doubleHReweight
            self.process.flashggDoubleHReweight.weightsFile = cms.untracked.FileInPath(str(self.metaConditions["doubleHTag"]["NodesReweightingFileName"]))
            self.process.p.replace(self.process.flashggHHWWggTagSequence, self.process.flashggDoubleHReweight*self.process.flashggHHWWggTagSequence)
