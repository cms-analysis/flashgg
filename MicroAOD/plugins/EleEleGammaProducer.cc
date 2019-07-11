#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/DataFormats/interface/EleEleGammaCandidate.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

using namespace edm;
using namespace std;

namespace flashgg {

    class EleEleGammaProducer : public EDProducer
    {

    public:
        EleEleGammaProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<flashgg::DiElectronCandidate> > dieleToken_;
        EDGetTokenT<View<flashgg::Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;

        double minPhotonPT_;
        //double maxPhotonEta_;

    };

    EleEleGammaProducer::EleEleGammaProducer( const ParameterSet &iConfig ) :
        dieleToken_( consumes<View<flashgg::DiElectronCandidate> >( iConfig.getParameter<InputTag> ( "DiElectronTag" ) ) ),
        photonToken_( consumes<View<flashgg::Photon> >( iConfig.getParameter<InputTag> ( "PhotonTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) )
    {
        minPhotonPT_ = iConfig.getParameter<double>( "minPhotonPT" );
        produces<vector<flashgg::EleEleGammaCandidate> >();
    }

    void EleEleGammaProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        const std::vector<edm::Ptr<reco::Vertex>> &pvPointers = primaryVertices->ptrs();
        edm::Ptr<reco::Vertex> pvx = pvPointers[0]; //selected vertex 0

        Handle<View<flashgg::DiElectronCandidate> > dielectrons;
        evt.getByToken( dieleToken_, dielectrons );
        const std::vector<edm::Ptr<flashgg::DiElectronCandidate>> &dielectronPointers = dielectrons->ptrs();

        Handle<View<flashgg::Photon> > photons;
        evt.getByToken( photonToken_, photons );
        const std::vector<edm::Ptr<flashgg::Photon> > &photonPointers = photons->ptrs();


        unique_ptr<vector<flashgg::EleEleGammaCandidate> > EleEleGammaColl( new vector<flashgg::EleEleGammaCandidate> );


        for( unsigned int i = 0 ; i < dielectronPointers.size() ; i++ ) {
            Ptr<flashgg::DiElectronCandidate> dielectron = dielectronPointers[i];
            for( unsigned int j = 0; j < photonPointers.size() ; j++ ) {
                Ptr<flashgg::Photon> photon = photonPointers[j];
                // recomputing photon 4-momenta wrt to chosen vertex
                flashgg::Photon photon_corr = PhotonIdUtils::pho4MomCorrection( photon, pvx );

                // check that photon doesn't overlap with electrons
                float dr1 = deltaR(dielectron->leadingElectron()->eta(), photon_corr.eta(), dielectron->leadingElectron()->phi(), photon_corr.phi());
                float dr2 = deltaR(dielectron->subleadingElectron()->eta(), photon_corr.eta(), dielectron->subleadingElectron()->phi(), photon_corr.phi());
                if (dr1 < 0.4 && dr2 < 0.4) continue;

                float PhotonSCEta = photon_corr.superCluster()->position().Eta();
                //if( fabs( PhotonSCEta ) > 2.5 || ( fabs( PhotonSCEta ) > 1.4442 && fabs( PhotonSCEta ) < 1.566 ) ) { continue; } // can't we make this selection after?
                float PhotonET =  photon_corr.pt();
                if( PhotonET < minPhotonPT_ ) { continue; }

                EleEleGammaCandidate eegamma( dielectron, photon_corr, pvx );
                eegamma.setVertex( pvx );

                // store the dielectron into the collection
                EleEleGammaColl->push_back( eegamma );
            }
        }
        evt.put( std::move( EleEleGammaColl ) );

    }
}

typedef flashgg::EleEleGammaProducer FlashggEleEleGammaProducer;
DEFINE_FWK_MODULE( FlashggEleEleGammaProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
