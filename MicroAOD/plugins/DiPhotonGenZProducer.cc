#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/DataFormats/interface/VertexCandidateMap.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

#include <map>

using namespace edm;
using namespace std;

namespace flashgg {

    class DiPhotonGenZProducer : public EDProducer
    {

    public:
        DiPhotonGenZProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<flashgg::DiPhotonCandidate> > diPhotonToken_;
        EDGetTokenT<View<reco::GenParticle> >      genPartToken_;
    };

    DiPhotonGenZProducer::DiPhotonGenZProducer( const ParameterSet &iConfig ) :
        diPhotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
        genPartToken_( consumes<View<reco::GenParticle> >( iConfig.getParameter<InputTag> ( "GenParticleTag" ) ) )
    {
        produces<vector<flashgg::DiPhotonCandidate> >();
    }

    void DiPhotonGenZProducer::produce( Event &evt, const EventSetup & )
    {
        Handle<View<flashgg::DiPhotonCandidate> > diphotons;
        evt.getByToken( diPhotonToken_, diphotons );

        math::XYZPoint higgsVtx;
        if( ! evt.isRealData() ) {
            Handle<View<reco::GenParticle> > genParticles;
            evt.getByToken( genPartToken_, genParticles );
            for( unsigned int genLoop = 0 ; genLoop < genParticles->size(); genLoop++ ) {
                int pdgid = genParticles->ptrAt( genLoop )->pdgId();
                if( pdgid == 25 || pdgid == 22 ) {
                    higgsVtx = genParticles->ptrAt( genLoop )->vertex();
                    break;
                }
            }
        }

        unique_ptr<vector<DiPhotonCandidate> > diPhotonColl( new vector<DiPhotonCandidate> );

        for( unsigned int i = 0 ; i < diphotons->size() ; i++ ) {

            DiPhotonCandidate dipho( diphotons->at(i) );
            dipho.setGenPV( higgsVtx );
            diPhotonColl->push_back( dipho );
        }

        evt.put( std::move( diPhotonColl ) );
    }
}

typedef flashgg::DiPhotonGenZProducer FlashggDiPhotonGenZProducer;
DEFINE_FWK_MODULE( FlashggDiPhotonGenZProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

