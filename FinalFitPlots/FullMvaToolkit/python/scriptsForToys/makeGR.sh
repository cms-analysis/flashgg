#!/bin/bash
#$1=toy directory $2=option

source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`

IC=false
MIT=false
if [ "$2" == "IC" ]; then
  IC=true
elif [ "$2" == "MIT" ]; then
  MIT=true
else
  IC=true
  MIT=true
fi

t=$1

if $IC; then
  r=IC
  python makeMLgr.py ${t}/${r}combResults/bf.root ${t}/${r}combResults/MaxLikelihoodFit/higgsCombineTest.MaxLikelihoodFit*  
  python makePLgr.py ${t}/${r}combResults/pvals.root ${t}/${r}combResults/ProfileLikelihood/higgsCombineTest.ProfileLikelihood*  
fi

if $MIT; then
  r=MIT
  python makeMLgr.py ${t}/${r}combResults/bf.root ${t}/${r}combResults/MaxLikelihoodFit/higgsCombineTest.MaxLikelihoodFit*  
  python makePLgr.py ${t}/${r}combResults/pvals.root ${t}/${r}combResults/ProfileLikelihood/higgsCombineTest.ProfileLikelihood*  
fi
