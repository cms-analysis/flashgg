// Makes partially blinded mass distribution + fit plots for Mass-fac MVA analysis

#ifndef __CINT__
#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooMsgService.h"
#include "RooMinimizer.h"
#include "RooAbsPdf.h"
#include "RooExtendPdf.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "TMath.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooGaussian.h"
#include "TROOT.h"
#include "TStyle.h"
#include "RooFitResult.h"
#include "RooStats/NumberCountingUtils.h"
#include "RooStats/RooStatsUtils.h"

#include <iostream>
#endif 

/*
void doBandsFit(TGraphAsymmErrors *onesigma, TGraphAsymmErrors *twosigma, 
		RooRealVar * hmass,
		RooAbsPdf *cpdf, 
		RooCurve *nomcurve,  RooAbsData *datanorm,
		RooPlot *plot, 
		TString & catname);
*/
using namespace RooFit;


void doBandsFit(TGraphAsymmErrors *onesigma, TGraphAsymmErrors *twosigma, 
		RooRealVar * hmass,
		RooAbsPdf *cpdf, 
		RooCurve *nomcurve,  RooAbsData *datanorm,
		RooPlot *plot, 
		TString & catname)
{
	RooRealVar *nlim = new RooRealVar(TString::Format("nlim%s",catname.Data()),"",0.0,0.0,1e+5);

	for (int i=1; i<(plot->GetXaxis()->GetNbins()+1); ++i) {

		double lowedge = plot->GetXaxis()->GetBinLowEdge(i);
		double upedge = plot->GetXaxis()->GetBinUpEdge(i);
		double center = plot->GetXaxis()->GetBinCenter(i);
        
		double nombkg = nomcurve->interpolate(center);

		nlim->setVal(nombkg);
		hmass->setRange("errRange",lowedge,upedge);
		RooAbsPdf *epdf = 0;
		epdf = new RooExtendPdf("epdf","",*cpdf,*nlim,"errRange");
        
		RooAbsReal *nll = epdf->createNLL(*datanorm,Extended(),NumCPU(4));
		RooMinimizer minim(*nll);
		minim.setStrategy(0);
		minim.setPrintLevel(-1);
		//double clone = 1.0 - 2.0*RooStats::SignificanceToPValue(1.0);
		double cltwo = 1.0 - 2.0*RooStats::SignificanceToPValue(2.0);
        
		minim.migrad();
		minim.minos(*nlim);
		
		printf("errlo = %5f, errhi = %5f\n",nlim->getErrorLo(),nlim->getErrorHi());
		
		onesigma->SetPoint(i-1,center,nombkg);
		onesigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());
        
		minim.setErrorLevel(0.5*pow(ROOT::Math::normal_quantile(1-0.5*(1-cltwo),1.0), 2)); // the 0.5 is because qmu is -2*NLL
		// eventually if cl = 0.95 this is the usual 1.92!      
        	minim.migrad();
		minim.minos(*nlim);
		
		twosigma->SetPoint(i-1,center,nombkg);
		twosigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());      
        		
		delete nll;
		delete epdf;
	}

	onesigma->Print("V");

}

void makeBkgPlotsGeneric(std::string filebkg, std::string filesig="", const int ncats=8, bool blind=true, bool doBands=true, bool baseline=false, bool spin=false, bool useBinnedData=false, bool reallyUseThisObsoleteScript=false){

  if(!reallyUseThisObsoleteScript){
    std::cerr << "You should probably be using makeBkgPlotsGeneric.py as makeBkgPlotsGeneric.C does not know how to treat WH and ZH separately." << std::endl;
    std::cerr << "If you *really* need to run this macro, pass it true as the last argument." << std::endl;
    std::cerr << "No output has been produced." << std::endl;
    return;
  }

	// Globals
	gROOT->SetStyle("Plain");
	gROOT->SetBatch(1);
	gStyle->SetOptStat(0);
  
  TFile *outf = TFile::Open("BkgPlotCanvs.root","RECREATE");

	RooMsgService::instance().setGlobalKillBelow(RooFit::MsgLevel(RooFit::FATAL));

	std::string * labels;
	
	std::string baselinelabels[9] = { "Both photons in barrel, R_{9}^{min} > 0.94"
					  ,"Both photons in barrel, R_{9}^{min} < 0.94"
					  ,"One or more photons in endcap, R_{9}^{min} > 0.94"
					  ,"One or more photons in endcap, R_{9}^{min} < 0.94"
		        ,"Dijet-tagged class BDT_{VBF} >= 0.985"
		        ,"Dijet-tagged class 0.93 <= BDT_{VBF} < 0.985"
            ,"Muon-tagged class"
            ,"Electron-tagged class"
            ,"MET-tagged class"
	};
	std::string massfactlabels[9] = { 
		"BDT_{#gamma#gamma} >= 0.91"
		,"0.79  <= BDT_{#gamma#gamma} < 0.91"
		,"0.49 <= BDT_{#gamma#gamma} < 0.79"
		,"-0.05  <= BDT_{#gamma#gamma} < 0.49"
		,"Dijet-tagged class BDT_{VBF} >= 0.985"
		,"Dijet-tagged class 0.93 <= BDT_{VBF} < 0.985"
		,"Muon-tagged class"
		,"Electron-tagged class"
		,"MET-tagged class"
	};
  std::string spinlabels[20] = {
    "#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{|cos(#theta*)| < 0.2}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.2 < |cos(#theta*)| < 0.375}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.375 < |cos(#theta*)| < 0.55}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.55 < |cos(#theta*)| < 0.75}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} > 0.94}{0.75 < |cos(#theta*)| < 0.1}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{|cos(#theta*)| < 0.2}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.2 < |cos(#theta*)| < 0.375}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.375 < |cos(#theta*)| < 0.55}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.55 < |cos(#theta*)| < 0.75}",
    "#splitline{|#eta|_{max} < 1.44, R_{9min} < 0.94}{0.75 < |cos(#theta*)| < 0.1}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{|cos(#theta*)| < 0.2}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.2 < |cos(#theta*)| < 0.375}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.375 < |cos(#theta*)| < 0.55}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.55 < |cos(#theta*)| < 0.75}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} > 0.94}{0.75 < |cos(#theta*)| < 0.1}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{|cos(#theta*)| < 0.2}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.2 < |cos(#theta*)| < 0.375}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.375 < |cos(#theta*)| < 0.55}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.55 < |cos(#theta*)| < 0.75}",
    "#splitline{|#eta|_{max} > 1.44, R_{9min} < 0.94}{0.75 < |cos(#theta*)| < 0.1}"
  };
	
	if( baseline ) { labels = baselinelabels; }
  else if ( spin ) { labels = spinlabels; }
	else { labels = massfactlabels; }
	
	TFile *fb = TFile::Open(filebkg.c_str());
	TFile *fs = ( filesig.empty() ? fb : TFile::Open(filesig.c_str()) );
	
	RooWorkspace *w_bkg  = (RooWorkspace*) fb->Get("cms_hgg_workspace");
//	w_bkg->Print();

	RooRealVar *x = (RooRealVar*) w_bkg->var("CMS_hgg_mass");
	RooRealVar *intL = (RooRealVar*) w_bkg->var("IntLumi");
	double lumi = intL->getVal()/1000.;

	TLatex *latex = new TLatex();	
	latex->SetTextSize(0.03);
	latex->SetNDC();
	
	TLatex *cmslatex = new TLatex();
	cmslatex->SetTextSize(0.03);
	cmslatex->SetNDC();

	double totalGGHinDIJET = 0;
	double totalVBFinDIJET = 0;
	double totalGGHinINCL = 0;
	double totalVBFinINCL = 0;

	double totalTTHinDIJET = 0;
	double totalWZHinDIJET = 0;
	double totalTTHinINCL = 0;
	double totalWZHinINCL = 0;

	for (int cat=0;cat<ncats;cat++){
		
		TCanvas *can = new TCanvas("c","",800,800);
		TLegend *leg = new TLegend(0.6,0.6,0.89,0.89);
		leg->SetFillColor(0);
		leg->SetBorderSize(0);

		// Get Dataset ->
		RooAbsData *data;
		if (useBinnedData) data =  (RooDataSet*)w_bkg->data(Form("data_mass_cat%d",cat));
		else  data =  (RooDataHist*)w_bkg->data(Form("roohist_data_mass_cat%d",cat));
		data->Print();

		// Background Pdf ->
		/// RooExtendPdf *bkg =  (RooExtendPdf*)w_bkg->pdf(Form("data_pol_model_8TeV_cat%d",cat));
		RooAbsPdf *bkg =  (RooAbsPdf*)w_bkg->pdf(Form("pdf_data_pol_model_8TeV_cat%d",cat));
		bkg->Print();
		bkg->fitTo(*data);
		RooFitResult *r = bkg->fitTo(*data,RooFit::Save(1));
    r->Print();
		
		// Get Signal pdf norms
		std::cout << "Getting Signal Components" << std::endl;
		TH1F *gghnorm = (TH1F*)fs->Get(Form("th1f_sig_ggh_mass_m125_cat%d",cat));
		TH1F *vbfnorm = (TH1F*)fs->Get(Form("th1f_sig_vbf_mass_m125_cat%d",cat));
		TH1F *wzhnorm = (TH1F*)fs->Get(Form("th1f_sig_wzh_mass_m125_cat%d",cat));
		TH1F *tthnorm = (TH1F*)fs->Get(Form("th1f_sig_tth_mass_m125_cat%d",cat));
    // Spin specials
    TH1F *gghgravnorm = NULL;
    if (spin){
      gghgravnorm = (TH1F*)fs->Get(Form("th1f_sig_gg_grav_mass_m125_cat%d",cat));
    }
		
		if (cat<=3){
			totalGGHinINCL+=gghnorm->Integral();
			totalVBFinINCL+=vbfnorm->Integral();
			totalTTHinINCL+=tthnorm->Integral();
			totalWZHinINCL+=wzhnorm->Integral();
		}else{
			totalGGHinDIJET+=gghnorm->Integral();
			totalVBFinDIJET+=vbfnorm->Integral();
			totalTTHinDIJET+=tthnorm->Integral();
			totalWZHinDIJET+=wzhnorm->Integral();
		}
		
		std::cout << "Rescaling Signal Components" << std::endl;
		gghnorm->Add(vbfnorm);
		gghnorm->Add(wzhnorm);
		gghnorm->Add(tthnorm);


		TH1F *allsig = (TH1F*)gghnorm->Clone();
		allsig->Rebin(2);
		allsig->SetLineColor(4);allsig->SetFillColor(38);allsig->SetFillStyle(3001) ;allsig->SetLineWidth(2);
    if (spin) {
      gghgravnorm->Rebin(2);
      allsig->SetLineColor(38);
      gghgravnorm->SetLineColor(kRed); gghgravnorm->SetLineWidth(2);
		  allsig->Scale(5.);
      gghgravnorm->Scale(5.);
    }
    /// allsig->SetLineColor(1);
		/// allsig->SetFillColor(38);
		TH1F dumData("d","",80,100,180); dumData.Sumw2();dumData.SetMarkerSize(1.0);dumData.SetMarkerStyle(20);dumData.SetLineWidth(3);
		dumData.Fill(101);
		// TH1F dumSignal("s","",80,100,180); dumSignal.SetLineColor(4);dumSignal.SetFillColor(38);dumSignal.SetFillStyle(3001) ;dumSignal.SetLineWidth(2);
		TH1F dum1Sig("1s","",80,100,180); dum1Sig.SetFillColor(kYellow);dum1Sig.SetFillStyle(1001);
		TH1F dum2Sig("2s","",80,100,180); dum2Sig.SetFillColor(kGreen);dum2Sig.SetFillStyle(1001);
		TH1F dumBkg("b","",80,100,180); dumBkg.SetLineColor(kRed);dumBkg.SetLineWidth(3);
		dumBkg.Draw("P");// dumSignal.Draw("LFsame");
		dumBkg.Draw("Fsame");dum1Sig.Draw("Fsame");dum2Sig.Draw("Lsame");

		// Plot background
		RooPlot *frame = x->frame();

		std::cout << "Plotting Components" << std::endl;
		data->plotOn(frame,RooFit::Binning(80),RooFit::Invisible());
		/// bkg->plotOn(frame,RooFit::VisualizeError(*r,2,1),RooFit::FillColor(kGreen));
		/// bkg->plotOn(frame,RooFit::VisualizeError(*r,1,1),RooFit::FillColor(kYellow));
		bkg->plotOn(frame,RooFit::LineColor(kRed));
		TGraphAsymmErrors *onesigma = 0, *twosigma = 0;
		if( doBands ) {
			onesigma = new TGraphAsymmErrors();
			twosigma = new TGraphAsymmErrors();
			TString name=Form("cat%d",cat);
      doBandsFit(onesigma, twosigma, x, bkg, dynamic_cast<RooCurve*>(frame->getObject(frame->numItems()-1)), 
				   data, frame, name );
		}
		if( blind ) {
			x->setRange("unblind_up",150,180);
			data->plotOn(frame,RooFit::Binning(80),RooFit::CutRange("unblind_up"));
			x->setRange("unblind_down",100,110);
			data->plotOn(frame,RooFit::Binning(80),RooFit::CutRange("unblind_down"));
		} else {
			data->plotOn(frame,RooFit::Binning(80));
		}
		///// x->setRange("unblind_1",100,110);
		///// x->setRange("unblind_2",150,180);
		///// data->plotOn(frame,RooFit::Binning(80),RooFit::CutRange("unblind_1"));
		///// data->plotOn(frame,RooFit::Binning(80),RooFit::CutRange("unblind_2"));

		frame->SetTitle("");
		frame->GetXaxis()->SetTitle("m_{#gamma#gamma} (GeV)");
		frame->GetXaxis()->SetNdivisions(5,5,0);
		frame->GetYaxis()->SetTitle("Events / (1 GeV)");
		frame->GetYaxis()->SetTitleOffset(1.2);
		
		leg->AddEntry(&dumData,"Data","PEL");
		leg->AddEntry(&dumBkg,"Bkg Model","L");
		leg->AddEntry(&dum1Sig,"#pm 1#sigma","F");
		leg->AddEntry(&dum2Sig,"#pm 2#sigma","F");
		/// leg->AddEntry(&dumSignal,"1xSM m_{H} = 125 GeV","F");
		if (spin){
      leg->AddEntry(allsig,"0^{+} x 5 m_{H} = 125 GeV","F");
      leg->AddEntry(gghgravnorm,"2^{+}_{m}(gg) x 5 m_{H} = 125 GeV","F");
    }
    else {
      leg->AddEntry(allsig,"1xSM m_{H} = 125 GeV","F");
	  }

		frame->Draw();
		frame->SetMinimum(0.0001);
 		if( doBands ) {
 			twosigma->SetLineColor(kGreen);
 			twosigma->SetFillColor(kGreen);
 			twosigma->SetMarkerColor(kGreen);
 			twosigma->Draw("L3 SAME");     
 			
 			onesigma->SetLineColor(kYellow);
 			onesigma->SetFillColor(kYellow);
 			onesigma->SetMarkerColor(kYellow);
 			onesigma->Draw("L3 SAME");
 			frame->Draw("same");
 		}
		allsig->Draw("samehistF");
    if (spin) gghgravnorm->Draw("samehistF");
		leg->Draw();
		cmslatex->DrawLatex(0.2,0.85,Form("#splitline{CMS Preliminary}{#sqrt{s} = 8TeV L = %2.1ffb^{-1}}",lumi));
		latex->DrawLatex(0.2,0.75,labels[cat].c_str());
    if (spin) {
      can->SetName(Form("spincat%d",cat));
      outf->cd();
      can->Write();
      can->SaveAs(Form("spincat%d.pdf",cat));
      can->SaveAs(Form("spincat%d.png",cat));
      can->SaveAs(Form("spincat%d.C",cat));
    }
    else {
      can->SaveAs(Form( (baseline ? "baselinecat%d.pdf" : "massfacmvacat%d.pdf"),cat));
      can->SaveAs(Form( (baseline ? "baselinecat%d.png" : "massfacmvacat%d.png"),cat));
    }
	}

	// JET ID Systematics
  if (!spin) {
    std::cout << "The following can be used (nick knows what they mean) as the JET Migration systematics" <<std::endl;
    std::cout << "XXX " << 1-(0.7*totalGGHinDIJET/(totalGGHinINCL))<<std::endl;
    std::cout << "YYY " << 1-(0.7*totalTTHinDIJET/(totalTTHinINCL))<<std::endl;
    std::cout << "MMM " << 1-(0.1*totalVBFinDIJET/(totalVBFinINCL))<<std::endl;
    std::cout << "NNN " << 1-(0.1*totalWZHinDIJET/(totalWZHinINCL))<<std::endl;
  }
  outf->Close();
  fb->Close();
}


