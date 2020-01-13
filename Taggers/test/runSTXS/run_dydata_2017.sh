export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load dydata_2017.json -d DYdata2017_3Dec19 --stage-to /eos/home-e/escott/HggLegacy/TrainingNtuples/Pass3/2017/DYdata/Raw/ -x cmsRun stxs_dumper.py maxEvents=-1 -q workday pujidWP=tight dumpJetSysTrees=False -n 50 --no-copy-proxy runOnZee=True
