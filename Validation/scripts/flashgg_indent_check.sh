#!/bin/bash

git status -z | tr '\0' '\n' | grep "^(A|M)" | grep -E "(\.cc$|\.h$|\.C$)" | awk '{print $2}' > filelist

for file in `cat filelist`;do $CMSSW_BASE/src/flashgg/Validation/scripts/astyle --options=$CMSSW_BASE/src/flashgg/Validation/scripts/indent.ast $file -q; done

find . -iname '*.orig' >> orig_filelist

export changed_files=`wc -l orig_filelist | awk '{print $1}'`

if [ $changed_files == 0 ]
then
    echo "no files changed due to indentation"
else
    echo "Your commit has $changed_files files failing indentation, please run Validation/scripts/flashgg_indent.sh before commit"
    cat orig_filelist | sed -e "s|.orig||g"
    echo "please run flashgg/Validation/scripts/flashgg_indent.sh before commit"
    echo
    rm orig_filelist filelist
    exit 1
fi

rm orig_filelist filelist
