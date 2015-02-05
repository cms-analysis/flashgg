/// READ HERE ----
/// to run this script you need to load some things in root first:
/// gSystem->SetIncludePath("-I$ROOTSYS/include -I$ROOFITSYS/include -I$CMSSW_BASE/src");
/// .L $CMSSW_BASE/lib/$SCRAM_ARCH/libHiggsAnalysisCombinedLimit.so
/// .L ../libLoopAll.so
/// .L makeParametricSignalModelPlots.C+g


#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TArrow.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TStopwatch.h"
#include "TMath.h"
#include "TColor.h"
#include "TLine.h"
#include "TPad.h"
#include "TList.h"
#include "TObjString.h"
#include "TMacro.h"
#include "TKey.h"
#include "TString.h"

#include "RooStats/NumberCountingUtils.h"
#include "RooStats/RooStatsUtils.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooArgList.h"
#include "RooAbsReal.h"
#include "RooGaussian.h"
#include "RooMinimizer.h"
#include "RooExtendPdf.h"
#include "RooCategory.h"

#include "HiggsAnalysis/CombinedLimit/interface/RooMultiPdf.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooBernsteinFast.h"

using namespace std;
using namespace RooFit;

int binning_=70;

// get effective sigma from culmalative distribution function
pair<double,double> getEffSigma(RooRealVar *mass, RooAbsPdf *pdf, double wmin=110., double wmax=130., double step=0.002, double epsilon=1.e-4){

  RooAbsReal *cdf = pdf->createCdf(RooArgList(*mass));
  cout << "Computing effSigma...." << endl;
  TStopwatch sw;
  sw.Start();
  double point=wmin;
  vector<pair<double,double> > points;
  
  while (point <= wmax){
    mass->setVal(point);
    if (pdf->getVal() > epsilon){
      points.push_back(pair<double,double>(point,cdf->getVal())); 
    }
    point+=step;
  }
  double low = wmin;
  double high = wmax;
  double width = wmax-wmin;
  for (unsigned int i=0; i<points.size(); i++){
    for (unsigned int j=i; j<points.size(); j++){
      double wy = points[j].second - points[i].second;
      if (TMath::Abs(wy-0.683) < epsilon){
        double wx = points[j].first - points[i].first;
        if (wx < width){
          low = points[i].first;
          high = points[j].first;
          width=wx;
        }
      }
    }
  }
  sw.Stop();
  cout << "effSigma: [" << low << "-" << high << "] = " << width/2. << endl;
  cout << "\tTook: "; sw.Print();
  pair<double,double> result(low,high);
  return result;
}

// get effective sigma from finely binned histogram
pair<double,double> getEffSigBinned(RooRealVar *mass, RooAbsPdf *pdf, double wmin=110., double wmax=130.,int stepsize=1 ){
  
  int nbins = int((wmax-wmin)/0.001/double(stepsize));
  TH1F *h = new TH1F("h","h",nbins,wmin,wmax);
  pdf->fillHistogram(h,RooArgList(*mass));

  double narrowest=1000.;
  double bestInt;
  int lowbin;
  int highbin;

  double oneSigma=1.-TMath::Prob(1,1);
  
  TStopwatch sw;
  sw.Start();
  // get first guess
  cout << "Getting first guess info. stepsize (MeV) = " << stepsize*100 << endl;
  for (int i=0; i<h->GetNbinsX(); i+=(stepsize*100)){
    for (int j=i; j<h->GetNbinsX(); j+=(stepsize*100)){
      double integral = h->Integral(i,j)/h->Integral();
      if (integral<oneSigma) continue;
      double width = h->GetBinCenter(j)-h->GetBinCenter(i);
      if (width<narrowest){
        narrowest=width;
        bestInt=integral;
        lowbin=i;
        highbin=j;
        i++;
      }
    }
  }
  cout << "Took: "; sw.Print(); 
  // narrow down result
  int thisStepSize=32;
  cout << "Narrowing....." << endl;
  while (thisStepSize>stepsize) {
    cout << "\tstepsize (MeV) = " << thisStepSize << endl;
    for (int i=(lowbin-10*thisStepSize); i<(lowbin+10*thisStepSize); i+=thisStepSize){
      for (int j=(highbin-10*thisStepSize); j<(highbin+10*thisStepSize); j+=thisStepSize){
        double integral = h->Integral(i,j)/h->Integral();
        if (integral<oneSigma) continue;
        double width = h->GetBinCenter(j)-h->GetBinCenter(i);
        if (width<narrowest){
          narrowest=width;
          bestInt=integral;
          lowbin=i;
          highbin=j;
          i++;
        }
      }
    }
    thisStepSize/=2;
  }

  sw.Stop();
  cout << narrowest/2. << " " << bestInt << " [" << h->GetBinCenter(lowbin) << "," << h->GetBinCenter(highbin) << "]" << endl;
  cout << "Took:"; sw.Print();
  pair<double,double> result(h->GetBinCenter(lowbin),h->GetBinCenter(highbin));
  delete h;
  return result;
}

// get FWHHM
vector<double> getFWHM(RooRealVar *mass, RooAbsPdf *pdf, RooDataSet *data, double wmin=110., double wmax=130., double step=0.0004) {
 
  cout << "Computing FWHM...." << endl;
  double nbins = (wmax-wmin)/step;
  TH1F *h = new TH1F("h","h",int(floor(nbins+0.5)),wmin,wmax);
  if (data){
    pdf->fillHistogram(h,RooArgList(*mass),data->sumEntries());
  }
  else {
    pdf->fillHistogram(h,RooArgList(*mass));
  }
  
  double hm = h->GetMaximum()*0.5;
  double low = h->GetBinCenter(h->FindFirstBinAbove(hm));
  double high = h->GetBinCenter(h->FindLastBinAbove(hm));

  cout << "FWHM: [" << low << "-" << high << "] Max = " << hm << endl;
  vector<double> result;
  result.push_back(low);
  result.push_back(high);
  result.push_back(hm);
  result.push_back(h->GetBinWidth(1));
  
  delete h;
  return result;
}

void performClosure(RooRealVar *mass, RooAbsPdf *pdf, RooDataSet *data, string closurename, double wmin=110., double wmax=130., double slow=110., double shigh=130., double step=0.002) {
  
  // plot to perform closure test
  cout << "Performing closure test..." << endl; 
  double nbins = (wmax-wmin)/step;
  TH1F *h = new TH1F("h","h",int(floor(nbins+0.5)),wmin,wmax);
  if (data){
    pdf->fillHistogram(h,RooArgList(*mass),data->sumEntries());
    h->Scale(2*h->GetNbinsX()/double(binning_));
  }
  else {
    pdf->fillHistogram(h,RooArgList(*mass));
  }
  int binLow = h->FindBin(slow);
  int binHigh = h->FindBin(shigh)-1;
  TH1F *copy = new TH1F("copy","c",binHigh-binLow,h->GetBinLowEdge(binLow),h->GetBinLowEdge(binHigh+1));
  for (int b=0; b<copy->GetNbinsX(); b++) copy->SetBinContent(b+1,h->GetBinContent(b+1+binLow));
  double areaCov = 100*h->Integral(binLow,binHigh)/h->Integral();
 
  // style
  h->SetLineColor(kBlue);
  h->SetLineWidth(3);
  h->SetLineStyle(7);
  copy->SetLineWidth(3);
  copy->SetFillColor(kGray);
  
  TCanvas *c = new TCanvas();
  if (data){
    RooPlot *plot = mass->frame(Bins(binning_),Range("higgsRange"));
    plot->addTH1(h,"hist");
    plot->addTH1(copy,"same f");
    if (data) data->plotOn(plot);
    pdf->plotOn(plot,Normalization(h->Integral(),RooAbsReal::NumEvent),NormRange("higgsRange"),Range("higgsRange"),LineWidth(1),LineColor(kRed),LineStyle(kDashed));
    plot->Draw();
    c->Print(closurename.c_str());
  }
  else {
    RooPlot *plot = mass->frame(Bins(binning_),Range("higgsRange"));
    h->Scale(plot->getFitRangeBinW()/h->GetBinWidth(1));
    copy->Scale(plot->getFitRangeBinW()/h->GetBinWidth(1));
    pdf->plotOn(plot,LineColor(kRed),LineWidth(3));
    plot->Draw();
    h->Draw("hist same");
    copy->Draw("same f");
    c->Print(closurename.c_str());
  }
  cout << "IntH: [" << h->GetBinLowEdge(binLow) << "-" << h->GetBinLowEdge(binHigh+1) << "] Area = " << areaCov << endl;
  delete c;
  delete copy;
  delete h;
}

void Plot(RooRealVar *mass, RooDataSet *data, RooAbsPdf *pdf, pair<double,double> sigRange, vector<double> fwhmRange, string title, string savename){

  double semin=sigRange.first;
  double semax=sigRange.second;
  double fwmin=fwhmRange[0];
  double fwmax=fwhmRange[1];
  double halfmax=fwhmRange[2];
  double binwidth=fwhmRange[3];

  RooPlot *plot = mass->frame(Bins(binning_),Range("higgsRange"));
  if (data) data->plotOn(plot,Invisible());
  pdf->plotOn(plot,NormRange("higgsRange"),Range(semin,semax),FillColor(19),DrawOption("F"),LineWidth(2),FillStyle(1001),VLines(),LineColor(15));
  TObject *seffLeg = plot->getObject(int(plot->numItems()-1));
  pdf->plotOn(plot,NormRange("higgsRange"),Range(semin,semax),LineColor(15),LineWidth(2),FillStyle(1001),VLines());
  pdf->plotOn(plot,NormRange("higgsRange"),Range("higgsRange"),LineColor(kBlue),LineWidth(2),FillStyle(0));
  TObject *pdfLeg = plot->getObject(int(plot->numItems()-1));
  if (data) data->plotOn(plot,MarkerStyle(kOpenSquare));
  TObject *dataLeg = plot->getObject(int(plot->numItems()-1));
  TLegend *leg = new TLegend(0.15,0.89,0.5,0.55);
  leg->SetFillStyle(0);
  leg->SetLineColor(0);
  leg->SetTextSize(0.03);
  if (data) leg->AddEntry(dataLeg,"Simulation","lep");
  leg->AddEntry(pdfLeg,"Parametric model","l");
  leg->AddEntry(seffLeg,Form("#sigma_{eff} = %1.2f GeV",0.5*(semax-semin)),"fl");

  plot->GetXaxis()->SetNdivisions(509);
  halfmax*=(plot->getFitRangeBinW()/binwidth);
  TArrow *fwhmArrow = new TArrow(fwmin,halfmax,fwmax,halfmax,0.02,"<>");
  fwhmArrow->SetLineWidth(2.);
  TPaveText *fwhmText = new TPaveText(0.15,0.45,0.45,0.58,"brNDC");
  fwhmText->SetFillColor(0);
  fwhmText->SetLineColor(kWhite);
  fwhmText->SetTextSize(0.03);
  fwhmText->AddText(Form("FWHM = %1.2f GeV",(fwmax-fwmin)));

  TLatex lat1(0.65,0.85,"#splitline{CMS Preliminary}{Simulation}");
  lat1.SetNDC(1);
  lat1.SetTextSize(0.03);
  TLatex lat2(0.65,0.75,title.c_str());
  lat2.SetNDC(1);
  lat2.SetTextSize(0.025);

  TCanvas *canv = new TCanvas("c","c",600,600);
  plot->SetTitle("");
  plot->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");
  plot->Draw();
  leg->Draw("same");
  fwhmArrow->Draw("same <>");
  fwhmText->Draw("same");
  lat1.Draw("same");
  lat2.Draw("same");
  canv->Print(Form("%s.pdf",savename.c_str()));
  canv->Print(Form("%s.png",savename.c_str()));
  string path = savename.substr(0,savename.find('/'));
  canv->Print(Form("%s/animation.gif+100",path.c_str()));
  delete canv;

}

string runQuickRejig(string oldfilename, int ncats){
  
  string newfilename="TestWS.root";
  TFile *oldFile = TFile::Open(oldfilename.c_str());
  TFile *newFile = new TFile(newfilename.c_str(),"RECREATE");

  RooWorkspace *oldWS = (RooWorkspace*)oldFile->Get("cms_hgg_workspace");
  RooWorkspace *newWS = new RooWorkspace("wsig_8TeV");
  
  RooRealVar *mass = (RooRealVar*)oldWS->var("CMS_hgg_mass");

  RooDataSet *tot;

  RooRealVar *normT1 = new RooRealVar("nT1","nT1",500,0,550);
  RooRealVar *meanT1 = new RooRealVar("mT1","mT1",125,122,128);
  RooRealVar *sigmaT1 = new RooRealVar("sT1","sT1",3.1,1.,10.);
  RooGaussian *gausT1 = new RooGaussian("gT1","gT1",*mass,*meanT1,*sigmaT1);
  RooRealVar *normT2 = new RooRealVar("nT2","nT2",2,0,500);
  RooRealVar *meanT2 = new RooRealVar("mT2","mT2",125,122,128);
  RooRealVar *sigmaT2 = new RooRealVar("sT2","sT2",1.7,0.,10.);
  RooGaussian *gausT2 = new RooGaussian("gT2","gT2",*mass,*meanT2,*sigmaT2);
  RooRealVar *normT3 = new RooRealVar("nT3","nT3",2,0,500);
  RooRealVar *meanT3 = new RooRealVar("mT3","mT3",125,122,128);
  RooRealVar *sigmaT3 = new RooRealVar("sT3","sT3",1.7,0.,10.);
  RooGaussian *gausT3 = new RooGaussian("gT3","gT3",*mass,*meanT3,*sigmaT3);
  RooAddPdf *gausT = new RooAddPdf("gT","gT",RooArgList(*gausT1,*gausT2,*gausT3),RooArgList(*normT1,*normT2,*normT3));

  for (int cat=0; cat<ncats; cat++){
    RooDataSet *thisData = (RooDataSet*)oldWS->data(Form("sig_ggh_mass_m125_cat%d",cat));
    newWS->import(*thisData);
    RooRealVar *norm1 = new RooRealVar(Form("n1%d",cat),Form("n1%d",cat),500,0,550);
    RooRealVar *mean1 = new RooRealVar(Form("m1%d",cat),Form("m1%d",cat),125,122,128);
    RooRealVar *sigma1 = new RooRealVar(Form("s1%d",cat),Form("s1%d",cat),3.1,1.,10.);
    RooGaussian *gaus1 = new RooGaussian(Form("g1%d",cat),Form("g1%d",cat),*mass,*mean1,*sigma1);
    RooRealVar *norm2 = new RooRealVar(Form("n2%d",cat),Form("n2%d",cat),2,0,500);
    RooRealVar *mean2 = new RooRealVar(Form("m2%d",cat),Form("m2%d",cat),125,122,128);
    RooRealVar *sigma2 = new RooRealVar(Form("s2%d",cat),Form("s2%d",cat),1.7,0.,10.);
    RooGaussian *gaus2 = new RooGaussian(Form("g2%d",cat),Form("g2%d",cat),*mass,*mean2,*sigma2);
    RooRealVar *norm3 = new RooRealVar(Form("n3%d",cat),Form("n3%d",cat),2,0,500);
    RooRealVar *mean3 = new RooRealVar(Form("m3%d",cat),Form("m3%d",cat),125,122,128);
    RooRealVar *sigma3 = new RooRealVar(Form("s3%d",cat),Form("s3%d",cat),1.7,0.,10.);
    RooGaussian *gaus3 = new RooGaussian(Form("g3%d",cat),Form("g3%d",cat),*mass,*mean3,*sigma3);
    RooAddPdf *gaus = new RooAddPdf(Form("g%d",cat),"g",RooArgList(*gaus1,*gaus2,*gaus3),RooArgList(*norm1,*norm2,*norm3));
    gaus->fitTo(*thisData,SumW2Error(kTRUE));
    newWS->import(*gaus);
    if (cat==0) {
      tot = thisData;
      tot->SetName("sig_ggh_m125");
    }
    else tot->append(*thisData);
  }
  newWS->import(*tot);
  gausT->fitTo(*tot,SumW2Error(kTRUE));
  newWS->import(*gausT);
  newWS->Write();
  oldFile->Close();
  newFile->Close();
  delete newFile;
  delete newWS;

  return newfilename;

}

void printInfo(map<string,RooDataSet*> data, map<string,RooAddPdf*> pdfs){
  
  for (map<string,RooDataSet*>::iterator dat=data.begin(); dat!=data.end(); dat++){
    if (!dat->second) {
      cout << "Dataset for " << dat->first << " not found" << endl;
      exit(1);
    }
    cout << dat->first << " : ";
    dat->second->Print();
  }
  for (map<string,RooAddPdf*>::iterator pdf=pdfs.begin(); pdf!=pdfs.end(); pdf++){
    if (!pdf->second) {
      cout << "Pdf for " << pdf->first << " not found" << endl;
      exit(1);
    }
    cout << pdf->first << " : ";
    pdf->second->Print();
  }

}

map<string,RooDataSet*> getGlobeData(RooWorkspace *work, int ncats, int m_hyp){
  
  map<string,RooDataSet*> result;
  for (int cat=0; cat<ncats; cat++){
    result.insert(pair<string,RooDataSet*>(Form("cat%d",cat),(RooDataSet*)work->data(Form("sig_mass_m%3d_cat%d",m_hyp,cat))));
  }
  result.insert(pair<string,RooDataSet*>("all",(RooDataSet*)work->data(Form("sig_mass_m%3d_AllCats",m_hyp))));

  return result;
}

map<string,RooAddPdf*> getGlobePdfs(RooWorkspace *work, int ncats){

  map<string,RooAddPdf*> result;
  for (int cat=0; cat<ncats; cat++){
    result.insert(pair<string,RooAddPdf*>(Form("cat%d",cat),(RooAddPdf*)work->pdf(Form("sigpdfrelcat%d_allProcs",cat))));
  }
  result.insert(pair<string,RooAddPdf*>("all",(RooAddPdf*)work->pdf("sigpdfrelAllCats_allProcs")));

  return result;
}

map<string,RooAddPdf*> getMITPdfs(RooWorkspace *work, int ncats, bool is2011){

  map<string,RooAddPdf*> result;

  RooArgList pdfs;
  RooArgList norms;

  string sqrts;
  string catstring;
  if (is2011){
    sqrts="7TeV";
    catstring="cat";
  }
  else {
    sqrts="8TeV";
    catstring="mvacat";
  }

  for (int cat=0; cat<ncats; cat++){
    RooAddPdf *ggh = (RooAddPdf*)work->pdf(Form("hggpdfsmrel_%s%d_%s_ggh",catstring.c_str(),cat,sqrts.c_str())); 
    RooAddPdf *vbf = (RooAddPdf*)work->pdf(Form("hggpdfsmrel_%s%d_%s_vbf",catstring.c_str(),cat,sqrts.c_str())); 
    RooAddPdf *wzh = (RooAddPdf*)work->pdf(Form("hggpdfsmrel_%s%d_%s_wzh",catstring.c_str(),cat,sqrts.c_str())); 
    RooAddPdf *tth = (RooAddPdf*)work->pdf(Form("hggpdfsmrel_%s%d_%s_tth",catstring.c_str(),cat,sqrts.c_str()));
    RooFormulaVar *ggh_norm = (RooFormulaVar*)work->function(Form("hggpdfsmrel_%s%d_%s_ggh_norm",catstring.c_str(),cat,sqrts.c_str()));
    RooFormulaVar *vbf_norm = (RooFormulaVar*)work->function(Form("hggpdfsmrel_%s%d_%s_vbf_norm",catstring.c_str(),cat,sqrts.c_str()));
    RooFormulaVar *wzh_norm = (RooFormulaVar*)work->function(Form("hggpdfsmrel_%s%d_%s_wzh_norm",catstring.c_str(),cat,sqrts.c_str()));
    RooFormulaVar *tth_norm = (RooFormulaVar*)work->function(Form("hggpdfsmrel_%s%d_%s_tth_norm",catstring.c_str(),cat,sqrts.c_str()));
    pdfs.add(*ggh); pdfs.add(*vbf); pdfs.add(*wzh); pdfs.add(*tth);
    norms.add(*ggh_norm); norms.add(*vbf_norm); norms.add(*wzh_norm); norms.add(*tth_norm);
    result.insert(pair<string,RooAddPdf*>(Form("cat%d",cat),new RooAddPdf(Form("sig_cat%d",cat),"sig",RooArgList(*ggh,*vbf,*wzh,*tth),RooArgList(*ggh_norm,*vbf_norm,*wzh_norm,*tth_norm))));
  }
  result.insert(pair<string,RooAddPdf*>("all",new RooAddPdf("sig_all","sig",pdfs,norms)));
  return result;

}

int getBestFitFunction(RooMultiPdf *bkg, RooAbsData *data, RooCategory *cat, bool silent=false){

	double global_minNll = 1E10;
	int best_index = 0;
	cat->Print("v");
	int number_of_indeces = cat->numTypes();
	
	RooArgSet snap,clean;
	RooArgSet *params = bkg->getParameters(*data);
	params->snapshot(snap);
	params->snapshot(clean);
	if (!silent) {
		std::cout << "CLEAN SET OF PARAMETERS" << std::endl;
		params->Print("V");
		std::cout << "-----------------------" << std::endl;
	}
	
	//bkg->setDirtyInhibit(1);
	RooAbsReal *nllm = bkg->createNLL(*data);
	RooMinimizer minim(*nllm);
	minim.setStrategy(2);
	
	for (int id=0;id<number_of_indeces;id++){		
		params->assignValueOnly(clean);
		cat->setIndex(id);

		//RooAbsReal *nllm = bkg->getCurrentPdf()->createNLL(*data);

		if (!silent) {
			std::cout << "BEFORE FITTING" << std::endl;
			params->Print("V");
			std::cout << "-----------------------" << std::endl;
		}
		
		minim.minimize("Minuit2","simplex");
		double minNll = nllm->getVal()+bkg->getCorrection();
		if (!silent) {
			std::cout << "After Minimization ------------------  " <<std::endl;
			std::cout << bkg->getCurrentPdf()->GetName() << " " << minNll <<std::endl;
			bkg->Print("v");
			bkg->getCurrentPdf()->getParameters(*data)->Print("V");
			std::cout << " ------------------------------------  " << std::endl;
	
			std::cout << "AFTER FITTING" << std::endl;
			params->Print("V");
			std::cout << "-----------------------" << std::endl;
		}
			
		if (minNll < global_minNll){
        		global_minNll = minNll;
			snap.assignValueOnly(*params);
        		best_index=id;
		}
	}
	params->assignValueOnly(snap);
    	cat->setIndex(best_index);
	
	if (!silent) {
		std::cout << "Best fit Function -- " << bkg->getCurrentPdf()->GetName() << " " << cat->getIndex() <<std::endl;
		bkg->getCurrentPdf()->getParameters(*data)->Print("v");
	}
	return best_index;
}

pair<double,double> getNormTermNllAndRes(RooRealVar *mgg, RooAbsData *data, RooMultiPdf *mpdf, RooCategory *mcat, double normVal=-1., double massRangeLow=-1., double massRangeHigh=-1.){
	
	double bestFitNll=1.e8;
	double bestFitNorm;

	for (int pInd=0; pInd<mpdf->getNumPdfs(); pInd++){
		mcat->setIndex(pInd);
		RooRealVar *normVar = new RooRealVar(Form("%snorm",mpdf->getCurrentPdf()->GetName()),"",0.,1.e6);
		RooExtendPdf *extPdf;
		RooAbsReal *nll;
		if (massRangeLow>-1. && massRangeHigh>-1.){
			mgg->setRange("errRange",massRangeLow,massRangeHigh);
			extPdf = new RooExtendPdf(Form("%sext",mpdf->getCurrentPdf()->GetName()),"",*(mpdf->getCurrentPdf()),*normVar,"errRange");
			nll = extPdf->createNLL(*data,Extended()); //,Range(massRangeLow,massRangeHigh));//,Range("errRange"));
		}
		else {
			extPdf = new RooExtendPdf(Form("%sext",mpdf->getCurrentPdf()->GetName()),"",*(mpdf->getCurrentPdf()),*normVar);
			nll = extPdf->createNLL(*data,Extended());
		}
		
		if (normVal>-1.){
			normVar->setConstant(false);
			normVar->setVal(normVal);
			normVar->setConstant(true);
		}

		RooMinimizer minim(*nll);
		minim.setStrategy(0);
		//minim.minimize("Minuit2","simplex");
		minim.migrad();
		double corrNll = nll->getVal()+mpdf->getCorrection();

		//cout << "Found fit: " << mpdf->getCurrentPdf()->GetName() << " " << mpdf->getCorrection() << " " << normVar->getVal() << " " << corrNll << endl;

		if (corrNll<bestFitNll){
			bestFitNll=corrNll;
			bestFitNorm=normVar->getVal();
		}
		if (normVal>-1.) normVar->setConstant(false);
	}
	//cout << "CACHE: " << bestFitNorm << " -- " << bestFitNll << endl;
	return make_pair(2*bestFitNll,bestFitNorm);
}

double getNormTermNll(RooRealVar *mgg, RooAbsData *data, RooMultiPdf *mpdf, RooCategory *mcat, double normVal=-1., double massRangeLow=-1., double massRangeHigh=-1.){
	pair<double,double> temp = getNormTermNllAndRes(mgg,data,mpdf,mcat,normVal,massRangeLow,massRangeHigh);
	return temp.first;
}

double guessNew(RooRealVar *mgg, RooMultiPdf *mpdf, RooCategory *mcat, RooAbsData *data, double bestPoint, double nllBest, double boundary, double massRangeLow, double massRangeHigh, double crossing, double tolerance){
	
	bool isLowSide;
	double guess, guessNll, lowPoint,highPoint;
	if (boundary>bestPoint) {
		isLowSide=false;
		lowPoint = bestPoint;
		highPoint = boundary;
	}
	else {
		isLowSide=true;
		lowPoint = boundary;
		highPoint = bestPoint;
	}
	//double prevDistanceFromTruth = 1.e6;
	double distanceFromTruth = 1.e6;
	int nIts=0;
	while (TMath::Abs(distanceFromTruth/crossing)>tolerance) {
		
		//prevDistanceFromTruth=distanceFromTruth;
		guess = lowPoint+(highPoint-lowPoint)/2.;
		guessNll = getNormTermNll(mgg,data,mpdf,mcat,guess,massRangeLow,massRangeHigh)-nllBest;
    distanceFromTruth = crossing - guessNll;
	
		// for low side. if nll val is lower than target move right point left. if nll val is higher than target move left point right
		// vice versa for high side
		if (isLowSide){
			if (guessNll>crossing) lowPoint = guess;
			else highPoint=guess;
		}
		else {
			if (guessNll>crossing) highPoint = guess;
			else lowPoint=guess;
		}
		nIts++;
		// because these are envelope nll curves this algorithm can get stuck in local minima
		// hacked get out is just to start trying again
		if (nIts>20) {
			lowPoint = TMath::Max(0.,lowPoint-20);
			highPoint += 20;
			nIts=0;
			// if it's a ridicolous value it wont converge so return value of bestGuess
			if (TMath::Abs(guessNll)>2e4) return 0.; 
		}
	}
	return guess;
}

pair<double,double> bkgEvPerGeV(RooWorkspace *work, int m_hyp, int cat, pair<double,double> &bkgTotal, bool is2011){
  
	if (work->GetName()==TString("cms_hgg_workspace")) {
		RooRealVar *mass = (RooRealVar*)work->var("CMS_hgg_mass");
		mass->setRange(100,180);
		RooAbsPdf *pdf;
		if (is2011) pdf = (RooAbsPdf*)work->pdf(Form("pdf_data_pol_model_7TeV_cat%d",cat));
		else pdf = (RooAbsPdf*)work->pdf(Form("pdf_data_pol_model_8TeV_cat%d",cat));
		RooAbsData *data = (RooDataSet*)work->data(Form("data_mass_cat%d",cat));
		RooPlot *tempFrame = mass->frame();
		data->plotOn(tempFrame,Binning(80));
		pdf->plotOn(tempFrame);
		RooCurve *curve = (RooCurve*)tempFrame->getObject(tempFrame->numItems()-1);
		double nombkg = curve->Eval(double(m_hyp));
	 
		RooRealVar *nlim = new RooRealVar(Form("nlim%d",cat),"",0.,0.,1.e5);
		//double lowedge = tempFrame->GetXaxis()->GetBinLowEdge(FindBin(double(m_hyp)));
		//double upedge  = tempFrame->GetXaxis()->GetBinUpEdge(FindBin(double(m_hyp)));
		//double center  = tempFrame->GetXaxis()->GetBinUpCenter(FindBin(double(m_hyp)));

		nlim->setVal(nombkg);
		mass->setRange("errRange",m_hyp-0.5,m_hyp+0.5);
		RooAbsPdf *epdf = 0;
		epdf = new RooExtendPdf("epdf","",*pdf,*nlim,"errRange");
			
		RooAbsReal *nll = epdf->createNLL(*data,Extended(),NumCPU(4));
		RooMinimizer minim(*nll);
		minim.setStrategy(0);
		minim.setPrintLevel(-1);
		minim.migrad();
		minim.minos(*nlim);
		
		double error = (nlim->getErrorLo(),nlim->getErrorHi())/2.;
		data->Print();
		bkgTotal.first += nombkg;
		bkgTotal.second += error*error;
		return pair<double,double>(nombkg,error); 
	}
	else if (work->GetName()==TString("multipdf")) {
		RooRealVar *mass = (RooRealVar*)work->var("CMS_hgg_mass");
		mass->setRange(100,180);
		RooMultiPdf *mpdf;
		if (is2011) mpdf = (RooMultiPdf*)work->pdf(Form("CMS_hgg_cat%d_7TeV_bkgshape",cat));
		else mpdf = (RooMultiPdf*)work->pdf(Form("CMS_hgg_cat%d_8TeV_bkgshape",cat));
		RooCategory *mcat;
		if (is2011)  mcat = (RooCategory*)work->cat(Form("pdfindex_%d_7TeV",cat));
		else mcat = (RooCategory*)work->cat(Form("pdfindex_%d_8TeV",cat));
		RooAbsData *data = (RooDataSet*)work->data(Form("roohist_data_mass_cat%d",cat));
		if (!mcat) {
			cout << "Category is NULL" << endl;
			exit(1);
		}
		if (!data){
			cout << "Data is NULL" << endl;
			exit(1);
		}
		
		// reset to best fit
		int bf = getBestFitFunction(mpdf,data,mcat,true);
		mcat->setIndex(bf);
		cout << "Best fit PDF and data:" << endl;
		cout << "\t"; mpdf->getCurrentPdf()->Print();
		cout << "\t"; data->Print();

		RooPlot *tempFrame = mass->frame();
		data->plotOn(tempFrame,Binning(80));
		mpdf->getCurrentPdf()->plotOn(tempFrame);
		RooCurve *nomBkgCurve = (RooCurve*)tempFrame->getObject(tempFrame->numItems()-1);
		
		double nomBkg = nomBkgCurve->interpolate(m_hyp);
		double nllBest = getNormTermNll(mass,data,mpdf,mcat,nomBkg,m_hyp-0.5,m_hyp+0.5);
		double lowRange = TMath::Max(0.,nomBkg - 3*TMath::Sqrt(nomBkg));
		double highRange = nomBkg + 3*TMath::Sqrt(nomBkg);

		double errLow1Value = guessNew(mass,mpdf,mcat,data,nomBkg,nllBest,lowRange,m_hyp-0.5,m_hyp+0.5,1.,0.05);
		double errHigh1Value = guessNew(mass,mpdf,mcat,data,nomBkg,nllBest,highRange,m_hyp-0.5,m_hyp+0.5,1.,0.05);
		double errLow1 = nomBkg - errLow1Value;
		double errHigh1 = errHigh1Value - nomBkg;
		
		double error = (errLow1+errHigh1)/2.; 
		data->Print();
		bkgTotal.first += nomBkg;
		bkgTotal.second += error*error;
		return pair<double,double>(nomBkg,error); 
	}
	else {
		cout << "Background workspace name " << work->GetName() << " not recognised. Bailing!" << endl;
		exit(1);
		return pair<double,double>(0.,0.);
	}
	return pair<double,double>(0.,0.);
}

double sobInFWHM(RooWorkspace *sigWS, RooWorkspace *bkgWS, int m_hyp, int cat, double fwhm, pair<double,double> &sobInFWHMTotal, bool splitVH, bool is2011){

  RooRealVar *mass = (RooRealVar*)bkgWS->var("CMS_hgg_mass");
  mass->setRange(100,180);
  mass->setRange(Form("fwhm_cat%d",cat),m_hyp-fwhm,m_hyp+fwhm);
  RooAbsPdf *pdf=NULL;
	RooAbsData *data=NULL;
	
	if (bkgWS->GetName()==TString("cms_hgg_workspace")) {
		if (is2011) pdf = (RooAbsPdf*)bkgWS->pdf(Form("data_pol_model_7TeV_cat%d",cat)); 
		else pdf = (RooAbsPdf*)bkgWS->pdf(Form("data_pol_model_8TeV_cat%d",cat));
		data = (RooDataSet*)bkgWS->data(Form("data_mass_cat%d",cat));
	}
	else if (bkgWS->GetName()==TString("multipdf")) {
		RooMultiPdf *mpdf;
		if (is2011) mpdf = (RooMultiPdf*)bkgWS->pdf(Form("CMS_hgg_cat%d_7TeV_bkgshape",cat));
		else mpdf = (RooMultiPdf*)bkgWS->pdf(Form("CMS_hgg_cat%d_8TeV_bkgshape",cat));
		RooCategory *mcat;
		if (is2011) mcat = (RooCategory*)bkgWS->cat(Form("pdfindex_%d_7TeV",cat));
		else mcat = (RooCategory*)bkgWS->cat(Form("pdfindex_%d_8TeV",cat));
		data = (RooDataSet*)bkgWS->data(Form("roohist_data_mass_cat%d",cat));
		// reset to best fit
		int bf = getBestFitFunction(mpdf,data,mcat,true);
		mcat->setIndex(bf);
		pdf = mpdf->getCurrentPdf();
	}
	else {
		cout << "Background workspace name " << bkgWS->GetName() << " not recognised. Bailing!" << endl;
		exit(1);
	}

	assert(pdf);
	assert(data);

  RooAbsReal *integral = pdf->createIntegral(RooArgSet(*mass),NormSet(*mass),Range(Form("fwhm_cat%d",cat)));
  double bkgInFWHM = integral->getVal()*data->sumEntries();
  
  RooDataSet *ggh = (RooDataSet*)sigWS->data(Form("sig_ggh_mass_m%d_cat%d",m_hyp,cat));
  RooDataSet *vbf = (RooDataSet*)sigWS->data(Form("sig_vbf_mass_m%d_cat%d",m_hyp,cat));
  RooDataSet *tth = (RooDataSet*)sigWS->data(Form("sig_tth_mass_m%d_cat%d",m_hyp,cat));
    
  string cutstring = Form("CMS_hgg_mass>=%7.3f && CMS_hgg_mass<=%7.3f",m_hyp-fwhm,m_hyp+fwhm);
  double sigInFWHM = ggh->sumEntries(cutstring.c_str())+vbf->sumEntries(cutstring.c_str())+tth->sumEntries(cutstring.c_str());
  if (splitVH) {
    RooDataSet *wh = (RooDataSet*)sigWS->data(Form("sig_wh_mass_m%d_cat%d",m_hyp,cat));
    RooDataSet *zh = (RooDataSet*)sigWS->data(Form("sig_zh_mass_m%d_cat%d",m_hyp,cat));
    sigInFWHM += wh->sumEntries(cutstring.c_str())+zh->sumEntries(cutstring.c_str()); 
  }
  else {
    RooDataSet *wzh = (RooDataSet*)sigWS->data(Form("sig_wzh_mass_m%d_cat%d",m_hyp,cat));
    sigInFWHM += wzh->sumEntries(cutstring.c_str()); 
  }

  sobInFWHMTotal.first += sigInFWHM;
  sobInFWHMTotal.second += bkgInFWHM;

  return sigInFWHM/(sigInFWHM+bkgInFWHM);

}

double sobInEffSigma(RooWorkspace *sigWS, RooWorkspace *bkgWS, int m_hyp, int cat, double effSigma, pair<double,double> &sobInEffSigmaTotal, bool splitVH, bool is2011){

  RooRealVar *mass = (RooRealVar*)bkgWS->var("CMS_hgg_mass");
  mass->setRange(100,180);
  mass->setRange(Form("effsigma_cat%d",cat),m_hyp-effSigma,m_hyp+effSigma);
  RooAbsPdf *pdf=NULL;
	RooAbsData *data=NULL;
	
	if (bkgWS->GetName()==TString("cms_hgg_workspace")) {
		if (is2011) pdf = (RooAbsPdf*)bkgWS->pdf(Form("data_pol_model_7TeV_cat%d",cat)); 
		else pdf = (RooAbsPdf*)bkgWS->pdf(Form("data_pol_model_8TeV_cat%d",cat));
		data = (RooDataSet*)bkgWS->data(Form("data_mass_cat%d",cat));
	}
	else if (bkgWS->GetName()==TString("multipdf")) {
		RooMultiPdf *mpdf;
		if (is2011) mpdf = (RooMultiPdf*)bkgWS->pdf(Form("CMS_hgg_cat%d_7TeV_bkgshape",cat));
		else mpdf = (RooMultiPdf*)bkgWS->pdf(Form("CMS_hgg_cat%d_8TeV_bkgshape",cat));
		RooCategory *mcat;
		if (is2011) mcat = (RooCategory*)bkgWS->cat(Form("pdfindex_%d_7TeV",cat));
		else mcat = (RooCategory*)bkgWS->cat(Form("pdfindex_%d_8TeV",cat));
		data = (RooDataSet*)bkgWS->data(Form("roohist_data_mass_cat%d",cat));
		// reset to best fit
		int bf = getBestFitFunction(mpdf,data,mcat,true);
		mcat->setIndex(bf);
		pdf = mpdf->getCurrentPdf();
	}
	else {
		cout << "Background workspace name " << bkgWS->GetName() << " not recognised. Bailing!" << endl;
		exit(1);
	}

	assert(pdf);
	assert(data);

  RooAbsReal *integral = pdf->createIntegral(RooArgSet(*mass),NormSet(*mass),Range(Form("effsigma_cat%d",cat)));
  double bkgInEffSigma = integral->getVal()*data->sumEntries();
  
  RooDataSet *ggh = (RooDataSet*)sigWS->data(Form("sig_ggh_mass_m%d_cat%d",m_hyp,cat));
  RooDataSet *vbf = (RooDataSet*)sigWS->data(Form("sig_vbf_mass_m%d_cat%d",m_hyp,cat));
  RooDataSet *tth = (RooDataSet*)sigWS->data(Form("sig_tth_mass_m%d_cat%d",m_hyp,cat));
    
  string cutstring = Form("CMS_hgg_mass>=%7.3f && CMS_hgg_mass<=%7.3f",m_hyp-effSigma,m_hyp+effSigma);
  double sigInEffSigma = ggh->sumEntries(cutstring.c_str())+vbf->sumEntries(cutstring.c_str())+tth->sumEntries(cutstring.c_str());
  if (splitVH) {
    RooDataSet *wh = (RooDataSet*)sigWS->data(Form("sig_wh_mass_m%d_cat%d",m_hyp,cat));
    RooDataSet *zh = (RooDataSet*)sigWS->data(Form("sig_zh_mass_m%d_cat%d",m_hyp,cat));
    sigInEffSigma += wh->sumEntries(cutstring.c_str())+zh->sumEntries(cutstring.c_str()); 
  }
  else {
    RooDataSet *wzh = (RooDataSet*)sigWS->data(Form("sig_wzh_mass_m%d_cat%d",m_hyp,cat));
    sigInEffSigma += wzh->sumEntries(cutstring.c_str()); 
  }

  sobInEffSigmaTotal.first += sigInEffSigma;
  sobInEffSigmaTotal.second += bkgInEffSigma;

  return sigInEffSigma/(sigInEffSigma+bkgInEffSigma);

}

vector<double> sigEvents(RooWorkspace *work, int m_hyp, int cat, string binnedSigFileName, vector<double> &sigTotal, bool splitVH, string spinProc=""){

  RooWorkspace *tempWork;
  if (binnedSigFileName!=""){
    TFile *temp = TFile::Open(binnedSigFileName.c_str());
    tempWork = (RooWorkspace*)temp->Get("cms_hgg_workspace");
  }
  else {
    tempWork = work;
  }
  vector<double> result;
  
  if (spinProc==""){
    
    RooDataSet *ggh = (RooDataSet*)tempWork->data(Form("sig_ggh_mass_m%d_cat%d",m_hyp,cat));
    RooDataSet *vbf = (RooDataSet*)tempWork->data(Form("sig_vbf_mass_m%d_cat%d",m_hyp,cat));
    RooDataSet *tth = (RooDataSet*)tempWork->data(Form("sig_tth_mass_m%d_cat%d",m_hyp,cat));
    RooDataSet *wzh = NULL;
    RooDataSet *wh = NULL;
    RooDataSet *zh = NULL;
    
    double total;
    if (splitVH) {
      wh = (RooDataSet*)tempWork->data(Form("sig_wh_mass_m%d_cat%d",m_hyp,cat));
      zh = (RooDataSet*)tempWork->data(Form("sig_zh_mass_m%d_cat%d",m_hyp,cat));
      total = ggh->sumEntries()+vbf->sumEntries()+wh->sumEntries()+zh->sumEntries()+tth->sumEntries();
    }
    else {
      wzh = (RooDataSet*)tempWork->data(Form("sig_wzh_mass_m%d_cat%d",m_hyp,cat));
      total = ggh->sumEntries()+vbf->sumEntries()+wzh->sumEntries()+tth->sumEntries();
    }
    result.push_back(total);
    result.push_back(ggh->sumEntries());
    result.push_back(vbf->sumEntries());
    if (splitVH) {
      result.push_back(wh->sumEntries());
      result.push_back(zh->sumEntries());
    }
    else {
      result.push_back(wzh->sumEntries());
    }
    result.push_back(tth->sumEntries());

    sigTotal[0] += total;
    sigTotal[1] += ggh->sumEntries();
    sigTotal[2] += vbf->sumEntries();
    if (splitVH) {
      sigTotal[3] += wh->sumEntries();
      sigTotal[4] += zh->sumEntries();
      sigTotal[5] += tth->sumEntries();
    }
    else {
      sigTotal[3] += wzh->sumEntries();
      sigTotal[4] += tth->sumEntries();
    }
  }
  else if (spinProc=="gg_grav"){
    
    RooDataHist *ggh = (RooDataHist*)tempWork->data(Form("roohist_sig_gg_grav_mass_m%d_cat%d",m_hyp,cat));

    double total = ggh->sumEntries();
    result.push_back(total);
    result.push_back(ggh->sumEntries());
    result.push_back(0.);
    result.push_back(0.);
    result.push_back(0.);

    sigTotal[0] += total;
    sigTotal[1] += ggh->sumEntries();
  }
  else if (spinProc=="qq_grav"){
    RooDataHist *vbf = (RooDataHist*)tempWork->data(Form("roohist_sig_qq_grav_mass_m%d_cat%d",m_hyp,cat));
    
    double total = vbf->sumEntries();
    result.push_back(total);
    result.push_back(0.);
    result.push_back(vbf->sumEntries());
    result.push_back(0.);
    result.push_back(0.);

    sigTotal[0] += total;
    sigTotal[1] += vbf->sumEntries();
  }
  else {
    cout << "WARNING -- spinProc " << spinProc << " is no recognised." << endl;
  }

  return result;
}

pair<double,double> datEvents(RooWorkspace *work, int m_hyp, int cat, pair<double,double> &runningTotal){
  
  vector<double> result;
  RooDataSet *data = (RooDataSet*)work->data(Form("roohist_data_mass_cat%d",cat));
  double evs = data->sumEntries();
  double evsPerGev;
  evsPerGev = data->sumEntries(Form("CMS_hgg_mass>=%4.1f && CMS_hgg_mass<%4.1f",double(m_hyp)-0.5,double(m_hyp)+0.5));
  runningTotal.first += evs;
  runningTotal.second += evsPerGev;
  return pair<double,double>(evs,evsPerGev);
}

// from p. meridiani with addition from m. kenzie
void makeSignalCompositionPlot(int nCats, map<string,string> labels, map<string,vector<double> > sigVals, map<string,double> sigEffs, map<string,double> fwhms, map<string,double> sobVals, string outfname, int mh, bool doBkgAndData, bool splitVH, bool isMassFac, bool is2011, int raiseVHdijet){

  TString catName[nCats+1];
	for (int cat=0; cat<nCats; cat++) catName[cat] = labels[Form("cat%d",cat)];
	catName[nCats] = labels["all"];

  const int nprocs = splitVH ? 5 : 4;

  vector<TString> processName;
  vector<int> colors;
  processName.push_back("ggH");
  processName.push_back("qqH");
  colors.push_back(kGreen+3);
  colors.push_back(kRed+2);
  if (splitVH) {
    processName.push_back("WH");
    processName.push_back("ZH");
    colors.push_back(kCyan+2);
    colors.push_back(kAzure-6);
  }
  else {
    processName.push_back("VH");
    colors.push_back(kAzure-6);
  }
	colors.push_back(kOrange+7);
  processName.push_back("ttH");

  for (int i=0;i<nprocs;++i)
  {
    std::cout <<"+++++++++++++++++ " << processName[i] << " ++++++++++++++++++++" << std::endl;
    for (int c=0;c<nCats;++c)
    {
			int cat = c;
			if (c == nCats-1-raiseVHdijet) cat = nCats-1;
			if (c > nCats-1-raiseVHdijet && c < nCats) cat = c-1;
      cout << catName[cat] << ": " <<  100.*sigVals[Form("cat%d",cat)][i+1]/sigVals[Form("cat%d",cat)][0] << "%" << std::endl;
    }
    cout << catName[nCats] << ": " <<  100.*sigVals["all"][i+1]/sigVals["all"][0] << "%" << std::endl;
  }

  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  
  TCanvas *canv;
  TPad *Plots;
  TPad *Width;
  TPad *SOB;
  if (doBkgAndData) {
    canv = new TCanvas("canv","canv",1,1,7000,4800);
    Plots = new TPad("Plots","Plots",0.,0.,0.5,1.);
    Width = new TPad("Width","Width",0.5,0.,0.75,1.);
    SOB = new TPad("SOB","SOB",0.75,0.,1.,1.);
  }
  else {
    canv = new TCanvas("canv","canv",1,1,4800,4800);
    Plots = new TPad("Plots","Plots",0.,0.,0.667,1.);
    Width = new TPad("Width","Width",0.667,0.,1.,1.);
  }
  canv->SetHighLightColor(2);
  Plots->cd();

  gStyle->SetOptStat(0);
  Plots->Range(-14.67532,-1.75,11.2987,15.75);
  Plots->SetFillColor(0);
  Plots->SetBorderMode(0);
  Plots->SetBorderSize(2);
  Plots->SetTopMargin(0.16);
  Plots->SetLeftMargin(0.25);
  Plots->SetRightMargin(0.05);
  Plots->SetFrameBorderMode(0);
  Plots->SetFrameBorderMode(0);

  TH2F *dummy = new TH2F("dummy","",10,0.,100.,nCats+1,-0.5,nCats+0.5);
  dummy->SetStats(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  Int_t ci;   // for color index setting
  ci = TColor::GetColor("#00ff00");
  dummy->SetFillColor(ci);

  for (int c=0;c<nCats+1;++c) {
		int cat = c;
		if (c == nCats-1-raiseVHdijet) cat = nCats-1;
		if (c > nCats-1-raiseVHdijet && c < nCats) cat = c-1;
		cout << "Old cat: " << c << " New cat: " << cat << " Bin: " << nCats+1-c << " Label: " << catName[cat] << endl;
		dummy->GetYaxis()->SetBinLabel(nCats+1-c,catName[cat]);
  }
	dummy->GetXaxis()->SetTickLength(0.01);
  dummy->GetYaxis()->SetTickLength(0);
  dummy->GetXaxis()->SetTitle("Signal Fraction (%)");
  dummy->GetXaxis()->SetNdivisions(510);
  dummy->GetXaxis()->SetLabelFont(42);
  dummy->GetXaxis()->SetLabelSize(0.045);
  dummy->GetXaxis()->SetTitleSize(0.045);
  dummy->GetXaxis()->SetTitleOffset(0.95);
  dummy->GetXaxis()->SetTitleFont(42);
  dummy->GetYaxis()->SetNdivisions(510);
  dummy->GetYaxis()->SetLabelSize(0.035);
  dummy->GetYaxis()->SetTitleSize(0.045);
  dummy->GetYaxis()->SetTitleOffset(1.1);
  dummy->GetYaxis()->SetTitleFont(42);
  dummy->GetZaxis()->SetLabelFont(42);
  dummy->GetZaxis()->SetLabelSize(0.035);
  dummy->GetZaxis()->SetTitleSize(0.035);
  dummy->GetZaxis()->SetTitleFont(42);
  dummy->Draw("");

  //loop on bins and draw a TPave
  float ymin = 0.0;
  float width = 0.34;

  TPave *pave;
	TPaveText *pavetext;
  for (int c=0; c<nCats+1; c++) {
		int cat = c;
		if (c == nCats-1-raiseVHdijet) cat = nCats-1;
		if (c > nCats-1-raiseVHdijet && c < nCats) cat = c-1;
    float ybin = nCats-c;
    float ybinmin = ybin-width;
    float ybinmax = ybin+width;
    float xbinmin = ymin;
    float xbinmax = ymin;
		// text yield
		pavetext = new TPaveText(xbinmin+0.5,ybinmin,xbinmin+30.5,ybinmax);
		if (cat==nCats) pavetext->AddText(Form("%.1f total expected signal",sigVals["all"][0])); 
		else pavetext->AddText(Form("%.1f total expected signal",sigVals[Form("cat%d",cat)][0]));
		pavetext->SetTextColor(0);
		pavetext->SetFillStyle(0);
		pavetext->SetFillColor(0);
		pavetext->SetLineColor(0);
		pavetext->SetBorderSize(0);
    //Stacked fractions
    for (int j=0; j<nprocs; j++){
      if (cat==nCats) xbinmax += 100.*sigVals["all"][j+1]/sigVals["all"][0];
      else xbinmax += 100.*sigVals[Form("cat%d",cat)][j+1]/sigVals[Form("cat%d",cat)][0];
      pave = new TPave(xbinmin,ybinmin,xbinmax,ybinmax);
      pave->SetFillColor(colors[j]);
      pave->Draw();
      pave->SetBorderSize(0);
      if (cat==nCats) xbinmin += 100.*sigVals["all"][j+1]/sigVals["all"][0];
      else xbinmin += 100.*sigVals[Form("cat%d",cat)][j+1]/sigVals[Form("cat%d",cat)][0];
    }
		pavetext->Draw();
  }

  TLatex *   tex_m=new TLatex();
  tex_m->SetNDC();
  tex_m->SetTextAlign(12);
  tex_m->SetTextSize(0.025);
  tex_m->SetLineWidth(2);

  if (splitVH) {
    pave=new TPave(0.25,0.85,0.3,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[0]);
    pave->Draw();
    tex_m->DrawLatex(0.32,0.84+0.025,"ggH");

    pave=new TPave(0.4,0.85,0.45,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[1]);
    pave->Draw();
    tex_m->DrawLatex(0.47,0.84+0.025,"qqH");

    pave=new TPave(0.55,0.85,0.6,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[2]);
    pave->Draw();
    tex_m->DrawLatex(0.62,0.84+0.025,"WH");

    pave=new TPave(0.7,0.85,0.75,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[3]);
    pave->Draw();
    tex_m->DrawLatex(0.77,0.84+0.025,"ZH");
    
    pave=new TPave(0.85,0.85,0.9,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[4]);
    pave->Draw();
    tex_m->DrawLatex(0.92,0.84+0.025,"ttH");
  }
  else {
    pave=new TPave(0.20,0.85,0.23,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[0]);
    pave->Draw();
    tex_m->DrawLatex(0.24,0.84+0.025,"ggH");

    pave=new TPave(0.38,0.85,0.41,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[1]);
    pave->Draw();
    tex_m->DrawLatex(0.42,0.84+0.025,"qqH");

    pave=new TPave(0.56,0.85,0.59,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[2]);
    pave->Draw();
    tex_m->DrawLatex(0.60,0.84+0.025,"VH");

    pave=new TPave(0.74,0.85,0.77,0.85+0.03,0,"NDC");
    pave->SetFillColor(colors[3]);
    pave->Draw();
    tex_m->DrawLatex(0.78,0.84+0.025,"ttH");
  }

	int nIncCats=8;
	int nVBFCats=2;
	if (isMassFac){
		if (is2011) {
			nIncCats=4;
			nVBFCats=2;
		}
		else {
			nIncCats=5;
			nVBFCats=3;
		}
	}
  TLine *line = new TLine(0,nCats-nIncCats+0.5,100,nCats-nIncCats+0.5);
  line->SetLineColor(kBlack);
  line->SetLineWidth(2);
  line->SetLineStyle(2);
  line->Draw();

  TLine *line2 = new TLine(0,nCats-nIncCats-nVBFCats+0.5,100,nCats-nIncCats-nVBFCats+0.5);
  line2->SetLineColor(kBlack);
  line2->SetLineWidth(2);
  line2->SetLineStyle(2);
  line2->Draw();

  TLine *line3 = new TLine(0,0.5,100,0.5);
  line3->SetLineColor(kBlack);
  line3->SetLineWidth(2);
  line3->SetLineStyle(9);
  line3->Draw();

  //Plots->RedrawAxis();

  Width->cd();
  Width->Range(-14.67532,-1.75,11.2987,15.75);
  Width->SetFillColor(0);
  Width->SetBorderMode(0);
  Width->SetBorderSize(2);
  Width->SetTopMargin(0.16);
  Width->SetLeftMargin(0.08);
  Width->SetRightMargin(0.05);
  Width->SetFrameBorderMode(0);
  Width->SetFrameBorderMode(0);
  TH2F *dummy2 = new TH2F("dummy2","",1,0.,3.,nCats+1,-0.5,nCats+0.5);
  for (int i=1; i<=dummy2->GetNbinsX(); i++) dummy2->GetYaxis()->SetBinLabel(i,"");
  dummy2->SetStats(0);
  dummy2->GetXaxis()->SetTickLength(0.01);
  dummy2->GetYaxis()->SetTickLength(0);
  dummy2->GetXaxis()->SetTitle("Width (GeV)");
  dummy2->GetXaxis()->SetNdivisions(505);
  dummy2->GetXaxis()->SetLabelFont(42);
  dummy2->GetXaxis()->SetLabelSize(0.07);
  dummy2->GetXaxis()->SetLabelOffset(-0.02);
  dummy2->GetXaxis()->SetTitleSize(0.08);
  dummy2->GetXaxis()->SetTitleOffset(0.5);
  dummy2->GetXaxis()->SetTitleFont(42);
  dummy2->GetYaxis()->SetNdivisions(505);
  dummy2->GetYaxis()->SetLabelSize(0.035);
  dummy2->GetYaxis()->SetTitleSize(0.045);
  dummy2->GetYaxis()->SetTitleOffset(1.1);
  dummy2->GetYaxis()->SetTitleFont(42);
  dummy2->GetZaxis()->SetLabelFont(42);
  dummy2->GetZaxis()->SetLabelSize(0.035);
  dummy2->GetZaxis()->SetTitleSize(0.035);
  dummy2->GetZaxis()->SetTitleFont(42);
  dummy2->Draw("");
  
  TPave *spave;
  TPave *fpave;
  for (int c=0; c<nCats+1; c++) {
		int cat = c;
		if (c == nCats-1-raiseVHdijet) cat = nCats-1;
		if (c > nCats-1-raiseVHdijet && c < nCats) cat = c-1;
    float ybin = nCats-c;
    float ybinmin = ybin-width;
    float ybinmax = ybin+width;
    
    double sEff;
    double fwhm;
    if (cat==nCats){
      sEff = sigEffs["all"];
      fwhm = fwhms["all"];
    }
    else {
      sEff = sigEffs[Form("cat%d",cat)];
      fwhm = fwhms[Form("cat%d",cat)];
    }
    spave = new TPave(0.,ybinmin,sEff,ybinmax);
    //spave = new TPave(-1.*sEff,ybinmin,sEff,ybinmax);
    spave->SetFillColor(kBlue-7);
    spave->SetLineColor(kBlue);
    spave->SetLineWidth(1);
    spave->SetBorderSize(1);
    spave->Draw();

    fpave = new TPave(0.,ybinmin,fwhm/2.35,ybinmax);
    //fpave = new TPave(fwhm/-2.35,ybinmin,fwhm/2.35,ybinmax);
    fpave->SetFillColor(kRed);
    fpave->SetFillStyle(3004);
    fpave->SetLineColor(kRed);
    fpave->SetLineWidth(1);
    fpave->SetBorderSize(1);
    fpave->Draw();
  }
  
  pave=new TPave(0.15,0.85,0.23,0.85+0.03,0,"NDC");
  pave->SetFillColor(kBlue-7);
  pave->SetLineColor(kBlue);
  pave->SetLineWidth(1);
  pave->SetBorderSize(1);
  pave->Draw();
  tex_m->SetTextSize(0.055);
  tex_m->DrawLatex(0.25,0.865,"#sigma_{eff}");

  pave=new TPave(0.60,0.85,0.68,0.85+0.03,0,"NDC");
  pave->SetFillColor(kRed);
  pave->SetFillStyle(3004);
  pave->SetLineColor(kRed);
  pave->SetLineWidth(1);
  pave->SetBorderSize(1);
  pave->Draw();
  tex_m->SetTextSize(0.045);
  tex_m->DrawLatex(0.7,0.865,"FWHM/2.35");
  
  TLine *sline = new TLine(0.,nCats-nIncCats+0.5,3.,nCats-nIncCats+0.5);
  sline->SetLineColor(kBlack);
  sline->SetLineWidth(2);
  sline->SetLineStyle(2);
  sline->Draw();

  TLine *sline2 = new TLine(0.,nCats-nIncCats-nVBFCats+0.5,3.,nCats-nIncCats-nVBFCats+0.5);
  sline2->SetLineColor(kBlack);
  sline2->SetLineWidth(2);
  sline2->SetLineStyle(2);
  sline2->Draw();

  TLine *sline3 = new TLine(0.,0.5,3.,0.5);
  sline3->SetLineColor(kBlack);
  sline3->SetLineWidth(2);
  sline3->SetLineStyle(9);
  sline3->Draw();

	/*
  TLine *sline4 = new TLine(0.,-0.5,0.,nCats+0.5);
  sline4->SetLineColor(kBlack);
  sline4->SetLineWidth(2);
  sline4->Draw();
	*/

  Width->RedrawAxis();

  if (doBkgAndData) {
    SOB->cd();
    SOB->Range(-14.67532,-1.75,11.2987,15.75);
    SOB->SetFillColor(0);
    SOB->SetBorderMode(0);
    SOB->SetBorderSize(2);
    SOB->SetTopMargin(0.16);
    SOB->SetLeftMargin(0.08);
    SOB->SetRightMargin(0.05);
    SOB->SetFrameBorderMode(0);
    SOB->SetFrameBorderMode(0);
		double sobMax=0.;
    for (int cat=0; cat<nCats; cat++) {
			if (sobVals[Form("cat%d",cat)]>sobMax) sobMax = sobVals[Form("cat%d",cat)];
		}
		if (sobVals["all"]>sobMax) sobMax = sobVals["all"];
		sobMax = ceil(sobMax*5.)/5.;
    TH2F *dummy3 = new TH2F("dummy3","",1,0.,sobMax,nCats+1,-0.5,nCats+0.5);
    for (int i=1; i<=dummy3->GetNbinsX(); i++) dummy3->GetYaxis()->SetBinLabel(i,"");
    dummy3->SetStats(0);
		dummy3->GetXaxis()->SetTickLength(0.01);
    dummy3->GetYaxis()->SetTickLength(0);
    dummy3->GetXaxis()->SetTitle("S/(S+B) in #pm #sigma_{eff}");
    dummy3->GetXaxis()->SetNdivisions(505);
    dummy3->GetXaxis()->SetLabelFont(42);
    dummy3->GetXaxis()->SetLabelSize(0.07);
    dummy3->GetXaxis()->SetLabelOffset(-0.02);
    dummy3->GetXaxis()->SetTitleSize(0.08);
    dummy3->GetXaxis()->SetTitleOffset(0.5);
    dummy3->GetXaxis()->SetTitleFont(42);
    dummy3->GetYaxis()->SetNdivisions(505);
    dummy3->GetYaxis()->SetLabelSize(0.035);
    dummy3->GetYaxis()->SetTitleSize(0.045);
    dummy3->GetYaxis()->SetTitleOffset(1.1);
    dummy3->GetYaxis()->SetTitleFont(42);
    dummy3->GetZaxis()->SetLabelFont(42);
    dummy3->GetZaxis()->SetLabelSize(0.035);
    dummy3->GetZaxis()->SetTitleSize(0.035);
    dummy3->GetZaxis()->SetTitleFont(42);
    dummy3->Draw("");
    
    TPave *sobpave;
    for (int c=0; c<nCats+1; c++) {
			int cat = c;
			if (c == nCats-1-raiseVHdijet) cat = nCats-1;
			if (c > nCats-1-raiseVHdijet && c < nCats) cat = c-1;
      float ybin = nCats-c;
      float ybinmin = ybin-width;
      float ybinmax = ybin+width;
      double sob;
      if (cat==nCats){
        sob = sobVals["all"];
      }
      else {
        sob = sobVals[Form("cat%d",cat)];
      }
      sobpave = new TPave(0.,ybinmin,sob,ybinmax);
      sobpave->SetFillColor(kRed-3);
      sobpave->SetLineColor(kRed-3);
      sobpave->SetLineWidth(2);
      sobpave->SetBorderSize(0);
      sobpave->Draw();
    }
    
    pave=new TPave(0.15,0.85,0.23,0.85+0.03,0,"NDC");
    pave->SetFillColor(kRed-3);
    pave->SetLineColor(kRed-3);
    pave->SetLineWidth(2);
    pave->SetBorderSize(0);
    pave->Draw();
    tex_m->SetTextSize(0.05);
    tex_m->DrawLatex(0.25,0.865,"S/(S+B) in #pm #sigma_{eff}");

		TLine *sbline = new TLine(0,nCats-nIncCats+0.5,sobMax,nCats-nIncCats+0.5);
    sbline->SetLineColor(kBlack);
    sbline->SetLineWidth(2);
    sbline->SetLineStyle(2);
    sbline->Draw();

		TLine *sbline2 = new TLine(0,nCats-nIncCats-nVBFCats+0.5,sobMax,nCats-nIncCats-nVBFCats+0.5);
    sbline2->SetLineColor(kBlack);
    sbline2->SetLineWidth(2);
    sbline2->SetLineStyle(2);
    sbline2->Draw();

    TLine *sbline3 = new TLine(0.,0.5,sobMax,0.5);
    sbline3->SetLineColor(kBlack);
    sbline3->SetLineWidth(2);
    sbline3->SetLineStyle(9);
    sbline3->Draw();

    SOB->RedrawAxis();
  }

  canv->cd();
  Plots->Draw();
  Width->Draw();
  if (doBkgAndData) SOB->Draw(); 
  tex_m->SetTextSize(0.045);
  tex_m->DrawLatex(0.11,0.935,Form("CMS Simulation    H#rightarrow#gamma#gamma (m_{H}=%d GeV/c^{2})",mh));
  canv->Print(Form("%s.pdf",outfname.c_str()));
  canv->Print(Form("%s.png",outfname.c_str()));
  gDirectory->DeleteAll();
}

void getConfigFromFile(TFile *inFile, bool &is2011, bool &splitVH, bool &isMassFac){
  
  TIter next(inFile->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())){
    if (string(key->ReadObj()->ClassName())=="TMacro") {
      //cout << key->ReadObj()->ClassName() << " : " << key->GetName() << endl;
      TMacro *macro = (TMacro*)inFile->Get(key->GetName());
      if (macro->GetName()==TString("statanalysis") || macro->GetName()==TString("spinanalysis")) isMassFac=false;
      if (macro->GetName()==TString("massfactorizedmvaanalysis")) isMassFac=true;
      TList *list = (TList*)macro->GetListOfLines();
      for (int l=0; l<list->GetSize(); l++){
        TObjString *obStr = (TObjString*)list->At(l);
        TString line(obStr->GetName());
        TString varName(line(0,line.First("=")));
        TString varVal(line(line.First("=")+1,line.Length()));
        if (varName=="splitwzh") splitVH = varVal.Atoi();
        if (varName=="dataIs2011") is2011 = varVal.Atoi();
      }
    }
  }
}

void getNCats(RooWorkspace *ws, int mh, int &ncats){
  ncats=0;
  RooDataSet *data = (RooDataSet*)ws->data(Form("sig_mass_m%d_cat0",mh));
  while (1) {
    data = (RooDataSet*)ws->data(Form("sig_mass_m%d_cat%d",mh,ncats));
    if (!data) break;
    else ncats++;
  }
}

// main function
void makeParametricSignalModelPlots(string sigFitFileName, string outPathName, int m_hyp=125, bool blind=true, bool doTable=false, string bkgdatFileName="0", int ncats=9, bool is2011=false, bool splitVH=false, bool isMassFac=true, int raiseVHdijet=0, bool spin=false, string spinProc="", string binnedSigFileName="", bool doCrossCheck=false, bool doMIT=false, bool rejig=false){

  gROOT->SetBatch();
  gStyle->SetTextFont(42);

  string newFileName;
  if (rejig) newFileName = runQuickRejig(sigFitFileName,ncats);
  else newFileName = sigFitFileName;

  TFile *hggFile = TFile::Open(newFileName.c_str());

  getConfigFromFile(hggFile,is2011,splitVH,isMassFac);
  
  string sqrts;
  if (is2011) sqrts="7TeV";
  else sqrts="8TeV";

  RooWorkspace *hggWS;
  if (is2011) hggWS = (RooWorkspace*)hggFile->Get(Form("wsig_%s",sqrts.c_str()));
  else        hggWS = (RooWorkspace*)hggFile->Get(Form("wsig_%s",sqrts.c_str()));
 
  if (!hggWS) {
    cerr << "Workspace is null" << endl;
    exit(1);
  }

  getNCats(hggWS,m_hyp,ncats);

  cout << "Configured options from file:" << endl;
  cout << "\t is2011: " << is2011 << endl;
  cout << "\t splitVH: " << splitVH << endl;
  cout << "\t isMassFac: " << isMassFac << endl;
  cout << "\t ncats: " << ncats << endl;
  
  RooRealVar *mass= (RooRealVar*)hggWS->var("CMS_hgg_mass");
  
  RooRealVar *mh = (RooRealVar*)hggWS->var("MH");
  mh->setVal(m_hyp);
  mass->setRange("higgsRange",m_hyp-20.,m_hyp+15.);

  map<string,string> labels;
	/*
  if (is2011) {
    if (isMassFac){
      labels.insert(pair<string,string>("cat0","BDT >= 0.89"));
      labels.insert(pair<string,string>("cat1","0.72 <= BDT <= 0.89"));
      labels.insert(pair<string,string>("cat2","0.55 <= BDT <= 0.72"));
      labels.insert(pair<string,string>("cat3","0.05 <= BDT <= 0.55"));
    }
    else {
      labels.insert(pair<string,string>("cat0","EBEB, min(R9) > 0.94"));
      labels.insert(pair<string,string>("cat1","EBEB, min(R9) < 0.94"));
      labels.insert(pair<string,string>("cat2","!EBEB, min(R9) > 0.94"));
      labels.insert(pair<string,string>("cat3","!EBEB, min(R9) < 0.94"));
    }
    labels.insert(pair<string,string>("cat4","BDT >= 0.05 VBF Tag"));
    labels.insert(pair<string,string>("all","All Categories Combined"));
  }
  else {
    if (isMassFac){
      labels.insert(pair<string,string>("cat0","BDT_{#gamma#gamma} >= 0.91"));
      labels.insert(pair<string,string>("cat1","0.79 <= BDT_{#gamma#gamma} <= 0.91"));
      labels.insert(pair<string,string>("cat2","0.49 <= BDT_{#gamma#gamma} <= 0.79"));
      labels.insert(pair<string,string>("cat3","-0.05 <= BDT_{#gamma#gamma} <= 0.49"));
    }
    else {
      labels.insert(pair<string,string>("cat0","EBEB, min(R9) > 0.94"));
      labels.insert(pair<string,string>("cat1","EBEB, min(R9) < 0.94"));
      labels.insert(pair<string,string>("cat2","!EBEB, min(R9) > 0.94"));
      labels.insert(pair<string,string>("cat3","!EBEB, min(R9) < 0.94"));
    }
    labels.insert(pair<string,string>("cat4","BDT_{jj} >= 0.985 Dijet Tag"));
    labels.insert(pair<string,string>("cat5","BDT_{jj} >= 0.93 Dijet Tag")); 
    labels.insert(pair<string,string>("cat6","BDT_{#gamma#gamma} >= -0.05 Muon Tag")); 
    labels.insert(pair<string,string>("cat7","BDT_{#gamma#gamma} >= -0.05 Electron Tag")); 
    labels.insert(pair<string,string>("cat8","BDT_{#gamma#gamma} >= -0.05 MET Tag")); 
    labels.insert(pair<string,string>("all","All Categories Combined"));
  }
	*/
	if (isMassFac) {
		labels.insert(pair<string,string>("cat0","Untagged 0"));
		labels.insert(pair<string,string>("cat1","Untagged 1"));
		labels.insert(pair<string,string>("cat2","Untagged 2"));
		labels.insert(pair<string,string>("cat3","Untagged 3"));
		if (is2011) {
			labels.insert(pair<string,string>("cat4","Dijet Tag 0"));
			labels.insert(pair<string,string>("cat5","Dijet Tag 1"));
			labels.insert(pair<string,string>("cat6","VH Lepton Tight"));
			labels.insert(pair<string,string>("cat7","VH Lepton Loose"));
			labels.insert(pair<string,string>("cat8","VH MET Tag"));     
			labels.insert(pair<string,string>("cat9","ttH Tags"));
			labels.insert(pair<string,string>("cat10","VH Dijet Tag"));
		}
		else {
			labels.insert(pair<string,string>("cat4","Untagged 4"));
			labels.insert(pair<string,string>("cat5","Dijet Tag 0"));
			labels.insert(pair<string,string>("cat6","Dijet Tag 1"));
			labels.insert(pair<string,string>("cat7","Dijet Tag 2"));
			labels.insert(pair<string,string>("cat8","VH Lepton Tight"));
			labels.insert(pair<string,string>("cat9","VH Lepton Loose"));
			labels.insert(pair<string,string>("cat10","VH MET Tag"));     
			labels.insert(pair<string,string>("cat11","ttH Leptonic Tag"));
			labels.insert(pair<string,string>("cat12","ttH Multijet Tag"));
			labels.insert(pair<string,string>("cat13","VH Dijet Tag"));
		}
		labels.insert(pair<string,string>("all","Combined"));
	}
	else {
		labels.insert(pair<string,string>("cat0","Untagged 0 (High p_{T})"));
		labels.insert(pair<string,string>("cat1","Untagged 1 (High p_{T})"));
		labels.insert(pair<string,string>("cat2","Untagged 2 (High p_{T})"));
		labels.insert(pair<string,string>("cat3","Untagged 3 (High p_{T})"));
		labels.insert(pair<string,string>("cat4","Untagged 4 (Low p_{T})"));
		labels.insert(pair<string,string>("cat5","Untagged 5 (Low p_{T})"));
		labels.insert(pair<string,string>("cat6","Untagged 6 (Low p_{T})"));
		labels.insert(pair<string,string>("cat7","Untagged 7 (Low p_{T})"));
		labels.insert(pair<string,string>("cat8","Dijet Tag Tight"));
		labels.insert(pair<string,string>("cat9","Dijet Tag Loose"));
		labels.insert(pair<string,string>("cat10","VH Lepton Tight"));
		labels.insert(pair<string,string>("cat11","VH Lepton Loose"));
		labels.insert(pair<string,string>("cat12","VH MET Tag"));
		if (is2011) {
			labels.insert(pair<string,string>("cat13","ttH Tags"));
			labels.insert(pair<string,string>("cat14","VH Dijet Tag"));
		}
		else {
			labels.insert(pair<string,string>("cat13","ttH Leptonic Tag"));
			labels.insert(pair<string,string>("cat14","ttH Multijet Tag"));
			labels.insert(pair<string,string>("cat15","VH Dijet Tag"));
		}
		labels.insert(pair<string,string>("all","Combined"));
	}
  if (spin){
    labels.clear();
    labels.insert(pair<string,string>("cat0","#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{|cos(#theta*)| < 0.2}"));
    labels.insert(pair<string,string>("cat1","#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.2 < |cos(#theta*)| < 0.375}"));
    labels.insert(pair<string,string>("cat2","#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.375 < |cos(#theta*)| < 0.55}"));
    labels.insert(pair<string,string>("cat3","#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.55 < |cos(#theta*)| < 0.75}"));
    labels.insert(pair<string,string>("cat4","#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.75 < |cos(#theta*)| < 0.1}"));
    labels.insert(pair<string,string>("cat5","#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{|cos(#theta*)| < 0.2}"));
    labels.insert(pair<string,string>("cat6","#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.2 < |cos(#theta*)| < 0.375}"));
    labels.insert(pair<string,string>("cat7","#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.375 < |cos(#theta*)| < 0.55}"));
    labels.insert(pair<string,string>("cat8","#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.55 < |cos(#theta*)| < 0.75}"));
    labels.insert(pair<string,string>("cat9","#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.75 < |cos(#theta*)| < 0.1}"));
    labels.insert(pair<string,string>("cat10","#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{|cos(#theta*)| < 0.2}"));
    labels.insert(pair<string,string>("cat11","#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.2 < |cos(#theta*)| < 0.375}"));
    labels.insert(pair<string,string>("cat12","#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.375 < |cos(#theta*)| < 0.55}"));
    labels.insert(pair<string,string>("cat13","#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.55 < |cos(#theta*)| < 0.75}"));
    labels.insert(pair<string,string>("cat14","#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.75 < |cos(#theta*)| < 0.1}"));
    labels.insert(pair<string,string>("cat15","#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{|cos(#theta*)| < 0.2}"));
    labels.insert(pair<string,string>("cat16","#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.2 < |cos(#theta*)| < 0.375}"));
    labels.insert(pair<string,string>("cat17","#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.375 < |cos(#theta*)| < 0.55}"));
    labels.insert(pair<string,string>("cat18","#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.55 < |cos(#theta*)| < 0.75}"));
    labels.insert(pair<string,string>("cat19","#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.75 < |cos(#theta*)| < 0.1}"));
    labels.insert(pair<string,string>("all","All Categories Combined"));
  }
  /*
  else {
    labels.insert(pair<string,string>("cat0","BDT >= 0.88"));
    labels.insert(pair<string,string>("cat1","0.71 <= BDT <= 0.88"));
    labels.insert(pair<string,string>("cat2","0.50 <= BDT <= 0.71"));
    labels.insert(pair<string,string>("cat3","-0.05 <= BDT <= 0.50"));
    labels.insert(pair<string,string>("cat4","BDT >= -0.05 Tight VBF"));
    labels.insert(pair<string,string>("cat5","BDT >= -0.05 Loose VBF")); 
    labels.insert(pair<string,string>("all","All Categories Combined"));
  }
  */

	for (int c=0; c<=ncats; c++){
		int cat = c;
		// VH rejig
		if (c == ncats-1-raiseVHdijet) cat = ncats-1;
		if (c > ncats-1-raiseVHdijet && c < ncats) cat = c-1;
		cout << "Raise: " << raiseVHdijet << endl;
		cout << "Old cat: " << c << " Old cat name: " << labels[Form("cat%d",c)] << endl;
		cout << "New cat: " << cat << " New cat name: " << labels[Form("cat%d",cat)] << endl;
	}

  map<string,RooDataSet*> dataSets;
  map<string,RooAddPdf*> pdfs;

  if (doMIT){
    dataSets = getGlobeData(hggWS,ncats,m_hyp);
    pdfs = getMITPdfs(hggWS,ncats,is2011);
  }
  else {
    dataSets = getGlobeData(hggWS,ncats,m_hyp);
    pdfs = getGlobePdfs(hggWS,ncats); 
  }
  
  printInfo(dataSets,pdfs);

  map<string,double> sigEffs;
  map<string,double> fwhms;
  
  system(Form("mkdir -p %s",outPathName.c_str()));
  system(Form("rm -f %s/animation.gif",outPathName.c_str()));
  for (map<string,RooDataSet*>::iterator dataIt=dataSets.begin(); dataIt!=dataSets.end(); dataIt++){
    pair<double,double> thisSigRange = getEffSigma(mass,pdfs[dataIt->first],m_hyp-10.,m_hyp+10.);
    //pair<double,double> thisSigRange = getEffSigBinned(mass,pdf[dataIt->first],m_hyp-10.,m_hyp+10);
    vector<double> thisFWHMRange = getFWHM(mass,pdfs[dataIt->first],dataIt->second,m_hyp-10.,m_hyp+10.);
    sigEffs.insert(pair<string,double>(dataIt->first,(thisSigRange.second-thisSigRange.first)/2.));
    fwhms.insert(pair<string,double>(dataIt->first,thisFWHMRange[1]-thisFWHMRange[0]));
    if (doCrossCheck) performClosure(mass,pdfs[dataIt->first],dataIt->second,Form("%s/closure_%s.pdf",outPathName.c_str(),dataIt->first.c_str()),m_hyp-10.,m_hyp+10.,thisSigRange.first,thisSigRange.second);
    Plot(mass,dataIt->second,pdfs[dataIt->first],thisSigRange,thisFWHMRange,labels[dataIt->first],Form("%s/%s",outPathName.c_str(),dataIt->first.c_str()));
  }
  
  map<string,pair<double,double> > bkgVals;
  map<string,vector<double> > sigVals;
  map<string,pair<double,double> > datVals;
  map<string,double> sobVals;

  // make PAS table
  if (doTable) {
    bool doBkgAndData=false;
    TFile *bkgFile;
    RooWorkspace *bkgWS;
    if (bkgdatFileName!="0"){
      bkgFile = TFile::Open(bkgdatFileName.c_str());
      bkgWS = (RooWorkspace*)bkgFile->Get("cms_hgg_workspace");
			if (!bkgWS) {
				bkgWS = (RooWorkspace*)bkgFile->Get("multipdf");
			}
			if (!bkgWS) {
				cout << "Background workspace is neither normal hgg or multipdf. Bailing!" << endl;
				exit(1);
			}
      doBkgAndData=true;
    }
    // keep track of sums
    pair<double,double> bkgTotal(0.,0.);
    pair<double,double> datTotal(0.,0.);
    pair<double,double> sobTotal(0.,0.);
    vector<double> sigTotal;
    int nprocs=5;
    if (splitVH) nprocs+=1;
    for (int i=0; i<6; i++) sigTotal.push_back(0.);
    for (int cat=0; cat<ncats; cat++){
      if (doBkgAndData) {
        bkgVals.insert(pair<string,pair<double,double> >(Form("cat%d",cat),bkgEvPerGeV(bkgWS,m_hyp,cat,bkgTotal,is2011)));
        datVals.insert(pair<string,pair<double,double> >(Form("cat%d",cat),datEvents(bkgWS,m_hyp,cat,datTotal)));
        sobVals.insert(pair<string,double>(Form("cat%d",cat),sobInEffSigma(hggWS,bkgWS,m_hyp,cat,sigEffs[Form("cat%d",cat)],sobTotal,splitVH,is2011)));
      }
      sigVals.insert(pair<string,vector<double> >(Form("cat%d",cat),sigEvents(hggWS,m_hyp,cat,binnedSigFileName,sigTotal,splitVH,spinProc)));
    }
    bkgTotal.second = sqrt(bkgTotal.second);
    bkgVals.insert(pair<string,pair<double,double> >("all",bkgTotal));
    sigVals.insert(pair<string,vector<double> > ("all",sigTotal));
    datVals.insert(pair<string,pair<double,double> >("all",datTotal));
    sobVals.insert(pair<string,double>("all",sobTotal.first/(sobTotal.first+sobTotal.second)));
    if (doBkgAndData) bkgFile->Close();
    
    FILE *file = fopen(Form("%s/table.tex",outPathName.c_str()),"w");
    FILE *nfile = fopen(Form("%s/table.txt",outPathName.c_str()),"w");
    if (splitVH) {
      fprintf(nfile,"---------------------------------------------------------------------------------------------------------------\n");
      fprintf(nfile,"Cat              SigY    ggh    vbf    wh     zh     tth   sEff  FWHM  FWHM/2.35  BkgEv/GeV    Data  DataEv/GeV\n");
      fprintf(nfile,"---------------------------------------------------------------------------------------------------------------\n");
    }
    else {
      fprintf(nfile,"---------------------------------------------------------------------------------------------------------------\n");
      fprintf(nfile,"Cat              SigY    ggh    vbf    wh     zh     tth   sEff  FWHM  FWHM/2.35  BkgEv/GeV    Data  DataEv/GeV\n");
      fprintf(nfile,"---------------------------------------------------------------------------------------------------------------\n");
    }
    for (int c=0; c<=ncats; c++){
			int cat = c;
			if (c == ncats-1-raiseVHdijet) cat = ncats-1;
			if (c > ncats-1-raiseVHdijet && c < ncats) cat = c-1;
      
			string catString;
      if (cat==ncats) catString = "all";
      else catString = Form("cat%d",cat);
			string thisCatName = labels[catString];
      // signal
      vector<double> sigs = sigVals[catString];
      // txt file
      fprintf(nfile,"%15s  ",thisCatName.c_str());
      fprintf(nfile,"%5.1f  ",sigs[0]);
      fprintf(nfile,"%4.1f%%  ",100.*sigs[1]/sigs[0]);
      fprintf(nfile,"%4.1f%%  ",100.*sigs[2]/sigs[0]);
      fprintf(nfile,"%4.1f%%  ",100.*sigs[3]/sigs[0]);
      fprintf(nfile,"%4.1f%%  ",100.*sigs[4]/sigs[0]);
      if (splitVH) fprintf(nfile,"%4.1f%%  ",100.*sigs[5]/sigs[0]);
      fprintf(nfile,"%4.2f  ",sigEffs[catString]);
      fprintf(nfile,"%4.2f  ",fwhms[catString]);
      fprintf(nfile,"%4.2f   ",fwhms[catString]/2.35);
      // tex file
      fprintf(file,"&  %15s  ",thisCatName.c_str());
      fprintf(file,"&  %5.1f  ",sigs[0]);
      fprintf(file,"&  %4.1f\\%%  ",100.*sigs[1]/sigs[0]);
      fprintf(file,"&  %4.1f\\%%  ",100.*sigs[2]/sigs[0]);
      fprintf(file,"&  %4.1f\\%%  ",100.*sigs[3]/sigs[0]);
      fprintf(file,"&  %4.1f\\%%  ",100.*sigs[4]/sigs[0]);
      if (splitVH) fprintf(file,"&  %4.1f\\%%  ",100.*sigs[5]/sigs[0]);
      fprintf(file,"&  %4.2f  ",sigEffs[catString]);
      fprintf(file,"&  %4.2f  ",fwhms[catString]);
      fprintf(file,"&  %4.2f  ",fwhms[catString]/2.35);
      // bkg + data
      if (doBkgAndData) {
        pair<double,double> bkg = bkgVals[catString];
        pair<double,double> dat = datVals[catString];
        // txt file
        fprintf(nfile,"%6.1f +/- %3.1f  ",bkg.first,bkg.second);
        fprintf(nfile,"%6.0f    ",dat.first);
        if (blind) fprintf(nfile,"%7s","----");
        else fprintf(nfile,"%7.1f  ",dat.second);
        // tex file
        fprintf(file,"&  %6.1f & $\\pm$ %3.1f ",bkg.first,bkg.second);
        fprintf(file,"&  %7.1f  ",dat.first);
        if (blind) fprintf(file,"& %7s","---- \\tabularnewline ");
        else fprintf(file,"&  %7.1f  \\tabularnewline ",dat.second);
      }
      fprintf(nfile,"\n");
      fprintf(file,"\n");
    }
    fclose(nfile);
    fclose(file);
    makeSignalCompositionPlot(ncats,labels,sigVals,sigEffs,fwhms,sobVals,Form("%s/signalComposition",outPathName.c_str()),m_hyp,doBkgAndData,splitVH,isMassFac,is2011,raiseVHdijet);
    system(Form("cat %s/table.txt",outPathName.c_str()));
    cout << "-->" << endl;
    cout << Form("--> LaTeX version of this table has been written to %s/table.tex",outPathName.c_str()) << endl;
    if (doBkgAndData) bkgFile->Close();
  }

  hggFile->Close();
}
