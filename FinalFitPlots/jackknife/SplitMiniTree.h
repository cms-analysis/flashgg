#ifndef _SplitMiniTree_
#define _SplitMiniTree_

#include "HggMiniTree.C"
#include "RooWorkspace.h"
#include "TString.h"

struct EvIndex
{
	EvIndex() {};
	EvIndex(int a, int b, int c) : run(a), lumis(b), event(c) {};
	
	bool operator == (const EvIndex & rhs ) { return run == rhs.run && lumis == rhs.lumis && event == rhs.event; }
	
	int run, lumis, event;

};

class SplitMiniTree : public HggMiniTree
{
public:
	SplitMiniTree(TString fileName, TString wsFile, TString wsName, TString pdfName, TString dsName, TString outName="");
	
	void addToPartition(int ipart, int run, int lumis, int event);
	
	void splitCopy();
	
	void setBinning(int nbins, float min, float max) { 
		nbins_ = nbins;
		minrange_ = min;
		maxrange_ = max;
	};
	
	void setNcat(int ncat) { ncat_ = ncat; }
	
private:
	void bookPartition(size_t ipart);
	void fillPartition(size_t ipart);
	void fitAndSavePartition(size_t ipart);
	
	void readFromWs();

	RooWorkspace * ws_;
	RooRealVar   * mass_;
	std::vector<RooAbsPdf *> pdfs_;
	std::vector<RooRealVar *> norms_;
	std::vector<RooAbsPdf *> extpdfs_;
	

	int   nbins_, ncat_;
	float minrange_, maxrange_;
	TString fileName_, pdfName_, dsName_, wsName_, outName_;
	std::vector<std::vector<EvIndex> > partitions_;
	std::vector<std::vector<TH1 *> > partData_;
	
};


#endif
