export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load data_2017.json -d Data2017_21Nov19 --stage-to /eos/home-e/escott/HggLegacy/WorkspaceTest/Pass1/2017/Data/Raw/ -x cmsRun workspaceStd.py maxEvents=-1 -q testmatch  -n 200 --no-copy-proxy dumpWorkspace=True doStageOne=True
