#include "TH1F.h"
#include "TFile.h"
#include "TMatrixD.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLatex.h"
#include <iostream>
#include <string>

using namespace std;

double getInterp(double C, double X1,double X2,double X3,double Y1,double Y2,double Y3){

	gROOT->SetStyle("Plain");
	gROOT->SetBatch(true);
	gStyle->SetOptStat(0);
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

TH1F rebin(TH1F* h){

	
	TH1F n(Form("%s",h->GetName()),h->GetTitle(),h->GetNbinsX(),0,h->GetNbinsX());
	for (int b=1;b<=n.GetNbinsX();b++){
		n.SetBinContent(b,h->GetBinContent(b));
		n.SetBinError(b,h->GetBinError(b));
	}
	
	return n;
}	

void interpolateSystematics(std::string filename,std::string sys, double M=120.){


	// Congifure some things here:
	std::string type = "ggh";
	const int ngraphs = 6;	// number of bins to look at for making the quad interp checks

	std::cout << "Test -- " << getInterp(0.,-1.,0.,1.,-.5,-.2,0.9) << std::endl;
	TFile *F = TFile::Open(filename.c_str());
	
	// Get One of the systematic histograms: 
	TH1F *cenbdt = (TH1F*) F->Get(Form("th1f_sig_BDT_grad_%s_%3.1f_%3.1f_cat0",type.c_str(),M));
	TH1F *dn1bdt = (TH1F*) F->Get(Form("th1f_sig_BDT_grad_%s_%3.1f_%3.1f_cat0_%sDown01_sigma",type.c_str(),M,M,sys.c_str()));
	TH1F *up1bdt = (TH1F*) F->Get(Form("th1f_sig_BDT_grad_%s_%3.1f_%3.1f_cat0_%sUp01_sigma"  ,type.c_str(),M,M,sys.c_str()));
	TH1F *dn2bdt = (TH1F*) F->Get(Form("th1f_sig_BDT_grad_%s_%3.1f_%3.1f_cat0_%sDown02_sigma",type.c_str(),M,M,sys.c_str()));
	TH1F *up2bdt = (TH1F*) F->Get(Form("th1f_sig_BDT_grad_%s_%3.1f_%3.1f_cat0_%sUp02_sigma"  ,type.c_str(),M,M,sys.c_str()));
	TH1F *dn3bdt = (TH1F*) F->Get(Form("th1f_sig_BDT_grad_%s_%3.1f_%3.1f_cat0_%sDown03_sigma",type.c_str(),M,M,sys.c_str()));
	TH1F *up3bdt = (TH1F*) F->Get(Form("th1f_sig_BDT_grad_%s_%3.1f_%3.1f_cat0_%sUp03_sigma"  ,type.c_str(),M,M,sys.c_str()));

	TH1F ceng = rebin(cenbdt);
	TH1F dn1g = rebin(dn1bdt);
	TH1F up1g = rebin(up1bdt);
	TH1F dn2g = rebin(dn2bdt);
	TH1F up2g = rebin(up2bdt);
	TH1F dn3g = rebin(dn3bdt);
	TH1F up3g = rebin(up3bdt);

	TH1F *cen = &ceng;
	TH1F *dn1 = &dn1g;
	TH1F *up1 = &up1g;
	TH1F *dn2 = &dn2g;
	TH1F *up2 = &up2g;
	TH1F *dn3 = &dn3g;
	TH1F *up3 = &up3g;
	

	TCanvas *canvas = new TCanvas("can",Form("%s",sys.c_str()),800,800);

	TGraph* graphs_line[ngraphs];
	TGraph* graphs_sig3[ngraphs];
	TGraph* graphs_sig2[ngraphs];
	int chosenbins[ngraphs];
	int nBins = cen->GetNbinsX();
	
	int m=0;	
	for (int b=nBins;m<ngraphs;b--){
		if (b == 8) continue;
		int npoints = 50;
		TGraph line(npoints);
		TGraph sig3(3);
		TGraph sig2(4);

		chosenbins[m]=b;
		double y1 = dn3->GetBinContent(b);
		double y2 = cen->GetBinContent(b);
		double y3 = up3->GetBinContent(b);

		// Get Loads of Interpolated Points
		double step=6./npoints;
		for (int c=0;c<=npoints;c++){
			double interpolated = getInterp(-3.0+c*step,-3.,0.,3.,y1,y2,y3);
		//	cout << interpolated <<endl;
		//	cout << "Looking at bin " << b <<endl;
			line.SetPoint(c,-3.0+c*step,interpolated);
		}
		sig3.SetPoint(0,-3.,y1);sig3.SetPoint(1,0.,y2);sig3.SetPoint(2,3.,y3);
		sig2.SetPoint(0,-2.,dn2->GetBinContent(b));sig2.SetPoint(1,-1.,dn1->GetBinContent(b));sig2.SetPoint(2,1.,up1->GetBinContent(b));sig2.SetPoint(3,2.,up2->GetBinContent(b));

		line.SetLineColor(1);
		line.SetLineWidth(2);
		sig3.SetMarkerStyle(20);
		sig3.SetMarkerSize(1.2);
		sig3.SetMarkerColor(2);
		sig3.SetLineColor(0);
		sig2.SetMarkerStyle(20);
		sig2.SetMarkerSize(1.2);
		sig2.SetMarkerColor(4);
		sig2.SetLineColor(0);
	
		graphs_line[m]=(TGraph*)line.Clone();
		graphs_sig3[m]=(TGraph*)sig3.Clone();
		graphs_sig2[m]=(TGraph*)sig2.Clone();
	
		m++;
	}

	TMultiGraph *MG = new TMultiGraph();
	
	for (int n=0;n<ngraphs;n++){	
		MG->Add(graphs_line[n]);
	}
	

	MG->Draw("ALP");
	MG->GetYaxis()->SetRangeUser(0.,8.);
	MG->GetXaxis()->SetRangeUser(-3.,3.);
	MG->GetYaxis()->SetTitle("Number of events in bin");
	MG->GetXaxis()->SetTitle("Systematic Shift (n#sigma)");
	MG->SetTitle(Form("%s",sys.c_str()));

	TLatex *mytext = new TLatex();
	mytext->SetTextSize(0.02);
	for (int n=0;n<ngraphs;n++){	
		//MG->Add(graphs_sig3[n]);
		//MG->Add(graphs_sig2[n]);
		graphs_sig3[n]->Draw("Psame");
		graphs_sig2[n]->Draw("Psame");
		mytext->DrawLatex(3.2,graphs_sig3[n]->Eval(3.0),Form("Bin %d",chosenbins[n]));
		
	}

	canvas->SaveAs(Form("systematic_interpolation_test_curves_%s.pdf",sys.c_str()));


	// Compare an entire BDT distribtion interpolation
	TCanvas *canvas2 = new TCanvas("can2",Form("%s",sys.c_str()),800,800);
	TPad *up = new TPad("u","u",0.01,0.25,0.99,0.99);
	TPad *dp = new TPad("d","d",0.01,0.01,0.99,0.25);
	up->SetNumber(1);
	dp->SetNumber(2);
	up->Draw();
	dp->Draw();
	// create an interpolated histogram at 1 sigma
	TH1F *h_new = (TH1F*) up3->Clone();
	for (int bb=1;bb<=h_new->GetNbinsX();bb++){
		double y1 = dn3->GetBinContent(bb);
		double y2 = cen->GetBinContent(bb);
		double y3 = up3->GetBinContent(bb);
		h_new->SetBinContent(bb,getInterp(1.0,-3.,0.,3.,y1,y2,y3));
		h_new->SetBinError(bb,0);
	}
	
	cen->SetLineColor(2);
	cen->SetLineWidth(2);
	cen->SetLineStyle(7);
	up3->SetLineColor(2);
	up3->SetLineWidth(2);
	up3->SetLineStyle(7);
	dn3->SetLineColor(2);
	dn3->SetLineWidth(2);
	dn3->SetLineStyle(7);

	TLegend *leg = new TLegend(0.35,0.6,0.85,0.8);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	up1->GetXaxis()->SetTitle("BDT Gradient Boost (bin number)");
	h_new->SetMarkerStyle(20);
	h_new->SetMarkerColor(1);
	h_new->SetMarkerSize(1.0);
	up1->GetYaxis()->SetTitle("Number of Events - 1xSM");
	up1->SetTitle("");
	h_new->SetLineColor(1);
	up1->SetLineColor(4);
	up1->SetLineWidth(2);

	leg->AddEntry(cen,"#pm3 #sigma and nominal Truth","L" );
	leg->AddEntry(up1,"+1 #sigma Truth","L" );
	leg->AddEntry(h_new,"+1 #sigma Interpolated","P" );

	canvas2->cd(1);
	up1->GetYaxis()->SetRangeUser(0,25);
	up1->Draw("hist");
	cen->Draw("histsame");
	up3->Draw("histsame");
	dn3->Draw("histsame");
	h_new->Draw("sameP");
	
	leg->Draw();

	
	canvas2->cd(2);
	TH1F *hist_1 =(TH1F*)up1->Clone();
	hist_1->Divide(up1);
	hist_1->SetMaximum(2);
	hist_1->SetMinimum(0);
	hist_1->SetTitle("");
	hist_1->GetYaxis()->SetLabelSize(0.12);
	hist_1->GetYaxis()->SetNdivisions(3);
	hist_1->GetXaxis()->SetLabelSize(0.12);
	hist_1->GetXaxis()->SetTitle("");
	hist_1->GetYaxis()->SetTitle("");
	hist_1->Draw("hist");
	TH1F *h_rat =(TH1F*) h_new->Clone();
	
	h_rat->Divide(up1);
	h_rat->Draw("Psame");
	canvas2->SaveAs(Form("systematic_interpolation_test_%s.pdf",sys.c_str()));
	

}
