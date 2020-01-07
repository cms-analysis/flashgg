export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load zh_2017.json -d ZH2017_21Nov19 --stage-to /eos/home-e/escott/HggLegacy/WorkspaceTest/Pass1/2017/ZH/Raw/ -x cmsRun workspaceStd.py maxEvents=-1 -q workday -n 50 --no-copy-proxy  dumpWorkspace=True doStageOne=True doSystematics=True useParentDataset=True
