#!/bin/bash

toy=$1

ind=0
for m in {110..149}; do
  massArray[$ind]=$m.0
  ind=$(($ind+1))
  massArray[$ind]=$m.5
  ind=$(($ind+1))
done
massArray[$ind]=150.0

all=false
totMissing=0

ind=0
for type in Asymptotic ProfileLikelihood MaxLikelihoodFit; do
  mitCount=0
  icCount=0
  for m in {110..150}; do
    mitFile=$1/MITcombResults/$type/higgsCombineTest.$type.mH$m.0.root
    if [ -e "$mitFile" ]; then
      mitCount=$(($mitCount+1))
    else
      if [ "$2" == "-v" ]; then echo Couldn\'t find: $mitFile; fi
			totMissing=$((totMissing+1))
    fi
  done
  for m in ${massArray[*]}; do
    icFile=$1/ICcombResults/$type/higgsCombineTest.$type.mH$m.root
    if [ -e "$icFile" ]; then
      icCount=$(($icCount+1))
    else
      if [ "$2" == "-v" ]; then echo Couldn\'t find: $icFile; fi
			totMissing=$((totMissing+1))
    fi
  done
  if [ "$mitCount" -eq "41" -a "$icCount" -eq "81" ]; then
    all=true
  else
    all=false
    strType[$ind]=$type
    strMIT[$ind]=$((41-mitCount))
    strIC[$ind]=$((81-icCount))
  fi
  ind=$((ind+1))
done

if [ "$totMissing" -eq "0" ]; then
  echo All files found
else
  for (( i=0; i<${#strType[@]}; i++ )); do
    echo -e MIT - ${strType[$i]} - ${strMIT[$i]} files missing;
    echo -e IC \ - ${strType[$i]} - ${strIC[$i]} files missing;
  done
	echo Total missing - $totMissing
fi
    
