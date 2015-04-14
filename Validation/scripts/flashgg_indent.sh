#!/bin/bash

if [ ! -f $CMSSW_BASE/src/flashgg/.git/HEAD ];
then
  echo "CMSSW_BASE and flashgg appear not to be set correctly"
  echo
  return 1
fi

cd $CMSSW_BASE/src/flashgg

find . -iname '*.C' > filelist
find . -iname '*.cc' >> filelist
find . -iname '*.h' >> filelist

for file in `cat filelist`;do Validation/scripts/astyle --options=Validation/scripts/indent.ast $file; done

#carefull this line just the first time
#for file in `cat filelist`;do cat $file Validation/scripts/header > $file.buff;mv $file.buff $file; done

find . -iname '*.orig' >> orig_filelist

for file in `cat orig_filelist`;do rm $file; done

rm orig_filelist filelist
