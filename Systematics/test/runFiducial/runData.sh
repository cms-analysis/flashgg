#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_data_16/
fggRunJobs.py --load data_jobs_2016.json -d ${outDir}_data_16 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=True --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True acceptance=NONE maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_data_16/ -q workday -n 500 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_data_17/
fggRunJobs.py --load data_jobs_2017.json -d ${outDir}_data_17 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=True --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True acceptance=NONE maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_data_17/ -q workday -n 500 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_data_18/
fggRunJobs.py --load data_jobs_2018.json -d ${outDir}_data_18 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=True --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True acceptance=NONE maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_data_18/ -q workday -n 500 -H --no-copy-proxy &

wait
