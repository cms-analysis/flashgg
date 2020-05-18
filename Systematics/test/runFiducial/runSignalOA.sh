#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_signal_OA_16/
fggRunJobs.py --load sig_jobs_OA_2016.json -d ${outDir}_signal_OA_16 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True puTarget=`cat lumi/PU2016.txt`  maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_signal_OA_16/ -q tomorrow -n 100 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_signal_OA_17/
fggRunJobs.py --load sig_jobs_OA_2017.json -d ${outDir}_signal_OA_17 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True puTarget=`cat lumi/PU2017.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_signal_OA_17/ -q tomorrow -n 100 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_signal_OA_18/
fggRunJobs.py --load sig_jobs_OA_2018.json -d ${outDir}_signal_OA_18 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False --nCondorCpu=2 --make-light-tarball copyInputMicroAOD=True puTarget=`cat lumi/PU2018.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_signal_OA_18/ -q workday -n 100 -H --no-copy-proxy &

wait
