#!/bin/bash
#1 Datacard
#2 Output Directory
#3 Number of Threads

SCRAM_ARCH=slc5_amd64_gcc462
export SCRAM_ARCH
eval `scramv1 runtime -sh`

if [ ! -d $2 ]; then
	mkdir $2
fi
THREADS=0
if [ "$3" == "" ]; then
	MAXTHREADS=`cat /proc/cpuinfo | grep processor | awk '{print $3}' | tail -1 | xargs -i echo '{}+1' | bc`
else
	MAXTHREADS=$3
fi

for MASS in `seq 110 0.5 150`; do
	THREADS=`ps | grep python | wc | awk '{print $1}'`
	while [ ! $THREADS -lt $MAXTHREADS ]; do
		sleep 5
		THREADS=`ps | grep python | wc | awk '{print $1}'`
	done
	echo "Making ${MASS}GeVmodel.root"
	text2workspace.py -m ${MASS} $1 -b -o ${2}/${MASS}GeVmodel.root &
	sleep 1
done
while [ ! $THREADS -eq 0 ]; do
	sleep 2
	THREADS=`ps | grep python | wc -l`
done
