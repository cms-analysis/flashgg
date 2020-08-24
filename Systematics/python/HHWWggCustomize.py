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
            tagList -- Categories:  0 : SemiLeptonic electron channel
                                    1 : SemiLeptonic muon channel
                                    2 : Fully Hadronic channel (jets selection based on min W and H mass OR pT depending on input flag. min WH by default)
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

        #-- Cut flow variables
        cutFlowVars = [
            "passPS[2,0,2] := Cut_Variables[0]",
            "passPhotonSels[2,0,2] := Cut_Variables[1]",
            "passbVeto[2,0,2] := Cut_Variables[2]",
            "ExOneLep[2,0,2] := Cut_Variables[3]",
            "AtLeast2GoodJets[2,0,2] := Cut_Variables[4]",
            "AtLeast4GoodJets[2,0,2] := Cut_Variables[5]",
            "AtLeast4GoodJets0Lep[2,0,2] := Cut_Variables[6]",
            "mW1_40To160[2,0,2] := Cut_Variables[7]",
            "mW1_65To105[2,0,2] := Cut_Variables[8]",
            "mW2_0To160[2,0,2] := Cut_Variables[9]",
            "mH_105To160[2,0,2] := Cut_Variables[10]",
            "mH_40To210[2,0,2] := Cut_Variables[11]",
            "AtLeastTwoLeps:= Cut_Variables[12]",
            "TwoDiffLeps:= Cut_Variables[13]",
            "TwoGoodMuons:= Cut_Variables[14]",
            "TwoGoodEles:= Cut_Variables[15]",
            "passLepDR:= Cut_Variables[16]",
            "passMetPt:= Cut_Variables[17]"
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

        otherVariables=[
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
            "HHCandidate_phi := HH.phi()"
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
          "leading_Electron_pt                     := Leading_Electron.pt() ",
          "leading_Electron_eta                    := Leading_Electron.eta()",
          "leading_Electron_phi                    := Leading_Electron.phi()",
          "leading_Electron_E                      := Leading_Electron.E()",
          "subleading_Electron_pt                  := Subleading_Electron.pt()",
          "subleading_Electron_eta                 := Subleading_Electron.eta()",
          "subleading_Electron_phi                 := Subleading_Electron.phi()",
          "subleading_Electron_E                   := Subleading_Electron.E()",
          # If there is no leading muon (muonVector_.size() == 0) or no subleading muon (muonVector_.size() <= 1) plot -99
          "leading_muon_pt                     := leading_muon.pt() ",
          "leading_muon_eta                    := leading_muon.eta() ",
          "leading_muon_phi                    := leading_muon.phi()",
          "leading_muon_E                      := leading_muon.E()",
          "subleading_muon_pt                  := subleading_muon.pt()",
          "subleading_muon_eta                 := subleading_muon.eta()",
          "subleading_muon_phi                 := subleading_muon.phi()",
          "subleading_muon_E                   := subleading_muon.E()",
          "Met_pt                              := MET.pt()"
        ]

        vars = ["E","pt","px","py","pz","eta","phi"]
        objects = ["Leading_Photon","Subleading_Photon","Electron","Muon","MET","Leading_Jet","Subleading_Jet","Sub2leading_Jet","Sub3leading_Jet"]
        finalStateVars = []
        finalStateVars.append("Leading_Photon_MVA:=lp_Hgg_MVA")
        finalStateVars.append("Subleading_Photon_MVA:=slp_Hgg_MVA")
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
                bscores = ["bDiscriminator('mini_pfDeepFlavourJetTags:probb')","bDiscriminator('pfDeepCSVJetTags:probb')",
                           "bDiscriminator('mini_pfDeepFlavourJetTags:probbb')","bDiscriminator('pfDeepCSVJetTags:probbb')"]

                btitles = ["bDiscriminator_mini_pfDeepFlavourJetTags_probb","bDiscriminator_pfDeepCSVJetTags_probb",
                           "bDiscriminator_mini_pfDeepFlavourJetTags_probbb","bDiscriminator_pfDeepCSVJetTags_probbb"
                          ]
                for ib,bscore in enumerate(bscores):
                    btitle = btitles[ib]
                    for i in range(checkN):
                        vtitle = "%s_%s_%s"%(objV,i,btitle)
                        vname = "? %s.size() >= %s ? %s[%s].%s : -999"%(objV,i+1,objV,i,bscore)
                        entry = "%s:=%s"%(vtitle,vname)
                        finalStateVars.append(entry)

        # Save extra Muon variables
        # nMuons = 5 # highest 5 pT muons (no selections applied)
        # nVars = 6 # 5 IDs and Isolation
        # extraMuonVars = ["isLooseMuon","isMediumMuon","isTightMuon","isSoftMuon","isHighPtMuon","muonIso"]
        # for m in range(0,nMuons):
        #     for n in range(0,nVars):
        #         muonVarTitle = extraMuonVars[n]
        #         # MuonVars_[muonIndex*numVars + 0] = isLooseMuon;
        #         # MuonVars_[muonIndex*numVars + 1] = isMediumMuon;
        #         # MuonVars_[muonIndex*numVars + 2] = isTightMuon;
        #         # MuonVars_[muonIndex*numVars + 3] = isSoftMuon;
        #         # MuonVars_[muonIndex*numVars + 4] = isHighPtMuon;
        #         # MuonVars_[muonIndex*numVars + 5] = muonIso;
        #         i = m*nVars + n
        #         vname = "MuonVars[%s]"%(i)
        #         vtitle = "allMuons_%s_%s"%(m,muonVarTitle)
        #         entry = "%s:=%s"%(vtitle,vname)
        #         finalStateVars.append(entry)

        # Save extra Jet variables
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
                finalStateVars.append(entry)

        # for removal of prompt-prompt events from QCD and GJet samples
        finalStateVars.append("Leading_Photon_genMatchType:=Leading_Photon.genMatchType()")
        finalStateVars.append("Subleading_Photon_genMatchType:=Subleading_Photon.genMatchType()")

        print"len(finalStateVars):",len(finalStateVars)

        # if self.customize.doHHWWggTagCutFlow:
            # variables += cutFlowVars
            # variables += bScores

        if self.customize.saveHHWWggFinalStateVars:
            variables += otherVariables
            variables += finalStateVars
            variables += cutFlowVars
            variables += FL_vars
            # variables += bScores

        if self.customize.doHHWWggDebug:
            variables += debugVars

        return variables

        # if self.customize.dumpWorkspace == False :
        #     return variables
        # else :
        #     return var_workspace


    def systematicVariables(self):
    #   systematicVariables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass","Mjj[120,70,190]:=dijet().M()","HHbbggMVA[100,0,1.]:=MVA()","MX[300,250,5000]:=MX()"]
      systematicVariables=[
        #   "dZ",
          "CMS_hgg_mass[160,100,180]:=diPhoton().mass"
        #   "lp_E[100,0,100] := Leading_Photon.p4().E()",
        #   "slp_E[100,0,100] := Subleading_Photon.p4().E()",
        #   "lp_initE[100,0,100] := Leading_Photon.energyAtStep('initial')",
        #   "slp_initE[100,0,100] := Subleading_Photon.energyAtStep('initial')", # also want final energies
      ]

      debugVars=[
          "leadPhoMVA[2,0,2]:=lp_Hgg_MVA",
          "subleadPhoMVA[2,0,2]:=slp_Hgg_MVA"
      ]

      if self.customize.doHHWWggDebug:
        systematicVariables += debugVars



      return systematicVariables


    def variablesToDumpData():
        variables = [
            # "testVariable[100,0,100] := 50"
            # "jet0_btag[2,0,2]                       := ? JetVector.size() >= 1 ? JetVector[0].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 ",
            # "TestVariable:=111"
           #  "leadingJet_DeepCSV := leadJet().bDiscriminator('pfDeepCSVJetTags:probb')+leadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",#FIXME make the btag type configurable?
           #  "subleadingJet_DeepCSV := subleadJet().bDiscriminator('pfDeepCSVJetTags:probb')+subleadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",
           #  "absCosThetaStar_CS := abs(getCosThetaStar_CS())",
           #  "absCosThetaStar_CS_old := abs(getCosThetaStar_CS_old(6500))",
           #  "absCosTheta_bb := abs(CosThetaAngles()[1])",
           #  "absCosTheta_gg := abs(CosThetaAngles()[0])",
           #  "diphotonCandidatePtOverdiHiggsM := diphotonPtOverM()",
           #  "dijetCandidatePtOverdiHiggsM := dijetPtOverM()",
           #  "customLeadingPhotonIDMVA := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
           #  "customSubLeadingPhotonIDMVA := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
           #  "leadingPhotonSigOverE := diPhoton.leadingPhoton.sigEOverE",
           #  "subleadingPhotonSigOverE := diPhoton.subLeadingPhoton.sigEOverE",
           #  "sigmaMOverM := sqrt(0.5*(diPhoton.leadingPhoton.sigEOverE*diPhoton.leadingPhoton.sigEOverE + diPhoton.subLeadingPhoton.sigEOverE*diPhoton.subLeadingPhoton.sigEOverE))",
           #  "PhoJetMinDr := getPhoJetMinDr()",#up to here input variables to MVA
           #  "leadingJet_bRegNNResolution := leadJet().userFloat('bRegNNResolution')",
           #  "subleadingJet_bRegNNResolution := subleadJet().userFloat('bRegNNResolution')",
           #  "sigmaMJets := getSigmaMOverMJets()",
            #  "HHbbggMVA := MVA()",
            #  "MX := MX()",
           #  "Mjj := dijet().M()",
           #  "eventNumber := eventNumber()",
             ]

        # for jeti in range(0,6):
        #     var = "jet" + str(jeti) + "_btag[2,0,2] := ? JetVector.size() >= " + str(jeti + 1) + " ? JetVector[" + str(jeti) + "].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 "
        #     variables.append(var)

        # if self.customize.doHHWWggttHKiller : variables +=[
        #     "ttHScore := ttHScore()",
        #    ]
        return variables

    def customizeTagSequence(self):
        self.process.load("flashgg.Taggers.flashggHHWWggTag_cff")

        # if self.customize.doHHWWggTagCutFlow:
        if self.customize.doHHWWggTagCutFlow or self.customize.saveHHWWggFinalStateVars:  ##-- set true for either case because finalstate vars only saved during cutflow
            self.process.flashggHHWWggTag.doHHWWggTagCutFlowAnalysis = cms.bool(True)

        if self.customize.doHHWWggNonResAnalysis:
            self.process.flashggHHWWggTag.doHHWWggNonResAnalysis = cms.bool(True)
        if self.customize.doHHWWggFHptOrdered:
            self.process.flashggHHWWggTag.doHHWWggFHptOrdered = cms.bool(True)
        if self.customize.doHHWWggDebug:
            self.process.flashggHHWWggTag.doHHWWggDebug = cms.bool(True)

        # print "HHWWggAnalysisChannel:",self.customize.HHWWggAnalysisChannel
        self.process.flashggHHWWggTag.HHWWggAnalysisChannel = self.customize.HHWWggAnalysisChannel

        # if self.customize.saveHHWWggGenVars:
            # self.process.flashggHHWWggTag.saveHHWWggGenVars = cms.bool(True)

        # if self.customize.saveHHWWggFinalStateVars:
            # self.process.flashggHHWWggTag.saveHHWWggFinalStateVars = cms.bool(True)

        ## customize meta conditions
        # self.process.flashggHHWWggTag.JetIDLevel=cms.string(str(self.metaConditions["doubleHTag"]["jetID"]))
        # self.process.flashggHHWWggTag.MVAConfig.weights=cms.FileInPath(str(self.metaConditions["doubleHTag"]["weightsFile"]))
        # self.process.flashggHHWWggTag.MVAscaling = cms.double(self.metaConditions["doubleHTag"]["MVAscalingValue"])
        # self.process.flashggHHWWggTag.MVAFlatteningFileName = cms.untracked.FileInPath(str(self.metaConditions["doubleHTag"]["MVAFlatteningFileName"]))
        # self.process.flashggHHWWggTag.dottHTagger = cms.bool(self.customize.doHHWWggttHKiller)
        # self.process.flashggHHWWggTag.ttHWeightfile = cms.untracked.FileInPath(str(self.metaConditions["doubleHTag"]["ttHWeightfile"]))
        # self.process.flashggHHWWggTag.ttHKiller_mean = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_mean"])
        # self.process.flashggHHWWggTag.ttHKiller_std = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_std"])
        # self.process.flashggHHWWggTag.ttHKiller_listmean = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_listmean"])
        # self.process.flashggHHWWggTag.ttHKiller_liststd = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_liststd"])

        ## remove single Higgs tags

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



    # def HHWWggTagRunSequence(self,systlabels,jetsystlabels,phosystlabels):
        # if self.customize.saveHHWWggGenVars:
            # self.addGenAnalysis()
        # print'[HHWWggTagRunSequence]: Doing Nothing for HHWWgg'
    #    if self.customize.HHWWggTagsOnly:
        #   print'systlabels = ',systlabels

        #   self.HHWWggTagMerger(systlabels)


        ## Not sure if this is necessary for HHWWgg
    #    if len(systlabels)>1 :
    #       print'[HHWWggTagRunSequence] - Add JetesSuffixes and diphotonsuffices'
    #       getattr(self.process, "flashggHHWWggTag").JetsSuffixes = cms.vstring([systlabels[0]]+jetsystlabels)
    #       getattr(self.process, "flashggHHWWggTag").DiPhotonSuffixes = cms.vstring([systlabels[0]]+phosystlabels)





    #    if self.customize.HHWWggReweight>0:
        #   self.addNodesReweighting()

    #    if self.customize.doHHWWggGenAnalysis:
    #       self.addGenAnalysis()

    #    if self.customize.saveHHWWggGenVars:




    def addNodesReweighting(self):
        print'[addNodesReweighting]: Doing Nothing for HHWWgg'
        # if self.customize.HHWWggReweight > 0 :
        #     from flashgg.Taggers.flashggHHWWggReweight_cfi import flashggHHWWggReweight
        #     self.process.flashggHHWWggReweight = flashggHHWWggReweight
        #     self.process.flashggHHWWggReweight.doReweight = self.customize.HHWWggReweight
        #     self.process.flashggHHWWggReweight.weightsFile = cms.untracked.FileInPath(str(self.metaConditions["HHWWggTag"]["NodesReweightingFileName"]))
        #     self.process.p.replace(self.process.flashggHHWWggTag, self.process.flashggHHWWggReweight*self.process.flashggHHWWggTag)

    # def addGenAnalysis(self):
    #     if self.customize.processId == "Data":
    #         return

    #     import flashgg.Taggers.dumperConfigTools as cfgTools
    #     ## load gen-level bbgg
    #     # self.process.load( "flashgg.MicroAOD.flashggGenDiPhotonDiBJetsSequence_cff" )

    #     ## match gen-level to reco tag
    #     self.process.load("flashgg.Taggers.flashggTaggedGenDiphotons_cfi")
    #     # self.process.flashggTaggedGenDiphotons.src  = "flashggSelectedGenDiPhotonDiBJets"
    #     self.process.flashggTaggedGenDiphotons.tags = "flashggTagSorter"
    #     self.process.flashggTaggedGenDiphotons.remap = self.process.tagsDumper.classifierCfg.remap

    #     ## prepare gen-level dumper
    #     self.process.load("flashgg.Taggers.genDiphotonDumper_cfi")
    #     self.process.genDiphotonDumper.dumpTrees = True
    #     self.process.genDiphotonDumper.dumpWorkspace = False
    #     self.process.genDiphotonDumper.src = "flashggTaggedGenDiphotons"

    #     from flashgg.Taggers.globalVariables_cff import globalVariables
    #     self.process.genDiphotonDumper.dumpGlobalVariables = True
    #     self.process.genDiphotonDumper.globalVariables = globalVariables

    #     genVariables = ["mgg := mass",
    #                     "mbb := dijet.mass",
    #                     "mhh := sqrt( pow(energy+dijet.energy,2) - pow(px+dijet.px,2) - pow(py+dijet.py,2) - pow(pz+dijet.pz,2))",


    #                     "leadPho_px := leadingPhoton.px",
    #                     "leadPho_py := leadingPhoton.py",
    #                     "leadPho_pz := leadingPhoton.pz",
    #                     "leadPho_e  := leadingPhoton.energy",
    #                     "subleadPho_px := subLeadingPhoton.px",
    #                     "subleadPho_py := subLeadingPhoton.py",
    #                     "subleadPho_pz := subLeadingPhoton.pz",
    #                     "subleadPho_e  := subLeadingPhoton.energy",

    #                     "leadJet_px := leadingJet.px",
    #                     "leadJet_py := leadingJet.py",
    #                     "leadJet_pz := leadingJet.pz",
    #                     "leadJet_e  := leadingJet.energy",
    #                     "subleadJet_px := subLeadingJet.px",
    #                     "subleadJet_py := subLeadingJet.py",
    #                     "subleadJet_pz := subLeadingJet.pz",
    #                     "subleadJet_e  := subLeadingJet.energy",

    #                     ]
    #     # if self.customize.HHWWggReweight > 0:
    #     #      for num in range(0,12):
    #     #            genVariables += ["benchmark_reweight_%d := getHHbbggBenchmarkReweight(%d)"%(num,num)]
    #     #      genVariables += ["benchmark_reweight_SM := getHHbbggBenchmarkReweight(12)"]
    #     #      genVariables += ["benchmark_reweight_box := getHHbbggBenchmarkReweight(13)"]
    #     #      genVariables += ["benchmark_reweight_2017fake := getHHbbggBenchmarkReweight(14)"]

    #     ## define categories for gen-level dumper
    #     cfgTools.addCategory(self.process.genDiphotonDumper,  ## events with not reco-level tag
    #                          "NoTag", 'isTagged("flashggNoTag")',1,
    #                          variables=genVariables,
    #                          )

    #     for tag in self.tagList: ## tagged events
    #         tagName,subCats = tag
    #         # need to define all categories explicitely because cut-based classifiers do not look at sub-category number
    #         for isub in xrange(subCats):
    #             cfgTools.addCategory(self.process.genDiphotonDumper,
    #                                  "%s_%d" % ( tagName, isub ),
    #                                  'isTagged("%s") && categoryNumber == %d' % (tagName, isub),0,
    #                                  variables=genVariables##+recoVariables
    #                                  )

    #     # self.process.genp = cms.Path(self.process.flashggGenDiPhotonDiBJetsSequence*self.process.flashggTaggedGenDiphotons*self.process.genDiphotonDumper)
    #     self.process.genp = cms.Path(self.process.flashggTaggedGenDiphotons*self.process.genDiphotonDumper)
