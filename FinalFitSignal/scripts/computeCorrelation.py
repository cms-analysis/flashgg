#!/usr/bin/env python

import sys
import json
from csv import DictReader

from math import sqrt
from pprint import pprint 
import numpy

from copy import deepcopy as copy

objs = []

def draw(mtx,name,catnames):
    
    ncat = len(catnames)
    
    h = ROOT.TH2F(name,name,ncat,0,ncat,ncat,0,ncat)
    h.SetMarkerSize(2)
    h.GetZaxis().SetRangeUser(0.,1.)
    for icat in range(ncat):  
        h.GetXaxis().SetBinLabel(icat+1,catnames[icat])
        h.GetYaxis().SetBinLabel(ncat-icat,catnames[icat])
        for jcat in range(ncat):  
            h.Fill(icat,jcat,mtx[icat][ncat-jcat-1])
    
    canv = ROOT.TCanvas(h.GetName(),h.GetName())
    canv.cd()
    h.Draw("colz")
    objs.extend([h,canv])

    for fmt in ['png']:
        canv.SaveAs("%s.%s" % (canv.GetName(), fmt) )
    
def cook(inp):
    out = copy(inp)
    
    for irow,row in enumerate(out):
        for icol,cell in enumerate(row):
            if cell < 0.05:
                out[irow][icol] = 0.
            else:
                out[irow][icol] = round(cell/0.05)*0.05
    return out

def readInput(fname):
    
    fin = open(fname)
    
    catvars = json.loads( fin.read() )

    systs = []
    categories = {}
    for cat,catvar in catvars.iteritems():
        if len(systs) == 0:
            systs = sorted([ s for s in catvar.keys() if s != "nominal" and not "smear" in s])

        categories[cat] = []
        for syst in systs:
            isys = float(catvar[syst])
            categories[cat].append( isys )
    print systs
    print categories

    catnames = sorted(categories.keys())
    ncat = len(catnames)

    ediff = [ [0. for i in range(ncat) ] for j in range(ncat) ]
    etot  = [ 0. for j in range(ncat) ]

    for icat,cat in enumerate(catnames):
        for isys,sys in enumerate(categories[cat]):
            etot[icat] += sys*sys
            for jcat,ocat in enumerate(catnames):
                corr = 0.
                if systs[isys] != 'stat' or icat == jcat:
                    corr = 1.
                osys = categories[ocat][isys]
                ediff[icat][jcat] += sys*sys + osys*osys   - 2.*corr*sys*osys
        etot[icat] = sqrt(etot[icat])

    pprint( catvars )
    ncats = len(categories)
    nnuis = len(systs)
    h2 = ROOT.TH2F("kmatrix","kmatrix",nnuis,-0.5,nnuis-0.5,ncats,-0.5,ncats-0.5)
    for icat,cat in enumerate(catnames):
        h2.GetYaxis().SetBinLabel(icat+1,cat )
    for inuis,nuis in enumerate(systs):
        h2.GetXaxis().SetBinLabel(inuis+1,nuis.replace("CMS_hgg_nuisance_",""))
    for icat,cat in enumerate(catnames):
        for inuis,nuis in enumerate(systs):
            h2.Fill(inuis,icat,catvars[cat][nuis]*1.e+3*125.)
    c = ROOT.TCanvas("kmatrix","kmatrix")
    c.cd()
    h2.Draw("colz")
    c.SaveAs("kmatrix.png")
    objs.extend( [h2,c] )
    
    print catnames

    print numpy.matrix(ediff)
    print numpy.array(etot)
    
    corr = copy(ediff)

    for icat in range(ncat):
        for jcat in range(ncat):
            ed = ediff[icat][jcat]
            ei = etot[icat]**2.
            ej = etot[jcat]**2.
            ## corr[icat][jcat] = round( 100. * ( 0.5*( ei + ej - ed ) / sqrt(ei*ej) ) )/100.
            corr[icat][jcat] = ( 0.5*( ei + ej - ed ) / sqrt(ei*ej) )
            
    print numpy.matrix(corr)
    draw(corr,'computed',catnames)
    
if __name__ == "__main__":

    ## sys.argv.append("-b")
    import ROOT
    ROOT.gStyle.SetOptStat(0)

    red    = numpy.array( [ 1.00, 0.00] )
    green  = numpy.array( [ 1.00, 0.00] )
    blue   = numpy.array( [ 1.00, 1.00] )
    length = numpy.array( [ 0.00, 1.00] )
    number = len(red)
    nb=255
    ROOT.TColor.CreateGradientColorTable(number,length,red,green,blue,nb)
    
    readInput(sys.argv[1])

    
