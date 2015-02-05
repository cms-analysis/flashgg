import ROOT
import sys
import numpy

files = [ ROOT.TFile.Open(f) for f in sys.argv[1:] ]


def drawModel(mh,ncat=6,color=ROOT.kBlue,opt="hist",procs = [ "ggh", "vbf", "wzh", "tth" ],syst=None):

    ROOT.gROOT.LoadMacro("ResultScripts/GraphToTF1.C+")

    for p in procs:
        c = ROOT.TCanvas("canv_%s_%1.5g" % (p,mh),"canv_%s_%1.5g" % (p,mh), 900, 400*ncat/3)
        c.Divide(3, ncat / 3 )
        globals()[c.GetName().replace(".","_")] = c
        for cat in range(ncat):
            c.cd(cat+1)
            name = "th1f_sig_%s_mass_m%1.5g_cat%d" % ( p, mh, cat )
            h = ROOT.gDirectory.Get(name)
            if syst:
                hup   = ROOT.gDirectory.Get("%s_%sUp01_sigma" % ( name, syst ) )
                hdown = ROOT.gDirectory.Get("%s_%sDown01_sigma" % ( name, syst ) )
                hup.Print()
                hdown.Print()
                
            h.SetLineColor(color)
            h.SetMarkerColor(color)

            h.Draw(opt)
            if syst:
                hup.SetLineColor(color+10)
                hup.SetMarkerColor(color+10)
                hdown.SetLineColor(color+10)
                hdown.SetMarkerColor(color+10)
                hup.Draw("%s same"%opt)
                hdown.Draw("%s same"%opt)
            
            print h.GetName(), h.Integral(), getFWHM(h), getFWHM(hup), getFWHM(hdown), h.GetRMS(), hup.GetRMS(), hdown.GetRMS()
            

def getPoint(h):

    from math import sqrt

    qi  = ROOT.GraphToTF1("%s_func" % h.GetName(), ROOT.TGraph(h) )
    fit = ROOT.TF1( "%s_func" % h.GetName(), qi, 110., 180.,1, "GraphToTF1")
    fit.SetParameter(0,0)
    hm  = fit.GetMaximum()*0.5
    hmx = fit.GetMaximumX()
    high = fit.GetX(hm, hmx, 180)
    low  = fit.GetX(hm, 110, hmx)

    return h.Integral(), h.Integral()/sqrt(h.GetEntries()), high-low, h.GetMean()
    

def getFWHM(h):
    integral, integralE, fwhm, mean = getPoint(h)
    return fwhm

def commit(obj,color):
    obj.SetLineColor(color)
    obj.SetMarkerColor(color)
    globals()[obj.GetName().replace(".","_")] = obj

def drawYields(masses,ncat=6,color=ROOT.kBlue,opt="PL",procs = [ "ggh", "vbf", "wzh", "tth" ],syst=None,colorUp=ROOT.kMagenta,colorDown=ROOT.kGreen):
    
    ROOT.gROOT.LoadMacro("ResultScripts/GraphToTF1.C+")

    for p in procs:
        c = ROOT.TCanvas("canv_%s" % (p),"canv_%s" % (p), 900, 400*ncat/3)
        c.Divide(3, ncat / 3 )
        globals()[c.GetName().replace(".","_")] = c

        d = ROOT.TCanvas("canv_%sFWHM" % (p),"canv_%sFWHM" % (p), 900, 400*ncat/3)
        d.Divide(3, ncat / 3 )
        globals()[d.GetName().replace(".","_")] = d

        e = ROOT.TCanvas("canv_%sMean" % (p),"canv_%sMean" % (p), 900, 400*ncat/3)
        e.Divide(3, ncat / 3 )
        globals()[d.GetName().replace(".","_")] = e

        for cat in range(ncat):
            gcat = ROOT.TGraphErrors()
            gcat.SetName("%s_cat%d" % ( p, cat) )
            gcatFWHM = ROOT.TGraphErrors()
            gcatFWHM.SetName("%s_cat%d_FWMH" % ( p, cat) )
            gcatMean = ROOT.TGraphErrors()
            gcatMean.SetName("%s_cat%d_Mean" % ( p, cat) )
            commit(gcat,color)
            commit(gcatFWHM,color)
            commit(gcatMean,color)
            
            if syst!= None:
                gcatUp = ROOT.TGraph()
                gcatUp.SetName("%s_cat%d_%sUp" % ( p, cat, syst ) )
                commit(gcatUp,colorUp)

                gcatDown = ROOT.TGraph()
                gcatDown.SetName("%s_cat%d_%sDown" % ( p, cat, syst ) )
                commit(gcatDown,colorDown)

                gcatFWHMUp = ROOT.TGraph()
                gcatFWHMUp.SetName("%s_cat%d_%sUp" % ( p, cat, syst ) )
                commit(gcatFWHMUp,colorUp)

                gcatFWHMDown = ROOT.TGraph()
                gcatFWHMDown.SetName("%s_cat%d_%sDown" % ( p, cat, syst ) )
                commit(gcatFWHMDown,colorDown)

                gcatMeanUp = ROOT.TGraph()
                gcatMeanUp.SetName("%s_cat%d_%sUp" % ( p, cat, syst ) )
                commit(gcatMeanUp,colorUp)

                gcatMeanDown = ROOT.TGraph()
                gcatMeanDown.SetName("%s_cat%d_%sDown" % ( p, cat, syst ) )
                commit(gcatMeanDown,colorDown)


            for mh in masses:
                name = "th1f_sig_%s_mass_m%1.5g_cat%d" % ( p, mh, cat )
                h = ROOT.gDirectory.Get(name)
                try:
                    integral, integralE, fwhm, mean = getPoint(h)
                    ip = gcat.GetN()
                    gcat.SetPoint(ip,mh,integral)
                    if( abs(round(mh / 5.)*5. - mh) < 1.e-5 ):
                        gcat.SetPointError(ip,0.,integralE)
                    
                    gcatFWHM.SetPoint(ip,mh,fwhm)
                    gcatMean.SetPoint(ip,mh,mean)

                    if syst!= None:
                        hup   = ROOT.gDirectory.Get("%s_%sUp01_sigma" % ( name, syst ) )
                        hdown = ROOT.gDirectory.Get("%s_%sDown01_sigma" % ( name, syst ) )
                        integralUp, integralEUp, fwhmUp, meanUp = getPoint(hup)
                        integralDown, integralEDown, fwhmDown, meanDown = getPoint(hdown)

                        gcatUp.SetPoint(ip,mh,integralUp)
                        gcatFWHMUp.SetPoint(ip,mh,fwhmUp)
                        gcatMeanUp.SetPoint(ip,mh,meanUp)

                        gcatDown.SetPoint(ip,mh,integralDown)
                        gcatFWHMDown.SetPoint(ip,mh,fwhmDown)
                        gcatMeanDown.SetPoint(ip,mh,meanDown)

                except:
                    print "%s not found" % name
                    
                    
                    
            
            c.cd(cat+1)
            gcat.SetMarkerStyle(ROOT.kFullCircle)
            gcat.Draw("A%s" % opt)
            if syst != None:
                gcatUp.Draw(opt)
                gcatDown.Draw(opt)
                
            d.cd(cat+1)
            gcatFWHM.SetMarkerStyle(ROOT.kFullCircle)
            gcatFWHM.Draw("A%s" % opt)
            if syst != None:
                gcatFWHMUp.Draw(opt)
                gcatFWHMDown.Draw(opt)

            e.cd(cat+1)
            gcatMean.SetMarkerStyle(ROOT.kFullCircle)
            gcatMean.Draw("A%s" % opt)
            if syst != None:
                gcatMeanUp.Draw(opt)
                gcatMeanDown.Draw(opt)
            
