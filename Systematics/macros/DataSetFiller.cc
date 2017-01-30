#include "../interface/DataSetFiller.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"

#include "TTreeFormula.h"

#include <iostream>
using namespace std;


RooDataHist* DataSetFiller::throwAsimov( double nexp, RooAbsPdf *pdf, RooRealVar *x, RooDataHist *asimov )
{
    RooArgSet mset( *x );
    if( asimov != 0 ) {
        asimov->reset();
    } else {
        asimov = new RooDataHist(Form("asimov_dataset_%s",pdf->GetName()),Form("asimov_dataset_%s",pdf->GetName()),mset);
    }
    pdf->fillDataHist( asimov, &mset, 1, false );

    for( int i = 0 ; i < asimov->numEntries() ; i++ ) {
        asimov->get( i ) ;

        // Expected data, multiply p.d.f by nEvents
        Double_t w = asimov->weight() * nexp;
        asimov->set( w, sqrt( w ) );
    }

    Double_t corr = nexp / asimov->sumEntries();
    for( int i = 0 ; i < asimov->numEntries() ; i++ ) {
        RooArgSet theSet = *( asimov->get( i ) );
        asimov->set( asimov->weight()*corr, sqrt( asimov->weight()*corr ) );
    }
    
    return asimov;
}

DataSetFiller::DataSetFiller(const char * name, const char * title, const RooArgList & variables, const char *weightVarName, bool fillTree) :
    vars_(variables),
    dataset_(new RooDataSet(name,title,RooArgSet(variables),weightVarName)),
    tree_(0)
{
    if( fillTree ) {
        std::string vars;
        size_t nvar = vars_.getSize();
        for(size_t ivar=0; ivar<nvar; ++ivar) {
            RooRealVar &var = dynamic_cast<RooRealVar &>( vars_[ivar] );
            if( ! vars.empty() ) { vars += ":"; }
            vars += var.GetName();
        }
        tree_ = new TNtuple(Form("tree_%s",name),Form("tree_%s",name),vars.c_str());
        // cout << vars << endl;
        treeBuf_.resize(nvar);
    }
}


DataSetFiller::DataSetFiller(RooDataSet * dset) :
    vars_(*(dset->get())),
    dataset_(dset),
    tree_(0)
{
}

//void DataSetFiller::fillFromTree(TTree * tree, const char * weightExpr, bool ignoreExpr, bool reduced)
void DataSetFiller::fillFromTree(TTree * tree, const char * weightExpr, bool ignoreExpr)
{
    size_t nvar = vars_.getSize();
    std::vector<TTreeFormula *> formulas(nvar);
    TTreeFormula * weight = (weightExpr != 0 ? new TTreeFormula("weight",weightExpr,tree) : 0);
    for(size_t ivar=0; ivar<nvar; ++ivar){
        RooRealVar &var = dynamic_cast<RooRealVar &>( vars_[ivar] );
        if( std::string(var.GetName()) == "weight" ) { 
            formulas[ivar] = weight;
        } else{
            formulas[ivar] = new TTreeFormula( var.GetName(), (ignoreExpr ? var.GetName() : var.GetTitle()), tree );
        }
    }
    

    for(int iev=0; iev<tree->GetEntries(); ++iev) {
        tree->GetEntry(iev);
        float wei = 1.;
        if( weight ) {
            wei = weight->EvalInstance();
        }
        if( wei == 0. ) { continue; }
        bool keep = true;
        for(size_t ivar=0; ivar<nvar; ++ivar){
            double val = formulas[ivar]->EvalInstance();
            RooRealVar & var = dynamic_cast<RooRealVar &>( vars_[ivar] );
            if( (var.hasMin() && val < var.getMin()) || (var.hasMax() && val > var.getMax()) ) { keep = false; break; }
            var.setVal( val  );
            if( tree_ ) { treeBuf_[ivar] = val; }
        }
        if( keep ) {
            if( tree_ ) { 
                tree_->Fill( &treeBuf_[0] );
            } else {
                dataset_->add( RooArgSet(vars_), wei );
            }
        }
    }
    
    for(size_t ivar=0; ivar<nvar; ++ivar){
        delete formulas[ivar];
    }
    //// if( weight ) { delete weight; }
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

