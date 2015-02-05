#!/bin/env python

import sys
#----------------------------------------------------------------------
def usage():
    print >> sys.stderr,"""

    usage: getRegressionNuisances.py nonparametric_input_file.root systematic_name

    extracts systematic uncertainty for the parametric model
    from non-parametric workspaces

    Specify 'all' as the systematic uncertainty to find the systematic
    shifts in the input file and run over all of them.

    """
    sys.exit(1)


#----------------------------------------------------------------------

def getListOfSystematicSources(rootFile):
    """ given a TFile object, returns a list of names of systematic
    sources """

    retval = set()
    import re
    
    for key in rootFile.GetListOfKeys():
        name = key.GetName()

        mo = re.match("th1f_sig_[^_]+_mass_m[^_]+_cat[^_]+_([^_]+)Up01_sigma$", name)
        if not mo:
            continue

        retval.add(mo.group(1))

    return sorted(list(retval))
        
#----------------------------------------------------------------------
# main
#----------------------------------------------------------------------
mass = 125
ARGV = sys.argv[1:]

if len(ARGV) != 2:
    usage()
    
fin = sys.argv[1]
syst = sys.argv[2]

sys.argv.append("-b")
import ROOT
ROOT.gROOT.ProcessLine(".L quadInterpolate.C+")
from ROOT import quadInterpolate


procs = [ "ggh", "vbf", "wzh", "tth" ]
ncat  = 9
ntot  = 9

file = ROOT.TFile.Open(fin)

if syst == 'all':
    allSyst = getListOfSystematicSources(file)
else:
    allSyst = [ syst ]

for syst in allSyst:
    nuis = {}
    for cat in range(ncat):
        nuis[cat]={}
        for proc in procs:
            nominal = file.Get("th1f_sig_%s_mass_m%d_cat%d" % (proc,mass, cat) )
            up      = file.Get("th1f_sig_%s_mass_m%d_cat%d_%sUp01_sigma" % (proc, mass, cat,syst) )
            down    = file.Get("th1f_sig_%s_mass_m%d_cat%d_%sDown01_sigma" % (proc,mass, cat,syst) )

            if nominal.Integral() != 0:
                downE = quadInterpolate(1.,-3.,0.,3.,down.Integral(),nominal.Integral(),up.Integral())
                upE = quadInterpolate(-1.,-3.,0.,1.,down.Integral(),nominal.Integral(),up.Integral())
                nuis[cat][proc] = (downE,upE) 
            else:
                nuis[cat][proc] = (1.,1.)

    print syst, '  lnN  ',  
    for cat in range(ncat):
        for proc in procs:
            print "%1.3f/%1.3f " % nuis[cat][proc],
        print "-     ",

    for cat in range(ncat,ntot):
        for proc in procs:
            print "%1.3f/%1.3f " % (1.,1.),
        print "-     ",

    print

