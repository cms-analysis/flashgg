export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load bkg_2017.json -d Bkg2017_8Nov19 --stage-to /eos/home-e/escott/HggLegacy/TrainingNtuples/Pass3/2017/Bkg/Raw/ -x cmsRun stxs_dumper.py maxEvents=-1 runOnZee=False -q workday pujidWP=tight dumpJetSysTrees=False -n 50 --no-copy-proxy
