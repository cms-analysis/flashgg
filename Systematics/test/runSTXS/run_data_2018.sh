export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load data_2018.json -d Data2018_21Nov19 --stage-to /eos/home-e/escott/HggLegacy/WorkspaceTest/Pass1/2018/Data/Raw/ -x cmsRun workspaceStd.py maxEvents=-1 -q workday -n 50 --no-copy-proxy dumpWorkspace=True doStageOne=True
