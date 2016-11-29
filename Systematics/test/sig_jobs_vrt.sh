# NB this command is specific to the configuration at IC and is not gaurenteed elsewhere
#fggRunJobs.py --load sig_jobs.json -q 1nd -d signal_120_125_130 -n 500  -D -P  -x cmsRun workspaceStd.py maxEvents=100 
## fggRunJobs.py -q all.q --load sig_jobs_vrt.json  -d signal_120_125_130 -n 500  -D -P  -x cmsRun workspaceStd.py maxEvents=100


# NB this command is specific to the configuration on lxplus and is not gaurenteed elsewhere
#outdir="/afs/cern.ch/work/s/sethzenz/ws/" # can't set absolute path on lsf because we're expecting to stage
queue="1nh"
useAAA=0
version="fiducial_IA_001"
puTarget=2.22e+03,7.42e+04,3.64e+05,7.57e+05,1.15e+06,1.62e+06,2.58e+06,1.23e+07,3.69e+07,7.59e+07,1.33e+08,1.88e+08,2.49e+08,3.3e+08,4.2e+08,4.97e+08,5.46e+08,5.6e+08,5.43e+08,5.02e+08,4.43e+08,3.7e+08,2.91e+08,2.12e+08,1.43e+08,8.98e+07,5.26e+07,2.9e+07,1.52e+07,7.61e+06,3.69e+06,1.74e+06,7.97e+05,3.54e+05,1.53e+05,6.52e+04,2.9e+04,1.47e+04,9.3e+03,7.29e+03,6.5e+03,6.15e+03,5.95e+03,5.8e+03,5.65e+03,5.49e+03,5.3e+03,5.09e+03,4.84e+03,4.58e+03

fggRunJobs.py --load sig_jobs_vrt_fiducial.json -d sig_jobs_$version -x cmsRun workspaceStd.py maxEvents=1 -D -P useAAA=$useAAA doFiducial=True  puTarget=$puTarget

# -n 500 -q $queue 