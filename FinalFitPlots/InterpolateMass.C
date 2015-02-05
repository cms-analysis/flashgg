// Original Author - Doug Berry

#include "TH1F.h"
#include "TList.h"
#include "TString.h"
#include "TFile.h"
#include "TF1.h"
#include "TROOT.h"
#include "TMatrixD.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "RooMsgService.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include "th1fmorph.C"
#include "Normalization_8TeV.h"
//#include "Normalization_ff.C"

using namespace std;
using namespace RooFit;

bool MAKE_PLOTS=false;

TCanvas *can = new TCanvas();
Normalization_8TeV *normalizer;
double getInterp(double C, double X1,double X2,double X3,double X4,double Y1,double Y2,double Y3,double Y4){


	TF1 func("f1","[0]*x*x*x+[1]*x*x+[2]*x+[3]",X1,X4);

	double entries[16];
	entries[0]=X1*X1*X1; entries[1]=X1*X1; entries[2]=X1; entries[3]=1;
	entries[4]=X2*X2*X2; entries[5]=X2*X2; entries[6]=X2; entries[7]=1;
	entries[8]=X3*X3*X3; entries[9]=X3*X3; entries[10]=X3; entries[11]=1;
	entries[12]=X4*X4*X4; entries[13]=X4*X4; entries[14]=X4; entries[15]=1;
	
	//create the Matrix;
	TMatrixD M(4,4);
	M.SetMatrixArray(entries);
	M.Invert();

	double a = M(0,0)*Y1+M(0,1)*Y2+M(0,2)*Y3+M(0,3)*Y4;
	double b = M(1,0)*Y1+M(1,1)*Y2+M(1,2)*Y3+M(1,3)*Y4;
	double c = M(2,0)*Y1+M(2,1)*Y2+M(2,2)*Y3+M(2,3)*Y4;
	double d = M(3,0)*Y1+M(3,1)*Y2+M(3,2)*Y3+M(3,3)*Y4;

	func.SetParameter(0,a);
	func.SetParameter(1,b);
	func.SetParameter(2,c);
	func.SetParameter(3,d);
	return func.Eval(C);
}

void cdf2pdf(TH1F *h){

        TH1F *pdf = (TH1F*) h->Clone();

        for (int b=1;b<=pdf->GetNbinsX();b++){
		double y1 = pdf->GetBinContent(b-1);
		double y2 = pdf->GetBinContent(b);
                if (y2>y1) h->SetBinContent(b,y2-y1) ;
		else h->SetBinContent(b,0);

        }

	//return pdf;
}


void pdf2cdf(TH1F *hb){

        // Make a CDF for this guy ..... 

        TH1F *cdf = (TH1F*) hb->Clone();

        for (int b=1;b<=cdf->GetNbinsX();b++){

                hb->SetBinContent(b,cdf->Integral(1,b));
        }

      //  return cdf;
}

void SmoothMe(TH1F *hist){

        int nEntriesPB = hist->GetEntries()/(hist->FindBin(hist->GetMean()+hist->GetRMS())-hist->FindBin(hist->GetMean()-hist->GetRMS()));

	int nBins = hist->GetNbinsX();
	// Make cdf histogram 	
	pdf2cdf(hist);

	if (nEntriesPB>100) {

  	  for (int b=1;b<=nBins;b++){
	
		if (b<3 || b>nBins-2) continue;
		else{

			double x1 = hist->GetBinCenter(b-2);
			double x2 = hist->GetBinCenter(b-1);
			double x3 = hist->GetBinCenter(b+1);
			double x4 = hist->GetBinCenter(b+2);

			double y1 = hist->GetBinContent(b-2);
			double y2 = hist->GetBinContent(b-1);
			double y3 = hist->GetBinContent(b+1);
			double y4 = hist->GetBinContent(b+2);

			hist->SetBinContent(b,getInterp(hist->GetBinCenter(b),x1,x2,x3,x4,y1,y2,y3,y4));
		
		}
	  }
	} else {
		hist->Smooth(20);
	}

	cdf2pdf(hist);
	
}


void HistogramSmooth(TH1F* hist){
	static std::list<const char*> existingPlots;

	TH1F * newhist = (TH1F*)hist->Clone();
	double integral = hist->Integral();
	int nBins = hist->GetNbinsX();

	SmoothMe(hist);

	hist->Scale(integral/hist->Integral());

	std::list<const char*>::iterator findIter = std::find(existingPlots.begin(), existingPlots.end(), hist->GetName());

	if (MAKE_PLOTS && findIter==existingPlots.end()){
		newhist->GetXaxis()->SetTitle("m_{#gamma#gamma}");
		newhist->SetMarkerStyle(20);
		newhist->SetMarkerColor(1);
		newhist->SetLineColor(1);
		newhist->SetMarkerSize(0.5);
		newhist->SetTitle(hist->GetName());
		newhist->Draw("P");
		hist->SetLineColor(4);
		hist->SetLineWidth(2);
		hist->Draw("histsame");
		std::cout << "Smoothed -- " << hist->GetName() <<std::endl;
		can->SaveAs("histogramsmoothcheck.pdf");
		existingPlots.push_back(hist->GetName());
	}
}

template <class type> string makestring(type value) {
  stringstream sstr;
  sstr << value;
  return sstr.str();
}

void dofit(double fitmass, vector <TString> InterpolationList, TFile* SourceFile, TFile* OutputFile, RooWorkspace* WorkSpace, int doSmoothing=0,int noverwritemasses=0, double *overwritemasses=NULL, int debug=1) {


  static map<const char*, TH1F*> existingHists;

  if (floor(fitmass)-fitmass<0.0000001 && floor(fitmass)-fitmass>0) fitmass=floor(fitmass);
  if (fitmass-ceil(fitmass)>-0.0000001 && fitmass-ceil(fitmass)<0) fitmass=ceil(fitmass);
  
  if (fitmass>150 || fitmass<110) {
    cout << "Warning!!!!!!!!!!! You must have an input mass between 110 and 150 GeV!" << endl << "Skipping !!!!" << endl;
    return;
  }

  bool OverWriteMass = false;
  if (doSmoothing) OverWriteMass = true;
  if (overwritemasses!=NULL) {
    for (int i=0; i<noverwritemasses; i++) {
      if (fabs(overwritemasses[i]-fitmass)<0.0000001) OverWriteMass=true;
    }
  }
  
  double Masses[] = {105.0, 110.0, 115.0, 120.0, 125.0, 130.0, 135.0, 140.0, 145.0, 150.0}; //Mass points used for interpolation
  size_t NumMasses = sizeof(Masses)/sizeof(Masses[0]);
  double lowerbound = 0;
  double upperbound = 0;
  for (size_t i=0; i<NumMasses; i++) {
    if (fitmass>Masses[i] && fitmass<Masses[i+1]) {
      lowerbound = Masses[i];
      upperbound = Masses[i+1];
    } else if (fitmass==Masses[0] || fitmass==Masses[NumMasses-1]) {
      lowerbound = Masses[i];
      upperbound = Masses[i];
    } else if (fitmass==Masses[i]) {
      lowerbound = Masses[i-1];
      upperbound = Masses[i+1];
    }
  }

  TString MassString = makestring(fitmass);
  //MassString.ReplaceAll(".","_");
  TString LowerBoundString = makestring(lowerbound);
  LowerBoundString.ReplaceAll(".0","");
  TString UpperBoundString = makestring(upperbound);
  UpperBoundString.ReplaceAll(".0","");
  RooRealVar RooRealMass = *(WorkSpace->var("CMS_hgg_mass"));
  
  cout << "Calculating mass point at " << fitmass << endl;
  
  for (unsigned int k=0; k < InterpolationList.size(); k++) {

    TString LowerHistName = InterpolationList[k];
    LowerHistName.ReplaceAll("110",LowerBoundString);
    TString UpperHistName = InterpolationList[k];
    UpperHistName.ReplaceAll("110",UpperBoundString);
    TString HistName = InterpolationList[k];
    HistName.ReplaceAll("110",MassString);
    
    TString HistTitle(Form("Interpolated Mass at %fGeV",fitmass));

    TH1F* LowerHist;
    TH1F* UpperHist;
	
    std::map<const char*,TH1F*>::iterator itHL = existingHists.find(LowerHistName.Data());
    std::map<const char*,TH1F*>::iterator itHU = existingHists.find(UpperHistName.Data());

    if (itHL == existingHists.end()) {
	LowerHist =(TH1F*)SourceFile->Get(LowerHistName.Data());
    	if (doSmoothing) HistogramSmooth(LowerHist);
	existingHists[LowerHistName.Data()]=LowerHist;
	
    } else {
	LowerHist =(*itHL).second;
    }

    if (itHU == existingHists.end()) {
	UpperHist =(TH1F*)SourceFile->Get(UpperHistName.Data());
    	if (doSmoothing)HistogramSmooth(UpperHist);
	existingHists[UpperHistName.Data()]=UpperHist;
    } else {
	UpperHist =(*itHU).second;
    }


    if (debug>=1) cout << "Calculating mass point at " << fitmass << "GeV with histograms " << LowerHistName << " and " << UpperHistName << endl;

    double Normalization = normalizer->GetNorm(lowerbound, LowerHist, upperbound, UpperHist, fitmass);

    TH1F* MCHist= (TH1F*) SourceFile->Get(HistName.Data());

    if (MCHist!=NULL && doSmoothing){
    		std::map<const char*,TH1F*>::iterator itH = existingHists.find(HistName.Data());
		if (itH==existingHists.end()){
			HistogramSmooth(MCHist);
			existingHists[UpperHistName.Data()]=UpperHist;
		} else {
			MCHist = (*itH).second;
		}
    }
	
    if (debug>=1) cout << "About to run horizontal Interpolation" << endl;
    TH1F *InterpolatedHist;
    if (doSmoothing && MCHist!=NULL) InterpolatedHist = (TH1F*) MCHist->Clone();
    else  InterpolatedHist = (TH1F*) th1fmorph((Char_t*) HistName.Data(),(Char_t*) HistTitle.Data(),LowerHist,UpperHist,lowerbound,upperbound,fitmass,Normalization,0);

    if (MCHist!=NULL && !OverWriteMass) {
      TString ResidualHistName = HistName;
      ResidualHistName += "_Residual";
      TH1F* ResidualHist = (TH1F*) InterpolatedHist->Clone(ResidualHistName.Data());
      ResidualHist->Add(MCHist,-1);
      OutputFile->WriteTObject(ResidualHist);
      ResidualHistName.ReplaceAll("th1f_","");
      RooDataHist RooDataResidual(Form("roohist_%s",ResidualHistName.Data()),ResidualHistName.Data(),RooRealMass,ResidualHist);
      WorkSpace->import(RooDataResidual);
    }

    if (MCHist==NULL || OverWriteMass) {
      OutputFile->WriteTObject(InterpolatedHist,InterpolatedHist->GetName());
      HistName.ReplaceAll("th1f_","");
      RooDataHist RooDataInterpolated(Form("roohist_%s",HistName.Data()),HistName.Data(),RooRealMass,InterpolatedHist);
      if (WorkSpace->data(Form("roohist_%s",HistName.Data()))==NULL) WorkSpace->import(RooDataInterpolated);
      else {
        if (debug>=1) cout << "Before: " << ((RooDataHist*) WorkSpace->data(Form("roohist_%s",HistName.Data())))->sumEntries() << endl;
        ((RooDataHist*) WorkSpace->data(Form("roohist_%s",HistName.Data())))->reset();
        ((RooDataHist*) WorkSpace->data(Form("roohist_%s",HistName.Data())))->add(RooDataInterpolated);
        if (debug>=1) cout << "After: " << ((RooDataHist*) WorkSpace->data(Form("roohist_%s",HistName.Data())))->sumEntries() << endl;
      }
    } else {
      HistName += "_Interpolated";
      InterpolatedHist->SetName(HistName.Data());
      OutputFile->WriteTObject(InterpolatedHist);
      HistName.ReplaceAll("th1f_","");
      RooDataHist RooDataInterpolated(Form("roohist_%s",HistName.Data()),HistName.Data(),RooRealMass,InterpolatedHist);
      WorkSpace->import(RooDataInterpolated);
      
    }
  }
  cout << "Done " << fitmass << endl;

}

void InterpolateMass(double fitmass, TString SourceFileName="CMS-HGG.root", double overwritemass=0) {

  TString FileNameMod = Form("_%s.root",makestring(fitmass).c_str());
  TString FileName = SourceFileName;
  FileName.ReplaceAll(".root",FileNameMod);

  if (fitmass>=150 || fitmass<=110) {
    cout << "Warning!!!!!!!!!!! You must have an input mass between 110 and 150 GeV!" << endl << "Exiting Program!!!!" << endl;
    exit(1);
  }

  cout << "Opening file: " << SourceFileName << endl;
  TFile* SourceFile = new TFile(SourceFileName.Data());
  TList* HistList = SourceFile->GetListOfKeys();
  RooWorkspace * WorkSpace = (RooWorkspace*) SourceFile->Get("cms_hgg_workspace");
  TFile* OutputFile = new TFile(FileName.Data(),"RECREATE");
  OutputFile->cd();
  vector<TString> InterpolationList;
  for (Int_t j=0; j<HistList->GetSize(); ++j) {
    TString HistName(HistList->At(j)->GetName());
    if (HistName.Contains("110_")) InterpolationList.push_back(HistName);
    if (HistName.Contains("th1f")) {
      TH1F* temphist = (TH1F*) SourceFile->Get(HistName.Data());
      OutputFile->WriteTObject(temphist);
    }
    if (HistName.Contains("plot_data_pol_model")) {
      TCanvas* tempcan = (TCanvas*) SourceFile->Get(HistName.Data());
      OutputFile->WriteTObject(tempcan);
    }
  }
  dofit(fitmass, InterpolationList, SourceFile, OutputFile, WorkSpace, 0,1, &overwritemass);

  cout << "Writing data to disk..." << endl;
  WorkSpace->Write();
  OutputFile->Close();
  delete WorkSpace;
  delete SourceFile;
  delete OutputFile;
  cout << "Done!" << endl;
}

void InterpolateMassPoints(int nmasses, double * masses, TString SourceFileName="CMS-HGG",
                           TString FileName="", int doSmoothing=0, bool is2011=false, int noverwritemasses=0, double *overwritemasses=NULL) 
{
	RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

  normalizer = new Normalization_8TeV(is2011);
  
  if( FileName == "" ) { FileName = SourceFileName + "_interpolated.root"; }

  SourceFileName += ".root";
  TFile* SourceFile = new TFile(SourceFileName);
  TList* HistList = SourceFile->GetListOfKeys();
  RooWorkspace * WorkSpace = (RooWorkspace*) SourceFile->Get("cms_hgg_workspace");
  TFile* OutputFile = new TFile(FileName.Data(),"RECREATE");
  OutputFile->cd();
  vector<TString> InterpolationList;

  for (Int_t j=0; j<HistList->GetSize(); ++j) {

    TString HistName(HistList->At(j)->GetName());
    if (HistName.Contains("110_")) InterpolationList.push_back(HistName);
    if (HistName.Contains("th1f")) {
      TH1F* temphist = (TH1F*) SourceFile->Get(HistName.Data());
      TString temphistname = temphist->GetName();
      OutputFile->WriteTObject(temphist);
    }
    if (HistName.Contains("plot_data_pol_model")) {
      TCanvas* tempcan = (TCanvas*) SourceFile->Get(HistName.Data());
      OutputFile->WriteTObject(tempcan);
    }
    
  }
  
  if (MAKE_PLOTS && doSmoothing) can->SaveAs("histogramsmoothcheck.pdf[");
  for(int imass=0; imass<nmasses; ++imass) {
    dofit(masses[imass], InterpolationList, SourceFile, OutputFile, WorkSpace,doSmoothing, noverwritemasses, overwritemasses);
  }
  if (MAKE_PLOTS && doSmoothing) can->SaveAs("histogramsmoothcheck.pdf]");
  
  cout << "Writing data to disk..." << endl;
  WorkSpace->Write();
  OutputFile->Close();
  delete WorkSpace;
  delete SourceFile;
  delete OutputFile;
  cout << "Done!" << endl;

}

void InterpolateMassRange(double Min, double Max, double Step, TString SourceFileName="CMS-HGG", int doSmoothing=0, bool is2011=false,int noverwritemasses=0, double *overwritemasses=NULL) 
{
  gROOT->SetBatch(1);
  TString FileName = "";
  std::vector<double> masses;
  for (double fitmass=Min; fitmass<=Max; fitmass+=Step) {
    masses.push_back(fitmass);
  }

  if (MAKE_PLOTS && doSmoothing) can->SaveAs("histogramsmoothcheck.pdf[");

  InterpolateMassPoints(masses.size(), &masses[0], SourceFileName,FileName, doSmoothing,is2011,noverwritemasses, overwritemasses); 
  if (MAKE_PLOTS && doSmoothing) can->SaveAs("histogramsmoothcheck.pdf]");
  
}


#ifndef __CINT__

int main(int argc, char ** argv)
{
	InterpolateMassRange(atof(argv[1]),atof(argv[2]),atof(argv[3]));
}

#endif
