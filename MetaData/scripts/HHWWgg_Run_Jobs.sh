#!/bin/sh

############################################################################################################################################################################################
#
# Abe Tishelman-Charny
# 12 December 2019
#
# The purpose of this script is the run fggrunjobs with the HHWWgg tagger on data, signal, or background.
#
# Example Usage:
#
## -- workspaces
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_v2-6_Workspaces_X600 --nEvents all --json Taggers/test/HHWWgg_v2-6/HHWWgg_v2-6_X600.json --condorQueue microcentury -g -s -w
#
## -- all final state variable branches with cut flow
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_v2-6_Trees --nEvents all --json Taggers/test/HHWWgg_v2-6/HHWWgg_v2-6.json --condorQueue microcentury -g -c -v -t
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_fggBackgrounds_v2_1_oneDY --nEvents all --json Taggers/test/Era2017_RR-31Mar2018_v2_1_oneDY.json --condorQueue microcentury -g -c -v -t
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_v2-3_CutFlow_SM_CutFlow --nEvents all --json Taggers/test/HHWWgg_v2-3/HHWWgg_v2-3_SM.json --condorQueue longlunch -g -c -t
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_fggBkgs_1_DataMC --nEvents all --json Taggers/test/Era2017_RR-31Mar2018_v2_1.json --condorQueue longlunch -g -c -v -t
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_2017Data_Again --nEvents all --json Taggers/test/HHWWgg_2017_Data_All/HHWWgg_Data_All_2017.json --condorQueue microcentury -g -c -v -t
# . HHWWgg_Run_Jobs.sh --labelName HHWWgg_fggBackgrounds_v2_3 --nEvents all --json Taggers/test/Era2017_RR-31Mar2018_v2_3.json --condorQueue longlunch -g -c -v -t
###########################################################################################################################################################################################

## Perform these steps before running:
# cmsenv
# voms
# cp /tmp/MYPROXY ~/
# export X509_USER_PROXY=~/MYPROXY

## User specific variables. Customize to your own working area(s)

#fggDirec="/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/" # flashgg directory
fggDirec="/afs/cern.ch/work/a/atishelm/private/HHWWgg_flashgg/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/"
ntupleDirec="/eos/user/a/atishelm/ntuples/HHWWgg/" # condor output directory
#fggDirec="$PWD/" # flashgg directory (It should be ${PWD})
echo "PWD = ${fggDirec}"
#ntupleDirec="$PWD/" # condor output directory
echo "ntupleDirec = ${ntupleDirec}"

## Other script parameters

label="" # name for condor output directory in ntupleDirec
numEvents="" # integer, or 'all' to run on all events
runWorkspaceStd="false" # use Systematics/test/workspaceStd.py as config
doCutFlow="false" # perform HHWWgg cutflow within workspaceStd.py workflow
saveHHWWggFinalStateVars="false" # save extra variables
runttH="false" # run on ttH background sample only
calcSystematics="false" # run workspaceStd.py systematics
dumpTrees="false" # dump trees in fggrunjobs output
dumpWorkspaces="false" # dump workspaces in fggrunjobs output
dryRun="false" # do not submit jobs
jsonpath="" # optional local json file to use for fggrunjobs arguments such as dataset and campaign
condorQueue="microcentury" # condor job flavour. Determines max running time for each job
year=""
doNonResAnalysis="" # do non resonant analysis 

## Get user specified argumenets

##-- what is the purpose of "output" ?
#options=$(getopt -o gcvstwr --long nEvents: --long output: --long labelName: --long json: --long condorQueue: --long year: -- "$@") # end name with colon ':' to specify argument string
options=$(getopt -o gcvstwrn --long nEvents: --long labelName: --long json: --long condorQueue: --long year: -- "$@") # end name with colon ':' to specify argument string 

[ $? -eq 0 ] || {
      echo "Incorrect option provided"
      exit 1
}
eval set -- "$options"
while true; do
      case "$1" in
      -g) runWorkspaceStd="true" ;;
      -c) doCutFlow="true" ;;
      -v) saveHHWWggFinalStateVars="true" ;;
      -s) calcSystematics="true" ;;
      -t) dumpTrees="true" ;;
      -w) dumpWorkspaces="true" ;;
      -r) dryRun="true" ;;
      #--output) shift; ntupleDirec=$1 ;; ##-- What's the purpose of this flag?
      -n) doNonResAnalysis="true" ;;
      --nEvents) shift; numEvents=$1 ;;
      --labelName) shift; label=$1 ;;
      --json) shift; jsonpath=$1 ;;
      --condorQueue) shift; condorQueue=$1 ;;
      --year) shift; year=$1 ;;
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
echo "jsonpath = $jsonpath"
echo "calcSystematics = $calcSystematics"
echo "dumpTrees = $dumpTrees"
echo "dumpWorkspaces = $dumpWorkspaces"
echo "year = $year"

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

if [ -z "$year" ]
then
  echo "Please choose a year with the --year flag"
  echo "This will determine which metaconditions to use"
  echo "Exiting"
  return
fi

## Make sure a json file is specified

# if [ $runData == 'false' ] && [ $runSignal == 'false' ] && [ $jsonpath == '' ]
if [ $jsonpath == '' ]
then
      echo "Please specify a json path with --jsonpath <json_path>"
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

## Run HHWWgg Tagger with Systematics/test/workspaceStd.py

if [ $runWorkspaceStd == 'true' ]
then
      echo "Submitting jobs with Systematics/test/workspaceStd.py as cmssw config"

      command='fggRunJobs.py --load '
      command+=$jsonpath
      command+=' -D -P -n 500 -d '
      command+=$output_direc
      command+=" --stage-to="$root_file_output
      command+=' -x cmsRun Systematics/test/workspaceStd.py maxEvents=' # workspaceStd.py
      command+=$numEvents
      command+=' -q ${condorQueue} --no-use-tarball --no-copy-proxy metaConditions='
      command+=$fggDirec

      metaConditions=""
      if [ $year == '2016' ]
      then
          metaConditions="MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json "
      elif [ $year == '2017' ]
      then
          metaConditions="MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json "
      elif [ $year == '2018' ]
      then
          metaConditions="MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json "
      else
          echo "ERROR - Need to specify 2016, 2017 or 2018 with --year flag"
          echo "Exiting script"
          return
      fi

      command+=${metaConditions}

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

      if [ $doCutFlow == 'true' ]
      then
           command+=' doHHWWggTagCutFlow=1 '
      fi

      if [ $saveHHWWggFinalStateVars == 'true' ]
      then
            command+=' saveHHWWggFinalStateVars=1'

      fi 

      if [[ $doNonResAnalysis == 'true' ]]
      then 
            command+=' doHHWWggNonResAnalysis=1'
      fi 
fi

echo "Evaluating command: $command"
eval "$command"
echo "Finished job for file: $jsonpath"
