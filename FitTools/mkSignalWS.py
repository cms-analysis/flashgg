import ROOT
import optparse
import argparse

parser = argparse.ArgumentParser(description='make WS')

parser.add_argument('--mass', metavar='mass', type=str, help='mass of a',required=True)
args = parser.parse_args()

mass  = args.mass
print mass
file = '/eos/cms/store/user/twamorka/NTuples_17Feb2019/Signal/signal_m_' + str(mass) + '.root'
chain = ROOT.TChain('h4gCandidateDumper/trees/SUSYGluGluToHToAA_AToGG_M_'+ str(mass) +'_TuneCUETP8M1_13TeV_pythia8_13TeV_4photons')
chain.Add(file)

h = ROOT.TH1F("hist","Avg, Diphoton mass;Mass[GeV];# of events",100, 0, 100)
# h = ROOT.TH1F("hist","Avg, Diphoton mass;Mass[GeV];# of events",100, 0, 10)
chain.Draw('avg_dp_mass >> hist')
fitResultPtr = h.Fit("gaus","S")

mean =  fitResultPtr.GetParams()[1]
sigma = fitResultPtr.GetParams()[2]

# print "mean =",mean, " ", "mean + 2sigma =", mean + 2*sigma, " ", "mean - 2sigma =", mean - 2*sigma

name_root_file_with_workspace = "w_signal_"+ str(mass) +".root"

root_file_with_workspace = ROOT.TFile (name_root_file_with_workspace, "RECREATE")
root_file_with_workspace.mkdir("tagsDumper")
root_file_with_workspace.cd("tagsDumper")

w = ROOT.RooWorkspace("cms_h4g_13TeV_4photons")
IntLumi = 1000.0

w.factory("weight[0,7e+06]")
w.factory("dZ[-100000,1000000]")
w.factory("IntLumi[1000.]")
w.factory("avg_dp_mass[0,1000]")
w.factory("tp_mass[0,1000]")

wsVars = ROOT.RooArgSet()
wsVars.add(w.var("weight"))
wsVars.add(w.var("dZ"))
wsVars.add(w.var("IntLumi"))
wsVars.add(w.var("avg_dp_mass"))
wsVars.add(w.var("tp_mass"))

data_RooDataSet = ROOT.RooDataSet( "Data_13TeV_4photons", "Data_13TeV_4photons", chain, wsVars )

data_reduced_RooDataSet = data_RooDataSet.reduce("avg_dp_mass >"+ str (mean - 2*sigma) + " && avg_dp_mass <"+ str (mean + 2*sigma) )

getattr(w,'import')(data_reduced_RooDataSet)

w.Write()
