import os
import ROOT as r

r.gROOT.ProcessLine('.L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so')
r.gROOT.SetBatch()

binnedSig = r.TFile('/afs/cern.ch/user/m/mkenzie/eos/cms/store/group/phys_higgs/cmshgg/analyzed/workspace_store/legacy_freeze_v3/CMS-HGG_mva_8TeV_Sig_interpolated.root')
paramSig = r.TFile('/afs/cern.ch/user/m/mkenzie/eos/cms/store/group/phys_higgs/cmshgg/analyzed/workspace_store/legacy_freeze_v3/CMS-HGG_mva_8TeV_sigfit.root')
nonInterpSig = r.TFile('/afs/cern.ch/user/m/mkenzie/eos/cms/store/group/phys_higgs/cmshgg/analyzed/workspace_store/legacy_freeze_v3/CMS-HGG_mva_8TeV_Sig.root')

paramWS = paramSig.Get('wsig_8TeV')
normSum = paramWS.function('normSum')
mh = paramWS.var('MH')
lumi = paramWS.var('IntLumi')

binnedWS = binnedSig.Get('cms_hgg_workspace')
nonInterpWS = nonInterpSig.Get('cms_hgg_workspace')

canv = r.TCanvas('c1','c1',700,700)

for proc in ['ggh','vbf','wh','zh','tth']:
	for c in range(14):
		nonInterpGraph = r.TGraph()
		paramGraph = r.TGraph()
		fevs = paramWS.function('hggpdfsmrel_8TeV_%s_cat%d_norm'%(proc,c))
		for p,m in enumerate(range(110,151)):
			mh.setVal(m)
			paramGraph.SetPoint(p,m,fevs.getVal()*lumi.getVal())
			if m%5==0:
				sigmc = nonInterpWS.data('sig_%s_mass_m%d_cat%d'%(proc,m,c))
				nonInterpGraph.SetPoint(p,m,sigmc.sumEntries())
		
		paramGraph.SetLineColor(r.kRed)
		paramGraph.SetLineWidth(4)
		paramGraph.SetLineStyle(2)
		nonInterpGraph.SetMarkerStyle(r.kCircle)
		nonInterpGraph.SetMarkerSize(2)

		nonInterpGraph.SetTitle('%s_cat%d'%(proc,c))
		nonInterpGraph.GetXaxis().SetRangeUser(105,155)
		nonInterpGraph.Draw("AEP")
		paramGraph.Draw("LP")
		nonInterpGraph.Draw("EPsame")
		canv.Print('expEvPlots/%s_cat%d.pdf'%(proc,c))


