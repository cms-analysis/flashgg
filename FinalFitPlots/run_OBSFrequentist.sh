#!/bin/bash
#1 Working Directory
#2 Mass
#3 RooModel Directory
#4 Grid File
REMOTEDIR=$PWD
cd $1
cp ../$4 $REMOTEDIR
GRIDFILENAME=`basename $4`
SCRAM_ARCH=slc5_amd64_gcc462
export SCRAM_ARCH
eval `scramv1 runtime -sh`
combine ../${3}/${2}GeVmodel.root -m ${2} -M HybridNew -s -1 --freq --rAbsAcc=0.001 --grid=$REMOTEDIR/$GRIDFILENAME
SEED=`/bin/ls higgsCombineTest.HybridNew.mH${2}.*.root | grep -v quant | sed "s|higgsCombineTest.HybridNew.mH${2}.\([-0-9][-0-9]*\).root|\1|" | grep -v .root`
NEWMASS=`echo $2 | sed 's|^\([0-9][0-9][0-9]\)$|\1.0|'`
mv higgsCombineTest.HybridNew.mH$2.${SEED}.root higgsCombineOBSERVED.HybridNew.mH$NEWMASS.root
