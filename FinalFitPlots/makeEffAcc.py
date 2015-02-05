# Simple script to make Effiency X Acceptance plot from Binned Baseline/Massfac analysis
# run with python makeEffAcc.py CMS-HGG.root
import ROOT
import sys
import re

procOrder=('ggh', 'vbf', 'wzh', 'wh', 'zh', 'tth')

adHocFactors={
	'ggh': 0.975,
	'vbf': 1.0,
	'wzh': 1.0,
	'wh': 1.0,
	'zh': 1.0,
	'tth': 1.0,
}


def preFlight(f):
	foundSplit = foundMerged = False
	procs = set()
	masses = set()
	cats = set()
	for i in f.GetListOfKeys():
		match = re.search('sig_(?P<proc>\w+)_mass_m(?P<mass>[0-9]*\.?[0-9]+)_.*_cat(?P<cat>[0-9]+)$', i.GetName())
		if match:
			d = match.groupdict()
			float(d['mass'])
			procs.add(d['proc'])
			masses.add(d['mass'])
			cats.add(d['cat'])

	if 'wzh' in procs and ('wh' in procs or 'zh' in procs) :
		raise RuntimeError('Bailing out: found both wh/zh and wzh in '+f.GetName())

	massesRet = sorted([ float(m) for m in masses ])
	catsRet = sorted([ int(cat) for cat in cats ])
	procsRet = sorted(procs, key=lambda x: procOrder.index(x))
	return (procsRet, massesRet, catsRet)
	
	
def getSigHistos(f, procs, suffix):
    slurpDic = { name : f.Get('th1f_sig_'+name+suffix) for name in procs}
    # filter out histos that are null pointers
    return { k : v for k, v in slurpDic.iteritems() if v }



#### ROOT.gROOT.ProcessLine(".L Normalization_7TeV.C++")
### ROOT.gROOT.ProcessLine(".L Normalization_8TeV.C++")
### from ROOT import GetBR
### from ROOT import GetXsection
### GetProcXsection = GetXsection

#ROOT.gSystem.Load("$CMSSW_BASE/lib/slc5_amd64_gcc472/libHiggsAnalysisCombinedLimit.so")
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
ROOT.gSystem.Load("../libLoopAll")
from ROOT import Normalization_8TeV
norm = Normalization_8TeV()  # Should be checking if 7TeV or 8TeV signal, default is 8TeV here
#norm.Init(8)
GetBR = lambda x : norm.GetBR(float(x))
GetXsection = lambda x : norm.GetXsection(float(x))
GetProcXsection = lambda x,y : norm.GetXsection(x,y)

# ROOT Setup
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(1)

# Global Setup, Modify with each Reload
##### NCAT = 5
##### lumi = 5089
#lumi=3770
#systematics = ["vtxEff","idEff","E_scale","E_res","triggerEff","regSig","phoIdMva"] # These are the main contributions to eff*Acc
systematics = ["vtxEff","idEff","E_scale","E_res","triggerEff"] # These are the main contributions to eff*Acc
## Masses = range(110,152,2) 
Masses = range(110,151,1) 
# -------------------------------------------------------------

f = ROOT.TFile(sys.argv[1])

(procs, masses, cats) = preFlight(f)

# Get The lumi from the workspace!
ws = f.Get("cms_hgg_workspace")
lRRV = ws.var("IntLumi")
lumi = lRRV.getVal()
sqrts = (ws.var("Sqrts")).getVal()
#print sqrts
norm.Init(int(sqrts))

# Some helpful output
print "File - ", sys.argv[1]
print 'Processes found:  ' + str(procs)
print 'Masses found:     ' + str(masses)
print 'Categories found: ' + str(cats)

printLine = "Data:      "
Sum = 0
for i in cats:
  h = f.Get("th1f_data_mass_cat%d"%i)
  print "%d   %4.0f    %4.0f" % (i, h.Integral(1,160), h.Integral(21,100) )
  Sum+=h.Integral()
  printLine+="%3.0f"%(h.Integral())+" "
printLine+="tot=%d"%Sum
print printLine


efficiency=ROOT.TGraphAsymmErrors()
central=ROOT.TGraphAsymmErrors()
efficiencyup=ROOT.TGraphAsymmErrors()
efficiencydn=ROOT.TGraphAsymmErrors()
centralsmooth=ROOT.TGraphAsymmErrors()

fitstring = "[0] + [1]*x + [2]*x*x"
cenfunc = ROOT.TF1("cenfunc",fitstring,109.75,140.25)
upfunc = ROOT.TF1("upfunc",fitstring,109.75,140.25)
dnfunc = ROOT.TF1("dnfunc",fitstring,109.75,140.25)


for point,M in enumerate(Masses):
	printLine = "Signal M%3.1f: "%M
	Sum = 0
	for i in cats:
		if int(M)==M:
			suffix = '_mass_m%d_cat%d'%(int(M),i)
			histos = getSigHistos(f, procs, suffix)

			integrals = { proc : h.Integral() for (proc, h) in histos.iteritems()}

			procLine = 'cat %d, mH=%3.1f:'%(i, M)
			for proc in procs:
				integral = integrals[proc]
				procLine += '   %s %.5f'% (proc, 100*integral/(GetBR(M)*( GetProcXsection(M,proc)*adHocFactors[proc] )*lumi) )
			print procLine

			hs = [ h for (proc, h) in histos.iteritems() ]
			h=hs[0]
			for j in hs[1:]:
				h.Add(j)
		else:
			h = f.Get("th1f_sig_mass_m%.1f_cat%d"%(M,i))
		
		Sum += h.Integral()
		printLine+="%3.5f "%h.Integral()
	printLine+="tot=%3.5f"%Sum
  
	xsecs = [ GetProcXsection(M,proc)*adHocFactors[proc] for proc in procs ]
	sm = GetBR(M) * sum(xsecs)
	
	effAcc = 100*Sum/(sm*lumi) # calculate Efficiency at mH
	centralsmooth.SetPoint(point,M,effAcc)
	central.SetPoint(point,M,effAcc)
	efficiency.SetPoint(point,M,effAcc)
	delUp = 0
	delDown = 0
	for s in systematics:
		syssumup=0
		syssumdn=0
		for i in cats:
			if int(M)==M:
				suffix = '_mass_m%d_cat%d_%sUp01_sigma'%(int(M),i,s)
				histosup = getSigHistos(f, procs, suffix)
				hs = [ h for (proc, h) in histosup.iteritems() ]
				hup=hs[0]
				for j in hs[1:]:
					hup.Add(j)

				suffix = '_mass_m%d_cat%d_%sDown01_sigma'%(int(M),i,s)
				histosdn = getSigHistos(f, procs, suffix)
				hs = [ h for (proc, h) in histosdn.iteritems() ]
				hdn=hs[0]
				for j in hs[1:]:
					hdn.Add(j)
			else:
				hup = f.Get("th1f_sig_mass_m%.1f_cat%d_%sUp01_sigma"%(M,i,s))
				hdn = f.Get("th1f_sig_mass_m%.1f_cat%d_%sDown01_sigma"%(M,i,s))
			syssumup+=hup.Integral()
			syssumdn+=hdn.Integral()

		# We make 3-sigma templates so need to scale back by 1/3
		delUp+=abs(syssumup-Sum)/3
		delDown+=abs(syssumdn-Sum)/3
   
	delUp=100*(delUp**0.5)/(sm*lumi)
	delDown=100*(delDown**0.5)/(sm*lumi)
	efficiencyup.SetPoint(point,M,delUp)
	efficiencydn.SetPoint(point,M,delDown)
	centralsmooth.SetPointError(point,0,0,0,0)
	efficiency.SetPointError(point,0,0,delDown,delUp)

	print printLine

centralsmooth.Fit(cenfunc,"R,0,EX0","")
efficiencyup.Fit(upfunc,"R,0,EX0","")
efficiencydn.Fit(dnfunc,"R,0,EX0","")

for point,M in enumerate(Masses):
	central.SetPoint(point,M,cenfunc.Eval(M))
	efficiency.SetPoint(point,M,cenfunc.Eval(M))

leg=ROOT.TLegend(0.46,0.16,0.79,0.39)
leg.SetFillColor(0)
leg.SetBorderSize(0)
leg.AddEntry(central,"Higgs Signal #varepsilon #times Acc","L")
leg.AddEntry(efficiency,"#pm 1 #sigma syst. error","F")

mytext = ROOT.TLatex()
mytext.SetTextSize(0.04)
mytext.SetNDC()

listy = []

MG=ROOT.TMultiGraph()
can = ROOT.TCanvas()
efficiency.SetFillColor(9)
efficiency.SetLineWidth(2)
central.SetLineWidth(2)
MG.Add(efficiency)
MG.Add(central)
MG.Draw("AL3")
MG.GetXaxis().SetTitle("m_{H} GeV")
MG.GetYaxis().SetTitle("Efficiency #times Acceptance - %")
mytext.DrawLatex(0.15,0.8,"CMS Simulation")
can.Update()
leg.Draw("same")
print "Int Lumi from workspace ", lumi
raw_input("Looks OK?")

can.Update()
print "Saving plot as effAcc_vs_mass.pdf"
can.SaveAs("effAcc_vs_mass.pdf")
can.SaveAs("effAcc_vs_mass.png")
can.SaveAs("effAcc_vs_mass.root")

  

	
