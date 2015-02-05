#!/bin/env python

import sys, types, os
import commands
import numpy
from math import sqrt, log
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

def getPdf(icat, sqrts, ws):
    pdf = ws.pdf("sigpdfrelcat%d_allProcs" % icat )
    if pdf:
        return pdf
    cat = "cat%d_%s" % ( icat, sqrts )
    pdf = ws.pdf("extPdfshapeSig_hgg_%s" % cat)
    if not pdf:
        comps = ROOT.RooArgList()
        coeffs = ROOT.RooArgList()
        for proc in "ggH", "qqH", "ttH", "WH", "ZH":
            exp = ws.function("n_exp_binhgg_%s_proc_%s" % ( cat, proc ) )
            norm = ws.function("shapeSig_%s_hgg_%s__norm" % ( proc, cat ) )
            ppdf = ws.pdf("shapeSig_%s_hgg_%s" % ( proc, cat ))
            enorm = ROOT.RooProduct("shapeSig_%s_hgg_%s_norm" % ( proc, cat ), "shapeSig_%s_hgg_%s_norm" % ( proc, cat ), ROOT.RooArgList(exp,norm))
            comps.add(ppdf)
            coeffs.add(enorm)
            objs.append(enorm)
        pdf = ROOT.RooAddPdf("extPdfshapeSig_hgg_%s" % cat, "extPdfshapeSig_hgg_%s" % cat, comps, coeffs)
        getattr(ws,"import")(pdf)
    return pdf

def getExpect(dat,icat,sqrts,ws,ival,isyst):
    name = isyst.replace("CMS_hgg_nuisance_","")
    val = 0.
    if name in dat["global"]:
        if icat in dat["global"][name]:
            val = dat["global"][name][icat]
        else:
            val = 1.
    elif name in dat[icat]["ggh"]:
        sumw = 0.
        for mproc,cproc in [ ("ggh","ggH"), ("vbf","qqH"), ("tth","ttH"), ("wh","WH"), ("zh","ZH") ]:

            nname = "shapeSig_%s_hgg_cat%d_%s_norm" % ( cproc, icat, sqrts )
            wei = ws.function(nname).getVal()
            val += dat[icat][mproc][name]*wei
            sumw += wei
        val /= sumw
    return val*ival


def readDatFile(dat):
    fdat = open(dat)
    curcat = 0
    curproc = ""
    perproc = {}
    for line in fdat.read().split("\n"):
        if line.startswith("#"):
            continue
        print line
        if line.startswith("diphotonCat"):
            curcat = int(line.split("=")[1])
            if not curcat in perproc:
                perproc[curcat] = {}
            continue
        if line.startswith("proc"):
            curproc = str(line.split("=")[1])
            if not curproc in perproc[curcat]:
                perproc[curcat][curproc] = {}
            continue
        if line.startswith("global"):
            gscales = str(line.split("=")[1])
            scales = gscales.split(",")
            if not "global" in perproc:
                perproc["global"] = {}
            for scale in scales:
                if ":" in scale:
                    name, lst = scale.split(":",1)
                    specs = lst.split(":")
                    perproc["global"][name] = {}
                    for ispec in range(len(specs)/2):
                        perproc["global"][name][ int(specs[2*ispec]) ] = float(specs[2*ispec+1])
                else:
                    perproc["global"][scale] = {}

            continue
        toks = [ t for t in line.split(" ") if t != "" ]
        if len(toks) >= 2:
            print toks
            name = toks[0]
            val = float(toks[1])
            if val < 5.e-5:
                val = 0.
            elif val < 1.e-4 and "scale" in name:
                val = 1.e-4
            perproc[curcat][curproc][name] = val
    return perproc

# -----------------------------------------------------------------------------------------------------------
def main(options,args):

    allnuis = [ ("nominal",0.) ]
    
    fin = open(options.infile)        
    for line in fin.read().split("\n"):
        toks = [ t for t in line.replace("\t"," ").split(" ") if t != "" ]
        if len(toks) >= 3:
            allnuis.append( (toks[0],float(toks[3])) )

    print "All nuiscances"
    pprint( allnuis )
    print
    

    ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
    catvars = {}        
    expects = {}        
    diffs = {}        
    for ncat,sqrts,workspace,dat in zip(options.ncat,options.sqrts,options.workspace,options.dat):

        tin = ROOT.TFile.Open(workspace)
        ws = tin.Get("w")
        if not ws:
           ws = tin.Get("wsig_%s" % sqrts) 
        ws.var("MH").setVal(125.)
    
        roonuis = {}
        for isyst,ival in allnuis:
            v = ws.var(isyst)
            if v:
                roonuis[ isyst ] = v
        print roonuis
        
        perproc = None
        if dat != "":
            perproc = readDatFile(dat)
            pprint( perproc )
            
        for icat in range(ncat):
            pdf = getPdf(icat, sqrts, ws)
            pdf.Print("V")
            vars = {}
            expect = {}
            diff = {}
            for isyst,ival in allnuis:
                vars[ isyst ] = getMean(pdf,isyst,ival,roonuis)
                if dat:
                    expect[ isyst ] = getExpect(perproc,icat,sqrts,ws,ival,isyst)
                    
            nom = vars[ 'nominal' ]
            for isyst,ival in allnuis:
                vars[ isyst ] = ( vars[ isyst ] - nom ) / nom * 1.
                if dat:
                    diff[ isyst ] = expect[isyst] - vars[isyst]
            ## catvars["cat%02d_%s" % ( icat, sqrts) ] = vars
            catvars["%s_cat%02d" % ( sqrts, icat ) ] = vars
            expects["%s_cat%02d" % ( sqrts, icat ) ] = expect
            diffs["%s_cat%02d" % ( sqrts, icat ) ] = diff
            
            
    strc = json.dumps(catvars,indent=4)
    ## print strc
    fout = open(options.outfile,"w+")
    fout.write(strc)
    fout.close()

    strc = json.dumps(expects,indent=4)
    ## print strc
    fout = open("%s_expect.json" % options.outfile.replace(".json",""),"w+")
    fout.write(strc)
    fout.close()

    strc = json.dumps(diffs,indent=4)
    ## print strc
    fout = open("%s_diff.json" % options.outfile.replace(".json",""),"w+")
    fout.write(strc)
    fout.close()

        
if __name__ == "__main__":

    parser = OptionParser(option_list=[
            make_option("-i", "--infile",
                        action="store", type="string", dest="infile",
                        default="",
                        help="input file",
                        ),
            make_option("-w", "--workspace",
                        action="callback", callback=MyAppend(),  type="string", dest="workspace",
                        default=["/afs/cern.ch/user/m/musella/Analysis/CMGTools/H2G_6_1_2/src/h2gglobe_sigmod/SimultaneousSignalFitting/signal_model_exact_fix/CMS-HGG_mva_7TeV_sigfit_newsysts_exact.root",
                                 "/afs/cern.ch/user/m/musella/Analysis/CMGTools/H2G_6_1_2/src/h2gglobe_sigmod/SimultaneousSignalFitting/signal_model_exact_fix/CMS-HGG_mva_8TeV_sigfit_newsysts_exact.root"],
                        help="input file",
                        ),
            make_option("-n", "--ncat",
                        action="callback", callback=MyAppend(),  type="int", dest="ncat",
                        default=[11,14],
                        help="",
                        ),
            make_option("-s", "--sqrts",
                        action="callback", callback=MyAppend(),  type="string", dest="sqrts",
                        default=["7TeV","8TeV"],
                        help="",
                        ),
            make_option("-d", "--dat",
                        action="callback", callback=MyAppend(),  type="string", dest="dat",
                        default=["",""],
                        help="",
                        ),
            make_option("-o", "--outfile",
                        action="store", type="string", dest="outfile",
                        default="nuisvar.json",
                        help="output file",
                        )
            ])
    
    (options, args) = parser.parse_args()
    
    print "\n---------------------------------------------"
    print "Job options "
    pprint(options.__dict__)
    
    import ROOT
    print ROOT.gROOT.IsBatch()
    

    ws=main(options,args)
