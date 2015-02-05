#!/bin/bash

set -x

workdir=$1 && shift
cat=$1 && shift
truth=$1 && shift
test=$1 && shift
ntoys=$1 && shift
outdir=$1 && shift

datacard="datacard_bias_study_template.txt"
[[ -n $1 ]] && datacard=$1 && shift

masses=""
for m in $(seq 110 5 150); do
    masses="$masses -m $m"
done


mydir=$(dirname $(which $0))

wd=$PWD

cd $mydir

eval `scram ru -sh`

cmsMkdir $outdir

cd $wd

cp -p $mydir/$workdir/datacard_truth_*.txt . 
cp -p $mydir/$workdir/datacard*template*.txt . 
cp -p $mydir/$workdir/CMS-HGG*.root . 

ls -l

$mydir/runBias.py --datacard-template $datacard --truth $truth --test $test --cat $cat --ntoys $ntoys $masses 

for f in higgsCombine*MaxLikelihoodFit*mHAll*.root; do
    cmsStage -f $f $outdir
done

if [[ -d asimov_plots ]]; then
    tar zcf plots_cat${cat}_truth_${truth}_asimov_test_${test}.tgz asimov_plots/*/*fit_s.png
    cmsMkdir $outdir/asimov_plots
    cmsStage -f plots_cat${cat}_truth_${truth}_asimov_test_${test}.tgz $outdir
fi
