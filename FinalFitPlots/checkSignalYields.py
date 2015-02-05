import ROOT
import sys,numpy

f = sys.argv[1]
if len(sys.argv)<2: 
	sys.exit("checkSignalYields.py usage:   python checkSignalYields.py NameOfFile.root (-interp for interpolated workspaces)")
doInterp=False
if len(sys.argv)>2:
	if sys.argv[2]=="-interp":
		doInterp=True

fil = ROOT.TFile(f)
print "Checking Signal / Background for - ", f
#m = [115,120,121,123,125,130,135,140,150]

if doInterp:
  m = numpy.arange(110,150.5,0.5)
  for mass in m :
	ggh = fil.Get("th1f_sig_ada_ggh_%3.1f_cat0"%mass)
	vbf = fil.Get("th1f_sig_ada_vbf_%3.1f_cat0"%mass)
	wzh = fil.Get("th1f_sig_ada_wzh_%3.1f_cat0"%mass)
	tth = fil.Get("th1f_sig_ada_tth_%3.1f_cat0"%mass)
	h = ggh.Clone()
	h.Add(vbf)
	h.Add(wzh)
	h.Add(tth)
	hB = fil.Get("th1f_bkg_ada_%3.1f_cat0"%mass)
	hD = fil.Get("th1f_data_ada_%3.1f_cat0"%mass)
	print "Mass-%3.1f Signal     (ada) -> "%mass ,h.Integral(), "Mass-%3.1f Background (ada) -> "%mass ,hB.Integral(), " S/B ",h.Integral()/hB.Integral() \
	       ," Data ", hD.Integral()
  print 
  for mass in m :
	ggh = fil.Get("th1f_sig_grad_ggh_%3.1f_cat0"%mass)
	vbf = fil.Get("th1f_sig_grad_vbf_%3.1f_cat0"%mass)
	wzh = fil.Get("th1f_sig_grad_wzh_%3.1f_cat0"%mass)
	tth = fil.Get("th1f_sig_grad_tth_%3.1f_cat0"%mass)
	h = ggh.Clone()
	h.Add(vbf)
	h.Add(wzh)
	h.Add(tth)
	hB = fil.Get("th1f_bkg_grad_%3.1f_cat0"%mass)
	hD = fil.Get("th1f_data_grad_%3.1f_cat0"%mass)
	print "Mass-%3.1f Signal     (grad) -> "%mass ,h.Integral(), "Mass-%3.1f Background (grad) -> "%mass ,hB.Integral(), " S/B ",h.Integral()/hB.Integral() \
	       ," Data ", hD.Integral()

else:
  m = [110,115,120,125,130,135,140,150]
  for mass in m :
	ggh = fil.Get("th1f_sig_ada_ggh_%d.0_%d.0_cat0"%(mass,mass))
	vbf = fil.Get("th1f_sig_ada_vbf_%d.0_%d.0_cat0"%(mass,mass))
	wzh = fil.Get("th1f_sig_ada_wzh_%d.0_%d.0_cat0"%(mass,mass))
	tth = fil.Get("th1f_sig_ada_tth_%d.0_%d.0_cat0"%(mass,mass))
	h = ggh.Clone()
	h.Add(vbf)
	h.Add(wzh)
	h.Add(tth)
	hB = fil.Get("th1f_bkg_mc_ada_%d.0_cat0"%(mass))
	hD = fil.Get("th1f_data_ada_%d.0_cat0"%(mass))
	print "Mass-%d Signal     (ada) -> "%mass ,h.Integral(), "Mass-%d Background (ada) -> "%mass ,hB.Integral(), " S/B ",h.Integral()/hB.Integral() \
	       ," Data ", hD.Integral()
  print
  for mass in m :
	ggh = fil.Get("th1f_sig_grad_ggh_%d.0_%d.0_cat0"%(mass,mass))
	vbf = fil.Get("th1f_sig_grad_vbf_%d.0_%d.0_cat0"%(mass,mass))
	wzh = fil.Get("th1f_sig_grad_wzh_%d.0_%d.0_cat0"%(mass,mass))
	tth = fil.Get("th1f_sig_grad_tth_%d.0_%d.0_cat0"%(mass,mass))
	h = ggh.Clone()
	h.Add(vbf)
	h.Add(wzh)
	h.Add(tth)
	hB = fil.Get("th1f_bkg_mc_grad_%d.0_cat0"%(mass))
	hD = fil.Get("th1f_data_grad_%d.0_cat0"%(mass))
	print "Mass-%d Signal     (grad) -> "%mass ,h.Integral(), "Mass-%d Background (grad) -> "%mass ,hB.Integral(), " S/B ",h.Integral()/hB.Integral() \
	       ," Data ", hD.Integral()
