from optpars_utils import *

from das_client import get_data as das_query

from pprint import pprint

import os,json,fcntl,sys
from parallel  import *
from threading import Semaphore
from fnmatch import fnmatch
import commands

import re

# -------------------------------------------------------------------------------
import pwd
def x509():
    "Helper function to get x509 either from env or tmp file"
    x509 = os.environ.get('X509_USER_PROXY', '')
    if  not x509:
        x509 = '/tmp/x509up_u%s' % pwd.getpwuid( os.getuid() ).pw_uid
        if  not os.path.isfile(x509):
            return ''
    return x509

# -------------------------------------------------------------------------------
def shell_expand(string):
    if string:
        return os.path.expanduser( os.path.expandvars(string) )
    return None


# -------------------------------------------------------------------------------
def ask_user(question,responses=["y","n"]):
    reply = None
    while not reply in responses:
        print "%s [%s]" % ( question, "".join(responses) ), 
        reply = raw_input()
    return reply

# -------------------------------------------------------------------------------
class SamplesManager(object):
    
    def __init__(self,
                 catalog,
                 cross_sections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"],
                 dbs_instance="prod/phys03",
                 queue=None, maxThreads=200,force=False,doContinue=False
                 ):
        """
        Constructur:
        @catalog: json file used to read/write dataset information
        @cross_sections: json file where samples cross sections are stored
        @dbs_instance: DBS instance tp use
        """
        self.cross_sections_ = {}
        self.dbs_instance_ = dbs_instance

        for xsecFile in cross_sections:
            fname = shell_expand(xsecFile)
            self.cross_sections_.update( json.loads( open(fname).read() ) )
            
        self.catalog_ = shell_expand(catalog)

        self.parallel_ = None
        self.sem_ = Semaphore()

        print "Will use the following datasets catalog:"
        print self.catalog_
        
        self.queue_ = queue
        self.maxThreads_ = maxThreads
        self.force_ = force
        self.continue_ = doContinue
        self.just_open_ = False

    def importFromCatalog(self,src,pattern):
        print "importing datasets from catalog %s" % src
        
        mine = self.readCatalog()
        
        other = self.readCatalog(path=src)
        
        doAll = False
        for key,val in other.iteritems():
            if (key == pattern or fnmatch(key,pattern)):
                take = True
                if not doAll:
                    reply=ask_user("import %s (yes/no/all)? " % key,["y","n","a"])
                    if reply == "a": doAll = True
                    if reply == "n": take = False
                if take:
                    print "importing %s " % key
                    if key in mine:
                        self.mergeDataset(mine[key],val)
                    else:
                        mine[key] = val
                        
        self.writeCatalog(mine)

    def importFromDAS(self,list_datasets):
        """
        Import datasets from DAS to the catalog.
        @datasets: wildecard to be usd in dataset query
        """
        # 
        ret,out = commands.getstatusoutput("voms-proxy-info -e")
        if ret != 0:
            print "\n\nNo valid voms proxy found. This is needed to query DAS.\nPlease create a valid proxy running the following command:\nvoms-proxy-init -voms cms\n"
            sys.exit(-1)
        
        catalog = self.readCatalog()
        
        print "Importing from das %s" % list_datasets
        datasets = []
        for dataset in list_datasets:
            if "*" in dataset:
                # response = das_query("https://cmsweb.cern.ch","dataset dataset=%s | grep dataset.name" % dataset, 0, 0, False, self.dbs_instance_, ckey=x509(), cert=x509())
                # response = das_query("https://cmsweb.cern.ch","dataset dataset=%s instance=%s | grep dataset.name" % (dataset, self.dbs_instance_), 0, 0, False, ckey=x509(), cert=x509())
                response = das_query("https://cmsweb.cern.ch","dataset dataset=%s instance=%s | grep dataset.name" % (dataset, self.dbs_instance_), 0, 0, False)
                ## print response
                for d in response["data"]:
                    ## print d
                    datasets.append( d["dataset"][0]["name"] )
            else:
                datasets.append(dataset)

        print "Datasets to import"
        print "\n".join(datasets)
        for dsetName in datasets:
            print "Importing %s" % dsetName
            files = self.getFilesFomDAS(dsetName)
            self.addToDataset(catalog,dsetName,files)
            
        print "Writing catalog"
        self.writeCatalog(catalog)
        print "Done"
    

    def getFilesFomDAS(self,dsetName):
        """
        Read dataset files from DAS.
        @dsetName: dataset name
        """
        ## response = das_query("https://cmsweb.cern.ch","file dataset=%s | grep file.name,file.nevents" % dsetName, 0, 0, False, self.dbs_instance_, ckey=x509(), cert=x509())
        response = das_query("https://cmsweb.cern.ch","file dataset=%s instance=%s | grep file.name,file.nevents" % (dsetName,self.dbs_instance_), 0, 0, False, )
        
        files=[]
        for d in response["data"]:
            for jf in d["file"]:
                if "nevents" in jf:
                    files.append({ "name" : jf["name"], "nevents" : jf["nevents"] })
                    break                
        return files

    def importFromEOS(self,folders):
        """
        Import datasets from DAS to the catalog.
        @datasets: dataset to be imported
        """
        catalog = self.readCatalog()
        
        auto=False
        assumeOk=False
        for folder in folders:
            dsetName = ""

            print
            print "importing folder\n %s" % folder
            
            while not len(dsetName.split("/")) == 4:
                if auto:
                    splitFolder = folder.split("/")
                    prim, sec = splitFolder[-4:-2]
                    dsetName = "/%s/%s/USER" % (prim,sec)
                    print "guessed dataset name ", dsetName
                    if not assumeOk:
                        resp=ask_user("ok?",["y","n","a"])
                        if resp == "n":
                            dsetName = ""
                            auto=False
                        elif resp=="a":
                            assumeOk=True
                if not auto:
                    print "enter dataset name (auto/noauto to enables/disables automatic guessing) ",
                    dsetName = raw_input()
                    if(dsetName=="auto"):
                        auto=True
                    elif (dsetName=="noauto"):
                        auto=False
                
                
            print "Importing %s as %s" % (folder,dsetName)
            files = self.getFilesFomEOS(folder)            
            self.addToDataset(catalog,dsetName,files)
            
        print "Writing catalog"
        self.writeCatalog(catalog)
        print "Done"
        
    def getFilesFomEOS(self,dsetName):
        """
        Read dataset files crawling EOS.
        @dsetName: dataset name
        Note: not implemented
        """
        
        if not self.parallel_:
            self.parallel_ = Parallel(200,self.queue_,maxThreads=self.maxThreads_,asyncLsf=True)
        
        ret,out = self.parallel_.run("/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select",["find",dsetName],interactive=True)[2]
        files = []
        for line in out.split("\n"):
            if line.endswith(".root") and not "failed" in line:
                files.append( {"name":line.replace("/eos/cms",""), "nevents":0} )

        return files

    def findDuplicates(self,dsetName):
        """
        Find duplicate job outputs in dataset.
        @dsetName: dataset name
        Note: not implemented
        """
        pass
    
    def invalidateBadFiles(self,dsetName):
        """
        Invalidate duplicate job output and corrupted files in DAS.
        @dsetName: dataset name
        Note: not implemented
        """
        pass

    def checkAllDatasets(self,match=None,light=False,justOpen=False):
        """
        Look for corrupted files in the whole catalog.
        """
        catalog = self.readCatalog()
        
        self.just_open_ = justOpen
        factory = WorkNodeJobFactory(os.getcwd(),stage_patterns=[".tmp*.json"],job_outdir=".fgg")
        self.parallel_ = Parallel(50,self.queue_,maxThreads=self.maxThreads_,asyncLsf=True,lsfJobName=".fgg/job",jobDriver=factory)
        ## self.parallel_ = Parallel(1,self.queue_)
        
        print "Checking all datasets"
        self.outcomes = []
        for dataset in catalog.keys():  
            if match and not (dataset == match or fnmatch(dataset,match)): 
                continue
            self.checkDatasetFiles(dataset,catalog,light=light)
        # write catalog to avoid redoing duplicates removal
        self.writeCatalog(catalog)
                
        if self.queue_:
            self.parallel_.wait(printOutput=True,handler=self)
            outcomes = self.outcomes
        else:
            outcomes = self.parallel_.wait(printOutput=False)
            
        nfailed = 0
        for oc in outcomes:
            try:
                ign1, ign2, outcome= oc
            except:
                outcome = None                
            if not outcome:
                if not self.continue_:
                    print "Error getting file check outcome. Someting went wrong.... \n", oc
                continue
            dsetName,ifile,fName,ret,out = outcome
            info = catalog[dsetName]["files"][ifile]
            if info["name"] != fName:
                print "Inconsistent outcome ", info["name"], dsetName,ifile,fName,ret,out
            else:
                if ret != 0:
                    info["bad"] = True
                    nfailed += 1
                else:
                    info["bad"] = False
                    if not self.just_open_:
                        try:
                            extraInfo = json.loads(str(out))
                        except Exception as e:
                            print e
                            print "WARNING: problem with extraInfo json line, so zeroing the dictionary, setting to bad, and skipping"
                            extraInfo = {}
                        if len(extraInfo.keys()) == 0:
                            nfailed += 1
                            info["bad"] = True
                        for key,val in extraInfo.iteritems():
                            info[key] = val
                            
        self.parallel_.stop()

        print "Writing catalog"
        self.writeCatalog(catalog)
        print "Done"

        if nfailed > 0:
            print 
            print "WARNING: some of the check jobs failed or did not return any output."
            print "         Those (%d) files were marked a bad and won't be usable for analysis." % nfailed
            print "         Re-running the check command may recover the temporary failures."
            print 
        
        if self.queue_:
            print 
            print "Note: log files may have been written in ./.fgg"
            print "      it's up to you to clean up though..."

    
    def checkDatasetFiles(self,dsetName,catalog=None,light=False):
        """
        Look for corrupted files in dataset.
        @dsetName: dataset name
        Note: not implemented
        """
        writeCatalog = False
        if not catalog:
            catalog = self.readCatalog()
            writeCatalog = True
        
        wait = False
        if not self.parallel_:
            self.parallel_ = Parallel(16,self.queue_,maxThreads=self.maxThreads_,asyncLsf=True)
            wait = True

        print 
        print "Checking dataset",dsetName
        info = catalog[dsetName]
        files = info["files"]
        print "Number of files: ", len(files)
        
        if self.force_ or not catalog[dsetName].get("vetted",False):
            toremove = []
            keep_wildcard=None
            for ifil,eifil in enumerate(files):
                if ifil in toremove:
                    continue
                for jfil,ejfil in enumerate(files[ifil+1:]):
                    if ifil+jfil in toremove:
                        continue
                    if eifil["name"] == ejfil["name"]:
                        toremove.append(ifil)
                    else:
                        iid = eifil["name"].rstrip(".root").rsplit("_",1)[-1]
                        jid = ejfil["name"].rstrip(".root").rsplit("_",1)[-1]
                        if iid == jid:
                            if not keep_wildcard:
                                print "duplicated file index ", iid
                                print eifil["name"]
                                print ejfil["name"]
                                reply=ask_user("keep both (yes/no/matching)? ",["y","n","m"])
                                if reply == "m":             
                                    while not keep_wildcard:
                                        print "enter wildcard matching expression",
                                        keep_wildcard=raw_input()
                                        if ask_user("keep all files matching '%s'?" % keep_wildcard) == "n":
                                            keep_wildcard=None
                            if keep_wildcard:                            
                                imatch=fnmatch(eifil["name"],keep_wildcard)
                                jmatch=fnmatch(ejfil["name"],keep_wildcard)
                                if imatch != jmatch:
                                    if imatch: toremove.append(ifil+jfil)
                                    else: toremove.append(ifil)                            
                                    continue                       
                                else:
                                    print "duplicated file index ", iid
                                    print eifil["name"]
                                    print ejfil["name"]
                                    reply=ask_user("keep both? ")
                            if reply == "n":
                                if ask_user( "keep %s? " % ejfil["name"] ) == "n":
                                    toremove.append(ifil+jfil)
                                if ask_user( "keep %s? " % eifil["name"] ) == "n":
                                    toremove.append(ifil)
                                    
            for ifile in sorted(toremove,reverse=True):
                files.pop(ifile)
            
        print "After duplicates removal: ", len(files)
        nsub = 0
        catalog[dsetName]["vetted"] = True
        if not light:
            info = catalog[dsetName]["files"] = files
            for ifile,finfo in enumerate(files):            
                name = finfo["name"]                
                if self.force_ or not "weights" in finfo:
                    nsub+=1
                    self.parallel_.run(SamplesManager.checkFile,[self,name,dsetName,ifile],interactive=(self.queue_!=None))
        if nsub == 0:
            print "No files needed to be checked"
        else:
            print "Submitted %d check jobs" % nsub
            
        if wait:
            self.parallel_.wait(printOutput=False)            
            self.parallel_ = None
            
        if writeCatalog:
            self.writeCatalog(catalog)

    def reviewCatalog(self):
        datasets,catalog = self.getAllDatasets()

        primaries = {}
        keepAll = False
        dataregex = re.compile("Run[0-9]+[A-Z]")
        for d in datasets:
            if not keepAll:
                reply = ask_user("keep this dataset (yes/no/all)?\n %s\n" % d, ["y","n","a"])
                if reply == "n":
                    catalog.pop(d)
                    continue
                if reply == "a": 
                    keepAll = True
            primary,secondary = d.split("/")[1:3]
            search = dataregex.search(secondary)
            if search: primary += "/%s" % search.group()
            if not primary in primaries:
                primaries[ primary ] = []
                
            primaries[ primary ].append(d)
            
        for name,val in primaries.iteritems():
            if len(val) == 1: continue
            reply = ask_user("More than one sample for %s:\n %s\nKeep all (yes/no/merge)?" % (name,"\n ".join(val)),["y","n","m"])
            if reply == "m":
                dst = val[0]
                for merge in val[1:]:
                    self.mergeDataset(catalog[dst],catalog[merge])
                    catalog.pop(merge)
            if reply == "n":
                for d in val:
                    reply = ask_user("keep this dataset?\n %s\n" % d)
                    if reply == "n":
                        catalog.pop(d)
           
        self.writeCatalog(catalog)
        
    def mergeDataset(self,dst,merge):
        dst["vetted"]=False
        
        from FWCore.PythonUtilities.LumiList import LumiList
        dstLumisToSkip = LumiList(compactList=dst.get('lumisToSkip',{}))
        mergeLumisToSkip = LumiList(compactList=merge.get('lumisToSkip',{}))
        dstLumisToSkip += mergeLumisToSkip
        dstLumisToSkip = dstLumisToSkip.compactList
        if len(dstLumisToSkip) > 0:
            dst['lumisToSkip'] = dstLumisToSkip
            print "\nWARNING: Merged lumisToSkip list. It is reccomended to run the 'overlap' command to re-geneate the list from scratch."
        
        dstFiles=dst["files"]
        mergeFiles=merge["files"]
        for fil in mergeFiles:
            skip = False
            for dfil in dstFiles:
                if dfil["name"] == fil["name"]:
                    skip = True
            if not skip:
                dstFiles.append( fil )
        
    def addToDataset(self,catalog,dsetName,files):
        if dsetName in catalog:
            if self.force_:
                catalog[ dsetName ]["files"]  = files
            else:
                self.mergeDataset(catalog[ dsetName ],{ "files" : files })
        else:
            catalog[ dsetName ] = { "files" : files }
            

    def checkFile(self,fileName,dsetName,ifile):
        """
        Check if file is valid.
        @fileName: file name
        """
        fName = fileName
        tmp = ".tmp%s_%d.json"%(dsetName.replace("/","_"),ifile)
        if self.continue_:
            if os.path.exists(tmp):
                print "%s already exists" % tmp
                outcome = self.readJobOutput(tmp,0,"",dsetName,fileName,ifile)
                if self.queue_:
                    self.outcomes.append((None,None,outcome))
                else:
                    return outcome
            return None
        if self.just_open_:
            ret,out = self.parallel_.run("fggOpenFile.py",[fName,tmp,dsetName,str(ifile)],interactive=True)[2]
        elif self.queue_:
            self.parallel_.run("fggCheckFile.py",[fName,tmp,dsetName,str(ifile)],interactive=False)
            return
        else:
            ret,out = self.parallel_.run("fggCheckFile.py",[fName,tmp,dsetName,str(ifile)],interactive=True)[2]

        if ret != 0:
            print "ERROR checking %s" % fName
            print out
        return self.readJobOutput(tmp,ret,out,dsetName,fileName,ifile)


    def readJobOutput(self,tmp,ret,out,dsetName,fileName,ifile):
        try:
            fout = open(tmp)
            out = fout.read()
            fout.close()
            os.remove(tmp)
        except Exception, e:
            print ret, out 
            print e
            out = "{}"

        return dsetName,int(ifile),fileName,ret,out
    
    def handleJobOutput(self,job,jobargs,ret):
        
        jobargs = jobargs[0].split(" ")[1:]

        fileName = jobargs[0]
        tmp = jobargs[1]
        dsetName = jobargs[2]
        ifile = jobargs[3]
        ret,out =ret

        self.outcomes.append( (None,None,self.readJobOutput(tmp,ret,out,dsetName,fileName,ifile))) 
        return 0
        ## return dsetName,ifile,fileName,ret,out
    
    def getDatasetLumiList(self,name,catalog,check=False):
        from FWCore.PythonUtilities.LumiList import LumiList

        lumisToSkip = catalog[name].get('lumisToSkip',None)
        if lumisToSkip:
            print "Dataset %s has list of lumi sections to skip in catalog" % name
            lumisToSkip = LumiList(compactList=lumisToSkip)
        dlist = LumiList()
        for fil in catalog[name]["files"]:
            flist = LumiList( runsAndLumis=fil.get("lumis",{}) )
            if lumisToSkip and not check:
                flist = flist.__sub__(lumisToSkip)
            if check:
                andlist = dlist.__and__(flist)
                ## print andlist,  fil.get("name")
                if len(andlist) != 0:
                    print "Warning: duplicate lumi sections in dataset. %s" % fil.get("name")
                    print andlist, flist
            dlist += flist
        
        return dlist

    def getOverlaps(self,*args):
        catalog = self.readCatalog(True)
        
        datasets = {}
        for dataset in catalog.keys():
            for arg in args:
                if dataset == arg or fnmatch(dataset,arg):
                    datasets[dataset] = self.getDatasetLumiList(dataset,catalog,check=True)
                    break
        
        keys = datasets.keys()
        for ik,ikey in enumerate(keys):
            for jkey in keys[ik+1:]:
                overlap = datasets[ikey].__and__(datasets[jkey])
                print ikey
                print jkey
                overlaps = overlap.compactList
                print overlaps
                if len(overlaps) > 0:
                    for key in ikey,jkey:
                        reply=ask_user("\nMask lumi sections in\n %s (yes/no)? " % key,["y","n"])
                        if reply == 'y': 
                            catalog[key]["lumisToSkip"] = overlaps
                            break
                        
        print "Writing catalog"
        self.writeCatalog(catalog)
        print "Done"


    def getLumisToSkip(self,dataset):
        catalog = self.readCatalog(True)
        if not dataset in catalog:
            return None

        from FWCore.PythonUtilities.LumiList import LumiList
        
        return LumiList( compactList=catalog[dataset].get('lumisToSkip',{}) )
    

    def getLumiList(self,*args):
        
        catalog = self.readCatalog(True)
        datasets = []
        output = filter(lambda x: "output=" in x, args)
        args = filter(lambda x: not "output=" in x, args)
        for dataset in catalog.keys():
            for arg in args:
                if dataset == arg or fnmatch(dataset,arg):
                    datasets.append(dataset)
                    break
        if len(output) > 1:
            print "ERROR: you specified the output json more than once:\n"
            print "      %s" % " ".join(output)
            sys.exit(-1)
        

        if len(output) > 0:
            output = output[0].replace("output=","")
        else:
            output = None
            
        from FWCore.PythonUtilities.LumiList import LumiList
        fulist = LumiList()
        for dataset in datasets:
            ## dlist = LumiList()
            dlist = self.getDatasetLumiList(dataset,catalog)
            jsonout = dataset.lstrip("/").rstrip("/").replace("/","_")+".json"
            ### for fil in catalog[dataset]["files"]:
            ###     flist = LumiList( runsAndLumis=fil.get("lumis",{}) )
            ###     ## print flist
            ###     dlist += flist
            if not output:
                with open(jsonout,"w+") as fout:
                    fout.write(json.dumps(dlist.compactList,sort_keys=True))
                    fout.close()
            else:
                fulist += dlist
                
        if output:
            with open(output,"w+") as fout:
                fout.write(json.dumps(fulist.compactList,sort_keys=True))
                fout.close()
        
    def lockCatalog(self):
        """
        Lock catalog file for writing.
        Note: not implemented.
        """
        pass
    
    def unlockCatalog(self):
        """
        Unlock catalog file for writing.
        Note: not implemented.
        """
        pass
    
    def readCatalog(self,throw=False,path=None):
        """
        Read catalog from JSON file.
        @throw: thow exception if file does not exists.
        """
        if not path:
            path = self.catalog_
        if os.path.exists(path):
            return json.loads( open(path).read() )
        if throw:
            raise Exception("Could not find dataset catalog %s" % ( path ))
        return {}
    
    def writeCatalog(self,content):
        """
        Write catalog to JSON file.
        @content: catalog content.
        """
        if not os.path.exists( os.path.dirname(self.catalog_) ):
            os.mkdir( os.path.dirname(self.catalog_) )

        with open(self.catalog_,"w+") as fout:
            fout.write( json.dumps(content,indent=4,sort_keys=True) )
            fout.close()
    
    def getDatasetMetaData(self,maxEvents,primary,secondary=None,jobId=-1,nJobs=0):
        """
        Extract dataset meta data.
        @maxEvents: maximum number of events to read.
        @primary: primary dataset name.
        @secondary: secondary dataset name.
        
        returns: tuple containing datasetName,cross-section,numberOfEvents,listOfFiles
        
        """
        catalog = self.readCatalog(True)
        primary = primary.lstrip("/")
        if "/" in primary:
            primary,secondary,tier = primary.split("/")
        found = False
        xsec  = None
        allFiles = []
        totEvents = 0.
        totWeights = 0.
        for dataset,info in catalog.iteritems():
            empty,prim,sec,tier=dataset.split("/")
            if prim == primary:
                if secondary and sec != secondary:
                    continue
                if found:
                    raise Exception("More then one dataset matched the request: /%s/%s" % ( primary, str(secondary) ))
                found = dataset
                if prim in self.cross_sections_:
                    xsec = self.cross_sections_[prim]
                for fil in info["files"]:
                    if fil.get("bad",False):
                        continue
                    nev, name = fil["nevents"], fil["name"]
                    totEvents += nev
                    totWeights += fil.get("weights",0.)
                    allFiles.append(name)
                    if maxEvents > -1 and totEvents > maxEvents:
                        break
        if not found:
            raise Exception("No dataset matched the request: /%s/%s" % ( primary, str(secondary) ))
        
        if jobId != -1 and maxEvents > -1 and totEvents > maxEvents:
            totWeights = maxEvents / totEvents * totWeights
            totEvents = maxEvents
        maxEvents = int(totEvents)
        
        if totWeights != 0.:
            totEvents = totWeights
            
        if jobId != -1:
            files = [ allFiles[i] for i in range(jobId,len(allFiles),nJobs) ]
        else:
            files = allFiles

        return found,xsec,totEvents,files,maxEvents

    def getAllDatasets(self):
        catalog = self.readCatalog()
        datasets = sorted(catalog.keys())
        return datasets,catalog
    
    def clearCatalog(self):
        self.writeCatalog({})
    
# -------------------------------------------------------------------------------
class SamplesManagerCli(SamplesManager):
    
    def __init__(self,*args,**kwargs):

        commands = [ "",
                     "import    [list_of_wildcards]                    imports datasets from DBS to catalog", 
                     "eosimport <list_of_folders>                      imports datasets from EOS", 
                     "catimport [source:]<catalog_name> <wildcard>     imports datasets from another catalog", 
                     "list      [raw|wildcard]                         lists datasets in catalog", 
                     "review                                           review catalog to remove datasets", 
                     "check      [wildcard]                            check duplicate files and errors in datasets and mark bad files",
                     "checkopen  [wildcard]                            as above but just try open file",
                     "checklite  [wildcard]                            check for duplicate files in datasets",
                     "getlumi    [wildcard|datasets]                   get list of processed lumi sections in dataset",
                     "overlap    [wildcard|datasets]                   checks overlap between datatasets"
                     ]
        
        parser = OptionParser(
            usage="""%%prog [options] <command> [[command2] [command3] ..]

Command line utility to handle FLASHgg samples catalog.

Commands:
%s
            """ % "\n   ".join(commands),
            option_list=[
                make_option("-V","--flashggVersion",
                            action="store", dest="flashggVersion", type="string",
                            default="*",
                            help="FLASHgg version to use (only relevant when importing). default: %default", 
                            ),
                make_option("-C","--campaign",
                            dest="campaign",action="store",type="string",
                            default="",
                            help="production campaign. default: %default",
                            ),
                make_option("-d","--dbs-instance",
                            dest="dbs_instance",action="store",type="string",
                            default="prod/phys03",
                            help="DBS instance to use. default: %default",
                            ),
                make_option("-m","--metaDataSrc",
                            dest="metaDataSrc",action="store",type="string",
                            default="flashgg",
                            help="MetaData package to use. default: %default",
                            ),
                make_option("-c","--continue",
                            dest="doContinue",action="store_true",
                            default=False,
                            help="Continue previous check",
                            ),
                make_option("--force",
                            dest="doForce",action="store_true",
                            default=False,
                            help="Force actions",
                            ),
                make_option("--load",  # special option to load whole configuaration from JSON
                            action="callback",callback=Load(),dest="__opt__",
                            type="string",
                            help="load JSON file with configuration",metavar="CONFIG.json"
                            ),
                make_option("-q","--queue",
                            dest="queue",action="store",type="string",
                            default=None,
                            help="Run jobs in batch using specified queue. default: %default",
                            ),
                make_option("-M","--max-threads",
                            dest="max_threads",action="store",type="int",
                            default=20,
                            help="Maximum number of threads to use. default: %default",
                            ),
                make_option("-v","--verbose",
                            action="store_true", dest="verbose",
                            default=False,
                            help="default: %default",)
                ]
                              )
        
        # parse the command line
        (self.options, self.args) = parser.parse_args()
        
    
    def __call__(self):
        
        (options,args) = (self.options,self.args)
        
        print options
        self.mn = SamplesManager("$CMSSW_BASE/src/%s/MetaData/data/%s/datasets.json" % (options.metaDataSrc,options.campaign),
                                 dbs_instance=options.dbs_instance,
                                 force=options.doForce,
                                 queue=options.queue,maxThreads=options.max_threads,doContinue=options.doContinue)
        
        ## pprint( mn.cross_sections_ )
        if len(args) == 0:
            args = ["list"]
        
        method = getattr(self,"run_%s" % args[0],None)
        if not method:
            sys.exit("Unkown command %s" % args[0])
        if len(args)>1:
            method(*args[1:])
        else:
            method()
            
    def run_import(self,*args):
        if len(args)>0:
            print args
            self.mn.importFromDAS(list(args))
        else:
            self.mn.importFromDAS(["/*/*%s-%s*/USER" % (self.options.campaign,self.options.flashggVersion)])
    
    def run_eosimport(self,*args):
        self.mn.importFromEOS(args)
        
    def run_catimport(self,src,pattern):
        if ":" in src:
            print src.split(":")
            src = "$CMSSW_BASE/src/%s/MetaData/data/%s/datasets.json" % tuple(src.split(":"))
        else:
            src = "$CMSSW_BASE/src/%s/MetaData/data/%s/datasets.json" % ( self.options.metaDataSrc, src )
        self.mn.importFromCatalog(shell_expand(src),pattern)

    def run_check(self,*args):
        self.mn.checkAllDatasets(*args)

    def run_checklite(self,*args):
        self.mn.checkAllDatasets(*args,light=True)

    def run_checkopen(self,*args):
        self.mn.checkAllDatasets(*args,justOpen=True)
    
    def run_getlumi(self,*args):
        self.mn.getLumiList(*args)
    
    def run_overlap(self,*args):
        self.mn.getOverlaps(*args)

    def run_list(self,what=None):        
        datasets,catalog = self.mn.getAllDatasets()
        if what=="raw":
            for d in datasets:
                print d
            return
        if what:
            datasets = [ d for d in datasets if d==what or fnmatch(d,what) ]
        maxSec = 50
        halfSec = maxSec / 2
        firstHalf = halfSec - 1
        secondHalf = maxSec - halfSec - 1
        slim_datasets = []
        for d in datasets:
            empty,prim,sec,tier = d.split("/")
            if not self.options.verbose and len(sec) > maxSec:
                sec = sec[0:firstHalf]+".."+sec[-secondHalf:-1]
            slim_datasets.append("/%s/%s/%s" % ( prim, sec, tier ) )
        ## datasets = slim_datasets
        ## largest = max( [len(d) for d in datasets] )
        totev = 0.
        totwei = 0.
        totfiles = 0
        largest = max( [50]+[len(d) for d in slim_datasets] )
        print
        print "Datasets in catalog:"
        print "-"*(largest+37)
        print "Name".ljust(largest), ("N events").rjust(11), ("N good").rjust(7), ("N bad" ).rjust(7), ("Avg"   ).rjust(7)
        print "    ".ljust(largest), (""        ).rjust(11), ("files" ).rjust(7), ("files" ).rjust(7), ("weight").rjust(7)
        print "-"*(largest+37)
        for d,n in zip(datasets,slim_datasets):
            nevents = 0.
            weights = 0.
            nfiles = len(catalog[d]["files"])
            for fil in catalog[d]["files"]:
                if fil.get("bad",False):
                    nfiles -= 1
                    continue
                nevents += fil.get("nevents",0.)
                weights += fil.get("weights",0.)
            print n.ljust(largest), ("%d" % int(nevents)).rjust(11), ("%d" % nfiles).rjust(7),
            print ("%d" % (len(catalog[d]["files"]) - nfiles )).rjust(7),
            if weights != 0.: print ("%1.2g" % ( weights/nevents ) ).rjust(7),
            else: print " ".rjust(7),
            print
            totev += nevents
            totwei += weights
            totfiles += nfiles
        print "-"*(largest+37)
        print "total".rjust(largest), ("%d" % int(totev)).rjust(11), ("%d" % totfiles).rjust(7)
        
    def run_clear(self):
        self.mn.clearCatalog()
    
    def run_review(self):
        self.mn.reviewCatalog()
