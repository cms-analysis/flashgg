#!/bin/bash
#1 method: ProfileLikelihood or MarkovChainMC
METHOD=$1
QUEUE=$2
DATE=`date +%F_%R_%S | tr -s ' ' '_' | tr -s ':' '_'`
WORKINGDIR=${METHOD}_$DATE
if [ ! -d $WORKINGDIR ]; then
	mkdir $WORKINGDIR
fi
if [ "$QUEUE" == "" ]; then
	QUEUE="8nh"
fi
cp run_OBS$METHOD.sh $WORKINGDIR
for M in `seq 110 0.5 140`; do
	echo "Submitting mass $M for $1 method. Results in $WORKINGDIR"
	bsub -q $QUEUE $PWD/$WORKINGDIR/run_OBS$METHOD.sh $PWD/$WORKINGDIR $M
done
