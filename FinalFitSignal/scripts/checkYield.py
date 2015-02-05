#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i","--infile")
parser.add_option("-p","--procs",default="ggh,vbf,wh,zh,tth")
parser.add_option("-c","--cats",default="0,1,2,3,4,5,6,7,8,9,10")
parser.add_option("--is2011",default=False,action="store_true")
parser.add_option("-o","--outname",default="effAccCrossCheck")
(options,args)=parser.parse_args()

import ROOT as r

r.gROOT.ProcessLine(".L lib/libSimultaneousSignalFit.so")

tf = r.TFile(options.infile)

sqrtS = 7 if options.is2011 else 8
ws = tf.Get('wsig_%dTeV'%sqrtS)
MH = ws.var('MH')
intL = ws.var('IntLumi')

MH.Print()
intL.Print()

print 'The output herein should be compared to the output of makeEffAcc.py'

tg = r.TGraph()


for p,m in enumerate(range(110,151,1)):
	MH.setConstant(False)
	MH.setVal(m)
	MH.setConstant(True)
	printLine = 'Signal m%3d.0: '%m
	sumTot = 0.
	for cat in [int(x) for x in options.cats.split(',')]:
		sumProc = 0.
		procLine = 'cat %d, mH=%3d.0: '%(cat,m)
		for proc in options.procs.split(','):
			normFunc = ws.function('hggpdfsmrel_%s_cat%d_norm'%(proc,cat))
			eaFunc = ws.function('fea_%s_cat%d'%(proc,cat))
			procLine += '   %s %.5f'%(proc,100.*eaFunc.getVal())
			sumProc += normFunc.getVal()*intL.getVal()
			sumTot += normFunc.getVal()*intL.getVal()
		print procLine
		printLine += '%3.5f '%sumProc
	printLine += 'tot=%3.5f'%sumTot
	print printLine
	effAccDenom=1.
	fbr = ws.function('fbr')
	effAccDenom *= fbr.getVal()
	xs = 0.
	for proc in options.procs.split(','):
		fxs = ws.function('fxs_%s'%proc)
		xs += fxs.getVal()
	effAccDenom *= xs
	effAccDenom *= intL.getVal()
	tg.SetPoint(p,m,100.*sumTot/effAccDenom)

r.gROOT.SetBatch()
canv = r.TCanvas()
tg.SetLineWidth(3)
tg.Draw("AL")
canv.Update()
canv.Modified()
canv.Print("%s.pdf"%options.outname)
canv.Print("%s.png"%options.outname)

tf.Close()
	
