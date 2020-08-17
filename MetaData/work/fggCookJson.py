#!/bin/env python

from flashgg.MetaData.optpars_utils import *
import os

from glob import glob
import commands
import sys

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
                make_option("--field", action="store", type="string", dest="field",
                            default="3.8T"),
                make_option("--extra", action="store", type="string", dest="extra",
                            default=None),
                make_option("--bunch-space", action="store", type="string", dest="bunch_space",
                            default="25ns",),
                make_option("--dqm-folder", action="store", type="string", dest="dqm_folder",
                            default="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/"
                            ),
                ])

        (self.options, self.args) = parser.parse_args()

        self.cert_mapping = { "0T" : "_ZeroTesla", "3.8T" : "" }
        self.dcs_mapping  = { "0T" : "_0T",        "3.8T" : "" }
                
    # -------------------------------------------------------------------------------------------------------------------
    def __call__(self):
        
        if self.options.bunch_space != "":
            self.options.bunch_space = "_%s" % self.options.bunch_space
        cert_pattern = "Cert_*_13TeV_*_Collisions*%s%s_JSON.txt" % ( self.cert_mapping[self.options.field], self.options.bunch_space )
        search = os.path.join(self.options.dqm_folder,cert_pattern )
        print search
        files = sorted( map(lambda x: (int(x.split("-")[1].rsplit("_13TeV")[0]),x),  
                            filter( lambda x: not "MuonPhys" in x and not "CaloOnly" in x, glob(search)) ), 
                        key=lambda x: x[0] )
        if len(files) > 0:
            latest_run = files[-1][0]
            latest_files = sorted( map(lambda x: (os.path.getmtime(x[1]), x[1]), 
                                       filter(lambda x: x[0] == latest_run, files) ), 
                                   key=lambda x: x[0] )

            most_recent_time, most_recent = latest_files[-1]
            latest_run += 1
        else:
            latest_run = 0
            most_recent_time, most_recent = 0, None

        dcs_only =  os.path.join(self.options.dqm_folder,"DCSOnly","json_DCSONLY%s.txt" % self.dcs_mapping[self.options.field] )
        most_recent_time = max( most_recent_time, os.path.getmtime(dcs_only) )
        print "Most recent jsons:\n %s\n %s" % (most_recent,dcs_only)

        my_json = "myjson_%s_%s_%d.txt" % ( self.options.field, self.options.bunch_space, latest_run  )
        if not os.path.exists(my_json) or os.path.getmtime(my_json) < most_recent_time:
            print "updating %s" % my_json
        
            run = "filterJSON.py --min %d %s --output myjson_DCSONLY%s-%d.txt" % (latest_run, dcs_only, self.options.field, latest_run)
            print run
            ret,out = commands.getstatusoutput(run)
            print out            

            if most_recent:
                run =  "compareJSON.py --or myjson_DCSONLY%s-%d.txt %s %s" % (self.options.field, latest_run, most_recent, my_json)
            else:
                run =  "cp -p myjson_DCSONLY%s-%d.txt %s" % (self.options.field, latest_run, my_json)
            print run
            ret,out = commands.getstatusoutput(run)
            print out
            
            if self.options.extra:
                run =  "compareJSON.py --or %s %s %s.tmp && mv %s.tmp %s" % (my_json, self.options.extra, my_json, my_json, my_json )
                print run
                ret,out = commands.getstatusoutput(run)
                print out

       
if __name__ == "__main__":
    JsonManipulator()()
    
