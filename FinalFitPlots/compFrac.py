import ROOT

fin = ROOT.TFile("/afs/cern.ch/user/c/capalmer/public/forMatt/CMS-HGG_splitwzh.root")

for cat in range(0,9):
  wh = fin.Get('th1f_sig_wh_mass_m125_cat%d'%cat)
  zh = fin.Get('th1f_sig_zh_mass_m125_cat%d'%cat)
  print cat, wh.Integral(), zh.Integral(), wh.Integral()/(wh.Integral()+zh.Integral())
