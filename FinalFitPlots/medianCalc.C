#include "TTree.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include <algorithm>

void medianCalc(std::string name,TTree *tree,double *lim,double *up68,double *dn68
	       ,double *up95,double *dn95){

     TH1F hist_r(Form("hist_r_%s",name.c_str()),";r;",100,0.,30.);
     double r;

     int nLimits = tree->GetEntriesFast();
     tree->SetBranchAddress("limit",&r);

     std::vector<double> limitHistory;
     for (int i=0;i<nLimits;i++){
        tree->GetEntry(i);
        limitHistory.push_back(r);
	hist_r.Fill(r);
     }

     sort(limitHistory.begin(), limitHistory.end());

     if (nLimits > 0) {
     double medianLimit = (nLimits % 2 == 0 ? 0.5*(limitHistory[nLimits/2-1]
			+limitHistory[nLimits/2]) : limitHistory[nLimits/2]);
        double hi68 = limitHistory[min<int>(nLimits-1,  ceil(0.84  * nLimits))];
        double lo68 = limitHistory[min<int>(nLimits-1, floor(0.16  * nLimits))];
        double hi95 = limitHistory[min<int>(nLimits-1,  ceil(0.975 * nLimits))];
        double lo95 = limitHistory[min<int>(nLimits-1, floor(0.025 * nLimits))];

	*lim  = medianLimit;
	*up68 = hi68; 
	*dn68 = lo68; 
	*up95 = hi95; 
	*dn95 = lo95; 
     } else {
	*lim  = 0;
	*up68 = 0; 
	*dn68 = 0; 
	*up95 = 0; 
	*dn95 = 0; 
     }
	
     gROOT->SetStyle("Plain");
     TCanvas *can = new TCanvas();
     can->SetLogy();
     hist_r.SetFillStyle(1001);
     hist_r.SetFillColor(kBlue);
     hist_r.Draw();
     can->SaveAs(Form("%s.pdf",name.c_str()));
     can->SaveAs(Form("%s.gif",name.c_str()));

}


double FrequentistLimits(std::string filename) {

  double r=-9999;
  
  TFile *LimitFile = TFile::Open(filename.c_str());
  TTree *LimitTree = (TTree*) LimitFile->Get("limit");
  LimitTree->SetBranchAddress("limit",&r);
  LimitTree->GetEntry(0);
  return r;
  
}

