#########################################################################################################################################################################################################
# Abraham Tishelman-Charny
# 13 May 2020
#
# The purpose of this module is to analyze HHWWgg ntuples. The current options are:
# - Cut flow efficiencies
# - Data / MC comparison 
#
# Example Usage: 
#
# ##-- Efficiency Analysis
# python NtupleAnalysis.py --Efficiency --folder HHWWgg_v2-4_NMSSM_CutFlow_Hadded/ --note NMSSM_Test --NMSSM --SumTags
# python NtupleAnalysis.py --Efficiency --folder HHWWgg_v2-4_CutFLow_Hadded/ --note EFT_Test --EFT --SumTags 
# 
# ##-- Efficiency Ratio 
# python NtupleAnalysis.py --Efficiency --folders HHWWgg_v2-3_Trees_Hadded_some/,HHWWgg_v2-6_Trees_Hadded/ --campaigns HHWWgg_v2-3,HHWWgg_v2-6 --massPoints X1000 --Res --ratio
# 
# ##-- Data / MC Analysis
# python NtupleAnalysis.py --DataMC --dataFolder 2017Data_Trees_Hadded_Combined --mcFolder DataMC_2017 --signalFolder DataMC_2017_Signal --ShortVarsList --Lumi 41.5 --Tags HHWWggTag_0,HHWWggTag_1,HHWWggTag_2,combined 
# python NtupleAnalysis.py --DataMC --dataFolder 2017Data_Trees_Hadded_Combined --mcFolder DataMC_2017 --signalFolder DataMC_2017_Signal --ShortVarsList --Lumi 41.5 --Tags HHWWggTag_0,HHWWggTag_1,HHWWggTag_2,combined --SigScale 1 --ShortCutsList
#
# python NtupleAnalysis.py --DataMC --dataFolder 2017Data_Trees_Hadded_Combined --mcFolder DataMC_2017 --signalFolder DataMC_2017_Signal --ShortVarsList --Lumi 41.5 --Tags HHWWggTag_0,HHWWggTag_1,HHWWggTag_2,combined --SigScale 25
#
# python NtupleAnalysis.py --DataMC --dataFolder 20May_2017Data_FinalStateVars_Hadded_Combined --mcFolder HHWWgg_fggBackgrounds_v2_1_oneDY_Hadded --ShortVarsList --Lumi 41.5
# python NtupleAnalysis.py --DataMC --dataFolder 20May_2017Data_FinalStateVars_Hadded_Combined --mcFolder HHWWgg_Backgrounds_Hadded --ShortCutsList --Lumi 41.5 --log
#########################################################################################################################################################################################################

from NtupleAnalysisTools import * 
from ROOT import gPad, TAxis, TTree, TChain, TLine, TGraph, TMultiGraph, TFile, TCanvas, gROOT, TH2F, TH1F, kPink, kGreen, kCyan, TLegend, kRed, kOrange, kBlack, TLegend, gStyle, TObjArray, kBlue, TGraphErrors
import argparse 
from array import array
import os 
from tkinter import Tcl

parser = argparse.ArgumentParser()

##-- Choose Analysis Option
parser.add_argument("--Efficiency", action="store_true", default=False, help="Compute cutflow efficiency", required=False)
parser.add_argument("--DataMC", action="store_true", default=False, help="Produce Data / MC Comparisons", required=False)

##-- Efficiency Plots 
parser.add_argument("--ratio", action="store_true", default=False, help="Efficiency Ratio", required=False)
parser.add_argument('--folders', type=str, default="", help="Comma separated list of ntuple folders", required=False)
parser.add_argument('--campaigns', type=str, default="", help="Comma separated list of campaigns", required=False)
parser.add_argument('--massPoints', type=str, default="", help="Comma separated list of mass points to run", required=False)

parser.add_argument("-p","--plot", action="store_true", default=False, help="Plot", required=False)
parser.add_argument("-n","--norm", action="store_true", default=False, help="normalize plots", required=False)
parser.add_argument("-df","--df", action="store_true", default=False, help="deep flavour b score", required=False)
parser.add_argument("-csv","--dcsv", action="store_true", default=False, help="deep csv b score", required=False)
parser.add_argument("-l","--log", action="store_true", default=False, help="Log y scale plot", required=False)
parser.add_argument("--Res", action="store_true", default=False, help="Resonant analysis", required=False)
parser.add_argument("--EFT", action="store_true", default=False, help="EFT analysis", required=False)
parser.add_argument("--NMSSM", action="store_true", default=False, help="NMSSM Analysis", required=False)
parser.add_argument("--SumTags", action="store_true", default=False, help="Sum entries from tags", required=False)
parser.add_argument('--note', type=str, default="", help="Note for titles and file path", required=False)
parser.add_argument('--folder', type=str, default="", help="Input folder with hadded files", required=False)

##-- Data / MC comparison 
parser.add_argument('--dataFolder', type=str, default="", help="Input folder with hadded Data ntuples", required=False)
parser.add_argument('--mcFolder', type=str, default="", help="Input folder with hadded MC ntuples", required=False)
parser.add_argument('--signalFolder', type=str, default="", help="Input folder with hadded Signal ntuples", required=False)
parser.add_argument("--ShortVarsList", action="store_true", default=False, help="Shorten Data / MC variable list for quicker testing", required=False)
parser.add_argument("--ShortCutsList", action="store_true", default=False, help="Shorten Data / MC cut list for quicker testing", required=False)
parser.add_argument("--drawPads", action="store_true", default=False, help="Draw each MC contribution to stack", required=False)
parser.add_argument('--Lumi', type=float, default=0, help="Luminosity for scaling MC (in fb-1)", required=False)
parser.add_argument('--SigScale', type=float, default=1, help="Artificial scale for signal", required=False)
parser.add_argument('--Tags', type=str, default="", help="Comma separated list of tags to run. Ex: HHWWggTag_0,HHWWggTag_1,HHWWggTag_2 or HHWWggTag_2 or HHWWggTag_2,combined", required=False)

args = parser.parse_args()

if __name__ == '__main__':
    gROOT.SetBatch(1) # Do not output upon draw statement 
    if(args.Efficiency): ol = '/eos/user/a/atishelm/www/HHWWgg/NtupleAnalysis/cutFlow/'
    elif(args.DataMC): ol = '/eos/user/a/atishelm/www/HHWWgg/NtupleAnalysis/DataMC_v3/'    
    prefix = '/eos/user/a/atishelm/ntuples/HHWWgg/'

    if(args.Efficiency):
        print"Performing cut flow efficiency analysis"
        if(args.ratio):
            nMassPoints = len(args.massPoints.split(','))
            ratio_x_vals = []
            for i in range(5):
                exec("c1_y_vals_%s = array( 'd' )"%(i)) # campaign 1 efficiencies          
                exec("c2_y_vals_%s = array ( 'd' )"%(i)) # campaign 2 efficiencies     

        folders = args.folders.split(',')
        campaigns = args.campaigns.split(',')
        massPoints = args.massPoints.split(',')

        for iFolder,folder in enumerate(folders):
            campaign = campaigns[iFolder]

            # folder = str(args.folder)

            direc = prefix + folder
            colors = GetColors()

            x_values = array( 'd' )
            x_errors = array( 'd' )
            y_errors = array( 'd' )
            y_sigeff00 = array( 'd' ) # MicroAOD (efficiency of 1). Need # of events. 
            for i in range(5):
                exec("y_sigeff%s = array( 'd' )"%(i))
            Npoints = 0

            # For files in directory. Containing all combined for each mass point
            # unorderedFiles = []
            # for path in enumerate(os.listdir(direc)):
                # unorderedFiles.append(path)
            # orderedFiles = []
            # orderedFiles = Tcl().call('lsort', '-dict', os.listdir(direc))
            files = []
            for file in os.listdir(direc):
                files.append(file)

            orderedFiles_ = Tcl().call('lsort', '-dict', files)

            # print"files:",files
            orderedFiles = []
            print"orderedFiles:",orderedFiles_
            for i in orderedFiles_:
                # print"i:",i
                i = i.replace(",","")
                i = i.replace("[","")
                i = i.replace("]","")
                i = i.replace("'","")
                print"i:",i
                orderedFiles.append(i)

            # print"orderedFiles:",orderedFiles

            # badMasses = ['X750','SM'] # masses to not include in the plot 
            badMasses = []
            plotLabels = []
            
            # for fi,file in enumerate(os.listdir(direc)):
            # for fi,path in enumerate(files):
            for fi,path in enumerate(orderedFiles):
                print"path:",path
                Npass_0_tot, Npass_1_tot, Npass_2_tot, Npass_3_tot, Npass_4_tot = 0,0,0,0,0
                badmass = 0
                if(args.Res): 
                    mass = path.split('_')[1]
                    print'On mass:',mass
                    for bM in badMasses:   
                        if mass == bM: badmass = 1 
                    if (badmass): continue   
                    # print"massPoints:",massPoints
                    if(mass not in massPoints): continue           
                    x_values.append(float(mass[1:]))
                    if(iFolder==0): ratio_x_vals.append(float(mass[1:]))
                    
                    nTotEvents = GetEvents(mass,campaign)

                elif(args.EFT):
                    SM_point = 0
                    print "thing:",path.split('_')[0]
                    if path.split('_')[0] == "SM":
                        SM_point = 1 
                        x_values.append(fi)
                        nTotEvents = GetEvents("SM")
                        plotLabels.append("SM")

                    else:
                        BM = path.split('_')[3][4]
                        print("BM:",BM)
                        print"appending:",float(BM) + 1 
                        # x_values.append(float(BM) + 1)
                        x_values.append(fi)
                        nodeStr = "node%s"%(float(BM)+1)
                        nTotEvents = GetEvents("node%s"%BM)
                        print("nTotEvents:",nTotEvents)
                        nodeStr = nodeStr.replace("node","node ")
                        nodeStr = nodeStr.replace(".0","")
                        plotLabels.append(nodeStr)

                elif(args.NMSSM):
                    mx = path.split('_')[2]
                    my = path.split('_')[3]
                    massPair = "%s_%s"%(mx,my)
                    print"massPair:",massPair
                    x_values.append(fi) # file i 
                    nTotEvents = GetEvents(massPair)
                    massPairString = massPair.replace("_",", ")
                    plotLabels.append(massPairString)

                y_sigeff00.append(1) # Max efficiency
                Npoints += 1 
                x_errors.append(0.5)
                y_errors.append(0)
                # if(SM_point): ntags = 1
                # else: ntags = 2
                ntags = 3
                if(campaign=="HHWWgg_v2-3"): ntags = 3 
                elif(campaign=="HHWWgg_v2-6"): ntags = 3 

                print"ntags:",ntags

                # if(args.SumTags): ntags = 3 
                # if(args.SumTags): ntags = 2 
                # else: ntags = 1 
                for tag in range(0,ntags):
                    # if(not args.SumTags and tag == 1): continue  
                    # print'path:',path
                    # print'fi =',fi
                    # print'len(orderedFiles):',len(orderedFiles)
                    # if fi == len(orderedFiles) - 2: continue 
                    color = colors[fi]

                    print'Num events in MicroAOD:',nTotEvents 

                    signal_path = direc + '/' + str(path)
                    signal_file = TFile.Open(signal_path)
                    if(args.Res): treeEnd = 'ggF_' + mass + '_WWgg_qqlnugg_13TeV_HHWWggTag_%s'%(tag)
                    elif(args.EFT): 
                        if(SM_point): treeEnd = 'ggF_SM_WWgg_qqlnugg_13TeV_HHWWggTag_%s'%(tag)
                        else: treeEnd = 'GluGluToHHTo_WWgg_qqlnu_node%s_13TeV_HHWWggTag_%s'%(BM,tag)
                        
                    elif(args.NMSSM): treeEnd = 'NMSSM_XYHWWggqqlnu_%s_13TeV_HHWWggTag_%s'%(massPair,tag) 

                    sig_tree = signal_file.Get('tagsDumper/trees/' + treeEnd)

                    # outputName = mass + '_CutFlow_efficiencies.txt' # output text file path 
                    # EfficienciesTxt = "bthresh\tsigeff\tttHeff\n"

                    sig_h_0 = TH1F('sig_h_0','sig_h_0',2,0,2)
                    sig_h_1 = TH1F('sig_h_1','sig_h_1',2,0,2)
                    sig_h_2 = TH1F('sig_h_2','sig_h_2',2,0,2)
                    sig_h_3 = TH1F('sig_h_3','sig_h_3',2,0,2)
                    sig_h_4 = TH1F('sig_h_4','sig_h_4',2,0.,2)

                    sig_tree.Draw("passPS >> sig_h_0", "passPS == 1")
                    sig_tree.Draw("passPhotonSels >> sig_h_1","passPhotonSels == 1 && passPS == 1")
                    sig_tree.Draw("passbVeto >> sig_h_2","passPhotonSels == 1 && passPS && passbVeto == 1")
                    sig_tree.Draw("ExOneLep >> sig_h_3","passPhotonSels == 1 && passPS && passbVeto == 1 && ExOneLep == 1 ")
                    sig_tree.Draw("goodJets >> sig_h_4","passPhotonSels == 1 && passPS && passbVeto == 1 && ExOneLep == 1 && goodJets == 1")

                    Npass_0, Npass_1, Npass_2, Npass_3, Npass_4 = sig_h_0.GetEntries(), sig_h_1.GetEntries(), sig_h_2.GetEntries(), sig_h_3.GetEntries(), sig_h_4.GetEntries()
                    Npass_0_tot, Npass_1_tot, Npass_2_tot, Npass_3_tot, Npass_4_tot
                    for i in range(0,5):
                        # print("Npass:",eval("Npass_%s"%(i)))
                        exec("Npass_%s_tot += Npass_%s"%(i,i))
                    # Neff_0, Neff_1, Neff_2, Neff_3, Neff_4 = float(Npass_0) / float(nTotEvents), float(Npass_1) / float(nTotEvents), float(Npass_2) / float(nTotEvents), float(Npass_3) / float(nTotEvents), float(Npass_4) / float(nTotEvents)
                    # print'efficiencies,',Neff_0,Neff_1,Neff_2,Neff_3,Neff_4

                for i in range(0,5):
                    # print("Total pass:",eval("Npass_%s_tot"%(i)))
                    # print("Neff:",eval("Npass_%s_tot"%(i)))
                    exec("Neff_%s = float(Npass_%s_tot) / float(nTotEvents)"%(i,i))

                y_sigeff0.append(Neff_0)
                y_sigeff1.append(Neff_1)
                y_sigeff2.append(Neff_2)
                y_sigeff3.append(Neff_3)
                y_sigeff4.append(Neff_4)

                for i in range(5):
                    if(iFolder == 0): exec("c1_y_vals_%s.append(Neff_%s)"%(i,i)) # campaign 1 efficiencies 
                    if(iFolder == 1): exec("c2_y_vals_%s.append(Neff_%s)"%(i,i)) # campaign 1 efficiencies 

                # sig_tree.Draw(hasHighbjet + ' >> sig_h',hasHighbjet)
                # NsigPass = float(Nsig) - float(sig_h.GetEntries())
                # print'NsigPass: ',NsigPass
                # sig_eff = float(NsigPass) / float(Nsig)
                # print'Signal efficiency = ',sig_eff
                # y_sigeff.append(sig_eff)

                # EfficienciesTxt += str(bthresh) + "\t" +  str(sig_eff) + "\t" + str(ttH_eff) + "\n"
                # EfficienciesTxt += str(bthresh) + "\t" +  str(sig_eff) + "\t" + str(ttH_eff) + "\n"

                # with open(outputName, "w") as output:
                        # output.write(EfficienciesTxt) # write txt file 

            if(args.NMSSM) or (args.EFT):
                sig_eff_g_00 = TGraphErrors(Npoints,x_values,y_sigeff00,x_errors,y_errors)
                # SetBinLabels(sig_eff_g_00,Npoints,plotLabels)
                for i in range(0,5):
                    exec("sig_eff_g_%s = TGraphErrors(Npoints,x_values,y_sigeff%s,x_errors,y_errors)"%(i,i))      
                    # exec("SetBinLabels(sig_eff_g_%d,Npoints,plotLabels)"%(i))

            else:
                sig_eff_g_00 = TGraph(Npoints,x_values,y_sigeff00)
                for i in range(0,5):
                    exec("sig_eff_g_%s = TGraph(Npoints,x_values,y_sigeff%s)"%(i,i))

            SetGraphStyle(sig_eff_g_00,1,kRed, 20)
            SetGraphStyle(sig_eff_g_0,10,kBlue, 21)
            SetGraphStyle(sig_eff_g_1,9,kGreen, 22)
            SetGraphStyle(sig_eff_g_2,6,kPink, 23)
            SetGraphStyle(sig_eff_g_3,2,95,34) # orange 
            SetGraphStyle(sig_eff_g_4,7,9,33) # purple 

            if(args.NMSSM) or (args.EFT):
                SetGraphStyle(sig_eff_g_00,1,kRed,20)
                SetGraphStyle(sig_eff_g_0,1,kBlue, 21)
                SetGraphStyle(sig_eff_g_1,1,kGreen, 22)
                SetGraphStyle(sig_eff_g_2,1,kPink, 23)
                SetGraphStyle(sig_eff_g_3,1,95,34) # orange 
                SetGraphStyle(sig_eff_g_4,1,9,33) # purple         

            sig_eff_g_00.SetTitle("MicroAOD")
            sig_eff_g_0.SetTitle("Pass #gamma#gamma cuts")
            sig_eff_g_1.SetTitle("Pass #gamma cuts")
            sig_eff_g_2.SetTitle("passbVeto")
            sig_eff_g_3.SetTitle("Lepton")
            sig_eff_g_4.SetTitle("Jets")

            mg = TMultiGraph()
            if(args.Res): mg.SetTitle("Resonant Signal Efficiency Cut Flow;Radion Mass (GeV);Efficiency")
            elif(args.EFT): mg.SetTitle("Non-Resonant Signal Efficiency Cut Flow;;Efficiency")
            elif(args.NMSSM): mg.SetTitle("NMSSM Signal Efficiency Cut Flow;;Efficiency")

            if(args.NMSSM) or (args.EFT): drawOption = "P2"
            else: drawOption = "PL"

            mg.Add(sig_eff_g_00)
            for i in range(5):
                # eval("mg.Add(sig_eff_g_%d,'%s')"%(i,drawOption))
                eval("mg.Add(sig_eff_g_%d,)"%(i))

            mg.SetMinimum(0)

            # outName = ol + 'CutFlow' + '_' + args.campaign
            outName = ol + 'CutFlow' + '_' + campaign
            if args.log: outName += 'Log'
            if args.note is not "": outName += '_' + str(args.note)
            outName += '.png'

            if(args.NMSSM) or (args.EFT): 
                DrawNonResHistogram(mg,"AP",outName,args.log,Npoints,plotLabels)
                outName = outName.replace("png","pdf")
                DrawNonResHistogram(mg,"AP",outName,args.log,Npoints,plotLabels)
            else: 
                Draw_Histogram(mg,"APL",outName,args.log)
                outName = outName.replace("png","pdf")
                Draw_Histogram(mg,"APL",outName,args.log)

        if(args.ratio):
            print"Plotting ratio of efficiencies"
            print"ratio_x_vals:"
            print ratio_x_vals
            for i in range(5):
                print"c1_y_vals_%s"%(i)
                exec("print c1_y_vals_%s"%(i))
                print"c2_y_vals_%s"%(i)
                exec("print c2_y_vals_%s"%(i))      

            x_values = array( 'd' )
            y_ratioEff00 = array ( 'd' )
            for entry in ratio_x_vals:
                x_values.append(entry)
                y_ratioEff00.append(1)
            
            for i in range(5):
                exec("y_ratioEff%s = array( 'd' )"%(i))
            for i in range(len(args.massPoints.split(','))):
                for j in range(5):
                    ratio = eval("c1_y_vals_%s[%s] / c2_y_vals_%s[%s]"%(j,i,j,i))
                    eval("y_ratioEff%s.append(%s)"%(j,ratio))
                    
            Nmasses = len(args.massPoints.split(','))
            sig_ratioEff_g_00 = TGraph(Nmasses,x_values,y_ratioEff00)
            for i in range(0,5):
                exec("sig_ratioEff_g_%s = TGraph(Nmasses,x_values,y_ratioEff%s)"%(i,i))

            SetGraphStyle(sig_ratioEff_g_00,1,kRed, 20)
            SetGraphStyle(sig_ratioEff_g_0,10,kBlue, 21)
            SetGraphStyle(sig_ratioEff_g_1,9,kGreen, 22)
            SetGraphStyle(sig_ratioEff_g_2,6,kPink, 23)
            SetGraphStyle(sig_ratioEff_g_3,2,95,34) # orange 
            SetGraphStyle(sig_ratioEff_g_4,7,9,33) # purple  

            sig_ratioEff_g_00.SetTitle("MicroAOD")
            sig_ratioEff_g_0.SetTitle("Pass #gamma#gamma cuts")
            sig_ratioEff_g_1.SetTitle("Pass #gamma cuts")
            sig_ratioEff_g_2.SetTitle("passbVeto")
            sig_ratioEff_g_3.SetTitle("Lepton")
            sig_ratioEff_g_4.SetTitle("Jets")

            ratio_mg = TMultiGraph()

            if(args.Res): ratio_mg.SetTitle("Resonant Signal Efficiency Cut Flow;Radion Mass (GeV);Efficiency")
            # elif(args.EFT): mg.SetTitle("Non-Resonant Signal Efficiency Cut Flow;;Efficiency")
            # elif(args.NMSSM): mg.SetTitle("NMSSM Signal Efficiency Cut Flow;;Efficiency")

            if(args.NMSSM) or (args.EFT): drawOption = "P2"
            else: drawOption = "PL"

            ratio_mg.Add(sig_ratioEff_g_00)
            for i in range(5):
                # eval("mg.Add(sig_eff_g_%d,'%s')"%(i,drawOption))
                eval("ratio_mg.Add(sig_ratioEff_g_%d,)"%(i))

            # ratio_mg.SetMinimum(0)
            # ratio_mg.SetMaximum(1.01)
            # ratio_mg.SetMinimum(0.5)
# 
            # outName = ol + 'CutFlow' + '_' + args.campaign
            outName = ol + 'CutFlow' + '_' + campaign
            if args.log: outName += 'Log'
            if args.note is not "": outName += '_' + str(args.note)
            outName += '.png'

            if(args.NMSSM) or (args.EFT): 
                DrawNonResHistogram(ratio_mg,"AP",outName,args.log,Npoints,plotLabels)
                outName = outName.replace("png","pdf")
                DrawNonResHistogram(ratio_mg,"AP",outName,args.log,Npoints,plotLabels)
            else: 
                Draw_Histogram(ratio_mg,"APL",outName,args.log)
                outName = outName.replace("png","pdf")
                Draw_Histogram(ratio_mg,"APL",outName,args.log)
    
    elif(args.DataMC):
        print"Performing Data / MC Analysis"
        dataFolder = str(args.dataFolder)
        dataDirec = prefix + dataFolder
        dataFiles = []
        for file in os.listdir(dataDirec):
            dataFiles.append(file)
        # print"dataFiles:",dataFiles
        mcFolder = str(args.mcFolder)
        mcDirec = prefix + mcFolder 
        mcFiles = []
        for file in os.listdir(mcDirec):
            mcFiles.append(file)
        # print"mcFiles:",mcFiles 
        signalFolder = str(args.signalFolder)
        signalDirec = prefix + signalFolder 
        signalFiles = [] 
        for file in os.listdir(signalDirec):
            signalFiles.append(file)
        # print"signalFiles:",signalFiles
        Tags = args.Tags.split(',')
        PlotDataMC(dataFiles,mcFiles,signalFiles,dataDirec,mcDirec,signalDirec,args.ShortVarsList,args.ShortCutsList,args.drawPads,args.Lumi,args.SigScale,ol,args.log,Tags)
