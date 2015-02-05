#!/bin/bash

toy=$1
for meth in IC MIT; do
  for type in Asymptotic ProfileLikelihood MaxLikelihoodFit; do
    for file in `ls $toy/${meth}combResults/$type`; do
      size=`ls -l $toy/${meth}combResults/$type/$file | awk -F" " ' { print $5 } '` 
      name=`ls -l $toy/${meth}combResults/$type/$file | awk -F" " ' { print $NF } '`
      if [ "$size" -lt "6000" ]; then
        echo $size $name suspected incomplete
      fi
    done
  done
done
  
