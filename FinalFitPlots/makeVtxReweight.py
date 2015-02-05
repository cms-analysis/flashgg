#================================================================================================
# makeVtxRegweight.py
# Constructs a ROOT file with the Efficiency to select the right/wrong vertex in the H->gg system
# Adapted from original script by P. Musella
#================================================================================================

class options:
   def __init__(self):
      ## self.eff = "/afs/cern.ch/user/m/malberti/public/scaleFactors/vtxIdEff_vs_bosonPt_globe_Glu120_S6_PUweights_2011_68000.root"
      ## self.eff_ratio = "/afs/cern.ch/user/m/malberti/public/scaleFactors/vtxIdScaleFactorFromZmumu_PUweights_minBiasXsec69400_observed_Run2012ABC.root"
      self.eff_ratio = "/afs/cern.ch/user/m/malberti/public/scaleFactors/vtxIdScaleFactorFromZmumu_PUweights_minBiasXsec69400_observed_Run2012ABC_BSreweight_new.root"
      ### "/afs/cern.ch/user/d/deguio/public/Hgg/vtxIdScaleFactorFromZmumu_Summer12_Puweights_minBiasXsec71000_observed_ichep_12JuneJSON.root"
      ### /afs/cern.ch/user/d/deguio/public/Hgg/vtxIdScaleFactorFromZmumu_Summer12_Puweights_minBiasXsec69000_observed_ichep_8JuneJSON.root
      self.n_categories = 8
      ### self.eff = "../AnalysisScripts/aux/vertex_efficiency_mva.root"
      ### self.outfile = "vertex_reweighing_mva.root"
      self.eff = "../AnalysisScripts/aux/vertex_efficiency_mva_HCP2012_unblind.root"
      self.outfile = "vertex_reweighing_mva_HCP2012_unblind.root"
      
# prevent ROOT from parsing command line
from ROOT import *
from math import sqrt

o=options()

gStyle.SetMarkerSize(1.5)
gROOT.SetBatch(True)

fr = TFile.Open(o.eff_ratio)
eff_ratio = fr.Get("scaleFactor")

eff_ratio_func = TF1("eff_ratio_func", "[0]+([1] + [2]*x)*(x<=20)",0,200)
eff_ratio.Fit(eff_ratio_func)

fe = TFile.Open(o.eff)
mc_eff = fe.Get("efficiency")

pass_rewei = mc_eff.Clone("pass_rewei")
pass_rewei.SetTitle("Wrong vertex reweighing; p_{T}(#gamma #gamma); Weight")
fail_rewei = mc_eff.Clone("fail_rewei") 
fail_rewei.SetTitle("Wrong vertex reweighing; p_{T}(#gamma #gamma); Weight")


lfwe = 0.
for i in range(mc_eff.GetN()):
    if mc_eff.GetX()[i] != eff_ratio.GetX()[i]:
         print "Efficeincy and ratio have different binning %d %f %f" % ( i, mc_eff.GetX()[i] , eff_ratio.GetX()[i] )
         sys.exit(1)
        
    pw  = eff_ratio.GetY()[i]
    pwe = eff_ratio.GetErrorY(i)

    x = mc_eff.GetX()[i]
    xe = mc_eff.GetErrorX(i)

    fpw  = eff_ratio_func.Eval(x)
    ## print pw - fpw
    ## pwe = 1.e-2

    eff  = mc_eff.GetY()[i]
    ### effe = mc_eff.GetErrorY(i)
    if( x < 40 ):
       effe = 0.005
    else:
       effe = 0.001
    ## if eff < 0.95 and eff*pw <0.95:
    if eff < 0.98:
        fw  = (1. - eff*pw)  / (1. - eff)
        fwe = sqrt( (1.-fw)*(1.-fw)*pwe*pwe + fw*fw/((1-eff)*(1.-eff))*effe*effe )
        lfwe = fwe
    else:
        fw  = 1.
        try:
            fwe = sqrt( (1.-fw)*(1.-fw)*pwe*pwe + fw*fw/((1-eff)*(1.-eff))*effe*effe )
        except:
            fwe = sqrt( (1.-fw)*(1.-fw)*pwe*pwe + fw*fw/(0.02*0.02)*effe*effe )
        ## fwe = lfwe

    pass_rewei.SetPoint(i,x,pw)
    pass_rewei.SetPointError(i,xe,xe,pwe,pwe)
    fail_rewei.SetPoint(i,x,fw)
    fail_rewei.SetPointError(i,xe,xe,fwe,fwe)

    print "x = %1.1f pass_w = %1.2f +- %1.2f fail_w = %1.2f +- %1.2f " % ( x, pw, pwe, fw, fwe ) 
    
fout = TFile.Open(o.outfile, "recreate")
fout.cd()

for c in range(o.n_categories):
    pass_rewei.Clone("ratioVertex_cat%d_pass" % c).Write()
    fail_rewei.Clone("ratioVertex_cat%d_fail" % c).Write()

fout.Close()
fr.Close()

gROOT.Reset()
