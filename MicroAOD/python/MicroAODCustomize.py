
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
                              0, # default value
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
    
    # empty default definition for userCutomize
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
        if len(self.globalTag) >0:
            self.customizeGlobalTag(process)
        if len(self.fileNames) >0:
            self.customizeFileNames(process)
        if self.timing == 1:
            self.customizeTiming(process)
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
        process.options = cms.untracked.PSet(
            allowUnscheduled = cms.untracked.bool(True)
            )
        from flashgg.MicroAOD.flashggJets_cfi import addFlashggPFCHSJets
        from flashgg.MicroAOD.flashggJets_cfi import maxJetCollections
        for vtx in range(0,maxJetCollections):
            addFlashggPFCHSJets (process = process,
                                 vertexIndex =vtx,
                                 doQGTagging = True,
                                 label = '' + str(vtx))
            
    def customizePuppi(self,process):
        # need to allow unscheduled processes otherwise reclustering function will fail                                                            
        process.options = cms.untracked.PSet(
            allowUnscheduled = cms.untracked.bool(True)
            )
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


# customization object
customize = MicroAODCustomize()
