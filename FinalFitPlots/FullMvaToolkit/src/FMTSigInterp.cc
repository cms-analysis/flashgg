#include <fstream>
#include <cmath>
#include <cassert>

#include "TROOT.h"
#include "TStyle.h"
#include "TError.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPaveText.h"

#include "boost/lexical_cast.hpp"

#include "../interface/FMTSigInterp.h"

using namespace std;
FMTSigInterp::FMTSigInterp(string filename):FMTBase()
{
  tFile = new TFile(filename.c_str(),"UPDATE");
}

FMTSigInterp::FMTSigInterp(string filename, double intLumi, bool is2011, bool diagnose, bool doSyst, int mHMinimum, int mHMaximum, double mHStep, double massMin, double massMax, int nDataBins, double signalRegionWidth, double sidebandWidth, int numberOfSidebands, int numberOfSidebandsForAlgos, int numberOfSidebandGaps, double massSidebandMin, double massSidebandMax, int nIncCategories, bool includeVBF, int nVBFCategories, bool includeLEP, int nLEPCategories, vector<string> systematics, bool rederiveOptimizedBinEdges, vector<map<int, vector<double> > > AllBinEdges, bool blind, bool verbose):
  
	FMTBase(intLumi, is2011, mHMinimum, mHMaximum, mHStep, massMin, massMax, nDataBins, signalRegionWidth, sidebandWidth, numberOfSidebands, numberOfSidebandsForAlgos, numberOfSidebandGaps, massSidebandMin, massSidebandMax, nIncCategories, includeVBF, nVBFCategories, includeLEP, nLEPCategories, systematics, rederiveOptimizedBinEdges, AllBinEdges, verbose)
 // diagnose_(diagnose),
 // blind_(blind)
{
  tFile = new TFile(filename.c_str(),"UPDATE");

}

FMTSigInterp::~FMTSigInterp(){
  delete normalizer;
  tFile->Close();
  delete tFile;
}

TH1F* FMTSigInterp::Interpolate(double massLow, TH1F* low, double massHigh, TH1F* high, double massInt, bool smoothEff,std::string prod){

  if (low->GetNbinsX()!=high->GetNbinsX()) std::cout << "Cannot interpolate differently binned histograms" << std::endl;
  assert(low->GetNbinsX()==high->GetNbinsX());

  // Scale histograms for interpolation
  double lowScale = (normalizer->GetXsection(massLow,low->GetName())*normalizer->GetBR(massLow));
  double highScale = (normalizer->GetXsection(massHigh,high->GetName())*normalizer->GetBR(massHigh));
  low->Scale(1./lowScale);
  high->Scale(1./highScale);
  
  TH1F *interp = (TH1F*)low->Clone();
  for (int i=0; i<interp->GetNbinsX(); i++){
    double OutLow = low->GetBinContent(i+1);
    double OutHigh = high->GetBinContent(i+1);
    double OutInt = (OutHigh*(massInt-massLow)-OutLow*(massInt-massHigh))/(massHigh-massLow);
    interp->SetBinContent(i+1,OutInt);
  }


  // Scale back interpolation histograms
  low->Scale(lowScale );
  high->Scale(highScale);

  std::string name = low->GetName();
  std::string strLow = Form("%3.1f",massLow);
  std::string strInt = Form("%3.1f",massInt);
  int ind = name.rfind(strLow);
  name.replace(ind-6,11,strInt);
  interp->SetName(name.c_str());

  // Smooth eff*acc for nominal signal
  double XSnorm = normalizer->GetXsection(massInt,interp->GetName())*normalizer->GetBR(massInt);
  if (smoothEff){
      double EffNorm = (efficiencyGraphs[prod])->GetFunction("pol4")->Eval(massInt);
      interp->Scale(EffNorm*XSnorm/interp->Integral());
  } else {
      interp->Scale(XSnorm);
  }

  return interp;
}

void FMTSigInterp::makeEfficiencyGraphs(){

  vector<int> mcMasses = getMCMasses();
  vector<string> productionTypes = getProdTypes();

  for (vector<string>::iterator prod=productionTypes.begin(); prod!=productionTypes.end(); prod++){
    TGraph *gr = new TGraph();
    gr->SetName(prod->c_str());
    int pt = 0;
    for (vector<int>::iterator mcMass=mcMasses.begin(); mcMass!=mcMasses.end(); mcMass++){
    	  double thismH = (double)*mcMass;
          TH1F *sig = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%3.1f_%3.1f",prod->c_str(),thismH,thismH));
	  double val = normalizer->GetXsection(thismH,prod->c_str())*normalizer->GetBR(thismH);
	  gr->SetPoint(pt,thismH,sig->Integral()/val);
	  pt++;
    }
    gr->Fit("pol4","F","");
    efficiencyGraphs.insert(std::pair<std::string, TGraph* >(*prod,(TGraph*)gr->Clone()));
  }  
}

void FMTSigInterp::runInterpolation(){
  
  normalizer = new Normalization_8TeV();
  if ( is2011_ ) normalizer->Init(7);
  else  normalizer->Init(8);

  gROOT->SetBatch();
  gStyle->SetOptStat(0);
  gErrorIgnoreLevel = kWarning;
  ofstream diagFile("plots/SigInterpolationDiagnostics.txt");
  
  vector<int> mcMasses = getMCMasses();
  vector<string> productionTypes = getProdTypes();

  // make Smooth efficiency Graphs

  makeEfficiencyGraphs();
  
  
  // now do interpolation
  for (vector<int>::iterator mcMass=mcMasses.begin(); mcMass!=mcMasses.end(); mcMass++){
    vector<double> mhMasses = getMHMasses(*mcMass);
    for (vector<double>::iterator mh = mhMasses.begin(); mh!=mhMasses.end(); mh++){
      if (fabs(*mh-boost::lexical_cast<double>(*mcMass))<0.01) {
        if (verbose_) cout << "Already know this mass " << *mh << endl;
        TH1F *allSig=0;
        for (vector<string>::iterator prod=productionTypes.begin(); prod!=productionTypes.end(); prod++){
          TH1F *sig = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%3.1f_%3.1f",prod->c_str(),*mh,*mh));
          sig->SetName(Form("th1f_sig_grad_%s_%3.1f",prod->c_str(),*mh));
          //write(tFile,sig);
	  // Make its Normalization and efficiency smoother!
      	  double EffNorm = (efficiencyGraphs[*prod])->GetFunction("pol4")->Eval(*mh);
	  double XSnorm  = normalizer->GetXsection(*mh,sig->GetName())*normalizer->GetBR(*mh);
      	  sig->Scale(EffNorm*XSnorm/sig->Integral());
	  
          if (verbose_) checkHisto(sig);
          gDirectory->Cd(Form("%s:/",tFile->GetName()));
          sig->Write();
          if (*prod=="ggh") allSig = (TH1F*)sig->Clone();
          else allSig->Add(sig);
          vector<string> theSystematics = getsystematics();
          if (verbose_) cout << "["; printVec(theSystematics); cout << "]" << endl;
	  if ( fabs(*mh-125.0)>0.01 ) continue;
          for (vector<string>::iterator syst=theSystematics.begin(); syst!=theSystematics.end(); syst++){
            if (verbose_) cout << Form("th1f_sig_grad_%s_%3.1f_%3.1f_%sUp01_sigma",prod->c_str(),*mh,*mh,syst->c_str()) << endl;
            if (verbose_) cout << Form("th1f_sig_grad_%s_%3.1f_%3.1f_%sDown01_sigma",prod->c_str(),*mh,*mh,syst->c_str()) << endl;
            TH1F* up = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%3.1f_%3.1f_%sUp01_sigma",prod->c_str(),*mh,*mh,syst->c_str()));
            TH1F* down = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%3.1f_%3.1f_%sDown01_sigma",prod->c_str(),*mh,*mh,syst->c_str()));
            up->SetName(Form("th1f_sig_grad_%s_%3.1f_%sUp01_sigma",prod->c_str(),*mh,syst->c_str()));
            down->SetName(Form("th1f_sig_grad_%s_%3.1f_%sDown01_sigma",prod->c_str(),*mh,syst->c_str()));
            //write(tFile,up);
            //write(tFile,down);
            gDirectory->Cd(Form("%s:/",tFile->GetName()));
            up->Write();
            down->Write();
          }
        }
      }
      else{
        int nearest = (getInterpMasses(*mh)).first;
        int nextNear = (getInterpMasses(*mh)).second;
        int binningMass, lowInterpMass, highInterpMass;
        if (nearest-nextNear>0) {
          binningMass=nearest;
          lowInterpMass=nextNear;
          highInterpMass=nearest;
          if (verbose_) cout << "Mass: " << *mh << " binMass: " << nearest << " (" << nextNear << "," << nearest << ")" << endl;
        }
        else {
          binningMass=nearest;
          lowInterpMass=nearest;
          highInterpMass=nextNear;
          if (verbose_) cout << "Mass: " << *mh << " binMass: " << nearest << " (" << nearest << "," << nextNear << ")" << endl;
        }
        // first do central signal histograms
        TH1F *allSig = 0;
        for (vector<string>::iterator prod=productionTypes.begin(); prod!=productionTypes.end(); prod++){
          TH1F *lowInterp = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%d.0_%d.0",prod->c_str(),binningMass,lowInterpMass));
          TH1F *highInterp = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%d.0_%d.0",prod->c_str(),binningMass,highInterpMass));
          if (verbose_) checkHisto(lowInterp);
          if (verbose_) checkHisto(highInterp);
          TH1F *interpolated = Interpolate(double(lowInterpMass), lowInterp, double(highInterpMass), highInterp, *mh,true,*prod);
          if (verbose_) checkHisto(interpolated);
          //write(tFile,interpolated);
          gDirectory->Cd(Form("%s:/",tFile->GetName()));
          interpolated->Write();
          if (*prod=="ggh") allSig = (TH1F*)interpolated->Clone();
          else allSig->Add(interpolated);
        }
        
        // then do signal systematic histograms
        vector<string> theSystematics = getsystematics();
        vector<string> UorD;
        UorD.push_back("Up");
        UorD.push_back("Down");
        for (vector<string>::iterator syst=theSystematics.begin(); syst!=theSystematics.end(); syst++){
          for (vector<string>::iterator ud=UorD.begin(); ud!=UorD.end(); ud++){
            for (vector<string>::iterator prod=productionTypes.begin(); prod!=productionTypes.end(); prod++){
              TH1F *lowInterp = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%d.0_%d.0_%s%s01_sigma",prod->c_str(),binningMass,lowInterpMass,syst->c_str(),ud->c_str()));
              TH1F *highInterp = (TH1F*)tFile->Get(Form("th1f_sig_grad_%s_%d.0_%d.0_%s%s01_sigma",prod->c_str(),binningMass,highInterpMass,syst->c_str(),ud->c_str()));
              if (verbose_) checkHisto(lowInterp);
              if (verbose_) checkHisto(highInterp);
              TH1F *interpolated = Interpolate(double(lowInterpMass), lowInterp, double(highInterpMass), highInterp, *mh);
              if (verbose_) checkHisto(interpolated);
              //write(tFile,interpolated);
              gDirectory->Cd(Form("%s:/",tFile->GetName()));
              interpolated->Write();
            }
          }
        }
      }
    }
  }
}
