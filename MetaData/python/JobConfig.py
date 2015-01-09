import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

class JobConfig(object):
    
    def __init__(self,*args,**kwargs):
        
        super(JobConfig,self).__init__()

        self.metaDataSrc=kwargs.get("metaDataSrc","flashgg")
        self.crossSections=kwargs.get("crossSections",["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])

        self.options = VarParsing.VarParsing("analysis")
        ## self.options.setDefault ('maxEvents',100)
        self.options.register ('dataset',
                       "", # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.string,          # string, int, or float
                       "dataset")
        self.options.register ('campaign',
                       "", # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.string,          # string, int, or float
                       "campaign")
        self.options.register ('useAAA',
                       False, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.bool,          # string, int, or float
                       "useAAA")
        self.options.register ('useEOS',
                       True, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.bool,          # string, int, or float
                       "useEOS")
        self.options.register ('targetLumi',
                       1.e+3, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.float,          # string, int, or float
                       "targetLumi")
        self.options.register ('nJobs',
                       0, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.int,          # string, int, or float
                       "nJobs")
        self.options.register ('jobId',
                               -1, # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.int,          # string, int, or float
                               "jobId")
        self.options.register ('dryRun',
                       False, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.bool,          # string, int, or float
                       "dryRun")
        
        self.parsed = False

    def __getattr__(self,name):
        ## did not manage to inherit from VarParsing, because of some issues in __init__
        ## this allows to use VarParsing methods on JobConfig
        if hasattr(self.options,name):
            return getattr(self.options,name)
        
        raise AttributeError
                                             
    def __call__(self,process):
        self.customize(process)
 
    # process customization
    def customize(self,process):
        self.parse()

        isFwlite = False
        hasOutput = False
        hasTFile = False
        if hasattr(process,"fwliteInput"):
            isFwlite = True
        if not isFwlite:
            hasOutput = hasattr(process,"out")            
            hasTFile = hasattr(process,"TFileService")
        
        if hasOutput and hasTFile:
            tfile = self.outputFile.replace(".root","_histos.root")
        else:
            tfile = self.outputFile
            
        if self.dryRun:
            import sys
            if self.dataset:
                name,xsec,totEvents,files = self.dataset
                if len(files) != 0:
                    if isFwlite:
                        print "hadd:%s" % self.outputFile
                    else:
                        if hasOutput:
                            print "edm:%s" % self.outputFile
                        if hasTFile:
                            print "hadd:%s" % tfile
                    sys.exit(0)
            sys.exit(1)
            


        if self.dataset:
            name,xsec,totEvents,files = self.dataset
            
            ## look for analyzers which have lumiWeight as attribute
            for name,obj in process.__dict__.iteritems():
                if hasattr(obj,"lumiWeight"):
                    obj.lumiWeight = xsec["xs"]/float(totEvents)*self.targetLumi
            
            ## fwlite
            if isFwlite:
                process.fwliteInput.fileNames.extend([ str("%s%s" % (self.filePrepend,f)) for f in  files])
            ## full framework
            else:
                process.source.fileNames.extend([ str("%s%s" % (self.filePrepend,f)) for f in  files])
 
        ## fwlite
        if isFwlite:
            process.fwliteInput.maxEvents = self.options.maxEvents
            process.fwliteOutput.fileName = self.outputFile
        ## full framework
        else:
            process.maxEvents.input = self.maxEvents
            
            if hasOutput:
                process.out.fileName = self.outputFile

            if hasTFile:
                process.TFileService.fileName = tfile
        
            
    # parse command line and do post-processing
    def parse(self):

        if self.parsed:
            return

        self.options.parseArguments()
        if self.useAAA:
            self.filePrepend = "root://xrootd-cms.infn.it/"
        elif self.useEOS:
            self.filePrepend = "root://eoscms//eos/cms"
            
        dataset = None
        if self.dataset != "":
            print "Reading dataset (%s) %s" % ( self.campaign, self.dataset)
            dataset = SamplesManager("$CMSSW_BASE/src/%s/MetaData/data/%s/datasets.json" % (self.metaDataSrc, self.campaign),
                                    self.crossSections,
                                     ).getDatasetMetaData(self.maxEvents,self.dataset,jobId=self.jobId,nJobs=self.nJobs)
        self.dataset = dataset

        outputFile=self.outputFile
        if self.jobId != -1:
            outputFile = "%s_%d.root" % ( outputFile.replace(".root",""), self.jobId )
        self.setType ("outputFile", VarParsing.VarParsing.varType.string)
        self.outputFile = outputFile


# customization object
customize = JobConfig()
