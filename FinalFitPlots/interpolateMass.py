#!/bin/env python

from sys import argv

target=argv[1]
sqrts = "8TeV"
if len(argv) > 2:
    sqrts = argv[2]


if ".root" in target:
    target = target.replace(".root","")
elif not "/" in target:
    target = "%s/CMS-HGG" % target


import ROOT
ROOT.gSystem.SetIncludePath("-I$ROOTSYS/include -I$ROOFITSYS/include")

ROOT.gSystem.Load("../libLoopAll.so")
ROOT.gROOT.LoadMacro("InterpolateMass.C+")
#if sqrts == "8TeV":
#    ROOT.gROOT.LoadMacro("InterpolateMass.C+")
#else:
#    ROOT.gROOT.LoadMacro("InterpolateMass7TeV.C+")

### ROOT.InterpolateMassRange(110,150,0.5,target)
import numpy
points = numpy.concatenate( (numpy.arange(110,123,0.5), numpy.arange(123,124.5,0.1), numpy.arange(124.5,125.5,0.02),
                             numpy.arange(125.5,126,0.1), numpy.arange(126,150.5,0.5) ) )
ROOT.InterpolateMassPoints(len(points),points,target,"",1,sqrts!="8TeV")
