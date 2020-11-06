############################################################################################################################################################################################
# Abraham Tishelman-Charny 
# 23 October 2020
#
# The purpose of this module is to submit flashgg jobs to HTCondor.
#
# Example Usage:
#
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label Private-SL-SM-saveGenVars --nEvents -1 --saveHHWWggFinalStateVars --HHWWgg --dumpTrees --jsonPath Taggers/test/HHWWgg_Combination/SL_Analysis/SL_SM2017.json --year 2017 --condorQueue longlunch --doHHWWggTagCutFlow --extraFlagsStr doubleHReweight=1
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label testNewScript --nEvents 1000 --saveHHWWggFinalStateVars --HHWWgg --dumpTrees --jsonPath Taggers/test/HHWWgg_Combination/SL_Analysis/SL_SM2017.json --year 2017 --condorQueue espresso --doHHWWggTagCutFlow --extraFlagsStr   
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label testNewScript --nEvents 1000 --saveHHWWggFinalStateVars --HHWWgg --dumpTrees --jsonPath Taggers/test/HHWWgg_Combination/SL_Analysis/SL_SM2017.json --year 2017 --condorQueue espresso --doHHWWggTagCutFlow --extraFlagsStr doubleHReweight=1  
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label testNewScript --nEvents -1 --saveHHWWggFinalStateVars --HHWWgg --dumpTrees --jsonPath Taggers/test/HHWWgg_Combination/SL_Analysis/SL_SM2017.json --year 2017 --condorQueue espresso --doHHWWggTagCutFlow --extraFlagsStr doubleHReweight=1  
############################################################################################################################################################################################

# Before running, run:
#
# cmsenv
# voms
# cp /tmp/MYPROXY ~/
# export X509_USER_PROXY=~/MYPROXY

import os 
from python.Options import * 
from python.SubmitJobs_Tools import * 

##-- Get User input Args
args = GetOptions() 

argNames = ['fggDirec','outDirec', 'label', 'nEvents', 'saveHHWWggFinalStateVars', 'HHWWgg', 'doSystematics', 'dumpTrees', 'dumpWorkspace', 'dryRun', 'jsonPath', 'condorQueue', 'year','doHHWWggTagCutFlow', 'extraFlagsStr']
for argName in argNames: 
    exec("%s = args.%s"%(argName,argName))

rootFileOutput = "%s/%s"%(outDirec,label)

os.system("mkdir -p %s"%(label)) 
os.system("mkdir -p %s"%(rootFileOutput))

metaConditionsPath = GetMetaConditionsPath(fggDirec, year)

submitCommand = 'fggRunJobs.py --load %s -D -P -n 500 -d %s --stage-to="%s" -x cmsRun Systematics/test/workspaceStd.py maxEvents=%s -q %s --no-use-tarball --no-copy-proxy metaConditions=%s '%(jsonPath, label, rootFileOutput, nEvents, condorQueue, metaConditionsPath)
if(HHWWgg): submitCommand += " doHHWWggTag=1 HHWWggTagsOnly=1 "
submitCommand += ' %s'%(extraFlagsStr)

flags = ['doSystematics', 'dumpTrees', 'dumpWorkspace', 'doHHWWggTagCutFlow', 'saveHHWWggFinalStateVars']
for flag in flags:
    flagTrue = str(eval(flag))
    submitCommand += " "
    submitCommand += "%s=%s"%(flag,flagTrue)

print("submitCommand:")
print"-------------------------------"
print submitCommand
print"-------------------------------"

if(not dryRun):
    print("Running command:")
    print submitCommand
    os.system(submitCommand)
    print"Finished job for file: %s"%(jsonPath)
