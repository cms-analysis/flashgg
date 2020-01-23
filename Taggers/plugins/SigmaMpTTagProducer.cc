#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/SigmaMpTTag.h"
#include "DataFormats/Common/interface/RefToPtr.h"


#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class SigmaMpTTagProducer : public EDProducer
    {

    public:
        SigmaMpTTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float, float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        string systLabel_;
        bool requireScaledPtCuts_;

        vector<double> boundaries_sigmaMoM;
        vector<double> boundaries_pToM;
        bool integratepT_;
    };

    SigmaMpTTagProducer::SigmaMpTTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        requireScaledPtCuts_   ( iConfig.getParameter<bool> ( "RequireScaledPtCuts" ) )
    {
        boundaries_sigmaMoM = iConfig.getParameter<vector<double > >( "BoundariesSigmaMoM" );
        //integratepT_ = iConfig.existsAs<vector<double >>("BoundariespToM") ? true: false;
        integratepT_ = true;
        if(!integratepT_){
            boundaries_pToM = iConfig.getParameter<vector<double > >( "BoundariespToM" );
        }
        assert( is_sorted( boundaries_sigmaMoM.begin(), boundaries_sigmaMoM.end() ) ); // we are counting on ascending order - update this to give an error message or exception
        assert( is_sorted( boundaries_pToM.begin(), boundaries_pToM.end() ) ); // we are counting on ascending order - update this to give an error message or exception
        produces<vector<SigmaMpTTag> >();
    }

    int SigmaMpTTagProducer::chooseCategory( float sigmaMoMvalue, float pToMvalue )
    {
        /// std::cout<<"sigmaM/M "<<sigmaMoMvalue<<", pT/M "<<pToMvalue<<std::endl;
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n1;
        int n2;
        for( n1 = 0 ; n1 < ( int )boundaries_sigmaMoM.size() ; n1++ ) {
            if( ( double )sigmaMoMvalue < boundaries_sigmaMoM[n1] ) { break; }
        }
        if(n1==0 || n1==(int)boundaries_sigmaMoM.size()){return -1;}//boundaries have to be provided including lowest and highest: if below lowest (0) or above highest (size) return -1 

        if(!integratepT_){
            for( n2 = 0 ; n2 < ( int )boundaries_pToM.size() ; n2++ ) {
                if( ( double )pToMvalue < boundaries_pToM[n2] ) { break; }
            }
            if(n2==0 || n2==(int)boundaries_pToM.size()){return -1;}//boundaries have to be provided including lowest and highest: if below lowest (0) or above highest (size) return -1 
        
            // std::cout<<"if we are here, we didn't throw away the event, n1 "<<n1<<", n2 "<<n2<<" and the cat number is  "<<((n1-1)*(boundaries_sigmaMoM.size()-1) + (n2-1))<<std::endl;
        return (n1-1)*((int)boundaries_sigmaMoM.size()-1) + (n2-1); // Does not pass, object will not be produced
        }
        else{
            return n1-1;
        }

    }

    void SigmaMpTTagProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
//   const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        std::unique_ptr<vector<SigmaMpTTag> > tags( new vector<SigmaMpTTag> );

        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            SigmaMpTTag tag_obj( dipho, mvares );
            tag_obj.setDiPhotonIndex( candIndex );

            tag_obj.setSystLabel( systLabel_ );

            int catnum = chooseCategory( mvares->decorrSigmarv, dipho->pt()/dipho->mass() );
            /// std::cout<<"cat num assigned is "<<catnum<<std::endl;
            tag_obj.setCategoryNumber( catnum );

            tag_obj.includeWeights( *dipho );

            bool passScaledPtCuts = 1;
            if ( requireScaledPtCuts_ ) {

                float pt_over_mgg_1 = (dipho->leadingPhoton()->pt() / dipho->mass());
                float pt_over_mgg_2 = (dipho->subLeadingPhoton()->pt() / dipho->mass());

                passScaledPtCuts = ( pt_over_mgg_1 > (1./3) && pt_over_mgg_2 > (1./4) );
                //                std::cout << " pt_over_mgg_1=" << pt_over_mgg_1 << " pt_over_mgg_2=" << pt_over_mgg_2 << " passScaledPtCuts=" << passScaledPtCuts << std::endl;
            }

            if( passScaledPtCuts && tag_obj.categoryNumber() >= 0 ) {
                tags->push_back( tag_obj );
            }
        }
        evt.put( std::move( tags ) );
    }
}

typedef flashgg::SigmaMpTTagProducer FlashggSigmaMpTTagProducer;
DEFINE_FWK_MODULE( FlashggSigmaMpTTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
