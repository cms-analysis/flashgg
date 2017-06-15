#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/DataFormats/interface/MuMuGammaCandidate.h"
#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

//-----------J. Tao from IHEP-Beijing--------------

using namespace edm;
using namespace std;

namespace flashgg {

    class MuMuGammaProducer : public EDProducer
    {

    public:
        MuMuGammaProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<flashgg::DiMuonCandidate> > dimuToken_;
        EDGetTokenT<View<flashgg::Photon> > photonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;

        double minPhotonPT_;
        //double maxPhotonEta_;

    };

    MuMuGammaProducer::MuMuGammaProducer( const ParameterSet &iConfig ) :
        dimuToken_( consumes<View<flashgg::DiMuonCandidate> >( iConfig.getParameter<InputTag> ( "DiMuonTag" ) ) ),
        photonToken_( consumes<View<flashgg::Photon> >( iConfig.getParameter<InputTag> ( "PhotonTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) )
    {
        minPhotonPT_ = iConfig.getParameter<double>( "minPhotonPT" );
        produces<vector<flashgg::MuMuGammaCandidate> >();
    }

    void MuMuGammaProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        //const PtrVector<reco::Vertex>& pvPointers = primaryVertices->ptrVector();
        const std::vector<edm::Ptr<reco::Vertex>> &pvPointers = primaryVertices->ptrs();
        edm::Ptr<reco::Vertex> pvx = pvPointers[0]; //selected vertex 0

        Handle<View<flashgg::DiMuonCandidate> > dimuons;
        evt.getByToken( dimuToken_, dimuons );
        //const PtrVector<flashgg::DiMuonCandidate>& dimuonPointers = dimuons->ptrVector();
        const std::vector<edm::Ptr<flashgg::DiMuonCandidate>> &dimuonPointers = dimuons->ptrs();

        Handle<View<flashgg::Photon> > photons;
        evt.getByToken( photonToken_, photons );
        //const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();
        const std::vector<edm::Ptr<flashgg::Photon> > &photonPointers = photons->ptrs();


        unique_ptr<vector<flashgg::MuMuGammaCandidate> > MuMuGammaColl( new vector<flashgg::MuMuGammaCandidate> );
        //    cout << "evt.id().event()= " << evt.id().event() << "\tevt.isRealData()= " << evt.isRealData() << "\tdimuonPointers.size()= " << dimuonPointers.size() << "\tpvPointers.size()= " << pvPointers.size() << endl;

        for( unsigned int i = 0 ; i < dimuonPointers.size() ; i++ ) {
            Ptr<flashgg::DiMuonCandidate> dimuon = dimuonPointers[i];
            //flashgg::DiMuonCandidate dimu = flashgg::DiMuonCandidate(*dimuon);
            for( unsigned int j = 0; j < photonPointers.size() ; j++ ) {
                Ptr<flashgg::Photon> photon = photonPointers[j];
                // A number of things need to be done once the vertex is chosen
                // recomputing photon 4-momenta accordingly
                flashgg::Photon photon_corr = PhotonIdUtils::pho4MomCorrection( photon, pvx );

                float PhotonSCEta = photon_corr.superCluster()->position().Eta();
                if( fabs( PhotonSCEta ) > 2.5 || ( fabs( PhotonSCEta ) > 1.4442 && fabs( PhotonSCEta ) < 1.566 ) ) { continue; }
                float PhotonET =  photon_corr.pt();
                if( PhotonET < minPhotonPT_ ) { continue; }

                //MuMuGammaCandidate mumugamma(dimu, photon_corr);
                MuMuGammaCandidate mumugamma( dimuon, photon_corr, pvx );
                mumugamma.setVertex( pvx );
                //====================
                bool passFSRZmmg2012 = 0;
                const pat::Muon *muon_lead = dimuon->leadingMuon();
                const pat::Muon *muon_sublead = dimuon->subleadingMuon();
                reco::MuonPFIsolation  leadmuIso04 = muon_lead->pfIsolationR04();
                reco::MuonPFIsolation  subleadmuIso04 = muon_sublead->pfIsolationR04();
                double DeltaR1 = reco::deltaR( photon_corr.eta(), photon_corr.phi(), muon_lead->eta(), muon_lead->phi() );
                double DeltaR2 = reco::deltaR( photon_corr.eta(), photon_corr.phi(), muon_sublead->eta(), muon_sublead->phi() );
                if( muon_lead->pt() > 10.5 && muon_sublead->pt() > 10.5 && ( muon_lead->pt() + muon_sublead->pt() ) > 30.
                        && dimuon->IsOSDiMuPair() && dimuon->IfBothTightMu() && dimuon->IfBothGlobalAndPF()
                        && leadmuIso04.sumChargedHadronPt / muon_lead->pt() < 0.2 && subleadmuIso04.sumChargedHadronPt / muon_sublead->pt() < 0.2
                        && PhotonET > 20.
                        && dimuon->mass() > 35. && mumugamma.mass() > 60. && mumugamma.mass() < 120. && ( dimuon->mass() + mumugamma.mass() ) < 180.
                        && min( DeltaR1, DeltaR2 ) < 0.8 && ( ( DeltaR1 > DeltaR2 && muon_lead->pt() > 21 ) || ( DeltaR2 > DeltaR1 && muon_sublead->pt() > 21. ) )
                  ) { passFSRZmmg2012 = 1; }
                mumugamma.setIs2012FSRZMMG( passFSRZmmg2012 );

                bool passHGammaStarGamma = 0;
                //if( muon_lead->pt()>23. && muon_sublead->pt()>4. &&   dimuon->pt()/mumugamma.mass()>0.3
                if( muon_lead->pt() > 23. && muon_sublead->pt() > 5. &&   dimuon->pt() / mumugamma.mass() > 0.3
                        && dimuon->IsOSDiMuPair() && dimuon->IfBothTightMu() && dimuon->IfBothGlobalAndPF()
                        && leadmuIso04.sumChargedHadronPt / muon_lead->pt() < 0.2 && subleadmuIso04.sumChargedHadronPt / muon_sublead->pt() < 0.2
                        && dimuon->mass() < 20.
                        && PhotonET > 25. && PhotonET / mumugamma.mass() > 0.3  && min( DeltaR1, DeltaR2 ) > 1.0
                        && mumugamma.mass() > 110. && mumugamma.mass() < 150.
                  ) { passHGammaStarGamma = 1; }
                mumugamma.setIsHGammaStarGamma( passHGammaStarGamma );

                bool passHZgamma = 0;
                if( muon_lead->pt() > 20. && muon_sublead->pt() > 10. &&   dimuon->pt() / mumugamma.mass() > 0.3
                        && dimuon->IsOSDiMuPair() && dimuon->IfBothTightMu() && dimuon->IfBothGlobalAndPF()
                        && leadmuIso04.sumChargedHadronPt / muon_lead->pt() < 0.2 && subleadmuIso04.sumChargedHadronPt / muon_sublead->pt() < 0.2
                        && dimuon->mass() > 70. && dimuon->mass() < 110.
                        && PhotonET > 20. && PhotonET / mumugamma.mass() > 0.2  && min( DeltaR1, DeltaR2 ) > 1.0
                        && mumugamma.mass() > 110. && mumugamma.mass() < 150. && ( dimuon->mass() + mumugamma.mass() ) > 185.
                  ) { passHZgamma = 1; }
                mumugamma.setIsHZgamma( passHZgamma );
                //=====================
                double PhotonTrkIsoHollow03 = photon_corr.trkSumPtHollowConeDR03();
                mumugamma.setPhotonTrkIsoHollow03( PhotonTrkIsoHollow03 );
                //===Near mu within ISO cone: first near then far==
                double PhotonTrkIsoHollow03_MuCorr = PhotonTrkIsoHollow03;
                double LeadMuTrackPT = muon_lead->pt(), SubLeadMuTrackPT = muon_sublead->pt();
                if( ! muon_lead->track().isNull() ) { LeadMuTrackPT = muon_lead->track()->pt(); }
                if( ! muon_sublead->track().isNull() ) { SubLeadMuTrackPT = muon_sublead->track()->pt(); }
                double DeltaR_PhotonNearMu = DeltaR1 < DeltaR2 ? DeltaR1 : DeltaR2;
                double DeltaR_PhotonFarMu = DeltaR2 > DeltaR1 ? DeltaR2 : DeltaR1;
                double MuNear_TrkPT =  DeltaR1 < DeltaR2 ? LeadMuTrackPT : SubLeadMuTrackPT;
                double MuFar_TrkPT =  DeltaR1 < DeltaR2 ? SubLeadMuTrackPT : LeadMuTrackPT;
                if( DeltaR_PhotonNearMu < 0.3 && PhotonTrkIsoHollow03 > 0.99 * MuNear_TrkPT ) { PhotonTrkIsoHollow03_MuCorr -= MuNear_TrkPT; }
                if( DeltaR_PhotonFarMu < 0.3 && PhotonTrkIsoHollow03 > 0.99 * MuFar_TrkPT ) { PhotonTrkIsoHollow03_MuCorr -= MuFar_TrkPT; }
                mumugamma.setPhotonTrkIsoHollow03MuCorr( PhotonTrkIsoHollow03_MuCorr );


                // store the dimuon into the collection
                MuMuGammaColl->push_back( mumugamma );
            }
        }
        evt.put( std::move( MuMuGammaColl ) );

    }
}

typedef flashgg::MuMuGammaProducer FlashggMuMuGammaProducer;
DEFINE_FWK_MODULE( FlashggMuMuGammaProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
