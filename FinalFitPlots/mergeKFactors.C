// Macro to make single kFactors file for Globe
// For us we use central = 0, down = 1, up = 6

// Assuming mapping is (eg for mH - 120):
// kfact_mh120_ren120_fac120 -> kfact120_0     (the central re-weighting)
// kfact_mh120_ren60_fac60   -> kfact120_1     (downwards fluccuation)
// kfact_mh120_ren240_fac240 -> kfact120_6     (upwards fluccuation)

#include <iostream>
#include "TH1F.h"
#include "TFile.h"
#include <string>

using namespace std;

void removeNegative(TH1F *h){

	for (int i=1;i<=h->GetNbinsX();i++){
		if (h->GetBinContent(i)<0) h->SetBinContent(i,0);
	}
}

void mergeKFactors(){

  TFile *newFile = new TFile("KFactors_AllScales.root","RECREATE");


  const int nMassPoints=10;
  int massPoints[nMassPoints] =  {105,110,115,120,125,130,135,140,145,150};

  std::string kFactorsFolder = "/afs/cern.ch/user/j/jueugste/public/html/kfactors/";

  for (int mass_i=0;mass_i<nMassPoints;mass_i++){

	int mass = massPoints[mass_i];
	int halfmass = mass/2;
	int doublemass = mass*2;
	std::cout << "Getting KFactors for mass - " << mass << endl;

	TFile *kfacFile = TFile::Open(Form("%s/kfactors_%d/Kfactors_%d_AllScales.root",kFactorsFolder.c_str(),mass,mass));

	TH1F *centralHist = (TH1F*) kfacFile->Get(Form("kfactors/kfact_mh%d_ren%d_fac%d",mass,mass,mass));
	
	TH1F *upHist = (TH1F*) kfacFile->Get(Form("kfactors/kfact_mh%d_ren%d_fac%d",mass,doublemass,doublemass));
	TH1F *downHist = (TH1F*) kfacFile->Get(Form("kfactors/kfact_mh%d_ren%d_fac%d",mass,halfmass,halfmass));

	TH1F *cen  = (TH1F*)centralHist->Clone();
	cen->SetName(Form("kfact%d_0",mass));
	TH1F *up   = (TH1F*)upHist->Clone();
	up->SetName(Form("kfact%d_6",mass));
	TH1F *down = (TH1F*)downHist->Clone();
	down->SetName(Form("kfact%d_1",mass));

	removeNegative(cen);
	removeNegative(up);
	removeNegative(down);

  	newFile->cd();
	cen->Write();
	up->Write();
	down->Write();	

  }

  newFile->Close();
}
