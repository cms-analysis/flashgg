#!/bin/bash

parallel=$(dirname $(which $0))/parallel

wd=$1 && shift
datacard=$1 && shift
label=$1 && shift

min=110
max=150
step=0.5

[[ -n $1 ]] && min=$1 && shift 
[[ -n $1 ]] && max=$1 && shift 
[[ -n $1 ]] && step=$1 && shift 

njobs=1
if ( hostname | grep lxplus ); then njobs=8; fi

cd $wd
mkdir $label 

seq $min $step $max | $parallel -j $njobs "combine --verbose=2  -M MaxLikelihoodFit --rMin=-10 --rMax=10. -n $label -m {} -S 1 $datacard | tee pvalue_${label}_{}.log"

hadd -f $label/higgsCombine$label.MaxLikelihoodFit.root higgsCombine$label.MaxLikelihoodFit.mH*.root 

mv *$label*.root $label

mv *$label*.log $label

## hadd 
