#!/bin/bash

## set -x

wd=$1 && shift
pattern=$1 && shift

npart=321
[[ -n $1 ]] && npart=$1 && shift

datacard=datacard.txt
[[ -n $1 ]] && datacard=$1 && shift

sig=
[[ -n $1 ]] && sig=$1 && shift

cd $wd

fname=$(awk '/data_obs/ { print $4 }' $datacard   | head -1 )

for part in $(seq 0 $npart); do
    mkdir part${part}
    mv ${pattern}${part}.root part${part}/${fname}
    cp -p $datacard part${part}
    [[ -f $sig ]] && ln -s $sig part${part}
    cp -p ../../seed.txt part${part}
done
