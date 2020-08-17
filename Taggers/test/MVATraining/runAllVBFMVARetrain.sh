#!/usr/bin/bash

# Author: Louie Corpe, adapated from Y.Haddad
# Email : lcorpe@cern.ch


export WORKSPACE=$PWD
TAGGERS=${CMSSW_BASE}/flashgg/Taggers/test/MVATraining
NEvent=10000

if [ -n $1 ]
	then
		NEvent=$1
fi

echo
echo -e "##################################################"
echo -e "######    RETRAINING MVAs:                  ######"
echo -e "######        - VBF DIJET MVA               ######"
echo -e "######        - VBF DIPHO-DIJET MVA         ######"
echo -e "##################################################"
echo
echo -e "+++ Generate the Training Trees for DiJet MVA ++++"
echo -e "+++++++++++ with := ${NEvent}     ++++++++++++++++"
echo 

# run the script
fggRunJobs.py --load VBFMVATrainingSamples.json -d \
	${WORKSPACE}/test_diphodijet_training -x \
	cmsRun VBFDiPhoDiJetMVA_Training.py maxEvents=${NEvent}

echo 
echo -e "+++++++ Run the Dijet MVA training macro ++++++++++"
echo
# run the training on background and without diplays

root -l -q VBFDiPhoDiJetMVA_Training.cc++\(\"${NEvent}\",\"VBF\"\)

echo 
echo -e "+++ Generate Training Trees for DiPho-DiJet MVA ++++"
echo -e "+++++++++++ Using new Dijet MVA weights ++++++++++++"
echo

fggRunJobs.py --load VBFMVATrainingSamples.json -d \
	${WORKSPACE}/test_diphodijet_training -x \
	cmsRun VBFDiPhoDiJetMVA_Training.py maxEvents=${NEvent}

echo 
echo -e "+++++++ Run the Dijet MVA training macro ++++++++++"
echo
# run the training on background and without diplays

root -l -q VBFDiPhoDiJetMVA_Training.cc++\(\"${NEvent}\",\"VBFDiPhoDiJet\"\)

echo 
echo -e "+++++++++++ Generate Comparison trees +++++++++++++"
echo

#produce the trained tree and histograms  
fggRunJobs.py --load VBFMVATrainingSamples.json -d \
    ${WORKSPACE}/test_diphodijet_compare/ -x \
    cmsRun VBFDiPhoDiJetMVA_Compare.py maxEvents=${NEvent}


echo 
echo -e "+++++++++++ Generating ROC curves ++++++++++++++++"
echo
mkdir -p plots
root -l -q makeROCs.cc++\(\"${NEvent}\",\"VBF\"\)
root -l -q makeROCs.cc++\(\"${NEvent}\",\"VBFDiPhoDiJet\"\)

echo
echo -e "##################################################"
echo -e "######          RETRAINING COMPLETE         ######"
echo -e "##################################################"
echo
echo -e " You can retrieve your ROC curve for this training from:"
echo -e " plots/VBF_ROCs.pdf"
echo -e " plots/VBFDiPhoDiJet_ROCs.pdf"
