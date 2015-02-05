#!/bin/bash
# script to output some extra info about types of polynomial

cd /afs/cern.ch/user/m/mkenzie/workPoint/SimultaneousSignalFit
eval `scramv1 runtime -sh`

mkdir -p extra_files

for dm in {0..2}; do
  for sigma in {0..2}; do
    for frac in {0..1}; do
      ./bin/SimultaneousSignalFit -i CMS-HGG.root -p ggh -c 0 -g 3 --dmOrder=$dm --sigmaOrder=$sigma --fracOrder=$frac -s -o extra_files/CMS-HGG_sigfit_ggh_cat0_${dm}${sigma}${frac}.root -S extra_files/CMS-HGG_extra_ggh_cat0_${dm}${sigma}${frac}.root -v 2
    done
  done
done
