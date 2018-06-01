queue="8nh"
LM=${CMSSW_BASE}/src/flashgg/MetaData/work/jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt
version="916"
fggRunJobs.py --load DoubleEG_ReMiniAOD_2_5_Y.json -d data_jobs_${version} -x cmsRun workspaceStd.py maxEvents=-1 -n 100 -q ${queue} -D -P useAAA=0 doFiducial=False tthTagsOnly=False lumiMask=${LM}
