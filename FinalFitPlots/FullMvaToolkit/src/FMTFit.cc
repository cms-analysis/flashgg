#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLine.h"
#include "TBox.h"
#include "TLatex.h"

#include "RooDataSet.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

#include "../interface/FMTFit.h"

using namespace std;
using namespace RooFit;
FMTFit::FMTFit(TFile *tFile, TFile *outFile):
	FMTBase(),
	blind_(false),
	plot_(true),
	infilename_(tFile->GetName()),
	outfilename_(outFile->GetName())
{
  gROOT->SetStyle("Plain");
  //r1 = new RooRealVar("r1","r1",-0.02,-10.,0.); 
  //r2 = new RooRealVar("r2","r2",-0.02,-10.,0.); 
  //r3 = new RooRealVar("r3","r3",-1.,-20.,0.); 
  //f1 = new RooRealVar("f1","f1",0.5,0.01,1.); 
  //f2 = new RooRealVar("f2","f2",0.001,0.001,0.49); 
  nBkgInSigReg = new RooRealVar("nbis","nbis",10,0,100000);
	inWS = (RooWorkspace*)tFile->Get("cms_hgg_workspace");
	outWS = new RooWorkspace("cms_hgg_workspace");
  mass_var = (RooRealVar*)inWS->var("CMS_hgg_mass");
  
  //RooExponential *e1 = new RooExponential("e1","e1",*mass_var,*r1);
  //RooExponential *e2 = new RooExponential("e2","e2",*mass_var,*r2);
  //fit = new RooAddPdf("data_exp_model","data_exp_model",*e1,*e2,*f1);
  //if (is2011){	// Single power law for 2011???
    r1 = new RooRealVar("r1","r1",-0.02,-10.,0.); 
    r2 = new RooRealVar("r2","r2",-0.02,-10.,0.); 
    r3 = new RooRealVar("r3","r3",-1.,-20.,0.); 
    f1 = new RooRealVar("f1","f1",0.3,0.001,1.); 
    f2 = new RooRealVar("f2","f2",0.01,0.001,0.49); 
    //fit = new RooGenericPdf("data_pow_model","data_pow_model","@1*TMath::Pow(@0,@2)+(1.-@1)*TMath::Pow(@0,@3)",RooArgList(*mass_var,*f1,*r1,*r2));
    fit2011 = new RooGenericPdf("data_pow_model","data_pow_model","TMath::Power(@0,@1)",RooArgList(*mass_var,*r1));
  // 2 Laurent series for 2012
    fit2012 = new RooGenericPdf("data_pow_model","data_pow_model","@1*TMath::Power(@0,-4.)+(1.-@1)*TMath::Power(@0,-5.)",RooArgList(*mass_var,*f1));
  

	// get data and combine all cats
	cout << "Looking for datasets....." << endl;
	/*
  data = (RooDataSet*)((RooDataSet*)inWS->data("data_mass_cat0"))->Clone("data_mass");
	for (int cat=0; cat<getNcats(); cat++){
    RooDataSet *temp = (RooDataSet*)inWS->data(Form("data_mass_cat%d",cat));
    outWS->import(*temp);
		if (cat>0) data->append(*((RooDataSet*)inWS->data(Form("data_mass_cat%d",cat))));
	}
  */
  data = (RooDataSet*)inWS->data("data_mass");
	if (!outWS->data("data_mass")) outWS->import(*data);
 
  //g_counter=0;

   readFitsFromFile = false;
    //normGraph = new TGraph();
    //normGraph->SetName("NormGraph");
}


FMTFit::FMTFit(TFile *tFile, TFile *outFile, double intLumi, bool is2011, int mHMinimum, int mHMaximum, double mHStep, double massMin, double massMax, int nDataBins, double signalRegionWidth, double sidebandWidth, int numberOfSidebands, int numberOfSidebandsForAlgos, int numberOfSidebandGaps, double massSidebandMin, double massSidebandMax, int nIncCategories, bool includeVBF, int nVBFCategories, bool includeLEP, int nLEPCategories, vector<string> systematics, bool rederiveOptimizedBinEdges, vector<map<int,vector<double> > > AllBinEdges, bool verbose):
	
	FMTBase(intLumi,is2011,mHMinimum, mHMaximum, mHStep, massMin, massMax, nDataBins, signalRegionWidth, sidebandWidth, numberOfSidebands, numberOfSidebandsForAlgos, numberOfSidebandGaps, massSidebandMin, massSidebandMax, nIncCategories, includeVBF, nVBFCategories, includeLEP, nLEPCategories, systematics, rederiveOptimizedBinEdges, AllBinEdges, verbose),
	blind_(false),
	plot_(true),
	infilename_(tFile->GetName()),
	outfilename_(outFile->GetName())
{
  gROOT->SetStyle("Plain");
  nBkgInSigReg = new RooRealVar("nbis","nbis",10,0,100000);
	inWS = (RooWorkspace*)tFile->Get("cms_hgg_workspace");
	outWS = new RooWorkspace("cms_hgg_workspace");
  mass_var = (RooRealVar*)inWS->var("CMS_hgg_mass");
  
  //RooExponential *e1 = new RooExponential("e1","e1",*mass_var,*r1);
  //RooExponential *e2 = new RooExponential("e2","e2",*mass_var,*r2);
  //fit = new RooAddPdf("data_exp_model","data_exp_model",*e1,*e2,*f1);
	// get data and combine all cats
	cout << "Looking for datasets....." << endl;
	/*
  data = (RooDataSet*)((RooDataSet*)inWS->data("data_mass_cat0"))->Clone("data_mass");
	for (int cat=0; cat<getNcats(); cat++){
    RooDataSet *temp = (RooDataSet*)inWS->data(Form("data_mass_cat%d",cat));
    outWS->import(*temp);
		if (cat>0) data->append(*((RooDataSet*)inWS->data(Form("data_mass_cat%d",cat))));
	}
  */
    r1 = new RooRealVar("r1","r1",-0.02,-10.,0.); 
    r2 = new RooRealVar("r2","r2",-0.02,-10.,0.); 
    r3 = new RooRealVar("r3","r3",-1.,-20.,0.); 
    f1 = new RooRealVar("f1","f1",0.4,0.001,1.); 
    f2 = new RooRealVar("f2","f2",0.001,0.001,0.49); 
    //fit = new RooGenericPdf("data_pow_model","data_pow_model","@1*TMath::Pow(@0,@2)+(1.-@1)*TMath::Pow(@0,@3)",RooArgList(*mass_var,*f1,*r1,*r2));
    fit2011 = new RooGenericPdf("data_pow_model","data_pow_model","TMath::Power(@0,@1)",RooArgList(*mass_var,*r1));
    fit2012 = new RooGenericPdf("data_pow_model","data_pow_model","@1*TMath::Power(@0,-4.)+(1.-@1)*TMath::Power(@0,-5.)",RooArgList(*mass_var,*f1));

  data = (RooDataSet*)inWS->data("data_mass");
	if (!outWS->data("data_mass")) outWS->import(*data);
 
  //g_counter=0;

   readFitsFromFile = false;
    //normGraph = new TGraph();
    //normGraph->SetName("NormGraph");
}

FMTFit::~FMTFit(){
	delete r1;
	delete r2;
	delete f1;
	delete nBkgInSigReg;
	delete outWS;
}

void FMTFit::SetNormGraph(TFile *nFile){

	//delete normGraph;
        
	std::cout << "OK "<<std::endl;
	normGraph = (TGraph*) (nFile->Get("NormGraph")->Clone());
	
  	readFitsFromFile = true;
	std::cout << "OK "<< normGraph<<std::endl;
}

pair<double,double> FMTFit::FitPow(double mass){
  
  gROOT->SetBatch();
  if (mass<(getmHMinimum()-(getmHStep()/2.)) || mass>(getmHMaximum()+(getmHStep()/2.))) {
    cout << Form("%3.1f invalid mass. Not in range [",mass) << getmHMinimum() << "," << getmHMaximum() << "]" << endl;
    exit(1);
  }

  if (readFitsFromFile){
	
	RooRealVar *temp = new RooRealVar(Form("NBkgInSignal_mH%3.1f",mass),"t",10,0,1e6);
	double normVal = normGraph->Eval(mass);
	temp->setVal(normVal);
	outWS->import(*temp,RecycleConflictNodes());
	gDirectory->Cd(Form("%s:/",outfilename_.c_str()));
	outWS->Write();
	//normGraph->Write();

  	return pair<double,double>(normGraph->Eval(mass),0);
  }
  //fitRes = fit->fitTo(*data,Save(true));
  if (is2011_) fit=fit2011;
  else fit=fit2012;

	// set up fit function
	r1->SetName(Form("r1_%3.1f",mass));
       r2->SetName(Form("r2_%3.1f",mass));
       r3->SetName(Form("r3_%3.1f",mass));
      f1->SetName(Form("f1_%3.1f",mass));
      f2->SetName(Form("f2_%3.1f",mass));
	fit->SetName(Form("data_exp_model_%3.1f",mass));

	// set up fit region
	double mLow = getmassMin();
	double mHigh = getmassMax();
  double sidebL = mass*(1-getsignalRegionWidth());
  double sidebH = mass*(1+getsignalRegionWidth());
  mass_var->setRange(Form("rangeLow_m%3.1f",mass),mLow,sidebL);
  mass_var->setRange(Form("rangeHig_m%3.1f",mass),sidebH,mHigh);
  mass_var->setRange(Form("sigReg_m%3.1f",mass),sidebL,sidebH);
  mass_var->setRange("rangeAll",mLow,mHigh);

	data->Print();
	cout << data->GetName() << " " << data->numEntries() << endl;
	
  fitRes = fit->fitTo(*data,Range(Form("rangeLow_m%3.1f,rangeHig_m%3.1f",mass,mass)),Save(true),PrintEvalErrors(-1));

	// make plot
	if (plot_) Plot(mass);
  
	// integral in sig region
  RooAbsReal *integral = fit->createIntegral(*mass_var,NormSet(*mass_var),Range(Form("sigReg_m%3.1f",mass)));
  // comb integral in two sideband regions 
  RooAbsReal *sidebandInt = fit->createIntegral(*mass_var,NormSet(*mass_var),Range(Form("rangeLow_m%3.1f,rangeHig_m%3.1f",mass,mass)));
  double tempEv = data->sumEntries(Form("CMS_hgg_mass>=%3.1f && CMS_hgg_mass<%3.1f",mLow,sidebL))+data->sumEntries(Form("CMS_hgg_mass>%3.1f && CMS_hgg_mass<=%3.1f",sidebH,mHigh));
  RooConstVar sidebandNevents(Form("sbEvents_m%3.1f",mass),Form("sbEvents_m%3.1f",mass),tempEv);
  RooFormulaVar normIntVar(Form("normIntVar_m%3.1f",mass),Form("normIntVar_m%3.1f",mass),"@0*@1/@2",RooArgSet(sidebandNevents,*integral,*sidebandInt));

  double result     = normIntVar.getVal();
  double fullError  = normIntVar.getPropagatedError(*fitRes);
  
	RooRealVar *temp = new RooRealVar(Form("NBkgInSignal_mH%3.1f",mass),"t",10,0,1e6);
	temp->setVal(result);
	temp->setError(fullError);
	
	if (verbose_) cout << "Transfer to workspace" << endl;
  RooRealVar *intL = (RooRealVar*)inWS->var("IntLumi");
  if (!intL) {
    intL = new RooRealVar("IntLumi","IntLumi",intLumi_);
    outWS->import(*intL);
  }
	/*
  for (vector<double>::iterator mH=MHMasses_.begin(); mH!=MHMasses_.end(); mH++){
		if (TMath::Abs(*mH-mass)<mHStep_/2.) continue;
		else {
			temp = (RooRealVar*)inWS->var(Form("NBkgInSignal_mH%3.1f",mass));
			if (temp) outWS->import(*temp);
		}
	}
  */
  if (verbose_) { 
    outWS->allVars().Print();
    temp->Print();
  }
	outWS->import(*temp,RecycleConflictNodes());
	gDirectory->Cd(Form("%s:/",outfilename_.c_str()));
	outWS->Write();
	if (verbose_) outWS->allVars().Print();
	//delete fit;
	//normGraph->SetPoint(g_counter,mass,result);

  // internal Counter 
 // g_counter++;
  return pair<double,double>(result,fullError);
/*
  nBkgInSigReg = outWS->var(Form("NBkgInSignal_mH%3.1f",mass));
  if (nBkgInSigReg!=NULL){
    nBkgInSigReg = (RooRealVar*)outWS->var(Form("NBkgInSignal_mH%3.1f",mass));
    nBkgInSigReg->setVal(result);
    nBkgInSigReg->setError(fullError);
		cout << "---- var " << nBkgInSigReg->GetName() << " already found - updating ------" << endl;
    outWS->Write(outWS->GetName(),TObject::kWriteDelete);
  }
  else {
    RooRealVar *temp = new RooRealVar(Form("NBkgInSignal_mH%3.1f",mass),"t",10,0,1e6);
    temp->setVal(result);
    temp->setError(fullError);
    outWS->import(*temp);
		cout << "---- writing var " << temp->GetName() << " to workspace -----" << endl;
    outWS->Write(outWS->GetName(),TObject::kWriteDelete);
    delete temp;
  }
	*/
}

void FMTFit::Plot(double mass){
   
    cout << "Plotting fit...." << endl;
    RooPlot *frame = mass_var->frame(Title(Form("Mass fit for %3.1f",mass)));
    frame->SetTitle("");
    frame->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");
    frame->GetYaxis()->SetTitle("Events / (0.5 GeV)");
    frame->GetYaxis()->SetTitleOffset(1.2);
    mass_var->setRange("unblindReg_1",getmassMin(),110);
    mass_var->setRange("unblindReg_2",150,getmassMax());
		if (blind_) {
      data->plotOn(frame,Binning(getnDataBins()),CutRange("unblindReg_1"));
      data->plotOn(frame,Binning(getnDataBins()),CutRange("unblindReg_2"));
      data->plotOn(frame,Binning(getnDataBins()),Invisible());
		}
    else data->plotOn(frame,Binning(getnDataBins()));
    //fit->plotOn(frame,LineColor(kRed),Range("rangeAll"),NormRange("rangeAll"));
    fit->plotOn(frame,Range(Form("rangeLow_m%3.1f,rangeHig_m%3.1f",mass,mass)),NormRange(Form("rangeLow_m%3.1f,rangeHig_m%3.1f",mass,mass)));
    frame->SetMinimum(0.0001);
    TCanvas *c1 = new TCanvas();
		frame->Draw();
		// make signal reg and sideband boxes
		TLine l1, l2;
    l1.SetLineColor(kRed);
    l1.SetLineWidth(2);
    l2.SetLineColor(kBlue);
    l2.SetLineWidth(2);
    l2.SetLineStyle(1);
    TBox b1, b2;
    b1.SetFillColor(kRed-9);
    b2.SetFillColor(kBlue-9);
    b1.SetFillStyle(1001);
    b2.SetFillStyle(1001);
    b1.IsTransparent();
    b2.IsTransparent();
		double sidebL = mass*(1-getsignalRegionWidth());
		double sidebH = mass*(1+getsignalRegionWidth());
    b1.DrawBox(sidebL,frame->GetMinimum(),sidebH,frame->GetMaximum());
    l1.DrawLine(sidebL,frame->GetMinimum(),sidebL,frame->GetMaximum());
    l1.DrawLine(sidebH,frame->GetMinimum(),sidebH,frame->GetMaximum());
    vector<double> lowEdges = getLowerSidebandEdges(mass);
    vector<double> highEdges = getUpperSidebandEdges(mass);
    for (unsigned int i=0; i<lowEdges.size(); i++) {
      if (i>0) b2.DrawBox(lowEdges[i-1],frame->GetMinimum(),lowEdges[i],frame->GetMaximum());
      l2.DrawLine(lowEdges[i],frame->GetMinimum(),lowEdges[i],frame->GetMaximum());
    }
    for (unsigned int i=0; i<highEdges.size(); i++) {
      if (i>0) b2.DrawBox(highEdges[i-1],frame->GetMinimum(),highEdges[i],frame->GetMaximum());
      l2.DrawLine(highEdges[i],frame->GetMinimum(),highEdges[i],frame->GetMaximum()); 
    }
		frame->Draw("same");
		TLatex *text = new TLatex();
		text->SetTextSize(0.04);
		text->SetNDC();
		text->DrawLatex(0.68,0.85,"CMS preliminary");
    if (getis2011()) text->DrawLatex(0.75,0.78,"#sqrt{s} = 7 TeV");
    else text->DrawLatex(0.75,0.78,"#sqrt{s} = 8 TeV");
		text->DrawLatex(0.75,0.71,Form("L = %3.1f fb^{-1}",0.001*double(intLumi_)));
    if (blind_) text->DrawLatex(0.67,0.64,"Blinded: [110,150]");
    c1->SaveAs(Form("plots/pdf/fit_m%3.1f.pdf",mass));
    c1->SaveAs(Form("plots/macro/fit_m%3.1f.C",mass));
    c1->SaveAs(Form("plots/png/fit_m%3.1f.png",mass));

    delete frame;
    delete c1;
}

void FMTFit::redoFit(double mass){
 

    pair<double,double> dummyVar = FitPow(mass);
    if (verbose_) cout << dummyVar.first << " +/- " << dummyVar.second << endl;
}

bool FMTFit::getblind(){
	return blind_;
}

bool FMTFit::getplot(){
	return plot_;
}

void FMTFit::setblind(bool blind){
	blind_=blind;
}

void FMTFit::setplot(bool plot){
	plot_=plot;
}



