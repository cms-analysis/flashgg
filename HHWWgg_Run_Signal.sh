#!/bin/sh
# fggRunJobs.py --load Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json -D -P -n 500 -d testing_fggrunjobs --stage-to=/eos/user/a/atishelm/ntuples/Event_Dumper/test_fgg_again -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents=100 -q microcentury --no-use-tarball --no-copy-proxy

## Do these steps before running:
# cmsenv
# voms
# cp /tmp/MYPROXY ~/
# export X509_USER_PROXY=~/MYPROXY

# initializeCommand(){
#       command='fggRunJobs.py --load '
#       command+=$jsonpath
#       command+=' -D -P -n 500 -d ' # May need to be careful not to have too many output files. EOS has a limit. 
#       command+=$output_direc
#       command+=" --stage-to="$root_file_output
#       command+=' -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents='
#       command+=$num_events
#       command+=' -q microcentury --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json'
#       return $command 
# }

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


# options may be followed by one colon to indicate they have a required argument
if ! options=$(getopt -u -o s -l  -- "$@")
then
# something went wrong, getopt will put out an error message for us
echo "should exit"
fi
set -- $options

# dofTest="false"
# createPoints="false"
# doSignalFit="false"
# plotSignal="false"
# makeDatacard="false"
runSystematics="false"

while [ $# -gt 0 ]
do
case $1 in
# -h|--help) usage; exit 0;;
# -f|--fTest) dofTest="true"; shift ;;
# -k|--signalfit) doSignalFit="true"; shift ;; 
# -p|--plotsig) plotSignal="true"; shift ;;
# -d|--dcard) makeDatacard="true"; shift ;;
-s|--rs) runSystematics="true"; shift ;;

(--) shift; break;;
(-*) usage; echo "$0: [ERROR] - unrecognized option $1" 1>&2; usage >> /dev/stderr; exit 1;;
(*) break;;
esac
shift
done

output_direc=$label
# Make output directories if they don't exist 
mkdir -p $output_direc;
mkdir -p /eos/user/a/atishelm/ntuples/Event_Dumper/$output_direc;
root_file_output="/eos/user/a/atishelm/ntuples/Event_Dumper/HHWWgg/"
root_file_output+=$output_direc

if [ $runSystematics == 'false' ]
then
      echo "Submitting jobs WITHOUT systematics"
      jsonpath='Taggers/test/HHWWgg_2017_Signal/HHWWgg_Signal_2017_DumperCandidate.json' 
      command='fggRunJobs.py --load '
      command+=$jsonpath
      command+=' -D -P -n 500 -d ' # May need to be careful not to have too many output files. EOS has a limit. 
      command+=$output_direc
      command+=" --stage-to="$root_file_output
      command+=' -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents='
      command+=$num_events
      command+=' -q microcentury --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json'
      
fi

if [ $runSystematics == 'true' ]
then
    echo "Submitting jobs WITH systematics"
    jsonpath='Taggers/test/HHWWgg_2017_Signal/HHWWgg_Signal_2017_Systematics.json' 
    command='fggRunJobs.py --load '
    command+=$jsonpath
    command+=' -D -P -n 500 -d ' # May need to be careful not to have too many output files. EOS has a limit. 
    command+=$output_direc
    command+=" --stage-to="$root_file_output
    command+=' -x cmsRun Systematics/test/workspaceStd.py maxEvents=' # workspaceStd.py 
    command+=$num_events
    command+=' -q microcentury --no-use-tarball --no-copy-proxy metaConditions=/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json'
    command+=' doHHWWggTag=True HHWWggTagsOnly=True doSystematics=False doBJetRegression=True dumpWorkspace=False dumpTrees=True'
fi

echo "command: $command"
eval "$command" 
echo "Finished job for file: $jsonpath"
