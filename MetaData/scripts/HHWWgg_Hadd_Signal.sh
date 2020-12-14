#!/bin/sh

########################################################################################################################################################
# Abraham Tishelman-Charny 
# 13 May 2020
# 
# The purpose of this script is to hadd outFolder files from HHWWgg condor jobs on data 
# Run from one directory outside directory with data outFolder files 
#
# Example Usage:
#
# . HHWWgg_Hadd_Signal.sh --inFolder HHWWgg_v2-4_CutFLow --outFolder HHWWgg_v2-4_NMSSM_CutFlow_Hadded --signalType NMSSM
########################################################################################################################################################

fgg_direc=/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg
ntuple_direc="/eos/user/a/atishelm/ntuples/HHWWgg"

cd ${ntuple_direc}

inputFolder=""
outputFolder=""
runSignal=""
runData=""

## Get user specified argumenets 

options=$(getopt -o sd --long inFolder: --long outFolder: --long signalType: -- "$@") # end name with colon ':' to specify argument string 
[ $? -eq 0 ] || {
      echo "Incorrect option provided"
      exit 1
}
eval set -- "$options"
while true; do
      case "$1" in
      -s) runSignal="true";;
      # -d) runData="true";;
      --inFolder) shift; inputFolder=$1 ;;
      --outFolder) shift; outputFolder=$1 ;;
      --signalType) shift; signalType=$1 ;;
      --)
            shift
            break
            ;;
      esac
      shift
done


# inFolder=$1 # path of directory with condor outFolder files 
# outFolder=$2 # path of directory to put hadded files into 

if [ -z "$inputFolder" ]
then
      echo "Please enter an inFolder directory with the --inFolder flag"
      echo "exiting"
      cd $fgg_direc
      return
fi

if [ -z "$outputFolder" ]
then
      echo "Please enter an outFolder directory with the --outFolder flag"
      echo "exiting"
      cd $fgg_direc
      return
fi

# mkdir -p $outputFolder;
# hadded_combined=$outputFolder
# hadded_combined+="_Combined"

if [ $signalType == "Res" ]; then 

      # for mass in X250 X260 X270 X280 X300 X320 X350 X400 X500 X550 X600 X650 X700 X750 X800 X850 X900 X1000 X1250 SM;
      for mass in X260 X600 X1000;
      do
      echo "Number of outFolder files for mass $mass:"
      # ls -1 $inputFolder/output_ggF_${mass}_WWgg_qqlnugg_*.root | wc -l 
      ls -1 $inputFolder/output_ggF_${mass}_HHWWgg_qqlnu_*.root | wc -l 
      echo "Hadding..."
      hadded_file="ggF_"
      hadded_file+=$mass
      hadded_file+="_WWgg_qqlnu"
      hadded_file+="_Hadded.root"
      mkdir -p $outputFolder
      # hadd $outputFolder/$hadded_file $inputFolder/output_ggF_${mass}_WWgg_qqlnugg_*.root
      hadd $outputFolder/$hadded_file $inputFolder/output_ggF_${mass}_HHWWgg_qqlnu_*.root

      done 

elif [ $signalType == "NMSSM" ]; then 

      for massPair in MX1000_MY800 MX1600_MY400 MX2000_MY1800 MX300_MY170;
      do 
            echo "Number of outFolder files for mass pair $massPair:"
            ls -1 $inputFolder/output_NMSSM_XYHWWggqqlnu_${massPair}_*.root | wc -l 
            echo "Hadding..."
            hadded_file="NMSSM_XYHWWggqqlnu_${massPair}_Hadded.root"
            mkdir -p $outputFolder 
            hadd $outputFolder/$hadded_file $inputFolder/output_NMSSM_XYHWWggqqlnu_${massPair}_*.root 
      done 

fi

echo "Finished hadding signal ntuples!"

#echo "Hadding eras..."
#mkdir -p $hadded_combined
#hadd $hadded_combined/Run2017_AllErasCombined_Hadded.root $outFolder/*.root 
#echo "Finished hadding data!"

cd $fgg_direc
