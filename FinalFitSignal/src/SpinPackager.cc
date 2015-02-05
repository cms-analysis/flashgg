#include "RooDataSet.h"
#include "RooExtendPdf.h"
#include "RooAddPdf.h"
#include "RooArgList.h"

#include "../interface/SpinPackager.h"

using namespace std;
using namespace RooFit;

SpinPackager::SpinPackager(RooWorkspace *ws, vector<string> smProcs, vector<string> gravProcs, int nCats, int mh, int sqrts):
	ws_(ws),
	smProcs_(smProcs),
	gravProcs_(gravProcs),
	nCats_(nCats),
	mh_(mh),
	sqrts_(sqrts)
{
}

void SpinPackager::package(vector<string> procs, string label) {

	RooDataSet *allData = 0;
	RooArgList *sumPdfs = new RooArgList();
	for (int cat=0; cat<nCats_; cat++){
		RooDataSet *allDataThisCat = 0;
		RooArgList *sumPdfsThisCat = new RooArgList();
		for (vector<string>::iterator proc=procs.begin(); proc!=procs.end(); proc++){
			
			RooDataSet *thisData = (RooDataSet*)ws_->data(Form("sig_%s_mass_m%d_cat%d",proc->c_str(),mh_,cat));
			if (cat==0 && proc==procs.begin()) {
				allData = (RooDataSet*)thisData->Clone(Form("sig_%s_mass_m%d_AllCats",label.c_str(),mh_));
			}
			else {
				allData->append(*thisData);
			}
			if (proc==procs.begin()) {
				allDataThisCat = (RooDataSet*)thisData->Clone(Form("sig_%s_mass_m%d_cat%d",label.c_str(),mh_,cat));
			}
			else {
				allDataThisCat->append(*thisData);
			}
			
			RooExtendPdf *thisPdf = (RooExtendPdf*)ws_->pdf(Form("extendhggpdfsmrel_%dTeV_%s_cat%dThisLumi",sqrts_,proc->c_str(),cat));
			sumPdfs->add(*thisPdf);
			sumPdfsThisCat->add(*thisPdf);
		}
		RooAddPdf *sumPdfThisCat = new RooAddPdf(Form("sigpdfrel_%s_cat%d_allProcs",label.c_str(),cat),"",*sumPdfsThisCat);
		ws_->import(*sumPdfThisCat,RecycleConflictNodes());
		ws_->import(*allDataThisCat);
	}
	RooAddPdf *sumPdf = new RooAddPdf(Form("sigpdfrel_%s_AllCats_allProcs",label.c_str()),"",*sumPdfs);
	ws_->import(*sumPdf,RecycleConflictNodes());
	ws_->import(*allData);

}

void SpinPackager::packageOutput() {
	package(smProcs_,"sm");
	for (vector<string>::iterator gravProc=gravProcs_.begin(); gravProc!=gravProcs_.end(); gravProc++){
		vector<string> dummy;
		dummy.push_back(*gravProc);
		package(dummy,*gravProc);
	}
}
