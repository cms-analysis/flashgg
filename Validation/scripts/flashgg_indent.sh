#!/bin/bash

find . -iname '*.C' > filelist
find . -iname '*.cc' >> filelist
find . -iname '*.h' >> filelist

for file in `cat filelist`;do Validation/scripts/astyle --options=Validation/scripts/indent.ast $file; done

#carefull this line just the first time
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
