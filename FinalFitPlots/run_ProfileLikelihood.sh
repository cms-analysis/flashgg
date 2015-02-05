#!/bin/bash
#1 Working Directory
#2 Mass
#3 Number of Toys
#4 Job Number
cd $1
JOBNUMBER=0
if [ -n $4 ]; then
	JOBNUMBER=$4
fi
SEED=$RANDOM
ODIR=$1/$2GeV
if [ ! -d $ODIR ]; then
	mkdir $ODIR
fi
eval `scramv1 runtime -sh`
combine cms-hgg-datacard_parBKG.txt -M ProfileLikelihood -D data_mass -m $2 -t $3 -s -1 -S 1 --generateBinnedWorkaround --tries 1  --maxTries 200 --rMin=0. --rMax=35 -H ProfileLikelihood --hintStatOnly | tee mH$2.$JOBNUMBER.$$.log
SEED=`cat mH$2.$JOBNUMBER.$$.log | grep '>>> Used OpenSSL to get a really random seed' | awk '{print$10}'`
mv mH$2.$$.$JOBNUMBER.log $ODIR/higgsCombineTEST.ProfileLikelihood.mH$2.$SEED.log
mv higgsCombineTEST.ProfileLikelihood.mH$2.$SEED.root $ODIR
