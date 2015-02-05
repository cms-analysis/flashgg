#!/bin/env python

from ROOT import TFile, TF1, TCanvas, kBlack, kBlue, kRed, kGreen, kCyan, kMagenta, kWhite, TGraph, TF2, TF12, kFullCircle, gROOT, TLegend, TPaveText
from sys import argv

def eff(fin,name,var="pt",rebin=5):

    num=fin.Get("%s_rv_cat0_tot" % var )
    den=fin.Get("%s_cat0_tot"    % var )

    if rebin > 0:
        num.Rebin(rebin)
        den.Rebin(rebin)


    eff = num.Clone(name)
    eff.Divide( num, den, 1., 1., "B" )

    num.Rebin( num.GetNbinsX() )
    den.Rebin( den.GetNbinsX() )
    avg_eff = num.Clone()
    avg_eff.Divide( num, den, 1., 1., "B" )
    print "%1.3f +- %1.3f" % ( avg_eff.GetBinContent(1), avg_eff.GetBinError(1) )
    return eff

def prob(fin,name,var="pt",profile=True,rebin=5):

    prb = fin.Get("vtxprob_%s_cat0_tot" % var)

    if rebin > 0:
        prb.RebinX(rebin)
    
    if profile:
        return prb.ProfileX(name,1,-1,"w")

    return prb.Clone(name)

gROOT.LoadMacro("../Macros/rootglobestyle.C")
from ROOT import setTDRStyle, gStyle
setTDRStyle()
gStyle.SetOptTitle(0)


file=argv[1]

fin=TFile.Open(file)
eff1 = eff(fin,"eff1",rebin=10)
eff2 = eff(fin,"eff2","nvtx",rebin=0)
print eff2

prob1 = prob(fin,"prob1")
prob2 = prob(fin,"prob2","nvtx",True,0)

eff1.SetMarkerStyle(21);
eff2.SetMarkerStyle(21);

eff1.SetLineColor(kBlack);
eff2.SetLineColor(kBlack);

prob1.SetFillColor(4);
prob1.SetLineColor(4);
prob1.SetMarkerColor(4);

prob2.SetFillColor(4);
prob2.SetLineColor(4);
prob2.SetMarkerColor(4);

pt = TPaveText(51.37856,0.5654967,190.8217,0.6623778,"br")
pt.SetFillColor(0)
pt.SetLineColor(0)
pt.SetTextAlign(13)
pt.SetTextFont(42)
pt.AddText("CMS Preliminary Simulation")
pt.AddText("<PU> = 19")
pt.Draw("same")

c1 = TCanvas("vtxProbPt","vtxProbPt")
eff1.SetTitle("Vertex efficiency;p_{T}(#gamma #gamma) (GeV);Fraction | z_{reco} - z_{true} | < 10 mm")
prob1.SetTitle("Average vertex probability;p_{T}(#gamma #gamma) (GeV);Fraction | z_{reco} - z_{true} | < 10 mm")
eff1.GetYaxis().SetRangeUser(0.5, 1.1)
eff1.GetXaxis().SetTitleOffset(1.1);
eff1.GetYaxis().SetTitleOffset(1.2);

eff1.Draw("e0")
prob1.Draw("e3 same")
eff1.Draw("e0 same")

leg1 = TLegend(0.38,0.44,0.84,0.60)
leg1.SetShadowColor(kWhite), leg1.SetLineColor(kWhite), leg1.SetFillColor(kWhite), leg1.SetTextFont(60)

leg1.AddEntry(eff1,"","pe")
leg1.AddEntry(prob1,"","f")
leg1.Draw("same")
pt.Draw("same")


c2 = TCanvas("vtxProbNvtx","vtxProbNvtx")
eff2.SetTitle("True vertex eff.;N_{vtx};Fraction | z_{reco} - z_{true} | < 10 mm")
prob2.SetTitle("Aveage vertex prob.;N_{vtx};Fraction | z_{reco} - z_{true} | < 10 mm")

eff2.GetYaxis().SetRangeUser(0.5, 1.1)
eff2.GetXaxis().SetRangeUser(0, 35)
eff2.GetXaxis().SetTitleOffset(1.1);
eff2.GetYaxis().SetTitleOffset(1.2);
eff2.Draw("e0")
prob2.Draw("e3 same")
eff2.Draw("e0 same")

leg2 = TLegend(0.52,0.62,0.82,0.82)
leg2.SetShadowColor(kWhite), leg2.SetLineColor(kWhite), leg2.SetFillColor(kWhite), leg2.SetTextFont(60)
leg2.AddEntry(eff2,"","pe")
leg2.AddEntry(prob2,"","f")
leg2.Draw("same")
pt.Draw("same")


c0 = TCanvas("vtxEffPt","vtxEffPt")
eff0 = eff1.Clone()
eff0.SetTitle("True vertex eff.;p_{T}(#gamma #gamma) (GeV);Fraction of events")
eff0.Draw("e1")
leg0=TLegend(0.332215, 0.357143, 0.60906, 0.423345)
leg0.AddEntry(eff0,"| z_{reco} - z_{true} | < 10 mm","lpf")
leg0.SetShadowColor(kWhite), leg0.SetLineColor(kWhite), leg0.SetFillColor(kWhite), leg0.SetTextFont(60)
leg0.Draw("same")
pt.Draw("same")

c3 = TCanvas("vtxEffNvtx","vtxEffNvtx")
eff3 = eff2.Clone()
eff3.SetTitle("True vertex eff.;N_{vtx};Fraction of events")
eff3.Draw("e1")
leg3=TLegend(0.434564,0.684669,0.711409,0.750871)
leg3.SetShadowColor(kWhite), leg3.SetLineColor(kWhite), leg3.SetFillColor(kWhite), leg3.SetTextFont(60)
leg3.AddEntry(eff3,"| z_{reco} - z_{true} | < 10 mm","lpf")
leg3.Draw("same")
pt.Draw("same")


for c in c0, c1, c2, c3:
    for fmt in "C", "png", "pdf":
        c.SaveAs( "%s.%s" % (c.GetName(), fmt) )
