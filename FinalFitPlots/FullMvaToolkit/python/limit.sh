#!/bin/bash
# run with $1= directory $2=boost type (grad / ada) $3=$PWD
cd $3
eval `scramv1 runtime -sh`
cd $1

for l in $(seq 110.0 0.5 150.0); do
  m=`printf "%5.1f\n" $l`
  echo -------------------------------
  echo Running $2 $1 limit for mass $m
  echo -------------------------------
  #combine mva-datacard_$2_$m.txt -M Asymptotic -D data_$2 -m $m  
  #combine mva-datacard_$2_$m.txt -M MaxLikelihoodFit -D data_$2 -m $m --minimizerStrategy=2 --rMin=-3. --rMax=3. 
  #combine mva-datacard_$2_$m.txt -M ProfileLikelihood -m $m --rMin=0. --rMax=25 --signif --pvalue 
  combine mva-datacard_$2_$m.txt -M ProfileLikelihood -m $m --rMin=0. --rMax=25 --signif --pvalue -t -1 --expectSignal=1 -n Exp 
done
