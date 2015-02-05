#!/bin/bash
#1 Working Directory
#2 Mass
cd $1
ODIR=$1
cd ..
if [ ! -d $ODIR ]; then
	mkdir $ODIR
fi
eval `scramv1 runtime -sh`
combine cms-hgg-datacard_parBKG.txt -M ProfileLikelihood -D data_mass -m $2 -s -1 -S 1 --generateBinnedWorkaround  --rMin=0 --rMax=30  --tries 1 --maxTries 200  -H ProfileLikelihood   --hintStatOnly | tee mH$2.$$.log
SEED=`cat mH$2.$$.log | grep '>>> Used OpenSSL to get a really random seed' | awk '{print$10}'`
mv mH$2.$$.log $ODIR/higgsCombine.ProfileLikelihood.mH$2.$SEED.log
mv higgsCombineTest.ProfileLikelihood.mH$2.$SEED.root $ODIR/higgsCombine.ProfileLikelihood.mH$2.root
