import ROOT
import array
from optparse import OptionParser
import sys

def main(options):
    fdata = ROOT.TFile(options.input)
    fmc = ROOT.TFile(options.weight)
    hmc = fmc.Get("eta_vs_r9_norm")
    
    hmc.Scale(1/hmc.Integral())
    
    fdata.cd()
    directories = fdata.GetListOfKeys()

    outFile = options.input.split(".root")[0]+"_norm.root"    
    newFile = ROOT.TFile(outFile, "RECREATE")

    for d in directories:
        print "Reducing tree in directory: ", d.GetName()
        directory = fdata.Get(d.GetName())

        tree = directory.Get("fitter_tree")
        entries = tree.GetEntries()  

        #--- Write to new file
        #tree.SetBranchStatus("*", 1)
        #tree.SetBranchStatus("totWeight", 0) 
        #tree.SetBranchStatus("PUweight", 0) 

        directory_new = newFile.mkdir(d.GetName())
        directory_new.cd()
        tree_new = tree.CloneTree(0)

        #tree.SetBranchStatus("totWeight", 1)
        #tree.SetBranchStatus("PUweight", 1)

        # FIXME TO NORMALIZE TO LUMINOSITY
        
        b_totWeight = array.array('f', [0])
        b_PUweight = array.array('f', [0])
        tree_new.Branch("totWeight", b_totWeight, "totWeight/F")
        tree_new.Branch("PUweight", b_PUweight, "PUweight/F")
        
        for z in range(entries):
            tree.GetEntry(z)
            #if (tree.weight >=0):
            b_totWeight[0] = hmc.GetBinContent(hmc.FindBin(tree.probe_Pho_r9, tree.probe_sc_abseta))
            tree_new.Fill()
        tree_new.GetCurrentFile().Write()

    tree_new.GetCurrentFile().Close() 


if __name__ == "__main__":  
    parser = OptionParser()
                    #../data/r9_eta_weights.root -input ../../output_histos.root
    parser.add_option("-w", "--weight",   default="../data/r9_eta_weights.root",   help="weights input filename")
    parser.add_option("-i", "--input", default="../../output_histos.root", help="Data input filename")

    (options, arg) = parser.parse_args()
    
    main(options)

