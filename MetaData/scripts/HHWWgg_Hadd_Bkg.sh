#!/bin/sh

# The purpose of this script is to hadd output files from HHWWgg condor jobs on data 

# Run from one directory outside directory with data output files 

fgg_direc=/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg
# ntuple_direc="/eos/user/a/atishelm/ntuples/HHWWgg"
ntuple_direc="/eos/user/b/bmarzocc/HHWWgg_Tagger/2017/"

cd ${ntuple_direc}

input=$1 # path of directory with condor output files 
output=$2 # path of directory to put hadded files into 

if [ -z "$input" ]
then
      echo "Please enter an input directory as the 1st argument"
      echo "exiting"
      cd $fgg_direc
      return
fi

if [ -z "$output" ]
then
      echo "Please enter an output directory as the 2nd argument"
      echo "exiting"
      cd $fgg_direc
      return
fi

mkdir -p $output
# hadded_combined=$output
# hadded_combined+="_Combined"

# Need to look through for background's in list 				

# Flashgg_bkg.json (2017)
# Bkgs=("QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8" "QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" 
#       "GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" "GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8" "GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" 
#       "DiPhotonJetsBox_M40_80-Sherpa" "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa" 
#       "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8" "THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5" "WW_TuneCP5_13TeV-pythia8" 
#       "GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8" "VBFHToGG_M-125_13TeV_powheg_pythia8" "VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8" 
#       "ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8" "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8" "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8" "TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8" )

# rerun jobs that had at least one corrupted file 
# Bkgs=("QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8")
# Bkgs=( "GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" "GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8" "GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" "WW_TuneCP5_13TeV-pythia8" )
# Bkgs=( "GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" "GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8" "GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" "WW_TuneCP5_13TeV-pythia8" )
# Bkgs=( "GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8" )

# HHWWgg_bkg_v2 
# Bkgs=( "TTJets_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8" "TTJets_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8"
# "TTJets_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8" "TTJets_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8" "TTJets_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8"
# "TTToHadronic_TuneCP5_13TeV-powheg-pythia8" )

# HHWWgg_bkg_v3 
# Bkgs=( "W1JetsToLNu_LHEWpT_50-150_TuneCP5_13TeV-amcnloFXFX-pythia8"
# "W1JetsToLNu_LHEWpT_400-inf_TuneCP5_13TeV-amcnloFXFX-pythia8"
# "W1JetsToLNu_LHEWpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8" 
# "W1JetsToLNu_LHEWpT_150-250_TuneCP5_13TeV-amcnloFXFX-pythia8" 
# "W1JetsToLNu_LHEWpT_100-150_TuneCP5_13TeV-amcnloFXFX-pythia8" 
# "W1JetsToLNu_LHEWpT_0-50_TuneCP5_13TeV-amcnloFXFX-pythia8" )

# HHWWgg_bkg_v4 
# Bkgs=( "W2JetsToLNu_LHEWpT_50-150_TuneCP5_13TeV-amcnloFXFX-pythia8" "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" "W2JetsToLNu_LHEWpT_0-50_TuneCP5_13TeV-amcnloFXFX-pythia8" "W2JetsToLNu_LHEWpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8"
      #   "W2JetsToLNu_LHEWpT_400-inf_TuneCP5_13TeV-amcnloFXFX-pythia8" "W2JetsToLNu_LHEWpT_100-150_TuneCP5_13TeV-amcnloFXFX-pythia8" "W2JetsToLNu_LHEWpT_150-250_TuneCP5_13TeV-amcnloFXFX-pythia8" )

# HHWWgg_bkg_v5 
# Bkgs=( "WGGJets_TuneCP5_13TeV_madgraphMLM_pythia8" "WGJJToLNuGJJ_EWK_aQGC-FS-FM_TuneCP5_13TeV-madgraph-pythia8"
# "WGJJToLNu_EWK_QCD_TuneCP5_13TeV-madgraph-pythia8"
# "WWTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8" "ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8" )

# Bkgs=( "TTJets_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8" "TTJets_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8" )

# Bkgs=( "W1JetsToLNu_LHEWpT_0-50_TuneCP5_13TeV-amcnloFXFX-pythia8" "W1JetsToLNu_LHEWpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8"
#  )

# Bkgs=( "W2JetsToLNu_LHEWpT_0-50_TuneCP5_13TeV-amcnloFXFX-pythia8" "W2JetsToLNu_LHEWpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8" )

##-- empty files:
# HHWWgg_bkg_v2/output_TTJets_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8_bmarzocc-HHWWgg_bkg_v2-94X_mc2017-RunIIFall18-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1-db49498e7dc78d32430682b35e9cae55_USER_57.root
# HHWWgg_bkg_v2/output_TTJets_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8_bmarzocc-HHWWgg_bkg_v2-94X_mc2017-RunIIFall18-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1-db49498e7dc78d32430682b35e9cae55_USER_34.root
# HHWWgg_bkg_v3/output_W1JetsToLNu_LHEWpT_0-50_TuneCP5_13TeV-amcnloFXFX-pythia8_bmarzocc-HHWWgg_bkg_v3-94X_mc2017-RunIIFall18-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1-db49498e7dc78d32430682b35e9cae55_USER_29.root
# HHWWgg_bkg_v3/output_W1JetsToLNu_LHEWpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8_bmarzocc-HHWWgg_bkg_v3-94X_mc2017-RunIIFall18-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1-db49498e7dc78d32430682b35e9cae55_USER_10.root
# HHWWgg_bkg_v4/output_W2JetsToLNu_LHEWpT_0-50_TuneCP5_13TeV-amcnloFXFX-pythia8_bmarzocc-HHWWgg_bkg_v4-94X_mc2017-RunIIFall18-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1-db49498e7dc78d32430682b35e9cae55_USER_31.root
# HHWWgg_bkg_v4/output_W2JetsToLNu_LHEWpT_250-400_TuneCP5_13TeV-amcnloFXFX-pythia8_bmarzocc-HHWWgg_bkg_v4-94X_mc2017-RunIIFall18-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1-db49498e7dc78d32430682b35e9cae55_USER_87.root

#-- Missing samples (expected):
# HHWWgg_bkg_v5/output_WGJJToLNuGJJ_EWK_aQGC-FS-FM_TuneCP5_13TeV-madgraph-pythia8*.root
# HHWWgg_bkg_v5/output_WGJJToLNu_EWK_QCD_TuneCP5_13TeV-madgraph-pythia8*.root

for Bkg in "${Bkgs[@]}"
do
   
   echo "Number of output files for Bkg $Bkg:"
   ls -1 $input/output_$Bkg*.root | wc -l 
   echo "Hadding..."

   haddedFile="${Bkg}_Hadded.root"
   hadd /eos/user/a/atishelm/ntuples/HHWWgg_DataMC/HHWWgg_Bkg/$haddedFile $input/output_$Bkg*.root 
#    hadd $output/$haddedFile $input/output_$Bkg*.root

done 

echo "Finished hadding Backgrounds!"

cd $fgg_direc
