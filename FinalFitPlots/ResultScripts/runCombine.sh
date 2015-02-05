#!/bin/bash
# To run Asymptotic limits at 110 do e.g.           ./runCombine.sh $PWD Asymptotic datacard.txt 110
# To run observed pvalue at 110 do e.g.             ./runCombine.sh $PWD ProfileLikelihood datacard.txt 110 --signif --pvalue
# To run expected pvalue at 110 do e.g.  ./runCombine.sh $PWD ExpProfileLikelihood datacard.txt 110 --signif --pvalue -t -1 --expectSignal=1

# can currently only take integer masses

eval `scramv1 runtime -sh`

dir=$1
method=$2
datcard=$3
mass=$4

if [ "$method" == "ExpProfileLikelihood" ]; then
  method=ProfileLikelihood
  folder=ProfileLikelihood
  naming=EXPECTED
else
  folder=$method
  naming=Test
fi

cd $dir

echo --------------------------------------------
echo   Running $method on $datcard at mass $mass 
echo --------------------------------------------

combine $datcard -M $method -m $mass $5 $6 $7 $8 $9 ${10} ${11} ${12} ${13} ${14}

if [ ! -d "$folder" ]; then
  mkdir $folder
fi

mv higgsCombineTest.$method.mH$mass.root $folder/higgsCombine$naming.$method.mH$mass.root
