#! /usr/bin/python

import os
import sys, math
import ROOT

def reRunPValue(dir, i, mass, datacard):
    cmd="python ../ResultScripts/PValuesPerCat.py -s "+dir+"/part"+str(i)+" -o -p -m "+mass+" -d "+dir+"/part"+str(i)+"/"+datacard+".txt"
    print cmd
    output=os.system(cmd)

def reRunMu(dir, i, mass, datacard):
    bestcmd="python ../ResultScripts/PValuesPerCat.py -s "+dir+"/part"+str(i)+" -b -m "+str(mass)+" -d "+workingDir+"/part"+str(i)+"/"+datacard+".txt"
    print bestcmd
    output=os.system(bestcmd)
    

def readMus(filename):
    par = {}
    file = open(filename,"a+")
    lines = file.readlines()
    file.close()

    for l in lines:
        items = l.split("\n")[0].split()
        par[int(items[0])] = (float(items[1]), float(items[2]), float(items[3]))
    return par

bestPValues = {}
mus = {}
workingDir=sys.argv[1]
datacardName = sys.argv[2]
parA = readMus(workingDir+"/"+datacardName+"_bestfits.txt")

for i in xrange(int(sys.argv[3])):
    if (i not in parA.keys()):
        skipJob = False
        r = [123+0.5*k for k in range(9)]
        maxMassPValue = (0, 9999.)

        badFiles = ""
        for j in r:
            filename = workingDir+"/part"+str(i)+"/"+datacardName+"/higgsCombinePValue.ProfileLikelihood.mH"+str(j)+".log"
            if (not os.path.isfile(filename)):
                badFiles = badFiles + str(j)+","
            else:
                file = open(filename)
                lines = file.readlines()
                file.close()

                fileIsOK = False
                for l in lines:
                    if ("p-value" in l):
                        fileIsOK = True

                if (not fileIsOK):
                    badFiles = badFiles + str(j)+","

        if (badFiles != ""):
            print "Job ", i, ":", badFiles
            reRunPValue(workingDir, i, badFiles[:-1], datacardName)

        for j in r:
            filename = workingDir+"/part"+str(i)+"/"+datacardName+"/higgsCombinePValue.ProfileLikelihood.mH"+str(j)+".log"
            file = open(filename)
            lines = file.readlines()
            file.close()
    
            for l in lines:
                if ("p-value" in l):
                    pvalue = (float(l.split()[-1]))
                    if (pvalue < maxMassPValue[1]):
                        maxMassPValue = (j, pvalue)
        
        bestPValues[i]=maxMassPValue
        #print bestPValues[i]
        
        thismu={}
        filename = workingDir+"/part"+str(i)+"/"+datacardName+"_BestFit/higgsCombineTest.ChannelCompatibilityCheck.mH"+str(maxMassPValue[0])+".log"

        #print filename
        if (not os.path.isfile(filename)):
            print "Job: ", i
            reRunMu(workingDir, i, maxMassPValue[0], datacardName)
            
        file = open(filename)
        lines = file.readlines()
        file.close()
        
        fileIsOK = False
        for line in lines:
            if ("Nominal fit" in line):
                fileIsOK = True
                break

        if (not fileIsOK):
            print "Job :", i
            reRunMu(workingDir, i, maxMassPValue[0], datacardName)
        
        file = open(filename)
        lines = file.readlines()
        file.close()
        
        for line in lines:
            if "Nominal fit" in line:
                linelist=line.split()
                thismu["best"]=[linelist[5],linelist[6].split("/")]
            elif "Alternate fit" in line:
                linelist=line.split()
                thismu[linelist[-1]]=[linelist[4],linelist[5].split("/")]
        
        mus[i]=thismu 

mukeys=mus.keys()
mukeys.sort()
    
file = open(workingDir+"/"+datacardName+"_bestfits.txt","w")
for mukey in mukeys:
    muinfo=mus[mukey]
        
    file.write(str(mukey)+"\t"+str(muinfo["best"][0])+"\t"+str(bestPValues[mukey][0])+"\t"+str(bestPValues[mukey][1])+"\n")
    bestPValues[i]=maxMassPValue
file.close()
            

        
