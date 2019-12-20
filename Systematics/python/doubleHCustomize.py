import FWCore.ParameterSet.Config as cms
from  flashgg.Systematics.flashggJetSystematics_cfi import jetSystematicsCustomize

class DoubleHCustomize():
    """
    HH->bbgg process customizaton class
    """
    
    def __init__(self, process, customize, metaConditions):
        self.process = process
        self.customize = customize
        self.metaConditions = metaConditions
        self.tagList = [ ["DoubleHTag",12] ]
        self.customizeTagSequence()

    def variablesToDump(self):
        var_workspace = []
        variables = []
        if(self.customize.doubleHTagsOnly):
            var_workspace += [
                "genMhh := genMhh()",
                "genAbsCosThetaStar_CS := abs(genCosThetaStar_CS())",
                "Mjj := dijet().M()",
                "eventNumber := eventNumber()",
                "MX := MX()",
                "HHbbggMVA := MVA()"
            ]
            variables += [
                "genMhh := genMhh()",
                "genAbsCosThetaStar_CS := abs(genCosThetaStar_CS())",
                "leadingJet_bDis := leadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",#FIXME make the btag type configurable?
                "subleadingJet_bDis := subleadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",
                "leadingJet_DeepCSV := leadJet().bDiscriminator('pfDeepCSVJetTags:probb')+leadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",#FIXME make the btag type configurable?
                "subleadingJet_DeepCSV := subleadJet().bDiscriminator('pfDeepCSVJetTags:probb')+subleadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",
                "leadingJet_DeepFlavour := leadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probb')+leadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probbb')+leadJet().bDiscriminator('mini_pfDeepFlavourJetTags:problepb')",#FIXME make the btag type configurable?
                "subleadingJet_DeepFlavour := subleadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probb')+subleadJet().bDiscriminator('mini_pfDeepFlavourJetTags:probbb')+subleadJet().bDiscriminator('mini_pfDeepFlavourJetTags:problepb')",#FIXME make the btag type configurable?
                "leadingJet_puJetIdMVA := leadJet().puJetIdMVA()",
                "subleadingJet_puJetIdMVA := subleadJet().puJetIdMVA()",
                "leadingJet_puJetIdMVA := leadJet().puJetIdMVA()",
                "subleadingJet_puJetIdMVA := subleadJet().puJetIdMVA()",
                "absCosThetaStar_CS := abs(getCosThetaStar_CS())",
                "absCosThetaStar_CS_old := abs(getCosThetaStar_CS_old(6500))",
                "absCosTheta_bb := abs(CosThetaAngles()[1])",
                "absCosTheta_gg := abs(CosThetaAngles()[0])",
                "diphotonCandidatePtOverdiHiggsM := diphotonPtOverM()",
                "dijetCandidatePtOverdiHiggsM := dijetPtOverM()",
                "customLeadingPhotonIDMVA := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
                "customSubLeadingPhotonIDMVA := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx",
                "EGMLeadingPhotonIDMVA := diPhoton.leadingPhoton.userFloat('EGMPhotonMVA')",
                "EGMSubLeadingPhotonIDMVA := diPhoton.subLeadingPhoton.userFloat('EGMPhotonMVA')",
                "leadingPhotonSigOverE := diPhoton.leadingPhoton.sigEOverE",
                "subleadingPhotonSigOverE := diPhoton.subLeadingPhoton.sigEOverE",
                "sigmaMOverM := sqrt(0.5*(diPhoton.leadingPhoton.sigEOverE*diPhoton.leadingPhoton.sigEOverE + diPhoton.subLeadingPhoton.sigEOverE*diPhoton.subLeadingPhoton.sigEOverE))",
                "sigmaMOverMDecorr := getSigmaMDecorr()",
                "PhoJetMinDr := getPhoJetMinDr()",#up to here input variables to MVA
                "PhoJetOtherDr := getPhoJetOtherDr()",
                "HHbbggMVA := MVA()",
                # "HHbbggMVAprob0 := MVAprob()[0]",
                "MX := MX()",
                "Mjj := dijet().M()",
                "dijet_pt := dijet().pt",
                "dijet_eta := dijet().eta",
                "dijet_phi := dijet().phi",
                "diphoton_pt := diPhoton.pt",
                "diphoton_eta := diPhoton.eta",
                "diphoton_phi := diPhoton.phi",
                'btagReshapeWeight := weight("JetBTagReshapeWeightCentral")',
                
                "diHiggs_pt := getdiHiggsP4().pt()",
                "diHiggs_mass := getdiHiggsP4().M()",
                "diHiggs_eta :=  getdiHiggsP4().eta()",
                "diHiggs_phi := getdiHiggsP4().phi()",
                "category := categoryNumber()",
                
                "leadingPhoton_pt := diPhoton.leadingPhoton.pt",
                "leadingPhoton_eta := diPhoton.leadingPhoton.eta",
                "leadingPhoton_phi := diPhoton.leadingPhoton.phi",
                "subleadingPhoton_pt := diPhoton.subLeadingPhoton.pt",
                "subleadingPhoton_eta := diPhoton.subLeadingPhoton.eta",
                "subleadingPhoton_phi := diPhoton.subLeadingPhoton.phi",
                
                "leadingJet_pt := leadJet().pt",
                "leadingJet_eta := leadJet().eta",
                "leadingJet_phi := leadJet().phi",
                "leadingJet_mass := leadJet().p4().M()",
                "leadingJet_hflav := leadJet().hadronFlavour()",
                "leadingJet_pflav := leadJet().partonFlavour()",
                
                "subleadingJet_pt := subleadJet().pt",
                "subleadingJet_eta := subleadJet().eta",
                "subleadingJet_phi := subleadJet().phi",
                "subleadingJet_mass := subleadJet().p4().M()",
                "subleadingJet_hflav := subleadJet().hadronFlavour()",
                "subleadingJet_pflav := subleadJet().partonFlavour()",
            ]
        if self.customize.doBJetRegression and self.customize.doubleHTagsOnly: variables +=[
                "leadingJet_bRegNNCorr := leadJet().userFloat('bRegNNCorr')",
                "leadingJet_bRegNNResolution := leadJet().userFloat('bRegNNResolution')",
                "subleadingJet_bRegNNCorr := subleadJet().userFloat('bRegNNCorr')",
                "subleadingJet_bRegNNResolution := subleadJet().userFloat('bRegNNResolution')",
                "sigmaMJets := getSigmaMOverMJets()"
        ]
        if self.customize.doubleHReweight > 0: 
            for num in range(0,12):  #12 benchmarks + 1 SM
                 variables += ["benchmark_reweight_%d := getBenchmarkReweight(%d)"%(num,num)]
                 var_workspace += ["benchmark_reweight_%d := getBenchmarkReweight(%d)"%(num,num)]
            variables += ["benchmark_reweight_SM := getBenchmarkReweight(12)"]
            variables += ["benchmark_reweight_box := getBenchmarkReweight(13)"]
            variables += ["benchmark_reweight_2017fake := getBenchmarkReweight(14)"]
            var_workspace += ["benchmark_reweight_SM := getBenchmarkReweight(12)"]
            var_workspace += ["benchmark_reweight_box := getBenchmarkReweight(13)"]
            var_workspace += ["benchmark_reweight_2017fake := getBenchmarkReweight(14)"]

        if self.customize.processId != "Data": 
            var_workspace += ['btagReshapeWeight := weight("JetBTagReshapeWeightCentral")']

        if self.customize.ttHKillerSaveInputVariables : variables += [
            "ttH_sumET := sumET()",
            "ttH_MET := MET()",
            "ttH_phiMET := phiMET()",
            "ttH_dPhi1 := dPhi1()",
            "ttH_dPhi2 := dPhi2()",
            "ttH_PhoJetMinDr := PhoJetMinDr()",
            "ttH_njets := njets()",
            "ttH_Xtt0 := Xtt0()",
            "ttH_Xtt1 := Xtt1()",
            "ttH_pte1 := pte1()",
            "ttH_pte2 := pte2()",
            "ttH_ptmu1 := ptmu1()",
            "ttH_ptmu2 := ptmu2()",
            "ttH_ptdipho := ptdipho()",
            "ttH_etae1 := etae1()",
            "ttH_etae2 := etae2()",
            "ttH_etamu1 := etamu1()",
            "ttH_etamu2 := etamu2()",
            "ttH_etadipho := etadipho()",
            "ttH_phie1 := phie1()",
            "ttH_phie2 := phie2()",
            "ttH_phimu1 := phimu1()",
            "ttH_phimu2 := phimu2()",
            "ttH_phidipho := phidipho()",
            "ttH_fabs_CosThetaStar_CS := fabs_CosThetaStar_CS()",
            "ttH_fabs_CosTheta_bb := fabs_CosTheta_bb()",
            "ttH_ptjet1 := ptjet1()",
            "ttH_ptjet2 := ptjet2()",
            "ttH_etajet1 := etajet1()",
            "ttH_etajet2 := etajet2()",
            "ttH_phijet1 := phijet1()",
            "ttH_phijet2 := phijet2()"
            ]
    
    
        if self.customize.doDoubleHttHKiller : 
             variables +=[
               "ttHScore := ttHScore()",
             ]
             var_workspace +=[
               "ttHScore := ttHScore()",
             ]

       # return var_workspace ##Only temp fix 
        if self.customize.doubleHTagDumpMinVariables or self.customize.dumpWorkspace :
            return var_workspace
        else :
            return variables


    def systematicVariables(self):
      systematicVariables=["CMS_hgg_mass[160,100,180]:=diPhoton().mass","Mjj[120,70,190]:=dijet().M()","HHbbggMVA[100,0,1.]:=MVA()","MX[300,250,5000]:=MX()","eventNumber[40,0.,1000000.]:=eventNumber()","genMhh[300,250,5000]:=genMhh()","genAbsCosThetaStar_CS[100,0,1]:=abs(genCosThetaStar_CS())",'btagReshapeWeight[100,-10.,10]:=weight("JetBTagReshapeWeightCentral")']
      
      if self.customize.doubleHReweight > 0: 
         for num in range(0,12):  #12 benchmarks
            systematicVariables += ["benchmark_reweight_%d[100,0,200] := getBenchmarkReweight(%d)"%(num,num)]
         systematicVariables+= ["benchmark_reweight_SM[100,0,200] := getBenchmarkReweight(12)"]
         systematicVariables+= ["benchmark_reweight_box[100,0,200] := getBenchmarkReweight(13)"]

      if self.customize.doDoubleHttHKiller : 
             systematicVariables +=["ttHScore[100,0,1.]:=ttHScore()"]

      return systematicVariables


    def variablesToDumpData():
        variables = [
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
             "HHbbggMVA := MVA()",
             "MX := MX()",
           #  "Mjj := dijet().M()",
           #  "eventNumber := eventNumber()",
             ]
        if self.customize.doDoubleHttHKiller : variables +=[
            "ttHScore := ttHScore()",
           ]
        return variables


    def customizeSystematics(self,systlabels,jetsystlabels,metsystlabels):
       for s in metsystlabels:
          systlabels.remove(s)
       metsystlabels = []
       if self.metaConditions['bRegression']['useBRegressionJERsf'] :
          for s in jetsystlabels:
             if "JER" in s :
                systlabels.remove(s)
                jetsystlabels.remove(s)
          if self.customize.doSystematics:
             for direction in ["Up","Down"]:
                jetsystlabels.append("JERbreg%s01sigma" % direction)
                systlabels.append("JERbreg%s01sigma" % direction)
       return systlabels,jetsystlabels,metsystlabels

    def customizeTagSequence(self):
        self.process.load("flashgg.Taggers.flashggDoubleHTag_cff")
        
        # customizing training file (with/wo Mjj) 
        training_type = 'with_Mjj' if self.customize.doubleHTagsUseMjj else 'wo_Mjj' 
        
        self.process.flashggDoubleHTag.MVAConfig.weights=cms.FileInPath(str(self.metaConditions["doubleHTag"]["weightsFile"][training_type]))  
        self.process.flashggDoubleHTag.MVAFlatteningFileName = cms.untracked.FileInPath(str(self.metaConditions["doubleHTag"]["MVAFlatteningFileName"][training_type]))
        if training_type == 'with_Mjj' :
            self.process.flashggDoubleHTag.MVABoundaries = cms.vdouble(0.33,0.56, 0.70)
            self.process.flashggDoubleHTag.MXBoundaries = cms.vdouble(250., 375.,470.,600.,250.,325.,365.,585.,250.,330.,360.,520.)
            self.process.flashggDoubleHTag.ttHScoreThreshold = cms.double(0.24)
        elif training_type == 'wo_Mjj' :
            self.process.flashggDoubleHTag.MVAConfig.variables.pop(0) 
            self.process.flashggDoubleHTag.MVABoundaries = cms.vdouble(0.30,0.54, 0.75)
            self.process.flashggDoubleHTag.MXBoundaries = cms.vdouble(250., 395.,470.,585.,250.,345.,375.,540.,250.,330.,375.,530.)
            self.process.flashggDoubleHTag.ttHScoreThreshold = cms.double(0.20)

        ## customize meta conditions
        self.process.flashggDoubleHTag.JetIDLevel=cms.string(str(self.metaConditions["doubleHTag"]["jetID"]))
        self.process.flashggDoubleHTag.MVAscaling = cms.double(self.metaConditions["doubleHTag"]["MVAscalingValue"])
        self.process.flashggDoubleHTag.dottHTagger = cms.bool(self.customize.doDoubleHttHKiller)
        self.process.flashggDoubleHTag.ttHWeightfile = cms.untracked.FileInPath(str(self.metaConditions["doubleHTag"]["ttHWeightfile"]))
        self.process.flashggDoubleHTag.ttHKiller_mean = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_mean"])
        self.process.flashggDoubleHTag.ttHKiller_std = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_std"])
        self.process.flashggDoubleHTag.ttHKiller_listmean = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_listmean"])
        self.process.flashggDoubleHTag.ttHKiller_liststd = cms.vdouble(self.metaConditions["doubleHTag"]["ttHKiller_liststd"])
        self.process.flashggDoubleHTag.MaxJetEta = cms.double(self.metaConditions["bTagSystematics"]["eta"])

        ## add double Higgs tag to the tag sequence
        #  self.process.flashggTagSequence.replace(self.process.flashggUntagged,(self.process.flashggDoubleHTag+self.process.flashggUntagged))

        ## remove single Higgs tags
        if self.customize.doubleHTagsOnly:
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
 
    def doubleHTagMerger(self,systlabels=[]):
        self.process.p.remove(self.process.flashggTagSorter)
        self.process.p.replace(self.process.flashggSystTagMerger,self.process.flashggDoubleHTagSequence*self.process.flashggTagSorter*self.process.flashggSystTagMerger)
        for systlabel in systlabels:
           if systlabel!='':
             self.process.p.remove(getattr(self.process,'flashggTagSorter'+systlabel))
             self.process.p.replace(self.process.flashggSystTagMerger,getattr(self.process, 'flashggTagSorter'+systlabel)*self.process.flashggSystTagMerger)
           setattr(getattr(self.process, 'flashggTagSorter'+systlabel), 'TagPriorityRanges', cms.VPSet( cms.PSet(TagName = cms.InputTag('flashggDoubleHTag', systlabel)) ))
        #print 'from loop after:',process.flashggSystTagMerger.src


    def doubleHTagRunSequence(self,systlabels,jetsystlabels,phosystlabels):
       if self.customize.doubleHTagsOnly: 
          self.doubleHTagMerger(systlabels)

       if len(systlabels)>1 :
          getattr(self.process, "flashggDoubleHTag").JetsSuffixes = cms.vstring([systlabels[0]]+jetsystlabels)
          getattr(self.process, "flashggDoubleHTag").DiPhotonSuffixes = cms.vstring([systlabels[0]]+phosystlabels)

       if self.customize.doubleHReweight>0:
          self.addNodesReweighting()
    
       if self.customize.doDoubleHGenAnalysis:
          self.addGenAnalysis()



    def addNodesReweighting(self):
        if self.customize.doubleHReweight > 0 :
            from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight
            self.process.flashggDoubleHReweight = flashggDoubleHReweight
            self.process.flashggDoubleHReweight.doReweight = self.customize.doubleHReweight
            self.process.flashggDoubleHReweight.weightsFile = cms.untracked.FileInPath(str(self.metaConditions["doubleHTag"]["NodesReweightingFileName"]))
            self.process.p.replace(self.process.flashggDoubleHTag, self.process.flashggDoubleHReweight*self.process.flashggDoubleHTag)


    def addGenAnalysis(self):
        if self.customize.processId == "Data": 
            return 

        import flashgg.Taggers.dumperConfigTools as cfgTools
        ## load gen-level bbgg 
        self.process.load( "flashgg.MicroAOD.flashggGenDiPhotonDiBJetsSequence_cff" )

        ## match gen-level to reco tag
        self.process.load("flashgg.Taggers.flashggTaggedGenDiphotons_cfi")
        self.process.flashggTaggedGenDiphotons.src  = "flashggSelectedGenDiPhotonDiBJets"
        self.process.flashggTaggedGenDiphotons.tags = "flashggTagSorter"
        self.process.flashggTaggedGenDiphotons.remap = self.process.tagsDumper.classifierCfg.remap
        self.process.flashggTaggedGenDiphotons.ForceGenDiphotonProduction = self.customize.ForceGenDiphotonProduction

        ## prepare gen-level dumper
        self.process.load("flashgg.Taggers.genDiphotonDumper_cfi")
        self.process.genDiphotonDumper.dumpTrees = True
        self.process.genDiphotonDumper.dumpWorkspace = False
        self.process.genDiphotonDumper.src = "flashggTaggedGenDiphotons"

        from flashgg.Taggers.globalVariables_cff import globalVariables
        self.process.genDiphotonDumper.dumpGlobalVariables = True
        self.process.genDiphotonDumper.globalVariables = globalVariables

        genVariables = ["absCosThetaStar_CS := abs(getcosthetaHHgen())",
                        "mhh := getmHHgen()",
                        "ptH1 := getptH1gen()",
                        "ptH2 := getptH2gen()"
                       ]

        if not self.customize.ForceGenDiphotonProduction:
            genVariables += ["mgg := mass",
                             "mbb := dijet.mass",
                             
                             "leadPho_px := leadingPhoton.px",
                             "leadPho_py := leadingPhoton.py",
                             "leadPho_pz := leadingPhoton.pz",
                             "leadPho_e  := leadingPhoton.energy",
                             "subleadPho_px := subLeadingPhoton.px",
                             "subleadPho_py := subLeadingPhoton.py",
                             "subleadPho_pz := subLeadingPhoton.pz",
                             "subleadPho_e  := subLeadingPhoton.energy",
                             
                             "leadJet_px := leadingJet.px",
                             "leadJet_py := leadingJet.py",
                             "leadJet_pz := leadingJet.pz",
                             "leadJet_e  := leadingJet.energy",
                             "subleadJet_px := subLeadingJet.px",
                             "subleadJet_py := subLeadingJet.py",
                             "subleadJet_pz := subLeadingJet.pz",
                             "subleadJet_e  := subLeadingJet.energy",
                            ]
            
        if self.customize.doubleHReweight > 0: 
             for num in range(0,12):
                   genVariables += ["benchmark_reweight_%d := getHHbbggBenchmarkReweight(%d)"%(num,num)]
             genVariables += ["benchmark_reweight_SM := getHHbbggBenchmarkReweight(12)"]
             genVariables += ["benchmark_reweight_box := getHHbbggBenchmarkReweight(13)"]
             genVariables += ["benchmark_reweight_2017fake := getHHbbggBenchmarkReweight(14)"]

        ## define categories for gen-level dumper
        cfgTools.addCategory(self.process.genDiphotonDumper,  ## events with not reco-level tag
                             "NoTag", 'isTagged("flashggNoTag")',1,
                             variables=genVariables,
                             )

        for tag in self.tagList: ## tagged events
            tagName,subCats = tag
            # need to define all categories explicitely because cut-based classifiers does not look at sub-category number
            for isub in xrange(subCats):
                cfgTools.addCategory(self.process.genDiphotonDumper,
                                     "%s_%d" % ( tagName, isub ), 
                                     'isTagged("%s") && categoryNumber == %d' % (tagName, isub),0,
                                     variables=genVariables##+recoVariables
                                     )

        self.process.genp = cms.Path(self.process.flashggGenDiPhotonDiBJetsSequence*self.process.flashggTaggedGenDiphotons*self.process.genDiphotonDumper)
