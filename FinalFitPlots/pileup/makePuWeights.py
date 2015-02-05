#!/bin/env python

# import ROOT with a fix to get batch mode (http://root.cern.ch/phpBB3/viewtopic.php?t=3198)
from sys import argv
argv.append( '-b-' )
import ROOT
ROOT.gROOT.SetBatch(True)
argv.remove( '-b-' )

import os, re
from pprint import pprint

from sampleDict import sampleString
from fsUtils import canonicalize

from optparse import OptionParser
parser = OptionParser(usage="usage: %prog target_pileup.root mc_pileup.root [mc_pileup2.root ...] \nrun with --help to get list of options")
parser.add_option("--output", default='weights.root', help="Weights ROOT file [default: %default].", metavar='FILE')
(options, args) = parser.parse_args()

if len(args) < 2:
    parser.print_usage()
    raise RunTimeError("Need at least 2 files.")

fNames = [canonicalize(x) for x in args]

dataPuFileName = fNames[0]
mcPuFileNames = fNames[1:]

from ROOT import TFile, gDirectory

def getPuHistos(tFile):
    fileObjs = list(tFile.GetListOfKeys())
    return [ tFile.Get(obj.GetName()) for obj in fileObjs if re.search('pileup', obj.GetName()) ]


dataPuFile = ROOT.TFile.Open(dataPuFileName)
dataPuHistosDict = dict( [(h.GetName(),h) for h in getPuHistos(dataPuFile)] )

fout = ROOT.TFile.Open(options.output,"recreate")
fout.cd()

for (name, hist) in dataPuHistosDict.iteritems():
    hist.Write('target_'+name)


def makeWeightHisto(dataOrig, mcOrig):

    #TODO: check that the axis dimensions are compatible
    if mcOrig.GetNbinsX()<dataOrig.GetNbinsX():
        nbins = mcOrig.GetNbinsX()
    elif mcOrig.GetNbinsX()>dataOrig.GetNbinsX():
        raise RuntimeException("Unhandled case: Gen PU in the samples has more bins than the Target PU one.\n Quitting.")
    else:
        nbins = False

    if nbins:
        import numpy as np
        data = dataOrig.Rebin(nbins,'',np.arange(0.0,nbins+1.0,1.0))
        mc = mcOrig.Rebin(nbins,'',np.arange(0.0,nbins+1.0,1.0))
    else:
        data = dataOrig
        mc = mcOrig
        
    w = data.Clone(data.GetName()+'_weights')
    w.SetTitle(w.GetName())
    w.Reset("ICE")
    w.SetEntries(0)

    #pprint( [x.GetXaxis().GetXbins().GetSize() for x in (dataOrig, mcOrig, data, mc)] )
                
    w.Divide( data, mc, 1./data.Integral(), 1./mc.Integral() )

#     eff = w.Clone('eff')
#     eff.Multiply( mc )
#     print "Unweighing efficiency: %1.3f; Weights integral %1.2f" % (eff.Integral() / mc.Integral(), w.Integral())

    return w
    
for mcPuFileName in mcPuFileNames:
    sampleName = sampleString(mcPuFileName)
    print sampleName
    
    mcPuFile = ROOT.TFile.Open(mcPuFileName)
    mcPuHistos = getPuHistos(mcPuFile)

    histToSave = list()
    for mcPuHisto in mcPuHistos:
        try:
            dataPuHisto = dataPuHistosDict[mcPuHisto.GetName()]
        except KeyError:
            continue

        histToSave += [ mcPuHisto, makeWeightHisto(dataPuHisto, mcPuHisto) ]

    if histToSave:
        fout.mkdir(sampleName)
        fout.cd(sampleName)
        for h in histToSave:
            h.Write()

    mcPuFile.Close()

fout.Close()


