# vim: sw=4 ts=4 expandtab
#!/usr/bin/env python

import sys

iname = sys.argv[1]
sqrtS = 8
if len(sys.argv)>2: 
	sqrtS = int(sys.argv[2])
sys.argv.append("-b")

import ROOT 
ROOT.gROOT.ProcessLine(".L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so")
## fin = ROOT.TFile.Open("moriond_parametric/CMS-HGG_sigfit_nonuis.root")
fin = ROOT.TFile.Open(iname)
fout = ROOT.TFile(iname.replace('.root','_nonuis.root'),'RECREATE')

wsig = fin.Get("wsig_%dTeV"%sqrtS)
vars = wsig.allVars()
it = vars.iterator()

for i in range( vars.getSize() ):
    var = it()
    name = var.GetName()
    if "nuisance" in name or 'globalscale' in name:
        var.setConstant(True)
        print name, var.isConstant()
    """
		if "nuis" in name or "globalscale" in name:
        print "here"
        var.setVal(0.)
        var.setConstant(True)
		if "MH" in name:
			print "here"
			var.setConstant(False)
		"""

for obj in fin.GetListOfKeys():
    ob = fin.Get(obj.GetName())
    if ob.Class().InheritsFrom(ROOT.TMacro.Class()):
        print ob.GetName()
        fout.cd()
        ob.Write()

fout.cd()
wsig.Write()

