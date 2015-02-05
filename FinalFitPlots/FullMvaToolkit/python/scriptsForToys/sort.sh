#!/bin/bash
#$1=toy directory $2=option

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
  for meth in Asymptotic ProfileLikelihood MaxLikelihoodFit; do
    for m in {110..150}; do
      mv ${t}/${r}combResults/$meth/higgsCombineTest.$meth.mH$m.root ${t}/${r}combResults/$meth/higgsCombineTest.$meth.mH$m.0.root 
    done
  done
fi

if $MIT; then
  r=MIT
  for meth in Asymptotic ProfileLikelihood MaxLikelihoodFit; do
    for m in {110..150}; do
      mv ${t}/${r}combResults/$meth/higgsCombineTest.$meth.mH$m.root ${t}/${r}combResults/$meth/higgsCombineTest.$meth.mH$m.0.root 
    done
  done
fi
