#!/bin/bash
#
# the purpose of this script is to wget dataset files 
# https://raw.githubusercontent.com/wamorkart/flashgg/HtoAAto4Gamma_TaggerDev/MetaData/data/H4GandHH4G_2016_27Sep2019/datasets_1.json
#https://github.com/cms-analysis/flashgg/tree/dev_legacy_runII/MetaData/data/Era2017_RR-31Mar2018_v2
#https://raw.githubusercontent.com/cms-analysis/flashgg/dev_legacy_runII/MetaData/data/Era2017_RR-31Mar2018_v2/datasets.json
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 
do
	wget https://raw.githubusercontent.com/cms-analysis/flashgg/dev_legacy_runII/MetaData/data/Era2017_RR-31Mar2018_v2/datasets_${i}.json
	#wget https://raw.githubusercontent.com/wamorkart/flashgg/HtoAAto4Gamma_TaggerDev/MetaData/data/H4GandHH4G_2016_27Sep2019/datasets_${i}.json
done

