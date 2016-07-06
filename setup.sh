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
echo "You should have checked out from cms-analysis/flashgg. Renaming this to upstream for convenience of existing developers..."
cd $CMSSW_BASE/src/flashgg
git remote rename origin upstream
git remote set-url --push upstream DISALLOWED
GITHUBUSERNAME=`git config user.github`
echo "Setting up a new origin repo, assuming your fork name is ${GITHUBUSERNAME} - check this!"
git remote add origin git@github.com:${GITHUBUSERNAME}/flashgg.git
git config branch.master.remote origin
git config merge.renamelimit 2000

if ${SETUP_REMOTES} ; then
  echo "Setting up remotes listed in setup script..."
  cd $CMSSW_BASE/src/flashgg
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

# Removed because it requires new merging and it is not needed for default PFCHS
# Will be restored if/when required for future studies
#echo
#echo "Setting up pileupjetid..."
#git cms-addpkg RecoJets/JetProducers
#git cms-merge-topic sethzenz:topic-pujid-74X
#echo

#echo "Setting up weight counter..."
#git cms-addpkg CommonTools/UtilAlgos 
#git cms-addpkg DataFormats/Common
#git cms-merge-topic sethzenz:topic-weights-count-74X

# PUPPI is automagically in the release since 7_4_11 and 7_5_2, but we still need Multi-PUPPI
# Not needed for Moriond
#echo "Setting up PUPPI..."
#git cms-addpkg CommonTools/PileupAlgos
#git cms-merge-topic sethzenz:topic-puppi-7_4_12 
#echo

#echo "Setting up Conversion tools for pat electron..."
#git cms-addpkg RecoEgamma/EgammaTools
#git cms-merge-topic -u sethzenz:topic-conversion-tools-for-pat-ele-74X

echo "Setting up pileup jet id..."
git cms-addpkg RecoJets/JetProducers
#git cms-merge-topic -u sethzenz:topic-PileupJetId-NonStandardVtx
git cms-merge-topic -u sethzenz:topic-PileupJetId-NonStandardVtx-bugfixSync

echo "Setting up QGL..."
git cms-merge-topic -u sethzenz:topic-QGL-MiniAOD-vertexIndex

echo "Setting up MET filters..."
git cms-merge-topic -u mplaner:METfilters808

echo "Setting up TnP tools..."
#git cms-addpkg DataFormats/RecoCandidate
#git cms-addpkg PhysiscsTools/TagAndProbe
#git cms-merge-topic -u matteosan1:egm_tnp_76X
git cms-merge-topic -u mplaner:egm_tnp_80x
git cms-merge-topic -u matteosan1:smearer_76X
#git remote add cmssw-ferriff https://github.com/ferriff/cmssw.git
#git fetch cmssw-ferriff
#git cherry-pick 5a028c0bd8fe8ea932ee602f2e82c455489f4ad1
#git cherry-pick a2dc7fa601a13b43e79c118ba8cb746f8dd684ce
#git cherry-pick 415903611957b5bd9f0b3f0c657cb903437db4f8

echo "Setting up weight and pat electron conversion..."
git cms-addpkg CommonTools/UtilAlgos
git cms-addpkg RecoEgamma/EgammaTools
git remote add cmssw-sethzenz https://github.com/sethzenz/cmssw.git
git fetch cmssw-sethzenz
git cherry-pick 5163a7c9937ebfbbd714b3d161af01f64b65224c
git cherry-pick a45d253ea9850acecbcfcd7bd2e5c3f00d8f0bd9
git remote rm cmssw-sethzenz

if [[ $CMSSW_BASE == *"7_6"* ]]
then
    echo "Setting up PDF weight tool..."
    git cms-addpkg PhysicsTools/HepMCCandAlgos
    git cherry-pick ca5f8100a28c597ad118e3a4b3dcadda7f6e45ca
else
    echo "Not setting up PDF weight because we appear to be in CMSSW 8"
fi

echo "copy databases for local running (consistency with crab)"
cp $CMSSW_BASE/src/flashgg/MicroAOD/data/Fall15_25nsV2_*.db $CMSSW_BASE/src/flashgg
cp $CMSSW_BASE/src/flashgg/MicroAOD/data/Spring16_25nsV3*.db $CMSSW_BASE/src/flashgg
cp $CMSSW_BASE/src/flashgg/MicroAOD/data/Spring16_25nsV6*.db $CMSSW_BASE/src/flashgg
cp $CMSSW_BASE/src/flashgg/MicroAOD/data/QGL_80X.db $CMSSW_BASE/src/flashgg

echo "copy smearing files tored in flashgg into egamma tools"
cp $CMSSW_BASE/src/flashgg/Systematics/data/Golden*.dat $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data

echo "adding hook for indentation"
ln -s $CMSSW_BASE/src/flashgg/Validation/scripts/flashgg_indent_check.sh $CMSSW_BASE/src/flashgg/.git/hooks/pre-commit

echo
echo "Done with setup script! You still need to build!"
echo
