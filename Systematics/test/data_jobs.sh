# NB this command is specific to the configuration at IC and is not gaurenteed elsewhere
#LM=/afs/cern.ch/work/s/sethzenz/fromscratch107/CMSSW_8_0_8_patch1/src/flashgg/Systematics/test/Cert_271036-275783_13TeV_PromptReco_Collisions16_JSON.txt #6.26/fb
#LM=/afs/cern.ch/work/s/sethzenz/fromscratch107/CMSSW_8_0_8_patch1/src/flashgg/MetaData/work/jsons/Cert_271036-276384_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt #9.17/fb
queue="8nh"
LM=/afs/cern.ch/work/s/sethzenz/fromscratch107/CMSSW_8_0_8_patch1/src/flashgg/MetaData/work/jsons/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt
version="745"
fggRunJobs.py --load data_jobs.json -d data_jobs_${version} -x cmsRun workspaceStd.py maxEvents=-1 -n 100 -q ${queue} -D -P useAAA=0 atIC=1 doFiducial=False lumiMask=${LM}