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
  git remote add dmajumder https://github.com/dmajumder/flashgg
  git remote add abeschi https://github.com/abeschi/flashgg
  git remote add andreh7 https://github.com/andreh7/flashgg
  git remote add andreypz https://github.com/andreypz/flashgg
  git remote add ArnabPurohit https://github.com/ArnabPurohit/flashgg
  git remote add bcourbon https://github.com/bcourbon/flashgg
  git remote add bmarzocc https://github.com/bmarzocc/flashgg
  git remote add camendola https://github.com/camendola/flashgg
  git remote add camilocarrillo https://github.com/camilocarrillo/flashgg
  git remote add cathatino https://github.com/cathatino/flashgg
  git remote add cms-flashgg https://github.com/cms-flashgg/flashgg
  git remote add CMSttHggAnalysis https://github.com/CMSttHggAnalysis/flashgg
  git remote add crovelli https://github.com/crovelli/flashgg
  git remote add edjtscott https://github.com/edjtscott/flashgg
  git remote add fabriciojm https://github.com/fabriciojm/flashgg
  git remote add famargar https://github.com/famargar/flashgg
  git remote add favaro https://github.com/favaro/flashgg
  git remote add fcouderc https://github.com/fcouderc/flashgg
  git remote add fedmante https://github.com/fedmante/flashgg
  git remote add ferriff https://github.com/ferriff/flashgg
  git remote add ffede https://github.com/ffede/flashgg
  git remote add forthommel https://github.com/forthommel/flashgg
  git remote add fravera https://github.com/fravera/flashgg
  git remote add gkrintir https://github.com/fravera/flashgg-1
  git remote add GiuseppeFasanella https://github.com/GiuseppeFasanella/flashgg
  git remote add gourangakole https://github.com/gourangakole/flashgg
  git remote add hbakhshi https://github.com/hbakhshi/flashgg
  git remote add HZgamma https://github.com/HZgamma/flashgg
  git remote add InnaKucher https://github.com/InnaKucher/flashgg
  git remote add ishvetso https://github.com/ishvetso/flashgg
  git remote add itopsisg https://github.com/itopsisg/flashgg
  git remote add J-C-Wright https://github.com/J-C-Wright/flashgg
  git remote add JunquanTao https://github.com/JunquanTao/flashgg
  git remote add khoumani https://github.com/khoumani/flashgg
  git remote add kmcdermo https://github.com/kmcdermo/flashgg
  git remote add kmondal https://github.com/kmondal/flashgg
  git remote add ldcorpe https://github.com/ldcorpe/flashgg
  git remote add lgray https://github.com/lgray/flashgg
  git remote add lsoffi https://github.com/lsoffi/flashgg
  git remote add malcles https://github.com/malcles/flashgg
  git remote add martinamalberti https://github.com/martinamalberti/flashgg
  git remote add matteosan1 https://github.com/matteosan1/flashgg
  git remote add mdonega https://github.com/mdonega/flashgg
  git remote add mez34 https://github.com/mez34/flashgg
  git remote add mmachet https://github.com/mmachet/flashgg
  git remote add molmedon https://github.com/molmedon/flashgg
  git remote add mplaner https://github.com/mplaner/flashgg
  git remote add musella https://github.com/musella/flashgg
  git remote add nancymarinelli https://github.com/nancymarinelli/flashgg
  git remote add OlivierBondu https://github.com/OlivierBondu/flashgg
  git remote add pmeridian https://github.com/pmeridian/flashgg
  git remote add gfasanel https://github.com/gfasanel/flashgg
  git remote add quittnat https://github.com/quittnat/flashgg
  git remote add rateixei https://github.com/rateixei/flashgg
  git remote add rdangovs https://github.com/rdangovs/flashgg
  git remote add ResonantHbbHgg https://github.com/ResonantHbbHgg/flashgg
  git remote add saghosh https://github.com/saghosh/flashgg
  git remote add sethzenz https://github.com/sethzenz/flashgg
  git remote add simonepigazzini https://github.com/simonepigazzini/flashgg
  git remote add vciriolo https://github.com/vciriolo/flashgg
  git remote add swagata87 https://github.com/swagata87/flashgg
  git remote add tklijnsma https://github.com/tklijnsma/flashgg
  git remote add vtavolar https://github.com/vtavolar/flashgg
  git remote add yhaddad https://github.com/yhaddad/flashgg
  git remote add upstream-writable git@github.com:cms-analysis/flashgg.git
else
  echo "Not setting up additional remote names (default)"
fi

cd $CMSSW_BASE/src

echo "QGTagger for flashgg..."
git cms-merge-topic -u simonepigazzini:topic_flashgg_10_5_0_qgtagger

# EGamma post reco tools for energy scales and smearings
echo "Settinga up Scales and Smearings form EGM"
git cms-merge-topic cms-egamma:EgammaPostRecoTools
git clone https://github.com/cms-egamma/EgammaAnalysis-ElectronTools.git 
cd EgammaAnalysis/ElectronTools/data
git checkout ScalesSmearing2018_Dev
cd $CMSSW_BASE/src
git cms-merge-topic cms-egamma:EgammaPostRecoTools_dev
git apply flashgg/EnergyScaleCorrection.patch

# TnP tools removed for 8_0_28, so Validation does not compile
# To be investigated
echo "Setting up TnP tools for 10_1_X..."
#git clone -b CMSSW_9_4_X https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer.git EgammaAnalysis/TnPTreeProducer
git clone -b v2018.05.11_10_1_X_prelim https://github.com/lsoffi/EgammaAnalysis-TnPTreeProducer EgammaAnalysis/TnPTreeProducer

#MET discrepancy mitigation
git cms-merge-topic cms-met:METFixEE2017_949_v2_backport_to_102X 

echo "copy smearing files stored in flashgg into egamma tools"
git clone https://github.com/ECALELFS/ScalesSmearings.git $CMSSW_BASE/src/EgammaAnalysis/ScalesSmearings/data

echo "adding hook for indentation"
ln -s $CMSSW_BASE/src/flashgg/Validation/scripts/flashgg_indent_check.sh $CMSSW_BASE/src/flashgg/.git/hooks/pre-commit

if [ -d "$CMSSW_BASE/src/PhysicsTools/PythonAnalysis" ]; then
  rm -r $CMSSW_BASE/src/PhysicsTools/PythonAnalysis
fi

echo "setting up XGBoost interface"
cd $CMSSW_BASE/src
git clone https://github.com/simonepigazzini/XGBoostCMSSW.git
cp XGBoostCMSSW/XGBoostInterface/toolbox/*xml $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/
scram setup rabit xgboost

echo
echo "Done with setup script! You still need to build!"
echo
