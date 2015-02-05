#!/bin/bash

wd=$1 && shift
mass=$1 && shift

cd $wd 

combine --verbose=2  -M MaxLikelihoodFit --rMin=-10 --rMax=10. -n BestMu -m $mass -S 1 datacard.txt | tee pvalue_BestMu_${mass}.log

mv pvalue_BestMu_${mass}.log BestMu
