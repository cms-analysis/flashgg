#!/bin/bash
#1 Input Datacard or RooModel Directory
#2 Output Dir
#3 Max Number of Threads

SCRAM_ARCH=slc5_amd64_gcc462
export SCRAM_ARCH
eval `scramv1 runtime -sh`

MASSES=`seq 110 0.5 150`
THREADS=0

if [ ! -d $2 ]; then
    mkdir $2
fi
cd $2
if [ "$3" == "" ]; then
	MAXTHREADS=`cat /proc/cpuinfo | grep processor | awk '{print $3}' | tail -1 | xargs -i echo '{}+1' | bc`
else
	MAXTHREADS=$3
fi
echo "Max Threads: $MAXTHREADS"
for MASS in $MASSES; do
	THREADS=`ps | grep combine | wc | awk '{print $1}'`
	while [ ! $THREADS -lt $MAXTHREADS ]; do
		sleep 5
		THREADS=`ps | grep combine | wc | awk '{print $1}'`
	done
	echo "Subitting Mass ${MASS}GeV"
	if [ -d ../$1 ]; then
		combine  ../$1/${MASS}GeVmodel.root -m $MASS -M ChannelCompatibilityCheck --rMin=-20 --verbose=1 --saveFitResult -s -1 -n SignalStrength >& higgsCombineTest.ChannelCompatibilityCheck.mH${MASS}.log &
	elif [ -f ../$1 ]; then
		combine ../$1 -m $MASS -M ChannelCompatibilityCheck --rMin=-20 --verbose=1 --saveFitResult -s -1 -n SignalStrength >& higgsCombineTest.ChannelCompatibilityCheck.mH${MASS}.log &
	fi
	sleep 1
done
while [ ! $THREADS -eq 0 ]; do
	sleep 2
	THREADS=`ps | grep combine | wc -l`
done
for FILE in `/bin/ls higgsCombineSignalStrength.ChannelCompatibilityCheck.*.root`; do
        TARGETFILE=`echo "$FILE" | sed 's|higgsCombineSignalStrength.ChannelCompatibilityCheck.mH\(1[0-9][0-9][\.5]*\)\.[0-9-][0-9-]*.root|higgsCombineSignalStrength.ChannelCompatibilityCheck.mH\1.root|' | sed 's|mH\(1[0-9][0-9]\).root|mH\1.0.root|'`
        mv $FILE $TARGETFILE
done
cd ..
