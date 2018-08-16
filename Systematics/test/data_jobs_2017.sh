queue="8nh"
version="1058"
fggRunJobs.py --load data_jobs_2017.json -d data_jobs_${version} -x cmsRun workspaceStd.py maxEvents=-1 -n 200 -q ${queue} -D -P useAAA=0 doFiducial=False tthTagsOnly=1 lumiMask=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt