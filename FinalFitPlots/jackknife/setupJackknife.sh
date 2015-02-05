#!/bin/bash

set -x

inputd1=$1 && shift
inputd2=$1 && shift

jsons=$1 && shift

name1=$(basename $(dirname $inputd1))
name2=$(basename $(dirname $inputd2))

ws1=$inputd1/../CMS-HGG_data.root
sig1=$PWD/$inputd1/CMS-HGG_signal.root

ws2=$inputd2/../CMS-HGG_data.root
sig2=$PWD/$inputd2/CMS-HGG_signal.root

input1=$inputd1/minitree.root
input2=$inputd2/minitree.root


for json in $(echo $jsons | tr ',' '\n'); do
    
    echo "Setting up $dir"
    
    dir=$(echo $json | sed 's%.json%%')
    mkdir $dir
    mkdir $dir/$name1
    mkdir $dir/$name2
    
    echo "Splitting minitrees"
    ./splitMiniTree.py -i $input1 -w $ws1 -g -p $json -d $dir/$name1 -c 9 > $dir/$name1/split.log
    npart=$(awk '/npart:/ { print $2 }' $dir/$name1/split.log)
    
    ./splitMiniTree.py -i $input2 -w $ws2 -g -p $json -d $dir/$name2  -c 9 > $dir/$name2/split.log
    ## ./splitMiniTree.py -i $input2 -w $ws2 -m -p $json -d $dir/$name2  -c 6 > $dir/$name2/split.log
    
    echo "Setting up for combine"
    cp -p $inputd1/../datacard.txt $dir/$name1
    cp -p $inputd2/../datacard.txt $dir/$name2

    ./setupForCombine.sh $dir/$name1 $(basename $input1 | sed 's%.root%%')_part $npart datacard.txt $sig1
    ./setupForCombine.sh $dir/$name2 $(basename $input2 | sed 's%.root%%')_part $npart datacard.txt $sig2

done

