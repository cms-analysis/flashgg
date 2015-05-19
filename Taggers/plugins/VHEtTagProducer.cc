#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"
//#include "flashgg/DataFormats/interface/VBFDiPhoDiJetMVAResult.h"
//#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "flashgg/DataFormats/interface/VHEtTag.h"

#include <vector>
#include <algorithm>

using namespace std;
using namespace edm;

namespace flashgg {

    class VHEtTagProducer : public EDProducer
    {

    public:
        VHEtTagProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        int chooseCategory( float );

        EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<DiPhotonMVAResult> > mvaResultToken_;
        EDGetTokenT<View<pat::MET> > METToken_;
        vector<double> boundaries;

    };

    VHEtTagProducer::VHEtTagProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getUntrackedParameter<InputTag> ( "DiPhotonTag", InputTag( "flashggDiPhotons" ) ) ) ),
        mvaResultToken_( consumes<View<flashgg::DiPhotonMVAResult> >( iConfig.getUntrackedParameter<InputTag> ( "MVAResultTag", InputTag( "flashggDiPhotonMVA" ) ) ) ),
        METToken_( consumes<View<pat::MET> >( iConfig.getUntrackedParameter<InputTag> ( "METTag", InputTag( "slimmedMETs" ) ) ) )
    {
        vector<double> default_boundaries;
        default_boundaries.push_back( 0.52 );
        default_boundaries.push_back( 0.85 );
        default_boundaries.push_back( 0.915 );
        default_boundaries.push_back( 1 ); // from here
        //https://github.com/h2gglobe/h2gglobe/blob/master/AnalysisScripts/massfac_mva_binned/massfactorizedmvaanalysis.dat#L32
        
        // getUntrackedParameter<vector<float> > has no library, so we use double transiently
        boundaries = iConfig.getUntrackedParameter<vector<double > >( "Boundaries", default_boundaries );
        
        assert( is_sorted( boundaries.begin(), boundaries.end() ) ); // we are counting on ascending order - update this to give an error message or exception
        
        produces<vector<VHEtTag> >();
    }

    int VHEtTagProducer::chooseCategory( float mvavalue )
    {
        // should return 0 if mva above all the numbers, 1 if below the first, ..., boundaries.size()-N if below the Nth, ...
        int n;
        for( n = 0 ; n < ( int )boundaries.size() ; n++ ) {
            if( ( double )mvavalue > boundaries[boundaries.size() - n - 1] ) { return n; }
        }
        return -1; // Does not pass, object will not be produced
    }

    void VHEtTagProducer::produce( Event &evt, const EventSetup & )
    {
        //        std::cout << "starting met tagger" << std::endl;
        Handle<View<flashgg::DiPhotonCandidate> > diPhotons;
        evt.getByToken( diPhotonToken_, diPhotons );
        //  const PtrVector<flashgg::DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken( mvaResultToken_, mvaResults );
        //  const PtrVector<flashgg::DiPhotonMVAResult>& mvaResultPointers = mvaResults->ptrVector();
        Handle<View<pat::MET> > METs;
        evt.getByToken( METToken_, METs );
        if(METs->size() !=1)
            std::cout << "WARNING number of MET is not equal to 1" << std::endl;
        Ptr<pat::MET> theMET = METs->ptrAt(0);
        
        std::auto_ptr<vector<VHEtTag> > tags( new vector<VHEtTag> );

        assert( diPhotons->size() ==
                mvaResults->size() ); // We are relying on corresponding sets - update this to give an error/exception
        
        for( unsigned int candIndex = 0; candIndex < diPhotons->size() ; candIndex++ ) {
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt( candIndex );
            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt( candIndex );
            int catnum = 1;
            
            //pass diphotonCiCSelection
            
            VHEtTag tag_obj( dipho, mvares );
            tag_obj.setDiPhotonIndex( candIndex );
            tag_obj.setCategoryNumber( catnum );

            //MetCorrections2012_Simple(leadp4,subleadp4);
            
            //if(diphoton.mass()>100&&diphoton.mass()<180)
            {
                //calculate met
            }
            //if(correctedMet>70)
            if(theMET->pt()>70)
            //if( tag_obj.categoryNumber() >= 0 ) 
            {
                //setdiphotonindex
                //setMET
                tags->push_back( tag_obj );
                std::cout << "in Met tagger" << std::endl;
            }
        }
        evt.put( tags );
    }
}

typedef flashgg::VHEtTagProducer FlashggVHEtTagProducer;
DEFINE_FWK_MODULE( FlashggVHEtTagProducer );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

