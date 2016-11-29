#!/bin/bash

queue="8nh"
useAAA=0
version="005_trees"

puTarget=2.22e+03,7.42e+04,3.64e+05,7.57e+05,1.15e+06,1.62e+06,2.58e+06,1.23e+07,3.69e+07,7.59e+07,1.33e+08,1.88e+08,2.49e+08,3.3e+08,4.2e+08,4.97e+08,5.46e+08,5.6e+08,5.43e+08,5.02e+08,4.43e+08,3.7e+08,2.91e+08,2.12e+08,1.43e+08,8.98e+07,5.26e+07,2.9e+07,1.52e+07,7.61e+06,3.69e+06,1.74e+06,7.97e+05,3.54e+05,1.53e+05,6.52e+04,2.9e+04,1.47e+04,9.3e+03,7.29e+03,6.5e+03,6.15e+03,5.95e+03,5.8e+03,5.65e+03,5.49e+03,5.3e+03,5.09e+03,4.84e+03,4.58e+03

## for proc in IA OA; do 
for proc in IA; do 
    fggRunJobs.py --load sig_jobs_fiducial_${proc}.json -d sig_jobs_fiducial_${proc}_${version} -x cmsRun workspaceStd.py  \
	-H -D -P \
	useAAA=$useAAA puTarget=$puTarget doPdfWeights=True doSystematics=False dumpTrees=False dumpWorkspace=False \
	maxEvents=-1 -n 50 -q $queue &
done

wait
