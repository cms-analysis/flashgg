#include "SplitMiniTree.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooExtendPdf.h"
#include "RooDataHist.h"
#include "TH1F.h"

#include <algorithm>

SplitMiniTree::SplitMiniTree(TString fileName, TString wsFile, TString wsName, TString pdfName, TString dsName, TString outName) :
	HggMiniTree((TTree *)TFile::Open(fileName)->Get("hgg_mini_tree")),
	fileName_(fileName), wsName_(wsName), pdfName_(pdfName), dsName_(dsName), outName_(outName)
{
	if( outName_ == "" ) { outName_ = fileName_; }
	outName_.ReplaceAll(".root","");
	TFile * wsFin = TFile::Open(wsFile);
	
	ws_ = (RooWorkspace*)wsFin->Get(wsName);

	ncat_ = 6;
	minrange_ = 100.;
	maxrange_ = 180.;
	nbins_    = 160;
}


void SplitMiniTree::addToPartition(int ipart, int prun, int plumis, int pevent)
{
	partitions_.resize(ipart+1);
	partitions_[ipart].push_back(EvIndex(prun,plumis,pevent));
}


void SplitMiniTree::splitCopy()
{
	std::cout << "Reading from workspace" << std::endl;
	readFromWs();
	
	Long64_t nentries = fChain->GetEntriesFast();
	
	for(size_t ipart=0; ipart<partitions_.size(); ++ipart){
		bookPartition(ipart);
	}
	

	std::cout << "Number of events in the tree " << nentries << "\n" 
		  << "Numberof partitions: " << partitions_.size() << std::endl;
	
	Long64_t nbytes = 0, nb = 0;
	std::cout << "Looping over events " << std::endl; 
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		EvIndex thisEvent(run,lumis,event);
		std::vector<std::vector<EvIndex> >::iterator beg=partitions_.begin();
		std::vector<std::vector<EvIndex> >::iterator ipart=partitions_.begin();
		std::vector<std::vector<EvIndex> >::iterator end=partitions_.end();
		
		for(;ipart!=end;++ipart) {
			if( find(ipart->begin(), ipart->end(), thisEvent) == ipart->end() ) {
				fillPartition(ipart-beg);
			}
		}
	}
	
	std::cout << "Fitting and saving " << std::endl; 
	for(size_t ipart=0; ipart<partitions_.size(); ++ipart){
		fitAndSavePartition(ipart);
	}
	
}

void SplitMiniTree::readFromWs()
{
	mass_ = ws_->var("CMS_hgg_mass");
	mass_->setRange(minrange_,maxrange_);
	mass_->setBins(nbins_);
	mass_->Print();
	for(int icat=0; icat<ncat_; ++icat) {
		pdfs_.push_back( ws_->pdf( Form(pdfName_,icat) ) );
		RooRealVar * norm = new RooRealVar(Form(pdfName_+"_norm",icat),Form(pdfName_+"_norm",icat),1.,0.,100000.);
		RooExtendPdf * extpdf = new RooExtendPdf(Form("extended_"+pdfName_,icat),Form("extended_"+pdfName_,icat),*pdfs_.back(),*norm);
		norms_.push_back(norm);
		extpdfs_.push_back(extpdf);
	}
}

void SplitMiniTree::bookPartition(size_t ipart)
{
	partData_.resize(ipart+1);
	for(int ii=0; ii<ncat_; ++ii) {
		TH1F * data = new TH1F(Form("data_part%d_cat%d",(int)ipart,ii),Form("data_part%d_cat%d",(int)ipart,ii),nbins_,minrange_,maxrange_);
		partData_[ipart].push_back(data);
	}
}

void SplitMiniTree::fillPartition(size_t ipart)
{
	partData_[ipart][category]->Fill(CMS_hgg_mass);
}

void SplitMiniTree::fitAndSavePartition(size_t ipart)
{
	RooMsgService::instance().setGlobalKillBelow(RooFit::MsgLevel(RooFit::WARNING));
	RooMsgService::instance().setSilentMode(true);
	
	std::cout << "Saving workspace for partition " << ipart << std::endl;
	RooWorkspace partws(wsName_,wsName_);
	partws.import(*mass_);

	std::vector<RooDataHist *> roohist(ncat_);
	int tot = 0;
	for(int ii=0; ii<ncat_; ++ii) {
		roohist[ii] = new RooDataHist(Form(dsName_,ii),Form(dsName_,ii),RooArgList(*mass_),partData_[ipart][ii]);
		std::cout << "Cat: " << ii <<  " nevents: " << roohist[ii]->sumEntries() << std::endl;
		tot += roohist[ii]->sumEntries();
		extpdfs_[ii]->fitTo( *roohist[ii] );
		partws.import(*extpdfs_[ii]);
		partws.import(*roohist[ii]);
	}
	std::cout << "Tot    nevents: " << tot << std::endl;
	
	partws.writeToFile( Form("%s_part%d.root", outName_.Data(), (int)ipart) );
	
	for(int ii=0; ii<ncat_; ++ii) {
		delete roohist[ii];
	}
}

