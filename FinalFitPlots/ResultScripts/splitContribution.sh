#!/bin/bash
#####
# This script take one datacard and split it in the different channels to study the contribution of each channel
# This script test also the effect of systematics
# Take the datacard, create one subdir called WORKDIR and creates subdirs for each contribution

usage(){
    echo "`basename $0` [options]"
    echo " -w,--workdir workdir [workdir]"
    echo " -n,--datacardName name (new name of the datacard in workdir)"
    echo " -d,--datacard datacard (*)"
    echo " --create (create the working dir and datacards"
    echo " --run (run the fits)"
    echo " --plot (make the plots)"
    echo " --channels (if you want only particular channels"
    echo " --statOnly"
    echo " --splitBin (splitting by bins)"
    echo " --splitProcess (splitting by process)"
    echo " --split bin, process (can be used multiple times, append each time)"
    echo " --lumiFactor lumi (1)"
    echo " -e expected 1 SM (default)" 
    echo " -o observed"
    echo " --expected n: n times SM"
    echo " --model cVcF, rVrF [cVcF]" 
    echo " --desc (description of usage and utility of the script)"
    echo " (*): mandatory options"
}

desc(){
    echo -e "
#################### DESCRIPTION #######################################
#   the `basename $0` script create a working directory to study all
#   the contributions to the CV CF limit coming from one datacard. 
#   You should give to this script the datacard, it will create a
#   subdirectory with the datacard and the workspaces indicated in the
#   datacard, it creates in this working directory as many
#   subdirectories as the channels in indicated by the datacard plus
#   one for the entier datacard. Using the option --statOnly, it will
#   remove all the systematics from all the datacards. With the option
#   --channels you can decide to use just one particular channel
#   instead of splitting in all channels. 
#   This script not only creates this working directory structure, but
#   launches also the limit calculation and make the plots. To decide
#   which steps you want to execute you can use the --create --run
#   --plot options. 
#   --lumiFactor permit to scale the workspace by a factor indicated
########################################################################
"
}

#--------------------
# directory where this script is located (used to be able to run the
# script from a different directory and call auxiliary programs
# in the script's directory)
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#--------------------

############################### OPTIONS
#------------------------------ default
WORKDIR=workdir
# file to be copied in the directories
FILES=$SCRIPTDIR/parallel
chPREFIX=ch1_
extraChannels="all"
LUMIFACTOR=1
MASS=125
MODEL=cVcF
EXPECTED=1
#------------------------------ parsing
commandLine=$@
# options may be followed by one colon to indicate they have a required argument
if ! options=$(getopt -u -o hw:d:n:sc:eo -l help,desc,workdir:datacard:,datacardName:,channels:,run,create,plot,statOnly,lumiFactor:,expected:,smooth,splitBin,splitProcess,split:,model: -- "$@")
    then
    # something went wrong, getopt will put out an error message for us
    exit 1
fi

set -- $options

while [ $# -gt 0 ]
  do
  case $1 in
      -h|--help) usage; exit 0;;
      --desc) desc; exit 0;;
      -w|--workdir) WORKDIR=$2; shift;;
      -d|--datacard) DATACARD=$2; shift;;
      -n|--datacardName) DATACARDNAME=$2;shift;;
      --run) RUN=y;;
      --create) CREATE=y;;
      --plot) PLOT=y;;
      -c|--channels) wantChannels="$wantChannels $2";  shift;;
      -s|--statOnly) STATONLY=-s ;;
      --lumiFactor) LUMIFACTOR=$2;shift ;;
      --splitBin) SPLIT="$SPLIT:bin";;
      --splitProcess) SPLIT="$SPLIT:process";;
      --split) SPLIT="$SPLIT:$2"; shift;;
      --model) MODEL=$2; shift;;
      -e) EXPECTED=1;;
      -o) EXPECTED=0;;
      --expected) EXPECTED=$2; shift;;
      --smooth) SMOOTH=y;;
      (--) shift; break;;
      (-*) echo "$0: error - unrecognized option $1" 1>&2; usage >> /dev/stderr; exit 1;;
      (*) break;;
  esac
  shift
done

echo "[OPTION] channels used: $wantChannels";

#------------------------------ check mandatory options
if [ -z "${DATACARD}" ];then
    echo "[ERROR] Datacard not defined: mandatory option" >> /dev/stderr
    usage >> /dev/stderr
    exit 1
elif [ ! -r "${DATACARD}" ];then
    echo "[ERROR] Datacard $DATACARD not found or not readable" >> /dev/stderr
    exit 1
fi

# list of the files containing the workspaces
shapeFiles=`grep shapes $DATACARD | awk '{for(col=1;col<NF+1;col++){if (match($col,"root")){print $col}}}' | sort | uniq`
# list of channels in the datacard
channels=`cat $DATACARD | grep -A1 bin | grep -B1 observation | head -1 | sed 's|bin||'`



# from command line you can choose a list of particular channels to run on
# otherwise use all the channels
wantChannels="$wantChannels $extraChannels"
if [ "`echo $SPLIT | grep -c bin`" != "0" ];then
    wantChannels="$wantChannels $channels"
fi
#if [ "`echo $SPLIT | grep -c process`" != "0" ];then
#fi

wantChannels="`echo $wantChannels |sed 's| |\n|' | sort|uniq`"
echo "Datacard name= ${DATACARDNAME:=${DATACARD}}"

if [ -n "$CREATE" ];then
    DATACARDNAME=`basename $DATACARDNAME`
    #------------------------------ create the working directory
    if [ ! -d "$WORKDIR" ];then
	mkdir $WORKDIR
        #------------------------------ copy the full datacard in the working directory
        # from now use the datacard in the working directory
	cp $DATACARD $WORKDIR/$DATACARDNAME
	DATACARD=$WORKDIR/${DATACARDNAME}  # from now the datacard is the copied one
        #------------------------------ workspaces
        # check the existance of the workspaces
	for file in $shapeFiles
	  do
	  if [ ! -r "$file" ];then
	      echo "[ERROR] workspace $file specified in the datacard not present in the current directory"
	      exit 1
	  fi
	done
	
	if [ "$LUMIFACTOR" != "1" ]; then
	    cd $SCRIPTDIR/../
	    for shapeFile in $shapeFiles
	      do
	      ./massInterpolator.py -i ResultScripts/`basename $shapeFile` -o ResultScripts/$WORKDIR/`basename $shapeFile .root`_x$LUMIFACTOR.root -I $MASS -O $MASS --doSmoothing -k $LUMIFACTOR &> ResultScripts/$WORKDIR/interpolator.log || exit 1
	      
	#./RescaleDatacardShapes.sh -l $LUMIFACTOR -d $DATACARD -m $MASS -w $WORKDIR || exit 1
	      mv ResultScripts/$WORKDIR/`basename $file .root`_x${LUMIFACTOR}_interpolated.root ResultScripts/$WORKDIR/`basename $file`
	    done
	    cd -
	    sed -i '/rate/ { s|[ ]1|6|g}' $WORKDIR/*/$DATACARDNAME
	else
	    cp $shapeFiles $WORKDIR/
	fi
	
    else
	#echo "[ERROR] Working dir $WORKDIR already exists" >> /dev/stderr
	#exit 1
	DATACARD=$WORKDIR/${DATACARDNAME}  # from now the datacard is the copied one
    fi
    echo $commandLine >> $WORKDIR/command.sh # keep trace of the command used to generate the workspace
    

    #--------------- split by channel
    # number of channels
    echo "Channels in datacard: $channels"
    for channel in $channels
      do
      excludeString="$excludeString --xc=${chPREFIX}$channel"
    done
    #echo $excludeString
    excludeChannel=`echo $excludeString`

    if [ -n "$SPLIT" ];then
	for channel in  $wantChannels 
	  do
	  case $channel in
	      all)
		  excludeChannel=""
		  ;;
	      *)
		  echo " - Using only channel $channel"
		  excludeChannel=`echo $excludeString | sed "s|--xc=${chPREFIX}$channel||"`
		  ;;
	  esac
	  if [ -e "$WORKDIR/$channel" ];then continue; fi # skip if exists
	  mkdir $WORKDIR/$channel
	  cd $WORKDIR/$channel
	  echo $excludeChannel
	  combineCards.py $STATONLY $excludeChannel ../`basename $DATACARD` > `basename $DATACARD`
	  cd -
	  cp $FILES $WORKDIR/$channel
	done
    else
	for channel in  $wantChannels 
	  do
	  case $channel in
	      all)
		  if [ -e "$WORKDIR/$channel" ];then continue; fi # skip if exists
		  mkdir $WORKDIR/$channel
		  cd $WORKDIR/$channel
		  combineCards.py $STATONLY ../`basename $DATACARD` > `basename $DATACARD`
		  cd -
		  cp $FILES $WORKDIR/$channel
		  ;;
	      *)
		  echo " - Adding channel $channel"
		  excludeChannel=`echo $excludeChannel | sed "s|--xc=${chPREFIX}$channel||"`
		  ;;
	  esac
	done
	if [ "`echo $wantChannels | wc -w`" != "1" ];then
	    channel=`echo $wantChannels | sed 's|[ ]all||;s| |_|g;'`
	    if [ ! -e "$WORKDIR/$channel" ];then 
		mkdir $WORKDIR/$channel
		cd $WORKDIR/$channel
		combineCards.py $STATONLY $excludeChannel ../`basename $DATACARD` > `basename $DATACARD`
		cd -
		cp $FILES $WORKDIR/$channel
	    fi
	fi
	wantChannels=`echo $wantChannels | sed 's| |_|g;s|_all| all|;s|all_|all |'`
    fi
    
fi


if [ -n "$RUN" ];then
    echo "#####"
    echo "## Starting the $MODEL MultiDimFit"
    if [ -z "$SPLIT" ];then
	wantChannels=`echo $wantChannels | sed 's| |_|g;s|_all| all|;s|all_|all |'`
    fi
    for channel in $wantChannels
      do
# this is not good, it's stressing the local machine, should be launched in batch
# -r 10: scanrange 10sigma -> variable limit from the model
      $SCRIPTDIR/multiDimFit.py -r 10 -w $WORKDIR/$channel/ -d $DATACARDNAME -m $MASS -M $MODEL -e $EXPECTED &> $WORKDIR/$channel/multiDimFit-$MODEL.log
#      rm $WORKDIR/$channel/higgsCombinecVcF125_grid[0-9].*.root
      rm $WORKDIR/$channel/combine_${MODEL}$MASS*.log
    done
    wait
fi


if [ -n "$PLOT" ];then
    if [ -z "$SPLIT" ];then
	wantChannels=`echo $wantChannels | sed 's| |_|g;s|_all| all|;s|all_|all |'`
    fi

    if [ ! -e "tmp/" ];then
	mkdir tmp/
    fi

    # hack because gSystem->AddIncludePath("$SCRIPTDIR") seems not to work in the following
    if [ ! -e "scripts/" ];then
	ln -s $SCRIPTDIR/scripts .
    fi

    cat > tmp/macroPlot.C <<EOF
{
  gROOT->ProcessLine(".L scripts/makeBands.cxx");
  //gROOT->LoadMacro("$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/plotting/bandUtils.cxx+");
  TFile *bands = new TFile("$WORKDIR/bands.root","RECREATE");

EOF
    for channel in  $wantChannels 
      do
      case $MODEL in 
	  cVcF)
	      cat >> tmp/macroPlot.C <<EOF
  readParamScan2D(bands, "${MODEL}_$channel", "$WORKDIR/$channel/higgsCombine${MODEL}${MASS}_grid.MultiDimFit.mH$MASS.root", "CV", "CF");
EOF
	      ;;
	  rVrF)
	      cat >> tmp/macroPlot.C <<EOF
  readParamScan2D(bands, "${MODEL}_$channel", "$WORKDIR/$channel/higgsCombine${MODEL}${MASS}_grid.MultiDimFit.mH$MASS.root", "RV", "RF");
EOF
	      ;;
	  rV)
	      cat >> tmp/macroPlot.C <<EOF
  readParamScan1D(bands, "${MODEL}_$channel", "$WORKDIR/$channel/higgsCombine${MODEL}${MASS}_grid.MultiDimFit.mH$MASS.root", "RV");
EOF
	      ;;      
      esac
    done
    
    
    cat >> tmp/macroPlot.C <<EOF
  bands->Close();
  }
EOF
    
    root -l -b -q tmp/macroPlot.C
    
    channelNUM=0
    for channel in $wantChannels 
      do
      let channelNUM=$channelNUM+1
    done
    
    cat > $WORKDIR/plot.C <<EOF
{
  gROOT->ProcessLine(".L scripts/makeBands.cxx");
  TCanvas *c = new TCanvas("c");
EOF
    case $channelNUM in
	1) divideX=1; divideY=1;;
	2) divideX=2; divideY=1;;
	3) divideX=3; divideY=1;;
#	3) divideX=2; divideY=2;;
	4) divideX=2;divideY=2;;
	5|6) divideX=3;divideY=2;;
	7|8|9) divideX=3; divideY=3;;
	10|11|12) divideX=3; divideY=4;;
    esac
    cat >> $WORKDIR/plot.C <<EOF
  gStyle->SetOptFit(0);
  c->Divide($divideX,$divideY);
  float zmax=3;
  float ytitleoffset=0.1;
  float ztitleoffset=0.7;
  float ytitlesize=0.5;
  float ztitlesize=0.5;
  TPaveText *pv = new TPaveText(0.45,0.5,0.7,0.9,"NDC");
EOF
    
    channelCount=0
    for channel in $wantChannels 
      do
      let channelCount=$channelCount+1
      cat >> $WORKDIR/plot.C <<EOF
  c->cd($channelCount);
EOF
    done
    
    case $MODEL in 
	cVcF)
	    cat >> $WORKDIR/plot.C <<EOF
  TString XaxisLabel="CV";
  TString YaxisLabel="CF";
EOF
	    ;;
	rVrF)
	    cat >> $WORKDIR/plot.C <<EOF
  TString XaxisLabel="RV";
  TString YaxisLabel="RF";
EOF
	    ;;
    esac
    case $MODEL in 
	cVcF|rVrF)
	    cat >> $WORKDIR/plot.C <<EOF
  TH2D *profile_${channel} = _file0->Get("${MODEL}_${channel}_th2");
  TGraph *best_${channel} =  _file0->Get("${MODEL}_${channel}_best");
  TList *contourList_${channel}_c68 = _file0->Get("${MODEL}_${channel}_c68");
  TList *contourList_${channel}_c95 = _file0->Get("${MODEL}_${channel}_c95");
  
  float area_${channel}= contourArea(profile_${channel},contourList_${channel}_c68);
  profile_${channel}->GetZaxis()->SetRangeUser(-0.001,zmax);
  profile_${channel}->Draw("colz");
  profile_${channel}->SetTitle("$channel");
  profile_${channel}->GetXaxis()->SetTitle(XaxisLabel);
  profile_${channel}->GetYaxis()->SetTitle(YaxisLabel);
  profile_${channel}->GetZaxis()->SetTitle("2*#Delta LL");
  profile_${channel}->GetYaxis()->SetTitleSize(ytitleoffset);
  profile_${channel}->GetYaxis()->SetTitleOffset(ytitlesize);
  profile_${channel}->GetZaxis()->SetTitleSize(ztitleoffset);
  profile_${channel}->GetZaxis()->SetTitleOffset(ztitlesize);
  
  //best_${channel}->Draw("P same");
EOF
	    ;;
	rV)
	    cat >> $WORKDIR/plot.C <<EOF
  TGraph *profile_${channel}_1d = _file0->Get("rV_${channel}_obs");
  profile_${channel}_1d -> Draw("AL");
  profile_${channel}_1d -> GetYaxis()->SetTitle("2*Delta LL");
  profile_${channel}_1d -> GetYaxis()->SetRangeUser(0,2);
  profile_${channel}_1d -> GetXaxis()->SetTitle("RV");
  profile_${channel}_1d -> GetXaxis()->SetRangeUser(0,2);
  profile_${channel}_1d -> Fit("pol2");
  profile_${channel}_1d -> Fit("pol2","","",0.5,1.5);
  TF1 *f_${channel}_1d = profile_${channel}_1d->GetFunction("pol2");
  pv->AddText(TString("#sigma = ")+TString::Format("%.2f",1./sqrt(2*f_${channel}_1d->GetParameter(2))));

EOF
	    ;;
    esac
    cat >> $WORKDIR/plot.C <<EOF
  pv->Draw();
  
  c->SaveAs("$WORKDIR/canvas.C");
  c->SaveAs("$WORKDIR/canvas.png");
  c->SaveAs("$WORKDIR/canvas.eps");
  }
EOF
fi
