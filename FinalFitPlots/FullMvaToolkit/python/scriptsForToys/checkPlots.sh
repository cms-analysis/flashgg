#!/bin/bash

toy=$1

count=0
for meth in IC MIT; do
  for type in limit pvals pvals_nobf; do
    for ext in pdf png; do
      file=$1/ResultPlots/$1_${meth}_$type.$ext
      if [ -e "$file" ]; then
        count=$(($count+1))
      else
        echo $file missing
      fi
    done
  done
done

if [ "$count" -eq "12" ]; then
  echo All files found
else
  echo $((12-$count)) plots missing
fi
