#! /usr/bin/python

import sys, math
import ROOT

def readMus(filename):
    par = {}
    file = open(filename)
    lines = file.readlines()
    file.close()

    for l in lines:
        items = l.split("\n")[0].split()
        par[int(items[0])] = (float(items[1]), float(items[2]), float(items[3])) 
    return par

parA = readMus(sys.argv[1])
parB = readMus(sys.argv[2])

matches= set(parA.keys()).intersection(  set(parB.keys()) )
nExp=len(matches)


mass1 = ROOT.TH1F("mass1", "mass 1", 20, 120., 130.)
mass2 = ROOT.TH1F("mass2", "mass 2", 20, 120., 130.)
pvalue = ROOT.TH1F("pvalue", "pvalue", 100, -0.05, 0.05)

sumMu = 0
rms = 0
sumMu1 = 0
rms1 = 0
sumMu2 = 0
rms2 = 0
for i in matches:
    sumMu = sumMu + (parA[i][0] - parB[i][0])
    sumMu1 = sumMu1 + (parA[i][0])
    sumMu2 = sumMu2 + (parB[i][0])
    mass1.Fill(parA[i][1])
    mass2.Fill(parB[i][1])
avgMu = sumMu/float(nExp)
avgMu1 = sumMu1/float(nExp)
avgMu2 = sumMu2/float(nExp)
for i in matches:
    rms = rms + math.pow(avgMu-(parA[i][0] - parB[i][0]), 2)
    rms1 = rms1 + math.pow((avgMu1-parA[i][0]), 2)
    rms2 = rms2 + math.pow((avgMu2-parB[i][0]), 2)
    
rms   = math.sqrt(rms/float(nExp))
rms1  = math.sqrt(rms1/float(nExp))
rms2  = math.sqrt(rms2/float(nExp))
rfac=6.

mu = ROOT.TH1F("sigmadmu", "#delta#mu_{Average}-#delta#mu", 50, float(-rfac*rms), float(rfac*rms))
dmu = ROOT.TH1F("dmu", "#delta#mu", 50,  float(avgMu-rfac*rms), float(avgMu+rfac*rms))
mu1 = ROOT.TH1F("mu", "#mu1", 50, float(avgMu1-rfac*rms1), float(avgMu1+rfac*rms1))
mu2 = ROOT.TH1F("mu", "#mu2", 50, float(avgMu2-rfac*rms2), float(avgMu2+rfac*rms2))
muaveMmu1 = ROOT.TH1F("muaveMmu1", "#mu_{average}-#mu_{1}", 50, float(-rfac*rms1), float(rfac*rms1))
    
for i in matches:
    mu.Fill(avgMu - (parA[i][0] - parB[i][0]))
    dmu.Fill((parA[i][0] - parB[i][0]))
    mu1.Fill(parA[i][0])
    mu2.Fill(parB[i][0])
    muaveMmu1.Fill(avgMu1-parA[i][0])
    
c2 = ROOT.TCanvas("c2", "c2", 1000, 1000)
c2.Divide(2,1)
c2.cd(1)
mu1.Draw()
mu1.Fit("gaus")
c2.cd(2)
muaveMmu1.Draw()
muaveMmu1.Fit("gaus")
c2.SaveAs("mu_.pdf")

c3 = ROOT.TCanvas("c3", "c3", 1000, 1000)
c3.Divide(2,1)
c3.cd(1)
mass1.Draw()
c3.cd(2)
mass2.Draw()
c3.SaveAs("mass.pdf")

c = ROOT.TCanvas("c", "c", 1000, 1000)
c.Divide(2,2)
c.cd(1)
mu1.Draw()
mu1.Fit("gaus")
c.cd(2)
mu2.Draw()
mu2.Fit("gaus")
c.cd(3)
dmu.Draw()
dmu.Fit("gaus")
c.cd(4)
mu.Draw()
mu.Fit("gaus")
c.SaveAs("dmu_.pdf")


print "Average  Mu1   : %1.3f" % (avgMu1)
print "By hand  rms1  : %1.4f" % (rms1)
print "Average  Mu2   : %1.3f" % (avgMu2)
print "By hand  rms2  : %1.4f" % (rms2)
print "Average  dMu   : %1.3f" % (avgMu)
print "By hand  rms   : %1.4f" % (rms)
print "FromHist rms   : %1.4f" % (mu.GetRMS())
print "By hand  s(dMu): %1.4f" % (float(nExp-1)/math.sqrt(float(nExp))*rms)
print "FromHist s(dMu): %1.4f" % (float(nExp-1)/math.sqrt(float(nExp))*mu.GetRMS())
print "Matchd experiments: %d" % nExp

raw_input()

