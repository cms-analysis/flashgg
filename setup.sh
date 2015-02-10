#!/bin/bash

SETUP_REMOTES=false

echo "Welcome to the FLASHgg automagic setup script!"

if [ ! -f $CMSSW_BASE/src/.git/HEAD ];
then
  echo "CMSSW area appears not to be set up correctly. Check README carefully."
  return 1
fi

NFILES=`ls -1 ${CMSSW_BASE}/src | wc -l`
if [ ! ${NFILES} = "1" ]
then
  echo "CMSSW area appears to have extra files already. Start over and README carefully."
  echo "You can remote this condition from the setup script if you wish, but proceed with caution!"
  return 1
fi

if ${SETUP_REMOTES} ; then
  echo "Setting up remotes listed in setup script..."
  cd $CMSSW_BASE/src/flashgg
  git remote add upstream https://github.com/cms-analysis/flashgg
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
  echo "Not setting up remote names (default)"
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
echo "Done with setup script! You still need to build!"
echo