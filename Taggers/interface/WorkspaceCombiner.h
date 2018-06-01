// Benoit Courbon
// https://raw.githubusercontent.com/bcourbon/flashgg/132cd5163b3564da81b79b91829e9aec47126b78/TagAlgos/interface/WorkspaceCombiner.h

#ifndef WORKSPACECOMBINER
#define WORKSPACECOMBINER

// ROOT includes
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TAxis.h"
#include "TString.h"
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TKey.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TDirectoryFile.h"
#include "TChain.h"
#include "TList.h"
#include "TObject.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"

// RooFit includes
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooAddPdf.h"
#include "RooGlobalFunc.h"
#include "RooCmdArg.h"

// RooStats includes
#include "RooWorkspace.h"

// standard includes
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


using namespace std;
using namespace RooFit;

class WorkspaceCombiner
{

public:

    WorkspaceCombiner();

    ~WorkspaceCombiner();

    void Init( string outputFileName_, vector<string> inputfiles_ );

    TDirectoryFile *GetFirstFile();

    void GetWorkspaces( TDirectoryFile *file );

    TDirectoryFile *MergeTreesAndHistograms();

    void GetTreesAndHistograms( TDirectoryFile *file );

    void MergeWorkspaces();

    void Save( bool doTreesAndHistograms );


private :

    vector<string> inputFileNames;

    string outputFileName;

    string outputAux; //store temporarely histograms and trees = output of standard hadd command

    vector<string> workspaceNames;

    //    vector<vector<RooDataSet *> > data; //[workspace][dataset(cat)]
    vector<std::unordered_map<std::string, RooDataSet *> > data; //[workspace][dataset(cat)]
    
    //    vector<vector<RooDataHist *> > dataH; //[workspace][dataset(cat)]
    vector<std::unordered_map<std::string, RooDataHist *> > dataH; //[workspace][dataset(cat)]
    
    //    vector<vector<RooRealVar *> > vars; //[workspace][dataset(cat)]
    vector<std::unordered_map<std::string, RooRealVar *> > vars; //[workspace][dataset(cat)]

    vector<TTree *> trees;

    vector<TH1 *> histos;

    vector<string> workspacePaths;

    vector<string> treePaths;

    vector<string> histoPaths;

};


#endif



// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
