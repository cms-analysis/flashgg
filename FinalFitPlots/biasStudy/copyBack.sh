#!/bin/bash

src=$1 && shift
dst=$1 && shift

id=0
[[ -n $1 ]] && id=$1

for f in $src/higgsCombine_*MaxLikelihoodFit.root; do
    fname=$(basename $f)
    modfname=$(echo $fname | sed "s%\.MaxLikelihoodFit%.MaxlikelihoodFit.mHAll.$id%")
    cp -pv $f $dst/$modfname
done

