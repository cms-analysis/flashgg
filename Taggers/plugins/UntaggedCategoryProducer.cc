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
#include "flashgg/DataFormats/interface/DiPhotonUntaggedCategory.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class UntaggedCategoryProducer : public EDProducer
    {

    public:
        UntaggedCategoryProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;

        vector<double> boundaries;

    };

    UntaggedCategoryProducer::UntaggedCategoryProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getUntrackedParameter<InputTag> ( "DiPhotonTag", InputTag( "flashggDiPhotons" ) ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getUntrackedParameter<InputTag> ( "MVAResultTag", InputTag( "flashggDiPhotonMVA" ) ) ) )

    {
        vector<double> default_boundaries;
        default_boundaries.push_back( 0.07 );
        default_boundaries.push_back( 0.31 );
        default_boundaries.push_back( 0.62 );
        default_boundaries.push_back( 0.86 );
        default_boundaries.push_back( 0.98 );

        // getUntrackedParameter<vector<float> > has no library, so we use double transiently
        boundaries = iConfig.getUntrackedParameter<vector<double > >( "Boundaries", default_boundaries );

        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception

        produces<vector<DiPhotonUntaggedCategory> >();
    }

    int UntaggedCategoryProducer::chooseCategory( float mvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void UntaggedCategoryProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
//   const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();

        std::auto_ptr<vector<DiPhotonUntaggedCategory> > tags( new vector<DiPhotonUntaggedCategory> );

        assert( diPhotons->size() == mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception

        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            DiPhotonUntaggedCategory tag_obj( dipho, mvares );
            tag_obj.setDiPhotonIndex( candIndex );
            //		tag_obj.setDiPhoMVAResult(mvares);
            //		tag_obj.setSigmaMwvoM( (float) mvares->sigmawv);
            //		tag_obj.setSigmaMrvoM( mvares->sigmarv);
            //		tag_obj.setVtxProb(   mvares->vtxprob);
            //		tag_obj.setDiphoMva(  mvares->mvaValue());

            int catnum = chooseCategory( mvares->result );
            tag_obj.setCategoryNumber( catnum );

            // Leave in debugging statement temporarily while tag framework is being developed
            // std::cout << "[UNTAGGED] MVA is "<< mvares->result << " and category is " << tag_obj.categoryNumber() << std::endl;

            if( tag_obj.categoryNumber() >= 0 ) {
                tags->push_back( tag_obj );
            }
        }
        evt.put( tags );

    }
}

typedef flashgg::UntaggedCategoryProducer FlashggUntaggedCategoryProducer;
DEFINE_FWK_MODULE( FlashggUntaggedCategoryProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

