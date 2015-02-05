#! /bin/bash

workingDir=$1
datacard=$2
nJobs=$3
offset=$4
filename=$5

let sum=$offset+$nJobs

for i in $(seq $offset $sum);
  do 
  rm $5_$i.sh
  echo "#! /bin/bash">>"$5_$i.sh"
  echo "cd $PWD">>"$5_$i.sh"
  echo "eval \`scram runtime -sh\`">>"$5_$i.sh"
  echo "python getPValues.py $workingDir $datacard 1 $i">>"$5_$i.sh"
  chmod 777 $5_$i.sh
  bsub -q 8nm $5_$i.sh
done;




