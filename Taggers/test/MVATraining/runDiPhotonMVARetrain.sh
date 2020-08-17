#!/usr/bin/bash/screen


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
echo -e "######     RETRAINING DiPhoton MVA         ######"
echo -e "##################################################"
echo
echo -e "+++++++++++ Generate the Training Trees ++++++++++"
echo -e "+++++++++++ with := ${NEvent}     ++++++++++++++++"
echo 

# run the script
fggRunJobs.py --load DiPhotonMVATraining_samples.json -d \
	${WORKSPACE}/test_dipho_training -x \
	cmsRun DiPhotonMVA_Training.py maxEvents=${NEvent}

echo 
echo -e "+++++++++++ Run the training macro ++++++++++++++"
echo
# run the training on background and without diplays

mkdir -p plots
root -l -q DiPhotonMVA_Training.cc++\(\"${NEvent}\",\"DiPhoton\"\)

echo 
echo -e "+++++++++++ Applying the MVA +++++++++++++++++"
echo

#produce the trained tree and histograms  
fggRunJobs.py --load DiPhotonMVATraining_samples.json -d \
    ${WORKSPACE}/test_dipho_compare/ -x \
    cmsRun DiPhotonMVA_Compare.py maxEvents=${NEvent}


echo 
echo -e "+++++++++++ Generating ROC curves ++++++++++++++++"
echo

root -l -q makeROCsDiPho.cc++\(\"${NEvent}\",\"DiPhoton\"\)

echo
echo -e "##################################################"
echo -e "######          RETRAINING COMPLETE         ######"
echo -e "##################################################"
echo
echo -e " You can retrieve your ROC curve for this training from plots/VBF_ROCs.pdf "
