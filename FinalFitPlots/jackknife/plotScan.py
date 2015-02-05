#!/usr/bin/env python

import math, ROOT

import sys

dir=sys.argv[1]

fin=open("%s/scan.csv" % dir)

##masses=[110,115,120,125,130,135,140,145,150]
##masses=[110,115,120,125,130,135,140,145,150]
masses=[120.,125.,130.]

h = ROOT.TH2F("deltaMuVsMass","deltaMu;m_{H};#delta #mu_{(i)};n psedo-experiments",9,107.5,152.5,1000,-5,5)

                                

sumDeltaMu   = {}
sumDeltaMuSq = {}
np = {}

reNormalizeAvg = False
reNormalizeTo = 125.
effAcc = None

for m in masses:
    sumDeltaMu[m] = 0.
    sumDeltaMuSq[m] = 0.
    np[m] = 0.
## n = 0
sumDeltaMu[-1] = 0.
sumDeltaMuSq[-1] = 0.
np[-1] = 0.


if reNormalizeAvg:
    ROOT.gSystem.Load("../../libLoopAll.so")
    reNormalizeAvg = ROOT.Normalization_8TeV()
    effAcc = ROOT.TGraphAsymmErrors(3);
    effAcc.SetName("");
    effAcc.SetTitle("");
    effAcc.SetFillColor(1);
    effAcc.SetLineWidth(2);
    effAcc.SetPoint(0,120,40.68102);
    effAcc.SetPointError(0,0,0,0,0);
    effAcc.SetPoint(1,125,41.95952);
    effAcc.SetPointError(1,0,0,0,0);
    effAcc.SetPoint(2,130,43.26737);
    effAcc.SetPointError(2,0,0,0,0);
    
    reNormalizeTo = reNormalizeAvg.GetXsection(reNormalizeTo)*reNormalizeAvg.GetBR(reNormalizeTo)*effAcc.Eval(reNormalizeTo)

    
for line in fin.read().split("\n"):
    line =line.lstrip().rstrip().replace("/",",")
    if line == "":
        continue

    vals = [ float(v) for v in line.split(",") if v != "" ]

    ## npoints = (len(vals)-1)/4
    npoints = (len(vals)-1)/3

    count = False
    for i in range(npoints):
        ## mass1 = vals[1+4*i]
        ## mu1   = vals[1+4*i+1]
        ## mass2 = vals[1+4*i+2]
        ## mu2   = vals[1+4*i+3]

        mass1 = vals[1+3*i]
        mu1   = vals[1+3*i+1]
        mass2 = vals[1+3*i]
        mu2   = vals[1+3*i+2]

        if mass1 != mass2:
            continue

        if mass1 in masses:
            if reNormalizeAvg:
                mu1 *= reNormalizeAvg.GetXsection(mass1)*reNormalizeAvg.GetBR(mass1)*effAcc.Eval(mass1)/reNormalizeTo
                mu2 *= reNormalizeAvg.GetXsection(mass1)*reNormalizeAvg.GetBR(mass1)*effAcc.Eval(mass1)/reNormalizeTo
        
            count = True
            deltaMu = mu1 - mu2
            h.Fill(mass1,deltaMu)

            sumDeltaMu[mass1] += deltaMu
            sumDeltaMuSq[mass1] += deltaMu*deltaMu

            sumDeltaMu[-1] += deltaMu
            sumDeltaMuSq[-1] += deltaMu*deltaMu

            np[mass1] += 1
            np[-1] += 1
            

avg = 0
print np
print sumDeltaMu
print sumDeltaMuSq
for m in masses:
    n = np[m]
    avgDelta = sumDeltaMu[m] / n
    varJkn = (n-1)/n * ( sumDeltaMuSq[m] - n*avgDelta*avgDelta )
    avg += math.sqrt(varJkn)
    
    print m, avgDelta, math.sqrt(varJkn)

avg /= len(masses)

print avg

### ROOT.gStyle.SetOptFit(1)
### c=ROOT.TCanvas()
### h.Fit("gaus")
### h.Draw()
### 
### ROOT.gStyle.SetOptFit(1)
### d=ROOT.TCanvas()
### hnev1.Draw("")
### hnev2.Draw("same")
### 
### for fmt in "C","png","pdf":
###     c.SaveAs("jackknife.%s" % fmt )
###     d.SaveAs("nevents.%s" % fmt )

