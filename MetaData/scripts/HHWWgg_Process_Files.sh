#!/bin/sh

##############################################################################################################################################
# Abe Tishelman-Charny
# 22 January 2020
#
# The purpose of this script is to process output files from fggrunjobs.
#
# Example Usage:
#
# . HHWWgg_Process_Files.sh --inFolder HHWWgg_v2-6_Workspaces_X600 --outFolder HHWWgg_v2-6_Workspaces_X600_Hadded -s --signalType Res
# . HHWWgg_Process_Files.sh --inFolder HHWWgg_v2-6_2017_Data_Workspaces --outFolder HHWWgg_v2-6_2017_Data_Workspaces_Hadded -d
# . HHWWgg_Process_Files.sh --inFolder HHWWgg_v2-6_2017_Data_Workspaces_Hadded --outFolder HHWWgg_v2-6_2017_Data_Workspaces_Hadded_Combined -d -c
##############################################################################################################################################

## User specific variables. Customize to your own working area(s)
# nTupleDirec="/eos/user/r/rasharma/post_doc_ihep/double-higgs/ntuples/HHWWgg_5July_v3/"
nTupleDirec="$PWD/" # condor output directory
echo "nTupleDirec = ${nTupleDirec}"
fggDirec="$PWD/" # flashgg directory (It should be ${PWD}, if now please change this)

## Other script parameters
inputFolder=""
outputFolder=""
runSignal=""
runBackground=""
runData=""
allData=""
haddWorkspace=""
haddTrees=""

## Get user specified argumenets
options=$(getopt -o sdbcwt --long nTupleDir: --long inFolder: --long outFolder: --long signalType: -- "$@") # end name with colon ':' to specify argument string
[ $? -eq 0 ] || {
      echo "Incorrect option provided"
      exit 1
}
eval set -- "$options"
while true; do
      case "$1" in
      -s) runSignal="true";;
      -d) runData="true";;
      -b) runBackground="true";;
      -c) allData="true";;
      -w) haddWorkspace="true";;
      -t) haddTrees="true";;
      --nTupleDir) shift; nTupleDirec=$1 ;;
      --inFolder) shift; inputFolder=$1 ;;
      --outFolder) shift; outputFolder=$1 ;;
      --signalType) shift; signalType=$1 ;;
      --)
            shift
            break
            ;;
      esac
      shift
done

## Make sure input and output folder names are specified. These are compulsory
if [ -z "$inputFolder" ]
then
      echo "Please enter an input directory name with the --inFolder flag"
      echo "exiting"
      return
fi

if [ -z "$outputFolder" ]
then
      echo "Please enter an output directory name with the --outFolder flag"
      echo "exiting"
      return
fi

if [ -z "$runSignal" ] && [ -z "$runData" ] && [ -z "$runBackground" ]
then
      echo "Please add the flag -s to run on signal output or -d to run on data output"
      echo "or add -b to run over backgrounds."
      echo "exiting"
      return
fi

if [ "$runSignal" == "true" ] && [ -z "$signalType" ]
then
      echo "If you are running signal, you need to specify the signal type with the --signalType flag"
      echo "Options are: Res, NONRES, NMSSM"
      echo "exiting"
      return
fi

if [ "$haddTrees" == "true" ] && [ "haddWorkspace" == "true" ]
then
      echo "The two flags -w and -t can't be specified together."
      echo "exiting"
      return
fi

if [ "$haddTrees" == "true" ]
then
    haddCommand="--haddTrees"
fi

if [ "$haddWorkspace" == "true" ]
then
    haddCommand="--haddWorkspace"
fi

## Output read arguments to user
echo "nTupleDirec = ${nTupleDirec}"
echo "inputFolder = $inputFolder"
echo "outputFolder = $outputFolder"
echo "runSignal = $runSignal"
echo "runBackground = $runBackground"
echo "runData = $runData"
echo "allData = $allData"
echo "signalType = $signalType"

## Set variables to user inputs
cd $nTupleDirec
mkdir -p $outputFolder
cd $outputFolder
scriptLoc=$fggDirec
scriptLoc+="Systematics/scripts/hadd_all.py"
scriptLoc+=" ${haddCommand}"

echo "SCRIPT: $scriptLoc"

cd $fggDirec
cmsenv
cd $nTupleDirec
cd $inputFolder

ls > filesBefore.txt
echo "Hadding workspaces..."
echo $scriptLoc
python $scriptLoc > /dev/null # do not print any output from command # Create workspace hadded files
echo "Done hadding..."
ls > filesAfter.txt

# Save new files to filesDiff.txt
comm -13 filesBefore.txt filesAfter.txt > filesDiff.txt
readarray filePaths < filesDiff.txt

echo "===> Move newly hadded files to output directory"
let "i=0"
for file_i in "${filePaths[@]}"
do
	echo $file_i
	if [[ $file_i == *"intermediate"* ]]; then
		echo "file contains 'intermediate', skipping"
		continue
	fi

	if [[ $file_i == *".txt"* ]]; then
		echo "file contains '.txt', skipping"
		continue
	fi

	if [[ $runSignal == "true" ]]; then

           if [[ ${signalType} == "RES" ]]; then
                 echo "===> Inside RES condition"
                 mass="$(cut -d'_' -f3 <<<$file_i)" # get third '_' delimited element of file path. Should be X250, X260, etc.
                 channel="$(cut -d'_' -f4 <<<$file_i)" # get fourth '_' delimited element. qqqq_nodeX.root
                 FinalState="$(cut -d'_' -f5 <<<$file_i)" # get fifth '_' delimited element of file path. Should be qqlnu, lnulnu, qqqq
                 FinalState=${FinalState//'.root'/} # remove ".root"
                 infilePath="${nTupleDirec}/${inputFolder}/${file_i}"
                 outfilePath="${nTupleDirec}/${outputFolder}/${mass}_HH${channel}_${FinalState}.root"
                 # EXAMPLE: outfilePath="${nTupleDirec}/${outputFolder}/X1100_HHWWgg_qqqq.root"
           elif [[ ${signalType} == "NORES" ]]; then
                 echo "===> Inside NONRES condition"
                 # Input root file should be named such that its fourth '_' delimited
                 # element should be "qqqq" or "lnuqq" or "lnulnu" (channel name) and
                 # fifth '_' delimited element should be like "nodeX".
                 node="$(cut -d'_' -f5 <<<$file_i)" # get fifth '_' delimited element. nodeX.root
                 node=${node//'.root'/} # remove ".root"
                 FinalState="$(cut -d'_' -f4 <<<$file_i)" # get fourth '_' delimited element. qqqq_nodeX.root
                 channel="$(cut -d'_' -f3 <<<$file_i)" # get third '_' delimited element. qqqq_nodeX.root
                 echo "node: ${node}"
                 echo "FinalState: ${FinalState}"
                 echo "channel: ${channel}"
                 infilePath="${nTupleDirec}/${inputFolder}/${file_i}"
                 outfilePath="${nTupleDirec}/${outputFolder}/${node}_HH${channel}_${FinalState}.root" ##-- I think it's helpful to have the production mode in the name
                 # EXAMPLE: outfilePath="${nTupleDirec}/${outputFolder}/node11_HHWWgg_lnuqq.root"
           elif [[ ${signalType} == "NMSSM" ]]; then
                 #ex: output_NMSSM_XYHWWggqqlnu_MX2000_MY1800_15.root
                 mX="$(cut -d'_' -f 4 <<<$file_i)"
                 mY="$(cut -d'_' -f 5 <<<$file_i)"
                 mY=${mY%?????} # remove ".root"
                 infilePath="${nTupleDirec}/${inputFolder}/${file_i}"
                 outfilePath="${nTupleDirec}/${outputFolder}/${mX}_${mY}_HHWWgg_qqlnu.root" ##-- I think it's helpful to have the production mode in the name
           else
                 infilePath="${nTupleDirec}/${inputFolder}/${file_i}"
                 outfilePath="${nTupleDirec}/${outputFolder}/${file_i}"
           fi
	fi

	if [[ $runData == "true" ]]; then
		infilePath="${nTupleDirec}/${inputFolder}/${file_i}"
		if [[ $allData == "true" ]]; then
                 outfilePath="${nTupleDirec}/${outputFolder}/allData.root" # all data combined
		else
                 outfilePath="${nTupleDirec}/${outputFolder}/Data_$i.root" # data by era
		fi
	fi

     if [[ $runBackground == "true" ]]; then
           infilePath="${nTupleDirec}/${inputFolder}/${file_i}"
           outfilePath="${nTupleDirec}/${outputFolder}/${file_i}" # all data combined
     fi

	echo "infilePath: $infilePath"
	echo "outfilePath: $outfilePath"
     echo "mv $infilePath $outfilePath"
	mv $infilePath $outfilePath
	#. ../run.sh ../RenameWorkspace_SignalTagger $infilePath $outfilePath $mass # could potential be helpful
	let "i=i+1"

done

cd $fggDirec
