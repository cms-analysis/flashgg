import ROOT

f = ROOT.TFile("output.root")

canvases = []
## Z PEAK PLOT
zpeak = f.Get("Zpeak")
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
zpeak.Draw()


# L1 Efficiencies
hTag_l1_eta = f.Get("TAG_L1_eta");
hTag_l1_pt = f.Get("TAG_L1_pt")


hProbe_l135_eta = f.Get("PROBE_L1_35_eta")
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
eff2 = ROOT.TEfficiency(hProbe_l135_eta, hTag_l1_eta)
eff2.Draw()
hProbe_l115_eta = f.Get("PROBE_L1_15_eta")
eff3 = ROOT.TEfficiency(hProbe_l115_eta, hTag_l1_eta)
eff3.Draw("SAME")
hProbe_l110_eta = f.Get("PROBE_L1_10_eta")
eff4 = ROOT.TEfficiency(hProbe_l110_eta, hTag_l1_eta)
eff4.Draw("SAME")


hProbe_l135_pt = f.Get("PROBE_L1_35_pt")
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
eff12 = ROOT.TEfficiency(hProbe_l135_pt, hTag_l1_pt)
eff12.Draw()
hProbe_l115_pt = f.Get("PROBE_L1_15_pt")
eff13 = ROOT.TEfficiency(hProbe_l115_pt, hTag_l1_pt)
eff13.Draw("SAME")
hProbe_l110_pt = f.Get("PROBE_L1_10_pt")
eff14 = ROOT.TEfficiency(hProbe_l110_pt, hTag_l1_pt)
eff14.Draw("SAME")

raw_input()
# HLT 42, 22 eta
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_eta_twosuite = f.Get("TAG_HLT_eta_twosuite")
PROBE_HLT_R9_eta_twosuite = f.Get("PROBE_HLT_R9_eta_twosuite")
PROBE_HLT_ISO_eta_twosuite = f.Get("PROBE_HLT_ISO_eta_twosuite")
PROBE_HLT_OR_eta_twosuite = f.Get("PROBE_HLT_OR_eta_twosuite")
eff5 = ROOT.TEfficiency(PROBE_HLT_R9_eta_twosuite, TAG_HLT_eta_twosuite)
eff6 = ROOT.TEfficiency(PROBE_HLT_ISO_eta_twosuite, TAG_HLT_eta_twosuite)
eff7 = ROOT.TEfficiency(PROBE_HLT_OR_eta_twosuite, TAG_HLT_eta_twosuite)
eff5.Draw()
eff6.Draw("PESAME")
eff7.Draw("PESAME")
eff5.SetMarkerStyle(20)
eff5.SetMarkerColor(ROOT.kBlack)
canvases[-1].Update()

# HLT 42, 22 pt
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_pt_twosuite = f.Get("TAG_HLT_pt_twosuite")
PROBE_HLT_R9_pt_twosuite = f.Get("PROBE_HLT_R9_pt_twosuite")
PROBE_HLT_ISO_pt_twosuite = f.Get("PROBE_HLT_ISO_pt_twosuite")
PROBE_HLT_OR_pt_twosuite = f.Get("PROBE_HLT_OR_pt_twosuite")
eff8 = ROOT.TEfficiency(PROBE_HLT_R9_pt_twosuite, TAG_HLT_pt_twosuite)
eff9 = ROOT.TEfficiency(PROBE_HLT_ISO_pt_twosuite, TAG_HLT_pt_twosuite)
eff10 = ROOT.TEfficiency(PROBE_HLT_OR_pt_twosuite, TAG_HLT_pt_twosuite)
eff8.Draw()
eff9.Draw("SAME")
eff10.Draw("SAME")

# HLT 42, 22 high eta
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_eta_twosuite_high = f.Get("TAG_HLT_eta_twosuite_high")
PROBE_HLT_R9_eta_twosuite_high = f.Get("PROBE_HLT_R9_eta_twosuite_high")
PROBE_HLT_ISO_eta_twosuite_high = f.Get("PROBE_HLT_ISO_eta_twosuite_high")
PROBE_HLT_OR_eta_twosuite_high = f.Get("PROBE_HLT_OR_eta_twosuite_high")
eff11 = ROOT.TEfficiency(PROBE_HLT_R9_eta_twosuite_high, TAG_HLT_eta_twosuite_high)
eff12 = ROOT.TEfficiency(PROBE_HLT_ISO_eta_twosuite_high, TAG_HLT_eta_twosuite_high)
eff13 = ROOT.TEfficiency(PROBE_HLT_OR_eta_twosuite_high, TAG_HLT_eta_twosuite_high)
eff11.Draw()
eff12.Draw("SAME")
eff13.Draw("SAME")

# HLT 42, 22 high pt
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_pt_twosuite_high = f.Get("TAG_HLT_pt_twosuite_high")
PROBE_HLT_R9_pt_twosuite_high = f.Get("PROBE_HLT_R9_pt_twosuite_high")
PROBE_HLT_ISO_pt_twosuite_high = f.Get("PROBE_HLT_ISO_pt_twosuite_high")
PROBE_HLT_OR_pt_twosuite_high = f.Get("PROBE_HLT_OR_pt_twosuite_high")
eff14 = ROOT.TEfficiency(PROBE_HLT_R9_pt_twosuite_high, TAG_HLT_pt_twosuite_high)
eff15 = ROOT.TEfficiency(PROBE_HLT_ISO_pt_twosuite_high, TAG_HLT_pt_twosuite_high)
eff16 = ROOT.TEfficiency(PROBE_HLT_OR_pt_twosuite_high, TAG_HLT_pt_twosuite_high)
eff14.Draw()
eff15.Draw("SAME")
eff16.Draw("SAME")

# HLT 42, 22 seeded
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_eta_seeded = f.Get("TAG_HLT_eta_seeded")
PROBE_HLT_R9_eta_seeded = f.Get("PROBE_HLT_R9_eta_seeded")
PROBE_HLT_ISO_eta_seeded = f.Get("PROBE_HLT_ISO_eta_seeded")
PROBE_HLT_OR_eta_seeded = f.Get("PROBE_HLT_OR_eta_seeded")
eff17 = ROOT.TEfficiency(PROBE_HLT_R9_eta_seeded, TAG_HLT_eta_seeded)
eff18 = ROOT.TEfficiency(PROBE_HLT_ISO_eta_seeded, TAG_HLT_eta_seeded)
eff19 = ROOT.TEfficiency(PROBE_HLT_OR_eta_seeded, TAG_HLT_eta_seeded)
eff17.Draw()
eff18.Draw("SAME")
eff19.Draw("SAME")

# HLT 42, 22 pt seeded 
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_pt_seeded = f.Get("TAG_HLT_pt_seeded")
PROBE_HLT_R9_pt_seeded = f.Get("PROBE_HLT_R9_pt_seeded")
PROBE_HLT_ISO_pt_seeded = f.Get("PROBE_HLT_ISO_pt_seeded")
PROBE_HLT_OR_pt_seeded = f.Get("PROBE_HLT_OR_pt_seeded")
eff20 = ROOT.TEfficiency(PROBE_HLT_R9_pt_seeded, TAG_HLT_pt_seeded)
eff21 = ROOT.TEfficiency(PROBE_HLT_ISO_pt_seeded, TAG_HLT_pt_seeded)
eff22 = ROOT.TEfficiency(PROBE_HLT_OR_pt_seeded, TAG_HLT_pt_seeded)
eff20.Draw()
eff21.Draw("SAME")
eff22.Draw("SAME")

# HLT 42, 22 eta unseeded
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_eta_unseeded = f.Get("TAG_HLT_eta_unseeded")
PROBE_HLT_R9_eta_unseeded = f.Get("PROBE_HLT_R9_eta_unseeded")
PROBE_HLT_ISO_eta_unseeded = f.Get("PROBE_HLT_ISO_eta_unseeded")
PROBE_HLT_OR_eta_unseeded = f.Get("PROBE_HLT_OR_eta_unseeded")
eff23 = ROOT.TEfficiency(PROBE_HLT_R9_eta_unseeded, TAG_HLT_eta_unseeded)
eff24 = ROOT.TEfficiency(PROBE_HLT_ISO_eta_unseeded, TAG_HLT_eta_unseeded)
eff25 = ROOT.TEfficiency(PROBE_HLT_OR_eta_unseeded, TAG_HLT_eta_unseeded)
eff23.Draw()
eff24.Draw("SAME")
eff25.Draw("SAME")

# HLT 42, 22 pt unseeded
canvases.append(ROOT.TCanvas("c"+str(len(canvases)), ""))
TAG_HLT_pt_unseeded = f.Get("TAG_HLT_pt_unseeded")
PROBE_HLT_R9_pt_unseeded = f.Get("PROBE_HLT_R9_pt_unseeded")
PROBE_HLT_ISO_pt_unseeded = f.Get("PROBE_HLT_ISO_pt_unseeded")
PROBE_HLT_OR_pt_unseeded = f.Get("PROBE_HLT_OR_pt_unseeded")
eff26 = ROOT.TEfficiency(PROBE_HLT_R9_pt_unseeded, TAG_HLT_pt_unseeded)
eff27 = ROOT.TEfficiency(PROBE_HLT_ISO_pt_unseeded, TAG_HLT_pt_unseeded)
eff28 = ROOT.TEfficiency(PROBE_HLT_OR_pt_unseeded, TAG_HLT_pt_unseeded)
eff26.Draw()
eff27.Draw("SAME")
eff28.Draw("SAME")

#print "TAG_L1: "
#print "PROBE_L1_7: ", PROBE_L1_7
#print "PROBE_L1_13: ",PROBE_L1_13
#print "PROBE_L1_22: ",PROBE_L1_22
#print
#print "TAG_HLT 26 : ",TAG_HLT_seeded
#print "PROBE_HLT_OR 26 : ",PROBE_HLT_OR_seeded
#print "PROBE_HLT_Iso 26 : ",PROBE_HLT_Iso_seeded
#print "PROBE_HLT_R9 26 : ",PROBE_HLT_R9_seeded
#print
#print "TAG_HLT 18 : ",TAG_HLT_unseeded
#print "PROBE_HLT_OR 18 : ",PROBE_HLT_OR_unseeded
#print "PROBE_HLT_Iso 18 : ",PROBE_HLT_Iso_unseeded
#print "PROBE_HLT_R9 18 : ",PROBE_HLT_R9_unseeded
#print
#print "TAG_HLT 36/22 ",TAG_HLT_twosuite
#print "PROBE_HLT_OR 36/22 : ",PROBE_HLT_OR_twosuite
#print "PROBE_HLT_Iso 36/22 : ",PROBE_HLT_Iso_twosuite
#print "PROBE_HLT_R9 36/22 : ",PROBE_HLT_R9_twosuite
#print
#print "TAG_HLT 36 ",TAG_HLT_twosuite_high
#print "PROBE_HLT_OR 36 : ",PROBE_HLT_OR_twosuite_high
#print "PROBE_HLT_Iso 36 : ",PROBE_HLT_Iso_twosuite_high
#print "PROBE_HLT_R9 36 : ",PROBE_HLT_R9_twosuite_high

#print "L1_35  eff:  %1.3f +- "%(hProbe_l135_eta.Integral()/hTag_l1_eta.Integral())

#print "HLT OR eff 26  | L1_15 : %1.3f +- "%(PROBE_HLT_OR_eta_seeded.Integral()/TAG_HLT_eta_seeded.Integral())
#print "HLT OR eff 18  | L1_10 : %1.3f +- "%(PROBE_HLT_OR_eta_unseeded.Integral()/TAG_HLT_eta_unseeded.Integral())
print "HLT OR eff 22  | !L1_10: %1.3f +- "%(PROBE_HLT_OR_eta_twosuite.Integral()/TAG_HLT_eta_twosuite.Integral())
print "HLT Iso eff 22 | !L1_10: %1.3f +- "%(PROBE_HLT_ISO_eta_twosuite.Integral()/TAG_HLT_eta_twosuite.Integral())
print "HLT R9 eff 22  | !L1_10: %1.3f +- "%(PROBE_HLT_R9_eta_twosuite.Integral()/TAG_HLT_eta_twosuite.Integral())
print "HLT OR eff 36  | L1_22 : %1.3f +- "%(PROBE_HLT_OR_eta_twosuite_high.Integral()/TAG_HLT_eta_twosuite_high.Integral())
print "HLT Iso eff 36 | L1_22 : %1.3f +- "%(PROBE_HLT_ISO_eta_twosuite_high.Integral()/TAG_HLT_eta_twosuite_high.Integral())
print "HLT R9 eff 36  | L1_22 : %1.3f +- "%(PROBE_HLT_R9_eta_twosuite_high.Integral()/TAG_HLT_eta_twosuite_high.Integral())

p10 = hProbe_l110_eta.Integral()/hTag_l1_eta.Integral()
p15 = hProbe_l115_eta.Integral()/hTag_l1_eta.Integral()
p22 = hProbe_l135_eta.Integral()/hTag_l1_eta.Integral()

P_18_giv7 =   PROBE_HLT_OR_eta_unseeded     .Integral()/TAG_HLT_eta_unseeded.Integral()
P_26_giv13 =  PROBE_HLT_OR_eta_seeded       .Integral()/TAG_HLT_eta_seeded.Integral()
P_22_givno7 = PROBE_HLT_OR_eta_twosuite     .Integral()/TAG_HLT_eta_twosuite.Integral()
P_36_giv22 =  PROBE_HLT_OR_eta_twosuite_high.Integral()/TAG_HLT_eta_twosuite_high.Integral()

P_26_giv13_Giv18 = P_26_giv13
#p15_giv10 = p15/p10
#P_L1 = p7*p7 * (1 - (1 - p13_giv7)*(1 - p13_giv7)) 
P_L1 = 2*p10*p15-p15*p15

#temp1 = p13*p13*(1-(1-P_26_giv13_Giv18)*(1-P_26_giv13_Giv18))
#temp2 = (P_L1-p13*p13)*P_18_giv7*P_26_giv13
#temp3 =  2*(1-p7)*P_22_givno7*p22*P_36_giv22
temp3 = P_22_givno7*p22*P_36_giv22
print temp3
#
#P_HLT = temp1+temp2+temp3;
print "L1_10  eff: %1.3f +- "%(hProbe_l110_eta.Integral()/hTag_l1_eta.Integral())
print "L1_15  eff: %1.3f +- "%(hProbe_l115_eta.Integral()/hTag_l1_eta.Integral())
print "P_L1_15_10: %1.3f +- "%(P_L1)
print "P_L1_35  :  %1.3f +- "%(p22)
#print "P_HLT    : %1.3f +- "%(P_HLT)
raw_input()
