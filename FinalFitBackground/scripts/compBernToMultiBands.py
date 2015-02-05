import os

import sys

import ROOT as r
r.gROOT.ProcessLine(".L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so")
r.gROOT.SetBatch()

legHigh = [False,True,True,False,False,True,False,False,False,True,True,True,True,True,True]

def addToSum(graphMulti,graphBern,sumPointsMulti,sumPointsBern):
	
	xM = r.Double(0.)
	yM = r.Double(0.)
	xB = r.Double(-1.)
	yB = r.Double(-1.)
	assert(graphMulti.GetN()==graphBern.GetN())

	for p in range(graphMulti.GetN()):
		graphMulti.GetPoint(p,xM,yM)
		graphBern.GetPoint(p,xB,yB)
		assert(xM==xB)
		mEYL = graphMulti.GetErrorYlow(p)
		mEYH = graphMulti.GetErrorYhigh(p)
		bEYL = graphBern.GetErrorYlow(p)
		bEYH = graphBern.GetErrorYhigh(p)

		# now set up sum
		if str(xM) not in sumPointsBern.keys():
			sumPointsMulti[str(xM)] = [yM,mEYL**2,mEYH**2]
			sumPointsBern[str(xB)] = [yB,bEYL**2,bEYH**2] 
		else:
			sumPointsMulti[str(xM)][0] += yM
			sumPointsMulti[str(xM)][1] += mEYL**2
			sumPointsMulti[str(xM)][2] += mEYH**2
			sumPointsBern[str(xB)][0] += yB
			sumPointsBern[str(xB)][1] += bEYL**2
			sumPointsBern[str(xB)][2] += bEYH**2

def plotDiffGraph(graphMulti,graphBern,name,title,legPos=True):

	diffBernNoErr = r.TGraph()
	diffBern = r.TGraphAsymmErrors()
	diffMulti = r.TGraphAsymmErrors()

	xM = r.Double(0.)
	yM = r.Double(0.)
	xB = r.Double(-1.)
	yB = r.Double(-1.)

	maxVal = 0.
	minVal = 0.
	assert(graphMulti.GetN()==graphBern.GetN())

	for p in range(graphMulti.GetN()):
		graphMulti.GetPoint(p,xM,yM)
		graphBern.GetPoint(p,xB,yB)
		assert(xM==xB)
		diffMulti.SetPoint(p,xM,yM-yM)
		diffBern.SetPoint(p,xM,yB-yM)
		diffBernNoErr.SetPoint(p,xM,yB-yM)
		mEYL = graphMulti.GetErrorYlow(p)
		mEYH = graphMulti.GetErrorYhigh(p)
		bEYL = graphBern.GetErrorYlow(p)
		bEYH = graphBern.GetErrorYhigh(p)

		diffMulti.SetPointError(p,0.,0.,mEYL,mEYH)
		diffBern.SetPointError(p,0.,0.,bEYL,bEYH)

		maxHere = max(yM-yM+mEYH,yB-yM+bEYH)
		maxVal = max(maxVal,maxHere)
		minHere = min(yM-yM-mEYL,yB-yM-bEYL)
		minVal = min (minVal,minHere)

	diffBern.SetFillColor(r.kRed-7)
	diffBern.SetLineColor(r.kRed+2)
	diffBern.SetLineWidth(3)
	diffBernNoErr.SetLineColor(r.kRed+2)
	diffBernNoErr.SetLineWidth(3)

	diffMulti.SetFillColor(r.kBlue+1)
	diffMulti.SetLineColor(r.kBlue+1)
	diffMulti.SetFillStyle(0)
	diffMulti.SetLineWidth(3)

	diffBern.GetYaxis().SetRangeUser(minVal*1.1,maxVal*1.3)
	diffBern.GetXaxis().SetTitle("m_{#gamma#gamma} (GeV)");
	diffBern.GetYaxis().SetTitle("Bkg (Bernstein) - Bkg (MultiPdf) Residual")
	diffBern.GetYaxis().SetTitleOffset(1.1)
	diffBern.SetTitle(title)
	
	if legPos:
		leg = r.TLegend(0.6,0.7,0.89,0.89)
	else:
		leg = r.TLegend(0.6,0.12,0.89,0.31)
	leg.SetFillColor(0)
	leg.AddEntry(diffBern,"Bernstein","LF")
	leg.AddEntry(diffMulti,"MultiPdf","LF")

	canv = r.TCanvas('c1','c1',700,700)
	diffBern.Draw("AL3")
	diffMulti.Draw("L SAME")
	diffBernNoErr.Draw("L SAME")
	leg.Draw("same")
	canv.Print('%s.pdf'%name)
	canv.Print('%s.png'%name)



ncats = int(sys.argv[3])

while len(legHigh)<ncats:
	legHigh.append(True)

# [mass,yp,el^2,eh^2]
sumPointsBern = {}
sumPointsMulti = {}

for cat in range(ncats):

	tfBern = r.TFile(sys.argv[1]+'_cat%d.root'%cat)
	tfMulti = r.TFile(sys.argv[2]+'_cat%d.root'%cat)
	wsBern = tfBern.Get('bkgplotws')
	wsMulti = tfMulti.Get('bkgplotws')

	dataBern = wsBern.data('roohist_data_mass_cat%d'%cat)
	dataMulti = wsMulti.data('roohist_data_mass_cat%d'%cat)

	one_sigmaBern = tfBern.Get('onesigma_cat%d'%cat)
	two_sigmaBern = tfBern.Get('twosigma_cat%d'%cat)

	one_sigmaMulti = tfMulti.Get('onesigma_cat%d'%cat)
	two_sigmaMulti = tfMulti.Get('twosigma_cat%d'%cat)

	plotDiffGraph(one_sigmaMulti,one_sigmaBern,'comp_cat%d'%cat,'Category %d'%cat,legHigh[cat])
	addToSum(one_sigmaMulti,one_sigmaBern,sumPointsMulti,sumPointsBern)

	tfBern.Close()
	tfMulti.Close()

# end loop on cats
sumGraphBern = r.TGraphAsymmErrors()
sorted_keys = sorted(sumPointsBern.keys())
for p, strm in enumerate(sorted_keys):
	m = float(strm)
	sumGraphBern.SetPoint(p,m,sumPointsBern[strm][0])
	sumGraphBern.SetPointError(p,0.,0.,sumPointsBern[strm][1]**0.5,sumPointsBern[strm][2]**0.5)

sumGraphMulti = r.TGraphAsymmErrors()
sorted_keys = sorted(sumPointsMulti.keys())
for p, strm in enumerate(sorted_keys):
	m = float(strm)
	sumGraphMulti.SetPoint(p,m,sumPointsMulti[strm][0])
	sumGraphMulti.SetPointError(p,0.,0.,sumPointsMulti[strm][1]**0.5,sumPointsMulti[strm][2]**0.5)

plotDiffGraph(sumGraphMulti,sumGraphBern,'comp_sum','All Categories',False)
