# NB this command is specific to the configuration on lxplus and is not gaurenteed elsewhere
#outdir="/afs/cern.ch/work/s/sethzenz/ws/" # can't set absolute path on lsf because we're expecting to stage
###queue="all.q"
queue='long.q -l h_vmem=6g'
useAAA=0
#atIC=0 # only effect is setting eos path
version="data_differential_moriond17_test_OldCatalog"
LM=/mnt/t3nfs01/data01/shome/vtavolar/Flashgg/clean8025/CMSSW_8_0_25/src/flashgg/Systematics/test/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt
fggRunJobs.py --load DoubleEG_ReMiniAOD_2_5_Y.json   -d data_jobs_$version -x cmsRun workspaceStd.py maxEvents=-1 -n 200 -q "$queue" -H -D useAAA=$useAAA --no-copy-proxy  doFiducial=True acceptance=NONE lumiMask=${LM}