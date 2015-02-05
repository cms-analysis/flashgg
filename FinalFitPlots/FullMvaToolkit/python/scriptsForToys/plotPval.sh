#!/bin/bash
# $1=$PWD $2=toy num $3=option
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
  python pvalPlots.py $t/${meth}combResults
  cp $t/${meth}combResults/pvalues.pdf $t/ResultPlots/${t}_${meth}_pvals.pdf 
  cp $t/${meth}combResults/pvalues.png $t/ResultPlots/${t}_${meth}_pvals.png 
  cp $t/${meth}combResults/pvalues_nobf.pdf $t/ResultPlots/${t}_${meth}_pvals_nobf.pdf 
  cp $t/${meth}combResults/pvalues_nobf.png $t/ResultPlots/${t}_${meth}_pvals_nobf.png 
fi

if $MIT; then
	meth=MIT
  python pvalPlots.py $t/${meth}combResults
  cp $t/${meth}combResults/pvalues.pdf $t/ResultPlots/${t}_${meth}_pvals.pdf 
  cp $t/${meth}combResults/pvalues.png $t/ResultPlots/${t}_${meth}_pvals.png 
  cp $t/${meth}combResults/pvalues_nobf.pdf $t/ResultPlots/${t}_${meth}_pvals_nobf.pdf 
  cp $t/${meth}combResults/pvalues_nobf.png $t/ResultPlots/${t}_${meth}_pvals_nobf.png 
fi
