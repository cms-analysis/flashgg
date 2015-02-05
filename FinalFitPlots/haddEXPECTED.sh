#!/bin/bash

# Hadd the expected limit files from Mass points
# set INDIR to where the overall set is stored
# run with -> bash haddEXPECTED.sh METHOD   (ProfileLikelihood, Bayesian, Frequentist)

METHOD=$1

# Nick uses at IC
INDIR=../output_limits/
SUBDIR=o_
FILEEXTRA=

#For use with MULTICRAB, say each job is m_MASS (eg m_100)
#INDIR=./$METHOD
#SUBDIR=m_
#FILEEXTRA=/res/

if [[ ! -d $INDIR ]] ; then 
	  echo "No Directory Exists -> "
	  echo $INDIR
	  echo "..... i'll make it now"
	  
fi

ODIR=./$METHOD
if [[ ! -d $ODIR ]] ; then mkdir $ODIR; echo "Created Directory - $ODIR"; fi

for D in {105,110,115,120,125,130,135,140}; do

  HADDIR=$INDIR/$METHOD/$SUBDIR$D$FILEEXTRA
  if [[ ! -d $INDIR ]] ; then 
    echo "Warning No Directory Found -> "
    echo $HADDIR
  else
    echo "HADDING $HADDIR"
    hadd -f $ODIR/expected$METHOD.mH$D.root $HADDIR/* > /dev/null
  fi  
  
done	

echo "Done"
