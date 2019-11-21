export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load sig_2018.json -d Sig2018_21Nov19 --stage-to /eos/home-e/escott/HggLegacy/WorkspaceTest/Pass1/2018/Sig/Raw/ -x cmsRun workspaceStd.py maxEvents=-1 -q testmatch -n 200 --no-copy-proxy  dumpWorkspace=True doStageOne=True doSystematics=True useParentDataset=True
