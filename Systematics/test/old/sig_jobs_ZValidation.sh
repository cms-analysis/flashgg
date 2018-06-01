# NB this command is specific to the configuration on lxplus and is not gaurenteed elsewhere
#outdir="/afs/cern.ch/work/s/sethzenz/ws/" # can't set absolute path on lsf because we're expecting to stage
queue="8nh"
useAAA=0
version="741_ZValidation"
fggRunJobs.py --load sig_jobs_ZValidation.json -d sig_jobs_$version -x cmsRun workspaceStd_ZValidation.py maxEvents=-1 -n 500 -q $queue -D -P useAAA=$useAAA doFiducial=False  puTarget=5.05e+03,2.41e+05,7.83e+05,1.74e+06,2.37e+06,3.41e+06,6.12e+06,2.43e+07,6.78e+07,1.45e+08,2.57e+08,4.06e+08,5.63e+08,7.06e+08,8.41e+08,9.54e+08,1.03e+09,1.06e+09,1.06e+09,1.02e+09,9.47e+08,8.51e+08,7.41e+08,6.19e+08,4.93e+08,3.72e+08,2.67e+08,1.82e+08,1.18e+08,7.18e+07,4.13e+07,2.24e+07,1.15e+07,5.57e+06,2.56e+06,1.12e+06,4.7e+05,1.92e+05,7.78e+04,3.3e+04,1.61e+04,9.87e+03,7.67e+03,6.92e+03,6.66e+03,6.56e+03,6.49e+03,6.4e+03,6.28e+03,6.12e+03