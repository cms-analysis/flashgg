##-- Run Locally 

##-- Fully Leptonic 
# cmsRun Systematics/test/workspaceStd.py metaConditions=MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json campaign=Era2017_RR-31Mar2018_v2 dataset=GluGluToHHTo2G2l2nu_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8 doHHWWggTag=1 HHWWggTagsOnly=1 maxEvents=500 doSystematics=0 dumpWorkspace=0 dumpTrees=1 useAAA=1 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 HHWWggAnalysisChannel=all HHWWgguseZeroVtx=1 doHHWWggDebug=1


##-- Signal
##-- Semi-Leptonic
# fggRunJobs.py --load Taggers/test/HHWWgg/SL_SM2017.json -D -P -n 500 -d SL_SM2017 --stage-to=/eos/user/a/atishelm/ntuples/HHWWgg_flashgg/TaggerOutput/SL_SM2017/ -x cmsRun Systematics/test/workspaceStd.py maxEvents=100000 -q longlunch --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json doHHWWggTag=1 HHWWggTagsOnly=1 doSystematics=0 dumpTrees=1 dumpWorkspace=0 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 copyInputMicroAOD=1

##-- Fully-Hadronic 
# fggRunJobs.py --load Taggers/test/HHWWgg/FH_SM2017.json -D -P -n 500 -d FH_SM2017 --stage-to=/eos/user/a/atishelm/ntuples/HHWWgg_flashgg/TaggerOutput/FH_SM2017/ -x cmsRun Systematics/test/workspaceStd.py maxEvents=100000 -q longlunch --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json doHHWWggTag=1 HHWWggTagsOnly=1 doSystematics=0 dumpTrees=1 dumpWorkspace=0 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 copyInputMicroAOD=True

##-- Fully-Leptonic 
# fggRunJobs.py --load Taggers/test/HHWWgg/FL_SM2017.json -D -P -n 500 -d FL_SM2017 --stage-to=/eos/user/a/atishelm/ntuples/HHWWgg_flashgg/TaggerOutput/FL_SM2017/ -x cmsRun Systematics/test/workspaceStd.py maxEvents=100000 -q longlunch --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json doHHWWggTag=1 HHWWggTagsOnly=1 doSystematics=0 dumpTrees=1 dumpWorkspace=0 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 copyInputMicroAOD=True

##-- ttH 
# fggRunJobs.py --load Taggers/test/HHWWgg/ttHJetToGG_2017.json -D -P -n 500 -d ttHJetToGG_2017 --stage-to=/eos/user/a/atishelm/ntuples/HHWWgg_flashgg/TaggerOutput/ttHJetToGG_2017/ -x cmsRun Systematics/test/workspaceStd.py maxEvents=100000 -q workday --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json doHHWWggTag=1 HHWWggTagsOnly=1 doSystematics=0 dumpTrees=1 dumpWorkspace=0 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 copyInputMicroAOD=1


# fggRunJobs.py --load Taggers/test/HHWWgg/SL_SM2017.json -D -P -n 500 -d SL_SM2017 --stage-to="/eos/user/a/atishelm/ntuples/HHWWgg_flashgg/TaggerOutput/SL_SM2017/" -x cmsRun Systematics/test/workspaceStd.py maxEvents=100000 -q longlunch --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json doHHWWggTag=1 HHWWggTagsOnly=1 doSystematics=0 dumpTrees=1 dumpWorkspace=0 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 copyInputMicroAOD=1
# fggRunJobs.py --load Taggers/test/HHWWgg_NLO/SL_SM2017.json -D -P -n 500 -d HHWWgg-SM-NLO-SF-Checks --stage-to="/eos/user/a/atishelm/ntuples/HHWWgg_flashgg/HHWWgg-SM-NLO-SF-Checks/" -x cmsRun Systematics/test/workspaceStd.py maxEvents=100000 -q longlunch --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1-HHWWgg.json doHHWWggTag=1 HHWWggTagsOnly=1 doSystematics=0 dumpTrees=1 dumpWorkspace=0 doHHWWggTagCutFlow=1 saveHHWWggFinalStateVars=1 copyInputMicroAOD=1

############################################################################################################################################################################################
# Abraham Tishelman-Charny 
# 23 October 2020
#
# The purpose of this module is to submit flashgg jobs to HTCondor.
#
# Example Usage:
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg/TaggerOutput/ --label HHWWgg-SL-SM-NLO-2016 --nEvents -1 --HHWWgg --dumpWorkspace --jsonPath Taggers/test/HHWWgg_NLO/SL_SM2016.json --year 2016 --condorQueue microcentury
#
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label HHWWgg-SL-SM-NLO-2016 --nEvents 20000 --HHWWgg --dumpTrees --jsonPath Taggers/test/HHWWgg_NLO/SL_SM2016.json  --year 2016 --condorQueue microcentury
#
# SM SL NLO 2018 example
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label HHWWgg-SL-SM-NLO-LowEvents-Trees --nEvents 1000 --saveHHWWggFinalStateVars --HHWWgg --dumpTrees --jsonPath Taggers/test/Examples/HHWWgg-SL-SM-NLO/SL_SM2018.json 
# 
# Signal
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label HHWWgg-SL-SM-NLO-2018 --nEvents -1 --HHWWgg --dumpWorkspace --jsonPath Taggers/test/Examples/HHWWgg-SL-SM-NLO/SL_SM2018.json
#
# Data
# python MetaData/scripts/SubmitJobs.py --fggDirec /afs/cern.ch/work/a/atishelm/private/flashgg-10_6_8/CMSSW_10_6_8/src/flashgg --outDirec /eos/user/a/atishelm/ntuples/HHWWgg_flashgg --label 2018-DoubleEG-Data-LowEvents --nEvents 1000 --HHWWgg --dumpTrees --jsonPath Taggers/test/Examples/HHWWgg-SL-SM-NLO/HHWWgg_Data_All_2018.json
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
