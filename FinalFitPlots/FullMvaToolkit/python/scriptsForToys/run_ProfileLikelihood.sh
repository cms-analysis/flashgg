#!/bin/bash
# run with $1=mass $2=$PWD $3=Toy
hostname
cd $2
cp CMS-HGG_massfac_full_110_150_1.root $3/
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
mkdir -p $3/MITcombResults/
cd $3/MITcombResults

combine ../cms-hgg-datacard_mit_full_toy.txt -M ProfileLikelihood -m $1 --signif --pvalue -v 2

mkdir ProfileLikelihood
mv higgsCombineTest.ProfileLikelihood.mH$1.root ProfileLikelihood/higgsCombineTest.ProfileLikelihood.mH$1.0.root
cd $2

