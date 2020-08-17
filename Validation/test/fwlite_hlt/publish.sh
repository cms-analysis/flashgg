#!/bin/bash
root -b make_up.C
export date_string=`date | sed -e "s| |_|g" -e "s|:|_|g" -e "s|,||g" -e "s|(UTC+0100)||g"`
echo $date_string
mkdir ~/www/higgs/flashgg/hlt/$date_string
ls ~/www/higgs/flashgg/hlt/$date_string
cp -rf *.C *.root plots index.html ~/www/higgs/flashgg/hlt/$date_string
echo "http://test-carrillo.web.cern.ch/test-carrillo/higgs/flashgg/hlt/$date_string"
