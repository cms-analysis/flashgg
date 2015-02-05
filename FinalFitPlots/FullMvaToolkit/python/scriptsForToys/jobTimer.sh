#!/bin/bash
# run with $1=$PWD $2=toydir $3=start toy n $4=end toy n
toy=$2
endpos=$4
curpos=$3

# run toy first
lstart=$curpos

for (( t=$lstart; t<$endpos; t++ )); do
	qsub -q hepshort.q ./subToy.sh $PWD ${toy}${t}
done
echo Submitted toys
# wait until they finish
jobsInQ=`qstat | grep 'mk1009' | wc -l`
until [ "$jobsInQ" -eq "0" ]; do
	sleep 10
	echo $jobsInQ jobs in queue
	jobsInQ=`qstat | grep 'mk1009' | wc -l`
done

# then run combine
curpos=$3
while [ "$curpos" -lt "$endpos" ]; do
  echo Checking space for next batch of combine jobs
	until [ "$jobsInQ" -lt "760" ]; do
		sleep 10
    echo $jobsInQ jobs in queue
		jobsInQ=`qstat | grep 'mk1009' | wc -l`
	done
  echo Submitting more
  lstart=$curpos
  lend=$(($curpos+10))
  for (( t=$lstart; t<$lend; t++ )); do
    ./subCombine.sh ${toy}${t} $1
		jobsInQ=`qstat | grep 'mk1009' | wc -l`
    curpos=$t
    echo Submitted toy $curpos
  done
  curpos=$(($curpos+1))
done

# wait until they finish
jobsInQ=`qstat | grep 'mk1009' | wc -l`
until [ "$jobsInQ" -eq "0" ]; do
	sleep 10
	echo $jobsInQ jobs in queue
	jobsInQ=`qstat | grep 'mk1009' | wc -l`
done

# then make plots
for (( t=$3; t<$4; t++ )); do
	qsub -q hepshort.q ./runAll.sh $1 ${toy}${t}
done
