#!/bin/env python

import sys, types, os
import commands
import numpy
from math import sqrt, log, fabs
import json
json.encoder.FLOAT_REPR = lambda o: format(o, '1.3g')

from optparse import OptionParser, make_option
from  pprint import pprint
from copy import deepcopy as copy

objs = []

class MyAppend:
    def __init__(self):
        self.cold = True
        
    def __call__(self,option, opt_str, value, parser, *args, **kwargs):
        target = getattr(parser.values,option.dest)
        if self.cold:
            del target[:]
        self.cold = False
        target.append(value)
                                                    
def getMean(pdf,nuis,val,allnuis):
            
    for nuname,nu in allnuis.iteritems():
        if nuname == nuis:
            nu.setVal(val)
        else:
            nu.setVal(0.)
            
    histo = pdf.createHistogram("CMS_hgg_mass")
    mean = histo.GetMean()
    histo.Delete()
    return mean

def setAliases(tree,variations):
    tot1= ""
    tot2= ""
    tree.SetAlias("energy1", "et1/cosh(eta1)")
    tree.SetAlias("energy2", "et2/cosh(eta2)")
    for name, defs in variations.iteritems():
        pho1cat, pho2cat, unc = defs
        ## print name, pho1cat, pho2cat, unc        
        tree.SetAlias("deltaE1%s" % name, "(%s)*%1.2g" % (pho1cat,unc) )
        tree.SetAlias("deltaE2%s" % name, "(%s)*%1.2g" % (pho2cat,unc) )
        ## tree.SetAlias("deltaM%s" % name, "(pho1%s*mass/energy1 + pho2%s*mass/energy2)*%1.3g" % ( name, name, unc*0.5 ) )
        ## tree.SetAlias("deltaM%s" % name, "sqrt((1.+(pho1%s)*(%1.3g))*(1+(pho2%s)*(%1.3g)))-1." % ( name, unc, name, unc) )
        
        if tot1 != "":
            tot1 += "+"
            tot2 += "+"
        tot1 += "deltaE1%s" % name
        tot2 += "deltaE2%s" % name
    tree.SetAlias("deltaE1",tot1)
    tree.SetAlias("deltaE2",tot2)
    
    
def readcats(fname):
    fin = open(fname)
    variations = {}
    for line in fin.read().split("\n"):
        if line.startswith("#"): continue
        toks = [ t for t in line.replace("\t"," ").split(" ") if t != "" ]
        if len(toks) > 0:
            name   = toks.pop(0)
            toks.pop(0)
            mineta = float(toks.pop(0))
            maxeta = float(toks.pop(0))
            minr9  = float(toks.pop(0))
            maxr9  = float(toks.pop(0))
            toks.pop(0)
            toks.pop(0)
            toks.pop(0)
            unc    = float(toks.pop(0))

            pho1cat = "(abs(eta1) >= %1.3g && abs(eta1) < %1.3g && r91 >= %1.3g && r91 < %1.3g)" % ( mineta, maxeta, minr9, maxr9  )
            pho2cat = "(abs(eta2) >= %1.3g && abs(eta2) < %1.3g && r92 >= %1.3g && r92 < %1.3g)" % ( mineta, maxeta, minr9, maxr9  )
            variations[name] = ( pho1cat, pho2cat, unc )
            
    return variations

# -----------------------------------------------------------------------------------------------------------
def main(options,args):

    all = []
    for inf,nuis in zip(options.infile,options.nuisances):
        variations = readcats(inf)
        nuisances  = readcats(nuis)
        all.append( (variations,nuisances) )

    procs = ["ggh", "vbf", "wzh", "tth"]
    ## procs = [ "vbf" ]
    procvars = {}
    for fname in args:
        tin = ROOT.TFile.Open(fname)
        
        for proc in procs:
            tree = tin.Get("%s_m125_%s" % ( proc, options.sqrts ) )
            objs.append(tree)
            
            for variations,nuisances in all:
                aliases = setAliases(tree,variations)
            
                for name,defs in nuisances.iteritems():
                    pho1, pho2, dum = defs
                    ## print pho1, pho2
                    ## tree.Scan("deltaE1:deltaE2:issyst:name1")
                    ## tree.Draw("deltaM*(%s || %s):full_cat>>hshift(15,-0.5,14.5,1001.,-1.0001e-2,1.0001e-2)" % (pho1, pho2),"full_weight","goff")
                    tree.Draw("sqrt( (1+deltaE1*(%s))*(1+deltaE2*(%s)) )-1:full_cat>>hshift(15,-0.5,14.5,1001.,-1.0001e-2,1.0001e-2)" % (pho1, pho2),"full_weight","goff")
                    h = ROOT.gDirectory.Get("hshift")
                    prf = h.ProfileX("%s_%s" % ( proc, name ) )
                    objs.append(prf)
                    ## print name
                    ## prf.Print("all")
                    h.Delete()                    
                    procvars["%s_%s" % ( proc, name )] = prf

    for cat in range(14):
        for proc in procs:
            print "diphotonCat=%d" % cat
            print "process=%s" % proc
            for variations,nuisances in all:
                for name,defs in nuisances.iteritems():
                    prf,prj = procvars["%s_%s" % ( proc, name )]
                    var = fabs(prf.GetBinContent(cat+1))
                    wei = prj.GetBinContent(cat+1)
                    print name, "%1.3g +- %1.3g" % ( var, prf.GetBinError(cat+1) )
            print
        print

    
if __name__ == "__main__":

    parser = OptionParser(option_list=[
            make_option("-i", "--infile",
                        action="append", type="string", dest="infile",
                        default=[],
                        help="input file",
                        ),
            make_option("-n", "--nuisances",
                        action="append", type="string", dest="nuisances",
                        default=[],
                        help="input file",
                        ),
            make_option("-s", "--sqrts",
                        action="store", type="string", dest="sqrts",
                        default="8TeV",
                        help="",
                        ),
            make_option("-o", "--outfile",
                        action="store", type="string", dest="outfile",
                        default="nuisvar.json",
                        help="output file",
                        )
            ])
    
    (options, args) = parser.parse_args()
    
    ## print "\n---------------------------------------------"
    ## print "Job options "
    ## pprint(options.__dict__)
    
    import ROOT
    ## print ROOT.gROOT.IsBatch()
    

    ws=main(options,args)
