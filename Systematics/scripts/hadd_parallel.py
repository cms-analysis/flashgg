#!/usr/bin/python

import sys, getopt
import os
import multiprocessing
from multiprocessing import Pool
import math

ncpu = 8

def haddws(filelist):
    ex_line = " ".join(filelist)
    ex_line = "hadd_workspaces "+str(ex_line)
    print ex_line
    os.system(ex_line)
    return 1

def splitList(filelist, module, prefix, suffix):
    if len(filelist)<=1 :
        print "List of files has less two elements, there is nothing to hadd..."
        return []
    splitlist=[]
    print len(filelist)
    print module
    num_elem = int(math.ceil(float(len(filelist))/float(module))) if int(math.ceil(float(len(filelist))/float(module))) != 1 else  module
    print "# of elements per core "+str(num_elem)
    num_cores = int(math.ceil(float(len(filelist))/float(num_elem)))
    for i in range(int(num_cores)):
        templist=[]
        templist.append(str(prefix)+str(i)+str(suffix))
        templist = templist + filelist[i*num_elem:(i+1)*num_elem]
        print templist
        splitlist.append(templist)
#    templist=[]
#    templist.append(str(prefix)+str(module)+str(suffix))
#    templist = templist + filelist[num_elem*module:]
#    splitlist.append(templist)
    return splitlist


def main(argv):
    outputfile = argv[0]
    inputfiles = argv[1:]
    splitlist = splitList(inputfiles, ncpu, "haddTemp1stOrder_", ".root")

    print splitlist
    pool = multiprocessing.Pool(ncpu)
    print argv
    pool.map(haddws,splitlist)
    k=2
    while(len(splitlist)>1):
        newlist=[]
        for ls in splitlist:
            newlist.append(ls[0])
        print "new list for hadd is"
        print newlist
        splitlist = splitList(newlist, ncpu, "haddTemp"+str(k)+"thOrder_", ".root")
        pool.map(haddws, splitlist)
        k=k+1
    
    mvline = "mv haddTemp"+str(k-1)+"thOrder_0.root "+str(outputfile)
    os.system(mvline)
    rmline = "rm haddTemp*Order*.root"
    os.system(rmline)

        

if __name__ == "__main__":
   main(sys.argv[1:])
