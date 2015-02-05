#include <iostream>
#include <string>
#include <vector>

#include "tmvaglob.C"

#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TTree.h"
#include "TText.h"
#include "TLegend.h"
#include "TROOT.h"

using namespace std;

std::vector<double> optimizedReverseBinning(TH1F *hb,int nTargetBins,bool revise_target, bool use_n_target){
	// Return a set of bins which are "smoother" 

	if (revise_target) {
		if (use_n_target){
		   std::cerr << "WARNING -- RooContainer::OptimizedBinning -- Can't use number of Entries as target in revised binning algo " << std::endl; 
		   use_n_target = false;  // geometric algo always use revised number of bins, not number of entries
		
		}
	}

	int nBins = hb->GetNbinsX();
	std::vector<double> binEdges;

	double targetNumbers;
	if (use_n_target) targetNumbers = nTargetBins; 
	else targetNumbers = hb->Integral()/nTargetBins;

	if (hb->Integral() < 2*targetNumbers){
		std::cout << "RooContainer::OptimizedBinning -- Not enough entries in histogram for target numbers calculated - " 
			  << targetNumbers 
			  << ", Returning current bin boundaries "  << std::endl;
		//for (int j=2;j<=nBins+1;j++) binEdges.push_back(hb->GetBinLowEdge(j));
		binEdges.push_back(hb->GetBinLowEdge(1));
		binEdges.push_back(hb->GetBinLowEdge(nBins+1));
		return binEdges;
	}
	binEdges.push_back(hb->GetBinLowEdge(nBins+1));

	std::cout << "RooContainer::optimizedBinning -- Performing Reverse Optimize Binning" <<std::endl;
	double sumBin = 0;
	int i=nBins;
	while (i>=1){
		if (revise_target) targetNumbers = hb->Integral(1,i)/nTargetBins;
		sumBin=hb->GetBinContent(i);
		double highEdge=hb->GetBinLowEdge(i);

		bool carryOn = sumBin <= targetNumbers;
		while ( carryOn){
			if (i>1){
			  sumBin+=hb->GetBinContent(i-1);
			  highEdge = hb->GetBinLowEdge(i-1);
			  carryOn =(sumBin <targetNumbers && i>=1);
			  i--;
			} else {
			  highEdge = hb->GetBinLowEdge(i);
			  carryOn=false;
			}
		}
	        binEdges.push_back(highEdge);
		i--;
	}
        if (sumBin < 10) binEdges.erase(binEdges.end()-2);
	reverse(binEdges.begin(),binEdges.end());
	return binEdges;

}

void histogramSmoothing(TH1F* h, int n){
   // Nothing too special, a function which will smooth a histogram but ignore the first and last
   // bins, useful for the "flat-binning" approach! 
   if (h->GetNbinsX()>3){
     int nbin = h->GetNbinsX();
     TH1F *h2 = new TH1F(Form("hn%s",h->GetName()),Form("hn%s",h->GetName()),nbin-2,0,1);
     for (int i=1;i<=nbin-2;i++){
           h2->SetBinContent(i,h->GetBinContent(i+1));
     }
     h2->Smooth(n);
     for (int i=2;i<=nbin-1;i++){
           h->SetBinContent(i,h2->GetBinContent(i-1));
     }

   }

   return;

   
}

TH1F* rebinBinnedDataset(std::string new_name,std::string name, TH1F *hb,std::vector<double> binEdges){

	double *arrBins = new double[binEdges.size()];
	int j=0;
	for (std::vector<double>::iterator it=binEdges.begin();it!=binEdges.end();it++){
		arrBins[j]=*it;
		j++;
		
	}
	//const char *h_name = (const char *) hb->GetName;
	//const char *title  = (const char *) hb->GetTitle;
	
	TH1F *hbnew =(TH1F*) hb->Rebin(binEdges.size()-1,hb->GetName(),arrBins);
	hbnew->SetName(Form("th1f_%s",new_name.c_str()));
	//cout << "title for new re-binned histogream - " << hb->GetTitle()<<endl; 
	hbnew->SetTitle(hb->GetTitle());

	// Just a quick test, mask the last "channel"
	//hbnew->SetBinContent(hbnew->GetNbinsX(),0);
	//cout << "DONT DO THIS IN MAIN PROGRAM ----- LINE 1563 rebin setting last bin to 0" <<endl;
	delete [] arrBins;
  return hbnew;	
}

std::vector<double> soverBOptimizedBinning(TH1F *hs,TH1F *hb,int nTargetBins,double penaltyScale){

	// Performs Optimized Binning based on a Signal and Background (S/B) distributions
	// First runs the optimizedBinning on background and rebins S and B clones, note, always performs 
	// revise_target=false,direction=-1 and use_n_entries=true
	// nTargetBins is used for the flat binning, decision to merge based on penaltyScale
	int ninitBins = hb->GetNbinsX();
	if (hs->Integral()==0 ||  hb->Integral()==0 || ninitBins < 2) {
		std::vector<double> binEdges;
		binEdges.push_back(hb->GetBinLowEdge(1));
		binEdges.push_back(hb->GetBinLowEdge(ninitBins+1));
		return binEdges;
	}

	std::vector<double> binEdges = optimizedReverseBinning(hb,nTargetBins,false,true);

	int j =0;
	double *arrBins = new double[binEdges.size()];
	for (std::vector<double>::iterator it=binEdges.begin();it!=binEdges.end();it++){
		//cout << *it << endl;
		arrBins[j]=*it;
		j++;	
	}
	// Create new rebinned histograms (only temporary)
	TH1F *hbnew =(TH1F*) hb->Rebin(binEdges.size()-1,"hbnew",arrBins);
	TH1F *hsnew =(TH1F*) hs->Rebin(binEdges.size()-1,"hsnew",arrBins);
	

	// Better smoothing which doesn't use the first and last bins	
	if (hsnew->Integral()!=0 && hbnew->Integral()!=0 && binEdges.size()-1 > 10){
		histogramSmoothing(hsnew,1000);
		histogramSmoothing(hbnew,1000);
		//hsnew->Smooth(1000);
		//hbnew->Smooth(1000);
        }
	// Do we really need the background histogram ?  we will be assuming that the first step is nentries per bin

	// Smooth signal new binned histograms, the size of smoothing should be ~1% of the total bins	
	//int nSmooth = (int) 0.01*hsnew->GetNbinsX();
	//hsnew->Smooth(nSmooth);

	delete [] arrBins;

	if (hbnew->Integral()==0 || hsnew->Integral()==0) return binEdges;
	std::vector<double> newbinEdges;
	newbinEdges.push_back(hbnew->GetBinLowEdge(1));
	int nNewBins = hbnew->GetNbinsX();
	int i=1;
	double maxSoB=0;
        for (int j=1;j<=hbnew->GetNbinsX();j++){
                double newMaximum = hsnew->GetBinContent(j)/hbnew->GetBinContent(j) ;
                if (newMaximum>maxSoB) maxSoB=newMaximum;
        }

	while (i<=nNewBins){

		int k = i+1;
		double highEdge=hbnew->GetBinLowEdge(i+1);
		double S = hsnew->GetBinContent(i);
		double B = hbnew->GetBinContent(i);
                double Stot =S;
                double Btot =B;

		if (B!=0){ 
		  bool carryOn=true;

		  while ( carryOn){

		  	double SoB = S/B;

			if (k<=nNewBins){

			  double S1 = hsnew->GetBinContent(k);
			  double B1 = hbnew->GetBinContent(k);
			  if (B1==0) {
				carryOn=true;
			      	highEdge = hbnew->GetBinLowEdge(k+1);
				Stot+=S1;
				k++;
			  }
			  else{

			    double SoB1 = S1/B1;
			    double scaler=penaltyScale;
                            double prob   = TMath::Prob((scaler*(Stot+S1))*(scaler*(Stot+S1))/(Btot+B1),1);
                            double prob1  = TMath::Prob(((scaler*Stot)*(scaler*Stot)/(Btot)) + ((scaler*S1)*(scaler*S1)/(B1)),2);
                  	    double importance = SoB/maxSoB;

//			      if (fabs(SoB-SoB1)/SoB < penaltyScale/importance ){
			      if ( prob<prob1 ){

			      highEdge = hbnew->GetBinLowEdge(k+1);
			      Stot+=S1;
			      Btot+=B1;
			      carryOn = true;
			      k++;
			    } else {carryOn=false;}
			 }

			} else {
			  highEdge = hbnew->GetBinLowEdge(k);
			  carryOn=false;
			}
		  }
		}
	        newbinEdges.push_back(highEdge);
		i=k;
	}

	// now we have new Bin edges to return to the 
	return newbinEdges;

}

void FillHist(TH1F* hist, string sorb, TTree* tree, string method){

  int SorB=-1;
  if (sorb=="Signal") SorB=0;
  else if (sorb=="Background") SorB=1;
  else {
    cout << "ERROR: must specify Signal or Background" << endl;
    exit(1);
  }
  if (SorB!=0 && SorB!=1){
    cout << "ERROR: " << endl;
    exit(1);
  }

  int classID;
  float BDTG;
  float weight;
  tree->SetBranchAddress("weight",&weight);
  tree->SetBranchAddress("classID",&classID);
  tree->SetBranchAddress(method.c_str(),&BDTG);

  for (int i=0; i<tree->GetEntries(); i++){
    tree->GetEntry(i);
    if (classID==SorB){
      hist->Fill(BDTG,weight);
    }
  }

}

TH1F* neatBin(TH1F* hist){

  string name(hist->GetName());
  TH1F *newHist = new TH1F(name.c_str(),name.c_str(),hist->GetNbinsX(),0,hist->GetNbinsX());
  for (int bin=1; bin<=hist->GetNbinsX(); bin++){
    newHist->SetBinContent(bin,hist->GetBinContent(bin));
  }
  return newHist;
}

void Plot(TH1F* sig, TH1F* bgd, TH1F* sigOv, TH1F* bgdOv,double ksSig, double ksBgd, string name){
 
  gStyle->SetOptStat(0);
  TMVAGlob::SetSignalAndBackgroundStyle( sig, bgd );
  TCanvas *c = new TCanvas();
  Int_t col = sig->GetLineColor();
  sigOv->SetMarkerColor( col );
  sigOv->SetMarkerSize( 0.7 );
  sigOv->SetMarkerStyle( 20 );
  sigOv->SetLineWidth( 1 );
  sigOv->SetLineColor( col );

  col = bgd->GetLineColor();
  bgdOv->SetMarkerColor( col );
  bgdOv->SetMarkerSize( 0.7 );
  bgdOv->SetMarkerStyle( 20 );
  bgdOv->SetLineWidth( 1 );
  bgdOv->SetLineColor( col );

  sig->GetYaxis()->SetRangeUser(0.,TMath::Max(sig->GetMaximum(),bgd->GetMaximum())*1.5);
  sig->Draw("hist");
  bgd->Draw("samehist");
  sigOv->Draw("e1same");
  bgdOv->Draw("e1same");
  
  TString probatext_ = Form( "Kolmogorov-Smirnov test: signal (background) probability = %5.3g (%5.3g)", ksSig, ksBgd );
  TText* tt_ = new TText( 0.12, 0.84, probatext_ );
  tt_->SetNDC(); tt_->SetTextSize( 0.032 ); 
  tt_->Draw("same");

  TLegend *legend= new TLegend( 0.6,0.5,0.9,0.8 ); 
  legend->SetFillStyle( 0 );
  legend->AddEntry(sig,TString("Signal") + "(test sample)", "F");
  legend->AddEntry(bgd,TString("Background") + "(test sample)", "F");
  legend->SetBorderSize(1);
  legend->AddEntry(sigOv,"Signal (training sample)","lep");
  legend->AddEntry(bgdOv,"Background (training sample)","lep");
  legend->Draw("same");

  c->Print(Form("plots/%s.pdf",name.c_str()),"pdf");

  delete c;
}

void Rebin(string infile, string methodName="BDTG"){
 
  gROOT->SetBatch();
  system("mkdir plots");
  TFile *inFile = TFile::Open(infile.c_str());

  TTree *testTree = (TTree*)inFile->FindObjectAny("TestTree");
  TTree *trainTree = (TTree*)inFile->FindObjectAny("TrainTree");

  TH1F *sig = new TH1F(Form("sig%s",methodName.c_str()),Form("sig%s",methodName.c_str()),5000,-1.,1.);
  TH1F *bgd = new TH1F(Form("bgd%s",methodName.c_str()),Form("bgd%s",methodName.c_str()),5000,-1.,1.);
  TH1F *sigOv = new TH1F(Form("sigOv%s",methodName.c_str()),Form("sigOv%s",methodName.c_str()),5000,-1.,1.);
  TH1F *bgdOv = new TH1F(Form("bgdOv%s",methodName.c_str()),Form("bgdOv%s",methodName.c_str()),5000,-1.,1.);

  FillHist(sig,"Signal",testTree,methodName);
  FillHist(bgd,"Background",testTree,methodName);
  FillHist(sigOv,"Signal",trainTree,methodName);
  FillHist(bgdOv,"Background",trainTree,methodName);

  //std::vector<double> binEdges = soverBOptimizedBinning(sigOv,bgdOv,20,50);
  std::vector<double> binEdges;
  binEdges.push_back(-1.);
  binEdges.push_back(0.2016);
  binEdges.push_back(0.4352);
  binEdges.push_back(0.6068);
  binEdges.push_back(0.7056);
  binEdges.push_back(0.762);
  binEdges.push_back(0.8152);
  binEdges.push_back(1);
  TH1F *sigReb = neatBin(rebinBinnedDataset(Form("sig_reb_%s",methodName.c_str())," ",sig,binEdges));
  TH1F *bgdReb = neatBin(rebinBinnedDataset(Form("bgd_reb_%s",methodName.c_str())," ",bgd,binEdges));
  TH1F *sigOvReb = neatBin(rebinBinnedDataset(Form("sig_train_reb_%s",methodName.c_str())," ",sigOv,binEdges));
  TH1F *bgdOvReb = neatBin(rebinBinnedDataset(Form("bgd_train_reb_%s",methodName.c_str())," ",bgdOv,binEdges));

  //Normalise to 1
  sig->Sumw2();
  bgd->Sumw2();
  sigOv->Sumw2();
  bgdOv->Sumw2();
  sig->Scale(1./sig->Integral());
  bgd->Scale(1./bgd->Integral());
  sigOv->Scale(1./sigOv->Integral());
  bgdOv->Scale(1./bgdOv->Integral());

  double theKSSig = sig->KolmogorovTest( sigOv );
  double theKSBkg = bgd->KolmogorovTest( bgdOv );

  Plot(sig,bgd,sigOv,bgdOv,theKSSig,theKSBkg,"FineBin");
  
  sig->Rebin(100);
  bgd->Rebin(100);
  sigOv->Rebin(100);
  bgdOv->Rebin(100);
  
  double redoKSSig = sig->KolmogorovTest( sigOv );
  double redoKSBkg = bgd->KolmogorovTest( bgdOv );
  
  Plot(sig,bgd,sigOv,bgdOv,redoKSSig,redoKSBkg,"NeatBin");
  Plot(sigReb,bgdReb,sigOvReb,bgdOvReb,redoKSSig,redoKSBkg,"ReBin"); 
  

  inFile->Close();
}
