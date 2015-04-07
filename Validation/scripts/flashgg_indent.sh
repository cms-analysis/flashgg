#!/bin/bash

cd ../../

find . -iname '*.C' > filelist
find . -iname '*.cc' >> filelist
find . -iname '*.h' >> filelist

for file in `cat filelist`;do Validation/scripts/astyle --options=Validation/scripts/indent.ast $file; done

#carefull this line just the first time
#for file in `cat filelist`;do cat $file Validation/scripts/header > $file.buff;mv $file.buff $file; done

find . -iname '*.orig' >> orig_filelist

for file in `cat orig_filelist`;do rm $file; done

rm orig_filelist filelist
