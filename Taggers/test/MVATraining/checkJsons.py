import sys
import os

sys.path.append('/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_26/bin/slc6_amd64_gcc530/')

f = open('dataset_list.txt')
datasets=[]
for d in f:
    datasets.append(d.rstrip())

##for ids in range(len(datasets)):
##    thisd = datasets[ids]
##    print thisd
##    for jds in range(len(datasets)):
##        if jds == ids: continue
##        otherd = datasets[jds]
##        print otherd
###        exec("/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_26/bin/slc6_amd64_gcc530/compareJSON.py --and "+str(thisd)+" "+str(otherd))
##        os.system("compareJSON.py --and "+str(thisd)+" "+str(otherd))



for ids in range(len(datasets)):
    thisd = datasets[ids]
    if ids ==0:
        nextd = datasets[ids+1]
    else:
        nextd = "temp_"+str(ids-1)+".json"
    os.system("compareJSON.py --or "+str(thisd)+" "+str(nextd)+" temp_"+str(ids)+".json")
