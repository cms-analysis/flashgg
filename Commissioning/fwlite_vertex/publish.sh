#!/bin/bash
export date_string=`date | sed -e "s| |_|g" -e "s|:|_|g" -e "s|,||g" -e "s|(UTC+0100)||g"`
echo $date_string
mkdir ~/www/higgs/flashgg/$date_string
ls ~/www/higgs/flashgg/$date_string
cp *.C *.root *.png index.html ~/www/higgs/flashgg/$date_string
echo "http://test-carrillo.web.cern.ch/test-carrillo/higgs/flashgg/$date_string"
