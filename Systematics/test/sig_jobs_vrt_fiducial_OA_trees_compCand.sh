# NB this command is specific to the configuration on lxplus and is not gaurenteed elsewhere
#outdir="/afs/cern.ch/work/s/sethzenz/ws/" # can't set absolute path on lsf because we're expecting to stage
###queue='long.q -l h_vmem=6g'
queue='all.q'
useAAA=0
##atIC=0 # only effect is setting eos path
version="compCand_moriond17_tunedCats_OA_phoIdCorr_JetIDGen_v3"
fggRunJobs.py --load sig_jobs_vrt_fiducial_moriond17_OA.json -d sig_jobs_$version -x cmsRun workspaceStd.py --no-copy-proxy maxEvents=-1 -n 500 -q "$queue" -D -P useAAA=$useAAA doFiducial=True dumpWorkspace=False dumpTrees=True doSystematics=True puTarget=2.51e+05,1.15e+06,2.47e+06,3.72e+06,5.19e+06,6.79e+06,8.67e+06,2.31e+07,5.89e+07,1.38e+08,3.12e+08,5.71e+08,8.76e+08,1.21e+09,1.56e+09,1.87e+09,2.08e+09,2.19e+09,2.24e+09,2.28e+09,2.29e+09,2.24e+09,2.15e+09,2.03e+09,1.88e+09,1.71e+09,1.54e+09,1.36e+09,1.19e+09,1.01e+09,8.48e+08,6.94e+08,5.57e+08,4.38e+08,3.37e+08,2.53e+08,1.85e+08,1.31e+08,8.96e+07,5.87e+07,3.68e+07,2.2e+07,1.25e+07,6.75e+06,3.46e+06,1.68e+06,7.79e+05,3.44e+05,1.46e+05,6.13e+04,2.68e+04,1.33e+04,8.25e+03,6.3e+03,5.45e+03,4.97e+03,4.59e+03,4.25e+03,3.92e+03,3.58e+03,3.25e+03,2.93e+03,2.62e+03,2.33e+03,2.05e+03,1.79e+03,1.56e+03,1.34e+03,1.14e+03,969,815,680,563,463,378