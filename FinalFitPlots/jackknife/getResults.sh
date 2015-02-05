#!/bin/bash

dir1=$1 && shift
dir2=$1 && shift

npart=321
[[ -n $1 ]] && npart=$1 && shift

for part in $(seq 0 $npart); do
    echo -n $part","
    ./getMostSignif.sh $dir1/part${part} | tr '\n' ' ' | tr ' ' ','
    ./getMostSignif.sh $dir2/part${part} | tr '\n' ' ' | tr ' ' ','
    echo
done
