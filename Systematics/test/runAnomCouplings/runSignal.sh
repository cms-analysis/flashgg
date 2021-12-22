#!/bin/bash

outDir=$1
if [[ -z $outDir ]]; then
    echo "usage: $0 <outDir>"
    exit -1
fi

mkdir -p /eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_161221/2016ReReco/${outDir}_signal_IA_16/
fggRunJobs.py --load sig_jobs_2016.json -d ${outDir}_signal_IA_16 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=False dumpTrees=True doSystematics=True applyNNLOPSweight=False recalculatePDFWeights=True doPdfWeights=True vbfTagsOnly=True dumpLHE=True melaEFT=True --nCondorCpu=2 pujidWP=tight maxEvents=-1 --stage-to=/eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_161221/2016ReReco/${outDir}_signal_IA_16/ -q workday -n 100 -H --no-copy-proxy --no-use-tarball &

mkdir -p /eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_161221/2017ULReReco/${outDir}_signal_IA_17/
fggRunJobs.py --load sig_jobs_2017.json -d ${outDir}_signal_IA_17 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=False dumpTrees=True doSystematics=True applyNNLOPSweight=False recalculatePDFWeights=True doPdfWeights=True vbfTagsOnly=True dumpLHE=True melaEFT=True  --nCondorCpu=2 pujidWP=tight maxEvents=-1 --stage-to=/eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_161221/2017ULReReco/${outDir}_signal_IA_17/ -q workday -n 100 -H --no-copy-proxy --no-use-tarball &

mkdir -p /eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_161221/2018ULReReco/${outDir}_signal_IA_18/
fggRunJobs.py --load sig_jobs_2018.json -d ${outDir}_signal_IA_18 -x cmsRun ../workspaceVbf.py metaDataSrc=flashgg dumpWorkspace=False dumpTrees=True doSystematics=True applyNNLOPSweight=False recalculatePDFWeights=True doPdfWeights=True vbfTagsOnly=True dumpLHE=True melaEFT=True  --nCondorCpu=2 pujidWP=tight maxEvents=-1 --stage-to=/eos/cms/store/group/dpg_ecal/comm_ecal/localreco/vbfhgg/HiggsCouplings/Trees_161221/2018ULReReco/${outDir}_signal_IA_18/ -q workday -n 100 -H --no-copy-proxy --no-use-tarball &

wait
