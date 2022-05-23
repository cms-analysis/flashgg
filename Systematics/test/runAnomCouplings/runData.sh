#!/bin/bash

storageDir="/eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_21052022"
outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p ${storageDir}/${outDir}_data_16/
fggRunJobs.py --load data_jobs_2016.json -d ${outDir}_data_16 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=True dumpTrees=True doSystematics=False applyNNLOPSweight=False recalculatePDFWeights=False vbfTagsOnly=True dumpLHE=False melaEFT=False --nCondorCpu=2 --no-use-tarball pujidWP=tight maxEvents=-1 --stage-to=${storageDir}/${outDir}_data_16/ -q workday -n 500 -H --no-copy-proxy &

mkdir -p ${storageDir}/${outDir}_data_17/
fggRunJobs.py --load data_jobs_2017.json -d ${outDir}_data_17 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=True dumpTrees=True doSystematics=False applyNNLOPSweight=False recalculatePDFWeights=False vbfTagsOnly=True dumpLHE=False melaEFT=False  --nCondorCpu=2 --no-use-tarball pujidWP=tight maxEvents=-1 --stage-to=${storageDir}/${outDir}_data_17/ -q workday -n 500 -H --no-copy-proxy &

mkdir -p ${storageDir}/${outDir}_data_18/
fggRunJobs.py --load data_jobs_2018.json -d ${outDir}_data_18 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=True dumpTrees=True doSystematics=False applyNNLOPSweight=False recalculatePDFWeights=False vbfTagsOnly=True dumpLHE=False melaEFT=False  --nCondorCpu=2 --no-use-tarball pujidWP=tight maxEvents=-1 --stage-to=${storageDir}/${outDir}_data_18/ -q workday -n 500 -H --no-copy-proxy &

wait
