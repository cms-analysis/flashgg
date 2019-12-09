#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

# mkdir -p /eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_signal_IA_16/
# fggRunJobs.py --load sig_jobs_2016.json -d ${outDir}_signal_IA_16 -x cmsRun fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False copyInputMicroAOD=True --nCondorCpu=2 puTarget=`cat lumi/PU2016.txt`  maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_signal_IA_16/ -q tomorrow -n 50 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_signal_IA_17/
fggRunJobs.py --load sig_jobs_2017.json -d ${outDir}_signal_IA_17 -x cmsRun fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False copyInputMicroAOD=True --nCondorCpu=2 puTarget=`cat lumi/PU2017.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_signal_IA_17/ -q tomorrow -n 50 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_signal_IA_18/
fggRunJobs.py --load sig_jobs_2018.json -d ${outDir}_signal_IA_18 -x cmsRun fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=True doBJetsAndMET=False copyInputMicroAOD=True --nCondorCpu=2 puTarget=`cat lumi/PU2018.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_signal_IA_18/ -q tomorrow -n 50 -H --no-copy-proxy &

wait
