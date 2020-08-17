from ROOT import *
import math
# from MyCMSStyle import *
gStyle.SetOptStat(0)
# gROOT.SetBatch(1)

files = []
##name,marker color, marker style, legend
# files.append(['/eos/cms/store/user/torimoto/physics/4gamma/H4Gamma_2016Analysis/Signal_LowMassPreselOnly/signal_hgg','GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_13TeV','hgg'])
# files.append(['/eos/cms/store/user/torimoto/physics/4gamma/H4Gamma_2016Analysis/Signal_LowMassPreselOnly/signal_m_60','SUSYGluGluToHToAA_AToGG_M_60_TuneCUETP8M1_13TeV_pythia8_13TeV','h4g'])
# files.append(['/afs/cern.ch/work/t/twamorka/ThesisAnalysis/CMSSW_10_5_0/src/flashgg/Signal_Jul15_ver5/signal_60/signal_m_60','SUSYGluGluToHToAA_AToGG_M_60_TuneCUETP8M1_13TeV_pythia8_13TeV','h4g'])
files.append(['/afs/cern.ch/work/a/atishelm/21JuneFlashgg/CMSSW_10_5_0/src/flashgg/X250_HHWWgg_qqlnu','_13TeV','HHWWgg'])

Cut = []
Cut.append(['abs(dZ)','1'])  # zero vtx 
Cut.append(['abs(dZ_Hgg_Vertex)','1']) 

Marker = []
# Marker.append([kGreen+2,20,'Hgg Signal wrt Hgg vtx'])
# Marker.append([kGreen+2,24,'Hgg Signal wrt 0th vtx'])
Marker.append([kGreen+2,24,'HH#rightarrowWW#gamma#gamma Signal wrt 0th vtx'])
Marker.append([kBlue,20,'HH#rightarrowWW#gamma#gamma Signal wrt Hgg vtx'])
# Marker.append([kBlack,21,'H4G Signal wrt H4G vtx'])

graphs  = TMultiGraph()
# leg = TLegend(0.586005, 0.543091, 0.876075, 0.732541)
leg = TLegend(0.50, 0.493091, 0.78, 0.682541)
leg.SetBorderSize(0)
leg.SetTextSize(0.035)
leg.SetFillColor(kWhite)
leg.SetFillStyle(0)
c0 = TCanvas("c", "c", 800,600)
# SetPadStyle(c0)
# c0.SetGridy()
hists = []
for fi, f in enumerate(files):  
    ch = TChain()
    # ch.Add(f[0]+str('.root/h4gCandidateDumper/trees/')+f[1]+str('_2photons'))
    # ch.Add(f[0]+str('.root/h4gCandidateDumper/trees/')+f[1]+str('_3photons'))
    ch.Add(f[0]+str('.root/HHWWggCandidateDumper/trees/ggF_125')+f[1]+str('_SL'))

    print ch.GetEntries()
    for ci, c in enumerate(Cut):
        hist = f[1]+"_"+c[0]
        print hist
        h = TH1F('h', '', 100, 0., 20.)
        # ch.Draw(c[0]+ '>> h','puweight*'+str(c[1]))
        ch.Draw(c[0]+ '>> h',''+str(c[1]))
        print "variable being plotted ", c[0]
        print "weight being applied ", c[1]
        print h.Integral()
        hists.append(h)

n_bins = hists[0].GetNbinsX()

for hi, h in enumerate(hists):
    gr = TGraphErrors()
    gr.SetMarkerColor()
    gr.SetMarkerStyle()
    gr.SetMarkerSize(1)
    for bin in range(1,n_bins+1): # avoid underflow and overflow bins 
        # print " bin=" , bin , " , dZ (cm)=" , 20./100.*bin , " -> eff= ", h.Integral(1,bin)/h.Integral() , " - " , h.Integral(1,bin-1) , " - " , h.Integral(1,bin) , " - " , h.Integral(), " - ", h.GetBinContent(bin)
        eff=h.Integral(1,bin)/h.Integral()
        # print("eff = ",eff)
        if ((20./100)*bin == 0.8): print("eff at 1cm: ",eff)
        gr.SetPoint(bin-1,20./100.*bin,eff)
        gr.SetPointError(bin-1,0.,math.sqrt(eff*(1-eff)/h.Integral()))
        gr.SetMarkerColorAlpha(Marker[hi][0],0.8)
        gr.SetMarkerStyle(Marker[hi][1])
    # print "***********************************************************************************"
    graphs.Add(gr)

    leg.AddEntry(gr,Marker[hi][2],"lp")

graphs.SetTitle("Signal Efficiency vs. dZ;dZ;Efficiency")
# graphs.GetYaxis().SetTitleOffset(1.1);

graphs_min, graphs_max = 0.975, 1.005

graphs.SetMinimum(graphs_min)
graphs.SetMaximum(graphs_max)

# style = gROOT.GetStyle()

# mcStyle = TStyle("mcStyle","mcStyle")
# mcStyle.SetTitleOffset(1.4,"y")
# gROOT.SetStyle("mcStyle")
gStyle.SetTitleOffset(1.4,"Y")
# c0.SetTitleOffset(1.3)

c0.Update()
# cxmin, cymin, cxmax, cymax = c0.GetUxmin(), c0.GetUymin(), c0.GetUxmax(), c0.GetUymax() 
# hymin, hymax = graphs.GetHistogram().GetMinimum(), graphs.GetHistogram().GetMaximum()

# print'graphs.GetHistogram() = ',graphs.GetHistogram

line = TLine(0.8, graphs_min, 0.8, graphs_max);
line.SetLineColor(kRed+2);
line.SetLineStyle(8);
line.SetLineWidth(2);

graphs.Draw("AP same")
leg.Draw("same")
line.Draw("same")
c0.SaveAs("test.pdf")
c0.SaveAs("test.C")

# c0.SaveAs("/afs/cern.ch/user/t/twamorka/www/H4Gamma/VertexEfficiencyStudy/H4G_60_Hgg_LowMassPresel_withWeight_4photonCategoryOnly.pdf")
# c0.SaveAs("/afs/cern.ch/user/t/twamorka/www/H4Gamma/VertexEfficiencyStudy/H4G_60_Hgg_LowMassPresel_withWeight_4photonCategoryOnly.png")
