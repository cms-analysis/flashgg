#include "../interface/FunctionHelpers.h"
#include "TTreeFormula.h"
#include "TFitResult.h"
#include "TCanvas.h"

using namespace std;


HistoConverter::~HistoConverter()
{
    if( sp_ )   { delete sp_; }
    if( hist_ ) { delete hist_; }
    if( g_ )    { delete g_; }
}

// ------------------------------------------------------------------------------------------------
TH1 *integrate1D( TH1 *h, bool normalize )
{
    TH1 *ret = ( TH1 * )h->Clone( Form( "%s_cdf", h->GetName() ) );
    ret->SetDirectory( 0 );
    for( int xx = ret->GetNbinsX() - 1; xx >= 0; --xx ) {
        ret->SetBinContent( xx, ret->GetBinContent( xx ) + ret->GetBinContent( xx + 1 ) );
    }
    if( normalize ) { ret->Scale( 1. / h->Integral() ); }
    return ret;
}

// ------------------------------------------------------------------------------------------------
TH2 *integrate2D( TH2 *h, bool normalize )
{
    TH2 *ret = ( TH2 * )h->Clone( Form( "%s_cdf", h->GetName() ) );
    ret->SetDirectory( 0 );
    for( int xx = ret->GetNbinsX(); xx >= 0; --xx ) {
        for( int yy = ret->GetNbinsY() - 1; yy >= 0; --yy ) {
            ret->SetBinContent( xx, yy, ret->GetBinContent( xx, yy ) + ret->GetBinContent( xx, yy + 1 ) );
        }
    }
    for( int yy = ret->GetNbinsY(); yy >= 0; --yy ) {
        for( int xx = ret->GetNbinsX() - 1; xx >= 0; --xx ) {
            ret->SetBinContent( xx, yy, ret->GetBinContent( xx, yy ) + ret->GetBinContent( xx + 1, yy ) );
        }
    }
    if( normalize ) { ret->Scale( 1. / h->Integral() ); }
    return ret;
}


// ------------------------------------------------------------------------------------------------
DecorrTransform::DecorrTransform( TH2 *histo, float ref, bool doRef, bool doRatio, bool invert ) : doRatio_( doRatio )
{
    doRef_ = doRef;
    //    std::cout<<"doRef_ is "<<doRef_<<std::endl;
    refbin_ = histo->GetXaxis()->FindBin( ref );
    hist_ = histo;
    //    hist_->Print();

    double miny = histo->GetYaxis()->GetXmin();

    //    std::cout<<"miny "<<miny<<std::endl;

    double maxy = histo->GetYaxis()->GetXmax();
    //    std::cout<<"maxy "<<maxy<<std::endl;
    for( int ii = 0; ii < histo->GetNbinsX() + 2; ++ii ) {
        TH1 *proj = histo->ProjectionY( Form( "%s_%d", histo->GetName(), ii ), ii, ii );
        //    proj->Print();
        /// dirtr_.push_back(cdf<GraphToTF1>(proj,miny,maxy));
        miny = proj->GetXaxis()->GetBinLowEdge( proj->FindFirstBinAbove(0.) ); // +1
        //        miny = proj->GetXaxis()->GetBinLowEdge( proj->FindFirstBinAbove(0.) +1 );
        if( invert ) {
            dirtr_.push_back( cdfInv( proj, miny, maxy ) );
        } else {
            dirtr_.push_back( cdf( proj, miny, maxy ) );
        }
        if( ii == refbin_ && ref!=0. ) {
            //            std::cout<<"we DON'T use a flat inverse"<<std::endl;
            /// invtr_ = cdfInv<GraphToTF1>(proj,miny,maxy);
            if( invert ) {
                invtr_ = cdf( proj, miny, maxy );
            } else {
                invtr_ = cdfInv( proj, miny, maxy );
            }
            //////      cout << invtr_->eval( 0. ) << " " << invtr_->eval( 0.5 )  << " " << invtr_->eval( 1. ) << endl;

        }

        delete proj;
    }
}

// ------------------------------------------------------------------------------------------------
double DecorrTransform::operator()( double *x, double *p )
{
    double ret = x[1];
    //    std::cout<<"doRef_ is "<<doRef_<<std::endl;
    if(doRef_){
        ret = invtr_->eval( getConverter( x[0] )->eval( x[1] ) );
        //        std::cout<<" getConverter( x[0] )->eval( x[1] ) is "<< getConverter( x[0] )->eval( x[1] )<<std::endl;
        //        std::cout<<"ret is "<<ret<<std::endl;
    }
    else{
        //        std::cout<<"operator()"<<std::endl;
        //        std::cout<<"x[0]"<< x[0] <<std::endl;
        //        std::cout<<"x[1]"<< x[1] <<std::endl;
        //        std::cout<<"converter "<< getConverter( x[0] )->Print() <<std::endl;
        //        std::cout<<"ret "<< getConverter( x[0] )->eval( x[1] ) <<std::endl;
        
        ret =  getConverter( x[0] )->eval( x[1] );
    }
    return ( doRatio_ ? ret / x[1] : ret );
}

// ------------------------------------------------------------------------------------------------
HistoConverter *DecorrTransform::clone() const
{
    return new DecorrTransform( *this );
}




// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

