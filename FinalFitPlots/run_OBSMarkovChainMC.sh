#!/bin/bash
#1 Working Directory
#2 Mass
if [ ! -d $1 ]; then
	mkdir $1
fi
cd $1
ln -s ../CMS-HGG.root CMS-HGG.root
ln -s ../cms-hgg-datacard_parBKG.txt cms-hgg-datacard_parBKG.txt
eval `scramv1 runtime -sh`
combine -d cms-hgg-datacard_parBKG.txt -M MarkovChainMC -D data_mass -m $2 -s -1 -S 1 -n OBSERVED --rMin=0.0 --rMax=60.0 -b 300 -i 5000 --tries=1 --optimizeSim=1 | tee mH$2.Markov.$$.log
SEED=`cat mH$2.Markov.$$.log | grep '>>> Used OpenSSL to get a really random seed' | awk '{print$10}'`
mv mH$2.Markov.$$.log higgsCombineOBSERVED.MarkovChainMC.mH$2.$SEED.log
mv higgsCombineOBSERVED.MarkovChainMC.mH$2.$SEED.root higgsCombineOBSERVED.MarkovChainMC.mH$2.root
