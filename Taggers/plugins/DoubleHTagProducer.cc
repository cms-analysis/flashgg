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
#include "flashgg/DataFormats/interface/DoubleHTag.h"
#include "flashgg/DataFormats/interface/TagTruthBase.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "flashgg/MicroAOD/interface/MVAComputer.h"


#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class DoubleHTagProducer : public EDProducer
    {

    public:
        typedef math::XYZPoint Point;

        DoubleHTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float mva, float mx );
        
        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        std::vector<edm::EDGetTokenT<edm::View<reco::Candidate> > > jetTokens_;
        double vetoConeSize_; 
        
        flashgg::MVAComputer<DoubleHTag> mvaComputer_;
        
        vector<double> mvaBoundaries_, mxBoundaries_;

    };

    DoubleHTagProducer::DoubleHTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        vetoConeSize_( iConfig.getParameter<double> ( "VetoConeSize" ) ),
        mvaComputer_( iConfig.getParameter<edm::ParameterSet>("MVAConfig") )
    {
        mvaBoundaries_ = iConfig.getParameter<vector<double > >( "MVABoundaries" );
        mxBoundaries_ = iConfig.getParameter<vector<double > >( "MXBoundaries" );

        auto jetTags = iConfig.getParameter<std::vector<edm::InputTag> > ( "JetTags" ); 
        for( auto & tag : jetTags ) { jetTokens_.push_back( consumes<edm::View<reco::Candidate> >( tag ) ); }
        
        /// assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception
        
        // SigmaMpTTag
        produces<vector<DoubleHTag> >();
    }

    int DoubleHTagProducer::chooseCategory( float mva, float mx )
    {
        //// // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        //// int n;
        //// for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
        ////     if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        //// }
        return -1; // Does not pass, object will not be produced
    }

    void DoubleHTagProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );

        //// Point higgsVtx;
        //// if( ! evt.isRealData() ) {
        ////     evt.getByToken( genParticleToken_, genParticles );
        ////     for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
        ////         int pdgid = genParticles->ptrAt( genLoop )->pdgId();
        ////         if( pdgid == 25 || pdgid == 22 ) {
        ////             higgsVtx = genParticles->ptrAt( genLoop )->vertex();
        ////             break;
        ////         }
        ////     }
        //// }

        /// edm::RefProd<vector<TagTruthBase> > rTagTruth = evt.getRefBeforePut<vector<TagTruthBase> >();
        std::unique_ptr<vector<DoubleHTag> > tags( new vector<DoubleHTag> );
        
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );

            size_t vtx = (size_t)dipho->vertexIndex();
            if( vtx >= jetTokens_.size() ) { vtx = 0; }
            
            edm::Handle<edm::View<reco::Candidate> > jets;
            evt.getByToken( jetTokens_[vtx], jets);
            
            std::vector<edm::Ptr<reco::Candidate> > cleaned_jets;
            for( size_t ijet=0; ijet < jets->size(); ++ijet ) {
                auto jet = jets->ptrAt(ijet);
                if( reco::deltaR( *jet, *(dipho->leadingPhoton()) ) > vetoConeSize_ && reco::deltaR( *jet, *(dipho->subLeadingPhoton()) ) > vetoConeSize_ ) {
                    cleaned_jets.push_back( jet );
                }
            }
            if( cleaned_jets.size() < 2 ) { continue; }
        
            DoubleHTag tag_obj( dipho, cleaned_jets[0], cleaned_jets[1] );
            // tag_obj.setDiPhotonIndex( candIndex );
            
            tag_obj.setMVA( mvaComputer_(tag_obj) );
            
            // tag_obj.setSystLabel( systLabel_ );

            int catnum = chooseCategory( tag_obj.MVA(), tag_obj.MX() );
            tag_obj.setCategoryNumber( catnum );
            
            tag_obj.includeWeights( *dipho );
            
            //// bool passScaledPtCuts = 1;
            //// if ( requireScaledPtCuts_ ) {
            //// 
            ////     float pt_over_mgg_1 = (dipho->leadingPhoton()->pt() / dipho->mass());
            ////     float pt_over_mgg_2 = (dipho->subLeadingPhoton()->pt() / dipho->mass());
            //// 
            ////     passScaledPtCuts = ( pt_over_mgg_1 > (1./3) && pt_over_mgg_2 > (1./4) );
            //// }

            tags->push_back( tag_obj );
        }

        evt.put( std::move( tags ) );
    }
}

typedef flashgg::DoubleHTagProducer FlashggDoubleHTagProducer;
DEFINE_FWK_MODULE( FlashggDoubleHTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
