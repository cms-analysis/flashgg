#!/bin/bash

storageDir="/eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_29052022"
outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p ${storageDir}/${outDir}_background_16/
fggRunJobs.py --load bkg_gjets_jobs_2016.json -d ${outDir}_background_16 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=False dumpTrees=True doSystematics=False applyNNLOPSweight=False doPdfWeights=False=False recalculatePDFWeights=False vbfTagsOnly=True dumpLHE=False melaEFT=False --nCondorCpu=2 --no-use-tarball pujidWP=tight maxEvents=-1 --stage-to=${storageDir}/${outDir}_background_16/ -q workday -n 100 -H --no-copy-proxy &

mkdir -p ${storageDir}/${outDir}_background_17/
fggRunJobs.py --load bkg_jobs_2017.json -d ${outDir}_background_17 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=False dumpTrees=True doSystematics=False applyNNLOPSweight=False doPdfWeights=False recalculatePDFWeights=False vbfTagsOnly=True dumpLHE=False melaEFT=False  --nCondorCpu=2 --no-use-tarball pujidWP=tight maxEvents=-1 --stage-to=${storageDir}/${outDir}_background_17/ -q workday -n 100 -H --no-copy-proxy &

mkdir -p ${storageDir}/${outDir}_background_18/
<<<<<<< HEAD
fggRunJobs.py --load bkg_jobs_2018.json -d ${outDir}_background_18 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=False dumpTrees=True doSystematics=False applyNNLOPSweight=False doPdfWeights=False recalculatePDFWeights=False vbfTagsOnly=True dumpLHE=False melaEFT=False  --nCondorCpu=2 --no-use-tarball pujidWP=tight maxEvents=-1 --stage-to=${storageDir}/${outDir}_background_18/ -q workday -n 100 -H --no-copy-proxy &
=======
fggRunJobs.py --load bkg_jobs_2018.json -d ${outDir}_background_18 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=False dumpTrees=True doSystematics=False applyNNLOPSweight=False recalculatePDFWeights=False vbfTagsOnly=True dumpLHE=False melaEFT=False  --nCondorCpu=2 --no-use-tarball pujidWP=tight maxEvents=-1 --stage-to=${storageDir}/${outDir}_background_18/ -q workday -n 100 -H --no-copy-proxy &
>>>>>>> origin/vbf-mela


