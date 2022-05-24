#!/bin/bash

SETUP_REMOTES=false

echo
echo "Welcome to the FLASHgg automagic setup script!"

if [ ! -f $CMSSW_BASE/src/.git/HEAD ];
then
  echo "CMSSW area appears not to be set up correctly. Check README carefully."
  echo
  return 1
fi

NFILES=`ls -1 ${CMSSW_BASE}/src | wc -l`
if [ ! ${NFILES} = "1" ]
then
  echo "CMSSW area appears to have extra files already. Start over and check README carefully."
  echo "You can remove this condition from the setup script if you wish, but proceed with caution!"
  echo
#  return 1
fi

echo
echo "You should have checked out from cms-analysis/flashgg. Renaming this to upstream for convenience of existing developers..."
cd $CMSSW_BASE/src/flashgg
git remote rename origin upstream
git remote set-url --push upstream DISALLOWED
GITHUBUSERNAME=`git config user.github`
echo "Setting up a new origin repo, assuming your fork name is ${GITHUBUSERNAME} - check this!"
git remote add origin git@github.com:${GITHUBUSERNAME}/flashgg.git
git config branch.master.remote origin
git config merge.renamelimit 2500

if ${SETUP_REMOTES} ; then
  echo "Setting up remotes listed in setup script..."
  cd $CMSSW_BASE/src/flashgg
  git remote add upstream-writable git@github.com:cms-analysis/flashgg.git
else
  echo "Not setting up additional remote names (default)"
fi

cd $CMSSW_BASE/src

# QGL
echo "QGTagger for flashgg..."
git cms-merge-topic -u simonepigazzini:topic_flashgg_10_5_0_qgtagger
cp $CMSSW_BASE/src/flashgg/MicroAOD/data/QGL_AK4chs_94X.db $CMSSW_BASE/src/flashgg/

# EGamma post reco tools for energy scales and smearings
echo "Settinga up Scales and Smearings form EGM"
git cms-addpkg RecoEgamma/EgammaTools
### Switch to my fork until the old format UL17 S+S files are uploaded in the main repo
git clone https://github.com/rchatter/EgammaAnalysis-ElectronTools.git EgammaAnalysis/ElectronTools/data
cd EgammaAnalysis/ElectronTools/data
cd $CMSSW_BASE/src
git apply flashgg/EnergyScaleCorrection.patch

# TnP tools removed for 8_0_28, so Validation does not compile
# To be investigated
#echo "Setting up TnP tools for 10_1_X..."
#git clone -b CMSSW_9_4_X https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer.git EgammaAnalysis/TnPTreeProducer
#git clone -b v2018.05.11_10_1_X_prelim https://github.com/lsoffi/EgammaAnalysis-TnPTreeProducer EgammaAnalysis/TnPTreeProducer

echo "adding hook for indentation"
ln -s $CMSSW_BASE/src/flashgg/Validation/scripts/flashgg_indent_check.sh $CMSSW_BASE/src/flashgg/.git/hooks/pre-commit

if [ -d "$CMSSW_BASE/src/PhysicsTools/PythonAnalysis" ]; then
  rm -r $CMSSW_BASE/src/PhysicsTools/PythonAnalysis
fi

echo "setting up XGBoost interface"
cd $CMSSW_BASE/src
git clone https://github.com/simonepigazzini/XGBoostCMSSW.git
cp XGBoostCMSSW/XGBoostInterface/toolbox/*xml $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/
scram setup rabit
scram setup xgboost

# Patch IOPool to avoid Run and Lumi trees being dropped when loading a parent dataset
git cms-addpkg IOPool/Input
git apply flashgg/LoadRunAndLumis.patch

# HTCondor python API
pip install --user htcondor

echo
echo "Done with setup script! You still need to build!"
echo
