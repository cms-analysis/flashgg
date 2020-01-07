export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load sig_2016.json -d Sig2016_21Nov19 --stage-to /eos/home-e/escott/HggLegacy/TrainingNtuples/Pass3/2016/Sig/Raw/ -x cmsRun workspaceStd.py maxEvents=-1 -q workday -n 50 --no-copy-proxy useParentDataset=True runOnZee=False pujidWP=tight dumpJetSysTrees=False
