import ROOT
import os, sys
import array
import numpy

def getGraph(filename,grname):
	tFile = ROOT.TFile(filename)
	gr = tFile.Get(grname)
	print 'Graph: ', gr.GetName()
	tFile.Close()
	return gr

def getMinPval(graph):
	minPval=1000.
	for i in range(graph.GetN()):
		mass = ROOT.Double(0.)
		pval = ROOT.Double(1.)
		graph.GetPoint(i,mass,pval)
		if pval<minPval:
			minPval=pval
			massMinPval=mass
			maxSig=ROOT.RooStats.PValueToSignificance(pval)

	return [massMinPval,minPval,maxSig]

ROOT.gStyle.SetPalette(1)
ROOT.gROOT.SetBatch()
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetStyle("Plain")

if not os.path.isdir('2Dplots'):
	os.makedirs('2Dplots')

toyName = sys.argv[1]
toyDir = sys.argv[2:]
print toyDir

pval2D = ROOT.TH2F("pval2D","Significance comparison",10,-1,1,40,110,150)
lim2D = ROOT.TH2F("lim2D","Exclusion comparison",10,-1,1,40,110,150)
mu2D = ROOT.TH2D("mu2D","Best fit comparison",10,-1,1,40,110,150)
pval2D.GetYaxis().SetTitle('m_{H} (GeV)')
pval2D.GetXaxis().SetTitle('IC_{#sigma}-MIT_{#sigma}')
lim2D.GetYaxis().SetTitle('m_{H} (GeV)')
lim2D.GetXaxis().SetTitle('IC_{#sigma_{95%CL}}-MIT_{#sigma_{95%CL}}')
mu2D.GetYaxis().SetTitle('m_{H} (GeV)')
mu2D.GetXaxis().SetTitle('IC_{#hat{#mu}}-MIT_{#hat{#mu}}')

sig2D = ROOT.TH2F("sigcomp","Significance comparison",10,0,5,10,0,5)
sig2D.GetXaxis().SetTitle('IC (Local #sigma)')
sig2D.GetYaxis().SetTitle('MIT (Local #sigma)')

mass2D = ROOT.TH2F("masscomp","Mass comparison (at largest #sigma)",40,110,150,40,110,150)
mass2D.GetXaxis().SetTitle('IC - Mass (GeV) at max #sigma')
mass2D.GetYaxis().SetTitle('MIT - Mass (GeV) at max #sigma')

sigDiff = ROOT.TH1F("sigdiff","Significance difference",20,-1.,1.)
sigDiff.GetXaxis().SetTitle("IC_{#sigma}-MIT_{#sigma}");

massDiff = ROOT.TH1F("massdiff","Mass difference",20,-10.,10.)
massDiff.GetXaxis().SetTitle("IC_{m_{H}}-MIT_{m_{H}}");

muDiff = ROOT.TH1F("mudiff","Best fit difference",20,-1.,1.)
muDiff.GetXaxis().SetTitle("IC_{#hat{#mu}}-MIT_{#hat{#mu}}");

exlDiff = ROOT.TH1F("exldiff","Exclusion difference",20,-1.,1.)
exlDiff.GetXaxis().SetTitle('IC_{#sigma_{95%CL}}-MIT_{#sigma_{95%CL}}');

for toy in toyDir:
	
	# get limit graphs
	icLim = getGraph('%s/ICcombResults/limit.root'%toy,'observed')
	miLim = getGraph('%s/MITcombResults/limit.root'%toy,'observed')
	# get pval graphs 
	icPval = getGraph('%s/ICcombResults/pvals.root'%toy,'observed')
	miPval = getGraph('%s/MITcombResults/pvals.root'%toy,'observed')
	# get best fit graphs 
	icMu = getGraph('%s/ICcombResults/bf.root'%toy,'bestfit')
	miMu = getGraph('%s/MITcombResults/bf.root'%toy,'bestfit')

	# loop integer masses
	for m in numpy.arange(110,151,1):
		pval2D.Fill(icPval.Eval(m)-miPval.Eval(m),m)
		lim2D.Fill(icLim.Eval(m)-miLim.Eval(m),m)
		mu2D.Fill(icMu.Eval(m)-miMu.Eval(m),m)

	# find min pval [mass,pval,sig]
	icMin = getMinPval(icPval)
	miMin = getMinPval(miPval)
	icMin.append(icLim.Eval(icMin[0]))
	miMin.append(miLim.Eval(miMin[0]))
	icMin.append(icMu.Eval(icMin[0]))
	miMin.append(miMu.Eval(miMin[0]))

	# now have [mass,pval,sig,exl,mu]

	sig2D.Fill(icMin[2],miMin[2])
	mass2D.Fill(icMin[0],miMin[0])
	
	massDiff.Fill(icMin[0]-miMin[0])
	sigDiff.Fill(icMin[2]-miMin[2])
	exlDiff.Fill(icMin[3]-miMin[3])
	muDiff.Fill(icMin[4]-miMin[4])

c = ROOT.TCanvas()

pval2D.Draw("colz")
c.SaveAs("2Dplots/%smH_pval.pdf"%toyName)
c.SaveAs("2Dplots/%smH_pval.png"%toyName)

lim2D.Draw("colz")
c.SaveAs("2Dplots/%smH_lim.pdf"%toyName)
c.SaveAs("2Dplots/%smH_lim.png"%toyName)

mu2D.Draw("colz")
c.SaveAs("2Dplots/%smH_mu.pdf"%toyName)
c.SaveAs("2Dplots/%smH_mu.png"%toyName)

sig2D.Draw("colz")
c.SaveAs("2Dplots/%smax_sigma.pdf"%toyName)
c.SaveAs("2Dplots/%smax_sigma.png"%toyName)

mass2D.Draw("colz")
c.SaveAs("2Dplots/%smax_sig_mH.pdf"%toyName)
c.SaveAs("2Dplots/%smax_sig_mH.png"%toyName)

sigDiff.Draw()
c.SaveAs("2Dplots/%sdiff_sigma.pdf"%toyName)
c.SaveAs("2Dplots/%sdiff_sigma.png"%toyName)

massDiff.Draw()
c.SaveAs("2Dplots/%sdiff_mass.pdf"%toyName)
c.SaveAs("2Dplots/%sdiff_mass.png"%toyName)

exlDiff.Draw()
c.SaveAs("2Dplots/%sdiff_exl.pdf"%toyName)
c.SaveAs("2Dplots/%sdiff_exl.png"%toyName)

muDiff.Draw()
c.SaveAs("2Dplots/%sdiff_mu.pdf"%toyName)
c.SaveAs("2Dplots/%sdiff_mu.png"%toyName)
