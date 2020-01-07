export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load powheg_2018.json -d Powheg2018_21Nov19 --stage-to /eos/home-e/escott/HggLegacy/TrainingNtuples/Pass3/2018/Powheg/Raw/ -x cmsRun workspaceStd.py maxEvents=-1 -q workday -n 50 --no-copy-proxy  useParentDataset=True runOnZee=False pujidWP=tight dumpJetSysTrees=False
