#!/bin/bash
# run with $1=toy directory number $2=boost type (grad / ada) $3=$PWD
cd $3
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
mkdir -p $1/ICcombResults/
cd $1/ICcombResults

for m in {110..150}; do 
  echo -------------------------------
  echo Running $2 $1 limit for mass $m.0
  echo -------------------------------
  combine ../mva-datacard_$2_$m.0.txt -M Asymptotic -D data_$2 -m $m --newGenerator=1 -H ProfileLikelihood
  combine ../mva-datacard_$2_$m.0.txt -M MaxLikelihoodFit -D data_$2 -m $m --minimizerStrategy=2
  combine ../mva-datacard_$2_$m.0.txt -M ProfileLikelihood -D data_$2 -m $m -S 1 --rMin=0. --rMax=25 --signif --pvalue
done

for m in {110..149}; do 
  echo -------------------------------
  echo Running $2 $1 limit for mass $m.5 
  echo -------------------------------
  combine ../mva-datacard_$2_$m.5.txt -M Asymptotic -D data_$2 -m $m.5 --newGenerator=1 -H ProfileLikelihood
  combine ../mva-datacard_$2_$m.5.txt -M MaxLikelihoodFit -D data_$2 -m $m.5 --minimizerStrategy=2
  combine ../mva-datacard_$2_$m.5.txt -M ProfileLikelihood -D data_$2 -m $m.5 -S 1 --rMin=0. --rMax=25 --signif --pvalue
done

mkdir Asymptotic MaxLikelihoodFit ProfileLikelihood
for m in {110..150}; do
	mv higgsCombineTest.Asymptotic.mH$m.root Asymptotic/higgsCombineTest.Asymptotic.mH$m.0.root
	mv higgsCombineTest.MaxLikelihoodFit.mH$m.root MaxLikelihoodFit/higgsCombineTest.MaxLikelihoodFit.mH$m.0.root
	mv higgsCombineTest.ProfileLikelihood.mH$m.root ProfileLikelihood/higgsCombineTest.ProfileLikelihood.mH$m.0.root
done

for m in {110..149}; do
	mv higgsCombineTest.Asymptotic.mH$m.5.root Asymptotic/higgsCombineTest.Asymptotic.mH$m.5.root
	mv higgsCombineTest.MaxLikelihoodFit.mH$m.5.root MaxLikelihoodFit/higgsCombineTest.MaxLikelihoodFit.mH$m.5.root
	mv higgsCombineTest.ProfileLikelihood.mH$m.5.root ProfileLikelihood/higgsCombineTest.ProfileLikelihood.mH$m.5.root
done
cd $3

