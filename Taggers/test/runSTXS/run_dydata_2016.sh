export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load dydata_2016.json -d DYdata2016_3Dec19 --stage-to /eos/home-e/escott/HggLegacy/TrainingNtuples/Pass3/2016/DYdata/Raw/ -x cmsRun legacy_dumper.py maxEvents=-1 -q workday pujidWP=tight dumpJetSysTrees=False -n 50 --no-copy-proxy runOnZee=True
