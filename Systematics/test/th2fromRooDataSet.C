#include "RooDataSet.h"
#include "RooFormulaVar.h"
#include "RooRealVar.h"
#include <string>
#include "RooWorkspace.h"
#include <vector>
#include "TH2F.h"
#include "TH1F.h"
#include "RooBinning.h"

using namespace std;

TH1F* th2fromRooDataSet(RooWorkspace* w, RooDataSet* d, string expr1, std::vector<std::string > listvar1, int nbins1, float lo1, float hi1,  string expr2="", std::vector<std::string > listvar2= std::vector<std::string>(), int nbins2=1, float lo2=0., float hi2=1.  ){

  std::vector<RooRealVar* > var1v, var2v;
  RooArgList* argl1 =0;
  RooArgList* arg1l = new RooArgList();
  for(unsigned int i=0; i<listvar1.size(); i++){
    var1v.push_back( (RooRealVar*)w->var( (listvar1[i]).c_str() ) );
    arg1l->add(*((RooRealVar*)w->var( (listvar1[i]).c_str() ) ) );
  }
  arg1l->Print("V");
  //  RooFormulaVar* a = new RooFormulaVar("a", "a", expr1.c_str(), *arg1l);
  RooFormulaVar* a = new RooFormulaVar(expr1.c_str(), expr1.c_str(), expr1.c_str(), *arg1l);
  RooRealVar* a_var = (RooRealVar*)d->addColumn(*a);

  TH1F* h2;
  if(listvar2.size()>0){
    RooArgList* arg2l = new RooArgList();
    for(unsigned int i=0; i<listvar2.size(); i++){
      var2v.push_back( (RooRealVar*)w->var( (listvar2[i]).c_str() ) );
      arg2l->add(*((RooRealVar*)w->var( (listvar2[i]).c_str()) ) );
    }
    arg2l->Print("V");
    //    RooFormulaVar* b = new RooFormulaVar("b", "b", expr2.c_str(), *arg2l);
    RooFormulaVar* b = new RooFormulaVar(expr2.c_str(), expr2.c_str(), expr2.c_str(), *arg2l);
    RooRealVar* b_var = (RooRealVar*)d->addColumn(*b);
    h2 = (TH1F*) d->createHistogram("h2",*a_var,RooFit::Binning(nbins1,lo1,hi1), RooFit::YVar(*b_var, RooFit::Binning(nbins2,lo2,hi2)));
  }
  
  
  else{
    h2 = (TH1F*) d->createHistogram("h2",*a_var,RooFit::Binning(nbins1,lo1,hi1));
  }
  return h2;

}
