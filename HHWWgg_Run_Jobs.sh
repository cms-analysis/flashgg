#!/bin/sh

#------------------------------------------------------------------------------------------------------------------------------

# Abe Tishelman-Charny
# 12 December 2019
#
# The purpose of this script is the run fggrunjobs with ether the HHWWgg candidate dumper or tagger, on data, signal, or background. 

#------------------------------------------------------------------------------------------------------------------------------

## Do these steps before running:
# cmsenv
# voms
# cp /tmp/MYPROXY ~/
# export X509_USER_PROXY=~/MYPROXY

# Example:
# . HHWWgg_Run_Jobs.sh --labelName HHWWggTaggerTest_Testagain --nEvents 1000 --json Taggers/test/HHWWgg_v2-2/HHWWgg_Signal_2017.json -g -c -s -t

# . HHWWgg_Run_Jobs.sh --labelName HHWWggTaggerTest --nEvents 1000 --json Taggers/test/HHWWgg_v2-2/HHWWgg_Signal_2017.json -w 
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_v2-3_Systematics --nEvents all --json Taggers/test/HHWWgg_v2-3/HHWWgg_v2-3.json -w -x 

## User specific variables. Customize to your own working area(s)

fggDirec="/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/" # flashgg directory 
ntupleDirec="/eos/user/a/atishelm/ntuples/HHWWgg/" # condor output directory 

## Other script parameters 

label="" # name for condor output directory in ntupleDirec 
numEvents="" # integer, or 'all' to run on all events 
runWorkspaceStd="false" # use Systematics/test/workspaceStd.py as config 
doCutFlow="false" # perform HHWWgg cutflow within workspaceStd.py workflow 
runttH="false" # run on ttH background sample only 
runData="false" # get datasets from data json file 
runSignal="false" # get dataset(s) from signal json file 
calcSystematics="false" # run workspaceStd.py systematics 
dumpTrees="false" # dump trees in fggrunjobs output 
dumpWorkspaces="false" # dump workspaces in fggrunjobs output 
dryRun="false" # do not submit jobs 
jsonpath="" # optional local json file to use for fggrunjobs arguments such as dataset and campaign 
condorQueue="tomorrow"

## Get user specified argumenets 

options=$(getopt -o dgcstwr --long nEvents: --long labelName: --long json: --long condorQueue: -- "$@") # end name with colon ':' to specify argument string 
[ $? -eq 0 ] || {
      echo "Incorrect option provided"
      exit 1
}
eval set -- "$options"
while true; do
      case "$1" in
      #-s)
      #      runSignal="true"
      #      ;;
      -d)
            runData="true"
            ;;
      -g) 
            runWorkspaceStd="true"
            ;;
      -c)
            doCutFlow="true"
            ;;
      -s)   
            calcSystematics="true"
            ;;
      -t)   
            dumpTrees="true"
            ;;
      -w)   
            dumpWorkspaces="true"
            ;;
      -r)   
            dryRun="true"
            ;;
      --nEvents)
            shift; 
            numEvents=$1
            ;;
      --labelName)
            shift; 
            label=$1
            ;;
      --json)
            shift; 
            jsonpath=$1
            ;;
      --condorQueue)
            shift;
            condorQueue=$1
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
echo "jsonpath = $jsonpath"
echo "calcSystematics = $calcSystematics"
echo "dumpTrees = $dumpTrees"
echo "dumpWorkspaces = $dumpWorkspaces"

## Make sure numEvents and label arguments are specified. These are compulsory

if [ -z "$numEvents" ]
then
      echo ""
      echo "Please enter a number of events with the --nEvents flag "
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
      echo "Please enter a directory name with the --labelName flag"
      echo "exiting"
      return
fi

## Make sure a json file is specified 

if [ $runData == 'false' ] && [ $runSignal == 'false' ] && [ $jsonpath == '' ]
then
      echo "Please choose to run on either Data OR signal with the -s or -d flag"
      echo "Or specify a json path with --jsonpath <json_path>"
      echo "exiting"
      return
fi   

if [ $runData == 'true' ] && [ $runSignal == 'true' ] && [ $jsonpath == '' ]
then
      echo "Please choose to run on either Data OR signal with the -s or -d flag"
      echo "Or specify a json path with --jsonpath <json_path>"
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

## Run HHWWgg Candidate Dumper with Taggers/test/HHWWggTest_cfg.py

if [ $runWorkspaceStd == 'false' ]
then
      echo "Submitting jobs with Taggers/test/HHWWggTest_cfg.py as cmssw config"
      # jsonpath=''
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

      #command="fggRunJobs.py --load ${jsonpath} -D -P -n 500 -d ${output_direc}"

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

## Run HHWWgg Tagger with Systematics/test/workspaceStd.py 

if [ $runWorkspaceStd == 'true' ]
then
      echo "Submitting jobs with Systematics/test/workspaceStd.py as cmssw config"

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
      command+=' -q ${condorQueue} --no-use-tarball --no-copy-proxy metaConditions='   
      command+=$fggDirec
      command+='MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json '
      command+=' doHHWWggTag=True HHWWggTagsOnly=True '

      if [ $calcSystematics == 'true' ]
      then
           command+=' doSystematics=True '
      else 
           command+='doSystematics=False '
      fi

      if [ $dumpTrees == 'true' ]
      then 
           command+=' dumpTrees=True '
      else 
           command+=' dumpTrees=False '
      fi  

      if [ $dumpWorkspaces == 'true' ]
      then 
           command+=' dumpWorkspace=True '

      else 
           command+=' dumpWorkspace=False '
      fi       

      echo "dryRun: $dryRun"

      if [ $dryRun == 'true' ]
      then 
           command+=' dryRun=1 '
      fi 

      # else 
      #      command+=' dryRun=0 '
      # fi   
      
      if [ $doCutFlow == 'true' ]
      then
           command+=' doHHWWggTagCutFlow=True '
      fi       
      #     command+=' doHHWWggTag=True HHWWggTagsOnly=True doSystematics=False doBJetRegression=True dumpWorkspace=False dumpTrees=True'
fi

echo "Evaluating command: $command"
eval "$command" 
echo "Finished job for file: $jsonpath"