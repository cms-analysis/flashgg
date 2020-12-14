# example usage:
# . HHWWgg_Count_Signal_Files.sh /eos/user/a/atishelm/ntuples/HHWWgg/HHWWgg_v2-3_NoSyst

input=$1

# for mass in X250 X260 X270 X280 X300 X320 X350 X400 X500 X550 X600 X650 X700 X750 X800 X850 X900 X1000 X1250 SM;
for mass in X250 X260 X270 X280 X300 X320 X350 X400 X500 X550 X600 X650 X700 X800 X850 X900 X1000 X1250 SM;
do
   # echo "Number of output files for mass $mass:"
   numFiles=`ls -1 $input/output_ggF_${mass}_WWgg_qqlnugg_*.root | wc -l`
   # echo $numFiles

   # echo "Number there should be:"
   
   query="dasgoclient --query='file dataset=/ggF_${mass}_HHWWgg_qqlnu/atishelm-HHWWgg_v2-7-94X_mc2017-RunIIFall18-v0-atishelm-HHWWgg_v2-7_100000events_wPU_MINIAOD-5f646ecd4e1c7a39ab0ed099ff55ceb9-894aca5f1702fc00de4ca2fc3fbcfe4c/USER instance=prod/phys03'"
   # numFiles="ls -1 ${query} | wc -l"
   # echo "query: $query"
   # echo "${query}" | wc -l
   # ${query} | tee >(wc -l)
   rm out.txt 
   eval $query >> out.txt 
   lines=`wc -l out.txt | cut -f1 -d' '`
   # echo $lines 

   echo "-----mass: $mass-----"
   echo "should have: $lines"

   if (( numFiles != lines )); then 
      echo "Number of output files for mass $mass DON'T MATCH"
      echo "Should have: $lines"
      echo "Actually have: $numFiles"
   fi 


   # wc -l out.txt
   # rm out.txt 
   # ${query} >> out.txt 
   # eval $numFiles 

   #echo "Hadding..."
   #hadded_file="ggF_"
   #hadded_file+=$mass
   #hadded_file+="_WWgg_qqlnu"
   #hadded_file+="_Hadded.root"
   #mkdir -p $output
   #hadd $output/$hadded_file $input/output_ggF_${mass}_WWgg_qqlnugg_*.root

done

rm out.txt 
