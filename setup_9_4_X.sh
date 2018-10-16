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
git config merge.renamelimit 2000

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

# Still needed, 8_0_28
# Temporarily removed, 9_2_0
#echo "EGM Pho ID recipe, Summer16"
#git cms-merge-topic ikrav:egm_id_80X_v3_photons

# Tag updated for 8_0_28 and may require further investigation
# Temporarily removed, 9_2_0
#echo "grabbing MET topic updates..."
#git cms-merge-topic cms-met:METRecipe_8020_for80Xintegration -u

# Straightofrward update for 8_0_28
echo "Setting up QGL..."
echo "... and setting up weight stuff..."
git cms-addpkg RecoJets/JetProducers
git cms-addpkg CommonTools/UtilAlgos
# Straightforward update for 8_0_28
echo "Tweaking ConfigToolBase.py to avoid assuming soft link path..."
git cms-addpkg FWCore/GuiBrowsers #temp-by hand
git cms-merge-topic -u shervin86:for-flashgg-QGL-vertexIndex-9_4_6

# TnP tools removed for 8_0_28, so Validation does not compile
# To be investigated
echo "Setting up TnP tools for 94X..."
git clone -b CMSSW_9_4_X https://github.com/cms-analysis/EgammaAnalysis-TnPTreeProducer.git EgammaAnalysis/TnPTreeProducer

echo "Temporary perl hack for LeptonSelection (to keep repo compatibility with 80X), due to CMSSW function name change"
perl -p -i.bak -e 's/numberOfHits/numberOfAllHits/g' $CMSSW_BASE/src/flashgg/Taggers/src/LeptonSelection.cc

#MET discrepancy mitigation
git cms-merge-topic -u kpedro88:METFixEE2017_949


# Updated for 8_0_28, and compiles and runs, but NOT checked by experts
# Update built from sethzenz:for-flashgg-smearer-conv-weights-8_0_26 and shervin86:Hgg_Gain_v1
echo "Setting up EGM stuff..."
#git cms-merge-topic -u shervin86:for-flashgg-smearer-conv-9_4_5
#git apply  flashgg/EnergyScaleCorrection.patch

#EGM IDs
#git cms-merge-topic lsoffi:CMSSW_9_4_0_pre3_TnP    
#git cms-merge-topic guitargeek:ElectronID_MVA2017_940pre3

git cms-merge-topic -u cms-egamma:EgammaPostRecoTools_940 #just adds in an extra file to have a setup function to make things easier
git cms-merge-topic -u sethzenz:Egamma80XMiniAODV2_949 #adds the c++ changes necessary to enable 2016 scale & smearing corrections
git cms-merge-topic -u shervin86:Egamma80XMiniAODV2_946_fix


echo "copy databases for local running (consistency with crab)"
cp $CMSSW_BASE/src/flashgg/Systematics/data/JEC/Fall17_17Nov2017*db $CMSSW_BASE/src/flashgg/
cp $CMSSW_BASE/src/flashgg/MicroAOD/data/QGL_cmssw8020_v2.db $CMSSW_BASE/src/flashgg

echo "copy smearing files stored in flashgg into egamma tools"
#cp $CMSSW_BASE/src/flashgg/Systematics/data/Golden*.dat $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data
#cp $CMSSW_BASE/src/flashgg/Systematics/data/80X_DCS05July_plus_Golden22_s*.dat $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data
#cp $CMSSW_BASE/src/flashgg/Systematics/data/80X_ichepV1_2016_pho_s* $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data
#cp $CMSSW_BASE/src/flashgg/Systematics/data/80X_ichepV2_2016_pho_s* $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data
#cp $CMSSW_BASE/src/flashgg/Systematics/data/Winter_2016_reReco_v1_ele_scales.dat $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data
#cp $CMSSW_BASE/src/flashgg/Systematics/data/Winter_2016_reReco_v1_ele_smearings.dat $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data
#cp $CMSSW_BASE/src/flashgg/Systematics/data/Moriond17_74x_pho_scales.dat $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data
#cp $CMSSW_BASE/src/flashgg/Systematics/data/Moriond17_74x_pho_smearings.dat $CMSSW_BASE/src/EgammaAnalysis/ElectronTools/data

echo "Simplified template cross section tools"
git cms-merge-topic -u sethzenz:for-flashgg-rivet-9_4_2

echo "linking classdef for release 94X"
ln -s $CMSSW_BASE/src/flashgg/DataFormats/src/classes_def_94X.xml $CMSSW_BASE/src/flashgg/DataFormats/src/classes_def.xml

echo "adding hook for indentation"
ln -s $CMSSW_BASE/src/flashgg/Validation/scripts/flashgg_indent_check.sh $CMSSW_BASE/src/flashgg/.git/hooks/pre-commit

echo
echo "Done with setup script! You still need to build!"
echo "After building, run afterbuild_9_4_X.sh"
echo
