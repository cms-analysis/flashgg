#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"

#include "TFile.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooHist.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooFitResult.h"
#include "RooMinuit.h"
#include "RooMinimizer.h"
#include "RooMsgService.h"
#include "RooDataHist.h"
#include "RooExtendPdf.h"
#include "TRandom3.h"
#include "TLatex.h"
#include "TMacro.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TArrow.h"
#include "TKey.h"
#include "RooCategory.h"

#include <Math/PdfFuncMathCore.h>
#include <Math/ProbFunc.h>
#include <iomanip>
#include "boost/program_options.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/predicate.hpp"


void test_Entries(){
TFile *inFile = TFile::Open("./output_numEvent500.root");
RooDataSet *dataFull;
RooWorkspace *inWS;
int Entries;
inWS = (RooWorkspace*)inFile->Get("tagsDumper/cms_hgg_13TeV");
dataFull = (RooDataSet*)inWS->data(Form("GluGluToHHTo_WWgg_lnulnugg_nodeSM_13TeV_HHWWggTag_3"));
Entries=dataFull->numEntries();
cout << Entries << endl;
RooRealVar *mass = (RooRealVar*)inWS->var("CMS_hgg_mass");
std:: cout << "[INFO] Got mass from ws " << mass << std::endl;

}




