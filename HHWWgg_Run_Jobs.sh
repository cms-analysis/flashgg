#!/bin/sh

# The purpose of this script is the run fggrunjobs with ether the HHWWgg candidate dumper or tagger, on either data or signal. 


## Do these steps before running:
# cmsenv
# voms
# cp /tmp/MYPROXY ~/
# export X509_USER_PROXY=~/MYPROXY

# Specify these two to the user 
fggDirec="/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/"
ntupleDirec="/eos/user/a/atishelm/ntuples/HHWWgg/"

label=$1 
numEvents=$2 # can't be -1 because it's a flag. need integer argument 

if [ $numEvents == "all" ]
then
      echo "MaxEvents = -1"
      numEvents=-1
fi

if [ -z "$label" ]
then
      echo "Please enter a directory name as the 1st argument"
      echo "exiting"
      return
fi

if [ -z "$numEvents" ]
then
      echo ""
      # echo "Please enter a number of events as the 2nd argument"
      # echo "exiting"
      # return
fi


# options may be followed by one colon to indicate they have a required argument
if ! options=$(getopt -u -o stn -l  -- "$@")
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
runttH="false"
runData="false"
runSignal="false"

while [ $# -gt 0 ]
do
case $1 in
# -h|--help) usage; exit 0;;
# -f|--fTest) dofTest="true"; shift ;;
# -k|--signalfit) doSignalFit="true"; shift ;; 
# -p|--plotsig) plotSignal="true"; shift ;;
# -d|--dcard) makeDatacard="true"; shift ;;
-s|--rs) runSystematics="true"; shift ;;
-t|--rs) runttH="true"; shift ;;
-d|--rs) runData="true"; shift ;;
-x|--rs) runSignal="true"; shift ;;

(--) shift; break;;
(-*) usage; echo "$0: [ERROR] - unrecognized option $1" 1>&2; usage >> /dev/stderr; exit 1;;
(*) break;;
esac
shift
done

output_direc=$label
# Make output directories if they don't exist 
mkdir -p $output_direc;
mkdir -p $ntupleDirec$output_direc;
root_file_output=$ntupleDirec
root_file_output+=$output_direc

if [ $runSystematics == 'false' ]
then
      echo "Submitting jobs WITHOUT systematics"
      jsonpath=''
      if [ $runData == 'true' ]
      then
            jsonpath='Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json'
      fi

      if [ $runSignal == 'true' ]
      then
            jsonpath='Taggers/test/HHWWgg_2017_Signal/HHWWgg_Signal_2017.json'  
      fi     

      if [ ($runData == 'false' && $runSignal == 'false') || ($runData == 'true' && $runSignal == 'true') ]
      then
            echo "Please choose to run on either Data OR signal"
            echo "exiting"
            return
      fi      

      if [ $runttH == 'true' ]
      then 
            echo "Running ttH only" 
            jsonpath='Taggers/test/ttH/HHWWgg_Bkg_1.json'
      fi 

      command='fggRunJobs.py --load '
      command+=$jsonpath
      command+=' -D -P -n 500 -d ' # May need to be careful not to have too many output files. EOS has a limit. 
      command+=$output_direc
      command+=" --stage-to="$root_file_output
      command+=' -x cmsRun Taggers/test/HHWWggTest_cfg.py maxEvents='
      command+=$numEvents
      command+=' -q microcentury --no-use-tarball --no-copy-proxy metaConditions='
      command+=$fggDirec
      command+='MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json'
fi

if [ $runSystematics == 'true' ]
then
      echo "Submitting jobs WITH systematics"
      jsonpath=''

      if [ $runData == 'true' ]
      then
      jsonpath='Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json'
      fi

      if [ $runSignal == 'true' ]
      then
      jsonpath='Taggers/test/HHWWgg_2017_Signal/HHWWgg_Signal_2017.json'  
      fi    

      if [ ($runData == 'false' && $runSignal == 'false') || ($runData == 'true' && $runSignal == 'true') ]
      then
            echo "Please choose to run on either Data OR signal"
            echo "exiting"
            return
      fi       

      command='fggRunJobs.py --load '
      command+=$jsonpath
      command+=' -D -P -n 500 -d ' # May need to be careful not to have too many output files. EOS has a limit. 
      command+=$output_direc
      command+=" --stage-to="$root_file_output
      command+=' -x cmsRun Systematics/test/workspaceStd.py maxEvents=' # workspaceStd.py 
      command+=$numEvents
      command+=' -q microcentury --no-use-tarball --no-copy-proxy metaConditions='   
      command+=$fggDirec
      command+='MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json '
      command+=' doHHWWggTag=True HHWWggTagsOnly=True doSystematics=False dumpWorkspace=True dumpTrees=True'
      #     command+=' doHHWWggTag=True HHWWggTagsOnly=True doSystematics=False doBJetRegression=True dumpWorkspace=False dumpTrees=True'
fi

echo "command: $command"
eval "$command" 
echo "Finished job for file: $jsonpath"
