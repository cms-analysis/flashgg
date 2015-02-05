import ROOT
import sys
import re
import logging

# Things in the map below get summed, so do not put qq_grav and gg_grav on the BSM line unless you want their sum.
procMap={
    'SM':('ggh', 'vbf', 'wzh', 'wh', 'zh', 'tth'),
    'BSM':('gg_grav',), 
    }

types = ('baseline','massfact','spin')

def makeLabels():
    aDict={}
    #one day this could be made with itertools...
    aDict['baseline'] = (
	"cat0",
	"cat1",
	"cat2",
	"cat3",
	"cat4",
	"cat5",
	"cat6",
	"cat7",
	"cat8",
	"cat9",
	"cat10",
	"cat11",
	"cat12",
	"cat13",
	"cat14",
	"cat15",
    )
    aDict['baseline7TeV'] = (
        #"Both photons in barrel, min(R_{9}) > 0.94",
        #"Both photons in barrel, min(R_{9}) < 0.94",
        #"One or more photons in endcap, min(R_{9}) > 0.94",
        #"One or more photons in endcap, min(R_{9}) < 0.94",
#		"Dijet-tagged",
	"cat0",
	"cat1",
	"cat2",
	"cat3",
	"cat4",
	"cat5",
	"cat6",
	"cat7",
	"cat8",
	"cat9",
	"cat10",
	"cat11",
	"cat12",
	"cat13",
	"cat14",
    )
    aDict['massfact7TeV'] = ( 
		"Untagged 0",
		"Untagged 1",
		"Untagged 2",
		"Untagged 3",
		"Dijet Tag 0",
		"Dijet Tag 1",
		"VH Lepton Tight",
		"VH Lepton Loose",
		"VH MET Tag",
		"ttH  Tag",
		"VH Hadronic Tag"
    )
    aDict['massfact'] = ( 
		"Untagged 0",
		"Untagged 1",
		"Untagged 2",
		"Untagged 3",
		"Untagged 4",
		"Dijet Tag 0",
		"Dijet Tag 1",
		"Dijet Tag 2",
		"VH Lepton Tight",
		"VH Lepton Loose",
		"VH MET Tag",
		"ttH Leptonic Tag",
		"ttH Hadronic Tag",
		"VH Hadronic Tag"
    )
    aDict['spin'] = (
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) > 0.94}{|cos(#theta*)| < 0.2}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) > 0.94}{0.2 < |cos(#theta*)| < 0.375}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) > 0.94}{0.375 < |cos(#theta*)| < 0.55}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) > 0.94}{0.55 < |cos(#theta*)| < 0.75}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) > 0.94}{|cos(#theta*)| > 0.75}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) < 0.94}{|cos(#theta*)| < 0.2}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) < 0.94}{0.2 < |cos(#theta*)| < 0.375}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) < 0.94}{0.375 < |cos(#theta*)| < 0.55}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) < 0.94}{0.55 < |cos(#theta*)| < 0.75}",
        "#splitline{max(|#eta|) < 1.44, min(R_{9}) < 0.94}{|cos(#theta*)| > 0.75}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) > 0.94}{|cos(#theta*)| < 0.2}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) > 0.94}{0.2 < |cos(#theta*)| < 0.375}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) > 0.94}{0.375 < |cos(#theta*)| < 0.55}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) > 0.94}{0.55 < |cos(#theta*)| < 0.75}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) > 0.94}{|cos(#theta*)| > 0.75}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) < 0.94}{|cos(#theta*)| < 0.2}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) < 0.94}{0.2 < |cos(#theta*)| < 0.375}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) < 0.94}{0.375 < |cos(#theta*)| < 0.55}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) < 0.94}{0.55 < |cos(#theta*)| < 0.75}",
        "#splitline{max(|#eta|) > 1.44, min(R_{9}) < 0.94}{|cos(#theta*)| > 0.75}",
    )
    return aDict
    

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
    return (procs, massesRet, catsRet)
	
	
def getSigHistos(f, procs, suffix):
    slurpDic = { name : f.Get('th1f_sig_'+name+suffix) for name in procs}
    # filter out histos that are null pointers
    return { k : v for k, v in slurpDic.iteritems() if v }

def doBandsFit(hmass, cpdf, nomcurve, datanorm, plot, catname, everyN):

    onesigma = ROOT.TGraphAsymmErrors()
    twosigma = ROOT.TGraphAsymmErrors()
    
    nlim = ROOT.RooRealVar('nlim'+catname,'',0.0,0.0,1e+5)

    clOne = 1.0 - 2.0*ROOT.RooStats.SignificanceToPValue(1.0)
    errLevelOne = 0.5*pow(ROOT.Math.normal_quantile(1-0.5*(1-clOne),1.0), 2)
    # the 0.5 is because qmu is -2*NLL
    # eventually if cl = 0.95 this is the usual 1.92! 

    clTwo = 1.0 - 2.0*ROOT.RooStats.SignificanceToPValue(2.0)
    errLevelTwo = 0.5*pow(ROOT.Math.normal_quantile(1-0.5*(1-clTwo),1.0), 2)
    
    for i, iBin in enumerate( xrange( 1, plot.GetXaxis().GetNbins()+1, everyN) ):
        lowedge = plot.GetXaxis().GetBinLowEdge(iBin)
        upedge  = plot.GetXaxis().GetBinUpEdge(iBin)
        center  = plot.GetXaxis().GetBinCenter(iBin)

        nombkg = nomcurve.interpolate(center)

        nlim.setVal(nombkg)
        hmass.setRange('errRange',lowedge, upedge)

        epdf = ROOT.RooExtendPdf('epdf','',cpdf,nlim,'errRange')
        nll = epdf.createNLL(datanorm, ROOT.RooFit.Extended())
        minim = ROOT.RooMinimizer(nll)
        minim.setStrategy(0)
        minim.setPrintLevel(-1)


        minim.setErrorLevel( errLevelOne )
        minim.migrad()
        minim.minos(ROOT.RooArgSet(nlim))

        onesigma.SetPoint(i,center,nombkg)
        onesigma.SetPointError(i,0,0,-nlim.getErrorLo(), nlim.getErrorHi())
        
        minim.setErrorLevel( errLevelTwo )
        minim.migrad()
        minim.minos(ROOT.RooArgSet(nlim))

        twosigma.SetPoint(i,center,nombkg)
        twosigma.SetPointError(i,0,0,-nlim.getErrorLo(),nlim.getErrorHi())

    return (onesigma, twosigma)
        
def main(o):
    from pprint import pprint

    ROOT.gROOT.SetStyle('Plain')
    ROOT.gROOT.SetBatch(1)
    ROOT.gStyle.SetOptStat(0)
    ROOT.gErrorIgnoreLevel = ROOT.kWarning

    ROOT.gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so");
    outf = ROOT.TFile.Open('BkgPlotCanvs.root', 'RECREATE')

    typ = ROOT.RooFit.FATAL
    lvl = ROOT.RooFit.MsgLevel(typ)
    svc = ROOT.RooMsgService.instance()
    svc.setSilentMode(True)
    # need to find way to make this work better in python
    # it should be possible: http://svn.cern.ch/guest/lhcb/Analysis/trunk/Phys/AnalysisPython/python/AnalysisPython/Utils.py
    #svc.setGlobalKillBelow(lvl)
    svc.setGlobalKillBelow(3)

    labels = makeLabels()[o.type]

    fb = ROOT.TFile.Open(o.fileBkg)
    fs = ROOT.TFile.Open(o.fileSig) if o.fileSig else fb

    (procs, masses, cats) = preFlight(fs)

    logging.info('Processes found: %s' % str(procs))
    logging.info('Masses found: %s' % str(masses))
    logging.info('Found %d categories' % len(cats))

    if not cats or not procs or not masses:
        raise RuntimeError('No categories, no processes or, no masses. Cannot continue.')
    
    if float(o.mass) not in masses:
        raise RuntimeError('Could not find mH="%s" among %s in input file %s'%(o.mass, str(masses), fs.GetName()))
    
    if len(labels) != len(cats):
         raise RuntimeError('I know of %d labels for type="%s", but in input file %s I found %d categories'%(len(labels), o.type, fs.GetName(), len(cats)))

    w_bkg = fb.Get('cms_hgg_workspace')

    x = w_bkg.var('CMS_hgg_mass')
    intL = w_bkg.var('IntLumi')
    lumi = intL.getVal()/1e3

    latex = ROOT.TLatex()
    latex.SetTextSize(0.025)
    latex.SetNDC()

    cmslatex = ROOT.TLatex()
    cmslatex.SetTextSize(0.035)
    cmslatex.SetNDC()
    
    for cat in cats:
        can = ROOT.TCanvas('c'+str(cat),'',800,800)
        leg = ROOT.TLegend(0.6,0.6,0.89,0.89)
        leg.SetFillColor(0)
        leg.SetBorderSize(0)

        data = w_bkg.data('data_mass_cat'+str(cat)) if o.useBinnedData else w_bkg.data('roohist_data_mass_cat'+str(cat))
        #data.Print()

        #-# The following is anything but Generic
	if '7TeV' in o.type:
        	bkg = w_bkg.pdf('pdf_data_pol_model_7TeV_cat'+str(cat))
	else:
        	bkg = w_bkg.pdf('pdf_data_pol_model_8TeV_cat'+str(cat))
        #bkg.Print()
        bkg.fitTo(data)
        r = bkg.fitTo(data, ROOT.RooFit.Save(1) )
        #r.Print()

        logging.debug( 'Getting Signal Components for category %d' % cat )

        allsig={}
        for lab, procList in procMap.iteritems():
            sigs = getSigHistos(fs, procList, '_mass_m'+o.mass+'_cat'+str(cat))
            if not sigs:
                allsig[lab]=None
                continue
            hs = [ h for (proc, h) in sigs.iteritems() ]
            h=hs[0]
            for j in hs[1:]:
                h.Add(j)
            allsig[lab] = h.Clone(lab)
                
        allsig['SM'].Rebin(2)
        allsig['SM'].SetLineColor(4)
        allsig['SM'].SetFillColor(38)
        allsig['SM'].SetFillStyle(3001)
        allsig['SM'].SetLineWidth(2)

        if allsig['BSM'] and o.type=='spin':
            allsig['BSM'].Rebin(2)
            allsig['BSM'].SetLineColor(ROOT.kRed)
            allsig['BSM'].SetLineWidth(2)
            allsig['SM'].SetLineColor(38)
            for k in procMap.keys():
                allsig[k].Scale(5)

        dumData = ROOT.TH1F('d'+str(cat),'',80,100,180)
        dumData.Sumw2()
        dumData.SetMarkerSize(1.0)
        dumData.SetMarkerStyle(20)
        dumData.SetLineWidth(3)
        dumData.Fill(101)

        dum1Sig = ROOT.TH1F ('1s'+str(cat),'',80,100,180)
        dum1Sig.SetFillColor(ROOT.kYellow)
        dum1Sig.SetFillStyle(1001)
        dum2Sig = ROOT.TH1F ('2s'+str(cat),'',80,100,180)
        dum2Sig.SetFillColor(ROOT.kGreen)
        dum2Sig.SetFillStyle(1001)
        dumBkg = ROOT.TH1F('b'+str(cat),'',80,100,180)
        dumBkg.SetLineColor(ROOT.kRed)
        dumBkg.SetLineWidth(3)
        dumBkg.Draw("P")
        dumBkg.Draw("Fsame")
        dum1Sig.Draw("Fsame")
        dum2Sig.Draw("Lsame")

        frame = x.frame()

        logging.debug( 'Plotting components for category %d' % cat )
        data.plotOn(frame,ROOT.RooFit.Binning(80),ROOT.RooFit.Invisible())
        bkg.plotOn(frame,ROOT.RooFit.LineColor(ROOT.kRed))

        if o.doBands:
            nomcurve = ROOT.RooCurve(frame.getObject(int(frame.numItems()-1)))
            skipPoints = 1 if o.final else frame.GetXaxis().GetNbins()/10
            
            (onesigma, twosigma) = doBandsFit(x, bkg, nomcurve, data, frame, 'cat'+str(cat), skipPoints)

        if o.blind:
            x.setRange('unblind_up',150,180)
            data.plotOn(frame,ROOT.RooFit.Binning(80),ROOT.RooFit.CutRange('unblind_up'))
            x.setRange('unblind_down',100,110)
            data.plotOn(frame,ROOT.RooFit.Binning(80),ROOT.RooFit.CutRange('unblind_down'))
        else:
            data.plotOn(frame,ROOT.RooFit.Binning(80))
        
        frame.SetTitle('')
        frame.GetXaxis().SetTitle('m_{#gamma#gamma} (GeV)')
        frame.GetXaxis().SetNdivisions(5,5,0)
        frame.GetYaxis().SetTitle('Events / (1 Gev)')
        frame.GetYaxis().SetTitleOffset(1.2)

        leg.AddEntry(dumData, 'Data', 'PEL')
        leg.AddEntry(dumBkg,  'Bkg model', 'L')
        leg.AddEntry(dum1Sig, '#pm 1#sigma Bkg model','F')
        leg.AddEntry(dum2Sig, '#pm 2#sigma Bkg model','F')

        if allsig['BSM'] and o.type=='spin':
            leg.AddEntry(allsig['SM'], '0^{+} #times5, m_{H} = %3.1f GeV' % float(o.mass), 'F')
            leg.AddEntry(allsig['BSM'],'2^{+}_{m}(gg) #times5, m_{H} = %3.1f GeV' % float(o.mass), 'F')
        else:
            leg.AddEntry(allsig['SM'], '1#times SM, m_{H} = %3.1f GeV' % float(o.mass), 'F')

        frame.Draw()
        frame.SetMinimum(1e-4)
        if o.doBands:
            twosigma.SetLineColor(ROOT.kGreen)
            twosigma.SetFillColor(ROOT.kGreen)
            twosigma.SetMarkerColor(ROOT.kGreen)
            twosigma.Draw('L3 same')

            onesigma.SetLineColor(ROOT.kYellow)
            onesigma.SetFillColor(ROOT.kYellow)
            onesigma.SetMarkerColor(ROOT.kYellow)
            onesigma.Draw('L3 same')

            frame.Draw('same')

        allsig['SM'].Draw('samehistF')
        if allsig['BSM'] and o.type=='spin':
            allsig['BSM'].Draw('samehistF')
        
        leg.Draw()
        #cmslatex.DrawLatex(0.2,0.85,'#splitline{CMS Preliminary}{#sqrt{s} = 8 TeV L = %2.1f fb^{-1}}'%lumi)
        #latex.DrawLatex(0.2,0.75,labels[cat])
        cmslatex.DrawLatex(0.1,0.905,'CMS Preliminary   #sqrt{s} = 8 TeV, L = %2.1f fb^{-1}'%lumi)
        latex.DrawLatex(0.2,0.82,labels[cat])

        canName = o.type+'cat'+str(cat)
        can.SetName(canName)
        outf.cd()
        can.Write()
        can.SaveAs(canName+'.pdf')
        if o.final:
            can.SaveAs(canName+'.png')
            can.SaveAs(canName+'.C')
                
    #-# missing jet id syst part
    if not o.type=='spin':
        logging.warn( 'Not calculating the jet id syst part; counters not implemented yet' )
        pass
        
    outf.Close()
        
    fb.Close()
    fs.Close()

if __name__ == "__main__":

    from optparse import OptionParser
    parser = OptionParser(usage='%prog -b fileBkg.root',
                          version='%prog 3.14')

    parser.add_option('-t', '--type', type='string', default='baseline',
                      help='Type of analysis', metavar='|'.join(types))
    parser.add_option('-b', '--fileBkg', type='string',
                      help='Background file', metavar='FILE')
    parser.add_option('-s', '--fileSig', type='string', default='',
                      help='Signal file', metavar='FILE')
    parser.add_option('--unblind', action='store_false', dest='blind', default=True,
                      help='Unblind signal region.')
    parser.add_option('--blind', action='store_true', dest='blind', default=True,
                      help='Blind signal region.')
    parser.add_option('--useBinnedData', action='store_true', default=False)
    parser.add_option('--doBands', action='store_true', default=False,
                      help='Create bkg uncertainty bands.')
    parser.add_option('--final', action='store_true', default=False,
                      help='Do not skip points when making bkg bands and produce more plot file formats (slower).')
    parser.add_option('--mass', type='string', default='125', metavar='MASS',
                      help='Set the mH value to use.')
    parser.add_option('-l', '--log', default='INFO', metavar='LEVEL',
                      help='Set the minimum logging level.')
    
    (o, args) = parser.parse_args()

    if not o.fileBkg:
        parser.error("Please specify (at least one) an input file.")

    logging.basicConfig(level=getattr(logging, o.log.upper()))

    logging.debug('%s'% str(o))
    
    sys.exit(main(o))
