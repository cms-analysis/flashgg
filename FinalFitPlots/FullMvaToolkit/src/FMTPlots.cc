#include <cmath>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>

#include "TROOT.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMacro.h"
#include "TObjString.h"
#include "TF1.h"
#include "TMatrix.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"

#include "../interface/FMTPlots.h"

using namespace std;

FMTPlots::FMTPlots(string filename):
	FMTBase(),runSB_(false)
{
  tFile = TFile::Open(filename.c_str(),"UPDATE");
  gStyle->SetOptStat(0);
}

FMTPlots::FMTPlots(string filename, bool runSB, double intLumi, bool is2011, int mHMinimum, int mHMaximum, double mHStep, double massMin, double massMax, int nDataBins, double signalRegionWidth, double sidebandWidth, int numberOfSidebands, int numberOfSidebandsForAlgos, int numberOfSidebandGaps, double massSidebandMin, double massSidebandMax, int nIncCategories, bool includeVBF, int nVBFCategories, bool includeLEP, int nLEPCategories, vector<string> systematics, bool rederiveOptimizedBinEdges, vector<map<int, vector<double> > > AllBinEdges, bool blind, bool verbose):

	FMTBase(intLumi, is2011, mHMinimum, mHMaximum, mHStep, massMin, massMax, nDataBins, signalRegionWidth, sidebandWidth, numberOfSidebands, numberOfSidebandsForAlgos, numberOfSidebandGaps, massSidebandMin, massSidebandMax, nIncCategories, includeVBF, nVBFCategories, includeLEP, nLEPCategories, systematics, rederiveOptimizedBinEdges, AllBinEdges, verbose),
  blind_(blind),
	runSB_(runSB)
{
  tFile = TFile::Open(filename.c_str(),"UPDATE");
  gStyle->SetOptStat(0);
}

FMTPlots::~FMTPlots(){
  tFile->Close();
}

void FMTPlots::Setup(){
  normalizer = new Normalization_8TeV();
  if ( is2011_ ) normalizer->Init(7);
  else  normalizer->Init(8);
}

TH1F* FMTPlots::linearBin(TH1F* hist){
  
  int nBins = hist->GetNbinsX();
  TH1F *h = new TH1F(Form("lin_%s",hist->GetName()),"",nBins,0,nBins);
	if (verbose_) cout << "Bin edges: "; 
  for (int b=1; b<=nBins; b++){
    h->SetBinContent(b,hist->GetBinContent(b));
		h->SetBinError(b,hist->GetBinError(b));
		if (verbose_) cout << hist->GetBinLowEdge(b) << "   ";
    if (hist->GetBinLowEdge(b)<1.) h->GetXaxis()->SetBinLabel(b,Form("BDT Bin %d",b));
    else if (hist->GetBinLowEdge(b)<2.) h->GetXaxis()->SetBinLabel(b,"Di-jet");
    else if (hist->GetBinLowEdge(b)<3.) h->GetXaxis()->SetBinLabel(b,"Di-lep");
  }
	if (verbose_) cout << endl;
	h->GetXaxis()->SetNdivisions(nBins);
  return h;
}

void FMTPlots::makePlots(TH1F* dataH, TH1F *sigH, TH1F *bkgH, TH1F *bkgmcH, vector<TH1F*> dataSideLowH, vector<TH1F*> dataSideHighH, vector<TH1F*> bkgSideLowH, vector<TH1F*> bkgSideHighH, vector<pair<TH1F*,TH1F*> > sigSystH, TH1F* lowInterpH, TH1F* highInterpH, double mh){

  TH1F *data = linearBin(dataH);
  TH1F *sig = linearBin(sigH);
	// Can just clone background - as it is already linear binned on correction (still need labels)
  TH1F *bkg = (TH1F*)bkgH->Clone();
	for (int bin=1; bin<=bkg->GetNbinsX(); bin++) bkg->GetXaxis()->SetBinLabel(bin,data->GetXaxis()->GetBinLabel(bin));
  TH1F *bkgmc = linearBin(bkgmcH);
  TH1F *sig3 = (TH1F*)sig->Clone();
  sig3->Scale(3.);
  TH1F *sig5 = (TH1F*)sig->Clone();
  sig5->Scale(5.);
  TH1F *lowInterp = linearBin(lowInterpH);
  TH1F *highInterp = linearBin(highInterpH);

  vector<TH1F*> dataSideLow;
  vector<TH1F*> dataSideHigh;
  vector<TH1F*> bkgSideLow;
  vector<TH1F*> bkgSideHigh;
  vector<pair<TH1F*,TH1F*> > sigSyst;
  
  for (vector<TH1F*>::iterator it = dataSideLowH.begin(); it != dataSideLowH.end(); it++) dataSideLow.push_back(linearBin(*it));
  for (vector<TH1F*>::iterator it = dataSideHighH.begin(); it != dataSideHighH.end(); it++) dataSideHigh.push_back(linearBin(*it));
  for (vector<TH1F*>::iterator it = bkgSideLowH.begin(); it != bkgSideLowH.end(); it++) bkgSideLow.push_back(linearBin(*it));
  for (vector<TH1F*>::iterator it = bkgSideHighH.begin(); it != bkgSideHighH.end(); it++) bkgSideHigh.push_back(linearBin(*it));
  for (vector<pair<TH1F*,TH1F*> >::iterator it = sigSystH.begin(); it != sigSystH.end(); it++) sigSyst.push_back(pair<TH1F*,TH1F*>(linearBin(it->first),linearBin(it->second)));

  plotOutput(data,bkg,sig,sig3,sig5,mh);
  plotSidebands(bkg,dataSideLow,dataSideHigh,bkgmc,bkgSideLow,bkgSideHigh,mh);
  plotSystFracs(sig,sigSyst,mh);
  plotInterpolation(sig,lowInterp,highInterp,mh);
	if (runSB_) plotByMH("fake-shape-cards",mh,data,sig,bkg);
  delete data;
  delete sig;
  delete bkg;
  delete bkgmc;
  delete sig3;
  delete sig5;
  delete lowInterp;
  delete highInterp;
}

double FMTPlots::quadInterp(double C, double X1,double X2,double X3,double Y1,double Y2,double Y3){

	// Use the 3 points to determine a,b,c
	TF1 func("f1","[0]*x*x+[1]*x+[2]",-5,5);

	double entries[9];
	entries[0]=X1*X1; entries[1]=X1; entries[2]=1;
	entries[3]=X2*X2; entries[4]=X2; entries[5]=1;
	entries[6]=X3*X3; entries[7]=X3; entries[8]=1;
	
	//create the Matrix;
	TMatrixD M(3,3);
	M.SetMatrixArray(entries);
	M.Invert();

	double a = M(0,0)*Y1+M(0,1)*Y2+M(0,2)*Y3;
	double b = M(1,0)*Y1+M(1,1)*Y2+M(1,2)*Y3;
	double c = M(2,0)*Y1+M(2,1)*Y2+M(2,2)*Y3;

	func.SetParameter(0,a);
	func.SetParameter(1,b);
	func.SetParameter(2,c);

	//delete entries;
	return func.Eval(C);
}

void FMTPlots::plotSystFracs(TH1F *sig, vector<pair<TH1F*,TH1F*> > sigSyst, double mh){
 	
	sig->SetLineColor(kBlack);
	sig->SetLineWidth(2);
  
	TCanvas *canv = new TCanvas();
	
	for (vector<pair<TH1F*,TH1F*> >::iterator it = sigSyst.begin(); it != sigSyst.end(); it++){
		(it->first)->SetLineColor(kBlue);
		(it->first)->SetLineWidth(2);
		(it->second)->SetLineColor(kRed);
		(it->second)->SetLineWidth(2);
		TH1F *tempUp = (TH1F*)(it->first)->Clone();
		TH1F *tempDown = (TH1F*)(it->second)->Clone();
		for (int bin=1; bin<=sig->GetNbinsX(); bin++){
			tempUp->SetBinContent(bin,quadInterp(1.,-3.,0.,3.,(it->second)->GetBinContent(bin),sig->GetBinContent(bin),(it->first)->GetBinContent(bin)));
			tempDown->SetBinContent(bin,quadInterp(-1.,-3.,0.,3.,(it->second)->GetBinContent(bin),sig->GetBinContent(bin),(it->first)->GetBinContent(bin)));
		}
		tempUp->SetLineStyle(kDashed);
    tempUp->SetLineColor(kBlue);
		tempDown->SetLineStyle(kDashed);
    tempDown->SetLineColor(kRed);
		(it->first)->Draw();
		sig->Draw("same");
		(it->second)->Draw("same");
		tempUp->Draw("same");
		tempDown->Draw("same");
		string temp((it->second)->GetName());
		string name = temp.substr(temp.find(".")+3,temp.find("Down")-temp.find(".")-3);
		canv->Print(Form("plots/png/systs_mH_%3.1f_%s.png",mh,name.c_str()));
		canv->Print(Form("plots/pdf/systs_mH_%3.1f_%s.pdf",mh,name.c_str()));
	}
  delete canv;
}

void FMTPlots::plotInterpolation(TH1F *sig, TH1F *interpLow, TH1F *interpHigh, double mh){
 
  TCanvas *c = new TCanvas("c","c",1800,2000);
  TPad *up = new TPad("u","u",0.01,0.6,0.99,0.99);
  TPad *dp = new TPad("d","d",0.01,0.01,0.99,0.6);
  up->SetNumber(1);
  dp->SetNumber(2);
  up->Draw();
  dp->Draw();

  c->cd(1);
  sig->SetLineColor(kBlack);
  sig->SetFillColor(kGray);
	sig->SetFillStyle(3003);
	sig->SetLineWidth(2);
  interpHigh->SetLineColor(kBlue);
	interpHigh->SetLineWidth(2);
  interpLow->SetLineColor(kRed);
	interpLow->SetLineWidth(2);

  interpHigh->SetTitle("");
  interpHigh->Draw("e");
  sig->Draw("hist same");
  interpLow->Draw("same e");

  TH1F *diffHigh = (TH1F*)interpHigh->Clone();
  diffHigh->Add(sig,-1);
  TH1F *diffLow = (TH1F*)interpLow->Clone();
  diffLow->Add(sig,-1);

  c->cd(2);
  diffHigh->SetTitle(Form("m_{H}=%3.1f",mh));
  diffHigh->Draw("e");
  TLine l1;
  l1.SetLineWidth(3);
  l1.DrawLine(0.,0.,diffHigh->GetNbinsX(),0.);
  diffHigh->Draw("same e");
	diffLow->Draw("same e");
  TLegend *leg = new TLegend(0.7,0.8,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->AddEntry(diffHigh,"Upper template","lep");
  leg->AddEntry(diffLow,"Lower template","lep");
  leg->Draw("same");

  c->Print(Form("plots/png/interp_mH_%3.1f.png",mh));
  c->Print(Form("plots/pdf/interp_mH_%3.1f.pdf",mh));
  
  delete c; 
}

void FMTPlots::plotSidebands(TH1F *bkg, vector<TH1F*> dataSideLow, vector<TH1F*> dataSideHigh, TH1F* bkgmc, vector<TH1F*> bkgSideLow, vector<TH1F*> bkgSideHigh, double mh){

  //int color[6] = {kGreen+4,kGreen-1,kGreen,kRed,kRed-2,kRed+4};
	int color[6] = {kMagenta,kBlue,kCyan+1,kGreen+1,kYellow+1,kRed};

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();
  TLegend *leg1 = new TLegend(0.6,0.6,0.85,0.85);
  TLegend *leg2 = new TLegend(0.6,0.6,0.85,0.85);
  leg1->SetLineColor(0);
  leg1->SetFillColor(0);
  leg2->SetLineColor(0);
  leg2->SetFillColor(0);
  TLatex *text = new TLatex();
  text->SetTextSize(0.04);
  text->SetNDC();

  c1->cd();
  bkg->SetMarkerStyle(20);
  bkg->SetMarkerColor(1);
  bkg->SetLineColor(1);
  leg1->AddEntry(bkg,"Bkg model in signal","lep");
  bkg->SetTitle(Form("Background model from data in sidebands m_{H} = %3.1f",mh));
  bkg->Draw("lep");

  c2->cd();
  bkgmc->SetMarkerStyle(20);
  bkgmc->SetMarkerColor(1);
  bkgmc->SetLineColor(1);
  bkgmc->SetTitle(Form("Background mc m_{H} = %3.1f",mh));
  leg2->AddEntry(bkgmc,"Bkg mc in signal","lep");
  bkgmc->Draw("lep");
 
  for (unsigned int h=0; h<dataSideHigh.size(); h++){
    c1->cd();
    dataSideHigh[h]->SetLineColor(color[h]);
    //dataSideHigh[h]->SetMarkerStyle(20);
    dataSideHigh[h]->SetMarkerColor(color[h]);
    dataSideHigh[h]->Scale(bkg->Integral()/bkgSideHigh[h]->Integral());
    leg1->AddEntry(dataSideHigh[h],Form("High sideband %d",h+1),"lep");
    if (!blind_) dataSideHigh[h]->Draw("same lep");
    c2->cd();
    bkgSideHigh[h]->SetLineColor(color[h]);
    //bkgSideHigh[h]->SetMarkerStyle(20);
    bkgSideHigh[h]->SetMarkerColor(color[h]);
    bkgSideHigh[h]->Scale(bkgmc->Integral()/bkgSideHigh[h]->Integral());
    leg2->AddEntry(bkgSideHigh[h],Form("High sideband %d",h+1),"lep");
    bkgSideHigh[h]->Draw("same lep");
  }
  for (unsigned int l=0; l<dataSideLow.size(); l++){
    c1->cd();
    dataSideLow[l]->SetLineColor(color[l+dataSideHigh.size()]);
    //dataSideLow[l]->SetMarkerStyle(20);
    dataSideLow[l]->SetMarkerColor(color[l+bkgSideHigh.size()]);
    dataSideLow[l]->Scale(bkg->Integral()/bkgSideLow[l]->Integral());
    leg1->AddEntry(dataSideLow[l],Form("Low sideband %d",l+1),"lep");
    if (!blind_) dataSideLow[l]->Draw("same lep");
    c2->cd();
    bkgSideLow[l]->SetLineColor(color[l+bkgSideHigh.size()]);
    //bkgSideLow[l]->SetMarkerStyle(20);
    bkgSideLow[l]->SetMarkerColor(color[l+dataSideHigh.size()]);
    bkgSideLow[l]->Scale(bkgmc->Integral()/bkgSideLow[l]->Integral());
    leg2->AddEntry(bkgSideLow[l],Form("Low sideband %d",l+1),"lep");
    bkgSideLow[l]->Draw("same lep");
  }
  c1->cd();
  leg1->Draw("same");
  text->DrawLatex(0.13,0.30,"CMS preliminary");
  if (getis2011()) text->DrawLatex(0.20,0.23,"#sqrt{s} = 7 TeV");
  else text->DrawLatex(0.20,0.23,"#sqrt{s} = 8 TeV");
  text->DrawLatex(0.18,0.15,Form("#int L = %1.1f fb^{-1}",getintLumi()));
  c2->cd();
  leg2->Draw("same");
  text->DrawLatex(0.13,0.30,"CMS preliminary");
  if (getis2011())text->DrawLatex(0.20,0.23,"#sqrt{s} = 7 TeV");
  else text->DrawLatex(0.20,0.23,"#sqrt{s} = 8 TeV");
  text->DrawLatex(0.18,0.15,Form("#int L = %1.1f fb^{-1}",getintLumi()));

  c1->SetLogy();
  c1->Print(Form("plots/png/dat_sideband_mH_%3.1f.png",mh));
  c1->Print(Form("plots/pdf/dat_sideband_mH_%3.1f.pdf",mh));
  c2->SetLogy();
  c2->Print(Form("plots/png/bkg_sideband_mH_%3.1f.png",mh));
  c2->Print(Form("plots/pdf/bkg_sideband_mH_%3.1f.pdf",mh));

  delete c1;
  delete c2;
}

void FMTPlots::plotOutput(TH1F* data, TH1F* bkg, TH1F* sig, TH1F* sig3, TH1F* sig5, double mh){
  // Outdated 
  return ;
  bkg->SetLineColor(kBlue);
  bkg->SetFillColor(kBlue-9);
  sig->SetLineColor(kRed);
  sig3->SetLineColor(kRed);
  sig3->SetLineStyle(3);
  sig5->SetLineColor(kRed);
  sig5->SetLineStyle(7);
  data->SetMarkerStyle(20);

  TCanvas *canv = new TCanvas();
  bkg->SetTitle(Form("m_{H} = %3.1f",mh));
  bkg->Draw("e2");
  sig->Draw("same hist");
  sig3->Draw("same hist");
  sig5->Draw("same hist");
  if (!blind_) data->Draw("same e");
  TLegend *leg = new TLegend(0.45,0.6,0.85,0.85);
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->AddEntry(bkg,"Background","f");
  if (!blind_) leg->AddEntry(data,"Data","lep");
  leg->AddEntry(sig,"Signal (1, 3, 5 #times SM)","l");
  TPaveText *txt = new TPaveText(0.2,0.1,0.4,0.35,"NDC");
  txt->SetFillColor(0);
  txt->SetLineColor(0);
  txt->AddText(Form("#int L = %1.1f fb^{-1}",getintLumi()));
  leg->Draw("same");
  txt->Draw("same");
  canv->SetLogy();
  canv->SaveAs(Form("plots/pdf/output_mH_%3.1f.pdf",mh));
  canv->SaveAs(Form("plots/macro/output_mH_%3.1f.C",mh));
  canv->SaveAs(Form("plots/png/output_mH_%3.1f.png",mh));

  TH1F *diff = (TH1F*)data->Clone();
  diff->Add(bkg,-1.);
  sig->SetTitle(Form("m_{H} = %3.1f",mh));
  sig5->Draw("hist");
  sig3->Draw("same hist");
  sig->Draw("same hist");
  if (!blind_) diff->Draw("same e2");
  TLegend *leg2 = new TLegend(0.45,0.6,0.85,0.85);
  leg2->SetLineColor(0);
  leg2->SetFillColor(0);
  if (!blind_) leg2->AddEntry(diff,"Data - background","lep");
  leg2->AddEntry(sig,"Signal (1, 3, 5 #times SM)","l");
  leg2->Draw("same");
  txt->Draw("same");
  canv->SetLogy();
  canv->SaveAs(Form("plots/pdf/diff_output_mH_%3.1f.pdf",mh));
  canv->SaveAs(Form("plots/macro/diff_output_mH_%3.1f.C",mh));
  canv->SaveAs(Form("plots/png/diff_output_mH_%3.1f.png",mh));

  delete canv;
  delete leg;
  delete leg2;
  delete txt;
}


void FMTPlots::plotAll(double mh){

  // This all breaks now!!!
  //if (!(mh==110||mh==120||mh==130||mh==140||mh==150) ) {
	//return;
  //}
  std::cout << "Plotting MC/Data for mass "<<mh<<std::endl;
  TH1F *bkg = (TH1F*)tFile->Get(Form("th1f_bkg_grad_%3.1f_fitsb_biascorr",mh));
  if (bkg==0)  {
	std::cout << " Cannot find corrected background model, Skipping. Rerun after sideband fit step!" << std::endl;
	return;
  }
  TH1F *bkgmc = (TH1F*)tFile->Get(Form("th1f_bkg_mc_grad_%3.1f",mh));
  TH1F *data = (TH1F*)tFile->Get(Form("th1f_data_grad_%3.1f",mh));
  //TH1F *bkgmc = (TH1F*) data->Clone();

  TH1F *sig = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%3.1f",mh)))->Clone();
  sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%3.1f",mh)));
  sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%3.1f",mh)));
  sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%3.1f",mh)));
  sig->SetName(Form("th1f_sig_grad_%3.1f",mh));
  write(tFile,sig);

  vector<pair<TH1F*,TH1F*> > sigSyst;
  vector<string> theSystematics = getsystematics();
  for (vector<string>::iterator syst = theSystematics.begin(); syst != theSystematics.end(); syst++){
    double systmass = 125.0;
    TH1F *up = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%3.1f_%sUp01_sigma",systmass,syst->c_str())))->Clone();
    up->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%3.1f_%sUp01_sigma",systmass,syst->c_str())));
    up->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%3.1f_%sUp01_sigma",systmass,syst->c_str())));
    up->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%3.1f_%sUp01_sigma",systmass,syst->c_str())));
    up->SetName(Form("th1f_sig_grad_%3.1f_%sUp01_sigma",systmass,syst->c_str()));
    write(tFile,up);
    TH1F *down = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%3.1f_%sUp01_sigma",systmass,syst->c_str())))->Clone();
    down->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%3.1f_%sDown01_sigma",systmass,syst->c_str())));
    down->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%3.1f_%sDown01_sigma",systmass,syst->c_str())));
    down->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%3.1f_%sDown01_sigma",systmass,syst->c_str())));
    down->SetName(Form("th1f_sig_grad_%3.1f_%sDown01_sigma",systmass,syst->c_str()));
    write(tFile,down);
    sigSyst.push_back(pair<TH1F*,TH1F*>(up,down));
  }

  int nearest = (getInterpMasses(mh)).first;
  int nextNear = (getInterpMasses(mh)).second;
  int binningMass, lowInterpMass, highInterpMass;
  if (nearest-nextNear>0) {
    binningMass=nearest;
    lowInterpMass=nextNear;
    highInterpMass=nearest;
  }
  else {
    binningMass=nearest;
    lowInterpMass=nearest;
    highInterpMass=nextNear;
  }

  TH1F *lowInterp = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%d.0_%d.0",binningMass,lowInterpMass)))->Clone();
  lowInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%d.0_%d.0",binningMass,lowInterpMass)));
  lowInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%d.0_%d.0",binningMass,lowInterpMass)));
  lowInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%d.0_%d.0",binningMass,lowInterpMass)));

  TH1F *highInterp = ((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%d.0_%d.0",binningMass,highInterpMass))->Clone());
  highInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%d.0_%d.0",binningMass,highInterpMass)));
  highInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%d.0_%d.0",binningMass,highInterpMass)));
  highInterp->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%d.0_%d.0",binningMass,highInterpMass)));

  int nLower = (getNsidebandsUandD(mh)).first;
  int nHigher = (getNsidebandsUandD(mh)).second;

  vector<TH1F*> dataSideLow;
  vector<TH1F*> dataSideHigh;
  vector<TH1F*> bkgSideLow;
  vector<TH1F*> bkgSideHigh;
  // loop lower
  for (int sideband_i=getnumberOfSidebandGaps()+1; sideband_i<=getnumberOfSidebandGaps()+nLower; sideband_i++){
    dataSideLow.push_back((TH1F*)tFile->Get(Form("th1f_bkg_%dlow_grad_%3.1f",sideband_i,mh)));
    bkgSideLow.push_back((TH1F*)tFile->Get(Form("th1f_bkg_mc_%dlow_grad_%3.1f",sideband_i,mh)));
  }

  // loop higher
  for (int sideband_i=getnumberOfSidebandGaps()+1; sideband_i<=getnumberOfSidebandGaps()+nHigher; sideband_i++){
    dataSideHigh.push_back((TH1F*)tFile->Get(Form("th1f_bkg_%dhigh_grad_%3.1f",sideband_i,mh)));
    bkgSideHigh.push_back((TH1F*)tFile->Get(Form("th1f_bkg_mc_%dhigh_grad_%3.1f",sideband_i,mh)));
  }
  
  makePlots(data,sig,bkg,bkgmc,dataSideLow,dataSideHigh,bkgSideLow,bkgSideHigh,sigSyst,lowInterp,highInterp,mh);
}

void FMTPlots::makeSignalNormPlot(){

  // make Xsection*BR plots
  vector<double> allMasses = getAllMH();
  
  normalizer->PlotExpected(allMasses[0],*(allMasses.end()-1));
  delete normalizer;
		
  TGraph *normG = new TGraph(getNumMHMasses());  
  int i=0;
  for (vector<double>::iterator mh=allMasses.begin(); mh!=allMasses.end(); mh++){
   TH1F *sig = (TH1F*)((TH1F*)tFile->Get(Form("th1f_sig_grad_ggh_%3.1f",*mh)))->Clone();
   sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_vbf_%3.1f",*mh)));
   sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_wzh_%3.1f",*mh)));
   sig->Add((TH1F*)tFile->Get(Form("th1f_sig_grad_tth_%3.1f",*mh)));
   
    normG->SetPoint(i,*mh,sig->Integral());
    i++;
  }

  normG->SetLineWidth(2);
  normG->SetLineColor(4);
  normG->SetTitle("Signal normalisations");
  normG->GetYaxis()->SetTitle("MC Signal events in signal region");
  normG->GetYaxis()->SetTitleOffset(1.2);
  normG->GetXaxis()->SetTitle("m_{H} (GeV)");

  TCanvas *canv = new TCanvas();
  normG->Draw("AL");
  canv->SaveAs("plots/pdf/signal_all_norm.pdf");
  canv->SaveAs("plots/macro/signal_all_norm.C");
  canv->SaveAs("plots/png/signal_all_norm.png");
  
}

void FMTPlots::makeNormPlot(){
 
  RooWorkspace *outWS = (RooWorkspace*)tFile->Get("cms_hgg_workspace");
  TGraph *normG = new TGraph(getNumMHMasses());  
  TGraph *dataN = new TGraph(getNumMHMasses());  
  TGraph *datadiff = new TGraph(getNumMHMasses());  
  vector<double> allMasses = getAllMH();
  int i=0; int di=0;
  for (vector<double>::iterator mh=allMasses.begin(); mh!=allMasses.end(); mh++){
    RooRealVar *val = (RooRealVar*)outWS->var(Form("NBkgInSignal_mH%3.1f",*mh));
    TH1F *dataHist = (TH1F*)tFile->Get(Form("th1f_data_grad_%3.1f",*mh));
    normG->SetPoint(i,*mh,val->getVal());
    if (blind_) {
	if (*mh<110 || *mh > 150) {
	  dataN->SetPoint(di,*mh,dataHist->Integral());
          datadiff->SetPoint(di,*mh,dataHist->Integral()-val->getVal());
	  di++;
	}
    } else {
	  dataN->SetPoint(di,*mh,dataHist->Integral());
          datadiff->SetPoint(di,*mh,dataHist->Integral()-val->getVal());
	  di++;
    }
    i++;
  }
  TCanvas *canv = new TCanvas();
  normG->SetMarkerSize(2);
  normG->SetLineWidth(2);
  normG->SetLineColor(4);
  normG->SetTitle("Fit normalisations");
  normG->GetYaxis()->SetTitle("Background events in signal region");
  normG->GetYaxis()->SetTitleOffset(1.2);
  normG->GetXaxis()->SetTitle("m_{H} (GeV)");
  dataN->SetMarkerStyle(21);
  dataN->SetMarkerSize(0.8);
  dataN->SetMarkerColor(1);
  datadiff->SetMarkerStyle(21);
  datadiff->SetMarkerSize(0.8);
  datadiff->SetMarkerColor(1);
  normG->Draw("ALP");
  dataN->Draw("P");
  TLatex *text = new TLatex();
	text->SetTextSize(0.04);
	text->SetNDC();
	text->DrawLatex(0.68,0.85,"CMS preliminary");
  if (getis2011()) text->DrawLatex(0.75,0.78,"#sqrt{s} = 7 TeV");
  else text->DrawLatex(0.75,0.78,"#sqrt{s} = 8 TeV");
  text->DrawLatex(0.73,0.71,Form("#int L = %1.1f fb^{-1}",getintLumi()));
  canv->SaveAs("plots/pdf/fit_all_norm.pdf");
  canv->SaveAs("plots/macro/fit_all_norm.C");
  canv->SaveAs("plots/png/fit_all_norm.png");

	datadiff->Draw("AP" );
  canv->SaveAs("plots/pdf/diff_fit_all_norm.pdf");
  canv->SaveAs("plots/macro/diff_fit_all_norm.C");
  canv->SaveAs("plots/png/diff_fit_all_norm.png");

	delete normG;
	delete canv;
}

void FMTPlots::plotByMH(string pathname, double mh, TH1F *data, TH1F *sig, TH1F *bkg){

	TH1F *sig5 = (TH1F*)sig->Clone();
	sig5->SetName(Form("th1f_sig5_grad_%3.1f",mh));
	sig5->Scale(5.);
	TH1F *bkg1 = (TH1F*)bkg->Clone();
	bkg1->SetName(Form("th1f_bkg1_grad_%3.1f",mh));
	TH1F *bkg2 = (TH1F*)bkg->Clone();
	bkg2->SetName(Form("th1f_bkg2_grad_%3.1f",mh));
	TH1F *bkgML = (TH1F*)bkg->Clone();
	bkgML->SetName(Form("th1f_bkgml_grad_%3.1f",mh));
	TH1F *sobML = (TH1F*)bkg->Clone();
	sobML->SetName(Form("th1f_sobml_grad_%3.1f",mh));
	TH1F *sbML = (TH1F*)bkg->Clone();
	sbML->SetName(Form("th1f_sbml_grad_%3.1f",mh));

	string textfile = Form("%s/%3.1fout.txt",pathname.c_str(),mh);
	vector<double> normErrs = getNormErrs(Form("%s/mva-datacard_grad_%3.1f.txt",pathname.c_str(),mh));

	for (int bin=1; bin<=bkg->GetNbinsX(); bin++){
		pair<pair<double,double>,pair<double,double> > res = getBandSB(textfile,bin);
		bkgML->SetBinContent(bin,(res.first).first);
		sbML->SetBinContent(bin,(res.first).second);
		sobML->SetBinContent(bin,(res.second).second/(res.second).first);
		if (verbose_) cout << normErrs[bin] << endl;
		bkg1->SetBinError(bin,pow(pow(bkg1->GetBinError(bin),2)+pow(bkg1->GetBinContent(bin)*(normErrs[bin-1]-1),2),0.5));
		bkg2->SetBinError(bin,2*pow(pow(bkg2->GetBinError(bin),2)+pow(bkg2->GetBinContent(bin)*(normErrs[bin-1]-1),2),0.5));
	}

	bkgML->SetLineColor(kRed);
	bkgML->SetLineWidth(3);
	sbML->SetLineColor(kBlue);
	sbML->SetLineWidth(3);
	bkg->SetLineColor(kBlack);
	bkg->SetLineWidth(2);
	bkg1->SetLineColor(1);
	bkg1->SetLineWidth(2);
	bkg1->SetFillStyle(1001);
	bkg1->SetFillColor(3);
	bkg2->SetLineColor(1);
	bkg2->SetLineWidth(2);
	bkg2->SetFillStyle(1001);
	bkg2->SetFillColor(5);
	sig5->SetLineStyle(kDashed);
	sig5->SetLineColor(kRed);
	sig5->SetLineWidth(2);
	sobML->SetLineColor(1);
	sobML->SetLineWidth(3);
	
	TLegend *leg = new TLegend(0.6,0.5,0.89,0.89);
	leg->SetFillColor(0);
	leg->SetFillStyle(0);
	leg->SetLineColor(0);
	if (!blind_) leg->AddEntry(data,"Data","lep");
	leg->AddEntry(sig5,Form("Higgs, m_{H}=%3.1f GeV (x5)",mh),"l");
	leg->AddEntry(bkg,"Background","l");
	leg->AddEntry(bkg1,"Background #pm 1#sigma","f");
	leg->AddEntry(bkg2,"Background #pm 2#sigma","f");
	leg->AddEntry(bkgML,"Background (ML fit)","l");
	leg->AddEntry(sbML,"S+B (ML fit)","l");

	double chi2bkg = data->Chi2Test(bkg);
	double chi2bkgML = data->Chi2Test(bkgML);
	double chi2sbML = data->Chi2Test(sbML);

	TPaveText *text = new TPaveText(0.3,0.6,0.5,0.89,"NDC");
	text->SetLineColor(0);
	text->SetFillColor(0);
	text->SetFillStyle(0);
	text->SetTextAlign(11);
	text->AddText(Form("#chi^{2} (B) = %1.3f",chi2bkg));
	text->AddText(Form("#chi^{2} (B_{ML}) = %1.3f",chi2bkgML));
	text->AddText(Form("#chi^{2} (SB_{ML}) = %1.3f",chi2sbML));
	TPaveText *massText = new TPaveText(0.15,0.8,0.3,0.89,"NDC");
	massText->SetFillStyle(0);
	massText->SetLineColor(0);
	massText->AddText(Form("m_{H}=%3.1f",mh));
	TLatex *runText = new TLatex();
	runText->SetTextSize(0.03);
	runText->SetNDC();
	
	TCanvas *canv = new TCanvas("c","c",1800,2000);
	TPad *up = new TPad("u","u",0.01,0.3,0.99,0.99);
	TPad *dp = new TPad("d","d",0.01,0.01,0.99,0.3);
	up->SetNumber(1);
	dp->SetNumber(2);
	up->Draw();
	dp->Draw();

	canv->cd(1);
	bkg2->GetYaxis()->SetRangeUser(1,2e4);
	bkg2->Draw("E2");
	bkg1->Draw("sameE2");
	bkg->Draw("samehist");
	bkgML->Draw("samehist");
	sbML->Draw("samehist");
	if (!blind_) data->Draw("samele1p");
	leg->Draw("same");
	text->Draw("same");
	massText->Draw("same");
	up->SetLogy();
	if (getis2011())runText->DrawLatex(0.1,0.94,"CMS preliminary, #sqrt{s} = 7 TeV");
	else runText->DrawLatex(0.1,0.94,"CMS preliminary, #sqrt{s} = 8 TeV");
	runText->SetTextSize(0.04);
	runText->DrawLatex(0.5,0.94,"#int L = 5.1");

	canv->cd(2);
	TLegend *leg2 = new TLegend(0.11,0.75,0.4,0.88);
	leg2->SetFillColor(0);
	leg2->SetBorderSize(0);
	leg2->AddEntry(sobML,"S/B (ML)","l");
	sobML->GetYaxis()->SetRangeUser(-0.2,0.8);
	sobML->SetTitle("");
	sobML->GetXaxis()->SetLabelSize(0.06);
	sobML->GetYaxis()->SetLabelSize(0.06);
	sobML->GetYaxis()->SetTitle("S/B (ML)");
	sobML->GetYaxis()->SetTitleSize(0.06);
	sobML->GetYaxis()->SetTitleOffset(0.5);
	sobML->Draw("hist");
	leg2->Draw("same");

	canv->Print(Form("plots/pdf/sb_by_mh_%3.1f.pdf",mh));
	canv->Print(Form("plots/png/sb_by_mh_%3.1f.png",mh));

	delete leg;
	delete leg2;
	delete text;
	delete massText;
	delete runText;
	delete up;
	delete dp;
	delete canv;
}

// returns b only, s+b, s+b (b part), s+b (s part)
pair<pair<double,double>,pair<double,double> > FMTPlots::getBandSB(string textfile, int bin){

	TMacro *mac = new TMacro(textfile.c_str(),"fake card");
	TList *list = (TList*)mac->GetListOfLines();
	double b=0;
	double sb=0;
	double sb_b=0;
	double s=0;
	for (int l=0; l<list->GetSize(); l++){
		TObjString *line = (TObjString*)list->At(l);
		string sline = Form("%s",line->GetName());
		boost::trim_all(sline);
		if (sline.find(Form("cat%d",bin))!=string::npos){
			vector<string> elements;
			boost::split(elements,sline,boost::is_any_of(" "));
			if (elements.size()!=4){
				cerr << "ERROR - FMTPlots::getBandSB - wrong number of elements in vector. I don\'t know what to do. Bailing out" << endl;
				exit(1);
			}
			b+=boost::lexical_cast<double>(elements[3]);
			sb+=boost::lexical_cast<double>(elements[2]);
			if (sline.find("bkg")!=string::npos) sb_b=boost::lexical_cast<double>(elements[2]);
		}
	}
	s=sb-sb_b;
	delete mac;
	pair<double,double> t1(b,sb);
	pair<double,double> t2(sb_b,s);
	return pair<pair<double,double>,pair<double,double> >(t1,t2);	
}

vector<double> FMTPlots::getNormErrs(string textfile){
	
	vector<double> result;
	TMacro *mac = new TMacro(textfile.c_str(),"dat card");
	TList *list = (TList*)mac->GetListOfLines();
	for (int l=0; l<list->GetSize(); l++){
		TObjString *line = (TObjString*)list->At(l);
		string sline = Form("%s",line->GetName());
		if (sline.find("bkg_norm")!=string::npos){
			boost::trim_all_if(sline,boost::is_any_of("bkg_norm lnN - -"));
			vector<string> elements;
			boost::split(elements,sline,boost::is_any_of(" "));
			for (vector<string>::iterator it = elements.begin(); it != elements.end(); it++) result.push_back(boost::lexical_cast<double>(*it));
		}
	}
	return result;
}
