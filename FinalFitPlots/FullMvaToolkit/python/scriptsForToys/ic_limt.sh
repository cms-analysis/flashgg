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
  combine ../mva-datacard_$2_$m.0.txt -M Asymptotic -D data_$2 -m $m
done

for m in {110..149}; do 
  echo -------------------------------
  echo Running $2 $1 limit for mass $m.5 
  echo -------------------------------
  combine ../mva-datacard_$2_$m.5.txt -M Asymptotic -D data_$2 -m $m.5
done

mkdir Asymptotic
mv higgsCombineTest.Asymptotic.mH1* Asymptotic/

cd $3

