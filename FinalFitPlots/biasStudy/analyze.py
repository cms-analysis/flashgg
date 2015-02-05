#!/bin/env python

from optparse import OptionParser, make_option
import sys, os, glob
from math import fabs,floor

objs = []

# -----------------------------------------------------------------------------------------------------------
def getQuantilesGraphs(histo,probs,twosided=False):
    from math import sqrt
    graphs = [ ROOT.TGraphErrors(histo.GetNbinsX()) for p in probs ]
    if twosided:
        qtiles = []
        for p in probs:
            t = 0.5 - p*0.5
            qtiles.append( t )
            qtiles.append( 1-t )
    else:
        qtiles=probs
        
    nq = len(qtiles)
    graph = ROOT.TGraph(nq+2)
    for iq in range(nq):
        graph.SetPoint(iq,qtiles[iq],0.)
    for g in graphs:        
        g.GetXaxis().SetTitle(histo.GetXaxis().GetTitle())
        g.SetMarkerStyle(histo.GetMarkerStyle())
    graph.SetPoint(nq,0.25,0.)
    graph.SetPoint(nq+1,0.75,0.)
        
    for ix in range(1,histo.GetNbinsX()+1):
        proj = histo.ProjectionY("qtiles",ix,ix)
        
        ## proj.Print("all")
        ## graph.Print("all")
        proj.GetQuantiles(nq+2,graph.GetY(),graph.GetX())
        ntot = proj.Integral()
        if ntot == 0: continue
        h = 1.2*( graph.GetY()[nq+1] - graph.GetY()[nq] ) * pow(ntot,-0.2)
        
        if twosided:
            for ig in range(nq/2):                
                quant1 = graph.GetY()[ig]
                quant2 = graph.GetY()[ig+1]
                quant = (quant2 - quant1)*0.5                
                quant1mh = proj.FindBin( quant1 - h*0.5 )
                quant1ph = proj.FindBin( quant1 + h*0.5 )
                quant2mh = proj.FindBin( quant2 - h*0.5 )
                quant2ph = proj.FindBin( quant2 + h*0.5 )
                
                nint = proj.Integral( quant1mh, quant1ph ) + proj.Integral( quant2mh, quant2ph )
                if nint > 0 and ntot > 0:
                    fq = nint / (2.*h*ntot)
                    err = 1./(2.*sqrt(ntot)*fq)
                else:
                    err = 0.

                graphs[ig/2].SetPoint(ix-1,histo.GetXaxis().GetBinCenter(ix),quant)
                graphs[ig/2].SetPointError(ix-1,histo.GetXaxis().GetBinWidth(ix)*0.5,err)
                
        else:
            for ig in range(nq):
                quant = graph.GetY()[ig]
                quantmh = proj.FindBin( quant - h )
                quantph = proj.FindBin( quant + h )
                nint = proj.Integral( quantmh, quantph )
                
                if nint > 0 and ntot > 0:
                    fq = nint / (2.*h*ntot)
                    err = 1./(2.*sqrt(ntot)*fq)
                else:
                    err = 0.
                
                graphs[ig].SetPoint(ix-1,histo.GetXaxis().GetBinCenter(ix),quant)
                graphs[ig].SetPointError(ix-1,histo.GetXaxis().GetBinWidth(ix)*0.5,err)
                
    return graphs

##
def analyze(options,input):
    
    iname = os.path.basename(input)
    truth = ""
    test = ""
    cat = ""
    asimov = ""
    toks = iname.split(".")[-3].split("_")
    for itok in range(len(toks)):
        tok = toks[itok]
        if tok == "truth":
            truth = toks[itok+1]
        if tok == "test":
            test = toks[itok+1]
        if "cat" in tok:
            cat = tok
        if "asimov" in tok:
            asimov = "_asimov"
    ## oname = os.path.join(os.path.basename(options.input),"%s_%s%s_%s" % (cat, asimov, test, truth ) )
    oname = "%s_test_%s_truth_%s%s" % (cat, test, truth, asimov )
    
    fin = ROOT.TFile.Open(input)
    limit = fin.Get("limit")

    limit.Draw("limit/limitErr:mh>>h(9,107.5,152.5,100,-2,2)","quantileExpected==-1 && abs(limit) < 9.","goff")
    ## limit.Draw("limitErr:mh>>herr(9,107.5,152.5,100,-2,2)","quantileExpected==-1 && abs(limit) < 9.","goff")

    h = ROOT.gDirectory.Get("h")
    ## herr = ROOT.gDirectory.Get("h")
    prof = h.ProfileX()
    median = getQuantilesGraphs(h,[0.5])[0]
    mean = ROOT.TGraphErrors(h.ProfileX())
    median.SetTitle("%s truth: %s%s test: %s" % (cat,truth,asimov,test) )
    median.GetYaxis().SetTitle("#delta #mu / #sigma #mu")
    median.GetXaxis().SetTitle("m_{H} (GeV)")
    mean.SetLineColor(ROOT.kBlack)
    mean.SetMarkerColor(ROOT.kBlack)
    median.SetLineColor(ROOT.kRed)
    median.SetMarkerColor(ROOT.kRed)
    
    
    maxbias = 0.
    maxbiasmH = 0.
    narmaxbias = 0.
    narmaxbiasmH = 0.
    for i in range(median.GetN()):
        if maxbias<fabs(median.GetY()[i]):
            maxbias = fabs(median.GetY()[i])
            maxbiasmH = fabs(median.GetX()[i])
        if median.GetX()[i]>110 and median.GetX()[i]<145 and narmaxbias<fabs(median.GetY()[i]):
                narmaxbias = fabs(median.GetY()[i])
                narmaxbiasmH = fabs(median.GetX()[i])

    
    canv = ROOT.TCanvas(oname,oname)
    canv.cd()


    
    median.Draw("ap")
    ## prof.Draw("same")
    median.GetYaxis().SetRangeUser(-2.*options.threshold,2.*options.threshold)
    box = ROOT.TBox(median.GetXaxis().GetXmin(),-options.threshold,median.GetXaxis().GetXmax(),options.threshold)
    box.SetFillColor(ROOT.kBlue)
    box.SetFillStyle(3002)
    box.Draw("same")
    median.Draw("p")
    mean.Draw("p")
    
    objs.append(h)
    objs.append(prof)
    objs.append(median)
    objs.append(box)

    for fmt in "C","png","pdf":
        canv.SaveAs(os.path.join(options.outdir,"%s.%s" % (oname,fmt) ))
    objs.append(canv)
    objs.append(fin)

    return cat,test,"%s%s" % (truth,asimov), (maxbias,maxbiasmH,narmaxbias,narmaxbiasmH)


##
def main(options,args):

    summary = {}
    for input in args:
        try:
            cat,test,truth,bias = analyze(options,input)
            if not cat in summary:
                summary[cat] = { "truths" : set(), "tests" : {} }
            summary[cat]["truths"].add(truth)
            if not test in summary[cat]["tests"]:
                summary[cat]["tests"][test] = {}
            summary[cat]["tests"][test][truth] = bias
        except Exception, e:
            print "Failed to analyze ", input, e


    cats = summary.keys()
    cats.sort()
    print summary
    
    readme = open("%s/README.txt" % options.outdir,"w+")
    for cat in cats:
        
        truths = summary[cat]["truths"]
        tests = summary[cat]["tests"].keys()

        readme.write('-' * (23*(len(truths)+1)+1+7) )
        readme.write("\n")
        readme.write("%s\n" % cat)
        readme.write('-' * (23*(len(truths)+1)+1+7) )
        readme.write("\n")
        tests.sort()
        readme.write("|".ljust(23))
        for truth in truths:
            readme.write( "| %s " % truth.center(20) )
        readme.write("|\n")
        for test in tests:

            readme.write("| %s " % test.ljust(20) )
            maxbias = 0.
            for truth in truths:
                readme.write( "| ")
                if truth in summary[cat]["tests"][test]:
                    bias = summary[cat]["tests"][test][truth]
                    if fabs(bias[0]) > maxbias:
                        maxbias = fabs(bias[0])
                    readme.write( ("%1.2g (%1.3g)" % (bias[0], bias[1])).center(18) )
                    if fabs(bias[0]) > options.threshold:
                        readme.write(" *")
                    else:
                        readme.write("  ")
                else:
                    readme.write(" ".center(20))
                readme.write(" ")

            maxbias = floor(maxbias*100.)*0.01
            if maxbias<options.threshold:
                readme.write("| pass |\n")
            else:
                readme.write("| fail |\n")

            readme.write("| %s " % test.ljust(20) )
            maxbias = 0.
            for truth in truths:
                readme.write( "| ")
                if truth in summary[cat]["tests"][test]:
                    bias = summary[cat]["tests"][test][truth]
                    if fabs(bias[2]) > maxbias:
                        maxbias = fabs(bias[2])
                    readme.write( ("%1.2g (%1.3g)" % (bias[2], bias[3])).center(18) )
                    if fabs(bias[2]) > options.threshold:
                        readme.write(" *")
                    else:
                        readme.write("  ")
                else:
                    readme.write(" ".center(20))
                readme.write(" ")

            maxbias = floor(maxbias*100.)*0.01
            if maxbias<options.threshold:
                readme.write("| pass |\n")
            else:
                readme.write("| fail |\n")

        readme.write("\n\n")
    readme.close()
    

if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-i", "--input",
                    action="store", type="string", dest="input",
                    default="",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-o", "--outdir",
                    action="store", type="string", dest="outdir",
                    default="",
                    help="default : [%default]", metavar=""
                    ),
        make_option("-t", "--threshold",
                    action="store", type="float", dest="threshold",
                    default=0.14,
                    help="default : [%default]", metavar=""
                    ),
        make_option("-c","--cat",
                    action="append", type="int", dest="cats",
                    default=[],
                    help="default : [%default]", metavar=""
                    ),
        ])
    
    (options, args) = parser.parse_args()

    if options.input != "":
        args.extend( glob.glob(options.input) )

    if options.outdir == "":
        options.outdir = os.path.dirname(args[0])

    sys.argv.append("-b")
    import ROOT

    main( options, args )
