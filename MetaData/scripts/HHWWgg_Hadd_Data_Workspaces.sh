#!/bin/sh

# The purpose of this script is to hadd output files from HHWWgg condor jobs on data 
# Run from one directory outside directory with data output files 

input=$1 # path of directory with condor output files 
output=$2 # path of directory to put hadded files into 

if [ -z "$input" ]
then
      echo "Please enter an input directory as the 1st argument"
      echo "exiting"
      cd $fgg_direc
      return
fi

# if [ -z "$output" ]
# then
#       echo "Please enter an output directory as the 2nd argument"
#       echo "exiting"
#       cd $fgg_direc
#       return
# fi

cmsenv
fgg_direc=/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg
ntuplesDirec=/eos/user/a/atishelm/ntuples/HHWWgg

echo "Changing directory to: $ntuplesDirec"
cd $ntuplesDirec

for Era in B C D E F;
do
   
   echo "Number of output files for era $Era:"
   ls -1 $input/*Run2017$Era*.root | wc -l 
#    echo "Hadding..."
#    hadded_file="Run2017_Era"
#    hadded_file+=$Era
#    hadded_file+="_Hadded.root"
#    mkdir -p $output
#    hadd $output/$hadded_file $input/*Run2017$Era*.root

done 

echo "Hadding workspaces..."

cd $input

haddCommand="python "
haddCommand+=$fgg_direc
haddCommand+="/Systematics/scripts/hadd_all.py"

echo "haddcomand: $haddCommand"

$haddCommand 

# after this 

echo "Hadding eras..."

cd $ntuplesDirec

hadded_combined=$output
hadded_combined+="_Combined"

mkdir -p $hadded_combined
# hadd $hadded_combined/Run2017_AllErasCombined_Hadded.root $output/*.root 
echo "Finished hadding data workspaces!"

# Next need to create a new directory with these hadded into 1 ...

# for Era in B C D E F;
# do
   
#    echo "Number of output files for era $Era:"
#    ls -1 $input/*Run2017$Era*.root | wc -l 
#    echo "Hadding..."
#    hadded_file="Run2017_Era"
#    hadded_file+=$Era
#    hadded_file+="_Hadded.root"
#    mkdir -p $output
#    hadd $output/$hadded_file $input/*Run2017$Era*.root

# done 

# echo "Hadding eras..."
# mkdir -p $hadded_combined
# hadd $hadded_combined/Run2017_AllErasCombined_Hadded.root $output/*.root 
# echo "Finished hadding data!"

cd $fgg_direc
