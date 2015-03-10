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
  return 1
fi

echo
echo "You should have checked out from flashgg. Renaming this to upstream for convenience of existing developers..."
cd $CMSSW_BASE/src/flashgg
git remote rename origin upstream
echo "Remember that:"
echo "  * To get a branch you can commit to, do: git checkout -b my-informative-branchname"
echo "  * YOU will need to add your own repo as a remote to push anything, e.g. by putting in yourusername here:"
echo "      cd ${CMSSW_BASE}/src/flashgg ; git remote add origin git@github.com:yourusername/flashgg.git"
echo

if ${SETUP_REMOTES} ; then
  echo "Setting up remotes listed in setup script..."
  cd $CMSSW_BASE/src/flashgg
  git remote add bmarzocc https://github.com/bmarzocc/flashgg
  git remote add camilocarrillo https://github.com/camilocarrillo/flashgg
  git remote add favaro https://github.com/favaro/flashgg
  git remote add fcouderc https://github.com/fcouderc/flashgg
  git remote add InnaKucher https://github.com/InnaKucher/flashgg
  git remote add ldcorpe https://github.com/ldcorpe/flashgg
  git remote add malcles https://github.com/malcles/flashgg
  git remote add matteosan1 https://github.com/matteosan1/flashgg
  git remote add mdonega https://github.com/mdonega/flashgg
  git remote add mmachet https://github.com/mmachet/flashgg
  git remote add molmedon https://github.com/molmedon/flashgg
  git remote add mplaner https://github.com/mplaner/flashgg
  git remote add musella https://github.com/musella/flashgg
  git remote add OlivierBondu https://github.com/OlivierBondu/flashgg
  git remote add pmeridian https://github.com/pmeridian/flashgg
  git remote add quittnat https://github.com/quittnat/flashgg
  git remote add sethzenz https://github.com/sethzenz/flashgg
  git remote add swagata87 https://github.com/swagata87/flashgg
  git remote add yhaddad https://github.com/yhaddad/flashgg
  git remote add upstream-writable git@github.com:cms-analysis/flashgg.git
else
  echo "Not setting up additional remote names (default)"
fi

cd $CMSSW_BASE/src

echo
echo "Setting up pileupjetid..."
git cms-addpkg RecoJets/JetProducers
git cms-merge-topic -u sethzenz:pileupjetid-for-flashgg-72x

echo
echo "Setting up PUPPI..."
git clone -b flashgg https://github.com/ldcorpe/Dummy

echo
echo "Setting up weight counter..."
git cms-addpkg CommonTools/UtilAlgos 
git cms-addpkg DataFormats/Common
git cms-merge-topic musella:topic-weights-count

echo
echo "Setting up Photon Tool bugfix..."
git cms-addpkg RecoEcal/EgammaCoreTools 
git cherry-pick fd6b6bd88dc74d8f73299f395cc075aa89c8d701 

echo
echo "Setting up Conversion tools for pat electron..."
git cms-addpkg RecoEgamma/EgammaTools
git cms-merge-topic -u sethzenz:topic-conversion-tools-for-pat-ele

echo
echo "Done with setup script! You still need to build!"
echo
