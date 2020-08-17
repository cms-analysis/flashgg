#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "flashgg/DataFormats/interface/DiMuonCandidate.h"

//-----------J. Tao from IHEP-Beijing--------------

using namespace edm;
using namespace std;

namespace flashgg {

    class DiMuonProducer : public EDProducer
    {

    public:
        DiMuonProducer( const ParameterSet & );
    private:
        void produce( Event &, const EventSetup & ) override;
        EDGetTokenT<View<pat::Muon> > muonToken_;
        EDGetTokenT<View<reco::Vertex> > vertexToken_;

        double minMuPT_;
        double maxMuEta_;

    };

    DiMuonProducer::DiMuonProducer( const ParameterSet &iConfig ) :
        muonToken_( consumes<View<pat::Muon> >( iConfig.getParameter<InputTag> ( "MuonTag" ) ) ),
        vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) )
    {
        minMuPT_ = iConfig.getParameter<double>( "minMuonPT" );
        maxMuEta_ = iConfig.getParameter<double>( "maxMuonEta" );
        produces<vector<flashgg::DiMuonCandidate> >();
    }

    void DiMuonProducer::produce( Event &evt, const EventSetup & )
    {

        Handle<View<reco::Vertex> > primaryVertices;
        evt.getByToken( vertexToken_, primaryVertices );
        //const PtrVector<reco::Vertex>& pvPointers = primaryVertices->ptrVector();
        edm::Ptr<reco::Vertex> vtx = primaryVertices->ptrAt( 0 ); //pvPointers[0]; //selected vertex 0
        const reco::Vertex myrecovtx = reco::Vertex( *vtx );

        Handle<View<pat::Muon> > muons;
        evt.getByToken( muonToken_, muons );
        const std::vector<edm::Ptr<pat::Muon> > &muonPointers = muons->ptrs();


        unique_ptr<vector<flashgg::DiMuonCandidate> > diMuonColl( new vector<flashgg::DiMuonCandidate> );
        //    cout << "evt.id().event()= " << evt.id().event() << "\tevt.isRealData()= " << evt.isRealData() << "\tmuonPointers.size()= " << muonPointers.size() << "\tpvPointers.size()= " << pvPointers.size() << endl;

        for( unsigned int i = 0 ; i < muonPointers.size() ; i++ ) {
            Ptr<pat::Muon> muon1 = muonPointers[i];
            double pt1 = muon1->pt();
            double eta1 = muon1->eta();
            if( pt1 < minMuPT_ || fabs( eta1 ) > maxMuEta_ ) { continue; }
            for( unsigned int j = i + 1 ; j < muonPointers.size() ; j++ ) {
                Ptr<pat::Muon> muon2 = muonPointers[j];
                double pt2 = muon2->pt();
                double eta2 = muon2->eta();
                if( pt2 < minMuPT_ || fabs( eta2 ) > maxMuEta_ ) { continue; }

                Ptr<pat::Muon> LeadMuon = muonPointers[i];
                Ptr<pat::Muon> SubLeadMuon = muonPointers[j];
                if( pt2 > pt1 ) {
                    LeadMuon = muonPointers[j];
                    SubLeadMuon = muonPointers[i];
                }

                DiMuonCandidate dimu( LeadMuon, SubLeadMuon );
                int mu1_charge = muon1->charge();
                int mu2_charge = muon2->charge();

                bool IsOPCharge = false;
                if( mu1_charge * mu2_charge < 0 ) { IsOPCharge = true; }
                dimu.setIsOSDiMuPair( IsOPCharge );

                bool leadmuIsTight  = LeadMuon->isTightMuon( myrecovtx );
                bool subleadmuIsTight  = SubLeadMuon->isTightMuon( myrecovtx );
                bool bothTightMu = false;
                if( leadmuIsTight && subleadmuIsTight ) { bothTightMu = true; }
                dimu.setIfBothTightMu( bothTightMu );

                bool bothGlobalAndPF = false;
                if( LeadMuon->isPFMuon() && LeadMuon->isGlobalMuon() && SubLeadMuon->isPFMuon() && SubLeadMuon->isGlobalMuon() ) { bothGlobalAndPF = true; }
                dimu.setIfBothGlobalAndPF( bothGlobalAndPF );

                // store the dimuon into the collection
                diMuonColl->push_back( dimu );
            }
        }
        evt.put( std::move( diMuonColl ) );

    }
}

typedef flashgg::DiMuonProducer FlashggDiMuonProducer;
DEFINE_FWK_MODULE( FlashggDiMuonProducer );

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
