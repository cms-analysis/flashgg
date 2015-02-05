#!/bin/bash

parallel=$(dirname $(which $0))/parallel

wd=$1 && shift
datacard=$1 && shift
label=$1 && shift
masses=$@

cd $wd
mkdir $label 

$parallel -j 8 "combine --verbose=2  -M ChannelCompatibilityCheck --saveFitResult --rMin=-10 --rMax=10. -n $label -m {} -S 1 $datacard | tee pvalue_${label}_{}.log" ::: $masses

hadd -f $label/higgsCombine$label.ChannelCompatibilityCheck.root higgsCombine$label.ChannelCompatibilityCheck.mH*.root 

mv *$label*.root $label

mv *$label*.log $label

