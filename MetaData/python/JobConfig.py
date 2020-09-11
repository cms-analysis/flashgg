import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager
import FWCore.ParameterSet.Config as cms
from Utilities.General.cmssw_das_client import get_data as das_query

import commands

def safe_das_query( search, cmd ):
    output = das_query( search, cmd=cmd )
    if not 'data' in output:
        raise Exception('Your das query has not worked properly - check your proxy is valid')
    return output

class JobConfig(object):
    
    def __init__(self,*args,**kwargs):
        
        super(JobConfig,self).__init__()

        self.crossSections=kwargs.get("crossSections",["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"])
        self.tfileOut=kwargs.get("tfileOut",None)

        self.processIdMap = {}

        self.options = VarParsing.VarParsing("analysis")
        self.options.register ('metaDataSrc',
                               'flashgg', # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "metaDataSrc")
        self.options.register ('metaConditions',
                                "", # default value
                               VarParsing.VarParsing.multiplicity.list, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "metaConditions")        
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
                               None, # default value
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
                               True, # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.bool,          # string, int, or float
                               "useAAA")
        self.options.register ('useEOS',
                               False, # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.bool,          # string, int, or float
                               "useEOS")
        self.options.register ('copyInputMicroAOD',
                               False, # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.bool,          # string, int, or float
                               "copyInputMicroAOD")
        self.options.register ('useParentDataset',
                               False, # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.bool,          # string, int, or float
                               "useParentDataset")
        self.options.register ('recalculatePDFWeights',
                               False, # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.bool,          # string, int, or float
                               "recalculatePDFWeights")
        self.options.register ('secondaryDataset',
                               "", # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,         # string, int, or float
                               "secondaryDataset")
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
        self.options.register ('WeightName', # for THQ/THW samples the LHE weight should be mentioned
                               None, # default value
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "WeightName")
        self.options.register ('HHWWggYear', ##-- hack to choose pu_distrib as input rather than read from secondary dataset 
                               "", # default value 
                               VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                               VarParsing.VarParsing.varType.string,          # string, int, or float
                               "HHWWggYear")                                
        
        self.parsed = False        
        
        from SimGeneral.MixingModule.mix_2015_25ns_Startup_PoissonOOTPU_cfi import mix as mix_2015_25ns
        from SimGeneral.MixingModule.mix_2015_50ns_Startup_PoissonOOTPU_cfi import mix as mix_2015_50ns
        self.pu_distribs = { "74X_mcRun2_asymptotic_v2" : mix_2015_25ns.input.nbPileupEvents }
        try:
            from SimGeneral.MixingModule.mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi import mix as mix_2015_76_25ns
            self.pu_distribs["PU25nsData2015v1"] = mix_2015_76_25ns.input.nbPileupEvents
        except Exception:
            print "Failed to load 76X mixing, this is expected in 74X!"
            
        try:
            from SimGeneral.MixingModule.mix_2016_25ns_SpringMC_PUScenarioV1_PoissonOOTPU_cfi import mix as mix_2016_80_25ns
            self.pu_distribs["PUSpring16"] = mix_2016_80_25ns.input.nbPileupEvents
        except Exception:
            print "Failed to load 80X mixing, this is expected in 7X!"

        try:
            from SimGeneral.MixingModule.mix_2016_25ns_Moriond17MC_PoissonOOTPU_cfi import mix as mix_Moriond17
            self.pu_distribs["Summer16"] = mix_Moriond17.input.nbPileupEvents
            self.pu_distribs["PUMoriond17"] = mix_Moriond17.input.nbPileupEvents
            self.pu_distribs["upgrade2017"] = mix_Moriond17.input.nbPileupEvents
        except Exception:
            print "Failed to load Moriond17 mixing, this is expected in earlier releases"
            
        try:
            from flashgg.MetaData.mix_2017MCv2_DYJetsToLL import mix as mix_94X_mc2017
            #from flashgg.MetaData.mix_2017MCv2_GJet_Combined import mix as mix_94X_mc2017
            self.pu_distribs["94X_mc2017"] = mix_94X_mc2017.input.nbPileupEvents
        except Exception:
            print "Failed to load 94X_mc2017 mixing"

	try:
	    from SimGeneral.MixingModule.mix_2017_25ns_UltraLegacy_PoissonOOTPU_cfi import mix as mix_UL17
	    self.pu_distribs["Summer19UL17"] = mix_UL17.input.nbPileupEvents
	except Exception:
	    print "Failed to load UL17 mixing"


        try:
            from SimGeneral.MixingModule.mix_2018_25ns_JuneProjectionFull18_PoissonOOTPU_cfi import mix as mix_Autumn18
            self.pu_distribs["Autumn18"] = mix_Autumn18.input.nbPileupEvents
        except Exception:
            print "Failed to load Autumn18 mixing"
            
        #self.pu_distribs_hack_2017 = {  }

        
        # try:
        #     import importlib
        #     from os import listdir,environ
        #     mixdir = "PU_MixFiles_2017_miniaodv2_310"
        #     thedir = "%s/src/flashgg/MetaData/python/%s" % (environ['CMSSW_BASE'],mixdir)
        #     print thedir
        #     for fn in listdir(thedir):
        #         print fn
        #         if fn.startswith("mix_2017MC_") and fn.endswith(".py"):
        #             mn = fn[:-3]
        #             print fn,mn
        #             m = importlib.import_module("flashgg.MetaData.%s.%s" % (mixdir,mn))
        #             kn = mn.replace("mix_2017MC_","")
        #             self.pu_distribs_hack_2017[kn] = m.mix.input.nbPileupEvents
        # except Exception,e:
        #     print "failed to load hacky 94X mixing by dataset"
        #     raise e
            
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

        # keep useParent and secondaryDataset as exclusive options for the moment
        if self.options.useParentDataset and self.options.secondaryDataset != "":
            raise Exception("useParentDataset cannot be set together with a secondaryDataset")

        isFwlite = False
        hasOutput = False
        hasTFile = False
        sp_unused = ""
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
                name,xsec,totEvents,files,maxEvents,sp_unused = self.dataset
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
            dsetname,xsec,totEvents,files,maxEvents,sp_unused = self.dataset
            if type(xsec) == float or xsec == None:
                print 
                print "Error: cross section not found for dataset %s" % dsetname
                print
                
            putarget = None
            samplepu = None
            if self.puTarget != "":
                putarget = map(float, self.puTarget.split(","))
                
            processId = self.getProcessId(dsetname)
            self.processId = processId

            #----------

            if self.options.processIndex != None:
                self.processIndex = self.options.processIndex
            else:
                # not specified on the command line, try to take it 
                # from the cross section file, otherwise use smallest int32 as default value
                # in order not to confuse it with data (index 0)

                if isinstance(xsec, dict):
                    self.processIndex = xsec.get('itype', -0x7FFFFFFF)
                else:
                    # note that in some cases (process not defined in cross_sections.json ?)
                    # this can still be a float
                    self.processIndex = -0x7FFFFFFF

            #----------

            if isinstance(xsec, dict) and "itype" in xsec:
                for name,obj in process.__dict__.iteritems():
                    if hasattr(obj, "sampleIndex"):
                        obj.sampleIndex = xsec["itype"]

            
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
#                                print dsetname
#                                print self.pu_distribs.keys()
#                                hack2017 = True
#                                found_hack2017 = False
#                                if hack2017:
                                # if self.options.PUyear=="2017":
                                #     print dsetname.split("/")[1]
                                #    # print self.pu_distribs.keys()
                                #     print self.pu_distribs_hack_2017.keys()
                                #    # matches = filter(lambda x: x == dsetname.split("/")[1],self.pu_distribs.keys())
                                #     matches = filter(lambda x: x == dsetname.split("/")[1],self.pu_distribs_hack_2017.keys())
                                #     if len(matches) == 1:
                                #         found_hack2017 = True
                                #         print "FOUND HACK2017 PILEUP DISTRIBUTION WITH KEY:",matches[0]
                                # if not found_hack2017:
                                
                                ##-- Run 2 PPD mixing used:
                                # 2016: "dbs:/Neutrino_E-10_gun/RunIISpring15PrePremix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v2-v2/GEN-SIM-DIGI-RAW"
                                # 2017: "dbs:/Neutrino_E-10_gun/RunIISummer17PrePremix-MCv2_correctPU_94X_mc2017_realistic_v9-v1/GEN-SIM-DIGI-RAW"
                                # 2018: "dbs:/Neutrino_E-10_gun/RunIISummer17PrePremix-PUAutumn18_102X_upgrade2018_realistic_v15-v1/GEN-SIM-DIGI-RAW"
                                if(self.HHWWggYear == "2016"): matches = ["PUMoriond17"]
                                elif(self.HHWWggYear == "2017"): matches = ["94X_mc2017"]
                                elif(self.HHWWggYear == "2018"): matches = ["Autumn18"] 
                                else: matches = filter(lambda x: x in dsetname, self.pu_distribs.keys() )
                                
                                print'PU matches below'
                                print matches
                                if len(matches) > 1:
                                    print "Multiple matches, check if they're all the same"
                                    allsame = True
                                    for i in range(1,len(matches)):
                                        if self.pu_distribs[matches[0]] != self.pu_distribs[matches[i]]:
                                            allsame = False
                                    if allsame:
                                        print "They're all the same so we just take the 0th one:",matches[0]
                                        matches = [matches[0]]
                                    else:
                                        print "Not all the same... so we return to the old behavior and take an exact match, otherwise leave empty..."
                                        matches = filter(lambda x: x == dsetname, matches)
                                if len(matches) != 1:
                                    raise Exception("Could not determine sample pu distribution for reweighting. Possible matches are [%s]. Selected [%s]\n dataset: %s" % 
                                                        ( ",".join(self.pu_distribs.keys()), ",".join(matches), dsetname ) )
                                # if self.options.PUyear=="2017": samplepu = self.pu_distribs_hack_2017[matches[0]]
                                # else :
                            samplepu = self.pu_distribs[matches[0]]
                            puObj.puReWeight = True
                            puObj.puBins = cms.vdouble( map(float, samplepu.probFunctionVariable) )
                            puObj.mcPu   = samplepu.probValue
                            puObj.dataPu = cms.vdouble(putarget)
                            puObj.useTruePu = cms.bool(True)
                    
            for name,obj in process.__dict__.iteritems():
                if hasattr(obj,"processId"):
                    obj.processId = str(processId)

            for name,obj in process.__dict__.iteritems():
                if hasattr(obj,"processIndex"):
                    obj.processIndex = int(self.processIndex)
                    
            lumisToSkip = None
            if isdata:
                lumisToSkip = self.samplesMan.getLumisToSkip(dsetname)
                process.source.lumisToSkip = lumisToSkip.getVLuminosityBlockRange()

            if isdata and self.lumiMask != "":
                if isFwlite:
                    sys.exit("Lumi mask not supported in FWlite",-1)

                import FWCore.PythonUtilities.LumiList as LumiList
                target = LumiList.LumiList(filename = self.lumiMask)
                if lumisToSkip: 
                    target = target.__sub__(lumisToSkip)                    
                process.source.lumisToProcess = target.getVLuminosityBlockRange()
                print process.source.lumisToProcess

        flist = []
        sflist = []

        # get the runs and lumis contained in each file of the secondary dataset
        if self.options.secondaryDataset:
            secondary_files = [fdata['file'][0]['name'] for fdata in safe_das_query("file dataset=%s instance=prod/phys03" % self.options.secondaryDataset, 
                                                                               cmd='dasgoclient --dasmaps=./')['data']]
            runs_and_lumis = {}
            for s in secondary_files:
                runs_and_lumis[str(s)] = {data['lumi'][0]['run_number'] : data['lumi'][0]['lumi_section_num']
                                          for data in safe_das_query("lumi file=%s instance=prod/phys03" % s, cmd='dasgoclient --dasmaps=./')['data']}

        for f in files:
            if len(f.split(":",1))>1:
                flist.append(str(f))
            else:
                flist.append(str("%s%s" % (self.filePrepend,f)))
            # keep useParent and secondaryDataset as exclusive options for the moment
            if self.options.useParentDataset:
                parent_files = safe_das_query("parent file=%s instance=prod/phys03" % f, cmd='dasgoclient --dasmaps=./')['data']
                for parent_f in parent_files:
                    parent_f_name = str(parent_f['parent'][0]['name'])
                    sflist.append('root://cms-xrd-global.cern.ch/'+parent_f_name if 'root://' not in parent_f_name else parent_f_name)
            elif self.options.secondaryDataset != "":
                # match primary file to the corresponding secondary file(s)
                f_runs_and_lumis = {data['lumi'][0]['run_number'] : data['lumi'][0]['lumi_section_num']
                                    for data in safe_das_query("lumi file=%s instance=prod/phys03" % f, cmd='dasgoclient --dasmaps=./')['data']}
                for s_name, s_runs_and_lumis in runs_and_lumis.items():
                    matched_runs = set(f_runs_and_lumis.keys()).intersection(s_runs_and_lumis.keys())
                    for run in matched_runs:
                        if any(lumi in f_runs_and_lumis[run] for lumi in s_runs_and_lumis[run]):
                            sflist.append(s_name)

        ## mitigate server glitches by copying the input files (microAOD) on the worker node
        if self.copyInputMicroAOD and not self.dryRun:
            commands.getstatusoutput('mkdir -p input_files/')
            for i,f in enumerate(flist):
                status, out = commands.getstatusoutput('xrdcp %s ./input_files/'%f)
                print(out)
                flocal = 'file:./input_files/'+f.split('/')[-1]
                flist[i] = flocal

        if len(flist) > 0:
            ## fwlite
            if isFwlite:
                ## process.fwliteInput.fileNames.extend([ str("%s%s" % (self.filePrepend,f)) for f in  files])
                process.fwliteInput.fileNames = flist
            ## full framework
            else:
                ## process.source.fileNames.extend([ str("%s%s" % (self.filePrepend,f)) for f in  files])
                process.source.fileNames = flist
                if len(sflist) > 0:
                    process.source.secondaryFileNames = cms.untracked.vstring(sflist)
 
        ## fwlite
        if isFwlite:
            process.fwliteInput.maxEvents = self.options.maxEvents
            process.fwliteOutput.fileName = self.outputFile
        ## full framework
        else:
            process.maxEvents.input = self.options.maxEvents
            
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
       #     self.filePrepend = "root://xrootd-cms.infn.it/"
            self.filePrepend = "root://cms-xrd-global.cern.ch/"
        elif self.useEOS:
            print'useEOS: True'
            self.filePrepend = "root://eoscms.cern.ch//eos/cms"
        self.samplesMan = None
        dataset = None
        if self.dataset != "":
            print "Reading dataset (%s) %s" % ( self.campaign, self.dataset)
            self.samplesMan = SamplesManager("$CMSSW_BASE/src/%s/MetaData/data/%s/datasets*.json" % (self.metaDataSrc, self.campaign),
                                         self.crossSections,
                                         )
            if self.dryRun and self.getMaxJobs:
                dataset = self.samplesMan.getDatasetMetaData(self.options.maxEvents,self.dataset,jobId=-1,nJobs=self.nJobs,weightName=self.WeightName)
            else:
                dataset = self.samplesMan.getDatasetMetaData(self.options.maxEvents,self.dataset,jobId=self.jobId,nJobs=self.nJobs,weightName=self.WeightName)
            if not dataset: 
                print "Could not find dataset %s in campaign %s/%s" % (self.dataset,self.metaDataSrc,self.campaing)
                sys.exit(-1)
                
        self.dataset = dataset
        # auto-detect data from xsec = 0
        if self.dataset:
            name,xsec,totEvents,files,maxEvents,specialPrepend = self.dataset
            if len(specialPrepend) > 0 and not self.useAAA:
                self.filePrepend = specialPrepend
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
