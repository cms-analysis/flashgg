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
#include "flashgg/DataFormats/interface/UntaggedTag.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class UntaggedTagProducer : public EDProducer
    {

    public:
        UntaggedTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        string systLabel_;
        bool requireScaledPtCuts_;

        vector<double> boundaries;

    };

    UntaggedTagProducer::UntaggedTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getParameter<InputTag> ( "MVAResultTag" ) ) ),
        systLabel_( iConfig.getParameter<string> ( "SystLabel" ) ),
        requireScaledPtCuts_   ( iConfig.getParameter<bool> ( "RequireScaledPtCuts" ) )
    {
        boundaries = iConfig.getParameter<vector<double > >( "Boundaries" );

        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception

        produces<vector<UntaggedTag> >();
    }

    int UntaggedTagProducer::chooseCategory( float mvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void UntaggedTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
//   const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        std::unique_ptr<vector<UntaggedTag> > tags( new vector<UntaggedTag> );

        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            UntaggedTag tag_obj( dipho, mvares );
            tag_obj.setDiPhotonIndex( candIndex );

            tag_obj.setSystLabel( systLabel_ );

            int catnum = chooseCategory( mvares->result );
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

typedef flashgg::UntaggedTagProducer FlashggUntaggedTagProducer;
DEFINE_FWK_MODULE( FlashggUntaggedTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
