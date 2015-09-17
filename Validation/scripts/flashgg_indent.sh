#!/bin/bash

if [ ! -f $CMSSW_BASE/src/flashgg/.git/HEAD ];
then
  echo "CMSSW_BASE and flashgg appear not to be set correctly"
  echo
fi

cd $CMSSW_BASE/src/flashgg

git status -z | tr '\0' '\n' | grep "^(A|M)" | grep -E "(\.cc$|\.h$|\.C$)" | awk '{print $2}' > filelist

for file in `cat filelist`;do $CMSSW_BASE/src/flashgg/Validation/scripts/astyle --options=$CMSSW_BASE/src/flashgg/Validation/scripts/indent.ast $file -q; done

#carefull this line is to add the headers, just the first time or to change it in the future
#for file in `cat filelist`;do cat $file Validation/scripts/header > $file.buff;mv $file.buff $file; done

find . -iname '*.orig' >> orig_filelist

export changed_files=`wc -l orig_filelist | awk '{print $1}'`

if [ $changed_files == 0 ]
then
    echo "no files changed due to indentation"
else
    echo "You are about to change $changed_files files due to indentation"
    cat orig_filelist | sed -e "s|.orig||g"

    read -p "Do you want to continue? (Y/N) " -n 1 -r
    echo    # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
    #yes, removing original files
	for file in `cat orig_filelist`;do rm $file; done
    else
    #no, restoring original files
	for file in `cat orig_filelist | sed -e "s|.orig||g"`;do mv $file.orig $file; done
    fi
fi

rm orig_filelist filelist
