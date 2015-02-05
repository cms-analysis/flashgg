#!/bin/env python

from optparse import OptionParser, make_option
import sys, json, copy, itertools, os.path, os

from math import sqrt,fabs

import rootglobestyle

class Helper:
    def __init__(self):
        self.histos = {}
        self.objs = []
        self.files = []
        self.dsets = []

    def add(self,val,nam):
        self.histos[nam] = val

# doesn't work yet
#def apply_modifs(obj,modifs):
#    for mod in modifs:
#        if type(mod) != tuple:
#            if type(mod) == str:
#                mod = obj.__getattribute__(mod)
#                mod(obj)
#        else:
#            (func,args) = mod
#            if type(func) == str:
#                obj.__getattribute__(func)(args)
#            else:
#                try:
#                    func(obj,args)
#                except Exception, e:
#                    func(obj,*args)
#
def build_pdf(ws,cat,ngaus=3,final=True):
    name = "hggpdfrel%s_%d" % (cat,ngaus)
    if final:
        name = "hggpdfrel%s" % cat
    buildstr = ""
    for igaus in range(ngaus):
        if igaus != 0:
            buildstr += ","
        buildstr += "f%(igaus)d%(cat)s[0,100.]*Gaussian::g%(igaus)d%(cat)s(CMS_hgg_mass,m%(igaus)d%(cat)s[100,150],s%(igaus)d%(cat)s[0,10]) " % { "igaus":igaus, "cat":"%(cat)s" }
    print buildstr
    ws.factory( ("SUM::%(name)s("+buildstr+")") % {"cat":cat, "name":name})
    ### ws.factory("SUM::%(name)s("
    ###            "f0%(cat)s[0,100.]*Gaussian::g0%(cat)s(CMS_hgg_mass,m0%(cat)s[100,150],s0%(cat)s[0,10]), "
    ###            "f1%(cat)s[0,50.]*Gaussian::g1%(cat)s(CMS_hgg_mass,m1%(cat)s[100,150],s1%(cat)s[0,10]), "
    ###            "f2%(cat)s[0,10.]*Gaussian::g2%(cat)s(CMS_hgg_mass,m2%(cat)s[100,150],s2%(cat)s[0,10])  "
    ###            ")" % {"cat":cat, "name":name})
    return ws.pdf(name)


def get_eff_sigma( mass, binned_pdf, binned_cdf, wmin=110, wmax=130, step=0.002, epsilon=1.e-4 ):
    point = wmin
    points = []
    while point<= wmax:
        mass.setVal( point )
        if binned_pdf.getVal() > epsilon:
            points.append( (point, binned_cdf.getVal() ) )
        point = point + step

    low = wmin
    high = wmax
    width = wmax - wmin 
    for ip in range(0, len(points) ):
        for jp in range(ip, len(points) ):
            wy = ( points[jp][1] - points[ip][1] )
            if fabs( wy - 0.683 ) < epsilon:
                wx = ( points[jp][0] - points[ip][0] )
                if wx < width:
                    low = points[ip][0]
                    high =  points[jp][0]
                    width = wx

    print low, high, width
    return low,high

def get_FWHM( mass, binned_pdf, binned_cdf, wmin=110, wmax=130, step=0.002, epsilon=1.e-4 ):
    from ROOT import TH1F, RooArgList
    nbins = (wmax-wmin)/step
    h = TH1F("h","h",int(nbins),wmin,wmax)
    binned_pdf.fillHistogram(h,RooArgList(mass))
    hm = h.GetMaximum()*0.5 
    low = h.GetBinCenter(h.FindFirstBinAbove(hm))
    high = h.GetBinCenter(h.FindLastBinAbove(hm))

    print low, high, hm
    return low, high, hm


def main(options,args):

    from ROOT import gSystem, gROOT, gStyle
    gROOT.SetBatch()
    gSystem.Load("libRooFitCore")

    if options.doWebPage:
        from lip.Tools.rootutils import loadToolsLib, apply_modifs
        loadToolsLib()
    
    from ROOT import TFile, RooFit, RooArgSet, RooDataHist, RooKeysPdf, RooHistPdf, TCanvas, TLegend, TLatex, TArrow, TPaveText, RooAddPdf, RooArgList
    from ROOT import kWhite, kBlue, kOpenSquare
    if options.doWebPage:
        from ROOT import HtmlHelper, HtmlTag, HtmlTable, HtmlPlot

    rootglobestyle.setTDRStyle()
    gStyle.SetMarkerSize(1.5)
    gStyle.SetTitleYOffset(1.5)
    
    gStyle.SetPadLeftMargin(0.16)
    gStyle.SetPadRightMargin(0.05)
    gStyle.SetPadTopMargin(0.05)
    gStyle.SetPadBottomMargin(0.13)
    
    gStyle.SetLabelFont(42,"XYZ")
    gStyle.SetLabelOffset(0.007, "XYZ")
    gStyle.SetLabelSize(0.05,"XYZ")
    
    gStyle.SetTitleSize(0.06,"XYZ")
    gStyle.SetTitleXOffset(0.9)
    gStyle.SetTitleYOffset(1.24)
    gStyle.SetTitleFont(42,"XYZ")
    
    ##
    ## Read files
    ##
    options.outdir = "%s_m%1.0f" % ( options.outdir, options.mH  )
    if options.fp:
        options.outdir += "_fp"

    ncat=options.ncat
    cats=options.cats
    if cats is "":
        categories =[  "_cat%d" % i for i in range(0,ncat) ]
    else:
        categories =[  "_cat%s" % i for i in cats.split(",") ]
    
    if options.mva:
        clables = { "_cat0" : ("MVA > 0.89",""),
                    "_cat1" : ("0.74 #leq MVA","MVA < 0.89"),
                    "_cat2" : ("0.545 #leq MVA","MVA < 0.74"),
                    "_cat3" : ("0.05 #leq MVA","MVA < 0.545"),
                    "_cat4" : ("Di-jet","Tagged"),
                    "_cat5" : ("Di-jet","Tagged"),
                    "_combcat" : ("All Classes","Combined")
                    }
    else:
        clables = { "_cat0" : ("max(|#eta|<1.5","min(R_{9})>0.94"),
                    "_cat1" : ("max(|#eta|<1.5","min(R_{9})<0.94"),
                    "_cat2" : ("max(|#eta|>1.5","min(R_{9})>0.94"),
                    "_cat3" : ("max(|#eta|>1.5","min(R_{9})<0.94"),
                    "_cat4" : ("Di-jet","Tagged"),
                    "_cat5" : ("Di-jet","Tagged"),
                    "_combcat" : ("All Classes","Combined")
                    }
    helper = Helper()

    fin = TFile.Open(options.infile)
    helper.files.append(fin)
    ws = fin.Get("cms_hgg_workspace")
    mass = ws.var("CMS_hgg_mass")
    mass.SetTitle("m_{#gamma#gamma}");
    mass.setUnit("GeV");
    mass.setRange(100.,150.)
    mass.setBins(100,"plot")
    mass.setBins(5000)

    print ws
    
    aset = RooArgSet(mass)

    helper.objs.append( mass )
    helper.objs.append( aset )

    fitopt = ( RooFit.Minimizer("Minuit2", ""), RooFit.Minos(False), RooFit.SumW2Error(False), RooFit.NumCPU(8) )
        
    if not options.binned and not options.refit:
        finpdf =  TFile.Open(options.infilepdf)
        helper.files.append(finpdf)
        wspdf = finpdf.Get("wsig")
    else:
        wspdf = ws
    
    for c in categories:
        processes = [ "ggh", "vbf", "wzh" ]
        if options.fp:
            processes = [ "vbf", "wzh" ]
        ### elif clables[c][0] == "Di-jet":
        ###     processes = [ "vbf", "ggh" ]
        
        dsname = "sig_mass_m%1.0f%s" % (options.mH,c)
        print dsname
        print ws
        ds = ws.data( "sig_%s_mass_m%1.0f%s" % (processes[0],options.mH,c)  ).Clone(dsname)
        for proc in processes[1:]:
            ds.append( ws.data( "sig_%s_mass_m%1.0f%s" % (proc,options.mH,c)  ) )
        helper.dsets.append( ds )


        if options.binned:
            binned_ds = RooDataHist( "binned_%s" % dsname,"binned_%s" % dsname,aset, ds)
            pdf = RooKeysPdf( "pdf_%s_%s" % (dsname, f), "pdf_%s" % dsname, mass,  ds )
            plot_pdf = RooHistPdf( "pdf_%s" % dsname, "pdf_%s" % dsname, aset,  plot_ds )
            helper.add( binned_ds,    binned_ds.GetName()  )
        else:
            if options.refit:
                if options.refitall and clables[c][0] != "Di-jet":
                    rpdfs = []
                    for proc in processes:
                        for ngaus in range(1,4):
                            pp = build_pdf(ws,"%s_%s" % (c,proc),ngaus,ngaus==3 )
                            pp.fitTo( ws.data( "sig_%s_mass_m%1.0f%s" % (proc,options.mH,c)), RooFit.Strategy(0), *fitopt )
                        rpdfs.append(pp)
                    pdf = RooAddPdf("hggpdfrel%s" % c, "hggpdfrel%s" % c, RooArgList(*tuple(rpdfs) ))
                else:
                    if options.refitall and clables[c][0] == "Di-jet":
                        for ngaus in range(1,5):
                            pdf = build_pdf(ws,c,ngaus,ngaus==5)
                            pdf.fitTo(ds, RooFit.Strategy(0), *fitopt )
                    else:
                        for ngaus in range(1,4):
                            pdf = build_pdf(ws,c,ngaus,ngaus==3)
                            pdf.fitTo(ds, RooFit.Strategy(0), *fitopt )
            else:
                pdfs = (wspdf.pdf( "hggpdfrel%s_%s" % (c, p)) for p in processes )
                pdf = RooAddPdf("hggpdfrel%s" % c, "hggpdfrel%s" % c, RooArgList(*pdfs ))
            helper.add(pdf,pdf.GetName())
            plot_pdf = pdf.Clone("pdf_%s" % dsname)

        plot_ds = RooDataHist( "plot_%s" % dsname,"plot_%s" % dsname, aset, "plot")
        plot_ds.add( ds )

        cdf = pdf.createCdf(aset)
        hmin, hmax, hm = get_FWHM( mass, pdf, cdf, options.mH-10., options.mH+10. )
        wmin, wmax = get_eff_sigma( mass, pdf, cdf, options.mH-10., options.mH+10. )
        ### hmin, hmax, hm = get_FWHM( points )
        
        helper.add( plot_ds,      plot_ds.GetName()    )
        helper.add( plot_pdf,     plot_pdf.GetName()   )
        helper.add( (wmin,wmax),  "eff_sigma%s" % c    )
        helper.add( (hmin, hmax, hm),  "FWHM%s" % c    )
        helper.add( ds.sumEntries(), "sumEntries%s" %c ) # signal model integral

        # data integral for PAS tables
        data = ws.data( "data_mass%s"%c)
        helper.add( data.sumEntries("CMS_hgg_mass>=%1.4f && CMS_hgg_mass<=%1.4f"%(options.mH-10.,options.mH+10.)),"data_sumEntries%s"%c)

        
        del cdf
        del pdf
    

    dsname = "sig_mass_m%1.0f_combcat" % options.mH
    print dsname
    combined_ds = helper.dsets[0].Clone(dsname)
    for d in helper.dsets[1:]:
        combined_ds.append(d)
    
    if options.binned:
        binned_ds = RooDataHist( "binned_%s" % dsname,"binned_%s" % dsname,aset, combined_ds)
        pdf = RooKeysPdf( "pdf_%s" % (dsname), "pdf_%s" % dsname, mass,  combined_ds )
        plot_pdf = RooHistPdf( "pdf_%s" % dsname, "pdf_%s" % dsname, aset,  plot_ds )
        helper.add( binned_ds, binned_ds.GetName()  )
    else:
        #### pdf = build_pdf(ws,"_combcat")
        #### pdf.fitTo(combined_ds, RooFit.Strategy(0), *fitopt )
        #### plot_pdf = pdf.Clone( "pdf_%s" % dsname )
        pdf = RooAddPdf(  "pdf_%s" % dsname,  "pdf_%s" % dsname, RooArgList( *(helper.histos["hggpdfrel%s" % c] for c in categories) ) )
        plot_pdf = pdf
        
    cdf = pdf.createCdf(aset)
    
    plot_ds = RooDataHist( "plot_%s" % dsname,"plot_%s" % dsname, aset, "plot")
    plot_ds.add( combined_ds )
      
    wmin, wmax = get_eff_sigma( mass, pdf, cdf, options.mH-10., options.mH+10. )
    hmin, hmax, hm = get_FWHM( mass, pdf, cdf, options.mH-10., options.mH+10. )
    
    helper.add( plot_ds, plot_ds.GetName()  )
    helper.add( plot_pdf, plot_pdf.GetName()  )
    helper.add( (wmin,wmax), "eff_sigma_combcat"  )
    helper.add( (hmin, hmax, hm),  "FWHM_combcat"  )
    helper.add( plot_ds.sumEntries(), "sumEntries_combcat"  )
    
    mass.setRange("higgsrange",options.mH-25.,options.mH+15.);
    
    del cdf
    del pdf
    del helper.dsets
    
    ### label = TLatex(0.1812081,0.8618881,"#scale[0.8]{#splitline{CMS preliminary}{Simulation}}")
    label = TLatex(0.7,0.86,"#scale[0.65]{#splitline{CMS preliminary}{Simulation}}")
    label.SetNDC(1)
    
    ## 
    ## Make web page with plots
    ##
    if options.doWebPage:
        hth = HtmlHelper(options.outdir)
        hth.navbar().cell( HtmlTag("a") ).firstChild().txt("..").set("href","../?C=M;O=D")
        hth.navbar().cell( HtmlTag("a") ).firstChild().txt("home").set("href","./")
    
        tab = hth.body().add( HtmlTable() )
    
    ip = 0
    for c in ["_combcat"]+categories:
    ### for c in categories:
        if options.doWebPage and ip % 4 == 0:
            row = tab.row()
        ip = ip + 1

        dsname = "sig_mass_m%1.0f%s" % (options.mH,c)
        canv = TCanvas(dsname,dsname,600,600)
        helper.objs.append(canv)
        
        ### leg = TLegend(0.4345638,0.6835664,0.9362416,0.9178322)
        leg = TLegend(0.2,0.96,0.5,0.55)
        #apply_modifs( leg, [("SetLineColor",kWhite),("SetFillColor",kWhite),("SetFillStyle",0),("SetLineStyle",0)] )
        
        hplotcompint = mass.frame(RooFit.Bins(250),RooFit.Range("higgsrange"))
        helper.objs.append(hplotcompint)
        helper.objs.append(leg)
        
        plot_ds  =helper.histos["plot_%s" % dsname ]  
        plot_pdf =helper.histos["pdf_%s" % dsname ]  
        wmin,wmax = helper.histos["eff_sigma%s" % c ]
        hmin, hmax, hm = helper.histos["FWHM%s" % c ]
        print hmin, hmax, hm
        
        style = ( RooFit.LineColor(kBlue), RooFit.LineWidth(2), RooFit.FillStyle(0) )
        style_seff = (  RooFit.LineWidth(2), RooFit.FillStyle(1001), RooFit.VLines(), RooFit.LineColor(15), )
        style_ds = ( RooFit.MarkerStyle(kOpenSquare), )
                  
        plot_ds.plotOn(hplotcompint,RooFit.Invisible())
        
        plot_pdf.plotOn(hplotcompint,RooFit.NormRange("higgsrange"),RooFit.Range(wmin,wmax), RooFit.FillColor(19),
                        RooFit.DrawOption("F"), *style_seff)
        seffleg = hplotcompint.getObject(int(hplotcompint.numItems()-1))
        plot_pdf.plotOn(hplotcompint,RooFit.NormRange("higgsrange"),RooFit.Range(wmin,wmax), RooFit.LineColor(15), *style_seff)

        plot_pdf.plotOn(hplotcompint,RooFit.NormRange("higgsrange"),RooFit.Range("higgsrange"),*style)
        pdfleg = hplotcompint.getObject(int(hplotcompint.numItems()-1))

        plot_ds.plotOn(hplotcompint,*style_ds)
        pointsleg = hplotcompint.getObject(int(hplotcompint.numItems()-1))
        
        iob = int( hplotcompint.numItems() - 1 )
        leg.AddEntry( pointsleg,  "Simulation", "pe" )  
        leg.AddEntry( pdfleg,  "Parametric model", "l" )  
        leg.AddEntry( seffleg, "#sigma_{eff} = %1.2f GeV " % ( 0.5*(wmax-wmin) ), "fl" )  

        clabel = TLatex(0.74,0.65,"#scale[0.65]{#splitline{%s}{%s}}" % clables[c])
        clabel.SetNDC(1)
        helper.objs.append(clabel)

        hm = hplotcompint.GetMaximum()*0.5*0.9
        ### hm = pdfleg.GetMaximum()*0.5
        fwhmarrow = TArrow(hmin,hm,hmax,hm)
        fwhmarrow.SetArrowSize(0.03)
        helper.objs.append(fwhmarrow)
        fwhmlabel = TPaveText(0.20,0.58,0.56,0.48,"brNDC")
        fwhmlabel.SetFillStyle(0)
        fwhmlabel.SetLineColor(kWhite)
        reducedFWHM = (hmax-hmin)/2.3548200
        fwhmlabel.AddText("FWHM/2.35 = %1.2f GeV" % reducedFWHM)
        helper.objs.append(fwhmlabel)

        hplotcompint.SetTitle("");
        hplotcompint.GetXaxis().SetNoExponent(True);
        hplotcompint.GetXaxis().SetTitle("m_{#gamma#gamma} (GeV)");
        hplotcompint.GetXaxis().SetNdivisions(509);
        ## hplotcompint.GetYaxis().SetTitle("A.U.");
        ## hplotcompint.GetYaxis().SetRangeUser(0.,hplotcompint.GetMaximum()*1.4);
        hplotcompint.Draw();    
        leg.Draw("same")
        label.Draw("same")
        clabel.Draw("same")
        fwhmarrow.Draw("<>")
        fwhmlabel.Draw("same")
        
        plot_ds.sumEntries()

        
        if options.doWebPage:
            hpl = HtmlPlot(canv,False,"",True,True,True)
            hpl.caption("<i>%s</i>" % canv.GetTitle())
            row.cell( hpl )
        else:
            if os.path.isdir(options.outdir) is False:
                os.mkdir(options.outdir)
            for ext in "C","png","pdf":
                canv.SaveAs( os.path.join(options.outdir,"%s.%s" % (canv.GetName(), ext)) )
        
        if "comb" in c:
            ip = 0
            
    if options.doWebPage:
        print "Creating pages..."
        hth.dump()
        
    for f in helper.files:
        f.Close()
    gROOT.Reset()

    from pprint import pprint
    pprint(helper)
 
    print 'Summary statistics per event class'
    print 'Cat\tSignal\t\tData/GeV (in %3.1f+/-10)\tsigEff\tFWHM/2.35'%options.mH
    sigTotal=0.
    dataTotal=0.
    for c in categories:
      sigVal = helper.histos["sumEntries%s"%c]
      datVal = helper.histos["data_sumEntries%s"%c]
      sigTotal+=sigVal
      dataTotal+=datVal
    for c in categories:
      sigVal = helper.histos["sumEntries%s"%c]
      datVal = helper.histos["data_sumEntries%s"%c]
      effSig = 0.5*(helper.histos["eff_sigma%s"%c][1]-helper.histos["eff_sigma%s"%c][0])
      fwhm = (helper.histos["FWHM%s"%c][1]-helper.histos["FWHM%s"%c][0]) / 2.3548200
      print c, '\t%3.1f (%3.1f%%)\t%3.1f (%3.1f%%)\t\t\t%2.2f\t%2.2f'%(sigVal,100.*sigVal/sigTotal,datVal/(10.+10.),100.*datVal/dataTotal,effSig,fwhm)

    print "Done."

if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-l", "--lumi",
                    action="store", type="float", dest="lumi",
                    default=5089.,
                    help="", metavar=""
                    ),
        make_option("-i", "--infile",
                    action="store", type="string", dest="infile",
                    default="",
                    help="", metavar=""
                    ),
        make_option("-p", "--infilepdf",
                    action="store", type="string", dest="infilepdf",
                    default="",
                    help="", metavar=""
                    ),
        make_option("-o", "--outdir",
                    action="store", type="string", dest="outdir",
                    default="",
                    help="", metavar=""
                    ),
        make_option("-w", "--doWebPage",
                    action="store_true", dest="doWebPage",
                    default=False,
                    help="geneate web page to present plots (requires lip/Tools)",
                    ),
        make_option("-m", "--mass",
                    action="store", type="float", dest="mH",
                    default=130.,
                    help="", metavar=""
                    ),
        make_option("-f", "--fp",
                    action="store_true", dest="fp",
                    default=False,
                    help="", metavar=""
                    ),
        make_option("-r", "--refit",
                    action="store_true", dest="refit",
                    default=False,
                    help="", metavar=""
                    ),
        make_option("-a", "--refitall",
                    action="store_true", dest="refitall",
                    default=False,
                    help="", metavar=""
                    ),
        make_option("-b", "--binned",
                    action="store_true", dest="binned",
                    default=False,
                    help="", metavar=""
                    ),
        make_option("-M", "--mva",
                    action="store_true", dest="mva",
                    default=False,
                    help="", metavar=""
                    ),
        make_option("-n", "--ncat",
                    action="store", type="int", dest="ncat",
                    default=5,
                    help="", metavar=""
                    ),
        make_option("-c","--cats",
                    action="store",type='string',dest="cats",
                    default="",
                    help="", metavar="",
                    ),
                    
        ])
    
    (options, args) = parser.parse_args()

    main( options, args) 
