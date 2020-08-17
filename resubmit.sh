#!/usr/bin/env bash
a=$2
if [ ! $a ]; then
	a='microcentury'
fi
echo queue:$a
num_jobs=`ls $1runJobs*.sh |wc -l`
echo num of jobs:${num_jobs}
# rm $1*.err
# rm $1*.out
# rm $1.log
for((num_job=0;num_job<$num_jobs;num_job++));
do
echo '###############'
pre_name=`grep 'outputFile=' ./$1runJobs${num_job}.sh |awk '{print$16}'`
snd_name=${pre_name#*/} ## '#' remove right
file_prefix=${snd_name%.*}
echo $file_prefix
num_file=`ls Samples/$1${file_prefix}_*.root |wc -l`
echo job:${num_job}
echo files:$num_file
pre_num=`grep -r nJobs ./$1runJobs${num_job}.sh |awk '{print$17}'`
real_num=${pre_num#*=}
echo $real_num
if [ $num_file -ne $real_num ];then
j=0

for((i=0;i<$real_num;i++));
do 
#  find ./Samples/Test_Cut_Signal_withTau/ -name *X$2*$i*.root
#  if [ $a ne 0 ]; then
file=Samples/$1${file_prefix}_$i.root ## '%' remove left
#$1 was num of jobs $2 was mass of samples $3 was saples path
if [ ! -f "$file" ]; then
  j=`expr $j + 1`
  # echo -e "$i \c"
  need_resub="$need_resub $i"
  echo $file
  # echo $need_resub
  fi
done
echo "j= $j"
runjob_name=$1runJobs${num_job}.sh
#cp ${1%/*}Bak/runJobs${num_job}.sh $1Bak_runJobs${num_job}.sh
line=`grep -n declare ${1}runJobs${num_job}.sh |awk -F ":" '{print $1}'`
#echo lnie:$line
sed -i 's/.*declare.*/declare -a jobIdsMap=('"${need_resub}"')/g' $1runJobs${num_job}.sh
cp $1runJobs${num_job}.sub ${runjob_name%/*}_${num_job}.sub
sed -i 's/.*queue.*/queue '"${j}"'/g' ${runjob_name%/*}_$num_job.sub
sed -i 's#microcentury#'"${a}"'#g' ${runjob_name%/*}_$num_job.sub
condor_submit ${runjob_name%/*}_$num_job.sub
need_resub=''
fi
done
