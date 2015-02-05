#!/bin/bash
# $1=$PWD $2=toy dir $3=option
IC=false
MIT=false
if [ "$3" == "IC" ]; then
  IC=true
elif [ "$3" == "MIT" ]; then
  MIT=true
else
  IC=true
  MIT=true
fi

source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
cd $1

t=$2
mkdir $t/ResultPlots

if $IC; then
  meth=IC
  cd $1
  cp limit-plotter-complete-$meth.py $1/$t/${meth}combResults/
  cp ../medianCalc.C $1/$t/${meth}combResults/
  cp ../Normalization.C $1/$t/${meth}combResults/
  cp ../theory_*.py $1/$t/${meth}combResults/
  cd $1/$t/${meth}combResults/
  python limit-plotter-complete-$meth.py Asymptotic sm -r -s -o limit.root
  cp limit_sm_Asymptotic_ratio.pdf ../ResultPlots/${t}_${meth}_limit.pdf
  cp limit_sm_Asymptotic_ratio.png ../ResultPlots/${t}_${meth}_limit.png
fi

if $MIT; then
  meth=MIT
  cd $1
  cp limit-plotter-complete-$meth.py $1/$t/${meth}combResults/
  cp ../medianCalc.C $1/$t/${meth}combResults/
  cp ../Normalization.C $1/$t/${meth}combResults/
  cp ../theory_*.py $1/$t/${meth}combResults/
  cd $1/$t/${meth}combResults/
  python limit-plotter-complete-$meth.py Asymptotic sm -r -o limit.root
  cp limit_sm_Asymptotic_ratio.pdf ../ResultPlots/${t}_${meth}_limit.pdf
  cp limit_sm_Asymptotic_ratio.png ../ResultPlots/${t}_${meth}_limit.png
fi
