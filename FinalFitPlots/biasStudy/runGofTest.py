#!/bin/env python

from optparse import OptionParser, make_option
import sys, os, glob

objs = []

##
def mkexp(ord,cat,ws,var="CMS_hgg_mass"):
    norms = []
    pdfs = []
    for i in range(ord):
        norm = ws.factory("exp%d_coeff%d_cat%d[1.,0,10.]" % (ord,i,cat) )
        norms.append(norm)
        pdf = ws.factory("RooExponential::exp%(ord)d_%(term)d_cat%(cat)d( %(var)s, exp%(ord)d_slope%(term)d_cat%(cat)d[-0.05,-10,0] )" % {
            "cat" : cat,
            "ord" : ord,
            "term" : i,
            "var" : var
            })
        pdfs.append(pdf)
        objs.append( [pdf, norm] )
    return norms, pdfs    
    
##
def mkpow(ord,cat,ws,var="CMS_hgg_mass"):
    norms = []
    pdfs = []
    for i in range(ord):
        norm = ws.factory("pow%d_coeff%d_cat%d[1.,0,10.]" % (ord,i,cat) )
        norm.setVal(1./float(i+1))
        slope = ws.factory("pow%d_slope%d_cat%d[1,0,100]" % (ord,i,cat) )
        slope.setVal(-float(i+1))
        norms.append(norm)
        names = {
            "cat" : cat,
            "ord" : ord,
            "term" : i,
            "var" : var
            }
        pdf = ROOT.RooGenericPdf( "pow%(cat)d_%(ord)d_%(term)d" % names,  "pow%(cat)d_%(ord)d_%(term)d" % names,
                                  "pow(@0,-@1)",
                                  ROOT.RooArgList(ws.var(var),slope) )
        pdfs.append(pdf)
        objs.append( [pdf, norm, slope] )
    return norms, pdfs    

##
def mkpol(ord,cat,ws,var="CMS_hgg_mass"):
    norms = []
    pdfs = []
    for i in range(ord+1):
        norm = ws.factory("pol%d_coeff%d_cat%d[0.1,-1,1.]" % (ord,i,cat) )
        objs.append(norm)
        norm2 = ROOT.RooFormulaVar("pol%d_sqcoeff%d_cat%d" % (ord,i,cat),"pol%d_sqcoeff%d_cat%d" % (ord,i,cat),
                               "@0*@0", ROOT.RooArgList(norm) )
        ### norm = ws.factory("pol%d_coeff%d_cat%d[1,-10,10.]" % (ord,i,cat) )
        ### objs.append(norm)
        ### norm2  = norm
        norms.append(norm2)

    names = {
        "cat" : cat,
        "ord" : ord,
        "var" : var
        }
    pdf = ROOT.RooBernstein("pol%(cat)d_%(ord)d" % names,  "pol%(cat)d_%(ord)d" % names,
                            ws.var(var), ROOT.RooArgList(*norms) )
    pdfs.append(pdf)
    objs.append( [pdf, norms] )
    return [ROOT.RooFit.RooConst(1.)], pdfs 

##
def mkbern(ord,cat,ws,var="CMS_hgg_mass"):
    norms = []
    pdfs = []
    for i in range(ord+1):
        norm = ws.factory("bern%d_coeff%d_cat%d[1,-10,10.]" % (ord,i,cat) )
        norm.setVal(1./float(i+1))
        objs.append(norm)
        norms.append(norm)

    names = {
        "cat" : cat,
        "ord" : ord,
        "var" : var
        }
    pdf = ROOT.RooBernstein("bern%(cat)d_%(ord)d" % names,  "bern%(cat)d_%(ord)d" % names,
                            ws.var(var), ROOT.RooArgList(*norms) )
    pdfs.append(pdf)
    objs.append( [pdf, norms] )
    return [ROOT.RooFit.RooConst(1.)], pdfs 

##
def mkpol2(ord,cat,ws,var="CMS_hgg_mass"):
    return mkpol(ord,cat,ws,var)

##
def mkcheb(ord,cat,ws,var="CMS_hgg_mass"):
    norms = []
    pdfs = []
    for i in range(ord):
        norm = ws.factory("cheb%d_coeff%d_cat%d[1,-10,10.]" % (ord,i,cat) )
        norm.setVal(1./float(i+1))
        objs.append(norm)
        norms.append(norm)

    names = {
        "cat" : cat,
        "ord" : ord,
        "var" : var
        }
    pdf = ROOT.RooChebychev("cheb%(cat)d_%(ord)d" % names,  "cheb%(cat)d_%(ord)d" % names,
                            ws.var(var), ROOT.RooArgList(*norms) )
    pdfs.append(pdf)
    objs.append( [pdf, norms] )
    return [ROOT.RooFit.RooConst(1.)], pdfs 



### ##
### def mkpol(ord,cat,ws,var="CMS_hgg_mass"):
###     norms = []
###     pdfs = []
###     for i in range(ord):
###         norm = ws.factory("pol%d_coeff%d_cat%d[1.,-20,20.]" % (ord,i,cat) )
###         norms.append(norm)
###         names = {
###             "cat" : cat,
###             "ord" : ord,
###             "term" : i,
###             "var" : var
###             }
###         pdf = ROOT.RooGenericPdf( "pol%(cat)d_%(ord)d_%(term)d" % names,  "pol%(cat)d_%(ord)d_%(term)d" % names,
###                                   "pow(@0,%d)" % i,
###                                   ROOT.RooArgList(ws.var(var)) )
###         pdfs.append(pdf)
###         objs.append( [pdf, norm] )
###     return norms, pdfs    

##
def mklau(ord,cat,ws,var="CMS_hgg_mass"):
    norms = []
    pdfs = []
    for i in range(ord):
        norm = ws.factory("lau%d_coeff%d_cat%d[1.,0,10.]" % (ord,i,cat) )
        norms.append(norm)
        names = {
            "cat" : cat,
            "ord" : ord,
            "term" : i,
            "var" : var
            }
        slope = -4.
        for j in range(1,i+2):
            if j%2 == 1:
                slope += j-1
            else:
                slope -= j-1
        pdf = ROOT.RooGenericPdf( "lau%(cat)d_%(ord)d_%(term)d" % names,  "lau%(cat)d_%(ord)d_%(term)d" % names,
                                  "pow(@0,%1.0f)" % slope,
                                  ROOT.RooArgList(ws.var(var)) )
        pdfs.append(pdf)
        objs.append( [pdf, norm] )
    return norms, pdfs    
    


##
def mkPdf(name,ord,cat,ws):
    norms, pdfs = globals()["mk%s" % name](ord,cat,ws)

    try:
        norms[0].setVal(1.)
        norms[0].setConstant(True)
    except:
        pass
    
    pdf = ROOT.RooAddPdf("%s%d_cat%d_pdf" % (name,ord,cat), "%s%d_cat%d_pdf" % (name,ord,cat), ROOT.RooArgList(*pdfs), ROOT.RooArgList(*norms) )
    norm = ws.factory("%s%d_cat%d_extpdf_norm[0,1.e+6]" % (name,ord,cat))

    extpdf = ROOT.RooExtendPdf("%s%d_cat%d_extpdf" % (name,ord,cat), "%s%d_cat%d_extpdf" % (name,ord,cat), pdf, norm)
    getattr(ws,"import")(pdf, ROOT.RooFit.RecycleConflictNodes())
    getattr(ws,"import")(extpdf, ROOT.RooFit.RecycleConflictNodes())
    
    objs.append( [pdf, extpdf] )

    ## extpdf = pdf
    ## getattr(ws,"import")(pdf, ROOT.RooFit.RecycleConflictNodes())
    
    objs.append( [pdf, extpdf] )

    return extpdf
    
##
def runGof(ws, cat, families):

    ret = []
    ### data = ws.data("roohist_data_mass_cat%d" % cat).Clone()
    data = ws.data("data_mass_cat%d" % cat).Clone()
    
    for name,ord,maxord in families:
        func0 = mkPdf(name,ord,cat,ws)
        ## func0.Print("V")
        ### func0.fitTo(data)
        ### print func0, data
        ### data.Print("V")
        nll0 = func0.createNLL( data, ROOT.RooFit.Extended()  )
        min0 = ROOT.RooMinimizer(nll0)
        min0.setMinimizerType("Minuit2")
        min0.setStrategy(2)
        min0.setPrintLevel(-1)
        min0.seek()
        min0.migrad()
        min0.improve()
        last = min0.migrad()
        minnll0 = nll0.getVal()

        ndf0 =  nll0.getParameters(ROOT.RooArgSet()).getSize()

        print "--------------------------------------------------------------------------------------------"
        print "cat%d" % cat, "lowest order ", name, ord, " ndf ", ndf0, "-logL ", minnll0, " status ", last
        
        chi2 = 0
        pval = 0.01
        jord = ord
        name0 = func0.GetName()
        while (pval < 0.05 and jord < 9):
            jord+=1
            
            funcj = mkPdf(name,jord,cat,ws)
            ## funcj.getParameters(ROOT.RooArgSet()).Print("V")
            nllj = funcj.createNLL( data, ROOT.RooFit.Extended() )
            minj = ROOT.RooMinimizer(nllj)
            minj.setMinimizerType("Minuit2")
            minj.setStrategy(2)
            minj.setPrintLevel(-1)
            minj.seek()
            minj.migrad()
            minj.improve()
            last = minj.migrad()
            if last != 0:
                minj.setMinimizerType("Minuit")
                minj.setStrategy(2)
                minj.seek()
                minj.migrad()
                minj.improve()
                last = minj.migrad()
                
            minnllj = nllj.getVal()

            chi2 = 2.*(minnll0 - minnllj)
            if abs(chi2)<1.e-2: chi2 = 0.
            ndf = nllj.getParameters(ROOT.RooArgSet()).getSize() - ndf0
            pval = ROOT.TMath.Prob(chi2,ndf)
            
            ndf0 = nllj.getParameters(ROOT.RooArgSet()).getSize()
            print name, jord, minnll0, minnllj, chi2, ndf, pval, ndf0, last
            
            minnll0 = minnllj
            if pval<0.05:
                name0 = funcj.GetName()
                min0 = minj

        for kord in range(jord,maxord+1):
            funcj = mkPdf(name,kord,cat,ws)
            ## funcj.getParameters(ROOT.RooArgSet()).Print("V")
            nllj = funcj.createNLL( data, ROOT.RooFit.Extended() )
            minj = ROOT.RooMinimizer(nllj)
            minj.setMinimizerType("Minuit2")
            minj.setStrategy(2)
            minj.setPrintLevel(-1)
            minj.seek()
            minj.migrad()
            minj.improve()
            
        
        func = mkPdf(name,jord-1,cat,ws)
        ## func.getParameters(ROOT.RooArgSet()).Print("V")
        nll = func.createNLL( data, ROOT.RooFit.Extended() )
        min = ROOT.RooMinimizer(nll)
        min.setMinimizerType("Minuit2")
        min.setStrategy(2)
        min.setPrintLevel(-1)
        min.seek()
        min.migrad()
        min.improve()

        print "--------------------------------------------------------------------------------------------"
        print "cat%d" % cat, name, " stopping at order ", jord-1, " chi2 ",chi2, " ndf ", ndf, " pval ", pval
        print "--------------------------------------------------------------------------------------------"
        print "best fit: "
        func.getParameters(ROOT.RooArgSet()).Print("V")
        print "--------------------------------------------------------------------------------------------"
        print
        print
        
        ret.append( (name,name0,func) )

    return ret

##
def main(options,args):

    
    fin = ROOT.TFile.Open(options.input)
    ws = fin.Get("cms_hgg_workspace")

    tin = open("%s/%s" % (options.mydir,options.datacard_template),"r")
    template = tin.read()
    tin.close()

    tout =  open("%s/%s" % (options.workdir,options.datacard_template),"w+")
    tout.write(template)
    tout.close()
    
    summary = {}
    ## families = [ ("exp",1,3), ("pow",1,3) , ("pol",1,6), ("pol2",2,6), ("lau",1,3) ]
    ## families = [ ("exp",1,3), ("pow",1,3) , ("pol",1,6), ("lau",1,3) ]
    families = [ ("exp",1,3), ("pow",1,3) , ("pol",1,6), ("cheb",1,6), ("bern",1,6),  ("lau",1,3) ]
    for cat in options.cats:
        gof = runGof( ws, cat, families)
        summary[cat] = gof

    cats = summary.keys()
    cats.sort()
    var = ws.var("CMS_hgg_mass")
    for cat in cats:
        funcs = []
        colors = [ROOT.kBlue,ROOT.kRed,ROOT.kGreen,ROOT.kMagenta,ROOT.kBlack,ROOT.kOrange]
        for res in summary[cat]:
            name, funcn, func = res
            ## print "shapes bkg      bias_study_cat%d   %s    %s:%s" % ( cat, options.output, ws.GetName(), funcn )
            card = open("%s/datacard_truth_cat%s_%s.txt" % ( options.workdir, cat, name ), "w+" )
            card.write( template % { "cat" : "cat%d" % cat,
                                     "bkg_shape" : "%s    %s:%s" % ( os.path.basename(options.output), ws.GetName(), funcn )
                                     }
                        )
            card.close()
            funcs.append(func)
            
        data = ws.data("roohist_data_mass_cat%d" % cat)
        frame = var.frame()
        data.plotOn(frame,ROOT.RooFit.Binning(80),ROOT.RooFit.Invisible())
        data.plotOn(frame,ROOT.RooFit.Binning(30,150,180))
        data.plotOn(frame,ROOT.RooFit.Binning(10,100,110))
        leg = ROOT.TLegend(0.6,0.6,0.89,0.89)
        leg.SetFillColor(0)
        leg.SetLineColor(0)
        for func in funcs:
            color = colors.pop(0)
            func.plotOn(frame,ROOT.RooFit.LineColor(color))
            obj = frame.getCurve(func.GetName())
            entry = leg.AddEntry(obj,func.GetName().split("_")[0],"l")
            entry.SetLineColor(color)
            entry.SetLineWidth(2)
        canv = ROOT.TCanvas("fits_cat%d" % cat,"fits_cat%d" % cat)
        canv.cd()
        frame.Draw()
        leg.Draw("same")
        for fmt in "png","pdf":
            canv.SaveAs("%s/%s.%s" % ( options.workdir, canv.GetName(), fmt ) )
        objs.append(frame)
        objs.append(var)
        objs.append(data)
        
    ws.writeToFile( options.output )

if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--input",
                    action="store", type="string", dest="input",
                    default="CMS-HGG.root",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-w", "--workdir",
                    action="store", type="string", dest="workdir",
                    default="",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-o", "--output",
                    action="store", type="string", dest="output",
                    default="",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-c","--cat",
                    action="append", type="int", dest="cats",
                    default=[],
                    help="default : [%default]", metavar=""
                    ),
        make_option("-d","--datacard-template",
                    action="store", type="string", dest="datacard_template",
                    default="datacard_bias_study_template.txt",
                    help="default : [%default]", metavar=""
                    ),
        ])
    
    (options, args) = parser.parse_args()

    if options.output == "":
        options.output = options.input.replace(".root","_gof.root")

    if options.workdir == "":
        options.workdir = os.path.dirname( options.input )

    options.mydir = os.path.dirname( sys.argv[0] )
    if options.mydir == "":
        options.mydir = os.getcwd()
    
    sys.argv.append("-b")
    import ROOT

    ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.FATAL)
    ROOT.RooMsgService.instance().setSilentMode(True)

    main( options, args )
