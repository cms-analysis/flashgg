import FWCore.ParameterSet.VarParsing as VarParsing

class MicroAODCustomize(object):
    
    def __init__(self,*args,**kwargs):
        
        super(MicroAODCustomize,self).__init__()
    
        self.options = VarParsing.VarParsing()
        
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

    # process customization
    def customize(self,process):
        self.options.parseArguments()
        
        if self.processType == "data":
            self.customizeData(process)
        elif self.processType == "signal":
            self.customizeSignal(process)
        if self.processType == "background":
            self.customizeBackground(process)

    # signal specific customization
    def customizeSignal(self,process):
        process.flashggGenPhotonsExtra.defaultType = 1

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
            
# customization object
customize = MicroAODCustomize()
