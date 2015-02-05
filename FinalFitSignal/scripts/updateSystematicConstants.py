import os

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--infile",help="Parametric model workspace")
parser.add_option("-o","--outfile",help="File for out ws")
parser.add_option("-d","--datfile",help="datfile with new systematic constants")
parser.add_option("-S","--sqrtS",type="int",help="com energy")
parser.add_option("--justPrint",default=False,action="store_true")
(options,args) = parser.parse_args()

import ROOT as r

r.gROOT.ProcessLine(".L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so")

inf = r.TFile.Open(options.infile)
ws = inf.Get('wsig_%dTeV'%options.sqrtS)

vars = ws.allVars()
it = vars.iterator()

if options.justPrint:
	for i in range(vars.getSize()):
		var = it()
		name = var.GetName()
		if 'const' in name:
			var.Print()

else:
	datf = open(options.datfile)

	for line in datf.readlines():
		line = line.strip('\n')
		if line.startswith('diphotonCat'):
			diphocat = line.split('=')[1]
		elif line.startswith('proc'):
			proc = line.split('=')[1]
		else:
			els = line.split()
			if len(els)!=4: continue
			systName = els[0]
			meanVal = float(els[1])
			sigmaVal = float(els[2])
			rateVal = float(els[3])
			meanName = 'const_%s_cat%s_mean_%s'%(proc,diphocat,systName)
			sigmaName = 'const_%s_cat%s_sigma_%s'%(proc,diphocat,systName)
			rateName = 'const_%s_cat%s_rate_%s'%(proc,diphocat,systName)

			var = ws.var(meanName)
			if var: var.setVal(meanVal)
			var = ws.var(sigmaName)
			if var: var.setVal(sigmaVal)
			var = ws.var(rateName)
			if var: var.setVal(rateVal)

	outf = r.TFile(options.outfile,'RECREATE')
	ws.Write()
	outf.Close()
