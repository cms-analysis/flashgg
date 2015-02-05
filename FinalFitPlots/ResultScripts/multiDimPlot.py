#!/bin/env python

from optparse import OptionParser, make_option
import sys, os

def read1D(files, x, igr, title, xtitle):
    ch = ROOT.TChain("limit")
    for f in files:
        print f
        ch.AddFile(f)
    ch.Draw("2*deltaNLL:%s" % (x), "", "" )
    gr = ROOT.gROOT.FindObject("Graph").Clone("gr_%s_%d" % (x,igr) )
    gr.SetTitle(title)
    gr.GetXaxis().SetTitle(xtitle)
    gr.GetYaxis().SetTitle("-2 #Delta LL")

    gr.Sort()

    last = None
    for i in range(gr.GetN(),0,-1):
        if gr.GetX()[i-1] == last:
            gr.RemovePoint(i-1)
        last = gr.GetX()[i-1]
    
    return gr

def main(options, args):

    from rootglobestyle import setTDRStyle
    setTDRStyle()
    
    titles = {
        "RV" : "#mu_{VBF+VH}^{#gamma #gamma}", ## "( #sigma_{VH} + #sigma_{qqH} ) * BR_{#gamma #gamma} / SM",
        "MH" : "m_{H} (GeV)"## "( #sigma_{VH} + #sigma_{qqH} ) * BR_{#gamma #gamma} / SM",
        }
    styles = [ (ROOT.kBlack,ROOT.kFullCircle),
               (ROOT.kBlue,ROOT.kOpenTriangleDown),
               (ROOT.kRed+1,ROOT.kFullDiamond),
               (ROOT.kGreen+2,ROOT.kOpenTriangleDown),
               (ROOT.kOrange+2,ROOT.kOpenCircle),
               (ROOT.kMagenta,ROOT.kOpenCircle)
               ]
    objs = []
    graphs = []
    
    for ifile in range(len(options.files)):
        file = options.files[ifile]
        label = "file%d" % ifile
        if len(options.labels)>0:
            label = options.labels.pop(0)

        color,marker = styles.pop(0)
        
        
        if "*" in file:
            files = glob(file)
        else:
            files = [file]

        if len(options.variables) == 1:
            x = options.variables[0]
            gr = read1D( files, x, ifile, label, titles[x] )
            gr.SetLineColor(color)
            gr.SetMarkerColor(color)
            gr.SetMarkerStyle(marker)

            sp = ROOT.GraphToTF1( "mygraph%d" % ifile, gr )
            ## func = ROOT.TF1("myfunc%d" % ifile,sp,0.,10.,1,"GraphToTF1")
            func = ROOT.TF1("myfunc%d" % ifile,sp,gr.GetX()[0],gr.GetX()[gr.GetN()-1],1,"GraphToTF1")
            func.SetParameter(0,0.)
            func.SetLineColor(color)
            gr.GetListOfFunctions().AddLast(func)
            
            
            graphs.append(gr)

            objs.append(sp)
            objs.append(func)
            objs.append(gr)
            
        elif len(options.variables) == 2:
            pass

    rng = 6
    if len(options.variables) == 1:
        axmin = 999.
        axmax = -999.
        for gr in graphs:
            func = gr.GetListOfFunctions().At(0)
            xmin = func.GetMinimumX()
            
            eminus = xmin - func.GetX(1.,func.GetXmin(),xmin)
            eplus  = func.GetX(1.,xmin,func.GetXmax()) - xmin

            eminus2 = xmin - func.GetX(4.,func.GetXmin(),xmin)
            eplus2  = func.GetX(4.,xmin,func.GetXmax()) - xmin

            eminusR = xmin - func.GetX(rng,func.GetXmin(),xmin)
            eplusR  = func.GetX(rng,xmin,func.GetXmax()) - xmin

            axmin = min(axmin,xmin - eminusR)
            axmax = max(axmax,xmin + eplusR)

            print "%s : %1.4f +%1.3g -%1.3g" % ( gr.GetName(), xmin, eplus , eminus )

        lines = [ ROOT.TLine(axmin, 0, axmax, 0),
                  ROOT.TLine(axmin, 1, axmax, 1), ROOT.TLine(xmin-eminus,  0, xmin-eminus,  1), ROOT.TLine(xmin+eplus,  0, xmin+eplus,  1)
                  ]
        if rng > 4:
            lines.extend( [ROOT.TLine(axmin, 4, axmax, 4), ROOT.TLine(xmin-eminus2, 0, xmin-eminus2, 4), ROOT.TLine(xmin+eplus2, 0, xmin+eplus2, 4)] )
        x = options.variables[0]
        fname = x 
        if options.outname:
            fname = options.outname
        canv = ROOT.TCanvas(fname,fname)
        ### canv.SetGridx()
        ### canv.SetGridy()
        leg  = ROOT.TLegend(0.35,0.7,0.6,0.9)
        leg.SetLineColor(ROOT.kWhite)
        leg.SetFillStyle(0)
        objs.append(canv)
        objs.append(leg)

        for g in graphs:
            g.SetLineWidth(3)
        graphs[0].Draw("AL")
        graphs[0].GetXaxis().SetRangeUser(axmin,axmax)
        ### graphs[0].GetXaxis().SetRangeUser(122,128)
        graphs[0].GetYaxis().SetRangeUser(0.,rng)
        leg.AddEntry(graphs[0],"","L")
        for gr in graphs[1:]:
            gr.Draw("L")
            leg.AddEntry(gr,"","L")

        if options.legend:
            leg.Draw("same")

        for l in lines:
            l.SetLineWidth(2)
            l.SetLineColor(ROOT.kRed)
            l.Draw("SAME")

        mytext = ROOT.TLatex()
        mytext.SetTextSize(0.04)
        mytext.SetNDC()

        mytext.DrawLatex(0.3, 0.9, "H #rightarrow #gamma #gamma")
        if len(options.lumi)==2:
            mytext.DrawLatex(0.3,0.82,"#splitline{CMS preliminary}{#splitline{#sqrt{s} = %i TeV L = %.1f fb^{-1}}{#sqrt{s} = %i TeV L = %.1f fb^{-1}}}" %(int(options.energy[0]),float(options.lumi[0]),int(options.energy[1]),float(options.lumi[1])))
        
        for fmt in "C","png","pdf":
            canv.SaveAs( "%s.%s" % ( canv.GetName(), fmt ) ) 
        
    return objs
    
if __name__ == "__main__":
    parser = OptionParser(option_list=[
        make_option("-f", "--file",
                    action="append", dest="files",
                    default=[],
                    ),
        make_option("-l", "--label",
                    action="append", dest="labels",
                    default=[],
                    ),
        make_option("-v", "--variables",
                    action="append", dest="variables",
                    default=[],
                    ),
        make_option("-L", "--lumi",
                    action="append", dest="lumi",
                    default=[],
                    ),
        make_option("-E", "--energy",
                    action="append", dest="energy",
                    default=[],
                    ),
        make_option("-g", "--legend",
                    action="store_true", dest="legend",
                    default=False,
                    ),
        make_option("-o", "--outname",
                    action="store", dest="outname", type="string",
                    default="",
                    ),
        make_option("-G", "--no-legend",
                    action="store_true", dest="noLegend",
                    default=False,
                    ),
        ])
    
    (options, args) = parser.parse_args()
    if len(options.files) > 1:
        if options.noLegend:
            options.legend=False
        else:
            options.legend=True
    print options
    
    ## sys.argv.append("-b")
    import ROOT

    ROOT.gROOT.LoadMacro("%s/GraphToTF1.C+" % os.path.dirname(sys.argv[0]) )
    
    objs = main( options, args )

