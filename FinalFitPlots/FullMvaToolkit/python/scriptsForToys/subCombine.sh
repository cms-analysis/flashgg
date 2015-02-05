#!/bin/bash
# run with $1=toy directory $2=$PWD $3=option
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

cd $2
source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
mkdir $PWD/$1/batchout

if $IC; then
  qsub -q hepshort.q -e $PWD/$1/batchout -o $PWD/$1/batchout ./limit.sh $1 grad $2
fi

if $MIT; then
	cp /vols/cms02/h2g/latest_workspace/cms-hgg-datacard_mit_full_toy.txt $PWD/$1
  for m in {110..150}; do
    cd $2
    qsub -q hepshort.q -e $PWD/$1/batchout -o $PWD/$1/batchout ./run_Asymptotic.sh $m $PWD $1
    qsub -q hepshort.q -e $PWD/$1/batchout -o $PWD/$1/batchout ./run_ProfileLikelihood.sh $m $PWD $1
    qsub -q hepshort.q -e $PWD/$1/batchout -o $PWD/$1/batchout ./run_MaxLH.sh $m $PWD $1
  done
fi
