#!/usr/bin/bash

# Author: Louie Corpe, adapated from Y.Haddad
# Email : lcorpe@cern.ch


export WORKSPACE=$PWD
TAGGERS=${CMSSW_BASE}/flashgg/Taggers/test
NEvent=10000

if [ -n $1 ]
	then
		NEvent=$1
fi

echo
echo -e "##################################################"
echo -e "######    RETRAINING VBF DIPHO-DIJET MVA    ######"
echo -e "##################################################"
echo
echo -e "+++++++++++ Generate the Training Trees ++++++++++"
echo -e "+++++++++++ with := ${NEvent}     ++++++++++++++++"
echo 

# run the script
fggRunJobs.py --load VBFMVATrainingSamples.json -d \
	${WORKSPACE}/test_diphodijet_training -x \
	cmsRun VBFDiPhoDiJetMVA_Training.py maxEvents=${NEvent}

echo 
echo -e "+++++++++++ Run the training macro ++++++++++++++"
echo
# run the training on background and without diplays

mkdir -p plots
root -l -q VBFDiPhoDiJetMVA_Training.cc++\(\"${NEvent}\",\"VBFDiPhoDiJet\"\)

echo 
echo -e "+++++++++++ Applying the MVA +++++++++++++++++"
echo

#produce the trained tree and histograms  
fggRunJobs.py --load VBFMVATrainingSamples.json -d \
    ${WORKSPACE}/test_diphodijet_compare/ -x \
    cmsRun VBFDiPhoDiJetMVA_Compare.py maxEvents=${NEvent}


echo 
echo -e "+++++++++++ Generating ROC curves ++++++++++++++++"
echo

root -l -q makeROCs.cc++\(\"${NEvent}\",\"VBFDiPhoDiJet\"\)

echo
echo -e "##################################################"
echo -e "######          RETRAINING COMPLETE         ######"
echo -e "##################################################"
echo
echo -e " You can retrieve your ROC curve for this training from plots/VBFDiPhoDiJet_ROCs.pdf
