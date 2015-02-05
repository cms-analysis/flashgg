#!/bin/usr/python
import sys

filename=sys.argv[1]

inputfile=open(filename)

lines=inputfile.readlines()

for iline in xrange(len(lines)):
    etalow= (lines[iline].split("-absEta")[1]).split("_")[1]
    etahigh=(lines[iline].split("-absEta")[1]).split("_")[2].split("-")[0]
    lines[iline]=lines[iline].replace("runNumber\t","")
    lines[iline]=lines[iline].replace("gold","Gold\t0.94\t999")
    lines[iline]=lines[iline].replace("bad","Bad\t-999\t0.94")
    lines[iline]=lines[iline].replace("-absEta_1.566_2-","lowEta")
    lines[iline]=lines[iline].replace("-absEta_0_1-","lowEta")
    lines[iline]=lines[iline].replace("-absEta_1_1.4442-","highEta")
    lines[iline]=lines[iline].replace("-absEta_2_2.5-","highEta")
    lines[iline]=lines[iline].replace("\n","")
    
    
    parts=lines[iline].split()
    if len(parts)>6:
        conversion=1-float(parts[5])
        print parts[0],"\t0\t",etalow,"\t",etahigh,"\t",parts[1],"\t",parts[2],"\t",parts[3],"\t",parts[4],"\t",conversion,"\t",parts[6]
