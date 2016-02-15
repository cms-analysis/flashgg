import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager
import FWCore.ParameterSet.Config as cms


class JobConfig(object):
    
    def __init__(self,*args,**kwargs):
        
        super(JobConfig,self).__init__()

        self.metaDataSrc=kwargs.get("metaDataSrc","flashgg")
        self.crossSections=kwargs.get("crossSections",["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
        self.tfileOut=kwargs.get("tfileOut",None)

        self.processIdMap = {}

        self.options = VarParsing.VarParsing("analysis")
        ## self.options.setDefault ('maxEvents',100)
        self.options.register ('dataset',
                       "", # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.string,          # string, int, or float
                       "dataset")
        self.options.register ('processId',
                       "", # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.string,          # string, int, or float
                       "processId")
        self.options.register ('processIdMap',
                       "", # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.string,          # string, int, or float
                       "processIdMap")
        self.options.register ('processIndex',
                       0, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.int,          # string, int, or float
                       "processIndex")
        self.options.register ('secondaryDatasetInProcId',
                       False, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.bool,          # string, int, or float
                       "secondaryDatasetInProcId")
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
        self.options.register ('lastAttempt',
                       False, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.bool,          # string, int, or float
                       "lastAttempt")
        self.options.register ('lumiMask',
                       "", # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.string,          # string, int, or float
                       "lumiMask")
        self.options.register ('dryRun',
                       False, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.bool,          # string, int, or float
                       "dryRun")
        self.options.register ('dumpPython',
                       "", # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.string,          # string, int, or float
                       "dumpPython")
        self.options.register ('getMaxJobs',
                       False, # default value
                       VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                       VarParsing.VarParsing.varType.bool,          # string, int, or float
                       "getMaxJobs")
        self.options.register ('processType',
                               "", # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "processType")
        self.options.register ('puTarget',
                               "", # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "puTarget")

        
        self.parsed = False
        
        
        from SimGeneral.MixingModule.mix_2015_25ns_Startup_PoissonOOTPU_cfi import mix as mix_2015_25ns
        from SimGeneral.MixingModule.mix_2015_50ns_Startup_PoissonOOTPU_cfi import mix as mix_2015_50ns
        from SimGeneral.MixingModule.mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi import mix as mix_2015_76_25ns
        self.pu_distribs = { "74X_mcRun2_asymptotic_v2" : mix_2015_25ns.input.nbPileupEvents,
                             "PU25nsData2015v1" : mix_2015_76_25ns.input.nbPileupEvents,
                             }
        

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
            if self.dataset and self.dataset != "":
                name,xsec,totEvents,files,maxEvents = self.dataset
                if self.getMaxJobs:
                    print "maxJobs:%d" % ( min(len(files),self.nJobs) )                    
                if len(files) != 0:
                    if isFwlite:
                        print "hadd:%s" % self.outputFile
                    else:
                        if hasOutput:
                            print "edm:%s" % self.outputFile
                        if hasTFile or self.tfileOut:
                            print "hadd:%s" % tfile
                    ## sys.exit(0)
            else:
                sys.exit(1)
            

        files = self.inputFiles
        if self.dataset and self.dataset != "":
            dsetname,xsec,totEvents,files,maxEvents = self.dataset
            self.maxEvents = int(maxEvents)
            
            putarget = None
            samplepu = None
            if self.puTarget != "":
                putarget = map(float, self.puTarget.split(","))
                
            processId = self.getProcessId(dsetname)

            self.processIndex = self.options.processIndex
            self.processId = processId
            
            isdata = self.processType == "data"
            if isdata or self.targetLumi > 0. or putarget:
                ## look for analyzers which have lumiWeight as attribute
                for name,obj in process.__dict__.iteritems():
                    
                    if hasattr(obj,"lumiWeight"):
                        if  isdata:
                            obj.lumiWeight = 1.
                        else:
                            wei = xsec["xs"]/float(totEvents)*self.targetLumi
                            wei *= xsec.get("br",1.)
                            wei *= xsec.get("kf",1.)
                            obj.lumiWeight = wei

                    if hasattr(obj,"intLumi"):
                        if isdata:
                            obj.intLumi= 0 # should not be used in final fits.
                            # setting to 0 will cause error if someone tries to use
                            #it for normalization downsteram
                        else:
                            obj.intLumi=self.targetLumi

                    if putarget and not isdata:
                        puObj = None
                        if hasattr(obj,"puReWeight"):
                            puObj = obj
                        elif hasattr(obj,"globalVariables") and hasattr(obj.globalVariables,"puReWeight"):
                            puObj = obj.globalVariables
                        if puObj:
                            if not samplepu:
                                matches = filter(lambda x: x in dsetname, self.pu_distribs.keys() )
                                print matches
                                if len(matches) > 1:
                                    matches = filter(lambda x: x == dsetname, matches)
                                if len(matches) != 1:
                                    raise Exception("Could not determine sample pu distribution for reweighting. Possible matches are [%s]. Selected [%s]\n dataset: %s" % 
                                                ( ",".join(self.pu_distribs.keys()), ",".join(matches), dsetname ) )
                                samplepu = self.pu_distribs[matches[0]]
                            puObj.puReWeight = True
                            puObj.puBins = cms.vdouble( map(float, samplepu.probFunctionVariable) )
                            puObj.mcPu   = samplepu.probValue
                            ## puObj.mcPu   = [0.00023481895458601418, 0.0007044568637580425, 0.0020664068003569246, 0.005353872164561123, 0.01009721504719861, 0.016155544075517777, 0.02385760578593904, 0.03289813553750059, 0.04172732822993472, 0.0496876907904006, 0.05544075517775795, 0.06079462734231907, 0.06027802564222984, 0.06499788662940872, 0.06401164702014747, 0.06443432113840229, 0.06196872211524914, 0.05692011459164984, 0.0529986380500634, 0.044991311698680314, 0.04283097731648899, 0.03613863711078758, 0.03116047527356408, 0.024327243695111068, 0.02115718780819988, 0.017940168130371484, 0.01425351054337106, 0.011153900342835674, 0.008641337528765322, 0.005870473864650355, 0.004696379091720284, 0.0033344291551214013, 0.0027238998731977646, 0.0021133705912741276, 0.0010332034001784623, 0.0011036490865542667, 0.0003757103273376227, 0.00035222843187902126, 0.00023481895458601418, 0.0003991922227962241, 0.00016437326821020992, 0.0001408913727516085, 4.6963790917202836e-05, 7.044568637580425e-05, 4.6963790917202836e-05, 2.3481895458601418e-05, 4.6963790917202836e-05,4.6963790917202836e-05,4.6963790917202836e-05,4.6963790917202836e-05,4.6963790917202836e-05,4.6963790917202836e-05]
                            ## puObj.mcPu   = [2.0983716635890548e-05, 0.0001259022998153433, 0.0009022998153432936, 0.002245257680040289, 0.00574953835823401, 0.009904314252140339, 0.016703038442168878, 0.02400537183145879, 0.031979184153097195, 0.04114906832298137, 0.047737955346651, 0.056068490851099544, 0.05936293436293436, 0.06339180795702534, 0.06712690951821386, 0.06563706563706563, 0.06263639415813328, 0.06123048514352862, 0.05585865368474064, 0.052522242739634045, 0.04746516703038442, 0.04247104247104247, 0.035462481114655026, 0.03017458452241061, 0.025474231995971125, 0.021277488668793018, 0.018108947456773543, 0.012821050864529126, 0.011226288400201444, 0.008519388954171562, 0.00612724525768004, 0.004238710760449891, 0.004196743327178109, 0.002371159979855632, 0.0017206647641430251, 0.0012170555648816517, 0.0006714789323484975, 0.0006085277824408259, 0.00039869061608192044, 0.00031475574953835825, 0.0002518045996306866, 6.295114990767164e-05, 0.00014688601645123384, 6.295114990767164e-05, 6.295114990767164e-05, 4.1967433271781096e-05, 2.0983716635890548e-05, 4.1967433271781096e-05, 4.1967433271781096e-05, 2.0983716635890548e-05, 2.0983716635890548e-05, 2.0983716635890548e-05]
                            puObj.dataPu = cms.vdouble(putarget)
                            puObj.useTruePu = cms.bool(True)
                        
                    
            for name,obj in process.__dict__.iteritems():
                if hasattr(obj,"processId"):
                    obj.processId = str(processId)

            for name,obj in process.__dict__.iteritems():
                if hasattr(obj,"processIndex"):
                    obj.processIndex = int(self.processIndex)
            
            if isdata and self.lumiMask != "":
                if isFwlite:
                    sys.exit("Lumi mask not supported in FWlite",-1)

                import FWCore.PythonUtilities.LumiList as LumiList
                process.source.lumisToProcess = LumiList.LumiList(filename = self.lumiMask).getVLuminosityBlockRange()
                
            
        flist = []
        for f in files:
            if len(f.split(":",1))>1:
                flist.append(str(f))
            else:
                flist.append(str("%s%s" % (self.filePrepend,f)))
        if len(flist) > 0:
            ## fwlite
            if isFwlite:
                ## process.fwliteInput.fileNames.extend([ str("%s%s" % (self.filePrepend,f)) for f in  files])
                process.fwliteInput.fileNames = flist
            ## full framework
            else:
                ## process.source.fileNames.extend([ str("%s%s" % (self.filePrepend,f)) for f in  files])
                process.source.fileNames = flist
 
        ## fwlite
        if isFwlite:
            process.fwliteInput.maxEvents = self.maxEvents
            process.fwliteOutput.fileName = self.outputFile
        ## full framework
        else:
            process.maxEvents.input = self.maxEvents
            
            if hasOutput:
                process.out.fileName = self.outputFile

            if hasTFile:
                process.TFileService.fileName = tfile
    
        if self.tfileOut:
            if hasTFile:
                print "Could not run with both TFileService and custom tfileOut"
                sys.exit(-1)
            name,attr = self.tfileOut
            setattr( getattr( process, name ), attr, tfile )
            

        if self.dumpPython != "":
            from gzip import open
            pyout = open("%s.gz" % self.dumpPython,"w+")
            pyout.write( process.dumpPython() )
            pyout.close()

    # parse command line and do post-processing
    def parse(self):

        if self.parsed:
            return

        self.options.parseArguments()
#        print "parsing arguments, processIdnex is "+str(self.options.processIndex)
        self.processIndex = self.options.processIndex
        if self.options.processIdMap != "":
            self.readProcessIdMap(self.options.processIdMap)
        
        if self.useAAA:
            self.filePrepend = "root://xrootd-cms.infn.it/"
        elif self.useEOS:
            self.filePrepend = "root://eoscms//eos/cms"
            
        dataset = None
        if self.dataset != "":
            print "Reading dataset (%s) %s" % ( self.campaign, self.dataset)
            if self.dryRun and self.getMaxJobs:
                dataset = SamplesManager("$CMSSW_BASE/src/%s/MetaData/data/%s/datasets.json" % (self.metaDataSrc, self.campaign),
                                         self.crossSections,
                                         ).getDatasetMetaData(self.maxEvents,self.dataset,jobId=-1,nJobs=self.nJobs)
            else:
                dataset = SamplesManager("$CMSSW_BASE/src/%s/MetaData/data/%s/datasets.json" % (self.metaDataSrc, self.campaign),
                                         self.crossSections,
                                         ).getDatasetMetaData(self.maxEvents,self.dataset,jobId=self.jobId,nJobs=self.nJobs)
            if not dataset:
                print "Could not find dataset %s in campaing %s/%s" % (self.dataset,self.metaDataSrc,self.campaing)
                sys.exit(-1)
                
        self.dataset = dataset
        # auto-detect data from xsec = 0
        if self.dataset:
            name,xsec,totEvents,files,maxEvents = self.dataset            
            if type(xsec) != dict or type(xsec.get("xs",None)) != float:
                print "Warning: you are running on a dataset for which you specified no cross section: \n %s " % name
            else:
                if self.processType == "" and xsec["xs"] == 0.:
                    self.processType = "data"
                    
            self.processId = self.getProcessId(name)
            
        outputFile=self.outputFile
        if self.jobId != -1:
            outputFile = "%s_%d.root" % ( outputFile.replace(".root",""), self.jobId )
        self.setType ("outputFile", VarParsing.VarParsing.varType.string)
        self.outputFile = outputFile

        self.parsed=True

    def datasetName(self):
        if type(self.dataset) == tuple:
            return self.dataset[0]
        return self.dataset
    
    def getProcessId(self,name):
        return self.getProcessId_(name).replace("/","").replace("-","_")
    
    def getProcessId_(self,name):
        if self.processId != "":
            return self.processId
        
        ## print name, self.processIdMap
        if name in self.processIdMap:
            return self.processIdMap[name]

        primSet,secSet = name.rsplit("/")[1:3]
        if primSet in self.processIdMap:
            return self.processIdMap[primSet]

        primSet = "/"+primSet
        if primSet in self.processIdMap:
            return self.processIdMap[primSet]
        
        if self.secondaryDatasetInProcId:
            return primSet + "_" + secSet
        return primSet

    def readProcessIdMap(self,fname):
        
        with open(fname) as fin:
            import json

            cfg = json.loads(fin.read())
            
            processes = cfg["processes"]
            for key,val in processes.iteritems():
                for dst in val:
                    if type(dst) == list:
                        name = dst[0]
                    else:
                        name = dst
                    self.processIdMap[name] = key
            
            fin.close()

        
# customization object
customize = JobConfig()
