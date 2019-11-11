#!/bin/sh
## Run these commands before running on jsons 
# cmsenv
# voms
# cp /tmp/MYPROXY ~/
# export X509_USER_PROXY=~/MYPROXY

label=$1 
num_events=$2

if [ -z "$label" ]
then
      echo "Please enter a directory name as the 1st argument"
      echo "exiting"
      return
fi

if [ -z "$num_events" ]
then
      echo "Please enter a number of events as the 2nd argument"
      echo "exiting"
      return
fi

output_direc=$label
# Make output directories if they don't exist 
mkdir -p $output_direc;
mkdir -p /eos/user/a/atishelm/ntuples/Event_Dumper/$output_direc;

for f in 9 10 16;
do
   echo "Submitting jobs for datasets from era = "$f

   path='Taggers/test/HHWWgg_2017_Bkg/HHWWgg_Bkg_'
   path+=${f}
   path+='.json'

   command='fggRunJobs.py --load '
   command+=$path
   command+=' -D -P -n 500 -d ' # Might need to be careful not to have too many output files. EOS has a limit. 
   command+=$output_direc
   command+=' -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents='
   command+=$num_events
   command+=' -q microcentury --no-use-tarball --no-copy-proxy'
   echo "command: $command"
   eval "$command" 
   echo "Finished job for file: $path"
   mv $output_direc/*.root /eos/user/a/atishelm/ntuples/Event_Dumper/$output_direc/ 
#    rm $output_direc/*.err
#    rm $output_direc/*.out
   echo "Finished moving files!"

   # fggRunJobs.py --load Taggers/test/Bkg_Jsons/HHWWgg_Bkg_${f}_2017.json -D -P -n 500 -d 1_old_sample_test -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents=100 -q microcentury --no-use-tarball --no-copy-proxy
   # echo "Finished job for era = "$f
   # mv 1_old_sample_test/*.root /eos/user/a/atishelm/1_old_sample_test
   # echo "Finished moving files!"
done 