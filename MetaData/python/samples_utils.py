from optpars_utils import *

from das_cli import get_data as das_query

from pprint import pprint

import os,json,fcntl

# -------------------------------------------------------------------------------
def shell_expand(string):
    if string:
        return os.path.expanduser( os.path.expandvars(string) )
    return None

# -------------------------------------------------------------------------------
class SamplesManager(object):
    
    def __init__(self,
                 catalog,
                 cross_sections=["$CMSSW_BASE/src/flashgg/MetaData/data/cross_sections.json"],
                 dbs_instance="prod/phys03",
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
            self.cross_sections_.update( json.loads( open(shell_expand(xsecFile)).read() ) )
            
        self.catalog_ = shell_expand(catalog)

    def importFromDAS(self,datasets):
        """
        Import datasets from DAS to the catalog.
        @datasets: wildecard to be usd in dataset query
        """
        catalog = self.readCatalog()
        
        if "*" in datasets:
            response = das_query("https://cmsweb.cern.ch","dataset dataset=%s | grep dataset.name" % datasets, 0, 0, False, self.dbs_instance_)
        
        datasets=[]
        for d in response["data"]:
            datasets.append( d["dataset"][0]["name"] )
    
        for dsetName in datasets:
            files = self.getFilesFomDAS(dsetName)
            if dsetName in catalog:
                catalog[ dsetName ]["files"]  = files
            else:
                catalog[ dsetName ] = { "files" : files }
            
        self.writeCatalog(catalog)

    def getFilesFomDAS(self,dsetName):
        """
        Read dataset files from DAS.
        @dsetName: dataset name
        """
        response = das_query("https://cmsweb.cern.ch","file dataset=%s | grep file.name,file.nevents" % dsetName, 0, 0, False, self.dbs_instance_)
        
        files=[]
        for d in response["data"]:
            files.append( { "name" : d["file"][0]["name"], "nevents" : d["file"][0]["nevents"] } )
        
        return files

    def getFilesFomEOS(self,dsetName):
        """
        Read dataset files crawling EOS.
        @dsetName: dataset name
        Note: not implemented
        """
        pass
    
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

    def checkDatasetFiles(self,dsetName):
        """
        Look for corrupted files in dataset.
        @dsetName: dataset name
        Note: not implemented
        """
        pass

    def checkFile(self,fileName):
        """
        Check if file is valid.
        @fileName: file name
        Note: not implemented
        """
        pass
    
    
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
    
    def readCatalog(self,throw=False):
        """
        Read catalog from JSON file.
        @throw: thow exception if file does not exists.
        """
        if os.path.exists(self.catalog_):
            return json.loads( open(self.catalog_).read() )
        if throw:
            raise Exception("Could not find dataset catalog %s" % ( self.catalog_ ))
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
    
    def getDatasetMetaData(self,maxEvents,primary,secondary=None):
        """
        Extract dataset meta data.
        @maxEvents: maximum number of events to read.
        @primary: primary dataset name.
        @secondary: secondary dataset name.
        
        returns: tuple containing datasetName,cross-section,numberOfEvents,listOfFiles
        
        """
        catalog = self.readCatalog(True)
        primary = primary.lstrip("/")
        found = False
        xsec  = 0.
        files = []
        totEvents = 0
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
                    nev, name = fil["nevents"], fil["name"]
                    totEvents += nev
                    files.append(name)
                    if maxEvents > -1 and totEvents > maxEvents:
                        break
        if not found:
            raise Exception("No dataset matched the request: /%s/%s" % ( primary, str(secondary) ))
        
        return found,xsec,totEvents,files

if __name__ == "__main__":
    
    parser = OptionParser(option_list=[
            make_option("-V","--flashggVersion",
                        action="store", dest="flashggVersion", type="string",
                        default="*",
                        help="FLASHgg version to use. default: %default", 
                        ),
            make_option("-C","--campaign",
                        dest="campaign",action="store",type="string",
                        default="CSA14",
                        help="production campaign. default: %default",
                        ),
            make_option("--load",  # special option to load whole configuaration from JSON
                        action="callback",callback=Load(),dest="__opt__",
                        type="string",
                        help="load JSON file with configuration",metavar="CONFIG.json"
                        ),
            make_option("-v","--verbose",
                        action="store_true", dest="verbose",
                        default=False,
                        help="default: %default",)
            ]
                          )
        
    # parse the command line
    (options, args) = parser.parse_args()
    
    mn = SamplesManager("$CMSSW_BASE/src/flashgg/MetaData/data/%s/datasets.json" % options.campaign)
    
    pprint( mn.cross_sections_ )
    mn.importFromDAS("/*/*%s-%s*/USER" % (options.campaign,options.flashggVersion) )
    
