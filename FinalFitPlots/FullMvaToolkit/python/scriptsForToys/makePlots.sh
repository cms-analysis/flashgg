#!/bin/bash
# run with $1=$PWD $2=toy Number

source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`
cd $1

./sort.sh $2
# need to check size of files
fail=0

for meth in IC MIT; do
  for type in Asymptotic ProfileLikelihood MaxLikelihoodFit; do
    for size in `ls -l Toy$2/${meth}combResults/$type/ | grep 'higgsCombine' | awk -F" " ' { print $5 $NF } '`; do
      fs=`echo $size | awk -F"higgs" ' { print $1 } '`
      fn=`echo $size | awk -F"higgs" ' { print $NF } '`
      if ((fs<5000)); then
        echo Toy$2/${meth}combResults/$type/higgs$fn is not big enough
        mass=`echo $fn | awk -F"$type.mH" ' { print $NF } ' | awk -F".root" ' {print $1 } '`
        if [ "$meth" == "IC" ]; then
          qsub -q hepshort.q -e $PWD -o $PWD ./limit.sh $2 grad $1
        elif [ "$meth" == "MIT" ]; then
          if [ "$type" == "MaxLikelihoodFit" ]; then
            qsub -q hepmedium.q -e $PWD -o $PWD ./run_MaxLH.sh $mass $PWD $2
          else
            qsub -q hepmedium.q -e $PWD -o $PWD ./run_${type}.sh $mass $PWD $2
          fi
        fi
        fail=$(($fail+1))
      fi
    done
  done
done

if [ $fail -eq 0 ]; then
  ./makeGR.sh $2
  python pvalPlots.py Toy$2/MITcombResults/
  python pvalPlots.py Toy$2/ICcombResults/
  ./plotLimit.sh $1 $2
fi



