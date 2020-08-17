#!/bin/env python

from flashgg.MetaData.optpars_utils import *
import os

from glob import glob
import commands
import sys

import json

# -------------------------------------------------------------------------------
class JsonManipulator(object):
    
    def __init__(self,
                 defaults={}
                 ):       
        """
        Constructur:
        @defaults: default options
        """

        # Command line options
        parser = OptionParser(option_list=[
                make_option("--dataset",action="callback",type="string",callback=ScratchAppend(),dest="datasets",
                            default=[]),
                make_option("--catalog",action="store",type="string",dest="catalog",
                            default=""),
                make_option("--target",action="store",type="string",dest="target",
                            default=""),
                ])

        (self.options, self.args) = parser.parse_args()
        
    # -------------------------------------------------------------------------------------------------------------------
    def __call__(self):
        
        targets = sorted( map(lambda x: (os.path.getmtime(x), x), glob(self.options.target) ),
                         key=lambda x: x[0] ) # [-1][1]
        print targets
        target = targets[-1][1]
        print target
        self.options.target = target

        for dataset in self.options.datasets: 
            self.processDataset(dataset)
            


    def processDataset(self,dataset):

        run = "das_client.py --query 'run dataset=%s | min(run.run_number)' | tail -1 | awk -F= '{ print $2 }'" % dataset
        ret,out = commands.getstatusoutput(run)
        if ret != 0:
            print "Error running %s" % run
            sys.exit(ret)
        minrun = int(out)
        
        run = "das_client.py --query 'run dataset=%s | max(run.run_number)' | tail -1 | awk -F= '{ print $2 }'" % dataset
        ret,out = commands.getstatusoutput(run)
        if ret != 0:
            print "Error running %s" % run
            sys.exit(ret)
        maxrun = int(out)

        print 'minrun, maxrun: ', minrun, maxrun

        myfolder = "%s_%s" % ( dataset.strip("/").replace("/","_"), self.options.target.rsplit("_",1)[0] )
        if not os.path.exists(myfolder):
            os.mkdir(myfolder)

        run = "filterJSON.py --min %d --max %d %s --output %s/target.json" % (minrun, maxrun, self.options.target, myfolder)
        ret,out = commands.getstatusoutput(run)
        if ret != 0:
            print "Error running %s" % run
            sys.exit(ret)
        

        if ":" in self.options.catalog:
            src,catalog = self.options.catalog.split(":")
        else:
            src,catalog = "flashgg", self.options.catalog
            
        if catalog != "":
            prim,sec,tier=dataset.strip("/").split("/")
            run = "fggManageSamples.py -m %s -C %s getlumi /%s/*%s*/USER output=%s/processed.json" % (src,catalog,prim,sec,myfolder)
            ret,out = commands.getstatusoutput(run)
            if ret != 0:
                print "Error running %s" % run
                sys.exit(ret)
        else:
            with open("%s/processed.json" % myfolder, "w+") as processed:
                processed.write("{}\n")
                processed.close()
            
        run = "compareJSON.py --sub %s/target.json %s/processed.json %s/missing.json" % (myfolder,myfolder,myfolder)
        ret,out = commands.getstatusoutput(run)
        if ret != 0:
            print "Error running %s" % run
            sys.exit(ret)
        with open("%s/missing.json" % myfolder) as mis:
            print "%s/missing.json" % myfolder
            print mis.read()
            mis.close()
        
        missing = { "data" : [] }
        if os.path.exists("all_missing.json"):
            with open("all_missing.json","r") as almis:
                try:
                    cont = almis.read()
                    missing.update(json.loads(cont))
                    almis.close()
                except:
                    print "Could not decode json in all_missing.json [%s]"  % cont
                
        missing["data"].append([dataset,os.path.abspath("%s/missing.json" % myfolder)])
        with open("all_missing.json","w+") as almis:
            almis.write(json.dumps(missing))
            almis.close()
        
            
if __name__ == "__main__":
    JsonManipulator()()
    
