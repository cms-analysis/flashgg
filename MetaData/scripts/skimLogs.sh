#!/bin/sh
# use to find jobs that didn't run properly 
direc=$1
max_files=-1
files=()
i=0
#HHWWgg_v2-3_CutFlow_WithSyst/runJobs15_4283143.158.out
for file in "${direc}"/runJobs*.err
#for file in "${direc}"/runJobs*.out
do
#   echo "$file"
  files+=( $file )
  let "i += 1"
#   echo "i = $i"
  if [ $i == $max_files ]; 
  then 
    break
  fi 
done

badFiles=()
badFileIDs=()
numBadFiles=0
clusterID="3970613"
# printf '%s\n' "${files[@]}"

#desiredOutput="HHWWgg_v2-3_CutFlow_WithSyst/outpu"
desiredOutput="dropped waiting message count 0"
for f in ${files[@]}; do
    # echo "entry = "
    lastLine=""
    lastLine=$( tail -n 1 $f )
    # echo "lastLine = $lastLine"
    if [ "$lastLine" != "$desiredOutput" ]; then
        # echo "[BadFile] - Last line does not equal DONE"
        # echo "[BadFile] - $f"
        # echo "[BadFile] - lastline = " $lastLine

        fileParts=$(echo $f | tr "." "\n") # parse file name by "." 
        fpi=0
        badFileID=""
        for fp in $fileParts 
        do 
            if [ $fpi == 2 ]; 
            then
                badFileID=$fp
                # echo "badFileID = " $badFileID
            fi 
            let "fpi += 1"
        done 

        badErrFile=""
        badErrFile="error/"
        badErrFile+="job."
        badErrFile+=clusterID
        badErrFile+='.'
        badErrFile+=$badFileID
        badErrFile+=".err"
        # echo "badErrFile = " $badErrFile
        badFiles+=( $badErrFile )
        badFileIDs+=( $badFileID )
	echo "last line: $lastLine"
	echo "file: $f"
        let "numBadFiles += 1"
    fi  
done

echo "numBadFiles = $numBadFiles"
echo "badFileIDs = " ${badFileIDs[@]}
# echo "badFiles = ${badFiles[@]}"


# Resubmit condor jobs for bad ones 
# for bfid in ${badFileIDs[@]}; do
    
#     echo "test" > myfile.txt 
#     echo "test" > myfile.txt 
# done 

# # Find error that caused job to not produce output root file 

# for bf in ${badFiles[@]}; do 
#     # skim for line where error comes up 
#     # Begin Fatal Exception
#     grep -hnr -A15 "Begin Fatal Exception" $bf # A15 = grep 15 lines after 
# done 

