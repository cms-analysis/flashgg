#!/bin/sh
# fggRunJobs.py --load Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json -D -P -n 500 -d testing_fggrunjobs --stage-to=/eos/user/a/atishelm/ntuples/Event_Dumper/test_fgg_again -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents=100 -q microcentury --no-use-tarball --no-copy-proxy

## Do these steps before running:
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

root_file_output="/eos/user/a/atishelm/ntuples/Event_Dumper/"
root_file_output+=$output_direc

jsonpath='Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json' # json 

command='fggRunJobs.py --load '
command+=$jsonpath
command+=' -D -P -n 500 -d ' # Might need to be careful not to have too many output files. EOS has a limit. 
command+=$output_direc
command+=" --stage-to="$root_file_output
command+=' -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents='
command+=$num_events
command+=' -q microcentury --no-use-tarball --no-copy-proxy'
echo "command: $command"
eval "$command" 
echo "Finished job for file: $jsonpath"