queue="8nh"
LM=/afs/cern.ch/work/s/sethzenz/fromscratch130/CMSSW_8_0_25/src/flashgg/MetaData/work/jsons/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt
version="817"
fggRunJobs.py --load data_jobs.json -d data_jobs_${version} -x cmsRun workspaceStd.py maxEvents=-1 -n 100 -q ${queue} -D -P useAAA=0 doFiducial=False tthTagsOnly=False lumiMask=${LM}
