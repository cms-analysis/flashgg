#!/bin/bash

for m in $(seq 110 5 150); do
    awk "/Best fit r:/ { printf \"%1.1f,%1.4f,\",$m,\$4 }" $1/BestMu5GeV/pvalue_BestMu5GeV_${m}.log
    awk "/Best fit r:/ { printf \"%1.1f,%1.4f,\",$m,\$4 }" $2/BestMu5GeV/pvalue_BestMu5GeV_${m}.log
done



