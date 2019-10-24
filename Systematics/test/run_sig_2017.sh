export X509_USER_PROXY=~/x509up_u72495
fggRunJobs.py --load sig_2017.json -d Sig2017_24Oct19 --stage-to /eos/home-e/escott/HggLegacy/WorkspaceTest/Pass0/2017/Sig/Raw/ -x cmsRun workspaceStd.py maxEvents=-1 -q testmatch useParentDataset=True -n 200 --no-copy-proxy  dumpWorkspace=True doStageOne=True doSystematics=True
