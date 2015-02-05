#include "TCanvas.h"
#include "TGraph.h"

#include "RooAddition.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooExtendPdf.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooSpline1D.h"

#include "../interface/Packager.h"

#include <algorithm>

using namespace std;
using namespace RooFit;

Packager::Packager(RooWorkspace *ws, vector<string> procs, int nCats, int mhLow, int mhHigh, vector<int> skipMasses, bool is2011, bool skipPlots, string outDir, 
		   RooWorkspace *wsMerge, const vector<int>& cats ):
  outWS(ws),
  mergeWS(wsMerge),
  procs_(procs),
  nCats_(nCats),
  cats_(cats),
  mhLow_(mhLow),
  mhHigh_(mhHigh),
  is2011_(is2011),
	skipPlots_(skipPlots),
  outDir_(outDir),
  skipMasses_(skipMasses)
{
	if (is2011) sqrts_=7;
	else sqrts_=8;
	normalization = new Normalization_8TeV();
	normalization->Init(sqrts_);
}

Packager::~Packager(){}

bool Packager::skipMass(int mh){
	for (vector<int>::iterator it=skipMasses_.begin(); it!=skipMasses_.end(); it++) {
		if (*it==mh) return true;
	}
	return false;
}

void Packager::packageOutput(){

  vector<string> expectedObjectsNotFound;

  // sum datasets first
  for (int mh=mhLow_; mh<=mhHigh_; mh+=5){
		if (skipMass(mh)) continue;
    RooDataSet *allDataThisMass = 0;
    for (int cat=0; cat<nCats_; cat++) {
      RooDataSet *allDataThisCat = NULL;
      bool merge = mergeWS != 0 && ( find(cats_.begin(),cats_.end(),cat) == cats_.end() );
      for (vector<string>::iterator proc=procs_.begin(); proc!=procs_.end(); proc++){
	RooDataSet *tempData = 0;
	if( merge ) { 
		      tempData = (RooDataSet*)mergeWS->data(Form("sig_%s_mass_m%d_cat%d",proc->c_str(),mh,cat));
		      outWS->import(*tempData);
	} else {
		      tempData = (RooDataSet*)outWS->data(Form("sig_%s_mass_m%d_cat%d",proc->c_str(),mh,cat));
	}
        if (!tempData) {
          cerr << "WARNING -- dataset: " << Form("sig_%s_mass_m%d_cat%d",proc->c_str(),mh,cat) << " not found. It will be skipped" << endl;
          expectedObjectsNotFound.push_back(Form("sig_%s_mass_m%d_cat%d",proc->c_str(),mh,cat));
          continue;
        }
        if (cat==0 && proc==procs_.begin()) allDataThisMass = (RooDataSet*)tempData->Clone(Form("sig_mass_m%d_AllCats",mh));
        else allDataThisMass->append(*tempData);
        if (proc==procs_.begin()) allDataThisCat = (RooDataSet*)tempData->Clone(Form("sig_mass_m%d_cat%d",mh,cat));
        else allDataThisCat->append(*tempData);
      }
      if (!allDataThisCat) {
        cerr << "WARNING -- allData for cat " << cat << " is NULL. Probably because the relevant datasets couldn't be found. Skipping.. " << endl;
        continue;
      }
      outWS->import(*allDataThisCat);
    }
    if (!allDataThisMass) {
      cerr << "WARNING -- allData for mass " << mh << " is NULL. Probably because the relevant datasets couldn't be found. Skipping.. " << endl;
      continue;
    }
    outWS->import(*allDataThisMass);
  }

  // now create pdf sums (these don't the relative amounts as just used for plotting so can use ThisLum versions)
  RooArgList *sumPdfs = new RooArgList();
  RooArgList *runningNormSum = new RooArgList();
  for (int cat=0; cat<nCats_; cat++){
    bool merge = mergeWS != 0 && ( find(cats_.begin(),cats_.end(),cat) == cats_.end() );
    RooWorkspace * inWS = ( merge ? mergeWS : outWS );
    RooArgList *sumPdfsThisCat = new RooArgList();
    for (vector<string>::iterator proc=procs_.begin(); proc!=procs_.end(); proc++){
      
      // sum eA
      RooSpline1D *norm = (RooSpline1D*)inWS->function(Form("hggpdfsmrel_%dTeV_%s_cat%d_norm",sqrts_,proc->c_str(),cat));
      if (!norm) {
        cerr << "WARNING -- ea: " << Form("hggpdfsmrel_%dTeV_%s_cat%d_norm",sqrts_,proc->c_str(),cat) << "not found. It will be skipped" << endl;
      }
      else {
        runningNormSum->add(*norm);
      }
      
      // sum pdf
      RooExtendPdf *tempPdf = (RooExtendPdf*)inWS->pdf(Form("extendhggpdfsmrel_%dTeV_%s_cat%dThisLumi",sqrts_,proc->c_str(),cat));
      if (!tempPdf) {
        cerr << "WARNING -- pdf: " << Form("extendhggpdfsmrel_%dTeV_%s_cat%d",sqrts_,proc->c_str(),cat) << " not found. It will be skipped" << endl;
        expectedObjectsNotFound.push_back(Form("extendhggpdfsmrel_%dTeV_%s_cat%d",sqrts_,proc->c_str(),cat));
        continue;
      }
      if( merge ) {
	      outWS->import(*norm);
	      outWS->import(*tempPdf,RecycleConflictNodes());
      }
      sumPdfsThisCat->add(*tempPdf);
      sumPdfs->add(*tempPdf);
    }
    if (sumPdfsThisCat->getSize()==0){
      cerr << "WARNING -- sumPdfs for cat " << cat << " is EMPTY. Probably because the relevant pdfs couldn't be found. Skipping.. " << endl;
      continue;
    }
    // Dont put sqrts here as combine never uses this (but our plotting scripts do)
    RooAddPdf *sumPdfsPerCat = new RooAddPdf(Form("sigpdfrelcat%d_allProcs",cat),Form("sigpdfrelcat%d_allProcs",cat),*sumPdfsThisCat);
    outWS->import(*sumPdfsPerCat,RecycleConflictNodes());
  }
  if (sumPdfs->getSize()==0){
    cerr << "WARNING -- sumAllPdfs is EMPTY. Probably because the relevant pdfs couldn't be found. Skipping.. " << endl;
  }
  else {
		// Dont put sqrts here as combine never uses this (but our plotting scripts do)
    RooAddPdf *sumPdfsAllCats = new RooAddPdf("sigpdfrelAllCats_allProcs","sigpdfrelAllCats_allProcs",*sumPdfs);
    outWS->import(*sumPdfsAllCats,RecycleConflictNodes());
  }

  if (runningNormSum->getSize()==0){
    cerr << "WARNING -- runningNormSum is EMPTY. Probably because the relevant normalizations couldn't be found. Skipping.. " << endl;
  }
  else {
    RooAddition *normSum = new RooAddition("normSum","normSum",*runningNormSum);
    outWS->import(*normSum,RecycleConflictNodes());
   
	 	if (!skipPlots_) {
			RooRealVar *MH = (RooRealVar*)outWS->var("MH");
			RooRealVar *intLumi = (RooRealVar*)outWS->var("IntLumi");
			RooAddition *norm = (RooAddition*)outWS->function("normSum");
			TGraph *effAccGraph = new TGraph();
			TGraph *expEventsGraph = new TGraph();
			int p=0;
			for (double mh=mhLow_; mh<mhHigh_+0.5; mh+=1){
				MH->setVal(mh);
				expEventsGraph->SetPoint(p,mh,intLumi->getVal()*norm->getVal());
				effAccGraph->SetPoint(p,mh,norm->getVal()/(normalization->GetXsection(mh)*normalization->GetBR(mh)));
				p++;
			}
			TCanvas *canv = new TCanvas();
			effAccGraph->SetLineWidth(3);
			effAccGraph->GetXaxis()->SetTitle("m_{H} (GeV)");
			effAccGraph->GetYaxis()->SetTitle("efficiency #times acceptance");
			effAccGraph->Draw("AL");
			canv->Print(Form("%s/effAccCheck.pdf",outDir_.c_str()));
			canv->Print(Form("%s/effAccCheck.png",outDir_.c_str()));
			expEventsGraph->SetLineWidth(3);
			expEventsGraph->GetXaxis()->SetTitle("m_{H} (GeV)");
			expEventsGraph->GetYaxis()->SetTitle(Form("Expected Events for %4.1ffb^{-1}",intLumi->getVal()/1000.));
			expEventsGraph->Draw("AL");
			canv->Print(Form("%s/expEventsCheck.pdf",outDir_.c_str()));
			canv->Print(Form("%s/expEventsCheck.png",outDir_.c_str()));
			makePlots();
		}
  }
}

void Packager::makePlots(){
	RooRealVar *mass = (RooRealVar*)outWS->var("CMS_hgg_mass");
	RooRealVar *MH = (RooRealVar*)outWS->var("MH");
	RooAddPdf *sumPdfsAllCats = (RooAddPdf*)outWS->pdf("sigpdfrelAllCats_allProcs");
	map<int,RooDataSet*> dataSets;
	for (int m=mhLow_; m<=mhHigh_; m+=5){
		if (skipMass(m)) continue;
		RooDataSet *data = (RooDataSet*)outWS->data(Form("sig_mass_m%d_AllCats",m));
		dataSets.insert(make_pair(m,data));
	}
	makePlot(mass,MH,sumPdfsAllCats,dataSets,"all");

	for (int cat=0; cat<nCats_; cat++){
		RooAddPdf *sumPdfsCat = (RooAddPdf*)outWS->pdf(Form("sigpdfrelcat%d_allProcs",cat));
		map<int,RooDataSet*> dataSetsCat;
		for (int m=mhLow_; m<=mhHigh_; m+=5){
			if (skipMass(m)) continue;
			RooDataSet *data = (RooDataSet*)outWS->data(Form("sig_mass_m%d_cat%d",m,cat));
			dataSetsCat.insert(make_pair(m,data));
		}
		makePlot(mass,MH,sumPdfsCat,dataSetsCat,Form("cat%d",cat));
	}
}

void Packager::makePlot(RooRealVar *mass, RooRealVar *MH, RooAddPdf *pdf, map<int,RooDataSet*> data, string name){
	
  TCanvas *canv = new TCanvas();
	RooPlot *dataPlot = mass->frame(Title(name.c_str()),Range(100,160));
	for (map<int,RooDataSet*>::iterator it=data.begin(); it!=data.end(); it++){
		int mh = it->first;
		RooDataSet *dset = it->second;
		dset->plotOn(dataPlot,Binning(160));
		MH->setVal(mh);
		pdf->plotOn(dataPlot);
	}
	dataPlot->Draw();
	canv->Print(Form("%s/%s_fits.pdf",outDir_.c_str(),name.c_str()));
	canv->Print(Form("%s/%s_fits.png",outDir_.c_str(),name.c_str()));
  
	RooPlot *pdfPlot = mass->frame(Title(name.c_str()),Range(100,160));
	pdfPlot->GetYaxis()->SetTitle(Form("Pdf projection / %2.1f GeV",(mass->getMax()-mass->getMin())/160.));
  for (int mh=mhLow_; mh<=mhHigh_; mh++){
    MH->setVal(mh);
		// to get correct normlization need to manipulate with bins and range
    pdf->plotOn(pdfPlot,Normalization(mass->getBins()/160.*(mass->getMax()-mass->getMin())/60.,RooAbsReal::RelativeExpected));
  }
  pdfPlot->Draw();
	canv->Print(Form("%s/%s_interp.pdf",outDir_.c_str(),name.c_str()));
	canv->Print(Form("%s/%s_interp.png",outDir_.c_str(),name.c_str()));
	delete canv;
}
