// Makes partially blinded mass distribution + fit plots for Mass-fac MVA analysis
// Needs the Parametric signal model also

///// #include "TFile.h"
///// #include "TH1F.h"
///// #include "RooDataSet.h"
///// #include "RooAbsPdf.h"
///// #include "RooExtendPdf.h"
///// #include "RooWorkspace.h"
///// #include "RooRealVar.h"
///// #include "TCanvas.h"
///// #include "TLegend.h"
///// #include "TLatex.h"
///// #include "TAxis.h"
///// #include "RooPlot.h"
///// #include "RooAddPdf.h"
///// #include "RooArgList.h"
///// #include "RooGaussian.h"
///// #include "TROOT.h"
///// #include "TStyle.h"
///// #include "RooFitResult.h"
///// #include <iostream>

#include "makeBkgPlotsGeneric.C"
void makeBkgPlotsBaseline(std::string filebkg,bool blind=true, bool doBands=true)
{
	makeBkgPlotsGeneric(filebkg, blind, doBands, true);
}

/////// #include "BandsFit.h"
/////// 
/////// void makeBkgPlotsBaseline(std::string filebkg,std::string filesig="",bool blind=true, bool doBands=true){
/////// 
/////// 	// Globals
/////// 	gROOT->SetStyle("Plain");
/////// 	gROOT->SetBatch(1);
/////// 	gStyle->SetOptStat(0);
/////// 	//// const int ncats = 6;
/////// 	const int ncats = 1;
/////// 
/////// 	std::string labels[6] = {
/////// 			"Both photons in barrel, R_{9}^{min} > 0.94"
/////// 			,"Both photons in barrel, R_{9}^{min} < 0.94"
/////// 			,"One or more photons in endcap, R_{9}^{min} > 0.94"
/////// 			,"One or more photons in endcap, R_{9}^{min} < 0.94"
/////// 			,"Dijet-tagged class m_{jj} > 500 GeV"
/////// 			,"Dijet-tagged class 250 < m_{jj} < 500 GeV"
/////// 			};
/////// 
/////// 	TFile *fb = TFile::Open(filebkg.c_str());
/////// 	///// TFile *fs = TFile::Open(filesig.c_str());
/////// 
/////// 	RooWorkspace *w_bkg  = (RooWorkspace*) fb->Get("cms_hgg_workspace");
/////// 	///// RooWorkspace *w_sig  = (RooWorkspace*) fs->Get("wsig");
/////// 
/////// 	RooRealVar *x = (RooRealVar*) w_bkg->var("CMS_hgg_mass");
/////// 	///// RooRealVar *mH = (RooRealVar*) w_sig->var("MH");
/////// 	///// mH->setVal(120.0);
/////// 
/////// 	TLatex *latex = new TLatex();	
/////// 	latex->SetTextSize(0.025);
/////// 	latex->SetNDC();
/////// 	
/////// 	TLatex *cmslatex = new TLatex();
/////// 	cmslatex->SetTextSize(0.04);
/////// 	cmslatex->SetNDC();
/////// 
/////// 	for (int cat=0;cat<ncats;cat++){
/////// 	
/////// 		TCanvas *can = new TCanvas("c","",800,800);
/////// 		TLegend *leg = new TLegend(0.6,0.6,0.89,0.89);
/////// 		leg->SetFillColor(0);
/////// 		leg->SetBorderSize(0);
/////// 
/////// 		// Get Dataset ->
/////// 		RooDataSet *data =  (RooDataSet*)w_bkg->data(Form("data_mass_cat%d",cat));
/////// 		
/////// 		// Background Pdf ->
/////// 		//// RooExtendPdf *bkg =  (RooExtendPdf*)w_bkg->pdf(Form("data_pol_model_cat%d",cat));
/////// 		/// RooExtendPdf *bkg =  (RooExtendPdf*)w_bkg->pdf(Form("data_pol_model_8TeV_cat%d",cat));
/////// 		RooAbsPdf *bkg =  (RooAbsPdf*)w_bkg->pdf(Form("pdf_data_pol_model_8TeV_cat%d",cat));
/////// 		RooFitResult *r = bkg->fitTo(*data,RooFit::Save(1));
/////// 		
/////// 		// Get Signal pdf norms
/////// 		std::cout << "Getting Signal Components" << std::endl;
/////// //		RooRealVar *gghnorm = w_sig->var(Form("hggpdf_cat%d_ggh_norm",cat));
/////// //		RooRealVar *vbfnorm = w_sig->var(Form("hggpdf_cat%d_vbf_norm",cat));
/////// //		RooRealVar *wzhnorm = w_sig->var(Form("hggpdf_cat%d_wzh_norm",cat));
/////// //		RooRealVar *tthnorm = w_sig->var(Form("hggpdf_cat%d_tth_norm",cat));
/////// 		TH1F *gghnorm = (TH1F*)fb->Get(Form("th1f_sig_ggh_mass_m120_cat%d",cat));
/////// 		TH1F *vbfnorm = (TH1F*)fb->Get(Form("th1f_sig_vbf_mass_m120_cat%d",cat));
/////// 		TH1F *wzhnorm = (TH1F*)fb->Get(Form("th1f_sig_wzh_mass_m120_cat%d",cat));
/////// 		TH1F *tthnorm = (TH1F*)fb->Get(Form("th1f_sig_tth_mass_m120_cat%d",cat));
/////// 
/////// 		std::cout << "Rescaling Signal Components" << std::endl;
/////// 		double totalSig = gghnorm->Integral()+vbfnorm->Integral()+wzhnorm->Integral()+tthnorm->Integral();
/////// 		totalSig*=2;
/////// //		gghnorm->setVal(2*gghnorm->getVal());
/////// //		vbfnorm->setVal(2*vbfnorm->getVal());
/////// //		wzhnorm->setVal(2*wzhnorm->getVal());
/////// //		tthnorm->setVal(2*tthnorm->getVal());
/////// 
/////// 		/////// // Signal Pdfs -> Taken from binned model refit for now
/////// 		/////// RooAbsPdf *ggh = w_sig->pdf(Form("sigpdfcat%d_ggh",cat));
/////// 		/////// RooAbsPdf *vbf = w_sig->pdf(Form("sigpdfcat%d_vbf",cat));
/////// 		/////// RooAbsPdf *wzh = w_sig->pdf(Form("sigpdfcat%d_wzh",cat));
/////// 		/////// RooAbsPdf *tth = w_sig->pdf(Form("sigpdfcat%d_tth",cat));
/////// 
/////// 		std::cout << "Combining Signal Components" << std::endl;
/////// 		/// RooArgList siglist(*ggh,*vbf,*wzh,*tth);
/////// 		/// /RooAddPdf allsig("all","all",siglist);
/////// 		std::cout << "Signal OK" << std::endl;
/////// 		
/////// 		// Make some dummy hists for latex to grab
/////// 		TH1F dumData("d","",80,100,180); dumData.Sumw2();dumData.SetMarkerSize(1.0);dumData.SetMarkerStyle(20);dumData.SetLineWidth(3);
/////// 		dumData.Fill(101);
/////// 		TH1F dumSignal("s","",80,100,180); dumSignal.SetLineColor(4);dumSignal.SetFillColor(38);dumSignal.SetFillStyle(3001) ;dumSignal.SetLineWidth(2);
/////// 		TH1F dum1Sig("1s","",80,100,180); dum1Sig.SetFillColor(kYellow);dum1Sig.SetFillStyle(1001);
/////// 		TH1F dum2Sig("2s","",80,100,180); dum2Sig.SetFillColor(kGreen);dum2Sig.SetFillStyle(1001);
/////// 		TH1F dumBkg("b","",80,100,180); dumBkg.SetLineColor(kRed);dumBkg.SetLineWidth(3);
/////// 		dumBkg.Draw("P");dumSignal.Draw("LFsame");dumBkg.Draw("Fsame");dum1Sig.Draw("Fsame");dum2Sig.Draw("Lsame");
/////// 
/////// 		// Plot background
/////// 		RooPlot *frame = x->frame();
/////// 
/////// 		std::cout << "Plotting Components" << std::endl;
/////// 		data->plotOn(frame,RooFit::Binning(80),RooFit::Invisible());
/////// 		//// bkg->plotOn(frame,RooFit::VisualizeError(*r,2,true),RooFit::FillColor(kGreen));
/////// 		//// bkg->plotOn(frame,RooFit::VisualizeError(*r,1,true),RooFit::FillColor(kYellow));
/////// 		bkg->plotOn(frame,RooFit::Invisible());
/////// 		
/////// 		TGraphAsymmErrors *onesigma = 0;
/////// 		TGraphAsymmErrors *twosigma = 0;
/////// 		if( doBands ) {
/////// 			TGraphAsymmErrors *onesigma = new TGraphAsymmErrors();
/////// 			TGraphAsymmErrors *twosigma = new TGraphAsymmErrors();
/////// 			doBandsFit(onesigma, twosigma, x, bkg, dynamic_cast<RooCurve*>(frame->getObject(frame->numItems()-1)), 
/////// 				   data, frame, Form("cat%d",cat) );
/////// 		}
/////// 		bkg->plotOn(frame,RooFit::LineColor(kRed));
/////// 		///// allsig.plotOn(frame,RooFit::LineColor(kBlue),RooFit::FillColor(38), RooFit::Normalization(totalSig,RooAbsReal::NumEvent));
/////// 		if( blind ) {
/////// 			x->setRange("unblind_up",150,180);
/////// 			data->plotOn(frame,RooFit::Binning(80),RooFit::CutRange("unblind_up"));
/////// 			x->setRange("unblind_down",100,110);
/////// 			data->plotOn(frame,RooFit::Binning(80),RooFit::CutRange("unblind_down"));
/////// 		} else {
/////// 			data->plotOn(frame,RooFit::Binning(80));
/////// 		}
/////// 		
/////// 		frame->SetTitle("");
/////// 		frame->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");
/////// 		frame->GetXaxis()->SetNdivisions(5,5,0);
/////// 		frame->GetYaxis()->SetTitle("Events / (1 GeV)");
/////// 		frame->GetYaxis()->SetTitleOffset(1.2);
/////// 		
/////// 		leg->AddEntry(&dumData,"Data","PEL");
/////// 		leg->AddEntry(&dumBkg,"Bkg Model","L");
/////// 		leg->AddEntry(&dum1Sig,"#pm 1#sigma","F");
/////// 		leg->AddEntry(&dum2Sig,"#pm 2#sigma","F");
/////// 		/// leg->AddEntry(&dumSignal,"2xSM m_{H} = 120 GeV","L");
/////// 		
/////// 		frame->Draw();
/////// 		if( doBands ) {
/////// 			twosigma->SetLineColor(kGreen);
/////// 			twosigma->SetFillColor(kGreen);
/////// 			twosigma->SetMarkerColor(kGreen);
/////// 			twosigma->Draw("L3 SAME");     
/////// 			
/////// 			onesigma->SetLineColor(kYellow);
/////// 			onesigma->SetFillColor(kYellow);
/////// 			onesigma->SetMarkerColor(kYellow);
/////// 			onesigma->Draw("L3 SAME");
/////// 			frame->Draw("same");
/////// 		}
/////// 		
/////// 		frame->SetMinimum(0.0001);
/////// 		leg->Draw();
/////// 		cmslatex->DrawLatex(0.15,0.8,"#splitline{CMS Preliminary}{#sqrt{s} = 8TeV L = 3.8fb^{-1}}");
/////// 		latex->DrawLatex(0.1,0.92,labels[cat].c_str());
/////// 		can->SaveAs(Form("baselinecat%d.pdf",cat));
/////// 		can->SaveAs(Form("baselinecat%d.png",cat));
/////// 	}
/////// 
/////// }

