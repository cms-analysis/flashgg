#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_diphoton_16/
fggRunJobs.py --load diphoton_jobs_2016.json -d ${outDir}_diphoton_16 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=False doBJetsAndMET=False filterNegR9=True puTarget=`cat lumi/PU2016.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2016ReReco/${outDir}_diphoton_16/ -q workday -n 200 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_diphoton_17/
fggRunJobs.py --load diphoton_jobs_2017.json -d ${outDir}_diphoton_17 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=False doBJetsAndMET=False filterNegR9=True puTarget=`cat lumi/PU2017.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2017ReReco/${outDir}_diphoton_17/ -q workday -n 200 -H --no-copy-proxy &

mkdir -p /eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_diphoton_18/
fggRunJobs.py --load diphoton_jobs_2018.json -d ${outDir}_diphoton_18 -x cmsRun ../fiducialStd.py metaDataSrc=flashgg useAAA=True dumpWorkspace=False dumpTrees=True doSystematics=False doJets=False doBJetsAndMET=False filterNegR9=True puTarget=`cat lumi/PU2018.txt` maxEvents=-1 --stage-to=/eos/user/t/threiten/Analysis/Differentials/2018ReABCPromptDReco/${outDir}_diphoton_18/ -q workday -n 200 -H --no-copy-proxy &

wait
