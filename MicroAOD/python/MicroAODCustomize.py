import os
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
class MicroAODCustomize(object):

    def __init__(self,*args,**kwargs):
        
        super(MicroAODCustomize,self).__init__()
    
        self.options = VarParsing.VarParsing()
        
        self.options.register ('fileNames',
                                "", # default value
                               VarParsing.VarParsing.multiplicity.list, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "fileNames")
        self.options.register ('datasetName',
                               "", # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "datasetName")
        self.options.register ('processType',
                               "", # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "processType")
        self.options.register('debug',
                              0, # default value
                              VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                              VarParsing.VarParsing.varType.int,          # string, int, or float
                              "debug")
        self.options.register('hlt',
                              0, # default value
                              VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                              VarParsing.VarParsing.varType.int,          # string, int, or float
                              "hlt")
        self.options.register('muMuGamma',
                              2, # 0 never, 1 always, 2 for DY and DoubleMuon
                              VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                              VarParsing.VarParsing.varType.int,          # string, int, or float
                              "muMuGamma")
        self.options.register ('globalTag',
                               "", # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "globalTag")
        self.options.register ('timing',
                               0,
                              VarParsing.VarParsing.multiplicity.singleton,
                              VarParsing.VarParsing.varType.int,
                               'timing')
        self.options.register ('puppi',
                               0,
                              VarParsing.VarParsing.multiplicity.singleton,
                              VarParsing.VarParsing.varType.int,
                               'puppi')
        self.options.register ('bunchSpacing',
                               25,
                               VarParsing.VarParsing.multiplicity.singleton,
                               VarParsing.VarParsing.varType.int,
                               'bunchSpacing'
                               )
        self.options.register ('runDec2016Regression',
                               0,
                               VarParsing.VarParsing.multiplicity.singleton,
                               VarParsing.VarParsing.varType.int,
                               'runDec2016Regression'
                               )
        self.options.register('runSummer16EleID',
                              1,
                               VarParsing.VarParsing.multiplicity.singleton,
                               VarParsing.VarParsing.varType.int,
                              'runSummer16EleID'
                              )
        self.options.register('runSummer16EGMPhoID',
                              1,
                               VarParsing.VarParsing.multiplicity.singleton,
                               VarParsing.VarParsing.varType.int,
                              'runSummer16EGMPhoID'
                              )

        self.parsed_ = False

    def __getattr__(self,name):
        ## did not manage to inherit from VarParsing, because of some issues in __init__
        ## this allows to use VarParsing methods on JobConfig
        if hasattr(self.options,name):
            return getattr(self.options,name)
        
        raise AttributeError
    
    def __call__(self,process):
        self.customize(process)
        self.userCustomize(process)
    
    # empty default definition for userCustomize
    def userCustomize(self,process):
        pass 

    def parse(self):
        if self.parsed_:
            return
        self.options.parseArguments()
        self.parsed_ = True

    # process customization
    def customize(self,process):
        self.parse()

        if self.puppi == 0:
            self.customizePFCHS(process)
            self.customizeRemovePuppi(process)
        elif self.puppi == 1:
            self.customizePuppi(process)
            self.customizeRemovePFCHS(process)
        else: # e.g. 2                                                                                                                               
            self.customizePFCHS(process)
            self.customizePuppi(process)
        if self.processType == "data":
            self.customizeData(process)
            if "Mu" in customize.datasetName:
                self.customizeDataMuons(process)
        elif "sig" in self.processType.lower():
            self.customizeSignal(process)
            if "tth" in customize.datasetName.lower():
                self.customizeTTH(process)
            elif "vh" in customize.datasetName.lower():
                self.customizeVH(process)
            elif "ggh" in customize.datasetName.lower() or "glugluh" in customize.datasetName.lower():
                self.customizeGGH(process)
            elif "vbf" in customize.datasetName.lower():
                self.customizeVBF(process)
            elif "thq" in customize.datasetName.lower() or "thw" in customize.datasetName.lower():
                self.customizeTH(process)
            else:
                raise Exception,"processType=sig but datasetName does not contain recognized production mechanism - see MicroAODCustomize.py"
        if self.processType == "background" or self.processType == "bkg":
            self.customizeBackground(process)
            if "thq" in customize.datasetName.lower() or "thw" in customize.datasetName.lower():
                raise Exception,"TH samples should now be classfied as signal - see MicroAODCustomize.py"
        if self.debug == 1:
            self.customizeDebug(process)
        if self.hlt == 1:
            self.customizeHLT(process)
        if self.muMuGamma == 1:
            self.customizeMuMuGamma(process)
        elif self.muMuGamma == 2 and ("DY" in customize.datasetName or "DoubleMuon" in customize.datasetName):
            self.customizeMuMuGamma(process)
        if "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" in customize.datasetName:
            self.customizePDFs(process)
        if len(self.globalTag) >0:
            self.customizeGlobalTag(process)
        if len(self.fileNames) >0:
            self.customizeFileNames(process)
        if self.timing == 1:
            self.customizeTiming(process)
        if os.environ["CMSSW_VERSION"].count("CMSSW_7_6"):
            self.customize76X(process)
        elif os.environ["CMSSW_VERSION"].count("CMSSW_8_0"):
            self.customize80X(process)
        if self.bunchSpacing == 25:
            pass #default
        elif self.bunchSpacing == 50:
            self.customize50ns(process)
        else:
            raise Exception,"Only bunchSpacing=25 and bunchSpacing=50 are supported"
        if self.runDec2016Regression:
            self.customizeDec2016Regression(process)
        if self.runSummer16EleID:
            self.customizeSummer16EleID(process)
        else:
            self.customizeSpring15EleID(process)
        if self.runSummer16EGMPhoID:
            self.customizeSummer16EGMPhoID(process)
        if os.environ["CMSSW_VERSION"].count("CMSSW_9_2"):
            self.customize92X( process ) # Needs to come after egm
        print "Final customized process:",process.p
            
    # signal specific customization
    def customizeSignal(self,process):
        process.flashggGenPhotonsExtra.defaultType = 1
        from flashgg.MicroAOD.flashggMet_RunCorrectionAndUncertainties_cff import runMETs,setMetCorr
        runMETs(process,True) #isMC
        from flashgg.MicroAOD.METcorr_multPhiCorr_80X_sumPt_cfi import multPhiCorr_MC_DY_80X
        setMetCorr(process,multPhiCorr_MC_DY_80X)
        process.p *=process.flashggMetSequence
        
        process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
        process.rivetProducerHTXS = cms.EDProducer('HTXSRivetProducer',
                                                   HepMCCollection = cms.InputTag('myGenerator','unsmeared'),
                                                   ProductionMode = cms.string('PRODUCTIONMODENOTSET'),
                                                   )
        process.mergedGenParticles = cms.EDProducer("MergedGenParticleProducer",
                                                    inputPruned = cms.InputTag("prunedGenParticles"),
                                                    inputPacked = cms.InputTag("packedGenParticles"),
                                                    )
        process.myGenerator = cms.EDProducer("GenParticles2HepMCConverterHTXS",
                                             genParticles = cms.InputTag("mergedGenParticles"),
                                             genEventInfo = cms.InputTag("generator"),
                                             )
        process.p *= process.mergedGenParticles
        process.p *= process.myGenerator
        process.p *= process.rivetProducerHTXS
        process.out.outputCommands.append("keep *_rivetProducerHTXS_*_*")
        self.customizePDFs(process)

    def customizePDFs(self,process):     
        process.load("flashgg/MicroAOD/flashggPDFWeightObject_cfi")
        process.p *= process.flashggPDFWeightObject

    # background specific customization
    def customizeBackground(self,process):
        if not os.environ["CMSSW_VERSION"].count("CMSSW_9"):
            from flashgg.MicroAOD.flashggMet_RunCorrectionAndUncertainties_cff import runMETs,setMetCorr
            runMETs(process,True) #isMC
            from flashgg.MicroAOD.METcorr_multPhiCorr_80X_sumPt_cfi import multPhiCorr_MC_DY_80X
            setMetCorr(process,multPhiCorr_MC_DY_80X)
            process.p *=process.flashggMetSequence
        if "sherpa" in self.datasetName:
            process.flashggGenPhotonsExtra.defaultType = 1
            
    # data specific customization
    def customizeData(self,process):
        ## remove MC-specific modules
        modules = process.flashggMicroAODGenSequence.moduleNames()
        from flashgg.MicroAOD.flashggMet_RunCorrectionAndUncertainties_cff import runMETs,setMetCorr
        runMETs(process,False) #!isMC
        if "2016G" in customize.datasetName or "2016H" in customize.datasetName:
            from flashgg.MicroAOD.METcorr_multPhiCorr_80X_sumPt_cfi import multPhiCorr_Data_G_80X
            setMetCorr(process,multPhiCorr_Data_G_80X)
        elif "2016" in customize.datasetName:
            from flashgg.MicroAOD.METcorr_multPhiCorr_80X_sumPt_cfi import multPhiCorr_Data_B_80X
            setMetCorr(process,multPhiCorr_Data_B_80X)
        else:
            pass
        process.p *=process.flashggMetSequence
        for pathName in process.paths:
            path = getattr(process,pathName)
            for mod in modules:
                path.remove( getattr(process,mod))
            print getattr(process,pathName)
        process.out.outputCommands.append("drop *_*Gen*_*_*")
        process.out.outputCommands.append("keep *_reducedEgamma_*RecHit*_*") # for bad events
        delattr(process,"flashggPrunedGenParticles") # will be run due to unscheduled mode unless deleted
        self.customizeHighMassIsolations(process)
        process.load("flashgg/MicroAOD/flashggDiPhotonFilter_cfi")
        process.p1 = cms.Path(process.diPhotonFilter) # Do not save events with 0 diphotons
        process.out.SelectEvents = cms.untracked.PSet(SelectEvents=cms.vstring('p1'))

    def customizeDec2016Regression(self,process):
        if not (process.GlobalTag.globaltag == "80X_mcRun2_asymptotic_2016_TrancheIV_v7" or process.GlobalTag.globaltag == "80X_dataRun2_2016SeptRepro_v6"):
            raise Exception,"Regression application turned on but globalTag has unexpected value %s - see MicroAODCustomize.py" % process.GlobalTag.globaltag
        
        from EgammaAnalysis.ElectronTools.regressionWeights_cfi import regressionWeights
        process = regressionWeights(process)
        process.load('EgammaAnalysis.ElectronTools.regressionApplication_cff')
        process.p.insert(0,process.regressionApplication)
        process.electronMVAValueMapProducer.srcMiniAOD = cms.InputTag("slimmedElectrons")
        process.photonMVAValueMapProducer.srcMiniAOD = cms.InputTag("slimmedPhotons")
        process.photonIDValueMapProducer.srcMiniAOD = cms.InputTag("slimmedPhotons")

    def customizeSpring15EleID(self,process):
        from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat,switchOnVIDElectronIdProducer,setupAllVIDIdsInModule,setupVIDElectronSelection
        dataFormat = DataFormat.MiniAOD
        switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)
        my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_nonTrig_V1_cff',
                         'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff',
                         'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']
        for idmod in my_id_modules:
            setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
            
    def customizeSummer16EleID(self,process):
        from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat,switchOnVIDElectronIdProducer,setupAllVIDIdsInModule,setupVIDElectronSelection
        dataFormat = DataFormat.MiniAOD
        switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)
        my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff',
                         'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff',
                         'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']
        for idmod in my_id_modules:
            setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
        process.flashggElectrons.effAreasConfigFile = cms.FileInPath("RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt")
        process.flashggElectrons.eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto")  
        process.flashggElectrons.eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose")
        process.flashggElectrons.eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium")
        process.flashggElectrons.eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight")
        process.flashggElectrons.eleMVAMediumIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90")
        process.flashggElectrons.eleMVATightIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp80")
        process.flashggElectrons.mvaValuesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values")

    def customizeSummer16EGMPhoID(self,process):
        from PhysicsTools.SelectorUtils.tools.vid_id_tools import DataFormat,switchOnVIDPhotonIdProducer,setupAllVIDIdsInModule,setupVIDPhotonSelection
        dataFormat = DataFormat.MiniAOD
        switchOnVIDPhotonIdProducer(process, DataFormat.MiniAOD)
        my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Spring16_nonTrig_V1_cff']
        for idmod in my_id_modules:
            setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

    def customizeDataMuons(self,process):
        process.diPhotonFilter.src = "flashggSelectedMuons"
        process.diPhotonFilter.minNumber = 2

    def customizeHighMassIsolations(self,process):
        # for isolation cones
        process.vetoPhotons = cms.EDFilter("CandPtrSelector",
                                           src=cms.InputTag("flashggPhotons"),
                                           cut=cms.string("pt>10"),
                                           )
        process.vetoJets = cms.EDFilter("CandPtrSelector",
                                        src=cms.InputTag("slimmedJets"),
                                        cut=cms.string("pt>30"),
                                        )

        process.flashggPhotons.extraIsolations.extend([
                cms.PSet(
                    algo=cms.string("FlashggRandomConeIsolationAlgo"),
                    name=cms.string("rnd03"),
                    coneSize=cms.double(0.3), doOverlapRemoval=cms.bool(False),
                    charged=cms.vdouble(0.02,0.02,0.1),
                    photon=cms.vdouble(0.0, 0.070, 0.015, 0.0, 0.0, 0.0),
                    vetoCollections_=cms.VInputTag(cms.InputTag("vetoPhotons"),cms.InputTag("vetoJets")),
                    veto=cms.double(0.699),
                    ),
                ###          cms.PSet(
                ###              algo=cms.string("DiphotonsFootPrintRemovedIsolationAlgo"),
                ###              name=cms.string("fpr03"),
                ###              coneSize=cms.double(0.3), doRandomCone=cms.bool(False), removePhotonsInMap=cms.int32(1),
                ###              rechitLinkEnlargement=cms.double(0.25),
                ###              charged=cms.vdouble(0.02,0.02,0.1),
                ###              photon=cms.vdouble(0.0, 0.070, 0.015, 0.0, 0.0, 0.0),
                ###              ),
                cms.PSet(
                    algo=cms.string("DiphotonsFootPrintRemovedIsolationAlgo"),
                    name=cms.string("fprNoMap03"),
                    coneSize=cms.double(0.3), doRandomCone=cms.bool(False), removePhotonsInMap=cms.int32(0),
                    rechitLinkEnlargement=cms.double(0.25),
                    photon=cms.vdouble(0.0, 0.070, 0.015, 0.0, 0.0, 0.0),
                    charged=cms.vdouble(0.02,0.02,0.1),
                    ),
                #### cms.PSet(
                ####     algo=cms.string("DiphotonsFootPrintRemovedIsolationAlgo"),
                ####     name=cms.string("fprRnd03"),
                ####     coneSize=cms.double(0.3), doRandomCone=cms.bool(True), removePhotonsInMap=cms.int32(1),
                ####     rechitLinkEnlargement=cms.double(0.25),
                ####     charged=cms.vdouble(0.02,0.02,0.1),
                ####     vetoCollections_=cms.VInputTag(cms.InputTag("vetoPhotons"),cms.InputTag("vetoJets")),
                ####     veto=cms.double(0.699),
                ####     ),
                ]
          )


        for icone,dphi in enumerate( [0.7,1.3,1.9,2.5,3.1,-2.5,-1.9,-1.3,-0.7] ):
            process.flashggPhotons.extraIsolations.append(
                cms.PSet(
                    algo=cms.string("FlashggRandomConeIsolationAlgo"),
                    name=cms.string("rnd03_%d" % icone), deltaPhi=cms.double(dphi),
                    coneSize=cms.double(0.3), doOverlapRemoval=cms.bool(False),
                    charged=cms.vdouble(0.02,0.02,0.1),
                    photon=cms.vdouble(0.0, 0.070, 0.015, 0.0, 0.0, 0.0),
                    vetoCollections_=cms.VInputTag(cms.InputTag("vetoPhotons"),cms.InputTag("vetoJets")),
                    veto=cms.double(0.699),
                    ## maxVtx=cms.int32(1), computeWorstVtx=cms.bool(False)
                    ),
                )
        
    # Add debug collections    
    def customizeDebug(self,process):    
        from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODDebugOutputCommand
        process.out.outputCommands += microAODDebugOutputCommand # extra items for debugging

    # Add HLT collections    
    def customizeHLT(self,process):    
        from flashgg.MicroAOD.flashggMicroAODOutputCommands_cff import microAODHLTOutputCommand
        process.out.outputCommands += microAODHLTOutputCommand # extra items for HLT efficiency

    def customizeMuMuGamma(self,process):
        process.load("flashgg/MicroAOD/flashggDiMuons_cfi")
        process.load("flashgg/MicroAOD/flashggMuMuGamma_cfi")
        process.p *= process.flashggDiMuons*process.flashggMuMuGamma

    def customizeTTH(self,process):
        process.load("flashgg/MicroAOD/ttHGGFilter_cfi")
        process.p1 = cms.Path(process.ttHGGFilter)
#        if not hasattr(process,"options"):
#            process.options = cms.untracked.PSet()
#        process.options.wantSummary = cms.untracked.bool(True)
        process.out.SelectEvents = cms.untracked.PSet(SelectEvents=cms.vstring('p1'))
        process.rivetProducerHTXS.ProductionMode = "TTH"

    def customizeVBF(self,process):
        process.rivetProducerHTXS.ProductionMode = "VBF"

    def customizeVH(self,process):
        process.rivetProducerHTXS.ProductionMode = "VH"

    def customizeGGH(self,process):
        process.rivetProducerHTXS.ProductionMode = "GGF"

    def customizeTH(self,process):
        process.out.outputCommands.append("keep *_source_*_LHEFile")
        process.rivetProducerHTXS.ProductionMode = "TH"
        process.flashggPDFWeightObject.LHEEventTag = "source"
        process.flashggPDFWeightObject.LHERunLabel = "source"
        process.flashggPDFWeightObject.isStandardSample = False
        process.flashggPDFWeightObject.isThqSample = True
        

    def customizeGlobalTag(self,process):
        process.GlobalTag.globaltag = self.globalTag

    def customizeFileNames(self,process):
        process.source.fileNames = cms.untracked.vstring(self.fileNames)

    def customizeTiming(self,process):
        from Validation.Performance.TimeMemoryInfo import customise as TimeMemoryCustomize
        TimeMemoryCustomize(process)
        process.MessageLogger.cerr.threshold = 'WARNING'

    def customizePFCHS(self,process):    
        # need to allow unscheduled processes otherwise reclustering function will fail
        if not hasattr(process,"options"):
            process.options = cms.untracked.PSet()
        process.options.allowUnscheduled = cms.untracked.bool(True)
        from flashgg.MicroAOD.flashggJets_cfi import addFlashggPFCHSJets
        from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections
        for vtx in range(0,maxJetCollections):
            addFlashggPFCHSJets (process = process,
                                 isData=(self.processType == "data"),
                                 vertexIndex =vtx,
                                 #doQGTagging = True,
                                 label = '' + str(vtx))
            
    def customizePuppi(self,process):
        # need to allow unscheduled processes otherwise reclustering function will fail                                                            
        if not hasattr(process,"options"):
            process.options = cms.untracked.PSet()
        process.options.allowUnscheduled = cms.untracked.bool(True)
        from flashgg.MicroAOD.flashggJets_cfi import addFlashggPuppiJets
        from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections
        for vtx in range(0,maxJetCollections):
            addFlashggPuppiJets (process     = process,                                                                                           \
                                     vertexIndex = vtx,
                                 debug       = False,
                                 label = '' + str(vtx))

    def customizeRemovePFCHS(self,process):
        for pathName in process.paths:
            path = getattr(process,pathName)
            path.remove(process.flashggVertexMapForCHS)
            path.remove(process.flashggFinalJets)
        process.out.outputCommands.append('drop *_flashggFinalJets_*_*')

    def customizeRemovePuppi(self,process):
        for pathName in process.paths:
            path = getattr(process,pathName)
            path.remove(process.flashggVertexMapForPUPPI)
            path.remove(process.flashggFinalPuppiJets)
        process.out.outputCommands.append('drop *_flashggFinalPuppiJets_*_*')

    def customize50ns(self,process):
        process.flashggPhotons.photonIdMVAweightfile_EB = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_Spring15_50ns_barrel.xml")
        process.flashggPhotons.photonIdMVAweightfile_EE = cms.FileInPath("flashgg/MicroAOD/data/MVAweights_Spring15_50ns_endcap.xml")

    def customize76X(self,process):
        delattr(process,"QGPoolDBESSource")

    def customize80X(self,process):
        pass
#        delattr(process,"QGPoolDBESSource")

    def customize92X(self,process):
        print "customize92X"
        print process.p
        hack_modifier = cms.Modifier()
        hack_modifier._setChosen()
        hack_modifier.toReplaceWith(process.flashggPrePhotonSequence80X,process.egmPhotonIDSequence)
        process.p.remove(process.flashggFinalJets)
#        print process.GlobalTag.globaltag
#        process.egmPhotonIsolationMiniAODTask = cms.Task(process.egmPhotonIsolation)
#        process.egmPhotonIDTask = cms.Task(process.egmPhotonIDs, process.egmPhotonIsolationMiniAODTask, process.photonIDValueMapProducer, process.photonMVAValueMapProducer, process.photonRegressionValueMapProducer)
#        process.egmPhotonIdSequence = cms.Sequence(process.egmPhotonIDTask)
#        print "process.egmPhotonIDTask",process.egmPhotonIDTask
#        print "process.egmPhotonIDSequence",process.egmPhotonIDSequence
 #       photonIndex = process.p.index(process.photonMVAValueMapProducer)
#        photonIndex = 0
#        process.p.remove(process.photonMVAValueMapProducer)
#        hack_modifier = cms.Modifier()
#        hack_modifier._setChosen()
#        process.p.remove(process.egmPhotonIDs)
#        print "BEFORE REPLACE %i"%photonIndex,process.p
#        process.p.insert(0,process.egmPhotonIDSequence)
#        hack_modifier.toReplaceWith(process.egmPhotonIDs,process.egmPhotonIDSequence) # requires same class type
#        print "AFTER REPLACE %i"%photonIndex,process.p

# customization object
customize = MicroAODCustomize()
