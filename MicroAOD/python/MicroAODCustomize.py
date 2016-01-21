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
        elif self.processType == "signal":
            self.customizeSignal(process)
        if self.processType == "background":
            self.customizeBackground(process)
        if self.debug == 1:
            self.customizeDebug(process)
        if self.hlt == 1:
            self.customizeHLT(process)
        if self.muMuGamma == 1:
            self.customizeMuMuGamma(process)
        elif self.muMuGamma == 2 and ("DY" in customize.datasetName or "DoubleMuon" in customize.datasetName):
            self.customizeMuMuGamma(process)
        if "ttH" in customize.datasetName:
            self.customizeTTH(process)
        if len(self.globalTag) >0:
            self.customizeGlobalTag(process)
        if len(self.fileNames) >0:
            self.customizeFileNames(process)
        if self.timing == 1:
            self.customizeTiming(process)
        if os.environ["CMSSW_VERSION"].count("CMSSW_7_6"):
            self.customize76X(process)
        elif len(self.globalTag) == 0:
            self.globalTag = "74X_mcRun2_asymptotic_v4"
            self.customizeGlobalTag(process)
        if self.bunchSpacing == 25:
            pass #default
        elif self.bunchSpacing == 50:
            self.customize50ns(process)
        else:
            raise Exception,"Only bunchSpacing=25 and bunchSpacing=50 are supported"
            
    # signal specific customization
    def customizeSignal(self,process):
        process.flashggGenPhotonsExtra.defaultType = 1

        # Default should be the right name for all signals
        process.load("flashgg/MicroAOD/flashggPDFWeightObject_cfi")
        process.p *= process.flashggPDFWeightObject

    # background specific customization
    def customizeBackground(self,process):
        if "sherpa" in self.datasetName:
            process.flashggGenPhotonsExtra.defaultType = 1

            
    # data specific customization
    def customizeData(self,process):
        ## remove MC-specific modules
        modules = process.flashggMicroAODGenSequence.moduleNames()
        for pathName in process.paths:
            path = getattr(process,pathName)
            for mod in modules:
                path.remove( getattr(process,mod))
            print getattr(process,pathName)
        process.out.outputCommands.append("drop *_*Gen*_*_*")
        process.out.outputCommands.append("keep *_*_*RecHit*_*") # for bad events
        delattr(process,"flashggPrunedGenParticles") # will be run due to unscheduled mode unless deleted
        self.customizeHighMassIsolations(process)

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

# customization object
customize = MicroAODCustomize()
