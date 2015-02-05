import os
import ROOT as r

r.gROOT.ProcessLine('.L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so')
r.gROOT.SetBatch()

binnedSig = r.TFile('/afs/cern.ch/user/m/mkenzie/eos/cms/store/group/phys_higgs/cmshgg/analyzed/workspace_store/legacy_freeze_v3/CMS-HGG_cic_7TeV_Sig_interpolated.root')
paramSig = r.TFile('/afs/cern.ch/user/m/mkenzie/eos/cms/store/group/phys_higgs/cmshgg/analyzed/workspace_store/legacy_freeze_v3/CMS-HGG_cic_7TeV_sigfit.root')
nonInterpSig = r.TFile('/afs/cern.ch/user/m/mkenzie/eos/cms/store/group/phys_higgs/cmshgg/analyzed/workspace_store/legacy_freeze_v3/CMS-HGG_cic_7TeV_Sig.root')

paramWS = paramSig.Get('wsig_7TeV')
normSum = paramWS.function('normSum')
mh = paramWS.var('MH')
lumi = paramWS.var('IntLumi')

binnedWS = binnedSig.Get('cms_hgg_workspace')
nonInterpWS = nonInterpSig.Get('cms_hgg_workspace')

binnedGraph = r.TGraph()
paramGraph = r.TGraph()
nonInterpGraph = r.TGraphErrors()
leg = r.TLegend(0.6,0.6,0.89,0.89)
leg.SetFillColor(0)
leg.SetLineColor(0)

for p,m in enumerate(range(110,151)):
	totalBinned=0
	totalParam=0
	totalNonInterp=0
	totalNonInterpErr=r.Double(0.)
	mh.setVal(m)
	totalParam += normSum.getVal()*lumi.getVal()
	for c in range(15):
		ggh = binnedWS.data('roohist_sig_ggh_mass_m%d_cat%d'%(m,c))
		vbf = binnedWS.data('roohist_sig_vbf_mass_m%d_cat%d'%(m,c))
		wh = binnedWS.data('roohist_sig_wh_mass_m%d_cat%d'%(m,c))
		zh = binnedWS.data('roohist_sig_zh_mass_m%d_cat%d'%(m,c))
		tth = binnedWS.data('roohist_sig_tth_mass_m%d_cat%d'%(m,c))
		totalBinned += ggh.sumEntries()
		totalBinned += vbf.sumEntries()
		totalBinned += wh.sumEntries()
		totalBinned += zh.sumEntries()
		totalBinned += tth.sumEntries()
		if m%5==0:
			ggh = nonInterpSig.Get('th1f_sig_ggh_mass_m%d_cat%d'%(m,c))
			vbf = nonInterpSig.Get('th1f_sig_vbf_mass_m%d_cat%d'%(m,c))
			wh = nonInterpSig.Get('th1f_sig_wh_mass_m%d_cat%d'%(m,c))
			zh = nonInterpSig.Get('th1f_sig_zh_mass_m%d_cat%d'%(m,c))
			tth = nonInterpSig.Get('th1f_sig_tth_mass_m%d_cat%d'%(m,c))
			if c==0: 
				total = ggh.Clone('th1f_sig_mass_m%d_cat%d'%(m,c))
				total.Add(vbf)
				total.Add(wh)
				total.Add(zh)
				total.Add(tth)
			else:
				total.Add(ggh)
				total.Add(vbf)
				total.Add(wh)
				total.Add(zh)
				total.Add(tth)
		totalNonInterp = total.IntegralAndError(0,total.GetNbinsX()+1,totalNonInterpErr)
			
	print m, totalBinned, totalParam
	binnedGraph.SetPoint(p,m,totalBinned)
	paramGraph.SetPoint(p,m,totalParam)
	if m%5==0:
		nonInterpGraph.SetPoint(p,m,totalNonInterp)
		nonInterpGraph.SetPointError(p,0,totalNonInterpErr)
		print totalNonInterp, totalNonInterpErr

canv = r.TCanvas('c1','c1',700,700)
binnedGraph.SetLineColor(r.kBlue)
binnedGraph.SetLineWidth(4)
binnedGraph.GetXaxis().SetTitle('m_{H} (GeV)')
binnedGraph.GetYaxis().SetTitle('Expected Signal Events')
paramGraph.SetLineColor(r.kRed)
paramGraph.SetLineWidth(4)
paramGraph.SetLineStyle(2)
nonInterpGraph.SetMarkerStyle(r.kCircle)
nonInterpGraph.SetMarkerSize(2)
leg.AddEntry(binnedGraph,'Binned Signal','L')
leg.AddEntry(paramGraph,'Parametric Signal','L')
leg.AddEntry(nonInterpGraph,'Non Interp Signal','EP')

binnedGraph.Draw("ALP")
paramGraph.Draw("LPsame")
nonInterpGraph.Draw("EPsame")
leg.Draw()
canv.Modified()
canv.Update()
canv.Print('expEvs7TeV.pdf')
#name = raw_input('Wanna print that? Give us a name boy\n')
#canv.Print('%s.pdf'%name)
