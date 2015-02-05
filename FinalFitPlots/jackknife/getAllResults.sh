#!/bin/bash

name1=ICHEP
name2=ICHEPLike

set -x 
for d in $@; do
    npart=$(( $(ls -d -C1 $d/$name1/part*/ | wc -l) - 1 ))
    ./getNevents.sh $d/$name1 $d/$name2  | tee $d/nevents.csv
    ./getResults.sh $d/$name1 $d/$name2 $npart | tee $d/results.csv
    ./getScanResults.sh $d/$name1 $d/$name2 $npart | tee $d/scan.csv
done

