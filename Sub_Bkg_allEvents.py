import os
import argparse 

parser = argparse.ArgumentParser()
parser.add_argument('--outDirec', type=str, default="", help="Output directory")
args = parser.parse_args()

outDirec = args.outDirec

if (not os.path.isdir(outDirec)):
    print"%s is not an existing directory"%(outDirec)
    print"Exiting"
    exit(1)

# outputDirectory = "/eos/user/a/atishelm/ntuples/HHWWgg_flashgg/January_2021_Production/2017/Backgrounds_Lowevents/Flashgg_bkg_short_lowEvents/"
command = "fggRunJobs.py --load Taggers/test/HHWWgg/January_2021_Production/2017/Backgrounds/Flashgg_bkg.json -D -P -n 500 -d Flashgg_2017_bkg_AllEvents --stage-to=%s -x cmsRun Systematics/test/workspaceStd.py maxEvents=-1 -q tomorrow --no-use-tarball --no-copy-proxy metaConditions=$CMSSW_BASE/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json doHHWWggTag=1 HHWWggTagsOnly=1 doSystematics=0 dumpTrees=1 dumpWorkspace=0 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 copyInputMicroAOD=1"%(outDirec)
print"Submit command:"
print "====================="
print command 
print "====================="
os.system(command)
print "SUBMITTED"