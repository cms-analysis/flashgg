import FWCore.ParameterSet.Config as cms

class DoubleHCustomize():
    """
    HH->bbgg process customizaton class
    """
    
    def __init__(self, customize, metaConditions):

        self.customize = customize
        self.metaConditions = metaConditions
        self.tagList = [ ["DoubleHTag",12] ]

        self.customizeTagSequence()

    def variablesToDump(self):
        var_workspace = [
#             "Mjj := dijet().M()"
        ]
        variables = [
            "leadingJet_bDis := leadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",#FIXME make the btag type configurable?
            "subleadingJet_bDis := subleadJet().bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags')",
            "leadingJet_DeepCSV := leadJet().bDiscriminator('pfDeepCSVJetTags:probb')+leadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",#FIXME make the btag type configurable?
            "subleadingJet_DeepCSV := subleadJet().bDiscriminator('pfDeepCSVJetTags:probb')+subleadJet().bDiscriminator('pfDeepCSVJetTags:probbb')",
            "leadingJet_puJetIdMVA := leadJet().puJetIdMVA()",
            "subleadingJet_puJetIdMVA := subleadJet().puJetIdMVA()",
            "absCosThetaStar_CS := abs(getCosThetaStar_CS(6500))",#FIXME get energy from somewhere?
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
            "HHbbggMVA := MVA()",
            # "HHbbggMVAprob0 := MVAprob()[0]",
            "MX := MX()",
            "genMhh := genMhh()",
            "Mjj := dijet().M()",
            "dijet_pt := dijet().pt",
            "dijet_eta := dijet().eta",
            "dijet_phi := dijet().phi",
            "diphoton_pt := diPhoton.pt",
            "diphoton_eta := diPhoton.eta",
            "diphoton_phi := diPhoton.phi",
            "leadingJet_btagWeight := leadJet.weight('JetBTagReshapeWeight') ",
            "subleadingJet_btagWeight := subleadJet.weight('JetBTagReshapeWeight') ",
            
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
#
#
#             "ttHMVA_MET := 0",# these variables are needed for ttH killer MVA, which has to be implemented in the producer with another mvaComputer
#             "ttHMVA_njets := 0",
#             "ttHMVA_Xtt0 :=0 ",
#             "ttHMVA_Xtt1 :=0 ",
#             "ttHMVA_MjjW0 :=0 ",
#             "ttHMVA_MjjW1 :=0 ",
#             "ttHMVA_Mjjbt0 :=0 ",
#             "ttHMVA_Mjjbt1 :=0 ",
#             "ttHMVA_leadingMuon :=0 ",
#             "ttHMVA_subleadingMuon :=0 ",
#             "ttHMVA_nmus :=0 ",
#             "ttHMVA_leadingElectron :=0 ",
#             "ttHMVA_subleadingElectron :=0 ",
#             "ttHMVA_nelecs :=0 ",
#             "ttHHHbggMVA := 0",
        ]
        if self.customize.doBJetRegression : variables +=[
                "leadingJet_bRegNNCorr := leadJet().userFloat('bRegNNCorr')",
                "leadingJet_bRegNNResolution := leadJet().userFloat('bRegNNResolution')",
                "subleadingJet_bRegNNCorr := subleadJet().userFloat('bRegNNCorr')",
                "subleadingJet_bRegNNResolution := subleadJet().userFloat('bRegNNResolution')",
                "sigmaMJets := getSigmaMOverMJets()"
        ]
        if self.customize.dumpWorkspace == False :
            return variables
        else :
            return var_workspace

    def customizeTagSequence(self):
        process.load("flashgg.Taggers.flashggDoubleHTag_cff")
        from flashgg.Taggers.flashggTags_cff import UnpackedJetCollectionVInputTag

        ## customize here (regression, kin-fit, MVA...)
        if self.customize.doBJetRegression : process.flashggDoubleHTag.JetTags = cms.VInputTag( ["bRegProducer%d" % icoll for icoll,coll in enumerate(UnpackedJetCollectionVInputTag) ] )

       # if customize.doubleHReweightTarget != -1:
       #     process.load("flashgg.Taggers.flashggDoubleHReweight_cfi")
       #     process.flashggDoubleHReweight.targetNode = customize.doubleHReweightTarget
       #     process.tagsDumper.reweight  =  cms.InputTag("flashggDoubleHReweight")

        ## remove single Higgs tags

        if self.customize.doubleHTagsOnly:
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
            process.flashggTagSequence.remove(process.flashggVBFMVA)
            process.flashggTagSequence.remove(process.flashggVBFDiPhoDiJetMVA)

            process.flashggTagSequence.replace(process.flashggUntagged, process.flashggDoubleHTagSequence)   

    def addNodesReweighting(self):
        if self.customize.doubleHReweightTarget != -1:
            from flashgg.Taggers.flashggDoubleHReweight_cfi import flashggDoubleHReweight
            process.flashggDoubleHReweight = flashggDoubleHReweight
            process.p.replace(process.tagsDumper, process.flashggDoubleHReweight*process.tagsDumper)
            process.flashggDoubleHReweight.targetNode = self.customize.doubleHReweightTarget
            process.tagsDumper.reweight  =  cms.InputTag("flashggDoubleHReweight")


    def addGenAnalysis(self):
        if self.customize.processId == "Data": 
            return 

        import flashgg.Taggers.dumperConfigTools as cfgTools
        ## load gen-level bbgg 
        process.load( "flashgg.MicroAOD.flashggGenDiPhotonDiBJetsSequence_cff" )

        ## match gen-level to reco tag
        process.load("flashgg.Taggers.flashggTaggedGenDiphotons_cfi")
        process.flashggTaggedGenDiphotons.src  = "flashggSelectedGenDiPhotonDiBJets"
        process.flashggTaggedGenDiphotons.tags = "flashggTagSorter"
        process.flashggTaggedGenDiphotons.remap = process.tagsDumper.classifierCfg.remap

        ## prepare gen-level dumper
        process.load("flashgg.Taggers.genDiphotonDumper_cfi")
        process.genDiphotonDumper.dumpTrees = True
        process.genDiphotonDumper.dumpWorkspace = False
        process.genDiphotonDumper.src = "flashggTaggedGenDiphotons"

        from flashgg.Taggers.globalVariables_cff import globalVariables
        process.genDiphotonDumper.dumpGlobalVariables = True
        process.genDiphotonDumper.globalVariables = globalVariables
        if self.customize.doubleHReweightTarget != -1:
            process.genDiphotonDumper.reweight  =  cms.InputTag("flashggDoubleHReweight")

        genVariables = ["mgg := mass",
                        "mbb := dijet.mass",
                        "mhh := sqrt( pow(energy+dijet.energy,2) - pow(px+dijet.px,2) - pow(py+dijet.py,2) - pow(pz+dijet.pz,2))",                    


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

        ## define categories for gen-level dumper
        cfgTools.addCategory(process.genDiphotonDumper,  ## events with not reco-level tag
                             "NoTag", 'isTagged("flashggNoTag")',1,
                             variables=genVariables,
                             )

        for tag in self.tagList: ## tagged events
            tagName,subCats = tag
            # need to define all categories explicitely because cut-based classifiers does not look at sub-category number
            for isub in xrange(subCats):
                cfgTools.addCategory(process.genDiphotonDumper,
                                     "%s_%d" % ( tagName, isub ), 
                                     'isTagged("%s") && categoryNumber == %d' % (tagName, isub),0,
                                     variables=genVariables##+recoVariables
                                     )

        process.genp = cms.Path(process.flashggGenDiPhotonDiBJetsSequence*process.flashggTaggedGenDiphotons*process.genDiphotonDumper)
