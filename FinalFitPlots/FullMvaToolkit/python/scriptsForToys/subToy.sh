#!/bin/bash
#run with $1=$PWD $2=outDir

cd $1
outDir=$2

source /vols/cms/grid/setup.sh
eval `scramv1 runtime -sh`

cd ../
python writeBinnedMvaCard.py -i /vols/cms02/h2g/latest_workspace/CMS-HGG_jan16reload_fullmva.root_interpolated.root  --makePlot --throwGlobalToy --inputMassFacWS /vols/cms02/h2g/latest_workspace/CMS-HGG_massfacmva-13-04-2012.root --inputBdtPdf combToyWS.root --tmvaWeightsFolder /vols/cms02/h2g/weights/wt_Jan16reload_v2 -o $outDir - p $outDir --expSig 1
mv $outDir $1
mv $outDir-plots $1
cd $1
