#!/bin/sh

#------------------------------------------------------------------------------------------------------------------------------

# Abe Tishelman-Charny
# 12 December 2019
#
# The purpose of this script is the run fggrunjobs with ether the HHWWgg candidate dumper or tagger, on either data or signal. 

#------------------------------------------------------------------------------------------------------------------------------

## Do these steps before running:
# cmsenv
# voms
# cp /tmp/MYPROXY ~/
# export X509_USER_PROXY=~/MYPROXY

## User specific variables. Customize to your own working area(s)

fggDirec="/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/" # flashgg directory 
ntupleDirec="/eos/user/a/atishelm/ntuples/HHWWgg/" # condor output directory 

## Other script parameters 

label="" # name for condor output directory in ntupleDirec 
numEvents="" # integer, or 'all' to run on all events 
runWorkspaceStd="false" # use Systematics/test/workspaceStd.py as config 
runttH="false" # run on ttH background sample only 
runData="false" # get datasets from data json file 
runSignal="false" # get dataset(s) from signal json file 

## Get user specified argumenets 

options=$(getopt -o sdw --long nEvents: --long labelName: -- "$@") # end name with colon ':' to specify argument string 
[ $? -eq 0 ] || {
      echo "Incorrect option provided"
      exit 1
}
eval set -- "$options"
while true; do
      case "$1" in
      -s)
            runSignal="true"
            ;;
      -d)
            runData="true"
            ;;
      -w) 
            runWorkspaceStd="true"
            ;;
      --nEvents)
            shift; 
            numEvents=$1
            ;;
      --labelName)
            shift; 
            label=$1
            ;;
      --)
            shift
            break
            ;;
      esac
      shift
done 

## Output read arguments to user 

echo "label = $label" 
echo "numEvents = $numEvents" 
echo "runWorkspaceStd = $runWorkspaceStd"
echo "rundata = $runData"
echo "runsignal = $runSignal"

## Make sure numEvents and label arguments are specified. These are necessary 

if [ -z "$numEvents" ]
then
      echo ""
      echo "Please enter a number of events with the -n flag "
      echo "exiting"
      return
fi

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

## Make sure only data OR signal is run on (for the moment it's not configured to run on both, but of course this can be implemented if desired)

if [ $runData == 'false' ] && [ $runSignal == 'false' ] 
then
      echo "Please choose to run on either Data OR signal"
      echo "exiting"
      return
fi   

if [ $runData == 'true' ] && [ $runSignal == 'true' ] 
then
      echo "Please choose to run on either Data OR signal"
      echo "exiting"
      return
fi  

## Set variables to user inputs 

output_direc=$label
# Make output directories if they don't exist 
mkdir -p $output_direc;
mkdir -p $ntupleDirec$output_direc;
root_file_output=$ntupleDirec
root_file_output+=$output_direc

## Run HHWWgg Dumper with HHWWggTest_cfg.py

if [ $runWorkspaceStd == 'false' ]
then
      echo "Submitting jobs with Taggers/test/HHWWggTest_cfg.py as cmssw config"
      jsonpath=''
      if [ $runData == 'true' ]
      then
            jsonpath='Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json'
      fi

      if [ $runSignal == 'true' ]
      then
            jsonpath='Taggers/test/HHWWgg_2017_Signal/HHWWgg_Signal_2017.json'  
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

## Run HHWWgg tagger with workspaceStd.py 

if [ $runWorkspaceStd == 'true' ]
then
      echo "Submitting jobs with Systematics/test/workspaceStd.py as cmssw config"
      jsonpath=''

      if [ $runData == 'true' ]
      then
      jsonpath='Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json'
      fi

      if [ $runSignal == 'true' ]
      then
      jsonpath='Taggers/test/HHWWgg_2017_Signal/HHWWgg_Signal_2017.json'  
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
      command+=' doHHWWggTag=True HHWWggTagsOnly=True doSystematics=False dumpWorkspace=True dumpTrees=True '
      #     command+=' doHHWWggTag=True HHWWggTagsOnly=True doSystematics=False doBJetRegression=True dumpWorkspace=False dumpTrees=True'
fi

echo "command: $command"
eval "$command" 
echo "Finished job for file: $jsonpath"
