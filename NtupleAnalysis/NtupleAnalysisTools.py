###########################################################################################################################
# Abraham Tishelman-Charny
# 13 May 2020
#
# The purpose of this module is to provide variables and definitions for NtupleAnalysis.py 
#
###########################################################################################################################

from ROOT import TCanvas, gROOT, gPad, TH1F, TFile, TChain, TPaveStats, gStyle, THStack, kBlue, kCyan, kRed, kGreen, TLegend, TRatioPlot, kBlack, TLine, kPink, TLatex, kOrange, gErrorIgnoreLevel, kWarning
import os 
import tdrstyle
from MCTools import * 

def GetColors():
    colorsTemp_ = [kRed, kOrange, kCyan, kBlue, kBlack, kGreen]
    colors_ = []
    for ic in colorsTemp_:
        colors_.append(ic)
        colors_.append(ic+2)
        colors_.append(ic-2)
        colors_.append(ic+4)
        colors_.append(ic-4)
    return colors_

def SetGraphStyle(gr_,num_,clr_,msty_):
    gr_.SetLineStyle(num_)
    gr_.SetMarkerStyle(msty_)
    gr_.SetLineColor(clr_)
    gr_.SetMarkerColor(clr_)
    gr_.SetMarkerSize(1.5)
    return 0 

def GetEvents(mass_,campaign_):
    evDict = {
        "X250": 98443,
        # "X260": 99932, # no taus or bs 
        # "X260": 99942, # with taus and bs HHWWgg_v2-5 
        "X260": 99942, # All tau decays added. HHWWgg_v2-6
        "X270": 99952,
        "X280": 99936,
        "X300": 99931,
        "X320": 99941,
        "X350": 99945,
        "X400": 99937,
        "X500": 99947,
        "X550": 99940,
        # "X600": 99961,
        # "X600": 99938, # HHWWgg_v2-5 
        "X600": 99933, # HHWWgg_v2-6 
        "X650": 99450,
        "X700": 99449,
        "X750": 99953,
        "X800": 99443,
        "X850": 99942,
        "X900": 99945,
        # "X1000": 99450,
        # "X1000": 99931, # HHWWgg_v2-5
        "X1000": 99936, # HHWWgg_v2-6
        "X1250": 97940,
        "SM": 99931,
        # "SM": 99431, # 199/200 files 
        "node2": 99944,
        "node9": 99938,
        "MX1000_MY800": 100000,
        "MX1600_MY400": 100000,
        "MX2000_MY1800": 100000,
        "MX300_MY170": 100000
    }

    if(campaign_=="HHWWgg_v2-3"):
        evDict["X260"] = 99932
        evDict["X600"] = 99961
        evDict["X1000"] = 99450

    return evDict[mass_]

def PlotEffCurves(sig_eff_vals_,ttH_eff_vals_,ol_):
    n = 100
    x, y = array( 'd' ), array( 'd' )
    for ientry,entry in enumerate(sig_eff_vals_):
        x_val, y_val = float(ttH_eff_vals_[ientry]), float(sig_eff_vals_[ientry])
        x.append(x_val)
        y.append(y_val)
        # effVseff_h.SetBinContent(ientry+1,)
        # print'x_val = ',x_val
        # print'y_val = ',y_val
        # effVseff_h.Fill(x_val,y_val)
        # effVseff_h.Fill(x_val,y_val)
        # effVseff_h.SetBinContent()
    # effVseff_h.SetMarkerStyle(8)
    effVseff_h = TGraph(n,x,y)
    effVseff_h.SetLineColor( 2 )
    effVseff_h.SetLineWidth( 4 )
    effVseff_h.SetMarkerColor( 4 )
    effVseff_h.SetMarkerStyle( 21 )
    effVseff_h.SetTitle( 'b veto efficiency' )
    effVseff_h.GetXaxis().SetTitle( 'ttH efficiency' )
    effVseff_h.GetYaxis().SetTitle( 'signal efficiency' )
    outputName = ol_ + 'sigEffvsttHeff.png'
    Draw_Histogram(effVseff_h,'ACP',outputName)
    # TLine *line = new TLine(-3,ymax,3,ymax);
    # effVseff_h.GetXaxis().SetTitle('ttH efficiency')
    # effVseff_h.GetYaxis().SetTitle('signal efficiency')
    # outputName = ol_ + 'sigEffvsttHeff.png'
    # Draw_Histogram(effVseff_h,'COLZ1',outputName)

def PlotEff(h_,ofn_):
    # plot efficiency vs cut
    cuts = [] 
    efficiencies = []
    nBins = h_.GetNbinsX()
    total_entries = h_.Integral()
    # for ix,x_ in enumerate(h_):
    eff_bins = 100 
    eff_min = 0.
    for eb in range(1,eff_bins+1): 
        # if ix == 0: continue # skip underflow bin 
        # elif ix == nBins + 1: break # skip overflow bin  
        thisCut = eff_min + 0.01*float(eb-1) # 0.01 because this is the bin interval
        cuts.append(thisCut)
        entries_in_range = h_.Integral(1,eb+1)
        efficiency = float(entries_in_range) / float(total_entries)
        efficiencies.append(efficiency)
        print'efficiency = ',efficiency
    eff_h = TH1F('eff_h','eff_h',100,0.,1)
    eff_h_2 = h_.Clone()
    eff_h_2.SetDirectory(0)

    for i,val in enumerate(efficiencies):
        x_val, y_val = float(cuts[i]), float(efficiencies[i])
        eff_h.SetBinContent(i+1,y_val) # USE THIS INSTEAD OF FILL 
    eff_h.SetMarkerStyle(8)
    Draw_Histogram(eff_h,'p',ofn_)
    return efficiencies
    
def CalcEff(h_,cut_):
    # return percentage of events that pass cut 
    pctPass_ = 0
    numPass = 0 
    N = h_.GetEntries()
    nBins = h_.GetNbinsX()

    for ix,y in enumerate(h_):
        if ix == 0: continue # skip underflow bin 
        elif ix == nBins + 1: break # skip overflow bin 
        x = h_.GetBinLowEdge(ix)
        if x < cut_: numPass += y # if the x value of the bin is less than the cut, all jets pass 
        elif x >= cut_: break # if the x value of the bin is greater than or equal to the cut, no more jets will pass 

    pctPass_ = float(numPass) / float(N)

    return pctPass_ 

def Draw_Histogram(h_,opt_,fn_,log_):
    c_tmp = TCanvas('c_tmp','c_tmp',1300,800)
    h_.Draw("apl")
    if(log_): gPad.SetLogy()
    h_.Draw(opt_)

    # c_tmp.BuildLegend(0.75,0.62,0.95,0.84)
    # c_tmp.BuildLegend(0.75,0.42,0.95,0.65)
    c_tmp.BuildLegend(0.75,0.57,0.95,0.8)

    # c_tmp.BuildLegend(0.83, 0.83, 0.99, 0.99)
    # c_tmp.BuildLegend(0.5, 0.3, 0.8, 0.8)
    # c_tmp.BuildLegend(0.6, 0.1, 0.9, 0.6)
    c_tmp.SaveAs(fn_)
    return 

def DrawNonResHistogram(h_,opt_,fn_,log_,N_,plotLabels_):
    c_tmp = TCanvas('c_tmp','c_tmp',1300,800)
    if(log_): gPad.SetLogy()
    # set bin labels 
    # frame = c_tmp.DrawFrame(1.4,0.001, 4.1, 10)
    # frame.SetDirectory(0)
    # frame.GetXaxis().SetLimits(-0.5,N_-0.5)
    # frame.GetXaxis().CenterLabels(True)
    # for i in range(N_):
        # frame.GetXaxis().SetBinLabel(int(1000.*(2*i+1)/(2*N_)),plotLabels_[i])
    # if(N_ > 3): frame.GetXaxis().SetLabelSize(0.07)
    # else: frame.GetXaxis().SetLabelSize(0.12)
    # frame.LabelsOption("h","X")    
    # frame.Draw('axis')
    # print"frame:",frame
    h_.Draw(opt_)
    gPad.Update()
    mg_hist = h_.GetHistogram()
    # mg_hist.Rebin(N_)
    # print"nbins:",mg_hist.GetNbinsX()
    N_xbins = mg_hist.GetNbinsX()
    # N_rebin = int(N_xbins) / 4
    N_rebin = int(N_xbins) / 100
    # N_rebin = 25
    mg_hist.Rebin(N_rebin)
    # mg_hist.SetBarOffset(100)
    # want 4 total
    # rebin means divide total by this. 4 = NXbins / Rebin -> Rebin = NXbins / 4 
    h_.Draw(opt_)
    # h_.GetXaxis().CenterLabels(True)
    # h_.SetBarOffset(-0.5)
    
    if(N_ > 3): h_.GetXaxis().SetLabelSize(0.05)
    else: h_.GetXaxis().SetLabelSize(0.12)

    for i in range(N_):
        h_.GetXaxis().LabelsOption("h")
        # h_.GetXaxis().LabelsOption("d")
        # bin = h_.GetXaxis().FindBin(i+1)
        bin = h_.GetXaxis().FindBin(i)
        #39 62 85 
        # print"bin:",bin
        # h_.GetXaxis().SetBinLabel(bin,plotLabels_[i])
        h_.GetXaxis().SetBinLabel(bin,plotLabels_[i])
    # print"frame:",frame 
    # frame.Draw('axissame')
    # frame.Draw("sameaxis")
        # c_tmp.BuildLegend()
        # c_tmp.BuildLegend(0.65,0.52,0.95,0.82)

    # c_tmp.BuildLegend(0.75,0.62,0.95,0.84)
    # c_tmp.BuildLegend(0.75,0.42,0.95,0.65)
    c_tmp.BuildLegend(0.75,0.57,0.95,0.8)

    # c_tmp.BuildLegend(0.83, 0.83, 0.99, 0.99)
    # c_tmp.BuildLegend(0.5, 0.3, 0.8, 0.8)
    # c_tmp.BuildLegend(0.6, 0.1, 0.9, 0.6)
    c_tmp.SaveAs(fn_)
    # h_.SetName('h')
    # h_.SaveAs("h.root")    

def SetBinLabels(gr_,Npoints_,plotLabels_):
    # nBins = gr_.GetNbinsX()
    nBins = Npoints_
    for i in range(Npoints_):
        gr_.GetXaxis().SetBinLabel(int(nBins*(2*i+1)/(2*Npoints_)),plotLabels_[i])
        if(Npoints_ > 3): gr_.GetXaxis().SetLabelSize(0.07)
        else: gr_.GetXaxis().SetLabelSize(0.12)

def SimpleDrawHisto(h_,option_,outName_,v_):
    c = TCanvas()
    h_.Draw(option_)
    c.SaveAs(outName_)

def GetVars(cutName_):
    finalStateVars_ = [] 
    # p4_variables = ["E","pt","px","py","pz","eta","phi"]

    # p4_variables = ["E","pt","eta","phi"]
    # checkN = 2
    # objectVectors = [] 
    # objs = ["Electrons","Muons","Jets"]
    # vecTypes = ["all","good"]
    # for t in vecTypes:
    #     for o in objs:
    #         objVec = "%s%s"%(t,o)
    #         objectVectors.append(objVec)    
    # for objV in objectVectors:
    #     vtitle = "N_%s"%(objV) 
    #     # vname = "%s.size()"%(objV)  
    #     entry = "%s"%(vtitle)
    #     finalStateVars_.append(entry)
    #     for v in p4_variables:
    #         for i in range(checkN):
    #             vtitle = "%s_%s_%s"%(objV,i,v)
    #             # vname = "? %s.size() >= %s ? %s[%s].p4().%s() : -999"%(objV,i+1,objV,i,v)
    #             entry = "%s"%(vtitle)
    #             finalStateVars_.append(entry)
    #     if("Electrons" in objV):
    #         eVars = ["passLooseId","passMediumId","passTightId","passMVALooseId","passMVAMediumId","passMVATightId"]
    #         for eV in eVars:
    #             for i in range(checkN):
    #                 vtitle = "%s_%s_%s"%(objV,i,eV)
    #                 # vname = "? %s.size() >= %s ? %s[%s].%s() : -999"%(objV,i+1,objV,i,eV)
    #                 entry = "%s"%(vtitle)
    #                 finalStateVars_.append(entry)
    #     if("Muons" in objV):
    #         mVars = ["pfIsolationR04().sumChargedHadronPt","pfIsolationR04().sumNeutralHadronEt","pfIsolationR04().sumPhotonEt",
    #                     "pfIsolationR04().sumPUPt"]
    #         mVarTitles = ["sumChargedHadronPt","sumNeutralHadronEt","sumPhotonEt","sumPUPt"]
    #         for imV,mV in enumerate(mVars):
    #             mVarTitle = mVarTitles[imV]
    #             for i in range(checkN):
    #                 vtitle = "%s_%s_%s"%(objV,i,mVarTitle)
    #                 # vname = "? %s.size() >= %s ? %s[%s].%s() : -999"%(objV,i+1,objV,i,mV)
    #                 entry = "%s"%(vtitle)
    #                 finalStateVars_.append(entry)  

    #     # var1 = "jet" + str(jeti) + "_DeepFlavourScore[2,0,2] := ? JetVector.size() >= " + str(jeti + 1) + " ? JetVector[" + str(jeti) + "].bDiscriminator('mini_pfDeepFlavourJetTags:probb') : -99 "  
    #     if("Jets" in objV):
    #         bscores = ["bDiscriminator('mini_pfDeepFlavourJetTags:probb')","bDiscriminator('pfDeepCSVJetTags:probb')",
    #                     "bDiscriminator('mini_pfDeepFlavourJetTags:probbb')","bDiscriminator('pfDeepCSVJetTags:probbb')"]
            
    #         btitles = ["bDiscriminator_mini_pfDeepFlavourJetTags_probb","bDiscriminator_pfDeepCSVJetTags_probb",
    #                     "bDiscriminator_mini_pfDeepFlavourJetTags_probbb","bDiscriminator_pfDeepCSVJetTags_probbb"
    #                     ]
    #         for ib,bscore in enumerate(bscores):
    #             btitle = btitles[ib]
    #             for i in range(checkN):
    #                 vtitle = "%s_%s_%s"%(objV,i,btitle)
    #                 # vname = "? %s.size() >= %s ? %s[%s].%s : -999"%(objV,i+1,objV,i,bscore)
    #                 entry = "%s"%(vtitle)
    #                 finalStateVars_.append(entry)                                                                      

    # # for removal of prompt-prompt events from QCD and GJet samples 
    # finalStateVars_.append("Leading_Photon_genMatchType")
    # finalStateVars_.append("Subleading_Photon_genMatchType")

    # Add photon variables 
    vars = ["E","pt","eta","phi"]
    objects = ["Leading_Photon","Subleading_Photon","MET"]
    # finalStateVars_ = []
    finalStateVars_.append("Leading_Photon_MVA")
    finalStateVars_.append("Subleading_Photon_MVA")
    for obj in objects:
        for var in vars:
            # vname = "%s.p4().%s()"%(obj,var)
            vtitle = "%s_%s"%(obj,var)
            # entry = "%s:=%s"%(vtitle,vname)
            finalStateVars_.append(vtitle)    
  
    return finalStateVars_ 

def GetBins(variable_):
    binDict = {
        "Leading_Photon_MVA": [20,-1,1],
        "Subleading_Photon_MVA": [20,-1,1],
        # "CMS_hgg_mass": [80,100,180],
        # "CMS_hgg_mass": [24,100,180],
        "CMS_hgg_mass": [30,100,180],
        "weight":[1000,-2,2],
        "puweight":[1000,-2,2],
    }    
    otherVars = ["Leading_Photon_MVA","Subleading_Photon_MVA","CMS_hgg_mass","weight","puweight"]
    if variable_ in otherVars:
        return binDict[variable_]
    elif "N_" in variable_:
        return [10,0,10]
    else:
        if ("eta" in variable_) or ("phi" in variable_):
            return [16,-4,4]
        elif ("pt" in variable_):
            return [20,0,200]   
        else:
            return [10,0,100]

def SetStyle():
    gStyle.SetPadTickX(1)
    gStyle.SetPadTickY(1)
    gStyle.SetPadGridX(1)
    gStyle.SetPadGridY(1)
    # gStyle.SetOptStat(0)

def PlotDataMC(dataFiles_,mcFiles_,signalFiles_,dataDirec_,mcDirec_,signalDirec_,ShortVarsList_,ShortCutsList_,drawPads_,Lumi_,SigScale_,ol_,log_,Tags_):
    print"Plotting Data / MC"
    # kPrint, kInfo, kWarning, kError, kBreak, kSysError, kFatal
    gROOT.ProcessLine("gErrorIgnoreLevel = kError")
    gStyle.SetOptStat(0)    
    gStyle.SetErrorX(0.0001)

    cuts = ["1", "passPhotonSels == 1", "passbVeto == 1", "ExOneLep == 1", "goodJets == 1"] # preselections, photon sels, bVeto, exactly 1 lepton, at least 2 good jets
    cutNames = ["PreSelections","PhotonSelections","bVeto","OneLep","TwoGoodJets"]
    HHWWggTags = []
    for t in Tags_:
        HHWWggTags.append(t)
    if(ShortCutsList_): 
        cuts = ["1"]
        cutNames = ["PreSelections"]
    for dF_ in dataFiles_:
        for HHWWggTag in HHWWggTags:
            dPath = "%s/%s"%(dataDirec_,dF_)
            dFile = TFile.Open(dPath)
            if(HHWWggTag=="combined"):
                ch = TChain('tagsDumper/trees/Data_13TeV_HHWWggTag_0')
                ch.Add("%s/tagsDumper/trees/Data_13TeV_HHWWggTag_0"%(dPath))
                ch.Add("%s/tagsDumper/trees/Data_13TeV_HHWWggTag_1"%(dPath))
                ch.Add("%s/tagsDumper/trees/Data_13TeV_HHWWggTag_2"%(dPath))
            else:
                ch = TChain('tagsDumper/trees/Data_13TeV_%s'%(HHWWggTag))
                ch.Add(dPath)

            BLIND_CUT = "(CMS_hgg_mass < 115 || CMS_hgg_mass > 135)"
            MC_WEIGHT = "1*weight"
            ZERO_CUT = "ZERO_CUT"
            MC_CUT = "%s*(%s)*(%s)"%(MC_WEIGHT,BLIND_CUT,ZERO_CUT)
            DATA_CUT = "%s*(%s)"%(BLIND_CUT,ZERO_CUT)       
            SIGNAL_CUT = "%s*(%s)"%(MC_WEIGHT,ZERO_CUT) # no blind cut on signal 

            for ic,cut in enumerate(cuts):
                cutName = cutNames[ic]
                outputFolder = "%s/%s"%(ol_,cutName)
                if(not os.path.exists(outputFolder)):
                    os.system('mkdir %s'%(outputFolder))
                    os.system('cp %s/index.php %s'%(ol_,outputFolder))
                MC_CUT += "*(%s)"%(cut)
                DATA_CUT += "*(%s)"%(cut)      
                SIGNAL_CUT += "*(%s)"%(cut) 
                finalStateVars = GetVars(cutName) # get certain vars based on cut 
                print"vars:",finalStateVars 
                # if(ShortVarsList_): finalStateVars = ["CMS_hgg_mass","Leading_Photon_E"]                 
                if(ShortVarsList_): 
                    finalStateVars = ["CMS_hgg_mass"]

                    # finalStateVars = ["CMS_hgg_mass","Leading_Photon_pt","Subleading_Photon_pt",
                    # # finalStateVars = ["CMS_hgg_mass"
                    #                     "N_allElectrons","N_allMuons","N_allJets",
                    #                     "N_goodElectrons","N_goodMuons","N_goodJets",
                                        # "goodElectrons_0_pt","goodMuons_0_pt",
                                        # "goodJets_0_pt","goodJets_1_pt",
                                        # "MET_pt","Leading_Photon_MVA","Subleading_Photon_MVA"    
                                    # ]                 
                # if(ShortVarsList_): finalStateVars = ["puweight","weight","CMS_hgg_mass","Leading_Photon_E"]                 
                for v in finalStateVars: 
                    MC_CUT = MC_CUT.replace("ZERO_CUT","(%s != 0) && (%s != -999)"%(v,v))
                    DATA_CUT = DATA_CUT.replace("ZERO_CUT","(%s != 0) && (%s != -999)"%(v,v))
                    SIGNAL_CUT = SIGNAL_CUT.replace("ZERO_CUT","(%s != 0) && (%s != -999)"%(v,v))
                    print"MC_CUT:",MC_CUT         
                    print"DATA_CUT:",DATA_CUT                   
                    # legend = TLegend(0.5,0.7,0.89,0.89)
                    # legend = TLegend(0.65,0.7,0.89,0.89)
                    # legend = TLegend(0.55,0.7,0.89,0.89)
                    legend = TLegend(0.55,0.65,0.89,0.89)
                    # legend.SetTextSize(0.015)
                    legend.SetTextSize(0.025)
                    legend.SetBorderSize(0)
                    legend.SetFillStyle(0)
                    xbins, xmin, xmax = GetBins(v)

                    ##-- Get Data 
                    # print"xbins xmin xmax",xbins, xmin, xmax 
                    Data_h_tmp = TH1F('Data_h_tmp',v,xbins,xmin,xmax)
                    Data_h_tmp.SetTitle("%s"%(v))
                    Data_h_tmp.SetMarkerStyle(8)
                    exec('ch.Draw("%s >> Data_h_tmp","%s")'%(v,DATA_CUT))
                    print"tag:",HHWWggTag
                    print"numEvents:",Data_h_tmp.GetEntries()                    
                    DataHist = Data_h_tmp.Clone("DataHist")
                    DataHist.SetDirectory(0)
                    legend.AddEntry(DataHist,"Data","P")

                    ##-- Get MC Backgrounds 
                    bkgStack = THStack("bkgStack","bkgStack")
                    histos = []
                    histCategories = [] 
                    for i,mcF_ in enumerate(mcFiles_):
                        mcPath = "%s/%s"%(mcDirec_,mcF_)
                        mcFile = TFile.Open(mcPath)
                        # print"Background File:",mcPath
                        treeName = GetMCTreeName(mcF_)
                        MC_Category = GetMCCategory(mcF_)
                        print"Background:",MC_Category
                        ##-- If HHWWgg_bkg, need to multiply by another weight 
                        ##-- MC_WEIGHT = "%s*%s"%(MC_WEIGHT,HHWWgg_MC_Weight)
                        if(HHWWggTag=="combined"):
                            mc_ch = TChain('tagsDumper/trees/%s_13TeV_HHWWggTag_0'%(treeName))
                            mc_ch.Add("%s/tagsDumper/trees/%s_13TeV_HHWWggTag_0"%(mcPath,treeName))
                            mc_ch.Add("%s/tagsDumper/trees/%s_13TeV_HHWWggTag_1"%(mcPath,treeName))
                            mc_ch.Add("%s/tagsDumper/trees/%s_13TeV_HHWWggTag_2"%(mcPath,treeName))
                        else:
                            mc_ch = TChain('tagsDumper/trees/%s_13TeV_%s'%(treeName,HHWWggTag))
                            mc_ch.Add(mcPath)
                        xbins, xmin, xmax = GetBins(v)
                        # print"tag:",HHWWggTag
                        # print"numEvents:",mc_ch.GetEvents()                        
                        # print"xbins xmin xmax",xbins, xmin, xmax 
                        exec("MC_h_tmp_%s = TH1F('MC_h_tmp_%s',v,xbins,xmin,xmax)"%(i,i))
                        thisHist = eval("MC_h_tmp_%s"%(i))
                        mcColor = GetMCColor(MC_Category)
                        # print"mcColor:",mcColor

                        if(MC_Category == "GJet" or MC_Category == "QCD"):
                            # print"Remove prompt-prompt"
                            removePromptPromptCut = "(!((Leading_Photon_genMatchType == 1) && (Subleading_Photon_genMatchType == 1)))" # selection: not true that both photons are prompt
                            removePromptPromptCut += "*(!((Leading_Photon_genMatchType == 0) || (Subleading_Photon_genMatchType == 0)))" # selection: not true that both photons are prompt
                            original_MC_CUT = "%s"%(MC_CUT)
                            this_MC_CUT = "%s*(%s)"%(original_MC_CUT,removePromptPromptCut)
                            # print"this_MC_CUT:",this_MC_CUT                     

                        eval("MC_h_tmp_%s.SetFillColor(eval(mcColor))"%(i))
                        eval("MC_h_tmp_%s.SetLineColor(eval(mcColor))"%(i))
                        if(MC_Category == "GJet" or MC_Category == "QCD"): exec('mc_ch.Draw("%s >> MC_h_tmp_%s","%s")'%(v,i,this_MC_CUT))
                        else: exec('mc_ch.Draw("%s >> MC_h_tmp_%s","%s")'%(v,i,MC_CUT))
                        eval("MC_h_tmp_%s.Scale(float(Lumi_))"%(i))
                        ##-- Check if MC should be reweighted
                        reWeightVals = ReWeightMC(mcF_)
                        doReWeight, reWeightScale = reWeightVals[0], reWeightVals[1]
                        # print"doReWeight,reWeightScale:",doReWeight, reWeightScale
                        if(doReWeight):
                            print"ReWeighting MC"
                            print"With scale: ",reWeightScale
                            eval("MC_h_tmp_%s.Scale(float(reWeightScale))"%(i))
                        
                        newHist = thisHist.Clone("newHist")
                        # set title based on treeName 
                        newHist.SetTitle(MC_Category)
                        newHist.GetXaxis().SetTitle(mcF_)
                        newHist.SetDirectory(0)
                        histos.append(newHist)
                        histCategories.append(MC_Category)

                    sig_histos = []
                    sig_histCategories = []             

                    ##-- Add Signal 
                    for i,sigF_ in enumerate(signalFiles_):
                        sigPath = "%s/%s"%(signalDirec_,sigF_)
                        sigFile = TFile.Open(sigPath)
                        # print"Signal File:",sigPath
                        treeName = GetMCTreeName(sigF_)
                        MC_Category = GetMCCategory(sigF_)
                        print"Signal:",MC_Category
                        ##-- If HHWWgg_bkg, need to multiply by another weight 
                        ##-- MC_WEIGHT = "%s*%s"%(MC_WEIGHT,HHWWgg_MC_Weight)
                        if(HHWWggTag=="combined"):
                            mc_ch = TChain('tagsDumper/trees/%s_13TeV_HHWWggTag_0'%(treeName))
                            mc_ch.Add("%s/tagsDumper/trees/%s_13TeV_HHWWggTag_0"%(sigPath,treeName))
                            mc_ch.Add("%s/tagsDumper/trees/%s_13TeV_HHWWggTag_1"%(sigPath,treeName))
                            mc_ch.Add("%s/tagsDumper/trees/%s_13TeV_HHWWggTag_2"%(sigPath,treeName))
                        else:
                            mc_ch = TChain('tagsDumper/trees/%s_13TeV_%s'%(treeName,HHWWggTag))
                            mc_ch.Add(sigPath)
                        xbins, xmin, xmax = GetBins(v)
                        exec("MC_h_tmp_%s = TH1F('MC_h_tmp_%s',v,xbins,xmin,xmax)"%(i,i))
                        thisHist = eval("MC_h_tmp_%s"%(i))
                        mcColor = GetMCColor(MC_Category) 
                        # eval("MC_h_tmp_%s.SetFillColor(eval(mcColor))"%(i))
                        # eval("MC_h_tmp_%s.SetFillStyle(3004)"%(i))
                        eval("MC_h_tmp_%s.SetFillColorAlpha(eval(mcColor),0.1)"%(i))
                        eval("MC_h_tmp_%s.SetLineColor(eval(mcColor))"%(i))
                        exec('mc_ch.Draw("%s >> MC_h_tmp_%s","%s")'%(v,i,SIGNAL_CUT))
                        # eval("MC_h_tmp_%s.Scale(float(Lumi_))"%(i))
                        # eval("MC_h_tmp_%s.Scale(float(SigScale_))"%(i))
                        newHist = thisHist.Clone("newHist")

                        # set title based on treeName 
                        newHist.SetTitle(MC_Category)
                        newHist.GetXaxis().SetTitle(sigF_)
                        newHist.SetLineStyle(1)
                        newHist.SetLineWidth(5)

                        newHist.SetDirectory(0)
                        sig_histos.append(newHist)
                        sig_histCategories.append(MC_Category)              
                        
                    MC_AddedtoLegend = {
                        "QCD" : 0,
                        "SMhgg" : 0,
                        "GJet" : 0,
                        "DiPhoJets" : 0,
                        "DiPhoJetsBox" : 0,
                        # "W1JetsToLNu" : 0,
                        # "W2JetsToLNu" : 0,
                        # "W3JetsToLNu" : 0,
                        # "W4JetsToLNu" : 0,
                        "WJets" : 0,
                        "WW" : 0,
                        "tt" : 0,
                        "DY" : 0,
                        "WGGJets" : 0,
                        "ttW" : 0
                    }

                    Signals_AddedtoLegend = {
                        "HHWWgg_SM" : 0
                    }

                    # order histograms by MC category 
                    orderedHistos = OrderHistos(histos,histCategories)
                    sig_orderedHistos = OrderHistos(sig_histos,sig_histCategories)

                    # for h in histos:
                    for h in orderedHistos:
                        bkgStack.Add(h,'hist')
                        bkgName = h.GetTitle()
                        added = MC_AddedtoLegend[bkgName]
                        # print"added:",added 
                        # print"bkgName:",bkgName
                        if(added): continue 
                        else:
                            legend.AddEntry(h,bkgName,"F")
                            MC_AddedtoLegend[bkgName] = 1
                            # print"color added:",h.GetFillColor()

                    for sig_h in sig_orderedHistos:
                        sigName = sig_h.GetTitle()
                        added = Signals_AddedtoLegend[sigName]
                        if(added): continue 
                        else:
                            legend.AddEntry(sig_h,sigName,"FL")
                            Signals_AddedtoLegend[sigName]

                        # legend.AddEntry(h,bkgName,"F")
                        # legend.AddEntry(h,bkgName,"F")
                        # Just add legend entry per category, not per histogram 
                        # If first histo in category, add legend entry 
                    # legend.AddEntry("")
                    # gStyle.SetOptStat(0)
                    outName = "%s/BackgroundsTest_%s.png"%(outputFolder,HHWWggTag)
                    bkgOutName = "%s/BackgroundsPADS_%s_%s.png"%(outputFolder,v,HHWWggTag)
                    # outName = "%s/BackgroundsTest.png"%(outputFolder)
                    # bkgOutName = "%s/BackgroundsPADS_%s.png"%(outputFolder,v)                    
                    SimpleDrawHisto(bkgStack,"PADS",bkgOutName,v)
                    bkgOutName = bkgOutName.replace(".png",".pdf")
                    SimpleDrawHisto(bkgStack,"PADS",bkgOutName,v)  

                    ##-- Add text box with selection type 
                    # offset = 0.05
                    offset = 0

                    # selText = TLatex(0.129+0.03+offset,0.85,cutName)
                    selText = TLatex(0.129,0.85,cutName)
                    selText.SetNDC(1)
                    selText.SetTextSize(0.04)   

                    # combinedText = TLatex(0.129,0.8,"Combined Cats")
                    CatText = TLatex(0.129,0.8,HHWWggTag)
                    CatText.SetNDC(1)
                    CatText.SetTextSize(0.04)                                   
                    # selText.SetTextAlign(33)
                    # selText.SetNDC(1)
                    # selText.SetTextSize(0.045)   
                    stackSum = bkgStack.GetStack().Last() #->Draw(); # for computing ratio 
                    stackSum.SetLineColor(kBlack)
                    stackSum.SetLineStyle(7) # to distinguish from data uncertainty 
                    DataHist.SetLineColor(kBlack)
                    if(log_): 
                        DataHist.SetMinimum(0.01)
                        stackSum.SetMinimum(0.01)
                        bkgStack.SetMinimum(0.01)
                    rp = TRatioPlot(DataHist,stackSum)
                    rp.SetH1DrawOpt("P")
                    rp.SetH2DrawOpt("hist")
                    # rp.SetGraphDrawOpt("PE2")
                    dMax = DataHist.GetMaximum()
                    bMax = stackSum.GetMaximum()
                    # print'dMax:',dMax
                    # print'bMax:',bMax
                    maxHeight = max(dMax,bMax)

                    for fileType in ["pdf"]:
                        gStyle.SetErrorX(0.0001)

                        # outName = "%s/Data_%s.%s"%(outputFolder,v,fileType)
                        # outName = "%s/DataMC_%s_%s.%s"%(outputFolder,v,HHWWggTag,fileType)
                        # if(log_): outName = "%s/DataMC_%s_log.%s"%(outputFolder,v,fileType)
                        # else: outName = "%s/DataMC_%s_nonLog.%s"%(outputFolder,v,fileType)
                        outName = "%s/DataMC_%s_%s.%s"%(outputFolder,v,HHWWggTag,fileType)
                        if(log_): outName = "%s/DataMC_%s_%s_log.%s"%(outputFolder,v,HHWWggTag,fileType)
                        else: outName = "%s/DataMC_%s_%s_nonLog.%s"%(outputFolder,v,HHWWggTag,fileType)                        
                        # outName = "%s/DataMC_%s.%s"%(outputFolder,v,fileType)
                        # gStyle.SetOptStat(0)
                        # DataMCRatio_c = TCanvas("DataMCRatio_c","DataMCRatio_c")
                        DataMCRatio_c = TCanvas("DataMCRatio_c","DataMCRatio_c",600,800)
                        # DataMCRatio_c.SetTicks(0,1)
                        rp.Draw("nogrid")
                        rp.GetLowYaxis().SetNdivisions(5)
                        DataMCRatio_c.Update()

                        # yaxis = rp.GetLowerRefGraph().GetYaxis()
                        # print"n divisions:",yaxis.GetNdivisions()
                        # yaxis.SetNdivisions(3)
                        # print"n divisions:",yaxis.GetNdivisions()

                        ratioGraph = rp.GetCalculationOutputGraph()
                        ratioGraph.SetMarkerStyle(8)
                        ratioGraph.SetMarkerSize(0.5)

                        # rp.SetGraphDrawOpt("EP")
                        # rp.SetGraphDrawOpt("EPZ2")
                        # rp.GetLowerRefYaxis().SetTitle("Data / MC")

                        # yaxis.SetNdivisions(2)   
                        # print"offset:",rp.GetLowerRefYaxis().GetTitleOffset()
                        # rp.GetUpperRefYaxis().SetTitleOffset(-0.0000001)
                        # print"offset",rp.GetUpperRefYaxis().GetTitleOffset()
                        rp.GetUpperRefYaxis().SetTitle("Entries")   
                        rp.GetLowerRefYaxis().SetTitle("Data / MC")
                        rp.GetLowerPad().Update()
                        if(log_): rp.GetUpperRefYaxis().SetRangeUser(0.1,maxHeight*100.)   
                        else: rp.GetUpperRefYaxis().SetRangeUser(0,maxHeight*1.3)
                                
                        UpperPad = rp.GetUpperPad()
                        UpperPad.cd()
                        bkgStack.Draw("same")
                        stackSum.Draw("sameE")
                        # sig_histos[0].Draw("sameC")
                        # for sigHist in sig_histos:
                            # sigHist.Draw("sameC")
                        # bkgStack.Draw("same")
                        DataHist.Draw("samePE")
                        for sig_hist in sig_histos:
                            sigMax = sig_hist.GetMaximum()
                            if sigMax == 0: sigMax = 1 
                            sigScale = float(maxHeight) / float(sigMax)
                            sig_hist.Scale(sigScale)
                            sig_hist.Draw("samehist")
                        legend.Draw("same")
                        selText.Draw("same")
                        CatText.Draw("same")
                        # ##-- Add text box with selection type 
                        # selText = TLatex(0.93,0.88,cutName)
                        # selText.SetTextAlign(33)
                        # selText.SetNDC(1)
                        # selText.SetTextSize(0.045)                       

                        # LowerPad = rp.GetLowerPad()
                        # rp.GetLowerRefGraph().SetMinimum(0)
                        # rp.GetLowerRefGraph().SetMaximum(2) 
                        rp.GetLowerRefGraph().SetMinimum(0.5)
                        rp.GetLowerRefGraph().SetMaximum(1.5)                     
                        Npoints = rp.GetLowerRefGraph().GetN()
                        for ip in range(0,Npoints):
                            # yError = rp.GetLowerRefGraph().GetErrorY()
                            rp.GetLowerRefGraph().SetPointEXhigh(ip,0)  
                            rp.GetLowerRefGraph().SetPointEXlow(ip,0)  
                        if(log_): 
                            # UpperPad.cd()
                            UpperPad.SetLogy()
                            UpperPad.Update() 
                            # don't know why this removes Data / MC title   
                            # 
                        rp.GetLowerPad().cd()
                        lowerPad = rp.GetLowerPad()
                        rp.GetLowerRefYaxis().SetTitle("Data / MC")
                        lineAtOne = TLine(lowerPad.GetUxmin(),1,lowerPad.GetUxmax(),1)
                        lineAtOne.SetLineStyle(3)
                        lineAtOne.Draw("same")
                        rp.GetLowerPad().Update()                        
                        DataMCRatio_c.Update()                
                        DataMCRatio_c.SaveAs(outName) 
                        outName = outName.replace(".pdf",".png")                    
                        DataMCRatio_c.SaveAs(outName)                     
                    if(not drawPads_):
                        bkgOutName = "%s/BackgroundsPADS_%s_%s.png"%(outputFolder,v,HHWWggTag)
                        os.system('rm %s'%(bkgOutName))
                        bkgOutName = bkgOutName.replace(".png",".pdf")
                        os.system('rm %s'%(bkgOutName))
                    MC_CUT = MC_CUT.replace("(%s != 0) && (%s != -999)"%(v,v),"ZERO_CUT")
                    DATA_CUT = DATA_CUT.replace("(%s != 0) && (%s != -999)"%(v,v),"ZERO_CUT")