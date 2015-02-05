#!/bin/bash
# $1=$PWD $2=toy dir $3=option

echo Hope you\'ve run subCombine.sh first

source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
cd $1

echo combineChecker...
./combineChecker.sh $2
echo plotLimit...
./plotLimit.sh $1 $2 $3
echo makeGR...
./makeGR.sh $2 $3
echo plotPval...
./plotPval.sh $1 $2 $3





