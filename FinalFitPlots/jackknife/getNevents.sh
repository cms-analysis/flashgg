#!/bin/bash

dir1=$1 && shift
dir2=$1 && shift

npart=321
[[ -n $1 ]] && npart=$1 && shift

echo -n "-1,"
awk '/Number of events in the tree/ { printf "%d," ,$7 }' $dir1/split.log
awk '/Number of events in the tree/ { print $7 }' $dir2/split.log

for part in $(seq 0 $npart); do
    echo -n $part","
    grep "partition $part\$" -A 11 $dir1/split.log | awk '/Tot/ { printf "%d," ,$3 }' 
    grep "partition $part\$" -A 11 $dir2/split.log | awk '/Tot/ { print $3 }'
done
